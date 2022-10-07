#include <vector>

template<template<typename T> class C>
int z(C<int>)
{
}

int main()
{
	std::vector<int> v;
	z(v);
}
