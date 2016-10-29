class Tortoise
{
    static std::vector<int> to_hms(int s)
    {
      int h = s / 3600;
      s %= 3600;
      int m = s / 60;
      s %= 60;
      return {h, m, s};
    }
public:
    static std::vector<int> race(int v1, int v2, int g)
    {
      if (v1 >= v2)
        return {-1, -1, -1};
      int diff = v2 - v1;
      // convert feet per hour into feet per seconds — g / (diff/3600) 
      int time = (3600*g) / diff;
      return to_hms(time);
    }
};
