template<typename T>
class List {
	T* data;
	List<T>* next;

public:
	// other methods

	T* Get(int index = 0)
	{
		int i = 0;
		for (List<T>* list = this; list; list = list->next)
			if (i++ == index)
				return list->data;
		return 0;
	}

	int Size() const
	{
		int size = 0;
		for (const List<T>* list = this; list; list = list->next)
			++size;
		return size;
	}

	void Remove(T* value)
	{
		for (List<T>* list = this; list; list = list->next)
		{
			if (list->data == value)
			{
				T* data       = list->data;
				List<T>* next = list->next;
				if (next)
				{
					list->data = next->data;
					list->next = next->next;
					delete next;
				}
				else
				{
					list->data = 0;
				}
				delete data;
			}
		}
	}
};


void somewhere_else_in_the_code()
{
	List<Someclass> list;


	for (int i = 0; i < list.Size(); ++i) {
		Someclass* s = list.Get(i);
		if (s->something == I_think_there_was_a_check())
		{
			for (int j = 0; j < list.Size(); ++j)
			{
				if (list.Get(j) == s)
				{
					list.Remove(s);
				}
			}
		}
	}

}
