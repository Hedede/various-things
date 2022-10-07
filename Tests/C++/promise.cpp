#include <future>
#include <iostream>



int main()
{
	std::promise<void> p;
	auto f = p.get_future();
	std::cout << f.valid() << '\n';

	std::cout << (f.wait_for(std::chrono::seconds(0)) == std::future_status::timeout) << '\n';
	try {
	p.set_value();
	} catch(std::system_error ex)
	{
		std::cerr <<ex.what();
	}
	std::cout << (f.wait_for(std::chrono::seconds(0)) == std::future_status::timeout) << '\n';
	std::cout << f.valid() << '\n';
}
