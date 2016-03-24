#include <archive>

template<class Base, typename...Args>
struct ClassDef {

	using creator_func = Base* (*)(Args...);

	ClassDef(std::string name, creator_func creator)
		: name(name), creator_func(creator)
	{
	}

	creator_func creator;
	static std::map<string, ClassDef> map;
};

class BaseEntity {
public:
	using ClassDef = ::ClassDef<BaseEntity, int, int>;

	virtual ClassDef& classDef() const = 0;
};


class Entity : public BaseEntity {
public:
	using ClassDef = BaseEntity::ClassDef;

	virtual ClassDef& classDef() const
	{
		auto creator = [] (int a, b) {return new Entity(a, b);};
		static ClassDef{"Entity", this, creator};

		return ClassDef;
	}

	Entity(int a, int b) { }
};


class Visual {
public:
	using ClassDef = ::ClassDef<Visual>

	virtual ClassDef& classDef() const = 0;
};

class Model : public Visual {
public:
	using ClassDef = Visual::ClassDef;

	virtual ClassDef& classDef() const
	{
		static ClassDef{"Model", this, [] {return new Model;}};

		return ClassDef;
	}

	Model() { }
};


template<class Base>
Archivable* Archiver::UnarchivePolymorphic(std::string typeName, Args... args)
{
	auto classdef = Base::ClassDef::map[typeName];

	auto obj = classdef.creator(args...);
	obj.unarchive(*this);

	return obj;
}


