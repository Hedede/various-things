#include <vector>
#include <memory>


template<typename T>
struct chain_storage_traits {
	// TODO: find sweetspot
	static constexpr size_t block_size = 4;
};

template<typename T>
struct unitialized_storage {
	using storage = typename std::aligned_storage<sizeof(T), alignof(T)>::type;

	template<typename...Args>
	void construct(Args&&... args)
	{
		new ( get_pointer() ) T( std::forward<Args>(args)... );
	}

	void destruct()
	{
		get_ref().~T();
	}

	void* get_pointer()
	{
		return reinterpret_cast<void*>(&_data);
	}

	T& get_ref()
	{
		return *static_cast<T*>( get_pointer() );
	}

private:
	storage _data;
};

template<typename T>
struct chain_storage {
	static constexpr size_t block_size = chain_storage_traits<T>::block_size;
	using storage = unitialized_storage<T>;
	using block_type = std::unique_ptr<storage[]>;

	size_t capacity() const
	{
		return _data.size() * block_size;
	}

	void allocate_block()
	{
		_data.emplace_back( new storage[block_size] );
	}

	void grow(size_t n)
	{
		while (n > capacity())
			allocate_block();
	}

	block_type& get_block(size_t n)
	{
		return _data[n];
	}

	storage& get(size_t idx)
	{
		return get_block(idx / block_size)[idx % block_size];
	}

	template<typename...Args>
	void construct(size_t pos, Args&&... args)
	{
		get(pos).construct(std::forward<Args>(args)... );
	}

	void destruct(size_t pos)
	{
		get(pos).destruct();
	}

private:
	std::vector<block_type> _data;
};




template<typename T>
struct array_chain_iterator {
	static constexpr size_t block_size = chain_storage_traits<T>::block_size;
	using block_type = typename chain_storage<T>::block_type;

	array_chain_iterator(block_type* array, size_t pos)
		: array{array}, pos{pos}
	{}

	T& operator*()
	{
		return *reinterpret_cast<T*>(&(*array)[pos]);
	}

	T* operator->()
	{
		return reinterpret_cast<T*>(&(*array)[pos]);
	}

	array_chain_iterator& operator++()
	{
		if (++pos == block_size) {
			++array;
			pos = 0;
		}
		return *this;
	}

	bool operator!=(array_chain_iterator& other)
	{
		return other.array != array || other.pos != pos;
	}

private:
	block_type* array;
	size_t pos;
};

template<typename T>
struct array_chain {
	using iterator = array_chain_iterator<T>;
	static constexpr size_t block_size = chain_storage_traits<T>::block_size;

	T& operator[](size_t index)
	{
		return _data.get( index ).get_ref();
	}

	void push_back(T const& item)
	{
		_data.grow(_size + 1);
		_data.construct( _size++, item );
	}

	template<typename...Args>
	void emplace_back(Args&&... args)
	{
		_data.grow(_size + 1);
		_data.construct( _size++, std::forward<Args>(args)... );
	}

	void pop_back()
	{
		_data.destruct(_size--);
	}

	size_t capacity() const
	{
		return _data.capacity();
	}

	size_t size() const
	{
		return _size;
	}

	iterator begin()
	{
		return {&_data.get_block( 0 ), 0};
	}

	iterator end()
	{
		size_t i = _size / block_size;
		return {&_data.get_block( i ), _size % block_size};
	}

private:
	chain_storage<T> _data;
	size_t _size = 0;
};


#include <iostream>
int main()
{
	array_chain<std::string> ss;
	ss.push_back("yoy");
	ss.push_back("yuy");
	ss.push_back("yey");
	ss.push_back("yay");
	ss.push_back("yiy");

	for (auto s : ss)
		std::cout << s;
}
