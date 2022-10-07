#include <stdio.h>
#include <execinfo.h>
#include <signal.h>
#include <stdlib.h>
#include <unistd.h>

#include <array>
#include <iostream>

void print_backtrace() {
	std::array<void*, 16> trace;
	size_t size;

	size = backtrace(trace.data(), trace.size());

	std::cerr << "\033[34m";
	backtrace_symbols_fd(trace.data(), size, STDERR_FILENO);
	std::cerr << "\033[0m";

	abort();
}

void handler(int sig)
{
	switch (sig) {
	case SIGSEGV:
		std::cout << "Segmentation fault" << std::endl;
		break;
	case SIGFPE:
		std::cout << "Floating point exception" << std::endl;
		break;
	}

	print_backtrace();
}

typedef void (*fault_handler)(int);

void installHandler(int sig, fault_handler fh)
{
	signal(sig, fh);
}

void baz() {
 int *foo = (int*)-1; // make a bad pointer
  printf("%d\n", *foo);       // causes segfault
}

void bar() { baz(); }
void foo() { bar(); }


int main(int argc, char **argv) {
	installHandler(SIGSEGV, handler);// install our handler
	foo(); // this will call foo, bar, and baz.  baz segfaults.
}
