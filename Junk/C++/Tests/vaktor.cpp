#include <tuple>
template<typename... Ts>
struct multi_vector {
    std::tuple<Ts*...> storage;
    size_t size;

};

template<typename T>
struct vaktor {
    T* storage;
    size_t size;

    void alloc(size_t size)
    {
        storage = (T*)operator new( size, (std::align_val_t)alignof( T ) );
    }

};

struct alignas(16) afloat {
    float f;
};

#include <iostream>
int main()
{
    vaktor<afloat> faf;
    faf.alloc(100);

    std::cout << "ryr!\n";
}
