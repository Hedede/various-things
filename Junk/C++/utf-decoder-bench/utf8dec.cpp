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

template<typename Iterator>
Iterator append(u32 cp, Iterator output)
{
	if(isValidCodepoint(cp)) {
		if(cp < 0x80) {
			*(output++) = u8(cp);
		} else if(cp < 0x800) {
			*(output++) = u8((cp >> 6)   | 0xC0);
			*(output++) = u8((cp & 0x3F) | 0x80);
		} else if(cp < 0x10000) {
			*(output++) = u8((cp >> 12)         | 0xE0);
			*(output++) = u8(((cp >> 6) & 0x3F) | 0x80);
			*(output++) = u8((cp & 0x3F)        | 0x80);
		} else {
			*(output++) = u8((cp >> 18)          | 0xF0);
			*(output++) = u8(((cp >> 12) & 0x3F) | 0x80);
			*(output++) = u8(((cp >> 6) & 0x3F)  | 0x80);
			*(output++) = u8((cp & 0x3F)         | 0x80);
		}
	}

	return output;
}

template<typename Iterator>
Iterator get(Iterator& input, Iterator end, u32& cp)
{
	cp = *(input++);
	size_t length = sequenceLength(math::mask8(cp));

	if (length == 1) {
		return input;
	}

	cp = cp & (0xFF >> (length + 1));

	--length;
	if (end - input < length) {
		cp = -1;
		return input;
	}

	u8 octet;
	do {
		u8 octet = *(input++);
		if (!isTrail(octet)) {
			cp = -1;
			break;
		}

		cp = (cp << 6) + (octet & 0x3F);
	} while (--length > 0);
	return input;
}

template<typename Iterator>
Iterator get_new(Iterator input, Iterator end, u32& cp)
{
	cp = *(input++);
	size_t length = sequenceLength(math::mask8(cp));

	if (length == 1)
		return input;

	cp = cp & (0xFF >> (length + 1));

	// Decrement length, because we incremented input by 1
	// so (end - input) is less by one
	--length;
	if (end - input < length) {
		cp = -1;
		return end;
	}

	u8 octet;
	switch(length) {
		case 3:
			octet = *(input++);
			if (!isTrail(octet)) {
				cp = -1;
				break;
			}

			cp = (cp << 6) + (octet & 0x3F);
		case 2:
			octet = *(input++);
			if (!isTrail(octet)) {
				cp = -1;
				break;
			}

			cp = (cp << 6) + (octet & 0x3F);
		case 1:
			octet = *(input++);
			if (!isTrail(octet)) {
				cp = -1;
				break;
			}

			cp = (cp << 6) + (octet & 0x3F);
			break;
		case 0:
			cp = -1;
			break;
	}

	return input;
}


//! Determine how many characters are in an utf8 sequence
inline size_t trailLength(u8 lead) {
	if (lead < 0xC2 || lead > 0xF4)
		return -1;

	if (lead < 0xE0)
		return 1;
	if (lead < 0xF0)
		return 2;

	return 3;
}

/*!
 * Decode UTF-8 sequence, if valid
 */
template<typename Iterator>
u32 decode_soopa(Iterator& input, Iterator end)
{
	u32 cp = *(input++);
	if (cp < 0x80)
		return cp;

	size_t length = trailLength(cp);
	cp = cp & (0x3F >> length);

	if (end - input < length)
		return -1;

	u8 octet;
	switch (length) {
	case 3:
		octet = *(input++);
		if (!isTrail(octet))
			return -1;

		cp = (cp << 6) | (octet & 0x3F);
	case 2:
		octet = *(input++);
		if (!isTrail(octet))
			return -1;

		cp = (cp << 6) | (octet & 0x3F);
	case 1:
		octet = *(input++);
		if (!isTrail(octet))
			return -1;

		cp = (cp << 6) | (octet & 0x3F);
		break;
	}

	return cp;
}

template<typename Iterator>
Iterator append_unchecked(u32 cp, Iterator output)
{
	if(cp < 0x80) {
		*(output++) = u8(cp);
	} else if(cp < 0x800) {
		*(output++) = u8((cp >> 6)   | 0xC0);
		*(output++) = u8((cp & 0x3F) | 0x80);
	} else if(cp < 0x10000) {
		*(output++) = u8((cp >> 12)         | 0xE0);
		*(output++) = u8(((cp >> 6) & 0x3F) | 0x80);
		*(output++) = u8((cp & 0x3F)        | 0x80);
	} else {
		*(output++) = u8((cp >> 18)          | 0xF0);
		*(output++) = u8(((cp >> 12) & 0x3F) | 0x80);
		*(output++) = u8(((cp >> 6)  & 0x3F) | 0x80);
		*(output++) = u8((cp & 0x3F)         | 0x80);
	}

	return output;
}

