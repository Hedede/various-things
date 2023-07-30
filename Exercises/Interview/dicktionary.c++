#include <stdexcept>

template<class TKey, class TValue>
class Dictionary
{
public:
	virtual ~Dictionary() = default;

	virtual const TValue& Get(const TKey& key) const = 0;
	virtual void Set(const TKey& key, const TValue& value) = 0;
	virtual bool IsSet(const TKey& key) const = 0;
};

template<class TKey>
class NotFoundException : public std::exception {
public:
	virtual const TKey& GetKey() const noexcept = 0;
};

//----------------------------------------
template<class TKey>
class KeyNotFoundException : public NotFoundException<TKey> {
	TKey key;
public:
	KeyNotFoundException(TKey key)
		: key(std::move(key))
	{}

	const TKey& GetKey() const noexcept override
	{
		return key;
	}
};


//----------------------------------------
template<class TBase>
class BaseDictionary : Dictionary<typename TBase::key_type, typename TBase::mapped_type> {
	using TKey = typename TBase::key_type;
	using TValue = typename TBase::mapped_type;
	TBase _map;
public:
	~BaseDictionary() override = default;

	const TValue& Get(const TKey& key) const override
	{
		auto iter = _map.find(key);
		if (iter == _map.end())
			throw KeyNotFoundException<TKey>( key );
		return iter->second;
	}

	void Set(const TKey& key, const TValue& value) override
	{
		_map[key] = value;
	}

	bool IsSet(const TKey& key) const override
	{
		return _map.count(key) > 0;
	}
};

//----------------------------------------
#include <map>
#include <unordered_map>
template<class TKey, class TValue>
using UnorderedMapDictionary = BaseDictionary<std::unordered_map<TKey, TValue>>;

template<class TKey, class TValue>
using OrderedMapDictionary = BaseDictionary<std::map<TKey, TValue>>;


//----------------------------------------
#include <vector>
#include <algorithm>
#include <utility>
template<class TKey, class TValue>
class FlatDictionary : Dictionary<TKey, TValue>
{
	using TKeyValuePair = std::pair<TKey,TValue>;
	std::vector<TKeyValuePair> _map;

	static bool Compare(TKeyValuePair const& pair, TKey const& key)
	{
		return pair.first < key;
	}

	auto Find(TKey const& key)
	{
		return std::lower_bound(_map.begin(), _map.end(), key, Compare);
	}

	auto Find(TKey const& key) const
	{
		return std::lower_bound(_map.begin(), _map.end(), key, Compare);
	}

public:
	~FlatDictionary() override = default;

	const TValue& Get(const TKey& key) const override
	{
		auto iter = Find(key);
		if (iter == _map.end())
			throw KeyNotFoundException<TKey>( key );
		return iter->second;
	}

	void Set(const TKey& key, const TValue& value) override
	{
		auto iter = Find(key);
		if (iter == _map.end() || key != iter->first)
			_map.emplace(iter, key, value);
		else
			iter->second = value;
	}

	bool IsSet(const TKey& key) const override
	{
		auto iter = Find(key);
		if (iter == _map.end())
			return false;
		return iter->first == key;
	}
};


#define BOOST_TEST_MODULE dictionary test
#include <boost/test/included/unit_test.hpp>
#include <boost/mpl/list.hpp>

using Dictionaries = boost::mpl::list<
	UnorderedMapDictionary<std::string, int>,
	OrderedMapDictionary<std::string, int>,
	FlatDictionary<std::string, int>>;

BOOST_AUTO_TEST_CASE_TEMPLATE( test_dictionary, TDictionary, Dictionaries )
{
	TDictionary dic;

	BOOST_CHECK( dic.IsSet("abc") == false );
	BOOST_CHECK( dic.IsSet("bca") == false );

	dic.Set("abc", 42);

	BOOST_CHECK( dic.Get("abc") == 42 );

	BOOST_CHECK( dic.IsSet("abc") == true );
	BOOST_CHECK( dic.IsSet("bca") == false );

	BOOST_CHECK_NO_THROW( dic.Get("abc") );
	BOOST_CHECK_THROW( dic.Get("bca"), KeyNotFoundException<std::string> );

	BOOST_CHECK( dic.IsSet("abc") == true );
	BOOST_CHECK( dic.IsSet("bca") == false );

	dic.Set("abc", 77);
	dic.Set("bca", 88);

	BOOST_CHECK( dic.Get("abc") == 77 );
	BOOST_CHECK( dic.Get("bca") == 88 );

	dic.Set("abb", 80);
	dic.Set("aba", 90);

	BOOST_CHECK( dic.Get("abc") == 77 );
	BOOST_CHECK( dic.Get("abb") == 80 );
	BOOST_CHECK( dic.Get("aba") == 90 );
	BOOST_CHECK( dic.Get("bca") == 88 );

	dic.Set("bcb", 100);

	BOOST_CHECK( dic.Get("bcb") == 100 );
}
