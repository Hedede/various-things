#include <iterator>
template<typename Iter>
struct MIter;

struct MagicIterator {
	template<typename T>
	void advance(T& vec) const { cast(vec).advance(); }
	template<typename T>
	bool finished(T& vec) const { return cast(vec).finished(); }

	template<typename T>
	typename T::reference get(T& vec) const
	{ return cast(vec).get(); }

	template<typename T>
	MIter<typename T::iterator> const& cast(T&) const
	{
		return *static_cast<MIter<typename T::iterator> const*>(this);
	}
	template<typename T>
	MIter<typename T::const_iterator> const& cast(T const&) const
	{
		return *static_cast<MIter<typename T::const_iterator> const*>(this);
	}
	operator bool() { return false; }
};

template<typename Iter>
struct MIter : MagicIterator {
	mutable Iter it;
 	mutable Iter end;

	typedef typename std::iterator_traits<Iter>::reference reference;

	MIter(Iter it, Iter end) : it(it), end(end) {}

	void advance()  const { ++it; }
	bool finished() const { return it == end; }
	reference get() const { return *it; }
};

template<typename T>
MIter<typename T::iterator> GetMIter(T& vec)
{
	return MIter<typename T::iterator>(vec.begin(), vec.end());
}

template<typename T>
MIter<typename T::const_iterator> GetMIter(T const& vec)
{
	return MIter<typename T::const_iterator>(vec.begin(), vec.end());
}

#define WITH( stmt )\
	if (bool fuck = false); else for( stmt; !fuck; fuck = true )

#define MAGIC_FOR(var, vec)\
	for (MagicIterator const& iter = GetMIter(vec); !iter.finished(vec); iter.advance(vec))\
	WITH(var = iter.get(vec))

#include <map>
#include <iostream>
#include <string>
int main()
{
	typedef std::map<std::string, int> map;
	map m;
	m["a"] = 22;
	m["b"] = 7;
	m["c"] = 91;

	MAGIC_FOR(map::value_type val,  m)
		std::cout << val.first << ':' << val.second << '\n';
}
