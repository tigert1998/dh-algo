#ifndef DH_BIG_INTEGER_H_
#define DH_BIG_INTEGER_H_

#include <stdint.h>
#include <iostream>
#include <memory>
#include <vector>

class BigInteger;

struct DivResult {
  std::unique_ptr<BigInteger> div, mod;
};

class BigInteger {
  friend std::ostream &operator<<(std::ostream &, const BigInteger &);
  friend std::istream &operator>>(std::istream &, BigInteger &);

 private:
  std::vector<int32_t> mem_;
  bool is_positive_;

  void Debug() const;

  void Carry(bool);

  int CompareAbs(const BigInteger &) const;
  static void LowPrecDivAbs(const BigInteger &, const BigInteger &,
                            int32_t *div, BigInteger *mod);

  BigInteger &SubAbsWithSideEffect(const BigInteger &);
  BigInteger &AddAbsWithSideEffect(const BigInteger &);

 public:
  constexpr static uint32_t kMaxDigits = 4;
  constexpr static uint32_t kMax = 10000;

  BigInteger();
  explicit BigInteger(int);
  BigInteger(const BigInteger &);
  bool operator==(const BigInteger &) const;

  BigInteger Add(const BigInteger &) const;
  BigInteger Sub(const BigInteger &) const;
  BigInteger Mul(const BigInteger &) const;
  DivResult Div(const BigInteger &) const;
};

#endif