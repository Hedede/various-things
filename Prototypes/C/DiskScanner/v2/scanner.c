
#include "stdlib.h"
#include "stdio.h"
#include "stdbool.h"
#include "assert.h"

#include "mbr.h"
#include "fat32.h"
#include "scanner.h"

struct ScannablePartitions
{
	char partitionsNumber;
	ScannablePartition partitions[4];
};

struct ScannablePartitions scannablePartitions;

void addPartition(HANDLE drive, long lbaBegin, long sectorsNumber)
{
	assert(drive != INVALID_HANDLE_VALUE);
		
	LARGE_INTEGER byteOffset;
	byteOffset.QuadPart = lbaBegin * 512;
	
	bool setPointer = SetFilePointerEx(drive,byteOffset,NULL,FILE_BEGIN);

	if(!setPointer)
	{
		return;
	}
	
	Vbr fatVbr;
	
	long bytesread;
	bool readFileSuccess = ReadFile(drive, &fatVbr, sizeof(Vbr), &bytesread, NULL);
		
	if(readFileSuccess)
	{
		if(!scannablePartitions.partitionsNumber)
		{
			scannablePartitions.partitionsNumber = 0;
		}
		
		long i = scannablePartitions.partitionsNumber;
		
		scannablePartitions.partitions[i].sectorsNumber = sectorsNumber;
		scannablePartitions.partitions[i].fatBeginLba = lbaBegin + *(WORD*)fatVbr.bpb.reservedSectors;
		scannablePartitions.partitions[i].fatSize = *(DWORD*)fatVbr.bpb.bigSectorsPerFAT;
		scannablePartitions.partitions[i].clusterBeginLba = lbaBegin + *(WORD*)fatVbr.bpb.reservedSectors + (fatVbr.bpb.fatCopies * *(DWORD*)fatVbr.bpb.bigSectorsPerFAT);
		scannablePartitions.partitions[i].sectorsPerCluster = fatVbr.bpb.sectorsPerCluster;
		scannablePartitions.partitions[i].rootDirFirstCluster = *(DWORD*)fatVbr.bpb.rootDirectoryCluster;

		scannablePartitions.partitionsNumber += 1;

		fprintf(stdout, "[SCANNER:] Found scannable FAT32 partition\n");
	}
}

ScannablePartition * getPartition(char partid)
{
	return &scannablePartitions.partitions[partid];
}

long getPartitionsNumber()
{
	if(!scannablePartitions.partitionsNumber)
	{
		scannablePartitions.partitionsNumber = 0;
	}
	return scannablePartitions.partitionsNumber;
}

long long clusterLbaAddrN(char partition, long cluster)
{
	return scannablePartitions.partitions[partition].clusterBeginLba + 
	(cluster - 2) * scannablePartitions.partitions[partition].sectorsPerCluster;
}

long long clusterLbaAddr(const ScannablePartition * partition, long cluster)
{
	return partition->clusterBeginLba + (cluster - 2) * partition->sectorsPerCluster;
}

const char WALKED = 0x04;
char * clusterMark;
long * fileAllocTable;

void scanDir(HANDLE drive, ScannablePartition partition, long startCluster)
{
	assert(drive != INVALID_HANDLE_VALUE);
	
	long bytesRead;
	bool readFileSuccess;
	
	LARGE_INTEGER byteOffset;
	byteOffset.QuadPart = 512*clusterLbaAddr(&partition,startCluster);
	
	bool setPointer = SetFilePointerEx(drive,byteOffset,NULL,FILE_BEGIN);
	if(!setPointer)
	{
		return;
	}
	
	bool loop = true;
	printf("%08x\n",startCluster);
	long nextCluster = fileAllocTable[startCluster];
	printf("%08x\n",nextCluster);
	long readClusters = 0;
	while(loop)
	{
		RootDirEntry rdir;
		BYTE byteRoot[512];
		memset(&rdir,0,32);
		
		readFileSuccess = ReadFile(drive, byteRoot, 512, &bytesRead, NULL);
		
		for(int i = 0; i < 15 && loop; ++ i)
		{
			memcpy(&rdir,&byteRoot[32*i],32);
			if(rdir.shortName[0] == 0xE5)
			{
				continue;
			}
			else if(rdir.shortName[0] == 0x00)
			{
				loop = false;
				continue;
			}
			else
			{
				char buffer[12];
				sprintf(buffer, "%c %c %c %c %c %c %c %c %c %c %c",
						rdir.shortName[0],
						rdir.shortName[1],
						rdir.shortName[2],
						rdir.shortName[3],
						rdir.shortName[4],
						rdir.shortName[5],
						rdir.shortName[6],
						rdir.shortName[7],
						rdir.shortName[8],
						rdir.shortName[9],
						rdir.shortName[10]);
				if(rdir.attributes & 0x0F)
				{
					printf( "[SCANNER:] long name entry, ignoring\n");
					continue;
				}
				else if(rdir.attributes & 0x10)
				{
					printf( "[SCANNER:] + folder entry: %s\n",buffer);
					if(rdir.shortName[0] == '.')
					{
						continue;
					}
					//printf( "%d \n",(long)rdir.firstClusterLow[0] + (long)rdir.firstClusterHigh[0] << 16);
					//scanDir(drive, partition, (long)rdir.firstClusterLow + (long)rdir.firstClusterHigh << 16);
				}
				else if(rdir.attributes & 0x08)
				{
					printf( "[SCANNER:] valume label: %s\n",buffer);
				}
				else
				{
					printf( "[SCANNER:] file entry: %s\n",buffer);
				}
			}
		}
		
		++ readClusters;
		
		if(readClusters == partition.sectorsPerCluster)
		{
			if(nextCluster < 0xFFFFFFF7)
			{
				byteOffset;
				byteOffset.QuadPart = 512*clusterLbaAddr(&partition,nextCluster);
	
				setPointer = SetFilePointerEx(drive,byteOffset,NULL,FILE_BEGIN);
				nextCluster = fileAllocTable[nextCluster];
			}
			else
			{
				loop = false;
			}
		}
		
	}
}

void scanPartition(HANDLE drive, char part)
{
	assert(drive != INVALID_HANDLE_VALUE);
	
	long bytesRead;
	bool readFileSuccess;
	ScannablePartition partition = scannablePartitions.partitions[part];
		
	long long metaInfoSize = sizeof(char)*partition.sectorsNumber/partition.sectorsPerCluster;
	clusterMark = (char * )malloc(metaInfoSize);
	memset(clusterMark,0,metaInfoSize);
	
	fileAllocTable = (long *)malloc(partition.fatSize*512);
	
	LARGE_INTEGER byteOffset;
	byteOffset.QuadPart = 512*partition.fatBeginLba;
	printf("%u\n",partition.fatSize);
	printf("%016x\n",(long long)partition.fatSize*512);
	bool setPointer = SetFilePointerEx(drive,byteOffset,NULL,FILE_BEGIN);
	if(!setPointer)
	{
		return;
	}
	
	readFileSuccess = ReadFile(drive, fileAllocTable, partition.fatSize*512, &bytesRead, NULL);
	/*LARGE_INTEGER byteOffset;
	byteOffset.QuadPart = 512*clusterLbaAddr(part,partition.rootDirFirstCluster);*/
		
	scanDir(drive,partition,partition.rootDirFirstCluster);
}