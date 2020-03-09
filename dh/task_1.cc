#include "big_integer.h"

BigInteger a, b;

int main() {
  try {
    std::cin >> a >> b;
    std::cout << a.Add(b) << "\n";
    std::cout << a.Sub(b) << "\n";
    std::cout << a.Mul(b) << "\n";
    auto res = a.Div(b);
    std::cout << *res.div << "\n";
    std::cout << *res.mod << "\n";
  } catch (std::exception &e) {
    std::cout << e.what() << std::endl;
    exit(0);
  }
  return 0;
}