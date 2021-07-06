// SPDX-License-Identifier: LGPL-2.1-or-later

#ifndef RANLUXPP_HELPERS_H
#define RANLUXPP_HELPERS_H

#include <cstdint>

/// Compute `a + b` and set `overflow` accordingly.
static inline uint64_t add_overflow(uint64_t a, uint64_t b,
                                    unsigned &overflow) {
  uint64_t add = a + b;
  overflow = (add < a);
  return add;
}

/// Compute `a + b` and increment `carry` if there was an overflow
static inline uint64_t add_carry(uint64_t a, uint64_t b, unsigned &carry) {
  unsigned overflow;
  uint64_t add = add_overflow(a, b, overflow);
  // Do NOT branch on overflow to avoid jumping code, just add 0 if there was
  // no overflow.
  carry += overflow;
  return add;
}

/// Compute `a - b` and set `overflow` accordingly
static inline uint64_t sub_overflow(uint64_t a, uint64_t b,
                                    unsigned &overflow) {
  uint64_t sub = a - b;
  overflow = (sub > a);
  return sub;
}

/// Compute `a - b` and increment `carry` if there was an overflow
static inline uint64_t sub_carry(uint64_t a, uint64_t b, unsigned &carry) {
  unsigned overflow;
  uint64_t sub = sub_overflow(a, b, overflow);
  // Do NOT branch on overflow to avoid jumping code, just add 0 if there was
  // no overflow.
  carry += overflow;
  return sub;
}

#endif
