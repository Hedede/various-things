	int main ()
	{
		if (({int i = 0; ++i; i;})) {
			return 0;
		}
		return 1;
	}
