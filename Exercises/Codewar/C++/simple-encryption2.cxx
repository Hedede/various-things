#include <algorithm>
class Kata
{
    // {'A','B','C'} instead of "ABC" — to avoid null-terminator
    // (makes other algorithms nicer-looking)
    static constexpr char region[] = {
      'A','B','C','D','E','F','G','H','I','J','K','L','M','N','O','P','Q','R','S','T','U','V','W','X','Y','Z',
      'a','b','c','d','e','f','g','h','i','j','k','l','m','n','o','p','q','r','s','t','u','v','w','x','y','z',
      '0','1','2','3','4','5','6','7','8','9','.',',',':',';','-','?','!',' ','\'','(',')','$','%','&','"',
    };

    static bool check_input(std::string const& str)
    {
      for (char c : str) {
        if (std::find(std::begin(region), std::end(region), c) == std::end(region))
          throw std::invalid_argument{"Invalid character in input string: " + str};
      }
      return !str.empty();
    }

    static void switch_case(std::string& text)
    {
      // switch case for every second char
      auto do_switch = [] (char c) -> char {
        if (std::isupper(c))
          return std::tolower(c);
        if (std::islower(c))
          return std::toupper(c);
        return c;
      };
      // can't use std::transform :(
      for (size_t i = 1; i < text.size(); i += 2)
        text[i] = do_switch(text[i]);
    }

    static void map_string(std::string& text)
    {
      // replace characters with indices to avoid using std::find at every step
      auto map_char = [] (char c) -> char {
        auto pos = std::find(std::begin(region), std::end(region), c);
        return char(pos - std::begin(region));
      };
      std::transform(begin(text), end(text), begin(text), map_char);
    }

    static void unmap_string(std::string& text)
    {
      // convert indices back to chars
      auto unmap_char = [] (char c) -> char {
        unsigned char pos = c;
        return region[pos];
      };
      std::transform(begin(text), end(text), begin(text), unmap_char);
    }

    static char diff_index(char c1, char c2)
    {
      // Calculate position of the new char from the difference of c1 and c2
      auto diff = c1 - c2;
      if (diff < 0)
        return sizeof(region) + diff;
      return diff;
    }
    
    static void diff_index(std::string& text)
    {
      // Replace chars by differences
      auto diff = [prev = text[0]] (char c) mutable {
        // Because I am transforming in-place, I need to remember original char
        auto cur = diff_index(prev, c);
        prev = c;
        return cur;
      };
      std::transform(begin(text)+1, end(text), begin(text)+1, diff); 
    }
     
    static void undiff_index(std::string& text)
    {
      // Get actual chars from indices
      auto diff = [prev = text[0]] (char c) mutable {
        prev = diff_index(prev, c);
        return prev;
      };
      std::transform(begin(text)+1, end(text), begin(text)+1, diff);
    }

    static void mirror(std::string& text)
    {
      // mirror first char
      char c = text[0];
      text[0] = sizeof(region)-1 - c;
    }

public:
    std::string encrypt(std::string text)
    {
        if (!check_input(text))
          return {};
        step1: switch_case(text);
        map_string(text);
        step2: diff_index(text);
        step3: mirror(text);
        unmap_string(text);
        return text;
    }

    std::string decrypt(std::string text)
    {
        if (!check_input(text))
          return {};
        map_string(text);
        step3: mirror(text);
        step2: undiff_index(text);
        unmap_string(text);
        step1: switch_case(text);
        return text;
    }
};

constexpr char Kata::region[];
