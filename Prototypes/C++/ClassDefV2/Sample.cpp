// ----------------------------------------------------------------
// Current.h
// (для сравнения)
// ClassDef содержит конструктор и указатель на базовый
// Если класс абстрактный, то вместо конструктора передается nullptr
// Если класс базовый, то вызывается ::base,
// Если деривед — вызывается         ::derived,
// с парметром <ParentClass>
//
#include <aw/archive/Binary.h>

struct Base {
	using ClassDef = aw::arc::ClassDef<Base, Base*(std::string)>;

	static ClassDef classdef;
	virtual ClassDef& classDef() const
	{
		return classdef;
	}

	virtual void save(OutputArchive& arc) const
	{
		arc("name", name);
	}

	virtual void load(InputArchive& arc) = 0;

	Base(std::string ent_name)
		: name(ent_name)
	{}

	std::string name
};

Base::ClassDef Base::classdef = Base::ClassDef::base("Base", nullptr);

void Base::load(InputArchive& arc)
{
	char x;
	arc("var1", x);
}

struct Derived : Base {
	using ClassDef = Base::ClassDef;

	static ClassDef classdef;
	virtual ClassDef& classDef() const
	{
		return classdef;
	}

	using Base::Base;

	virtual void save(OutputArchive& arc) const
	{
		Base::save(arc);
		arc("var2", var2);
	}

	virtual void load(InputArchive& arc)
	{
		Base::load(arc);
		arc("var2", var2);
	}
	
	char var2;
};

Base::ClassDef Derived::classdef = Base::ClassDef::derived<Base>(
	"Derived",
	[] (std::string c) -> Base* {
		return new Derived(c);
	}
);

struct Derived2 : Derived {
	using ClassDef = Base::ClassDef;

	static ClassDef classdef;
	virtual ClassDef& classDef() const
	{
		return classdef;
	}

	using Derived::Derived;

	virtual void save(OutputArchive& arc) const
	{
		Derived::save(arc);
		arc("var2", var2);
	}

	virtual void load(InputArchive& arc)
	{
		Derived::load(arc);
		arc("var2", var2);
	}
	
	char var2;
};

Base::ClassDef Derived2::classdef = Base::ClassDef::derived<Derived>(
	"Derived2",
	[] (std::string c) -> Base* {
		return new Derived2(c);
	}
);

int main()
{
	aw::arc::InputBinary arc;

	// в какой-нибудь функции, скажем load_game()
	{
		Base* entity;

		arc("entity", entity,
		    "some_mandatory_but_not_saved_in_savegame_bullshit");
	}
}

// ----------------------------------------------------------------
// CurrentImporved.h
// Слегка подчищенный интерфейс, с двумя отличиями
// — в конструктор классдефа передается только класснейм,
// в create автоматически создаётся обёртка конструктора, и
// автоматически определяется, является ли класс абстрактным
// — нет ::base, вместо этого в параметр <ParentClass> передаётся
// сам класс
#include <aw/archive/Binary.h>

struct Base {
	using ClassDef = aw::arc::ClassDef<Base, std::string>;

	static ClassDef classdef;
	virtual ClassDef& classDef() const
	{
		return classdef;
	}

	virtual void save(OutputArchive& arc) const
	{
		arc("name", name);
	}

	virtual void load(InputArchive& arc) = 0;

	Base(std::string ent_name)
		: name(ent_name)
	{}

	std::string name
};

Base::ClassDef Base::classdef = Base::ClassDef::create<Base>("Base")

void Base::load(InputArchive& arc)
{
	char x;
	arc("var1", x);
}

struct Derived : Base {
	using ClassDef = Base::ClassDef;

	static ClassDef classdef;
	virtual ClassDef& classDef() const
	{
		return classdef;
	}

	using Base::Base;

	virtual void save(OutputArchive& arc) const
	{
		Base::save(arc);
		arc("var2", var2);
	}

	virtual void load(InputArchive& arc)
	{
		Base::load(arc);
		arc("var2", var2);
	}
	
	char var2;
};

Base::ClassDef Derived::classdef = Base::ClassDef::create<Base>("Derived");

