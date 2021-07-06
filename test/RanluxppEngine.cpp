// SPDX-License-Identifier: LGPL-2.1-or-later

#include <RanluxppEngine.h>

#include "gtest/gtest.h"

TEST(RanluxppEngine, compare) {
  RanluxppEngine rng(314159265);
  // Match the assembly implementation in skipping the first 11 numbers.
  rng.Skip(11);

  // Values extracted from the assembly implementation.
  EXPECT_EQ(rng.IntRndm(), 1357063655714534);
  EXPECT_DOUBLE_EQ(rng.Rndm(), 0.500504017418743);

  // Skip ahead in block.
  rng.Skip(2);
  EXPECT_EQ(rng.IntRndm(), 160414309741165);
  EXPECT_DOUBLE_EQ(rng.Rndm(), 0.9422050833832005);

  // Skip ahead to start of next block.
  rng.Skip(5);
  EXPECT_EQ(rng.IntRndm(), 911953872946889);
  EXPECT_DOUBLE_EQ(rng.Rndm(), 0.7498142796273863);

  // Skip ahead across blocks.
  rng.Skip(42);
  EXPECT_EQ(rng.IntRndm(), 4265826975858336);
  EXPECT_DOUBLE_EQ(rng.Rndm(), 0.472544363223621);
}
