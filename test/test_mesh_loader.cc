#include "gtest/gtest.h"

TEST(SanityCheck, Fail) {
  EXPECT_EQ(1, 0);
}
