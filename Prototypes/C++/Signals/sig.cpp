#include <set>
#include <functional>

struct signal;
struct slot;

struct connection {
	signal* sender;
	slot* receiver;

	std::function<void()> callback;

	void disconnect();
};

struct sigslot_base {
	std::set<connection*> connections;

	~sigslot_base()
	{
		for (auto conn : connections)
			conn->disconnect();
	}
};

struct slot : sigslot_base { };

struct signal : sigslot_base {
	template<class T>
	connection& connect(T& obj, void (T::*  func  )())
	{
		auto conn = new connection{this, &obj, std::bind(func, &obj)};

		this->connections.insert(conn);
		obj.connections.insert(conn);

		return *conn;
	}

	void operator()()
	{
		for (auto conn : connections)
			conn->callback();
	}
};

void connection::disconnect()
{
	sender->connections.erase(this);
	receiver->connections.erase(this);
	delete this;
}

#include <iostream>
struct test : slot {
	void on_signal()
	{
		std::cout << "blyad, I replaced somebodys 7500 lines of code with 50\n";
	}
};

int main()
{
	signal A;
	{
		test B;

		A.connect(B, &test::on_signal);

		A();
	}
	A();
}
