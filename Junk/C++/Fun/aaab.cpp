#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include <sys/mman.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <new>
#include <iostream>

class Vehicle {
public:
	Vehicle(int wh)
		: wheels(wh)
	{
	}

	virtual int getWheels()
	{
		return wheels;
	}
private:
	int wheels;
};

void* makeMmap(size_t size, int fd)
{
	auto map = mmap(NULL, size, PROT_WRITE | PROT_READ | PROT_EXEC, MAP_SHARED, fd, 0);

	if (map == MAP_FAILED) {
		perror("mmap");
		exit(EXIT_FAILURE);
	}

	return map;
}

int main(int argc, char *argv[])
{
	int fd;
	void* map;
	Vehicle veh2(1); // kostul

	if (argv[2][0] == 'n') {
		fd = open(argv[1], O_RDWR | O_CREAT, mode_t(0666));
		ftruncate(fd, 512);

		map = makeMmap(512, fd);

		Vehicle* veh = new (map) Vehicle(6);

		std::cout << veh->getWheels() << std::endl;
	} else if(argv[2][0] == 'l') {
		fd = open(argv[1], O_RDWR);

		struct stat st;
		fstat(fd, &st);
		
		map = makeMmap(st.st_size, fd);

		Vehicle* veh = (Vehicle*)map;
		memcpy(veh, &veh2, sizeof(void*)); // KOSTUL: copy vtable pointer

		std::cout << veh->getWheels() << std::endl;
	}
	close(fd);

	return 0;
}
