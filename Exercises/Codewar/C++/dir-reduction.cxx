#include <map>
class DirReduction
{
    static bool is_opposite(std::string const& a, std::string const& b)
    {
      static std::map<std::string,std::string> opposites{
        {"NORTH", "SOUTH"},
        {"SOUTH", "NORTH"},
        {"WEST", "EAST"},
        {"EAST", "WEST"},
      };
      return opposites[a] == b;
    }
    static bool doReduc(std::vector<std::string>& arr)
    {
      size_t size = arr.size();

      if (size < 2)
        return false;

      for (auto it = begin(arr); end(arr) - it > 1; ++it) {
        if (is_opposite(*it, *(it+1)))
          it = arr.erase(it, it+2);
      }

      return arr.size() < size;  
    }
public:
    static std::vector<std::string> dirReduc(std::vector<std::string> &arr)
    {
      while (doReduc(arr));
      return arr;
    }
};
