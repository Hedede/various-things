#ifndef __FAT32_H__
#define __FAT32_H__

#define WIN32_LEAN_AND_MEAN
#include <windows.h>


typedef struct 
{
    BYTE shortName[11];
    BYTE attributes;
    BYTE unused;
    BYTE timeCrtTenth;
    BYTE timeCreated[2];
    BYTE dateCreated[2];
    BYTE dateAccessed[2];
	BYTE firstClusterHigh[2];
    BYTE timeModified[2];
    BYTE dateModified[2];
	BYTE firstClusterLow[2];
	BYTE fileSize[4];
} RootDirEntry;

typedef struct 
{
    BYTE bytesPerSector[2];
    BYTE sectorsPerCluster;
    BYTE reservedSectors[2];
    BYTE fatCopies;
	BYTE rootDirEntries[2]; // fat 16
	BYTE smallSectors [2];// fat 16
	BYTE mediaType;
	BYTE sectorsPerFAT[2]; // fat 16
	BYTE sectorsPerTrack[2];
	BYTE numberOfHeads[2];
	BYTE hiddenSectors[4];
	BYTE totalSectors [4];
	BYTE bigSectorsPerFAT[4];
	BYTE extFlags[2];
	BYTE fileSystemVersion[2];
	BYTE rootDirectoryCluster[4];
	BYTE fsinfoSectorNumber[2];
	BYTE backupBootSector[2];
	BYTE reserved[12];
} BpbFat32;

typedef struct 
{
    BYTE physicalDriveNumber;
    BYTE unused;
	BYTE extendedBootSignature;
	BYTE volumeSerialNumber[4];
	BYTE volumeLabel[11];
	BYTE systemId[8];
} ExtendedBpb;

typedef struct 
{
    unsigned char jump[3];
    unsigned char oemId[8];
	BpbFat32 bpb;
	ExtendedBpb extendedBpb;
    unsigned char boot_code[420];
    unsigned char signature[2];
} Vbr;

#endif