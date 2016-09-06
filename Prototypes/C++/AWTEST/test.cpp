#include "test.h"

using namespace aw;

namespace aw {
namespace test {
namespace {
context  ctx{"FILENAME"};
context& file_context{ctx};
}
}
}


struct stuff {
	void do_that() {}
	void do_this() {}
	bool empty() { return false; }

	int whatever = 0;
} something;

test::test sometest{
	"sometest",
	[] {
		stuff x;

		{test::block _1_ {test::stage::setup};
			x = something;
		}

		{test::block _2_ {test::stage::preconditions};
			int value = 0;
			test::check<test::equal>(x.whatever, value);
		}

		{test::block _3_ {test::stage::check};
			x.do_this();
			x.do_that();
		}

		{test::block _4_ {test::stage::postconditions};
			test::check<test::equal>(x.empty(), true);
		}
	}
};

int main()
{
	extern int main2();
	test::file_context.run();
	return main2();
}
