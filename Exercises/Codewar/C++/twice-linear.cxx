#include <set>
// brute-force solution
class DoubleLinear
{
    static std::set<int> u;
    static void generate(int n)
    {
      // Generates at most n*2 new numbers
      for (auto x : u) {
        u.insert(2 * x + 1);
        u.insert(3 * x + 1);
        if (n-- == 0)
          break;
      }
    }
public:
    static int dblLinear(int n)
    {
      if (u.size() < n*2)
        generate(n);
      auto it = begin(u);
      std::advance(it, n);
      return *it;
    }
};
std::set<int> DoubleLinear::u { 1 };


// more efficient solution
class DoubleLinear
{
    static std::set<int> closed;
    static std::set<int> open;

    // Generate sequence up to nth number
    static void generate(size_t n);

public:
    static int dblLinear(int n)
    {
      generate(n);
      // Retrieve nth number
      auto it = begin(closed);
      std::advance(it, n);
      return *it;
    }
};

// I keep two sets: open and closed
// Open set contains all the xs for which y and z were calculated
// And closed set contains numbers, which aren't processed yet
std::set<int> DoubleLinear::closed {  };
std::set<int> DoubleLinear::open{ 1 };
void DoubleLinear::generate(size_t n)
{
  // On each iteration, x is taken from the front of open set,
  // and y and z are inserted at their appropriate positions,
  // then x is added to the closed set.
  while (open.size() <= n) {
    int x = *open.begin();
    open.erase(x);
    open.insert(2*x + 1);
    open.insert(3*x + 1);
    closed.insert(x);
  }
}
