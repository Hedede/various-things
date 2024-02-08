#ifndef __SCANNER_H__
#define __SCANNER_H__

#include "fat32.h"

typedef struct 
{
	unsigned long sectorsNumber;
	unsigned long fatBeginLba;
	unsigned long fatSize;
	unsigned long clusterBeginLba;
	unsigned long sectorsPerCluster;
	unsigned long rootDirFirstCluster;
} ScannablePartition;

void addPartition(HANDLE drive, long lbaBegin, long sectorsNumber);
ScannablePartition * getPartition(char partid);
long getPartitionsNumber();
void scanPartition(HANDLE drive, char part);

#endif