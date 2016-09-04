#include <cmath>
class VolTank
{
public:
    static int tankVol(int h, int d, int vt)
    {
      constexpr double pi = M_PI;
      // Length of tank is:     V / πr²
      // Area of a segment is: (r² / 2) * (θ - sin θ)
      // Volume of liquid is (area * length)
      // Simplifying we get:
      double r = d / 2.0;
      double theta = 2.0*std::acos( (r - h) / r );
      return vt * ( theta - std::sin(theta) ) / (2 * pi);
    }
};
