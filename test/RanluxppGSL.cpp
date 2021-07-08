// SPDX-License-Identifier: LGPL-2.1-or-later

// This test uses EXPECT_EQ also for floating point numbers - the expected
// values are entered with enough digits to ensure binary equality.

#include <gsl/gsl_rng.h>

extern const gsl_rng_type *gsl_rng_ranluxpp;

#include "gtest/gtest.h"

TEST(gsl_rng_ranluxpp, compare) {
  gsl_rng *r = gsl_rng_alloc(gsl_rng_ranluxpp);
  gsl_rng_set(r, 314159265);

  // The following values were obtained from RanluxppEngine without skipping.

  EXPECT_EQ(gsl_rng_get(r), 39378223178113);
  EXPECT_EQ(gsl_rng_uniform(r), 0.57072241146576274673);

  // Skip ahead in block.
  for (int i = 0; i < 8; i++) {
    gsl_rng_get(r);
  }
  EXPECT_EQ(gsl_rng_get(r), 52221857391813);
  EXPECT_EQ(gsl_rng_uniform(r), 0.16812543081078956675);

  // The next call needs to advance the state.
  EXPECT_EQ(gsl_rng_get(r), 185005245121693);
  EXPECT_EQ(gsl_rng_uniform(r), 0.28403302782895423206);

  // Skip ahead to start of next block.
  for (int i = 0; i < 10; i++) {
    gsl_rng_get(r);
  }
  EXPECT_EQ(gsl_rng_get(r), 89237874214503);
  EXPECT_EQ(gsl_rng_uniform(r), 0.79969842495805920635);

  // Skip ahead across blocks.
  for (int i = 0; i < 42; i++) {
    gsl_rng_get(r);
  }
  EXPECT_EQ(gsl_rng_get(r), 49145148745150);
  EXPECT_EQ(gsl_rng_uniform(r), 0.74670661284082484599);

  gsl_rng_free(r);
}
