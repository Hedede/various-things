#include <vector>
#include <thread>

template<typename T>
struct Vector3d {
	T x[4];

	Vector3d(T x, T y, T z)
		: x{x,y,z}
	{
	}

	Vector3d<T>& operator += (T v)
	{
		x[0] += v;
		x[1] += v;
		x[2] += v;
		x[3] += v;
		return *this;
	}

	Vector3d<T>& operator += (Vector3d<T>& oth)
	{
		x[0] += oth.x[0];
		x[1] += oth.x[1];
		x[2] += oth.x[2];
		x[3] += oth.x[3];
		return *this;
	}

	T length() {
		return x[0] + x[1] + x[2] + x[3];
	}
};

extern void nonopt(Vector3d<float>& vec);

void doostuff()
{
	Vector3d<float> vec1(0,0,0);
	Vector3d<float> vec2(1.0e-9,0.99e-9,1.03e-9);

	while (true) {
		if (vec1.length() < 1.0e5) {
			vec1 += vec2;
			nonopt(vec1);
		} else {
			vec1 += -(1.0e6 + 1.0e-3);
			nonopt(vec1);
		}
	}
}


int main()
{
	std::vector<std::thread> threds(4);

	for (auto& thr : threds)
		thr = std::thread(doostuff);

	for (auto& thr : threds)
		thr.join();
}
