#include <iostream>
#include <string_view>
#include <experimental/source_location>

using sloc = std::experimental::source_location;
 
void log(std::string_view message,
         const sloc& location = sloc::current())
{
    std::cout << "info:"
              << location.file_name() << ":"
	      << location.function_name() << ":"
              << location.line() << ":"
              << location.column() << " "
              << message << '\n';
}

auto gloc = sloc::current();

int main()
{
    log("Hello world!");

    auto lam = [] {
	    log ("asasa");
    };

    auto loc = sloc::current();
    log("hullo", loc);
    log("hullo", gloc);
    lam();
}
