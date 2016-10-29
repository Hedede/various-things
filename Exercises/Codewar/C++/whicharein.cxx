class WhichAreIn
{

  public:
  static std::vector<std::string> inArray(std::vector<std::string> &array1, std::vector<std::string> &array2)
  {
    std::vector<std::string> results;
    for (auto const& s1 : array1) {
      for (auto const& s2 : array2) {
        if (s2.find(s1) != std::string::npos) {
          results.push_back(s1);
          break;
        }
      }
    }
    std::sort(begin(results), end(results));
    return results;
  }
};

