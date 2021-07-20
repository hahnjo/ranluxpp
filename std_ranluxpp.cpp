// SPDX-License-Identifier: LGPL-2.1-or-later

#include "std_ranluxpp.h"

#include "ranluxpp/mulmod.h"
#include "ranluxpp/ranlux_lcg.h"

#include <cassert>
#include <cstdint>

namespace {

static const uint64_t kA_2048[] = {
    0xed7faa90747aaad9, 0x4cec2c78af55c101, 0xe64dcb31c48228ec,
    0x6d8a15a13bee7cb0, 0x20b2ca60cb78c509, 0x256c3d3c662ea36c,
    0xff74e54107684ed2, 0x492edfcc0cc8e753, 0xb48c187cf5b22097,
};

} // end anonymous namespace

void ranluxpp::seed(result_type __sd) {
  uint64_t lcg[9];
  lcg[0] = 1;
  for (int i = 1; i < 9; i++) {
    lcg[i] = 0;
  }

  uint64_t a_seed[9];
  // Skip 2 ** 96 states.
  powermod(kA_2048, a_seed, uint64_t(1) << 48);
  powermod(a_seed, a_seed, uint64_t(1) << 48);
  // Skip another __sd states.
  powermod(a_seed, a_seed, __sd);
  mulmod(a_seed, lcg);

  to_ranlux(lcg, fState, fCarry);
  fPosition = 0;
}

void ranluxpp::discard(unsigned long long __z) {
  int left = (max_pos - fPosition) / word_size;
  assert(left >= 0 && "position was out of range!");
  if (__z < (uint64_t)left) {
    // Just skip the next few entries in the currently available bits.
    fPosition += __z * word_size;
    assert(fPosition <= max_pos && "position out of range!");
    return;
  }

  __z -= left;
  int skip = (__z / used_block);

  uint64_t a_skip[9];
  powermod(kA_2048, a_skip, skip + 1);

  uint64_t lcg[9];
  to_lcg(fState, fCarry, lcg);
  mulmod(a_skip, lcg);
  to_ranlux(lcg, fState, fCarry);

  // Potentially skip numbers in the freshly generated block.
  int remaining = __z - skip * used_block;
  assert(remaining >= 0 && "should not end up at a negative position!");
  fPosition = remaining * word_size;
  assert(fPosition <= kMaxPos && "position out of range!");
}

ranluxpp::result_type ranluxpp::operator()() {
  if (fPosition + word_size > max_pos) {
    // Advance the generator state.
    uint64_t lcg[9];
    to_lcg(fState, fCarry, lcg);
    mulmod(kA_2048, lcg);
    to_ranlux(lcg, fState, fCarry);
    fPosition = 0;
  }

  int idx = fPosition / 64;
  int offset = fPosition % 64;
  size_t numBits = 64 - offset;

  result_type bits = fState[idx] >> offset;
  if (numBits < word_size) {
    bits |= fState[idx + 1] << numBits;
  }
  bits &= max();

  fPosition += word_size;
  assert(fPosition <= max_pos && "position out of range!");

  return bits;
}
