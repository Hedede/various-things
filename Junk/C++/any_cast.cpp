namespace mystd {
template <typename T>
struct remove_cvref
{
    using type = T
};

template <typename T>
using remove_cvref_t = typename remove_cvref<T>::type;

class any;

template <typename T>
T any_cast(any& any)
{
	using U = remove_cvref_t<T>;
    any_cast<U>(&any);
}
}

int main()
{
    mystd::any a(4);
    mystd::any_cast<int&>(a) = 5;
}
