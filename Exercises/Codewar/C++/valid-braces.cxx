bool valid_braces(std::string braces) 
{
  std::vector<char> stack;
  for (char tok : braces) {
    switch(tok) {
    case '(':
      stack.push_back(')');
      break;
    case '[':
      stack.push_back(']');
      break;
    case '{':
      stack.push_back('}');
      break;
    case '}': case ']': case ')':
      if (stack.empty() || stack.back() != tok)
        return false;
      stack.pop_back();
    }
  }
  return stack.empty();
}
