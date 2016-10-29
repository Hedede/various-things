#include <numeric>
#include <algorithm>
#include <cctype>
size_t sum_string(std::string& s1)
{
    if (std::find_if_not(begin(s1), end(s1), ::isalpha) != end(s1))
      return 0;
    std::transform(begin(s1), end(s1), begin(s1), ::toupper);
    return std::accumulate(begin(s1), end(s1), size_t(0));
}

using std::string;
bool compare(string s1, string s2)
{
    //just do it!
    return sum_string(s1) == sum_string(s2);
}
