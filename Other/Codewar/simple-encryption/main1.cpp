std::string encrypt(std::string text, int n)
{
    if (n < 1)
      return text;
    for (size_t i = 0, e = text.size(); i < e; ++i, --e) {
      char c = text[i];
      text.erase(i,1);
      text.push_back(c);
    }
    return encrypt(text, n - 1);
}

#include <algorithm>
std::string decrypt(std::string text, int n)
{
    if (n < 1)
      return text;
    for (size_t i = text.size() / 2, f = 0; f < i; ++i, f+=2) {
      char c = text[i];
      text.erase(i,1);
      text.insert(begin(text)+f,c);
    }

    return decrypt(text, n-1);
}
