//class string and std namespace are already added for you
//Don't use inline function definition
class Pattern {
public:
    static string pattern(int n);
};

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

string Pattern::pattern(int n)
{
  std::vector<std::string> result;
  std::string str;
  while (n >= 1) {
    str += to_string(n--);
    result.push_back(str);
  }
  using namespace std::string_literals;
  return join(rbegin(result), rend(result), "\n"s);
}
