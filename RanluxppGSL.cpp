// SPDX-License-Identifier: LGPL-2.1-or-later

#include "ranluxpp/mulmod.h"
#include "ranluxpp/ranlux_lcg.h"

#include <gsl/gsl_rng.h>

#include <assert.h>
#include <stdint.h>

static const uint64_t kA_2048[] = {
    0xed7faa90747aaad9, 0x4cec2c78af55c101, 0xe64dcb31c48228ec,
    0x6d8a15a13bee7cb0, 0x20b2ca60cb78c509, 0x256c3d3c662ea36c,
    0xff74e54107684ed2, 0x492edfcc0cc8e753, 0xb48c187cf5b22097,
};

static constexpr int kMaxPos = 9 * 64;
static constexpr int kBits = 48;

typedef struct {
  uint64_t state[9]; ///< RANLUX state of the generator
  unsigned carry;    ///< Carry bit of the RANLUX state
  int position;      ///< Current position in bits
} ranluxpp_state;

static void ranluxpp_set(void *vstate, unsigned long int s) {
  ranluxpp_state *state = (ranluxpp_state *)vstate;
  uint64_t lcg[9];
  lcg[0] = 1;
  for (int i = 1; i < 9; i++) {
    lcg[i] = 0;
  }

  uint64_t a_seed[9];
  // Skip 2 ** 96 states.
  powermod(kA_2048, a_seed, uint64_t(1) << 48);
  powermod(a_seed, a_seed, uint64_t(1) << 48);
  // Skip another s states.
  powermod(a_seed, a_seed, s);
  mulmod(a_seed, lcg);

  to_ranlux(lcg, state->state, state->carry);
  state->position = 0;
}

static void ranluxpp_advance(ranluxpp_state *state) {
  uint64_t lcg[9];
  to_lcg(state->state, state->carry, lcg);
  mulmod(kA_2048, lcg);
  to_ranlux(lcg, state->state, state->carry);
  state->position = 0;
}

static uint64_t ranluxpp_next(ranluxpp_state *state) {
  if (state->position + kBits > kMaxPos) {
    ranluxpp_advance(state);
  }

  int idx = state->position / 64;
  int offset = state->position % 64;
  int numBits = 64 - offset;

  uint64_t bits = state->state[idx] >> offset;
  if (numBits < kBits) {
    bits |= state->state[idx + 1] << numBits;
  }
  bits &= ((uint64_t(1) << kBits) - 1);

  state->position += kBits;
  assert(state->position <= kMaxPos && "position out of range!");

  return bits;
}

static unsigned long int ranluxpp_get(void *vstate) {
  return ranluxpp_next((ranluxpp_state *)vstate);
}

static double ranluxpp_get_double(void *vstate) {
  static constexpr double div = 1.0 / (uint64_t(1) << kBits);
  uint64_t bits = ranluxpp_next((ranluxpp_state *)vstate);
  return bits * div;
}

static const gsl_rng_type ranluxpp_type = {
    "RANLUX++",
    /*max=*/(uint64_t(1) << kBits) - 1,
    /*min=*/0,
    /*size=*/sizeof(ranluxpp_state),
    &ranluxpp_set,
    &ranluxpp_get,
    &ranluxpp_get_double,
};

// The only symbol exported from this translation unit.
const gsl_rng_type *gsl_rng_ranluxpp = &ranluxpp_type;
