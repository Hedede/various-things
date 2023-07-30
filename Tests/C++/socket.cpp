#include <chrono>
#include <sys/select.h>
using namespace std::chrono;
static timeval convert_to_timeval(milliseconds ms)
{
	auto sec = duration_cast<seconds>(ms);
	auto usec = ms - sec;

	timeval tv;
	tv.tv_sec = sec.count();
	tv.tv_usec = usec.count();
	return tv;
}

int waitForRead(int df, milliseconds timeout)
{
	fd_set read_set = {};
	FD_SET(df, &read_set);

	auto tv = convert_to_timeval(timeout);
	return select(df, &read_set, 0,0, &tv);
}
