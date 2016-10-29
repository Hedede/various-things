// Iterator-based helper to join vector<string> into single string.
// looks ugly, but I can't think of better way
template<typename Iterator, typename T>
T join(Iterator begin, Iterator end, T const& delim)
{
  T sink;
  // If range is empty, return empty value
  if (begin == end)
    return sink;
  // Add first elements
  sink += *begin++;
  // Add rest of elements separated by delimiter
  while (begin != end)
    sink += delim + *begin++;
  return sink;
}

class PartList {
public:
  static std::vector<std::pair <std::string, std::string>>
  partlist(std::vector<std::string> &arr)
  {
    // Assuming that the input array has at lest two elements

    std::vector<std::pair <std::string, std::string>> result;

    // Algorithm is quite simple:
    // On each iteration array is split into two ranges: [begin, middle) and [middle, end)
    // then each range is joined into a string, and resulting pair is added to the output.
    // Loop continues until there is only one element between middle and end
    auto begin  = arr.begin();
    auto end    = arr.end();
    auto middle = begin + 1;
    auto last   = end - 1;

    do {
      using namespace std::string_literals;
      auto first  = join(begin, middle, " "s);
      auto second = join(middle, end, " "s);
      result.emplace_back( first, second );
    } while (middle++ != last);
    return result;
  }
};
