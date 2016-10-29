int opposite(int number) 
{
  return -number;
}

std::string get_middle(std::string input) 
{
  if (input.empty())
    return "";
  size_t pos = input.size()/2;
  bool odd = input.size() & 1;
  return input.substr(pos - 1 + odd, 2 - odd);
}
