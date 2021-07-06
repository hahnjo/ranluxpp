// SPDX-License-Identifier: LGPL-2.1-or-later

#ifndef RanluxppEngine_h
#define RanluxppEngine_h

#include <cstdint>
#include <memory>

class RanluxppEngine final {

private:
  uint64_t fState[9]; ///< RANLUX state of the generator
  unsigned fCarry;    ///< Carry bit of the RANLUX state
  int fPosition = 0;  ///< Current position in bits

  /// Produce next block of random bits
  void Advance();
  /// Return the next random bits, generate a new block if necessary
  uint64_t NextRandomBits();

public:
  RanluxppEngine(uint64_t seed = 314159265);

  /// Generate a double-precision random number with 52 bits of randomness
  double Rndm();
  /// Generate a random integer value with 52 bits
  uint64_t IntRndm();

  /// Initialize and seed the state of the generator
  void SetSeed(uint64_t seed);
  /// Skip `n` random numbers without generating them
  void Skip(uint64_t n);
};

#endif // RanluxppEngine_h
