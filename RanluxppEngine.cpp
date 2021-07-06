// SPDX-License-Identifier: LGPL-2.1-or-later

/** \class RanluxppEngine
Implementation of the RANLUX++ generator

RANLUX++ is an LCG equivalent of RANLUX using 576 bit numbers.

Described in
A. Sibidanov, *A revision of the subtract-with-borrow random numbergenerators*,
*Computer Physics Communications*, 221(2017), 299-303,
preprint https://arxiv.org/pdf/1705.03123.pdf

The code is loosely based on the Assembly implementation by A. Sibidanov
available at https://github.com/sibidanov/ranluxpp/.
*/

#include "RanluxppEngine.h"

#include "mulmod.h"

#include <cassert>
#include <cstdint>

namespace {

static const uint64_t kA_2048[] = {
    0xed7faa90747aaad9, 0x4cec2c78af55c101, 0xe64dcb31c48228ec,
    0x6d8a15a13bee7cb0, 0x20b2ca60cb78c509, 0x256c3d3c662ea36c,
    0xff74e54107684ed2, 0x492edfcc0cc8e753, 0xb48c187cf5b22097,
};

static constexpr int kMaxPos = 9 * 64;
static constexpr int kBits = 52;

} // end anonymous namespace

RanluxppEngine::RanluxppEngine(uint64_t seed) { SetSeed(seed); }

void RanluxppEngine::SetSeed(uint64_t s) {
  fState[0] = 1;
  for (int i = 1; i < 9; i++) {
    fState[i] = 0;
  }

  uint64_t a_seed[9];
  // Skip 2 ** 96 states.
  powermod(kA_2048, a_seed, uint64_t(1) << 48);
  powermod(a_seed, a_seed, uint64_t(1) << 48);
  // Skip another s states.
  powermod(a_seed, a_seed, s);
  mulmod(a_seed, fState);

  fPosition = 0;
}

void RanluxppEngine::Advance() {
  mulmod(kA_2048, fState);
  fPosition = 0;
}

uint64_t RanluxppEngine::NextRandomBits() {
  if (fPosition + kBits > kMaxPos) {
    Advance();
  }

  int idx = fPosition / 64;
  int offset = fPosition % 64;
  int numBits = 64 - offset;

  uint64_t bits = fState[idx] >> offset;
  if (numBits < kBits) {
    bits |= fState[idx + 1] << numBits;
  }
  bits &= ((uint64_t(1) << kBits) - 1);

  fPosition += kBits;
  assert(fPosition <= kMaxPos && "position out of range!");

  return bits;
}

/// Skip `n` random numbers without generating them
void RanluxppEngine::Skip(uint64_t n) {
  int left = (kMaxPos - fPosition) / kBits;
  assert(left >= 0 && "position was out of range!");
  if (n < (uint64_t)left) {
    // Just skip the next few entries in the currently available bits.
    fPosition += n * kBits;
    assert(fPosition <= kMaxPos && "position out of range!");
    return;
  }

  n -= left;
  // Need to advance and possibly skip over blocks.
  int nPerState = kMaxPos / kBits;
  int skip = (n / nPerState);

  uint64_t a_skip[9];
  powermod(kA_2048, a_skip, skip + 1);
  mulmod(a_skip, fState);

  // Potentially skip numbers in the freshly generated block.
  int remaining = n - skip * nPerState;
  assert(remaining >= 0 && "should not end up at a negative position!");
  fPosition = remaining * kBits;
  assert(fPosition <= kMaxPos && "position out of range!");
}

double RanluxppEngine::Rndm() {
  // Get 52 bits of randomness.
  uint64_t bits = NextRandomBits();

  // Construct the double in [1, 2), using the random bits as mantissa.
  static constexpr uint64_t exp = 0x3ff0000000000000;
  union {
    double dRandom;
    uint64_t iRandom;
  };
  iRandom = exp | bits;

  // Shift to the right interval of [0, 1).
  return dRandom - 1;
}

uint64_t RanluxppEngine::IntRndm() { return NextRandomBits(); }
