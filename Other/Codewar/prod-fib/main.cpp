#include <vector>
typedef unsigned long long ull;
class ProdFib
{
public:
  static std::vector<ull> productFib(ull prod)
  {
    constexpr ull n = 0;
    ull F[2] { 1, 1 };
    while ( F[n]*F[n+1] < prod ) {
      ull next = F[n] + F[n+1];
      F[n]   = F[n+1];
      F[n+1] = next;
    };
    bool found = F[n]*F[n+1] == prod;
    return { F[n], F[n+1], found };
  }
};


class SumFct
{
  static ull sum_fib(int i)
  {
    constexpr ull n = 0;
    ull sum = 1;
    ull F[2] { 1, 1 };
    while ( i-- > 0 ) {
      F[n+1] = F[n+1] + F[n];
      F[n]   = F[n+1] - F[n];
      sum += F[n];
    };
    return sum;
  }
  public:
  static ull perimeter(int n)
  {
    return 4 * sum_fib(n);
  }
};

