#ifndef __MBR_H__
#define __MBR_H__

#include "stdbool.h"

#include <Windows.h>

typedef struct
{
    unsigned char status;
    unsigned char chsBegin[3];
    unsigned char systemId;
    unsigned char chsEnd[3];
    unsigned char lbaBegin[4];
    unsigned char sectorsNumber[4];
} Partition;

typedef struct
{
    unsigned char boot_code[446];
    Partition partitions[4];
    unsigned char signature[2];
} Mbr;

HANDLE OpenDrive(char driveId);
bool ReadMBR(HANDLE drive, Mbr * target);
void AnalyzeMBR(HANDLE drive, const Mbr * mbr);

#endif