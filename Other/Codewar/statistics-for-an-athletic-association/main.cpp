#include <vector>
// Split string separated by 'delim'
auto split(std::string const& source, std::string const& delim)
{
  std::vector<std::string> result;
  size_t pos1 = 0;
  size_t pos2;

  do {
    pos2 = source.find(delim, pos1);
    result.push_back(source.substr(pos1, pos2 - pos1));
    if (pos2 == std::string::npos)
      break;

    pos1 = pos2 + delim.size();
  } while (true);

  return result;
}

#include <algorithm>
#include <numeric>
class Stat
{
  // seconds -> hh|mm|ss
  static std::string to_hms(unsigned s);
  // h|m|s -> seconds
  static unsigned to_s(std::string const& str);
public:
  static std::string stat(const std::string &strg)
  {
    if (strg.empty())
      return {};
    std::vector<std::string> results = split(strg, ", ");
    std::vector<unsigned> s;
    // convert h|m|s to seconds
    std::transform(begin(results), end(results), std::back_inserter(s), to_s);

    std::sort(begin(s), end(s));

    size_t n = s.size();
    unsigned range   = *std::max_element(begin(s), end(s)) - *std::min_element(begin(s), end(s));
    unsigned average = std::accumulate(begin(s), end(s), 0u) / s.size();
    unsigned median  = (n % 2) ? (s[n/2]) : ((s[n/2]+s[n/2-1]) / 2);

    return "Range: " + to_hms(range) + " Average: " + to_hms(average) + " Median: " + to_hms(median);
  }
};

unsigned Stat::to_s(std::string const& str)
{
  auto vals = split(str, "|");
  unsigned h = std::stoul(vals[0])*3600u;
  unsigned m = std::stoul(vals[1])*60u;
  unsigned s = std::stoul(vals[2]);
  return h + m + s;
}

#include <sstream>
#include <iomanip>
std::string Stat::to_hms(unsigned s)
{
  unsigned h = s / 3600u;
  s %= 3600u;
  unsigned m = s / 60u;
  s %= 60u;
  std::stringstream out;
  out << std::setw(2) << std::setfill('0') << h << "|";
  out << std::setw(2) << std::setfill('0') << m << "|";
  out << std::setw(2) << std::setfill('0') << s;
  return out.str();
}
