#include "big_integer.h"

BigInteger p, g, a, b;

BigInteger Power(const BigInteger &x, const BigInteger &t,
                 const BigInteger &mod) {
  if (t == BigInteger(0)) {
    return BigInteger(1);
  }
  auto div_res = t.Div(BigInteger(2));
  auto ans = Power(x, *div_res.div, mod);
  ans = *ans.Mul(ans).Div(mod).mod;
  if (!(*div_res.mod == BigInteger(0))) {
    ans = *ans.Mul(x).Div(mod).mod;
  }
  return ans;
}

int main() {
  try {
    std::cin >> p >> g >> a >> b;
    std::cout << Power(g, a, p) << "\n";
    std::cout << Power(g, b, p) << "\n";
    std::cout << Power(Power(g, a, p), b, p) << "\n";
  } catch (std::exception &e) {
    std::cout << e.what() << std::endl;
    exit(0);
  }
  return 0;
}