class Accumul {
  static std::string genString(char c, size_t count)
  {
    std::string res(count, std::tolower(c));
    res[0] = std::toupper(res[0]);
    return res;
  }
public:
  static std::string accum(const std::string &s)
  {
    std::string ret;
    size_t i = 1;
    for (char c : s)
      ret += "-" + genString(c, i++);
    ret.erase(begin(ret));
    return ret;
  }
};
