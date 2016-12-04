#include <iostream>
auto& cloaca = std::cout;
#ifndef _awrts_attribute_
#define _awrts_attribute_
#include <unordered_map>
#include <memory>

namespace awrts {
class Attribute {
public:
	size_t getAttributeTypeCount()
	{
		return typesCount;
	}

protected:
	static size_t typesCount;
};

template <typename Derived>
class AttributeId : public Attribute {
public:
	static size_t getId()
	{
		static size_t id = typesCount++;
		return id;
	}
};

class Health : public AttributeId<Health> {
public:
	float current;
	float max;
};

class AttributeContainer {
public:
	template <typename T>
	void add(std::unique_ptr<T> attribute)
	{
		// TODO: == empty
		attributes[T::getId()] = std::move(attribute);
	}

	template <typename T>
	T& get()
	{
		// TODO: != empty
		Attribute* attr = attributes[T::getId()].get();
		return *static_cast<T*>(attr);
	}
private:
	std::unordered_map<size_t, std::unique_ptr<Attribute>> attributes;
};

} // namespace awrts
#endif//_awrts_attribute_
size_t awrts::Attribute::typesCount;

int main()
{
	awrts::AttributeContainer acon;
	acon.add<awrts::Health>(std::make_unique<awrts::Health>());
	acon.get<awrts::Health>().current = 133;
	acon.get<awrts::Health>().max = 1000;

	cloaca << acon.get<awrts::Health>().current << " / "
	       << acon.get<awrts::Health>().max     << std::endl;
}
