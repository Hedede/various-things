#include <chrono>
#include <iostream>

#include <hrengin/math/Matrix4.h>

int main ()
{
	hrengin::Matrix4<double> A(
		1,2,3,4,
		5,6,7,8,
		9,1,1,2,
		3,4,5,1);
		
	hrengin::Matrix4<double> B;
	hrengin::Matrix4<double> C;

	std::chrono::high_resolution_clock::time_point t1 = std::chrono::high_resolution_clock::now();
	int i = 0;
	for( ; i < 100000; ++i) {
		A.getInverse(B);
		A = B;
		C = C + B;
	}
	A.getInverse(B);
	std::chrono::high_resolution_clock::time_point t2 = std::chrono::high_resolution_clock::now();

	hrengin::Matrix4<double> I = A * B;

	auto duration = std::chrono::duration_cast<std::chrono::microseconds>( t2 - t1 ).count();

	std::cout << duration <<std::endl;
	
	for(int i=0; i<4; ++i) {
		for(int j=0; j<4; ++j) {
			std::cout << I[i][j] << " ";	
		}
		std::cout << std::endl;
	}

	for(int i=0; i<4; ++i) {
		for(int j=0; j<4; ++j) {
			std::cout << C[i][j] << " ";	
		}
		std::cout << std::endl;
	}

	std::cin.get();
	return 0;
}
