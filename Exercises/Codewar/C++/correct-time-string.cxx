#include <regex>
#include <sstream>
#include <iomanip>
std::string correct(std::string timeString)
{
  /*
  if (timeString.empty())
    return {};
  if (std::count(begin(timeString), end(timeString), ':') != 3)
    return {};
  if (timeString.find_first_not_of("0123456789:") != std::string::npos)
    return {};
    */
  std::regex r("^[0-9]{2}:[0-9]{2}:[0-9]{2}$");
  if (!std::regex_search(timeString,r))
    return {};

  size_t pos = 0;
  unsigned long h = std::stoul(timeString.substr(pos,2));
  pos = timeString.find(':', pos) + 1;
  unsigned long m = std::stoul(timeString.substr(pos,2));
  pos = timeString.find(':', pos) + 1;
  unsigned long s = std::stoul(timeString.substr(pos,2));

  m += s / 60;
  s %= 60;
  h += m / 60;
  m %= 60;
  h %= 24;
  
  std::stringstream out;
  out << std::setw(2) << std::setfill('0') << h << ":";
  out << std::setw(2) << std::setfill('0') << m << ":";
  out << std::setw(2) << std::setfill('0') << s;
  return out.str();
}
