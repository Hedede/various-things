/* variation 1, I had an idea, of wrapping stream in a special wrapper class,
 * so that templated stream classes would work with runtime polymorphism */
struct file_stream {
	template<typename OutputIterator>
	void get(OutputIterator begin, size_t count, char delim);
	size_t position() const;
};


template <typename OutputIterator>
struct input_stream {
	struct wrapper_base {
		virtual void get(OutputIterator begin, size_t count, char space) = 0;
	};

	template<typename Stream>
	struct wrapper : wrapper_base {
		Stream& impl;
		void get(OutputIterator begin, size_t count, char space) override
		{
			impl.get(begin,count,space);
		}
	};

	template<typename Stream>
	input_stream(Stream& s)
		: wrap(new wrapper<Stream>(s))
	{}

	void get(OutputIterator begin, size_t count, char space)
	{
		wrap->get(begin,count,space);
	}

	wrapper_base* wrap;
};

struct HdfReader {
	using OutputIterator = std::back_inserter<std::string>;
	using Stream = input_stream<OutputIterator>;

	HdfReader(Stream&);
}

struct HdfReader2 {
	using OutputIterator = std::back_inserter<std::string>;

	template<class Stream>
	HdfReader(Stream& fjashogdasjkgflhdfjsgdpolsfgj)
		: yuh(input_stream<OutputIterator>(fjashogdasjkgflhdfjsgdpolsfgj))
	{}
}


int main()
{
	file_stream stream;
	HdfReader2 a(stream);
	stdcout_stream stream2;
	HdfReader2 b(stream2);

	HdfReader::Stream warper(stream);
	HdfReader c(warper);
	HdfReader::Stream warper2(stream2);
	HdfReader d(warper2);
}



/* variation 2, I decided that wrapper is unnecessary, and it is simpler to just
 * template whole class (which also solves issue of runtime polymorphism) */
template<typename OutputIterator>
struct stream {
	static_assert(is_same<char,std::iterator_traits<OutputIterator>::value_type>,
	              "stream supports only char type");

	virtual void get(char& c) = 0;
	virtual void peek(char& c) = 0;
	virtual size_t position() const = 0;

	void get(OutputIterator begin, size_t count)
	{
		get(begin, count, aw::nl);
	}

	void get(OutputIterator begin, char delim)
	{
		get(begin, 0, delim);
	}

	virtual void get(OutputIterator begin, size_t count, char delim) = 0;
};
