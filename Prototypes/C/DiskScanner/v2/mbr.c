
#include "stdlib.h"
#include "string.h"
#include "stdio.h"

#include <Windows.h>

#include "scanner.h"
#include "ui.h"
#include "mbr.h"

HANDLE OpenDrive(char driveId)
{
    wchar_t buffer[128];

	wsprintfW(buffer,L"\\\\.\\PhysicalDrive%1d", driveId);
	HANDLE hDisk = CreateFileW(buffer,
		GENERIC_READ,
		FILE_SHARE_READ, 
		NULL, OPEN_EXISTING, 0, NULL);
	
	return hDisk;
}

bool ReadMBR(HANDLE drive, Mbr * target)
{
    DWORD bytes_read;
	
	LARGE_INTEGER byteOffset;
	byteOffset.QuadPart = 0;
	
	bool setPointer = SetFilePointerEx(drive,byteOffset,NULL,FILE_BEGIN);

	if(!setPointer)
	{
		return false;
	}

    ReadFile(drive, target, sizeof(Mbr), &bytes_read, NULL);
	
	
    if(bytes_read)
    {
        return true;
    }
	else
	{
		return false;
	}
}


void AnalyzeMBR(HANDLE drive, const Mbr * mbr)
{
    unsigned int i;
    char buffer[128];

    memset(buffer, 0, sizeof(buffer));
    
    if(memcmp(mbr->signature, "\x55\xAA", sizeof(mbr->signature)) != 0)
    {
        printLine("Invalid MBR signature\n");
        return;
    }

    for(i = 0; i < 4; i++)
    {
		if(mbr->partitions[i].systemId == 0x00)
		{
			continue;
		}
		
        sprintf(buffer, "Partition: %u\n", i);
        printLine(buffer);
    
    
        switch(mbr->partitions[i].status)
        {
			case 0x80:
				sprintf(buffer, "    Status: %02X Bootable \n", mbr->partitions[i].status);
			break;
			case 0x00:
				sprintf(buffer, "    Status: %02X Inactive \n", mbr->partitions[i].status);
			break;
			default:
				sprintf(buffer, "    Status: %02X Invalid \n", mbr->partitions[i].status);
			break;
		}
		printLine(buffer);

		sprintf(buffer, "    System ID: %02X\n", mbr->partitions[i].systemId);
		printLine(buffer);

		sprintf(buffer, "    CHS address of first absolute sector %02X %02X %02X\n", 
			mbr->partitions[i].chsBegin[2],
			mbr->partitions[i].chsBegin[1],
			mbr->partitions[i].chsBegin[0]);
        printLine(buffer);

        sprintf(buffer, "    CHS address of last absolute sector %02X %02X %02X\n",
			mbr->partitions[i].chsEnd[2],
			mbr->partitions[i].chsEnd[1],
			mbr->partitions[i].chsEnd[0]);
        printLine(buffer);

        sprintf(buffer, "    LBA of first absolute sector %02X %02X %02X %02X\n",
			mbr->partitions[i].lbaBegin[3],
			mbr->partitions[i].lbaBegin[2],
			mbr->partitions[i].lbaBegin[1],
			mbr->partitions[i].lbaBegin[0]);
        printLine(buffer);

        sprintf(buffer, "    Number of sectors %02X %02X %02X %02X\n", 
			mbr->partitions[i].sectorsNumber[3],
			mbr->partitions[i].sectorsNumber[2],
			mbr->partitions[i].sectorsNumber[1],
			mbr->partitions[i].sectorsNumber[0]);
        printLine(buffer);
		
		if(mbr->partitions[i].systemId == 0x0B || mbr->partitions[i].systemId == 0x0C)
		{
			long sectorsNumber = 0;
			sectorsNumber += (long)mbr->partitions[i].sectorsNumber[0];
			sectorsNumber += (long)mbr->partitions[i].sectorsNumber[1] << 8;
			sectorsNumber += (long)mbr->partitions[i].sectorsNumber[2] << 16;
			sectorsNumber += (long)mbr->partitions[i].sectorsNumber[3] << 24;
			long partitionLba = 0;
			partitionLba += (long)mbr->partitions[i].lbaBegin[0];
			partitionLba += (long)mbr->partitions[i].lbaBegin[1] << 8;
			partitionLba += (long)mbr->partitions[i].lbaBegin[2] << 16;
			partitionLba += (long)mbr->partitions[i].lbaBegin[3] << 24;
	
			addPartition(drive, partitionLba, sectorsNumber);
		}
    }

}