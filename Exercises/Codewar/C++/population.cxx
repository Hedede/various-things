struct Arge {
	static int nbYear(int p0, double percent, int aug, int p)
	{
		int years = 0;
		auto next_pop = [](int p0, double perc, int aug) {
			return int(p0 * (1+perc/100.0)) + aug;
		};
		for (; p0 < p; ++years)
			p0 = next_pop(p0, percent, aug);
		return years;
	}
};
