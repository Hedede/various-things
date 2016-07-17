#include <iostream>

struct Player
{
	float WorldPosition[4];
	int SpawnWeapons[10];
	int CurrentHealth;
	int CurrentArmor;
};

void SetSpawnPosition(Player *player_struct, float x, float y, float z, float ang)
{
	player_struct->WorldPosition[1]=x;
	player_struct->WorldPosition[2]=y;
	player_struct->WorldPosition[3]=z;
	player_struct->WorldPosition[4]=ang;
}

int main ()
{
	int a = 5;
	int *pa = &a;
	int araray[10]={1,2,3,4,5,6,7,8,9,10};
	*pa = 430;
	std::cout << "orara? " << a << "\n" << "araray size: " << sizeof(araray) << "\n";

	struct Player Sotrudneg;
	Sotrudneg.CurrentHealth=100;
	Sotrudneg.CurrentArmor=0;
	//for(int i; i<4; i++) { Sotrudneg.WorldPosition[i]=0.0; }
	SetSpawnPosition(&Sotrudneg, 0.0, 0.0, 0.0, 0.0);
	for(int i; i<10; i++) { Sotrudneg.SpawnWeapons[i]=0; }
} 
