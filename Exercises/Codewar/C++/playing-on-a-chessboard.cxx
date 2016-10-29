using namespace std;

class Suite2
{
    public:
    static string game(unsigned long long n)
    {
      // After a little bit of analysing the pattern,
      // I noticed that two numbers opposite of each other add up to 1,
      // except the ones along the main diagonal: if n is odd, there is 
      // one number which doesn't have pair.

      // After more thinking I realised that answer is simply (number of cells)/2
      unsigned long long num_cells = n*n;
      if (n%2 == 0)
        return "[" + to_string(num_cells/2) + "]";
      return "[" + to_string(num_cells) + ", " + to_string(2) + "]";
    }
};
