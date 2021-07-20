// SPDX-License-Identifier: LGPL-2.1-or-later

// This test uses EXPECT_EQ also for floating point numbers - the expected
// values are entered with enough digits to ensure binary equality.

#include <std_ranluxpp.h>

#include "gtest/gtest.h"

#include <random>

TEST(std_ranluxpp, compare) {
  ranluxpp rng(314159265);

  // The following values were obtained from RanluxppEngine without skipping.

  EXPECT_EQ(rng(), 39378223178113);

  // Skip ahead in block.
  rng.discard(9);
  EXPECT_EQ(rng(), 52221857391813);
  rng.discard(1);

  // The next call needs to advance the state.
  EXPECT_EQ(rng(), 185005245121693);

  // Skip ahead to start of next block.
  rng.discard(11);
  EXPECT_EQ(rng(), 89237874214503);

  // Skip ahead across blocks.
  rng.discard(43);
  EXPECT_EQ(rng(), 49145148745150);
}

TEST(std_ranluxpp, seed) {
  ranluxpp rng;
  // Seed with a non-default value.
  rng.seed(1);
  // Skip some numbers.
  rng.discard(42);

  // Seed with the default value.
  rng.seed();
  EXPECT_EQ(rng(), 39378223178113);
}

template <class Engine> void test_distributions() {
  Engine e;

  std::uniform_real_distribution<> real;
  real(e);

  std::normal_distribution<> normal;
  normal(e);
}

TEST(std_ranluxpp, distributions) {
  test_distributions<std::ranlux48>();
  test_distributions<ranluxpp>();
}
