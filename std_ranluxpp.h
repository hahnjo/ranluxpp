// SPDX-License-Identifier: LGPL-2.1-or-later

#ifndef std_ranluxpp_h
#define std_ranluxpp_h

#include <cstddef>
#include <cstdint>

class ranluxpp {

public:
  using result_type = uint64_t;

  static constexpr result_type default_seed = 314159265u;

  // Mimick interface of std::subtract_with_carry_engine
  static constexpr size_t word_size = 48;
  static constexpr size_t short_lag = 5;
  static constexpr size_t long_lag = 12;

  // Mimick the interface of std::discard_block_engine
  static constexpr size_t block_size = 2048;
  static constexpr size_t used_block = 12;

  static constexpr size_t max_pos = word_size * used_block;
  static_assert(max_pos <= 9 * 64,
                "maximum position must be smaller than size of state");

  ranluxpp() : ranluxpp(default_seed) {}
  ranluxpp(result_type __sd) { seed(__sd); }

  void seed(result_type __sd = default_seed);

  static constexpr result_type min() { return 0; }
  static constexpr result_type max() {
    return (result_type(1) << word_size) - 1;
  }

  void discard(unsigned long long __z);

  result_type operator()();

private:
  uint64_t fState[9]; ///< RANLUX state of the generator
  unsigned fCarry;    ///< Carry bit of the RANLUX state
  int fPosition = 0;  ///< Current position in bits
};

#endif // RanluxppEngine_h
