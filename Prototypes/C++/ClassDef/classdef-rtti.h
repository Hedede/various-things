#include <archive>
struct ClassDef {
	template<class T, class F>
	ClassDef(std::string name, T exemplar, F creator)
		: classname(name), creator(creator), ty(typeid(creator))
	{
		classdefs[name] = this;
	}

	void* creator;
	std::type_index ty;
	std::string classname;

	static std::map<std::string,ClassDef*> classdefs;
};

class Archivable {
public:
	virtual ClassDef& classDef() const = 0;
};


class Object : public Archivable {
public:
	virtual ClassDef& classDef() const
	{
		auto creator = [] (int value) {return new Object(value);};
		static ClassDef{"Archivable", this, creator};
	}

	Object(int value) { }
};


template<class... Args>
Archivable* Archiver::UnarchivePolymorphic(std::string typeName, Args... args)
{
	auto clasdef = ClassDef::classdefs[typeName];

	assert(typeid(Base* (*) (Args...)) == clasdef.ty);

	
	Archivable* a = clasdef.creator(args...);
	a->unarchive(*this);
	return a;
}
