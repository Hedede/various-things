

#include <stddef.h>
#include <utility>
#include <limits>
#include <type_traits>
#include <algorithm>


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

//template <typename U, typename... Ts>
//constexpr size_t get_index = _impl::get_index<U, Ts...>::value;

template <template <typename> class Checker, typename... Ts>
constexpr size_t find_index = _impl::find_index<Checker, Ts...>();

template<typename T>
struct IsSame {
	template<typename U>
	struct Checker : std::is_same<T, U> {};
};

template <typename U, typename... Ts>
constexpr size_t get_index = _impl::find_index<IsSame<U>::template Checker, Ts...>();

#include <string.h>
#include <iostream>
#include <experimental/optional>


struct variant_shared {
protected:
	struct Destroy {
		using return_type = void;

		template<typename T>
			void operator()(T& data)
			{
				data.~T();
			}
	};
};

template <typename... Ts>
struct variant : variant_shared {
	static constexpr size_t size  = std::max({sizeof(Ts)...});
	static constexpr size_t align = std::max({alignof(Ts)...});

	/*!
	 * Constructs empty variant.
	 */
	variant() = default;

	/*!
	 * Construct variant holding a value of type T.
	 *
	 * \note
	 * I decided to not allow constructing variant from types,
	 * which are not included in type list, but can be converted
	 * to one of these types.
	 * (It requires either complicated template magic to fire static assert
	 *  when this is ambiguous, reshuffle template list like in Loki library,
	 *  or just live with unexpected types being constructed)
	 */
	template<typename T>
	variant(T const& value)
	{
		construct<T>(value);
	}

	/*!
	 * Copy variant from another
	 */
	variant(variant const& other)
	{
		if (other.empty()) {
			reset();
			return;
		}

		other.apply(Copy{*this});

	}

	/*!
	 * Move variant from another
	 */
	variant(variant&& other)
	{
		if (other.empty()) {
			reset();
			return;
		}

		other.apply(Move{*this});
		other.reset();
	}

	/*!
	 * Move variant from a subset variant type
	 */
	template<typename... Os>
	variant(variant<Os...> const& other)
	{
		static_assert(!is_same<variant<Is...>, variant<Os...>>,
		              "Non-template constructor should be used for variant of same type.");

		if (other.empty()) {
			reset();
			return;
		}

		other.apply(Copy{*this});

	}

	/*!
	 * Move variant from a subset variant type
	 */
	template<typename... Os>
	variant(variant<Os...>&& other)
	{
		static_assert(!is_same<variant<Is...>, variant<Os...>>,
		              "Non-template constructor should be used for variant of same type.");

		if (other.empty()) {
			reset();
			return;
		}

		other.apply(Move{*this});
		other.reset();
	}

	variant& operator=(variant const& other)
	{
		if (other.empty()) {
			reset();
			return *this;
		}

		other.apply(Copy{*this});
		return *this;
	}

	variant& operator=(variant&& other)
	{
		if (other.empty()) {
			reset();
			return *this;
		}

		other.apply(Move{*this});
		other.reset();
		return *this;
	}

	template<typename... Os>
	variant& operator=(variant<Os...> const& other)
	{
		if (other.empty()) {
			reset();
			return *this;
		}

		index = other.apply(GetIndex{});
		other.apply(Copy{*this});
		return *this;
	}

	template<typename... Os>
	variant& operator=(variant<Os...>&& other)
	{
		if (other.empty()) {
			reset();
			return *this;
		}

		other.apply(Move{*this});
		other.reset();
		return *this;
	}

	/*!
	 * Set variant to value v of type T.
	 * Uses assignments if variant already contains a value of type T,
	 * or destroys current value if it is of a different type.
	 */
	template<typename T>
	void set(T const& v)
	{
		if (check_type<T>()) {
			assign(v);
		} else {
			reset();
			construct<T>(v);
		}
	}

	/*!
	 * Sets variant only when types match, or variant is empty.
	 */
	template<typename T>
	bool try_set(T const& v)
	{
		if (empty()) {
			construct<T>(v);
			return true;
		}
		if (check_type<T>()) {
			assign(v);
			return true;
		}
		return false;
	}

	/*!
	 * Extract value from variant.
	 * \return
	 *     true if types match and \a target was modified, false otherwise.
	 */
	template<typename T>
	bool get(T& target) const
	{
		if (check_type<T>()) {
			target = *reinterpret_cast<T const*>(&storage);
			return true;
		}
		return false;
	}

	/*!
	 * Extract value from variant.
	 */
	template<typename T>
	std::experimental::optional<T> get() const
	{
		if (check_type<T>())
			return *reinterpret_cast<T const*>(&storage);

		return std::experimental::nullopt;
	}

	/*!
	 * Destroy current value and mark variant as empty.
	 */
	void reset()
	{
		if (empty())
			return;
		destroy();
		index = invalid;
	}

	/*!
	 * This type is used to avoid accidental comparisons of
	 * indices from different variant types.
	 */
	enum class index_t : size_t { };
	static constexpr index_t invalid = index_t(std::numeric_limits<size_t>::max());

	/*!
	 * Check if variant is empty.
	 */
	bool empty() const
	{
		return index == invalid;
	}

	/*!
	 * Check if variant contains type T.
	 */
	template<typename T>
	bool check_type() const
	{
		return index == index_t(get_index<T, Ts...>);
	}

	/*
	 * Get index of currently stored type.
	 */
	index_t type_index() const
	{
		return index;
	}

	/*!
	 * Get index of particular type
	 */
	template<typename T>
	static constexpr index_t index_of()
	{
		return index_t(get_index<T, Ts...>);
	}

