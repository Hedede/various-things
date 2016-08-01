#include <memory>

struct Base {

};
#include <iostream>
struct Derived : Base{
	~Derived(){
		std::cout << "sejfios\n";

	}
};


int main()
{
	struct Deleter {
		void operator()(Base* der)
		{
			delete static_cast<Derived*>(der);
		}
	};
	std::unique_ptr<Base, Deleter> poitr { new Derived, Deleter{} };
	std::cout << sizeof(poitr) << "\n";
}
