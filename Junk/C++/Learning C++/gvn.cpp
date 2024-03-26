#define _CRT_SECURE_NO_WARNINGS
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <iostream>



void test1 ()
{
	char a[128];
	int b[26];
	int i = 0; 
	char max = 0;	
	do {
		b[i] = 0;
	} while(i++ < 26);

	fgets (a, 128, stdin);

	i = 0;
	do {
		if((a[i] >= 'a') && (a[i] <= 'z')) b[a[i] - 'a']++;
		if((a[i] >= 'A') && (a[i] <= 'Z')) b[a[i] - 'A']++;
	} while(a[i++] != '.');
	
	i = 0;
	do {
		if(b[i] > b[max]) 
		{
			max = i;
		}
	} while(i++ < 26);

	printf("%c %d",'A'+max,b[max]);
}

void test2 ()
{
	char a[128];
	int b[26];
	int i = 0;
	do {
		b[i] = 0;
	} while(i++ < 26);

	fgets (a, 128, stdin);

	i = 0;
	do {
		if((a[i] >= 'a') && (a[i] <= 'z')) b[a[i] - 'a']++;
	} while(a[i++] != '.' && a[i++] != '\0');
	
	i = 0;
	do {
		if(b[i] > 0) printf("%c%d\n",'a'+i,b[i]);
	} while(i++ < 26);

}


int main1 ()
{
	char a[10];
	test2();

	scanf("%s",&a);

	return 0;
}