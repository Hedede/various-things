#include <type_traits>
#include <cstddef>
#include <utility>

#include <iostream>
struct dummy {
    template<typename D>
    operator D*() const;
};


struct di_container {
    template<typename T>
    T* resolve() const;
};

template<size_t N> struct rank : rank<N-1> { };
template<> struct rank<0> { };

struct resolver {
    const di_container& di;
    template<typename T>
    T* construct()
    {
        T* result = nullptr;
        return construct_impl2(result, rank<5>());
    }

    template<typename T>
    auto construct_impl2(T*& result, rank<0>)
        -> decltype(new T())
    {
        result = new T();
        return result;
    }

    template<typename T>
    auto construct_impl2(T*& result, rank<1>)
        -> decltype(new T( dummy{} ))
    {
        result = new T(*this);
        return result;
    }

    template<typename T>
    auto construct_impl2(T*& result, rank<2>)
        -> decltype(new T( dummy{}, dummy{} ))
    {
        result = new T(*this, *this);
        return result;
    }

    template<typename T>
    auto construct_impl2(T*& result, rank<3>)
        -> decltype(new T( dummy{}, dummy{}, dummy{} ))
    {
        result = new T(*this, *this, *this);
        return result;
    }

    template<typename T>
    auto construct_impl2(T*& result, rank<3>)
        -> decltype(new T( dummy{}, dummy{}, dummy{}, dummy{} ))
    {
        result = new T(*this, *this, *this, *this);
        return result;
    }

    template<typename T>
    auto construct_impl2(T*& result, rank<4>)
        -> decltype(new T( dummy{}, dummy{}, dummy{}, dummy{}, dummy{} ))
    {
        result = new T(*this, *this, *this, *this, *this);
        return result;
    }

    template<typename T>
    auto construct_impl2(T*& result, rank<5>)
        -> decltype(new T( dummy{}, dummy{}, dummy{}, dummy{}, dummy{}, dummy{} ))
    {
        result = new T(*this, *this, *this, *this, *this, *this);
        return result;
    }

private:
    template<typename T>
    operator T*() const
    {
        return di.resolve<T>();
    }
};
template<typename T>
T* di_container::resolve() const
{
    resolver r{*this};
    return r.construct<T>();
}

struct dependency1{
    static constexpr auto n_depends = 0u;
    dependency1()
    {
        std::cout << "dependency1 constructed\n";
    }
};

struct dependency2{
    static constexpr auto n_depends = 0u;
    dependency2()
    {
        std::cout << "dependency2 constructed\n";
    }
};

struct dependency3{
    static constexpr auto n_depends = 1u;
    dependency3(dependency1*)
    {
        std::cout << "dependency3 constructed\n";
    }
};

struct test {
    static constexpr auto n_depends = 2u;
    test(dependency2*, dependency3*)
    {
        std::cout << "test constructed\n";
    }
};

int main()
{
    di_container di;
    auto x = di.resolve<test>();
}
