#include <iostream>
#include <unordered_set>
class Vehicle;

std::unordered_set<Vehicle const*> vehs;

struct Vehicle {
	Vehicle()
	{
		vehs.insert(this);
	}
	~Vehicle()
	{
		vehs.erase(this);
	}
	bool isValid() const
	{
		std::cout << "Vehicle::isValid()\n";
		return vehs.find(this) != end(vehs);
	}
};

/*
 * Proof that this code is still unsafe:
 *
 * there's a chance pointer will get assigned to other object
 * which means we will perform operations on new object, thinking
 * it is still the old one.
 */
Vehicle* func1()
{
	auto veh = new Vehicle;
	std::cout << (void*)veh << "\n";
	return veh;
}

Vehicle* func2()
{
	auto veh2 = new Vehicle;
	std::cout << (void*)veh2 << "\n";
	return veh2;
}

int main()
{
	auto veh = func1();

	delete veh;
	
	func2();

	if (veh->isValid())
		delete veh;

}
