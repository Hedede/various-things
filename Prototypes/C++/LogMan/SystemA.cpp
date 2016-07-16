#include "Logger.h"

namespace A {
LogFront log;
void setLogger(Logger* l)
{
	log.logger = l;
}

void do_stuff()
{
	log.log({"sysa", "do_stuff()"});
}
};
