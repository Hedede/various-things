class Outer {
	struct Inner { } i;
public:
	Inner& inner() { return i; }
};

int main()
{
	Outer o;
	Outer::Inner  iv = o.inner();  // error: 'Inner' is a private member of 'Outer'
	Outer::Inner& ir = o.inner();  // error: 'Inner' is a private member of 'Outer'
	Outer::Inner* ip = &o.inner(); // error: 'Inner' is a private member of 'Outer'
	auto          ia = o.inner();  // OK
}
