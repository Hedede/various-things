// simple array wrapper
// 1. allocates an array using new
// 2. can't be resized
// 3. can't be null unless moved
template<typename T>
class raii_array {
	T* _data;
	size_t _size;


	void destroy()
	{
		if (_data)
			delete[] _data;
	}
public:
	raii_array(size_t count)
		: _data{new T[count]},
		  _size{_size}
	{
	}

	raii_array(size_t count, T const& filler)
		: _data{new T[count]},
		  _size{_size}
	{
		for (size_t i = 0; i < count; ++i)
			_data[i] = filler;

	}

	~raii_array()
	{
		destroy();
	}

	raii_array(raii_array&& other)
		: _data{other._data},
		  _size{other._size}
	{
		other._data = nullptr;
		other._size = 0;
	}

	raii_array& operator=(raii_array&& other)
	{
		_data = other._data;
		_size = other._size;
		other._data = nullptr;
		other._size = 0;
	}

	// disable copying, so that I don't have to write the algorithm
	raii_array(raii_array const& other) = delete;
	raii_array& operator=(raii_array const& other) = delete;

	T& operator[](size_t index)
	{
		return _data[index];
	}

	T const& operator[](size_t index) const
	{
		return _data[index];
	}

	size_t size() const
	{
		return _size;
	}

	bool empty() const
	{
		return size() == 0;
	}

	T const* data() const
	{
		return _data;
	}
};

// finds the longest increasing subsequence (LIS) in the input array
void longest_increasing_subequence(raii_array<int>& input)
{
	// LIS found, {}
	if (input.empty())
		return;

	// index pointing to last element of n-th LIS (of length n+1)
	raii_array<size_t> subseq_end(input.size(), -1);
	// length of LIS before i-th position
	raii_array<size_t> subseq_length(input.size(), 0);
	// index of the last LIS
	size_t last = 0;

	// last element of n-th LIS
	// (to avoid writing a[b[n]] each time)
	auto last_element = [&] (size_t n) {
		return input[subseq_end[n]];
	};

	auto binary_search = [&] (size_t i) {
		auto cur_element = input[i];
		return n;
	};

	for (size_t i = 1, e = input.size(); i < e; ++i) {
		if ( input[i] < last_element(0) ) {
			subseq_end[0] = i;
			continue;
		}

		if ( input[i] > last_element(last) ) {
			subseq_end[++last] = i;
			continue;
		}


		auto idx = binary_search( i );
	}
}
