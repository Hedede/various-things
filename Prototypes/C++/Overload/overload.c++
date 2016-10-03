#include <utility>

template<typename F, typename...Fs>
struct overloaded : overloaded<F>, overloaded<Fs...> {
	overloaded(F&& f, Fs&&... fs)
		: overloaded<F>{std::forward<F>(f)},
		  overloaded<Fs...>{std::forward<Fs>(fs)...}
	{}

	using overloaded<F>::operator();
	using overloaded<Fs...>::operator();
};

template<typename F>
struct overloaded<F> : F {
	overloaded(F&& f)
		: F{std::forward<F>(f)}
	{}
};

template<typename...Fs>
overloaded<Fs...> overload(Fs&&... fs)
{
	return {std::forward<Fs>(fs)...};
}

#include <iostream>
int main()
{
	std::cout << std::boolalpha;
	std::string str = "got boool";
	auto lambda = overload(
		[] (int x) { std::cout << "got int " << x << "\n"; },
		[&] (bool b) { std::cout << str << ' ' << b << '\n'; }
	);

	lambda(1);
	lambda(true);

	str = "got bole";

	lambda(false);
}

/* аттемптинг беттер экцпланацион */
// аз ай шовн, из лямбд можно наследовать
template<typename F>
struct func : F {
	using F::operator(); // is not necessary now, but will be later
};
// но есть одна проблема: лямбды можно только копировать
{
	auto lambda = [] { return 3; };
	func<decltype(lambda)> f; // error no matching constructor
}
// посему нужно сделать функ конструироваемым из лябмды
template<typename F>
struct func : F {
	func(F&& f)
		: F{std::forward<F>(f)} // обычное обращение к конструктору бейзкласса
	{}
	using F::operator();
};
// и чтобы не писать decltype(), сделаем функцию:
template<typename F>
func<F> make_func(F&& f)
{
	return {std::forward<F>(f)};
}
// теперь можно писать:
{
	auto lambda = func([] { return 3; });
}

// нот вери юзфул, бат летс мейк ит оверлоадабле
template<typename F1, typename F2>
struct func : F1, F2 {
	func(F1&& f1, F2&& f2)
		: F1{std::forward<F1>(f1)},
		  F2{std::forward<F2>(f2)}
	{}
	// а вот теперь нам нужен юзинг, иначе компилер не знает
	// к какому из бейзклассов обращаться, а с юзингом
	// они становятся частью общего набора оверлоадед функций
	using F1::operator();
	using F2::operator();
};

template<typename F1, typename F2>
func<F1,F2> make_func(F1&& f1, F2&& f2)
{
	return {std::forward<F1>(f1), std::forward<f2>(f2)};
}
// теперь можно делать простые оверлоады:
{
	auto lambda = make_func(
		[] (float f) { return int(f); }
		[] (int i)   { return i; }
	);
}
// но мы потеряли возможность (бесполезную) создавать функции из 1 лямбды,
// и что насчёт 3,4,5 и более?
// вступают вариадик темплаты
template<typename F, typename...Rest>
struct func : F, func<Rest...> {
	func(F&& f, Rest&&...rest)
		// всё еще обычные вызовы конструкторов бейзклассов
		: F{std::forward<F>(f)},
		  func<Rest...>{std::forward<Rest>(rest)...}
	{}

	// сейм
	using F::operator();
	using func<Rest...>::operator();
};
// этот класс рекурсивно деривится с меньшим и меньшим количеством
// параметров шаблона, пока не достигнет <>, но
// чтобы он не пытался сконструировать func<> с пустым паком <Rest...>,
// (<Ts...> — называется параметр пак, и он может быть пустым (<>))
// нужна специализация:

template<typename F1>
struct func<F1> : F1 { // [ F = F1, Rest... = <empty>]
	func(F1&& f)
		: F{std::forward<F1>(f)},
	{}

	using F1::operator();
};
// всо, класс готов

// за счет множественного наследования func получает все () лямбд из которых
// он наследовался, а также все захваченные ими переменные
// (хмм, ай вондер можно ли их соптимизировать, чтобы не дублировался одинаковый захват…)

// ах да
template<typename... Fs>
func<Fs...> make_func(Fs&&... fs)
{
	return {std::forward<Fs>(fs)...};
}


// ещё что то хотел сказать… про шаблоны, но забыл

// алсо, конечно хотелось бы
template<typename...Fs>
// possible
struct func : Fs... {
	func(Fs&&... fs)
		// not possible :(
		// WAIT, ya mooduck,
		: Fs(std::forward<Fs>(fs)...)...
	{}

	func(Fs&&... fs)
		// possible
		: Fs(std::forward<Fs>(fs))...
	{}

	// not possible :(
	using Fs...::operator();
};
