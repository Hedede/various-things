#include <type_traits>
#include <cstdio>


// Helper class

template <typename T>
struct Printer
{
  static typename std::enable_if<std::is_floating_point<T>::value, int>::type
  print(T x, char * out, std::size_t n)
  {
    return std::snprintf(out, n, "%f", x);
  }
};

// Convenience function wrapper

template <typename T> int print(T x, char * out, std::size_t n)
{
  return Printer<T>::print(x, out, n);
}

void f()
{
  char a[10];

  Printer<double>::print(1.2, a, 10);  // use helper class
  print(1.4f, a, 10);                  // wrapper deduces type for you
}