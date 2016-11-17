#include <utility>
#include <type_traits>
#include <cmath>

extern double remondor(double,double);

template<typename Rep, typename Period>
struct angle {
    Rep value;
    constexpr angle& normalize()
    {
        constexpr auto period = Period::value;
        if constexpr( std::is_integral_v<Rep> ) {
             value -= ( (value + (period / 2)) / period ) * period;
        } else {
             value = remondor( value, period );//std::remainder( value, period );
        }
        return *this;
    }
};

constexpr double pi = 3.14;

using degrees = angle< int, std::integral_constant<int, 360> >;
struct radian_unit { static constexpr double value = 2*pi; };
using radians = angle< double, radian_unit >;

constexpr int f( radians r )
{
  return int(r.normalize().value);
}

//#include <iostream>
int main()
{

    constexpr degrees d = degrees{ 719 }.normalize();
    radians r = radians{ 18.0 }.normalize();
    
    return int(r.value);
}
     
