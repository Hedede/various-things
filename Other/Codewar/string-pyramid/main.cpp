#include <string>
#include <vector>
#include <algorithm>
template<typename Iterator, typename T>
T join(Iterator begin, Iterator end, T const& delim)
{
    T sink;
    if (begin == end)
        return sink;
    sink += *begin++;
    while (begin != end)
        sink += delim + *begin++;
    return sink;
}

std::string watchPyramidFromTheSide(std::string characters)
{
    if (characters.empty())
        return {};
    size_t height = characters.size();
    size_t width = height*2 - 1;
    std::vector<std::string> lines(height, std::string(width,' '));

    for (size_t i = height; i !=0; --i) {
        std::fill_n(begin(lines[height-i]) + (i-1), width - 2*(i-1), characters[i-1]);
    }

    using namespace std::string_literals;
    return join(begin(lines), end(lines), "\n"s) + "\n";
}

std::string watchPyramidFromAbove(std::string characters)
{
    if (characters.empty())
        return {};
    size_t height = characters.size();
    size_t width = height*2 - 1;
    std::vector<std::string> lines(width, std::string(width,'*'));

    for (size_t i = 0; i < height; ++i) {
        std::fill_n(begin(lines[i]) + i, width - 2*i, characters[i]);
        std::fill_n(begin(lines) + i+1, width - 2*i-1, lines[i]);
    }

    using namespace std::string_literals;
    return join(begin(lines), end(lines), "\n"s) + "\n";
}

int countVisibleCharactersOfThePyramid(std::string characters)
{
    if (characters.empty())
        return -1;
    size_t height = characters.size();
    size_t width = height*2 - 1;
    return width * width;
}

int countAllCharactersOfThePyramid(std::string characters)
{
    if (characters.empty())
        return -1;
    size_t n = characters.size();
    return (4*n*n*n - n)/3;
}

#include <iostream>
int main()
{
    std::cout << watchPyramidFromTheSide("=====");
    std::cout << watchPyramidFromAbove(".o0O");
    std::cout << countVisibleCharactersOfThePyramid("abcd") << '\n';
    std::cout << countAllCharactersOfThePyramid("abcd") << '\n';
}
