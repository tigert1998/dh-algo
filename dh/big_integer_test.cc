#include "gtest/gtest.h"

#include "dh/big_integer.h"

#include <stdint.h>

namespace {
TEST(BigIntegerTest, Random) {
  constexpr int64_t kMax = 10000;
  for (int i = 0; i < 100000; i++) {
    int64_t a = rand() % kMax - kMax / 2, b = rand() % kMax - kMax / 2;
    auto big_a = BigInteger(a), big_b = BigInteger(b);
    EXPECT_EQ(big_a.Add(big_b), BigInteger(a + b));
    EXPECT_EQ(big_a.Sub(big_b), BigInteger(a - b));
    EXPECT_EQ(big_a.Mul(big_b), BigInteger(a * b));

    a = rand() % kMax, b = rand() % kMax + 1;
    big_a = BigInteger(a), big_b = BigInteger(b);
    auto div_res = big_a.Div(big_b);
    EXPECT_EQ(*div_res.div, BigInteger(a / b));
    EXPECT_EQ(*div_res.mod, BigInteger(a % b));
  }
}
}  // namespace