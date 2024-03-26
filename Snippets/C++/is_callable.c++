// partial substitute for std::is_callable
template<typename, typename R = void> struct is_callable;
template<typename Func, typename...Args, typename R>
struct is_callable<Func(Args...),R> : std::false_type {};

template<typename Func, typename...Args>
struct is_callable<
	Func(Args...),
	decltype( std::declval<Func>()(std::declval<Args>()...) )
> : std::true_type {};
