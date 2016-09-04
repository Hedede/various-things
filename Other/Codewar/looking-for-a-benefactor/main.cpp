#include <vector>
#include <numeric>
#include <cmath>

class NewAverage
{
public:
   static long long newAvg(std::vector<double> &arr, double navg)
   {
     double sum = std::accumulate(begin(arr), end(arr), 0.0);
     long long res = std::ceil(navg * (arr.size() + 1) - sum);
     if (res < 0)
       throw std::invalid_argument{"New average is too small"};
     return res;
   }
};

