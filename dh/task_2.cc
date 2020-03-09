#include "big_integer.h"

BigInteger p, g, a, b;

BigInteger Power(const BigInteger &x, const BigInteger &t) {
  if (t == BigInteger(0)) {
    return BigInteger(1);
  }
  auto div_res = t.Div(BigInteger(2));
  auto ans = Power(x, *div_res.div);
  ans = ans.Mul(ans);
  if (!(*div_res.mod == BigInteger(0))) {
    ans = ans.Mul(x);
  }
  return ans;
}

int main() {
  try {
    std::cin >> p >> g >> a >> b;
    std::cout << *Power(g, a).Div(p).mod << "\n";
    std::cout << *Power(g, b).Div(p).mod << "\n";
    std::cout << *Power(g, a.Mul(b)).Div(p).mod << "\n";
  } catch (std::exception &e) {
    std::cout << e.what() << std::endl;
    exit(0);
  }
  return 0;
}