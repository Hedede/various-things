#include <vector>
#include <iostream>
#include <algorithm>
#include <string>
#include <cmath>

std::string ld(double d)
{
    auto s = std::to_string(d) ;
    auto p = s.find('.');
    return s.substr(0,p);
}

std::string fd(double d, int m=0)
{
    auto s = std::to_string(d) ;
    auto p = s.find('.');
    s = s.substr(0,p);
    for (auto i = s.size()+m; ; )
    {
        if (i < 3)
            break;
        i -= 3;
        if (i == 0)
            break;
        if (i>s.size())
            continue;
        s.insert(i, 1, ' ');
        if (m!=0)
        std::cerr <<  m<<"ins"<<i<<'\n';
    }
    return s;
}

std::string suffix(long long power)
{
    power /= 3;
        
    switch (power)
    {
        case 1:
            return " thousand"; break;
        case 2:
            return  " million"; break;
        case 3:
            return  " billion"; break;
        case 4:
            return " trillion"; break;
        case 5:
            return  " quadrillion"; break;
        case 6:
            return  " quintillion"; break;
    }
    
    return "";
}

std::string human_double(double d, size_t precision = 3)
{
    long long power = std::floor(std::log10(d));
    if (power < 3)
        return std::to_string(std::lround(d));
    
    if (power > 18)
    {
        auto s = std::to_string(power);
        std::string s1 = std::to_string(std::lround(d/std::pow(10.0,power-precision)))+"×10";
    auto pos = 1;
    if (pos < precision)
        s1.insert(pos, 1, '.');
        for (auto c: s)
        {
            switch(c)
            {
                    case '1':s1+="¹";break;
                    case '2':s1+="²";break;
                    case '3':s1+="³";break;
                    case '4':s1+="⁴";break;
                    case '5':s1+="⁵";break;
                    case '6':s1+="⁶";break;
                    case '7':s1+="⁷";break;
                    case '8':s1+="⁸";break;
                    case '9':s1+="⁹";break;
                    case '0':s1+="⁰";break;
                    case '-':s1+="⁻";break;
            }
        }
        return s1;
    }

    
    long long dot = power - precision + 1;
    
        auto base = std::pow(10.0, dot);
        //std::cerr << fd(d) << '\n';
        d = d/base;
        //std::cerr << fd(d, power - precision + 1) << ' ' << ld(base) << '\n';
    
    
    std::string suff = suffix(power);
    
    std::string s = std::to_string(std::lround(d));

    auto pos = (dot + s.size() - 1) % 3 + 1;
    //while (pos > 3) pos -= 3;
    if (pos < precision)
        s.insert(pos, 1, '.');
    
    s += suff;
    
    return s;   
}

int m(int x)
{while (x > 3) x -= 3;
 return x;
}

int main()
{
    for (int i = 0; i < 15; ++i)
    std::cerr << m(i) << ':' << i << '\n';
    std::cerr << human_double(1) << '\n';
    std::cerr << human_double(111) << '\n';
    std::cerr << human_double(132'434) << '\n';
    std::cerr << human_double(3'594'949) << '\n';
    std::cerr << human_double(13'594'949) << '\n';
    std::cerr << human_double(13'594'534'949) << '\n';
    std::cerr << human_double(13'594'534'949'423'333) << '\n';
    std::cerr << human_double(13'594'534'949'423'333'111.) << '\n';
    std::cerr << human_double(138'594'534'949'423'333'111'111.) << '\n';
    std::cerr << human_double(138'594'534'949'423'333'111'111'000.) << '\n';
    std::cerr << human_double(138'594'534'949'423'333'111'111'000'000.) << '\n';
}
