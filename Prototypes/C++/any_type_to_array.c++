/*
 * Copyright (C) 2016  Hedede <haddayn@gmail.com>
 *
 * License LGPLv3 or later:
 * GNU Lesser GPL version 3 <http://gnu.org/licenses/lgpl-3.0.html>
 * This is free software: you are free to change and redistribute it.
 * There is NO WARRANTY, to the extent permitted by law.
 */
#ifndef aw_glsl_types_h
#define aw_glsl_types_h
#include <aw/meta/void_t.h>
#include <aw/types/types.h>
#include <aw/utility/index_sequence.h>
#include <array>
namespace aw {
using std::declval;

template<typename T>
using decay = typename std::decay<T>::type;

template<typename L, size_t... Ns>
auto unpack(L&& list, index_sequence<Ns...>) -> std::array<decay<decltype(list[0])>, sizeof...(Ns)>
{
	return {list[Ns]...};
}

template<typename T, typename L, size_t... Ns>
auto convert(L&& list, index_sequence<Ns...>) -> std::array<T, sizeof...(Ns)>
{
	return {T(list[Ns])...};
}

template<size_t N, typename T>
struct vec {
	std::array<T,N> v;

	constexpr vec(vec const&) = default;
	template<typename V, typename = void_t<decltype( declval<V>()[0] )>>
	constexpr vec(V&& v) : v{ convert<T>(std::forward<V>(v), make_index_sequence<N>()) } {}
	template<typename... U>
	constexpr vec(U...u) : v{T(u)...} {}
	constexpr T operator[](size_t n) { return v[n]; }
};

using vec2 = vec<2, float>;
using vec3 = vec<3, float>;
using vec4 = vec<4, float>;

using ivec2 = vec<2, int>;
using ivec3 = vec<3, int>;
using ivec4 = vec<4, int>;

using bvec2 = vec<2, bool>;
using bvec3 = vec<3, bool>;
using bvec4 = vec<4, bool>;

using uvec2 = vec<2, unsigned>;
using uvec3 = vec<3, unsigned>;
using uvec4 = vec<4, unsigned>;

using dvec2 = vec<2, double>;
using dvec3 = vec<3, double>;
using dvec4 = vec<4, double>;

} // namespace aw
#endif//aw_glsl_types_h

#include <vector>
#include <map>
#include <deque>
#include <unordered_map>
#include <iostream>
#include <aw/math/vector3d.h>
#include <aw/math/vector4d.h>
#include <bitset>
int main()
{
	using namespace aw;
	using std::cout;

	vec2 v{0,0};
	float x[5] = {16, 8};
	v = x;                                                        cout<<v[0]<<' '<<v[1]<<'\n';
	v = {1,2};                                                    cout<<v[0]<<' '<<v[1]<<'\n';
	v = vec3{1.0, -1.0, 2.0};                                     cout<<v[0]<<' '<<v[1]<<'\n';
	v = math::vector2d<float>{1.0, 2.0};                          cout<<v[0]<<' '<<v[1]<<'\n';
	v = math::vector3d<float>{1.0, -1.0, 2.0};                    cout<<v[0]<<' '<<v[1]<<'\n';
	v = math::vector4d<float>{0,10,1,20};                         cout<<v[0]<<' '<<v[1]<<'\n';
	v = std::vector<float>{1,2};                                  cout<<v[0]<<' '<<v[1]<<'\n';
	v = std::array<float,10>{1.0, -1.0, 2.0};                     cout<<v[0]<<' '<<v[1]<<'\n';
	v = new float[20]{1000,2};                                    cout<<v[0]<<' '<<v[1]<<'\n';
	v = &x[0];                                                    cout<<v[0]<<' '<<v[1]<<'\n';
	v = std::deque<float>{1e+19,2};                               cout<<v[0]<<' '<<v[1]<<'\n';
	v = std::map<unsigned, float>{{0,1.0},{1, 10.0}};             cout<<v[0]<<' '<<v[1]<<'\n';
	v = std::unordered_map<unsigned,float>{{0,10},{1,20}};        cout<<v[0]<<' '<<v[1]<<'\n';
	v = std::string("\x10\x20");                                  cout<<v[0]<<' '<<v[1]<<'\n';
	v = std::wstring(L"\x100\x200");                              cout<<v[0]<<' '<<v[1]<<'\n';
	v = std::bitset<100>{"1010100101010101010101010"};            cout<<v[0]<<' '<<v[1]<<'\n';
}
