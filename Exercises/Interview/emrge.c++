#include <vector>

void Merge(std::vector<int>& A, size_t p, size_t q, size_t r)
{
	auto B = A;

	auto p1 = p;
	auto p2 = q;

	for (auto i = p; i < r; ++i)
	{
		if (p2 == r) {
			A[i] = B[p1++];
			continue;
		}
		if (p1 == q) {
			A[i] = B[p2++];
			continue;
		}

		// if ((B[p1] < B[p2] || p2 == r) && p1 != q)
		if (B[p1] < B[p2])
			A[i] = B[p1++];
		else
			A[i] = B[p2++];
	}
}

void Sort(std::vector<int>& A, size_t p, size_t r)
{
	if (r - p < 2)
		return;
	auto q = (p+r)/2;
	Sort(A,p,q);
	Sort(A,q+1,r);
	Merge(A,p,q,r);
}

#include <iostream>
void Print(std::vector<int>& A, size_t p, size_t r)
{
	std::cout << A[p];
	for(size_t i = p+1; i < r; ++i)
		std::cout << ' ' << A[i];
	std::cout << '\n';
}

int main()
{
	std::vector<int> A = {5,2,4,6,1,3,2,6};

	Print(A,0,A.size());
	Sort(A,0,A.size());
	Print(A,0,A.size());
}
