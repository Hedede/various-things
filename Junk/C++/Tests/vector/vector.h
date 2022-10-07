// declare and implement the template vector class here
#include <iostream>
#include <algorithm>
#include <cassert>
#include <new>

template <class T>
class vector {
	void check_capacity( size_t new_size )
	{
		if ( _capacity < new_size )
			reallocate( new_size );
	}

	void construct(T* begin, T* end)
	{
		for (T* it = begin; it != end; ++it)
			new(begin) T;
	}

	void destruct(T* begin, T* end)
	{
		for (T* it = begin; it != end; ++it)
			it->~T();
	}

	void reallocate(size_t new_size)
	{
		size_t size_bytes = new_size*sizeof(T);
		T* new_data = reinterpret_cast<T*>( operator new(size_bytes) );

		try {
			for (size_t i = 0; i < _size; ++i)
			{
				new(new_data+i) T( data[i] );
			}
		} catch(...) {
			if (new_data)
				destruct(new_data, new_data + new_size);

			operator delete( new_data, new_size );
			throw;
		}

		data = new_data;
		_capacity = new_size;
	}

public:
	T*      data = nullptr;

	size_t _size = 0;
	size_t _capacity = 0;

	vector() = default;

	vector(size_t size)
	{
		resize(size);
	}

	vector(T* begin, T* end)
	{
		assign( begin, end );
	}

	void resize(size_t new_size)
	{
		check_capacity( new_size );

		if (_size > new_size)
			destruct( data+new_size, data+_size );
		else
			construct( data+_size, data+new_size );
		
		_size = new_size;
	}

	void assign(T* begin, T* end)
	{
		resize(end - begin);

		/*T* it = data;
		while (begin != end)
			*it++ = *begin++;*/
		std::copy(begin,end,data);
	}

	size_t getSize() const
	{
		return _size;
	}

	T& operator[](size_t index)
	{
		return data[index];
	}

	T const& operator[](size_t index) const
	{
		return data[index];
	}

	void BubbleSort()
	{
		T* end = end = data+_size;
		for (size_t i = 1; i <= _size; ++i)
			for (T* it = data; it < end-i; ++it)
				if (*it > *(it+1))
					std::iter_swap(it, it+1);
	}

	void Rotate(T* begin, T* new_begin, T* end)
	{
		std::rotate(begin, new_begin, end);
	}

	vector add(vector const& b) const
	{
		assert(_size == b._size);
		vector result(data, data+_size);
		for (size_t i = 0; i < _size; ++i)
			result[i] += b[i];
		return result;
	}
};

template<typename T>
vector<T> operator+(vector<T> const& a, vector<T> const& b)
{
	return a.add(b);
}

template<typename T>
void INFO(vector<T> vec)
{
	std::cout << "size = " << vec.getSize() << '\n';
	std::cout << "capacity = " << vec._capacity << '\n';
	std::cout << "data = " << vec.data << " { ";

	if (vec.getSize() > 0) {
		std::cout << vec[0];
		for ( size_t i = 1; i < vec.getSize(); ++i )
			std::cout << ", " << vec[i];
	}
	std::cout << " }\n";
}
