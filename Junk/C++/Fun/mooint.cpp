struct Int {

	Int& operator/=(Int const& other)
	{
		int x = 0;
		while (i >= other.i) {
			i -= other.i;
			++x;
		}
		i = x;
		return *this;
	}
	int i;
};
