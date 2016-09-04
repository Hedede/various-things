class TwoToOne {
public:
    static std::string longest(const std::string &s1, const std::string &s2)
    {
      std::string tmp = s1 + s2;
      std::stable_sort(std::begin(tmp), std::end(tmp));
      auto new_end = std::unique(std::begin(tmp), std::end(tmp));
      tmp.erase(new_end, std::end(tmp));
      return tmp;
    }
};
