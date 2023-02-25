/*
 * Copyright (C) 2023  Hedede <haddayn@gmail.com>
 *
 * License LGPLv3 or later:
 * GNU Lesser GPL version 3 <http://gnu.org/licenses/lgpl-3.0.html>
 * This is free software: you are free to change and redistribute it.
 * There is NO WARRANTY, to the extent permitted by law.
 */
#ifndef aw_utility_factory_registry_h
#define aw_utility_factory_registry_h

#include <aw/utility/static_object.h>

#include <map>
#include <string>
#include <memory>
#include <cassert> // TODO: replace with aw_assert

namespace aw {
/*!
 * Serves as a single point for registering factories for a class
 */
template<typename T, typename... Params>
class factory_registry {
	struct factory
	{
		virtual ~factory() = default;
		virtual std::unique_ptr<T> create(Params&&...) const = 0;
	};

	struct factory_map : std::map<std::string, factory*, std::less<>> {};

	static void register_factory(std::string name, factory* factory)
	{
		factory_map& named_factories = static_object<factory_map>::instance();

		auto res = named_factories.try_emplace(std::move(name), factory);
		assert(res.second);
	}

public:
	struct entry {
		template<typename Functor>
		entry(std::string name, Functor&& functor)
		{
			// TODO: maybe just use std::function?
			struct wrapper : factory {
				Functor f;
				std::unique_ptr<T> create(Params&&... params) const override
				{
					return f(std::forward<Params>(params)...);
				}
			};

			register_factory(
				std::move(name),
				new wrapper(std::forward<Functor>(functor)));
		}
	};

	template<typename U>
	struct register_class {
		register_class(std::string name) // TODO: use type_name by default
		{
			struct concrete_factory : factory {
				std::unique_ptr<T> create(Params&&... params) const override
				{
					return std::make_unique<U>(std::forward<Params>(params)...);
				}
			};

			register_factory(std::move(name), new concrete_factory);
		}
	};

	static std::unique_ptr<T> create(std::string_view identifier, Params&&... params)
	{
		factory_map& named_factories = static_object<factory_map>::instance();

		auto it = named_factories.find(identifier);
		if (it == named_factories.end())
			return nullptr;

		return it->second->create(std::forward<Params>(params)...);
	}
};

} // namespace aw
#endif//aw_utility_factory_registry_h
