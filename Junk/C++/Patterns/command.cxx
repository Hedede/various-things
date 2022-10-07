#include <iostream>
#include <functional>
#include <vector>

using std::vector;
using std::function;
using std::cout;
struct couter {
	couter(size_t id) : id{id} { cout << "couter orr" << id << '\n'; }
	~couter() { cout << "couter died" << id << "\n"; }
	size_t id;
};


struct command {
	std::function<void()> redo;
	std::function<void()> undo;
};


struct edidor {

	void redo()
	{
		if (!redos.empty()) {
			auto cmd = redos.back();
			redos.pop_back();
			cmd().redo();
			undos.push_back(cmd);
		}
	}

	void undo()
	{
		if (!undos.empty()) {
			auto cmd = undos.back();
			undos.pop_back();
			cmd().undo();
			redos.push_back(cmd);
		}
	}


	void add(function<command()> cmd)
	{
		cmd().redo();
		undos.push_back(cmd);
	}

	std::vector<function<command()>> redos;
	std::vector<function<command()>> undos;
	size_t pos = 0;
};


edidor ed;


void commando()
{
	couter* ctr = nullptr;
	static size_t gid = 0;
	size_t id = ++gid;
	ed.add( [=] () mutable {
		return command{
		[&] { ctr = new couter{id}; },
		[&] { delete ctr; }
		};
	});
}

int main()
{
	commando();
	commando();

	ed.undo();
	ed.undo();
	ed.undo();
	ed.redo();
	ed.redo();
	ed.redo();
	ed.undo();
	ed.redo();
}