struct Derived2 : Derived {
	using ClassDef = Base::ClassDef;

	static ClassDef classdef;
	virtual ClassDef& classDef() const
	{
		return classdef;
	}

	using Derived::Derived;

	virtual void save(OutputArchive& arc) const
	{
		Derived::save(arc);
		arc("var2", var2);
	}

	virtual void load(InputArchive& arc)
	{
		Derived::load(arc);
		arc("var2", var2);
	}
	
	char var2;
};

Base::ClassDef Derived2::classdef = Base::ClassDef::create<Derived>("Derived2");

// ----------------------------------------------------------------
// Registration.h
// Здесь классдефы заменены макросами, регистрирующими класс в синглтоне
#include <aw/archive/Binary.h>

struct Base {
#if 1
	static constexpr char const* class_name = "Base";
	virtual char const* className() const
	{
		return class_name;
	}

#else // if c++ reflection proposal is accepted
	virtual std::string class_name() const
	{
		return {std::meta::get_name_v<reflexpr(Base)>};
	}
#endif

	virtual void save(OutputArchive& arc) const
	{
		arc("name", name);
	}

	virtual void load(InputArchive& arc) = 0;

	Base(std::string ent_name)
		: name(ent_name)
	{}

	std::string name
};

// параметры: Class,  Base,   ctor_args...
AW_REGISTER_CLASS(Base,   Base,   std::string);

void Base::load(InputArchive& arc)
{
	char x;
	arc("var1", x);
}

struct Derived : Base {
	static constexpr char const* class_name = "Derived";

	virtual char const* className() const
	{
		return class_name;
	}

	using Base::Base;

	virtual void save(OutputArchive& arc) const
	{
		Base::save(arc);
		arc("var2", var2);
	}

	virtual void load(InputArchive& arc)
	{
		Base::load(arc);
		arc("var2", var2);
	}
	
	char var2;
};

AW_REGISTER_CLASS(Derived,   Base,   std::string);

struct Derived2 : Derived {
	// идентично ручному объявлению class_name
	// и className
	// (для тех кто предпочитает макросы)
	AW_DECLARE_CLASS_NAME(Derived2);

	using Derived::Derived;

	virtual void save(OutputArchive& arc) const
	{
		Derived::save(arc);
		arc("var2", var2);
	}

	virtual void load(InputArchive& arc)
	{
		Derived::load(arc);
		arc("var2", var2);
	}
	
	char var2;
};

AW_REGISTER_CLASS(Derived2,   Base,   std::string);


// ----------------------------------------------------------------
// Factory.h
// Создание классов — на стороне пользователя
#include <aw/archive/Binary.h>


struct Base {
	static constexpr char const* class_name = "Base";
	virtual char const* className() const
	{
		return class_name;
	}

	using Creator = struct Factory;

	virtual void save(OutputArchive& arc) const
	{
		arc("name", name);
	}

	virtual void load(InputArchive& arc) = 0;

	Base(std::string ent_name)
		: name(ent_name)
	{}

	std::string name
};

void Base::load(InputArchive& arc)
{
	char x;
	arc("var1", x);
}

struct Derived : Base {
	AW_DECLARE_CLASS_NAME(Derived);

	using Base::Base;

	virtual void save(OutputArchive& arc) const
	{
		Base::save(arc);
		arc("var2", var2);
	}

	virtual void load(InputArchive& arc)
	{
		Base::load(arc);
		arc("var2", var2);
	}
	
	char var2;
};

struct Derived2 : Derived {
	AW_DECLARE_CLASS_NAME(Derived2);

	using Derived::Derived;

	virtual void save(OutputArchive& arc) const
	{
		Derived::save(arc);
		arc("var2", var2);
	}

	virtual void load(InputArchive& arc)
	{
		Derived::load(arc);
		arc("var2", var2);
	}
	
	char var2;
};

struct Factory {
	template<class... Args>
	static Base* create(std::string name, Args... args)
	{
		using std::string_literals;
		if (name == Base::class_name)
			return new Base(args...);
		if (name == Derived::class_name)
			return new Derived(args...);
		if (name == Derived2::class_name)
			return new Derived2(args...);
		return nullptr;
	}
};
