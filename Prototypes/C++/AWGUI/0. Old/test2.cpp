#include <memory>
#include <vector>
#include <iostream>
#include <algorithm>


struct tester {
	// чилдс
	std::vector<std::unique_ptr<tester>> v;

	// ту // what the fuck is this comment?!
	typedef std::unique_ptr<tester> ptr;
	typedef decltype(v)::iterator iterator;

	iterator begin()
	{
		return std::begin(v);
	}
	iterator end()
	{
		return std::end(v);
	}

	void add_child(ptr child) {
		v.push_back(std::move(child));
		// child->setParent(this);
	}

	ptr remove_child(tester* child) {
		auto boolexpr = [&child] (ptr const& p) {
					return p.get() == child;
				};
		iterator ii = std::find_if(
				begin(),
				end(),
				boolexpr);
		if (ii != end()) {
			// ii->get()->removeParent(this);
			auto tmp = std::move(*ii);
			v.erase(ii);
			return std::move(tmp);
		}
		return nullptr;
	}
	

	int i;
};


int main ()
{
	tester t;
	t.add_child(std::make_unique<tester>());
	t.add_child(std::make_unique<tester>());
	t.add_child(std::make_unique<tester>());
	t.add_child(std::make_unique<tester>());
	t.add_child(std::make_unique<tester>());

	for (auto const& test : t)
	{
		test->i++;
		std::cout << ".";
	}
	std::cout << std::endl;
	
	t.remove_child((*t.begin()).get());
	for (auto const& test : t)
	{
		test->i++;
		std::cout << ".";
	}
	std::cout << std::endl;

	t.remove_child((*t.begin()).get());
	for (auto const& test : t)
	{
		test->i++;
		std::cout << ".";
	}
	std::cout << std::endl;
}
