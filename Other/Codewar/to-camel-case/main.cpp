#include <string>
#include <vector>
#include <cctype>

// Split string separated by 'delim'
auto split(std::string const& source, std::string const& delim)
{
  std::vector<std::string> result;

  size_t pos = source.find_first_not_of(delim);
  size_t delim_pos = source.find_first_of(delim, pos);

  while (pos != std::string::npos) {
    result.push_back(source.substr(pos, delim_pos - pos));

    pos = source.find_first_not_of(delim, delim_pos);
    delim_pos = source.find_first_of(delim, pos);
  }

  return result;
}

std::string join(std::string& sink, std::vector<std::string> const& strs)
{
  for (auto const& s : strs)
    sink += s;
  return sink;
}

void correct_case(std::string& word, bool capitalize = true)
{
  auto begin = std::begin(word);
  if (capitalize)
    *begin++ = std::toupper(*begin);
  //std::transform(begin, std::end(word), begin, ::tolower);
}

std::string to_camel_case(std::string text) {
  if (text.empty())
    return {};
  auto words = split(text, "-_");
  auto result = words[0];
  words.erase(begin(words));

  correct_case(result, std::isupper(result[0]));
  for (auto& word : words)
    correct_case(word);

  return join(result, words);
}
