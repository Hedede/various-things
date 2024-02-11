#include <deque>
#include <iostream>
#include <cstdlib>
#include <sys/time.h>

class Timer
{
	timespec timer[2];

public:

	timespec start()
	{
		clock_gettime(CLOCK_REALTIME, &timer[0]);
		return this->timer[0];
	}

	timespec stop()
	{
		clock_gettime(CLOCK_REALTIME, &timer[1]);
		return this->timer[1];
	}

	double duration() const
	{
		int secs(this->timer[1].tv_sec - this->timer[0].tv_sec);
		int nsecs(this->timer[1].tv_nsec - this->timer[0].tv_nsec);

		if (nsecs < 0)
		{
			--secs;
			nsecs += 1000000;
		}

		return static_cast<double>(secs * 1000 + nsecs / 1000000.0);
	}
};

long long lpf(long long int x)
{
	long long int lpf = 2;
	while (x > lpf)
	{
		if (x%lpf==0)
		{
			x = x/lpf;
			lpf = 2;
		}
		else
		{
			lpf++;
		}
	}
	return lpf;
}

int main ()
{
	Timer tm;

	tm.start();
	std::cout << lpf(600851475143L)<<std::endl;
	tm.stop();

	std::cout << tm.duration() <<std::endl;
}