template<typename Iterator>
u32 get_unchecked(Iterator& input, Iterator end)
{
	u32 cp = *(input++);
	size_t length = sequenceLength(math::mask8(cp));

	if(length == 1)
		return cp;

	cp = cp & (0x7F >> (length + 1));

	switch(length) {
		case 4:
			cp = (cp << 6) + (*(input++) & 0x3F);
		case 3:
			cp = (cp << 6) + (*(input++) & 0x3F);
		case 2:
			cp = (cp << 6) + (*(input++) & 0x3F);
	}

	return cp;
}

template<typename Iterator>
u32 get_unchecked2(Iterator& input, Iterator end)
{
	u32 cp = *(input++);

	if (cp < 0x80)
		return cp;

	size_t length = sequenceLength(cp & 0xFF);
	cp = cp & (0x3F >> length);

	switch(length) {
		case 4:
			cp = (cp << 6) | (*(input++) & 0x3F);
		case 3:
			cp = (cp << 6) | (*(input++) & 0x3F);
		case 2:
			cp = (cp << 6) | (*(input++) & 0x3F);
	}

	return cp;
}

template<typename Iterator>
u32 get_unchecked3(Iterator& input, Iterator end)
{
	u32 cp = *(input++);

	if (cp < 0x80)
		return cp;

	size_t length;
	if ((cp >> 5) == 0b110) {
		length = 2;
		cp = cp & 0b0001'1111;
	} else if ((cp >> 4) == 0b1110) {
		length = 3;
		cp = cp & 0b0000'1111;
	} else if ((cp >> 3) == 0b1111'0) {
		length = 4;
		cp = cp & 0b0000'0111;
	} else {
		return -1;
	}

	switch(length) {
		case 4:
			cp = (cp << 6) | (*(input++) & 0x3F);
		case 3:
			cp = (cp << 6) | (*(input++) & 0x3F);
		case 2:
			cp = (cp << 6) | (*(input++) & 0x3F);
	}

	return cp;
}
} // namespace utf8

// Copyright (c) 2008-2010 Bjoern Hoehrmann <bjoern@hoehrmann.de>
// See http://bjoern.hoehrmann.de/utf-8/decoder/dfa/ for details.

#define UTF8_ACCEPT 0
#define UTF8_REJECT 12

static const uint8_t utf8d[] = {
  // The first part of the table maps bytes to character classes that
  // to reduce the size of the transition table and create bitmasks.
   0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,  0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
   0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,  0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
   0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,  0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
   0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,  0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
   1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,  9,9,9,9,9,9,9,9,9,9,9,9,9,9,9,9,
   7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,  7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,
   8,8,2,2,2,2,2,2,2,2,2,2,2,2,2,2,  2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,
  10,3,3,3,3,3,3,3,3,3,3,3,3,4,3,3, 11,6,6,6,5,8,8,8,8,8,8,8,8,8,8,8,

  // The second part is a transition table that maps a combination
  // of a state of the automaton and a character class to a state.
   0,12,24,36,60,96,84,12,12,12,48,72, 12,12,12,12,12,12,12,12,12,12,12,12,
  12, 0,12,12,12,12,12, 0,12, 0,12,12, 12,24,12,12,12,12,12,24,12,24,12,12,
  12,12,12,12,12,12,12,24,12,12,12,12, 12,24,12,12,12,12,12,12,12,24,12,12,
  12,12,12,12,12,12,12,36,12,36,12,12, 12,36,12,12,12,12,12,36,12,36,12,12,
  12,36,12,12,12,12,12,12,12,12,12,12, 
};

uint32_t inline
decode(uint32_t* state, uint32_t* codep, uint32_t byte) {
  uint32_t type = utf8d[byte];

  *codep = (*state != UTF8_ACCEPT) ?
    (byte & 0x3fu) | (*codep << 6) :
    (0xff >> type) & (byte);

  *state = utf8d[256 + *state + type];
  return *state;
}


#include <iostream>
#include <chrono>
#include <utility>

void
printCodePoints(uint8_t* s) {
  uint32_t codepoint;
  uint32_t state = 0;

  for (; *s; ++s)
    if (!decode(&state, &codepoint, *s))
      printf("U+%04X\n", codepoint);

  if (state != UTF8_ACCEPT)
    printf("The string is not well-formed\n");

}

