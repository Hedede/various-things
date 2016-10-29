#include <algorithm>

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

std::string join(std::vector<std::string> const& strs, std::string const& delim)
{
  return join(begin(strs), end(strs), delim);
}

class Opstrings1
{
public:
    static std::string rot(const std::string &strng)
    {
      std::string copy = strng;
      std::reverse(begin(copy), end(copy));
      return copy;
    }
    static std::string selfieAndRot(const std::string &strng)
    {
      std::string slf  = strng;
      std::string copy = rot(strng);    
      std::string dots( slf.find('\n'), '.' );
      return join(split(slf,"\n"), dots+"\n") + dots+"\n"+dots + join(split(copy,"\n"), "\n"+dots);
    }

    using Func = std::string(const std::string &strng);
    static std::string oper(Func* transform, const std::string &s)
    {
      return transform(s);
    }
};
