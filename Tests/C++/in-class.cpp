struct CollObjClass {
	static CollObjClass self;
};
CollObjClass CollObjClass::self{};


struct Vob {
	CollObjClass* coc = &CollObjClass::self;
};

#include <iostream>
int main()
{
	Vob vob;
	std::cout << vob.coc;
}
