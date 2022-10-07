class Poeben {
public:
	int a;
protected:
	int b;
};

class px6880 : public Poeben {
public:
	using Poeben::b;
protected:
	int c;
};

class px6880c : public px6880 {
public:
	using px6880::c;
};
int main()
{
	px6880c poeben;
	poeben.a = poeben.b = poeben.c = 10;

	px6880* poeben2 = new px6880();

	int x = (delete poeben2, 10);

	return x;
}
