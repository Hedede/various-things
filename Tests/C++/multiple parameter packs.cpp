#include <type_traits>

template<bool... Bools>
constexpr bool bool_and = (Bools && ...);

template<bool... Bools>
constexpr bool bool_or  = (Bools || ...);

template <class T, typename... Ts>
constexpr bool is_in_pack = bool_or<std::is_same<T,Ts>::value...>;

template <typename... Os, typename... Ts>
constexpr bool all_in_pack = bool_and<is_in_pack<Os,Ts...>...>;


template <typename... Os>
struct all_in_pack2 {
    template <typename... Ts>
	static constexpr bool check = bool_and<is_in_pack<Os,Ts...>...>;
};




template <typename... Ts>
struct variant {
    variant() = default;

    template<typename... Os>
    variant(variant<Os...> const& other)
    {
        static_assert(all_in_pack<Os...,Ts...>, "Must be same or subset.");
        //static_assert(all_in_pack2<Os...>::template check<Ts...>, "Must be same or subset.");
    }
};

int main()
{
    {
        variant<int,float> v1;
        variant<int,float,double> v2(v1);
    }
    
    variant<int,float,double> v1;
    variant<int,float> v2(v1);
}
