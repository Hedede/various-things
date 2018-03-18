#include <cstddef>
#include <cstdint>
#include <string>
#include <vector>
#include <sstream>

namespace math {
inline char mask8(uint32_t x) {
	return x & 0xFF;
}
}
typedef uint8_t        u8;
typedef  int8_t        i8;

typedef uint16_t       u16;
typedef  int16_t       i16;

typedef uint32_t       u32;
typedef  int32_t       i32;

typedef uint64_t       u64;
typedef  int64_t       i64;

extern void do_nothing(u32& uu);

//! Enumeration of 'special' unicode codepoints
enum class CodePoint : u32 {
	MaxASCII = 0x80,
	SurrogateStart =  0xD800,
	LeadSurrogateStart = 0xD800,
	LeadSurrogateEnd = 0xDBFF,
	TailSurrogateStart = 0xDC00,
	TailSurrogateEnd = 0xDFFF,
	SurrogateEnd = TailSurrogateEnd,
	MaxCodePoint = 0x10FFFF // I hate UTF-16
};

//! Check if code point is a surrogate
inline bool isSurrogate(u16 cp)
{
	return (CodePoint(cp) >= CodePoint::SurrogateStart &&
		CodePoint(cp) <= CodePoint::SurrogateEnd);
}

//! Check if code point is valid
inline bool isValidCodepoint(u32 cp)
{
	return CodePoint(cp) <= CodePoint::MaxCodePoint && !isSurrogate(cp);
}

namespace utf8 {
//! Check whether character is a trail shcracter
inline bool isTrail(u8 ch)
{
	// Two most significant bits are equal '10'
	return (ch & 0xC0) == 0x80;
	//return (ch & 0b11000000) == 0b10000000;
}

//! Determine how many characters are in an utf8 sequence
inline size_t sequenceLength(u8 lead) {
	if(lead < 0x80) {
		return 1;
	} else if((lead >> 5) == 0x6) {
		return 2;
	} else if((lead >> 4) == 0xE) {
		return 3;
	} else if((lead >> 3) == 0x1E) {
		return 4;
	} else {
		return 0;
	}
}

//! Determine how many characters are required to encode codepoint \a cp
inline size_t width(u32 cp) {
	if(cp < 0x80) {
		return 1;
	} else if(cp < 0x800) {
		return 2;
	} else if(cp < 0x10000) {
		return 3;
	}
	return 4;
}

inline bool isOverlong(u32 cp, size_t length)
{
	return width(cp) != length;
}
} //namespace utf8
namespace utf16 {
//! Check if code point is first surrogate
inline bool isFirstSurrogate(u16 cp)
{
	return (CodePoint(cp) >= CodePoint::LeadSurrogateStart &&
	        CodePoint(cp) <= CodePoint::LeadSurrogateEnd);
}

//! Check if code point is second surrogate
inline bool isSecondSurrogate(u16 cp)
{
	return (CodePoint(cp) >= CodePoint::TailSurrogateStart &&
	        CodePoint(cp) <= CodePoint::TailSurrogateEnd);
}

inline size_t width(u32 cp) {
	if(cp < 0x10000)
		return 1;

	return 2;
}

constexpr int accept = 1;
constexpr int reject = 0;

i32 get1(u16 input, u32& output, i32& state)
{
	if (state == 0) {
		if (!isSurrogate(input)) {
			output = input;
			return accept;
		}
		
		if (isFirstSurrogate(input)) {
			output = (input & 0x3FF) << 10;
			state = 1;
		}

		return reject;
	}
	if (state == 1) {
		state = 0;
		if (!isSecondSurrogate(input)) {
			return reject;
		}

		output += input & 0x3FF;

		return accept;
	}
}

template<typename Iterator>
inline Iterator get2(Iterator input, Iterator end, u32& cp)
{
	u16 first = *(input++);

	if (!isSurrogate(first)) {
		cp = first;
		return input;
	}

	if (isFirstSurrogate(first)) {
		u16 second = *(input++);
		if (isSecondSurrogate(second)) {
			cp = ((first  & 0x3FF) << 10) +
			      (second & 0x3FF) + 0x10000;
			return input;
		}
	} 

	cp = -1;
	return input;
}
} // namespace


#include <iostream>
#include <chrono>
#include <utility>
void test_get1(std::u16string& test)
{
	auto begin = std::begin(test);
	auto end = std::end(test);

	auto start = std::chrono::high_resolution_clock::now();

	uint32_t codepoint;

	int32_t state = 0;
	while (begin != end) {
		auto ret = utf16::get1(*begin++, codepoint, state);

		if (!ret)
			continue;

		do_nothing(codepoint);
	}

	auto finish = std::chrono::high_resolution_clock::now();
	
	std::chrono::duration<double> diff = finish - start;
	std::cout << "get_unchecked: " << diff.count() << std::endl;
}

void test_get2(std::u16string& test)
{
	auto begin = std::begin(test);
	auto end = std::end(test);

	auto start = std::chrono::high_resolution_clock::now();

	uint32_t codepoint;
	uint32_t state = 0;

	while (begin != end) {
		u32 cp;
		begin = utf16::get2(begin, end, cp);

		if (cp == -1)
			continue;

		do_nothing(cp);
	}
	auto finish = std::chrono::high_resolution_clock::now();
	
	std::chrono::duration<double> diff = finish - start;
	std::cout << "decode: " << diff.count() << std::endl;
}

int main(int argc, char** argv)
{
	std::string test = std::u16string;
	size_t count = std::stoll(argv[1]);
	while (count--)
		test = test + test;

	test_get1(test);
	test_get2(test);

	test_get1(test);
	test_get1(test);

	test_get2(test);
	test_get2(test);

	test_get2(test);
	test_get1(test);

/*
	std::string x;
	typedef typename std::back_insert_iterator<std::string> out_iterator_type;
	out_iterator_type it(x);
	for (auto b1 = std::begin(test2), b2 = std::begin(test3);
	     b1 != std::end(test2) && b2 != std::end(test3);
	     ++b1, ++b2) {
		utf8::append(*b1, it);
		utf8::append(u32(' ' & 0xFF), it);
		utf8::append(u32('|' & 0xFF), it);
		utf8::append(u32(' ' & 0xFF), it);
		utf8::append(*b2, it);
		utf8::append(u32('\n' & 0xFF), it);
	}
	std::cout << x << std::endl;*/
}
