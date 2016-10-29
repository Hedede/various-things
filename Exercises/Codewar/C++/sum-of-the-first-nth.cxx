#include <string>
#include <vector>
#include <algorithm>
#include <utility>
#include <numeric>
std::string seriesSum(int n)
{
  std::vector<int> denom;
  denom.resize(n);

  std::generate(begin(denom), end(denom), [i = 1] () mutable { return std::exchange(i, i+3); });
  double result = std::accumulate(begin(denom), end(denom), 0.0, [] (double d, int i) { return d + 1.0/double(i); });
  char buf[5];
  std::snprintf(buf, sizeof(buf), "%f.2", result);
  return {buf, sizeof(buf)};
}

#include <iostream>
int main()
{
  for (int i = 0; i < 25; ++i)
    std::cout << seriesSum(i) << '\n';
}
