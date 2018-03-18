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
u32 get(Iterator& input, Iterator end)
{
	u32 cp = *(input++);
	size_t length = sequenceLength(math::mask8(cp));

	if (length == 1) {
		return cp;
	}

	cp = cp & (0xFF >> (length + 1));

	--length;
	if (end - input < length)
		return -1;

	u8 octet;
	do {
		u8 octet = *(input++);
		if (!isTrail(octet)) {
			cp = -1;
			break;
		}

		cp = (cp << 6) + (octet & 0x3F);
	} while (--length > 0);
	return cp;
}

template<typename Iterator>
u32 get2(Iterator& input, Iterator end)
{
	u32 cp = *(input++);
	size_t length = sequenceLength(math::mask8(cp));

	if (length == 1) {
		return cp;
	}

	cp = cp & (0xFF >> (length + 1));

	--length;
	if (end - input < length)
		return -1;

	u8 octet;
	switch(length) {
		case 4:
			octet = *(input++);
			if(!isTrail(octet)) {
				return -1;
			}
			cp = (cp << 6) + (octet & 0x3F);
		case 3:
			octet = *(input++);
			if(!isTrail(octet)) {
				return -1;
			}
			cp = (cp << 6) + (octet & 0x3F);
		case 2:
			octet = *(input++);
			if(!isTrail(octet)) {
				return -1;
			}
			cp = (cp << 6) + (octet & 0x3F);
			break;
		case 0:
			cp = -1;
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

	if(length == 1) {
		return cp;
	}

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


int main(int argc, char** argv)
{
	std::string test = std::string(
	std::istreambuf_iterator<char>(std::cin),
	std::istreambuf_iterator<char>());


	size_t count = std::stoll(argv[1]);
	while (count--)
		test = test + test;

	std::vector<u32> razogrev;
	razogrev.reserve(test.size());
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

			razogrev.push_back(cp);
		}

		auto finish = std::chrono::high_resolution_clock::now();
		
		std::chrono::duration<double> diff = finish - start;
		std::cout << diff.count() << std::endl;
	}

	size_t sz = razogrev.size();
	razogrev.clear();



	std::vector<u32> test3;
	test3.reserve(sz);
	{
		auto begin = std::begin(test);
		auto end = std::end(test);

		auto start = std::chrono::high_resolution_clock::now();

		uint32_t codepoint;
		uint32_t state = 0;

		while (begin != end)
			if (!decode(&state, &codepoint, (*begin++) & 0xFF ))
				test3.push_back(codepoint);

		auto finish = std::chrono::high_resolution_clock::now();
		
		std::chrono::duration<double> diff = finish - start;
		std::cout << "decode: " << diff.count() << std::endl;
	}

	
	std::vector<u32> test4;
	test4.reserve(sz);
	{
		auto begin = std::begin(test);
		auto end = std::end(test);

		auto start = std::chrono::high_resolution_clock::now();
		while (begin != end) {
			u32 cp = utf8::get2(begin, end);

			if (cp == -1)
				continue;

			test4.push_back(cp);
		}
		auto finish = std::chrono::high_resolution_clock::now();

		std::chrono::duration<double> diff = finish - start;
		std::cout << "get2:" << diff.count() << std::endl;
	}


	std::vector<u32> test2;
	test2.reserve(sz);
	{
		auto begin = std::begin(test);
		auto end = std::end(test);

		auto start = std::chrono::high_resolution_clock::now();
		while (begin != end) {
			u32 cp = utf8::get(begin, end);

			if (cp == -1)
				continue;

			test2.push_back(cp);
		}
		auto finish = std::chrono::high_resolution_clock::now();
		
		std::chrono::duration<double> diff = finish - start;
		std::cout << "get: "<< diff.count() << std::endl;
	}



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
	std::cout << test2.size() << std::endl;
	std::cout << test3.size() << std::endl;

}