	/*!
	 * Apply a functor to variant.
	 * Functor must have an * template overloaded operator(),
	 * with template parameter T, * and argument T& or T const&:
	 *
	 * `template<typename T> R operator()(T&, Args...);`
	 *
	 * \arg func
	 *    Instance of a functor.
	 * \arg args
	 *    Other arguments that should be passed to the functor.
	 * \return
	 *    Return value of the functor, if present.
	 */
	template<typename Functor, typename...Args>
	auto apply(Functor func, Args&&... args) -> typename Functor::return_type
	{
		return apply_impl(func, std::forward<Args>(args)...);
	}

	template<typename Functor, typename...Args>
	auto apply(Functor func, Args&&... args) const -> typename Functor::return_type
	{
		return apply_impl(func, std::forward<Args>(args)...);
	}

private:
	/*!
	 * Different instantiations of variant should have access to
	 * eachother's internals, as though they are of same class.
	 */
	template<typename... Os>
	friend class variant;

	// Helpers
	/*
	 * Constructs new object of type T and marks T as current type.
	 */
	template<typename T, typename... Args>
	void construct(Args&&... args)
	{
		index = index_t(get_index<T, Ts...>);
		new (&storage) T(std::forward<Args>(args)...);
	}

	/*
	 * Calls assignment operator on currently held object.
	 * Doesn't do typechecking, that should be done by public interface.
	 */
	template<typename T>
	void assign(T const& other)
	{
		*reinterpret_cast<T*>(&storage) = other;
	}

	/*
	 * Destroys currently held value by applying Destroy functor.
	 */
	void destroy()
	{
		apply(Destroy{});

		if (_DEBUG)
			memset(&storage, sizeof(storage), 0);
	}

	// Functors
	using variant_shared::Destroy;

	struct Copy {
		using return_type = void;

		Copy(variant& self)
			: self(self)
		{}

		template<typename T>
		void operator()(T const& value)
		{
			self.construct<T>(value);
		}

	private:
		variant& self;
	};

	struct Move {
		using return_type = void;

		Move(variant& self)
			: self(self)
		{}

		template<typename T>
		void operator()(T& value)
		{
			self.construct<T>(std::move(value));
		}

	private:
		variant& self;
	};

	// Functor dispatch
	template<typename Functor, typename T, typename... Args> static auto
	apply_functor(void* storage, Functor f, Args&&...args) -> typename Functor::return_type
	{
		return f(*reinterpret_cast<T*>(storage), std::forward<Args>(args)...);
	}

	template<typename Functor, typename T, typename... Args> static auto
	apply_functor(void const* storage, Functor f, Args&&...args) -> typename Functor::return_type
	{
		return f(*reinterpret_cast<T const*>(storage), std::forward<Args>(args)...);
	}

	/*
	 * TODO: check for degenerate cases: variant<> and variant<T>
	 */
	/*
	 * Uses dispatch table to select appropriate functor.
	 */
	template<typename Functor, typename...Args>
	auto apply_impl(Functor f, Args&&... args) -> typename Functor::return_type
	{
		using return_type = typename Functor::return_type;
		using func_type   = return_type(void* storage, Functor f, Args...);

		static func_type* table[sizeof...(Ts)] = {
			(apply_functor<Functor, Ts, Args...>)...
		};

		size_t index = size_t(this->index);
		return table[index](reinterpret_cast<void*>(&storage), f, std::forward<Args>(args)...);
	}

	template<typename Functor, typename...Args>
	auto apply_impl(Functor f, Args&&... args) const -> typename Functor::return_type
	{
		using return_type = typename Functor::return_type;
		using func_type   = return_type(void const* storage, Functor f, Args...);

		static func_type* table[sizeof...(Ts)] = {
			(apply_functor<Functor, Ts, Args...>)...
		};

		size_t index = size_t(this->index);
		return table[index](reinterpret_cast<void const*>(&storage), f, std::forward<Args>(args)...);
	}

	// Storage
	using Storage = typename std::aligned_storage<size, align>::type;

	index_t index = invalid;
	Storage storage;
};

template<typename T>
struct ISCONST {
	static constexpr bool value = !std::is_const<T>::value;
};

template<typename EC>
auto to_underlying(EC ec) -> typename std::underlying_type<EC>::type
{
	return typename std::underlying_type<EC>::type(ec);
}

template<typename Variant>
void print_variant(Variant const& v)
{
	switch(v.type_index()) {
	case Variant::template index_of<int>():
		std::cout << *v.template get<int>() << "\n";
		return;
	case Variant::template index_of<float>():
		std::cout << *v.template get<float>() << "\n";
		return;
	case Variant::template index_of<std::string>():
		std::cout << *v.template get<std::string>() << "\n";
		return;
	};
}

int main()
{
	{
		std::string str = "A string.";
		variant<int, float, std::string> var1(str);
		var1.set(100);
		var1.set(str);
		std::cout << str << "\n";
	}
	std::string str = "222";
	variant<int, float, std::string> var1(str);
	var1.set(100);
	var1.set(str);
	variant<float, int, std::string> var2;
	var2 = var1;

	std::cout << sizeof(var1) << "\n";
	std::cout << std::boolalpha << var1.check_type<int>() << " " << var1.check_type<std::string>() << "\n";

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

	var1.set(std::string("a--555"));

	variant<int, float, std::string> var3;
	var3 = std::move(var1);

	std::cout << to_underlying(var3.type_index()) << "\n";
	std::cout << *var3.get<std::string>() << "\n";
	std::cout << to_underlying(var1.type_index()) << "\n";

	print_variant(var3);
	print_variant(var3);
	var3.set(100);
	print_variant(var3);
	var3.set(1.0f);
	print_variant(var3);
}
