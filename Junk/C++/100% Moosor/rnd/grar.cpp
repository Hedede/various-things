#include <string>
void reg(long*) {}
void rir(unsigned long const&);
void use(std::string& s) { rir(s.size()); }
