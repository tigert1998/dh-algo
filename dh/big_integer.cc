#include "dh/big_integer.h"

#include <algorithm>
#include <cassert>
#include <iomanip>
#include <sstream>

std::istream &operator>>(std::istream &is, BigInteger &o) {
  o.is_positive_ = true;

  bool started = false;
  char c;
  do {
    is.get(c);
    if (!started) {
      if (isspace(c)) {
        continue;
      } else {
        started = true;
      }
    }
    if (c == '-') {
      o.is_positive_ ^= 1;
    } else if (c == '+' || isspace(c)) {
      continue;
    } else {
      break;
    }
  } while (1);

  std::stringstream ss;
  while ('0' <= c && c <= '9') {
    ss << c;
    is.get(c);
  }

  std::string num = ss.str();
  if (!isspace(c) || num.size() == 0) {
    throw std::invalid_argument("invalid input stream");
  }

  o.mem_.resize((num.size() + BigInteger::kMaxDigits - 1) /
                BigInteger::kMaxDigits);

  for (int i = 0; i < o.mem_.size(); i++) {
    int from = num.size() - (i + 1) * 4;
    uint32_t ans = 0;
    for (int j = std::max(0, from); j < from + 4; j++) {
      ans = ans * 10 + num[j] - '0';
    }
    o.mem_[i] = ans;
  }

  return is;
}

std::ostream &operator<<(std::ostream &os, const BigInteger &o) {
  if (!o.is_positive_) {
    os << "-";
  }

  for (int i = o.mem_.size() - 1; i >= 0; i--) {
    if (i < o.mem_.size() - 1) {
      os << std::setfill('0') << std::setw(BigInteger::kMaxDigits);
    }
    os << o.mem_[i];
  }
  return os;
}

int BigInteger::CompareAbs(const BigInteger &o) const {
  if (mem_.size() < o.mem_.size()) {
    return -1;
  } else if (mem_.size() > o.mem_.size()) {
    return 1;
  } else {
    for (int i = mem_.size() - 1; i >= 0; i--) {
      if (mem_[i] < o.mem_[i]) {
        return -1;
      } else if (mem_[i] > o.mem_[i]) {
        return 1;
      }
    }
  }
  return 0;
}

BigInteger::BigInteger() : BigInteger(0) {}

void BigInteger::Carry() {
  for (int i = 0; i < mem_.size() - 1; i++) {
    if (mem_[i] < 0) {
      mem_[i] += kMax;
      mem_[i + 1]--;
    }
  }

  int len = mem_.size();
  while (mem_[len - 1] == 0) len--;
  len = std::max(len, 1);
  mem_.resize(len);

  int i = 0;
  while (i < mem_.size()) {
    if (mem_[i] >= kMax) {
      if (i == mem_.size() - 1) {
        mem_.push_back(0);
      }
      mem_[i + 1] += mem_[i] / kMax;
      mem_[i] %= kMax;
    }
    i++;
  }
}

BigInteger::BigInteger(int value) {
  is_positive_ = value >= 0;
  mem_.resize(1);
  mem_[0] = std::abs(value);
  Carry();
}

BigInteger::BigInteger(const BigInteger &) = default;

void BigInteger::Debug() const {
  for (int i = 0; i < mem_.size(); i++) {
    std::cout << "mem_[" << i << "] = " << mem_[i] << "\n";
  }
}

BigInteger &BigInteger::AddAbsWithSideEffect(const BigInteger &o) {
  int len = std::max(mem_.size(), o.mem_.size());

  mem_.resize(len);

  for (int i = 0; i < o.mem_.size(); i++) {
    mem_[i] += o.mem_[i];
  }

  Carry();

  return *this;
}

BigInteger &BigInteger::SubAbsWithSideEffect(const BigInteger &o) {
  for (int i = 0; i < o.mem_.size(); i++) {
    mem_[i] -= o.mem_[i];
  }

  Carry();

  return *this;
}

BigInteger BigInteger::Add(const BigInteger &o) const {
  if (is_positive_ != o.is_positive_) {
    int ret = CompareAbs(o);
    if (ret < 0) {
      BigInteger ans(o);
      ans.SubAbsWithSideEffect(*this);
      ans.is_positive_ = o.is_positive_;
      return ans;
    } else if (ret > 0) {
      BigInteger ans(*this);
      ans.SubAbsWithSideEffect(o);
      ans.is_positive_ = is_positive_;
      return ans;
    } else {
      return BigInteger(0);
    }
  }

  BigInteger ans(*this);
  ans.AddAbsWithSideEffect(o);
  return ans;
}

BigInteger BigInteger::Sub(const BigInteger &o) const {
  if (is_positive_ != o.is_positive_) {
    BigInteger ans = *this;
    ans.AddAbsWithSideEffect(o);
    return ans;
  }

  int ret = CompareAbs(o);
  if (ret < 0) {
    BigInteger ans = o;
    ans.SubAbsWithSideEffect(*this);
    ans.is_positive_ = !is_positive_;
    return ans;
  } else if (ret > 0) {
    BigInteger ans = *this;
    ans.SubAbsWithSideEffect(o);
    ans.is_positive_ = is_positive_;
    return ans;
  } else {
    return BigInteger(0);
  }
}

BigInteger BigInteger::Mul(const BigInteger &o) const {
  BigInteger ans(0);
  ans.mem_.resize(mem_.size() + o.mem_.size() - 1);

  for (int i = 0; i < mem_.size(); i++) {
    for (int j = 0; j < o.mem_.size(); j++) {
      ans.mem_[i + j] += mem_[i] * o.mem_[j];
    }
  }

  ans.Carry();
  ans.is_positive_ = is_positive_ == o.is_positive_;
  return ans;
}

void BigInteger::LowPrecDivAbs(const BigInteger &a, const BigInteger &b,
                               int32_t *div, BigInteger *mod) {
  // a / b
  int l = 0, r = kMax - 1;
  while (l < r) {
    int mid = (l + r + 1) >> 1;
    int ret = a.CompareAbs(b.Mul(BigInteger(mid)));
    if (ret < 0) {
      r = mid - 1;
    } else if (ret > 0) {
      l = mid;
    } else {
      l = r = mid;
    }
  }
  *div = l;
  *mod = a;
  mod->SubAbsWithSideEffect(b.Mul(BigInteger(*div)));
  mod->is_positive_ = true;
}

DivResult BigInteger::Div(const BigInteger &o) const {
  BigInteger div, mod;

  mod = BigInteger(0);
  div.mem_.resize(mem_.size());
  for (int i = 0; i < mem_.size(); i++) {
    mod = mod.Mul(BigInteger(kMax)).Add(BigInteger(*(mem_.end() - i - 1)));
    LowPrecDivAbs(mod, o, &div.mem_[div.mem_.size() - i - 1], &mod);
  }
  div.Carry();

  if (is_positive_ != o.is_positive_) {
    div.is_positive_ = false;
    if (mod.CompareAbs(BigInteger(0)) != 0) {
      div = div.Sub(BigInteger(1));
    }
    mod = this->Sub(o.Mul(div));
  }

  return {std::make_unique<BigInteger>(std::move(div)),
          std::make_unique<BigInteger>(std::move(mod))};
}