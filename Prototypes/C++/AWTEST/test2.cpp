#include "test.h"

TestFile("Test2");

Test (testtest) {
	bool b = true;

	Setup {
	}

	Preconditions {
	}

	Checks {
		b = false;
	}

	Postconditions {
		aw::test::check<aw::test::equal>(b, true);
	}
}

Test(test2) {

}

int main()
{
	return RunTests();
}
