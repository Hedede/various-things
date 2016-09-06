#include <vector>
#include <algorithm>

// choose one of three values, based on relation between a and b
template<typename T, typename R>
R choose(T a, T b, R a_greater, R equal, R b_greater)
{
  if (a < b) return b_greater;
  if (b < a) return a_greater;
  return equal;
}

// join strings, with delimiter
template<typename Iterator, typename T>
T join(Iterator begin, Iterator end, T const& delim)
{
    T sink;
    if (begin == end)
        return sink;
    sink += *begin++;
    while (begin != end)
        sink += delim + *begin++;
    return sink;
}

class Mix
{
public:
  static std::string mix(const std::string &s1, const std::string &s2)
  {
    std::vector<std::string> result;
    for (char c = 'a'; c <= 'z'; ++c) {
      size_t count[2] {
        size_t(std::count(begin(s1), end(s1), c)),
        size_t(std::count(begin(s2), end(s2), c))
      };
      size_t max = std::max(count[0], count[1]);
      if (max > 1) {
        // construct string immediately (I used tuples and contructed string
        // in the end, but decided to simplify)
        std::string tmp;
        tmp.reserve(max + 3);
        tmp.append(1, choose(count[0], count[1], '1', '=', '2'));
        tmp.append(1, ':');
        tmp.append(max, c);
        result.push_back(std::move(tmp));
      }
    }

    // sort by size, result is already sorted alphabetically
    auto compare = [] (std::string const& a, std::string const& b) {
      if (a.size() > b.size())
        return true;
      return (a.size() > b.size()) || (a.size() == b.size() && a[0] < b[0]);
    };
    // stabel sort: don't want to ruin alphabetic order
    std::stable_sort(begin(result), end(result), compare);

    using namespace std::string_literals;
    return join(begin(result), end(result), "/"s);
  }
};

