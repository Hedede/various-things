#include <exception>

struct my_exception : std::exception {};


struct callback { void(*func)(); ~callback() { func(); } };

struct test {
	void run()
	{
		try {
			callback thing {
				[] { throw my_exception{}; }
			};
			thing.func();
		} catch (my_exception&) {
		}
	}
} tst;



int main()
{
	tst.run();
}
