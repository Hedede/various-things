//template<typename Operator>
struct accessor
{
template<typename T>
	void get();
}

template<typename T>
class operations {
	//friend accessor<operations<T>>;
	friend accessor;

	void get() {}
};

template<typename T>
	void get()
	{
		operations<T>::get();
	}

void used()
{
	[[maybe_unused]] int i = 0;
}
