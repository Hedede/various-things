#include "Logger.h"
namespace A {
void setLogger(Logger* log);
void do_stuff();
}

namespace B {
void setLogger(Logger* log);
void do_stuff();
}

#include <iostream>
struct LogCerr : LogBook {
	using LogBook::LogBook;

	void log(Message msg) override
	{
		std::cerr << msg.source << ": " << msg.text << "\n";
	}
};

int main()
{
	MultiLogger l;

	l.log({"main", "-1-"});
	A::do_stuff();
	B::do_stuff();

	A::setLogger(&l);

	l.log({"main", "-2-"});
	A::do_stuff();
	B::do_stuff();

	LogCerr log(l);

	l.log({"main", "-3-"});
	A::do_stuff();
	B::do_stuff();

	B::setLogger(&l);

	l.log({"main", "-4-"});
	A::do_stuff();
	B::do_stuff();

	l.remove(log);

	l.log({"main", "-5-"});
	A::do_stuff();
	B::do_stuff();
}
