template<typename T>
struct base {
	base(T,bool) {}
};

using babase = base<int>;

struct derived : babase
{
	using babase::babase;
};

int main()
{
	derived der(1,true);
}
