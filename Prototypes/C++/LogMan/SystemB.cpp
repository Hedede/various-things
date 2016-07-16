#include "Logger.h"

namespace B {
LogFront log;
void setLogger(Logger* l)
{
	log.logger = l;
}

void do_stuff()
{
	log.log({"sysb", "do_stuff()"});
}
};
