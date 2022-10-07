#include <vector>
template<typename T>
struct s {
	T v;
	bool null = false;
};

template<typename T>
struct s<std::vector<T>>
{
	std::vector<T> v;
	operator s<T>() const { return {v.back()}; }
	bool null = false;
};

template<typename... Ts>
struct a : s<Ts>...
{
};

int main()
{
		a<int, float, std::vector<double>> aa;
	
		s<double> v = (aa);

}
