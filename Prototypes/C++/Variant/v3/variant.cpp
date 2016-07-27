

#include <stddef.h>
#include <utility>
#include <limits>
#include <type_traits>
#include <algorithm>

struct Destroy {
	using return_type = void;

	template<typename T>
	void operator()(T* data)
	{
		data->~T();
	}
};


struct Copy {
	using return_type = void;

	template<typename T>
	void operator()(T* to, T const& from)
	{
		new (to) T(from);
	}
};

struct Move {
	using return_type = void;

	template<typename T>
	void operator()(T* to, T&& from)
	{
		new (to) T(std::move(from));
	}
};

template <bool Const, typename... Ts>
struct type_switch {
	using Storage = typename std::conditional<Const, void*, void const*>::type;

	template<typename Functor, typename T, typename... Args> static
	auto
	apply_functor(Storage storage, Functor f, Args&&...args) -> typename Functor::return_type
	{
		using Type = typename std::conditional<Const, T*, T const*>::type;
		return f(reinterpret_cast<Type>(storage), std::forward<Args>(args)...);
	}

	template<typename Functor, typename... Args>
	auto
	operator()(size_t index, Storage storage, Functor f, Args&&... args) -> typename Functor::return_type
	{
		using return_type = typename Functor::return_type;
		using func_type   = return_type(Storage storage, Functor f, Args...);

		static func_type* table[sizeof...(Ts)] = {
			(apply_functor<Functor, Ts, Args...>)...
		};

		return table[index](storage, f, std::forward<Args>(args)...);
	}
};

namespace _impl {
template<template <typename> class Checker>
constexpr size_t find_index()
{
	return 0;
}

template<template <typename> class Checker, typename T, typename... Ts>
constexpr size_t find_index()
{
#if 0
	if (sizeof...(Ts) == 0)
		return 0;
#endif
	if (Checker<T>::value)
		return 0;
	return 1 + find_index<Checker, Ts...>();
}


template <typename Type, typename... Ts>
struct get_index;

template <typename Type, typename... Ts>
struct get_index<Type, Type, Ts...> : std::integral_constant<std::size_t, 0> {};

template <typename Type, typename Head, typename... Ts>
struct get_index<Type, Head, Ts...> : std::integral_constant<std::size_t, 1 + get_index<Type, Ts...>::value> {};
} // namespace _impl

template <typename U, typename... Ts>
constexpr size_t get_index = _impl::get_index<U, Ts...>::value;

template <template <typename> class Checker, typename... Ts>
constexpr size_t find_index = _impl::find_index<Checker, Ts...>();

#include <iostream>
#include <experimental/optional>

template <typename... Ts>
struct variant {
	static constexpr size_t size  = std::max({sizeof(Ts)...});
	static constexpr size_t align = std::max({alignof(Ts)...});
	static constexpr size_t invalid = std::numeric_limits<size_t>::max();

	using Storage = typename std::aligned_storage<size, align>::type;

	variant() = default;

	template<typename T>
	variant(T const& value)
	{
		set(value);
	}

	struct GetIndex {
		using return_type = size_t;

		template<typename T>
		size_t operator()(T const*)
		{
			return get_index<T,Ts...>;
		}
	};

	struct Copy {
		using return_type = void;

		Copy(variant& self)
			: self(self)
		{}

		template<typename T>
		void operator()(T const* value)
		{
			self.construct<T>(*value);
		}

	private:
		variant& self;
	};

	template<typename... Os>
	variant(variant<Os...> const& other)
	{
		index = other.template apply(GetIndex{});
		other.template apply(Copy{*this});

	}

	template<typename T>
	bool check() const
	{
		return index == get_index<T, Ts...>;
	}

	template<typename T>
	void set(T const& v)
	{
		reset();
		construct<T>(v);
		index = get_index<T, Ts...>;
	}

	template<typename T>
	bool try_set(T const& v)
	{
		if (check<T>()) {
			set(v);
			return true;
		}
		return false;
	}

	template<typename T>
	bool get(T& target) const
	{
		if (check<T>()) {
			target = *reinterpret_cast<T const*>(&storage);
			return true;
		}
		return false;
	}

	template<typename T>
	std::experimental::optional<T> get()
	{
		if (check<T>())
			return *reinterpret_cast<T const*>(&storage);

		return std::experimental::nullopt;
	}

	void reset()
	{
		if (index == invalid)
			return;
		destroy();
		index = invalid;
	}

	template<typename T, typename... Args>
	void construct(Args&&... args)
	{
		new (&storage) T(std::forward<Args>(args)...);
	}

	void destroy()
	{
		apply(Destroy{});
	}

	template<typename Functor, typename...Args>
	auto apply(Functor f, Args&&... args) -> typename Functor::return_type
	{
		type_switch<false, Ts...> tswitch{};
		return tswitch(index, (void*)&storage, f, std::forward<Args>(args)...);
	}

	template<typename Functor, typename...Args>
	auto apply(Functor f, Args&&... args) const -> typename Functor::return_type
	{
		type_switch<true, Ts...> tswitch{};
		return tswitch(index, (void*)&storage, f, std::forward<Args>(args)...);
	}

	Storage storage;
	size_t  index = invalid;
};

template<typename T>
struct ISCONST {
	static constexpr bool value = !std::is_const<T>::value;
};

int main()
{
	variant<int, float, std::string> var1(std::string("222"));
	variant<float, int, std::string> var2;
	var2 = var1;

	std::cout << std::boolalpha << var1.check<int>() << " " << var1.check<std::string>() << "\n";

	std::string s;
	var1.get(s);
	std::cout << s << "\n";
	var2.get(s);
	std::cout << s << "\n";

	auto i = var1.get<int>();
	if (i)
		std::cout << "int\n";
	else
		std::cout << "not int\n";

	auto size = find_index<ISCONST, const int, const float, const int>;
	std::cout << size << "\n";
}
