#include <iostream>
#include <utility>

int main()
{
	auto _WAITTIME = 100;
	auto _waitTimeForOneITeration = 1000;
	std::cout << std::min(_WAITTIME,_waitTimeForOneITeration) << '\n';
	std::cout << (_WAITTIME > _waitTimeForOneITeration ? _waitTimeForOneITeration : _WAITTIME) << '\n';
}
