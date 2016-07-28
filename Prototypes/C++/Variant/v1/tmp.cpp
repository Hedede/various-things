#include <cstddef>
#include <utility>
#include <memory>
#include <type_traits>
template<typename T>
constexpr T largestOf(T value)
{
	return value;
}

template<typename T, typename... Ts>
constexpr T largestOf(T value, Ts... values)
{
	return value > largestOf(values...) ?
	       value : largestOf(values...);
}

template<typename T>
constexpr size_t getType()
{
	return 0;
}

template<>
constexpr size_t getType<int>()
{
	return 1;
}

template<>
constexpr size_t getType<double>()
{
	return 2;
}

template <typename... Ts>
struct Helper;

template <typename T, typename... Ts>
struct Helper<T, Ts...> {
	static void destroy(void* data, size_t type)
	{
		if (getType<T>() == type)
			reinterpret_cast<T*>(data)->~T();
		else
			Helper<Ts...>::destroy(data, type);
	}
	static void copy(void const* from, void const* to, size_t type)
	{
		if (getType<T>() == type)
			new (to) T(*reinterpret_cast<T const*>(from));
		else
			Helper<Ts...>::copy(from, to, type);
	}
	static void move(void* from, void* to, size_t type)
	{
		if (getType<T>() == type)
			new (to) T(std::move(from));
		else
			Helper<Ts...>::move(from, to, type);
	}
};

template <>
struct Helper<> {
	static void destroy(void* data, size_t type) { }
	static void copy(void* from, void* to, size_t type) { }
	static void move(void* from, void* to, size_t type) { }
};

template <typename... Ts>
class Variant {
private:
	static size_t const size = largestOf(sizeof(Ts)...);
	static size_t const align = largestOf(alignof(Ts)...);

	typedef typename std::aligned_storage<size, align>::type data_t;

	size_t type;
	data_t data;

public:
	Variant()
		: type(0)
	{
	}

	Variant(Variant<Ts...> const& other)
		: type(other.type)
	{
		Helper<Ts...>::copy(other.type, &other.data, &data);
	}

	Variant(Variant<Ts...>& other)
		: type(other.type)
	{
		Helper<Ts...>::move(other.type, &other.data, &data);
	}

	Variant<Ts...>& operator=(Variant<Ts...> const& other)
	{
		Helper<Ts...>::copy(other.type, &other.data, &data);
		type = other.type;
	}

	~Variant()
	{
		Helper<Ts...>::destroy(&data, type);
	}

	template<typename T, typename... Args>
	void set(Args&&... args)
	{
		Helper<Ts...>::destroy(&data, type);
		new (&data) T(std::forward<Args>(args)...);
		type = getType<T>();	
	}

	template<typename T>
	bool get(T& target)
	{
		if (type == getType<T>()) {
			target = *reinterpret_cast<T*>(&data);
			return true;
		}
		return false;
	}
};


int main()
{
	Variant<int, double> var;
	var.set<int>(10);
	var.set<double>(20.0);

	int a123 = 11;
	var.get(a123);
	var.set<int>(10);
	var.get(a123);

	return a123;
}
