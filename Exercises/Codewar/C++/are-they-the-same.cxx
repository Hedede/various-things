#include <algorithm>
#include <cmath>
using std::vector;
class Same {
    static bool is_present(vector<int>& a, int num)
    {
      for (auto it = a.begin(); it != a.end(); ++it) {
        if (*it * *it == num) {
          a.erase(it);
          return true;
        }
      }
      return false;
    }

    public :static bool comp(vector<int> a, vector<int> b)
    {
      if (a.empty() && b.empty())
        return true;
      if (a.empty() || b.empty())
        return false;
      for (int i : b) {
        // if (std::find(begin(a), end(a), std::sqrt(b)) == end(a))
        if (!is_present(a, i))
          return false;
      }
      return true;
    }
};


// ver.2
#include <algorithm>
using std::vector;
class Same {
    public :static bool comp(vector<int> a, vector<int> b)
    {
      std::transform(begin(a), end(a), begin(a), [] (int x) { return x*x; });
      std::sort(begin(a), end(a));
      std::sort(begin(b), end(b));
      return a == b;
    }
};
