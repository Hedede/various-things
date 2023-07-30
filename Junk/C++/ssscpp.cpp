
#include <string>
#include <string_view>
#include <cmath>
#include <iostream>
struct my_string :
    public std::string
{
    my_string( const std::string_view& str );
    ~my_string();

    template <typename T>
    my_string& arg( T );
};

template <typename T>
my_string& my_string::arg( T )
{
    return *this;
}

// my_string.cpp


my_string::my_string( const std::string_view& str ) :
std::string( str.data(), str.size() )
{
	    std::cout << "mestreng ctored\n";
}

my_string::~my_string()
{
	    std::cout << "mestreng dtored\n";
}

// my_string_view.h

#pragma once


struct my_string_view : public std::string_view
{
    my_string_view( const std::string_view::value_type* val ) :
        std::string_view( val ) {}

    template <typename... PARAMS>
    my_string arg( PARAMS&&... prms ) {
        return my_string( *this ).arg( std::forward<PARAMS>( prms )... );
    }
};



// main.cpp

#include <vector>

template <typename T>
struct basic_color
{
    T r, g, b, a;

    basic_color() : r( 0 ), g( 0 ), b( 0 ), a( 255 ) {}

    template <typename U>
    explicit basic_color( const basic_color<U>& c ) :
        r( static_cast<T>(c.r) ),
        g( static_cast<T>(c.g) ),
        b( static_cast<T>(c.b) ),
        a( static_cast<T>(c.a) )
    {}
};

using color = basic_color<std::uint8_t>;
using float_color = basic_color<float>;

[[gnu::noinline]]
bool change_float_color( float_color& color )
{
    color.r = 0.1f;
    return true;
}

int main()
{
    std::vector<float_color> colors = { {} };
    double sum = 0.0;
    for ( int i = 0; i < 1; ++i )
    {
        float_color fc;
        change_float_color( fc );
        color c( fc );

        std::vector<std::string> msgs;
        msgs.push_back( my_string_view( "" ).arg( c.r ) );
        msgs.push_back( my_string_view( "" ).arg( c.g ) );
        sum += fc.b - colors[i].b;
    }

    return static_cast<int>(sqrt( sum ));
}
