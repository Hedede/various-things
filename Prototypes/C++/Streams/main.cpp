#include <cstdio>
#include <iostream>
#include <experimental/string_view>

namespace std {
	using experimental::string_view;
}

namespace aw {
constexpr const char nl = '\n';

std::string as_string(char c)
{
	return std::string(1, c);
}

std::string as_string(char const* s)
{
	return {s};
}

std::string as_string(std::string const& s)
{
	return {s};
}

namespace io {

enum class stream_status {
};

namespace input {
struct stream {
	virtual ~stream() = default;

	stream& peek(char& c)
	{
		read(c);
		return *this;
	}

	stream& get(char& c)
	{
		read(c);
		advance();
		return *this;
	}

	template<typename OutputIterator>
	stream& get(OutputIterator begin, size_t count)
	{
		return get(begin, count, aw::nl);
	}

	template<typename OutputIterator>
	stream& get(OutputIterator begin, char delimiter)
	{
		return get(begin, 0, delimiter);
	}

	template<typename OutputIterator>
	stream& get(OutputIterator begin, size_t count, char delimiter)
	{
		if (count > 0) {
			
		}
	}

	stream& get(char* buf, size_t count, char delimiter)
	{
		read(buf, count, delimiter);
	}

	size_t position() const;
	stream_status status() const;

protected:
	void set_status();

private:
	virtual void read(char& c) = 0;
	virtual void advance() = 0;
	
	virtual void read(char* buf, size_t count, char delimiter) = 0;

	stream_status status_;
};
} // namespace input

namespace output {
struct stream {
	virtual ~stream() = default;

	stream& put(char c)
	{
		write(c);
		return *this;
	}

	stream& put(std::string_view s)
	{
		write(s);
		return *this;
	}

	stream_status status() const
	{
		return status_;
	}

	virtual size_t position() const = 0;

protected:
	void set_status();

private:
	virtual void write(std::string_view s) = 0;
	virtual void write(char c) = 0;

	stream_status status_;
};
} // namespace input

struct out : output::stream {
	void write(std::string_view s) override
	{
		fwrite(s.data(), s.size(), sizeof(char), stdout);
	}

	void write(char c) override
	{
		fputc(c, stdout);
	}

	size_t position() const override
	{
		return ftell(stdout);
	}
};

class printer {
public:
	printer(output::stream& stream)
		: out(stream)
	{}

	template<typename... Args>
	stream_status print(Args&&...args)
	{
		(write(args), ...);
		return out.status();
	}

	template<typename... Args>
	stream_status println(Args&&...args)
	{
		(write(args), ..., write(aw::nl));
		return out.status();
	}

	template<typename... Args>
	stream_status operator()(Args&&...args)
	{
		return print(std::forward<Args>(args)...);
	}

private:
	void write(char c)
	{
		out.put(c);
	}

	void write(std::string_view s)
	{
		out.put(s);
	}

	void write(std::string const& s)
	{
		out.put(s);
	}

	void write(char const* s)
	{
		out.put(s);
	}

	template<typename T>
	void write(T const& v)
	{
		out.put(as_string(v));
	}

	output::stream& out;
};
} // namespace io
} // namespace aw

int main()
{
	using namespace aw::io;
	aw::io::out out;
	printer p(out);

	p("Hullo!", aw::nl);
	out.put('%');
	p.println();
}