void test_get_unchecked(std::string& test)
{
	auto begin = std::begin(test);
	auto end = std::end(test);

	auto start = std::chrono::high_resolution_clock::now();

	uint32_t codepoint;
	uint32_t state = 0;

	while (begin != end) {
		u32 cp = utf8::get_unchecked(begin, end);

		if (cp == -1)
			continue;

		do_nothing(cp);
	}

	auto finish = std::chrono::high_resolution_clock::now();
	
	std::chrono::duration<double> diff = finish - start;
	std::cout << "get_unchecked: " << diff.count() << std::endl;
}

void test_get_unchecked2(std::string& test)
{
	auto begin = std::begin(test);
	auto end = std::end(test);

	auto start = std::chrono::high_resolution_clock::now();

	uint32_t codepoint;
	uint32_t state = 0;

	while (begin != end) {
		u32 cp = utf8::get_unchecked2(begin, end);

		if (cp == -1)
			continue;

		do_nothing(cp);
	}

	auto finish = std::chrono::high_resolution_clock::now();
	
	std::chrono::duration<double> diff = finish - start;
	std::cout << "get_unchecked2: " << diff.count() << std::endl;
}

void test_get_unchecked3(std::string& test)
{
	auto begin = std::begin(test);
	auto end = std::end(test);

	auto start = std::chrono::high_resolution_clock::now();

	uint32_t codepoint;
	uint32_t state = 0;

	while (begin != end) {
		u32 cp = utf8::get_unchecked3(begin, end);

		if (cp == -1)
			continue;

		do_nothing(cp);
	}

	auto finish = std::chrono::high_resolution_clock::now();
	
	std::chrono::duration<double> diff = finish - start;
	std::cout << "get_unchecked3: " << diff.count() << std::endl;
}

void test_decode_soopa(std::string& test)
{
	auto begin = std::begin(test);
	auto end = std::end(test);

	auto start = std::chrono::high_resolution_clock::now();

	uint32_t codepoint;
	uint32_t state = 0;

	while (begin != end) {
		u32 cp = utf8::decode_soopa(begin, end);

		if (cp == -1)
			continue;

		do_nothing(cp);
	}

	auto finish = std::chrono::high_resolution_clock::now();
	
	std::chrono::duration<double> diff = finish - start;
	std::cout << "decode_soopa: " << diff.count() << std::endl;
}


void test_decode(std::string& test)
{
	auto begin = std::begin(test);
	auto end = std::end(test);

	auto start = std::chrono::high_resolution_clock::now();

	uint32_t codepoint;
	uint32_t state = 0;

	while (begin != end)
		if (!decode(&state, &codepoint, (*begin++) & 0xFF ))
			do_nothing(codepoint);

	auto finish = std::chrono::high_resolution_clock::now();
	
	std::chrono::duration<double> diff = finish - start;
	std::cout << "decode: " << diff.count() << std::endl;
}

void test_get_new(std::string& test)
{
	auto begin = std::begin(test);
	auto end = std::end(test);

	auto start = std::chrono::high_resolution_clock::now();
	while (begin != end) {
		u32 cp;
		begin = utf8::get_new(begin, end, cp);

		if (cp == -1)
			continue;

		do_nothing(cp);
	}
	auto finish = std::chrono::high_resolution_clock::now();

	std::chrono::duration<double> diff = finish - start;
	std::cout << "get_new:" << diff.count() << std::endl;
}

void test_get(std::string& test)
{
	auto begin = std::begin(test);
	auto end = std::end(test);

	auto start = std::chrono::high_resolution_clock::now();
	while (begin != end) {
		u32 cp;
		begin = utf8::get(begin, end, cp);

		if (cp == -1)
			continue;

		do_nothing(cp);
	}
	auto finish = std::chrono::high_resolution_clock::now();
	
	std::chrono::duration<double> diff = finish - start;
	std::cout << "get: "<< diff.count() << std::endl;
}

int main(int argc, char** argv)
{
	std::string test = std::string(
	                std::istreambuf_iterator<char>(std::cin),
	                std::istreambuf_iterator<char>()
	);

	size_t count = std::stoll(argv[1]);
	while (count--)
		test = test + test;

	/*
	test_get_unchecked(test);
	test_decode(test);

	test_get(test);
	test_get(test);

	test_get_unchecked(test);
	test_decode(test);

	test_get(test);
	test_get_new(test);
*/

	test_get(test);
	test_get_new(test);
	test_decode_soopa(test);


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
