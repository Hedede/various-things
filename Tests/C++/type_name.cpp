#include <string>

template<typename T>
constexpr const char* n()
{
    return __PRETTY_FUNCTION__;
}

struct static_string {
    const char* data;
    size_t size;
};

template<typename T>
struct type
{
    static static_string name()
    {
        return { n<T>() };
    }
};


template<typename T>
struct type<T*>
{
    static static_string name()
    {
        static std::string s = type<T>::name().data;
        s+='*';
        return { s.data(), s.size() };
    }
};

template<template <typename...> class T, typename... Args>
struct type<T<Args...>>
{
    using Ty = T<Args...>;
    static static_string name()
    {
        static std::string s = type<Ty>::name().data;
        s+='<';
        for (auto t : {type<Args>::name() ...})
        {
            s += t.data;
            s += ',';
        }
        s+='>';
        return { s.data(), s.size() };
    }
};

#include <iostream>
int main()
{
    std::cout << type<int>::name().data << '\n';
    std::cout << type<int*>::name().data << '\n';
    std::cout << type<std::string>::name().data << '\n';
}
