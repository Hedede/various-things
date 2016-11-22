struct hrenuct {
	int x() { return 1; }
protected:
	int x() const { return 2; }
};

struct heruct : hrenuct {
	int y() const { return hrenuct::x(); }
};

int main()
{
	hrenuct const h;
	heruct const e;
	auto x = h.x();
	auto y = e.y();
}
