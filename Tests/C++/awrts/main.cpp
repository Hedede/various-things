/*
 * Copyright (C) 2013-2016 absurdworlds
 * Copyright (C) 2016      Hedede <Haddayn@gmail.com>
 *
 * License LGPLv3 or later:
 * GNU Lesser GPL version 3 <http://gnu.org/licenses/lgpl-3.0.html>
 * This is free software: you are free to change and redistribute it.
 * There is NO WARRANTY, to the extent permitted by law.
 */
#include <time.h>
#include <cstdio>
#include <csetjmp>
#include <stdlib.h>
#include <signal.h>
#include <unistd.h>
#include <iostream>
#include <chrono>
#include <ratio>


unsigned long getTime()
{
	struct timespec tp;
	int rc = clock_gettime(CLOCK_MONOTONIC, &tp);
	if (rc == 0) {
		return (tp.tv_sec * 1000000000ULL) + (tp.tv_nsec); // 1000000);
	}
	return 0;
}

namespace aw {
namespace rts {

static bool what = true;
std::jmp_buf jump_buffer;

void jump_back(int sig)
{
	what = false;
	std::longjmp(jump_buffer, 0);
}

void update()
{
}

using u64 = unsigned long;
static u64 troeben[0xFFFFFF] = {};

int main(int c, char const* const* v)
{
	bool run = true;

	using namespace std::chrono;

	signal(SIGINT, jump_back);
	setjmp(jump_buffer);

	if (what) {
	// unsure: is it ‘good enough’ to ditch hrengin::getTime()?
	auto time      = getTime();//std::chrono::high_resolution_clock::now();
	auto prev_time = time;

	//using Duration = std::chrono::duration<u64, std::nano>;
	using Duration = decltype(time);

	while (run) {
		prev_time = time;
		time = getTime();//std::chrono::high_resolution_clock::now();

		//Duration diff = duration_cast<Duration>(time - prev_time);

		//std::cout << diff.count() << "\n";
		//auto val = diff;//.count();
		static u64 govn=0;
		troeben[govn++] = time - prev_time;//val;

		//fwrite(&val, sizeof(val), 1, stdout);
		//write(1,&val,sizeof(val));
	}
	}

	std::cout << "PRINT\n" << std::endl;
	for (auto v : troeben) {
		std::cout << v << "\n";
		if (v <= 10ULL) return 0;
	}
}

} // namespace rts
} // namespace aw

int main(int c, char** v)
{
	return aw::rts::main(c,v);
}
