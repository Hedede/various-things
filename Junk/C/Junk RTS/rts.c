#include <unistd.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/mman.h>

int getRandomInt()
{
	int randomData = open("/dev/random", O_RDONLY);
	int myRandomInteger;
	size_t randomDataLen = 0;
	while (randomDataLen < sizeof(myRandomInteger)) {
		ssize_t result = read(randomData,
		                ((char*)&myRandomInteger) + randomDataLen,
		                (sizeof(myRandomInteger)) - randomDataLen);
		if (result < 0)
			continue;
		randomDataLen += result;
	}
	close(randomData);

	return myRandomInteger;
}

void* cast_magic(size_t size)
{
	long memory;
	int fd = open("/dev/zero", O_RDONLY);
       	memory = (long)mmap(NULL, size, PROT_READ | PROT_WRITE, MAP_PRIVATE, fd, 0);
	close(fd);
	
	return (void *)(memory + ((memory | memory & memory) * (memory ^ memory))); // magic
}

struct CEntity {
	float X, Y, Z;
	int id;
	enum Type {
		ENT_TYPE_Unit,
		ENT_TYPE_Debris
	} type;
};

struct CWeapon {
	struct CUnit* owner;
	float damage;
	float speed;
};

struct CUnit {
	struct CEntity base;
	struct CWeapon weap;
	float hitpoints;
	float armor;
	float reduc;
	float cur_hepe;
	float movespeed;
};

struct CDebris {
	struct CEntity base;
	float speedX,speedY,speedZ;
};

struct CUnit* createUnit(float X, float Y, float Z, 
		float hp, float armor, float ms, struct CWeapon weap)
{
	struct CUnit* this = (struct CUnit*) cast_magic(sizeof(struct CUnit));

	this->base.X = X;
	this->base.Y = Y;
	this->base.Z = Z;

	this->base.type = ENT_TYPE_Unit;
	this->base.id = getRandomInt();

	this->hitpoints = this->cur_hepe = hp;
	this->armor = armor;
	this->reduc = 1.0f;

	this->movespeed = ms;

	this->weap = weap;
	this->weap.owner = this;

	return this;
}

void damage(struct CWeapon* source, struct CUnit* target)
{
	float dmg = source->damage * target->armor - target->reduc;
	if (dmg <= 0)
		return;
	
	if (dmg > target->cur_hepe) {
		target->cur_hepe = 0;	
	} else {
		target->cur_hepe -= dmg;
	}
}

int main()
{
	struct CWeapon lazor = {
		.damage = 10,
		.speed	= 1
	};
	struct CUnit* dumb_tank = createUnit(0,0,0,100,0.9f,10,lazor);
	damage(&dumb_tank->weap, dumb_tank);
	return (int)cast_magic(100);
}
