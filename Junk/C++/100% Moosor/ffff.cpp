
template <typename... Ts>
struct Helper;

template <typename F, typename T, typename... Ts>
struct Helper<F, T, Ts...> {
	static bool func()
	{
		return F<T>() ? true : return Helper<F, Ts...>::func();
	}
};

template <>
struct Helper<> {
	static bool func()
	{
		return false;
	}
};

