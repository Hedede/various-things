#include "stdlib.h"
#include "string.h"
#include "stdio.h"

#include "mbr.h"

#include "ssds.h"


struct Options
{
	unsigned beQuiet : 1;
	unsigned niceTable : 1;
	unsigned noProcess : 1;
	unsigned fixLost : 1;
};

struct DiskImage
{
	FileTableEntry * fileTable;
	Cluster * clusters;
}

enum EArgument 
{
	ARG_UNKNOWN = 0,
	ARG_FILENAME = 1,
	ARG_OUTNAME = 2
};

struct FileTableEntry
{
	unsigned int firstCluster;
	char namePath[16];
};

struct Cluster
{
	unsigned int entryNo;
	//unsigned int data;
	unsigned int nextCluster;
};

bool getFileName(char *out, char *name)
{
	if(strlen(name) < 255)
	{
		strcpy(out,name);
		return true;
	}
	else
	{
		fprintf(stderr,"[ERROR:] file name too long: %d\n", strlen(name));
		return false;
	}
}

// Entry point
int main(int argc, char *argv[])
{
	char fileName[256];
	unsigned fileNameReceived = false;
	char outputFile[256];
	unsigned outputFileReceived = false;*/
	Options options = 
	{
		/*.beQuiet = */			false,
		/*.niceTable = */		false,
		/*.noProcess = */		false,
		/*.fixLost = */			false
	};

	EArgument curArg = ARG_UNKNOWN;*/
		
	/* parse argument line */
	for (int i = 1; i < argc; ++i)
	{
		if(curArg == ARG_UNKNOWN)
		{
			if(strcmp(argv[i], "--help") == 0)
			{
				fprintf(stdout,"usage: ssds <args>\n");
				fprintf(stdout," --help           display this message\n");
				fprintf(stdout," --file  or -f    name of input disk image\n");
				fprintf(stdout," --out   or -o    name of output file\n");
				fprintf(stdout," --quiet or -q    be more quiet\n");
				fprintf(stdout," --table          output to CSV table\n");
				fprintf(stdout," --noprocess      don't process input\n");
				fprintf(stdout," --fixlost        create files for lost clusters\n");
				return EXIT_SUCCESS;
			} 
			else if(strcmp(argv[i], "-f") == 0 || strcmp(argv[i], "--file") == 0)
			{
				curArg = ARG_FILENAME;
			}
			else if(strcmp(argv[i], "-o") == 0 || strcmp(argv[i], "--out") == 0)
			{
				curArg = ARG_OUTNAME;
			}
			else if(strcmp(argv[i], "-q") == 0 || strcmp(argv[i], "--quiet") == 0)
			{
				options.beQuiet = true;
			}
			else if(strcmp(argv[i], "--table") == 0)
			{
				options.niceTable = true;
			}
			else if(strcmp(argv[i], "--noprocess") == 0)
			{
				options.noProcess = true;
			}
			else if(strcmp(argv[i], "--fixlost") == 0)
			{
				options.fixLost = true;
			}
			else 
			{
				if(!options.beQuiet) fprintf(stderr,"[WARNING:] unknown argument, ignoring\n");
			}
		}
		else if(curArg == ARG_FILENAME)
		{
			if(!fileNameReceived)
			{
				if(!getFileName(fileName,argv[i])) return EXIT_FAILURE;
				fileNameReceived = true;
			}
			else
			{
				if(!options.beQuiet) fprintf(stderr,
					"[WARNING:] duplicate -f/--file argument, ignoring\n");
			}
			
			/* reset state*/
			curArg = ARG_UNKNOWN;
		}
		else if(curArg == ARG_OUTNAME)
		{
			if(!outputFileReceived)
			{
				if(!getFileName(outputFile,argv[i])) return EXIT_FAILURE;	
				outputFileReceived = true;
			}
			else
			{
				if(!options.beQuiet) fprintf(stderr,
					"[WARNING:] duplicate -o/--out argument, ignoring\n");
			}
			*/
			/* reset state*/
			curArg = ARG_UNKNOWN;
		}
    }

	if(!fileNameReceived)
	{
		if(!options.beQuiet) fprintf(stderr,
			"[WARNING:] input file name uspecified, using default file name: <input>\n");
		strcpy(fileName,"input");
	}
	
	if(!outputFileReceived)
	{
		if(!options.beQuiet) fprintf(stderr,
			"[WARNING:] output file name uspecified, using default file name: <output>\n");
		strcpy(outputFile,"output");
	}
	
	FILE * diskImage;
	diskImage = fopen(fileName,"rb");

	if(!diskImage)
	{
		fprintf(stderr,"[ERROR:] cannot read file %s\n",fileName);
		return EXIT_FAILURE;
	}

	/* not length in bytes, but an amount of entries */
	int fileTableLength = 0;
	int clustersLength = 0; 
	fread(&fileTableLength,sizeof(int),1,diskImage);
	fread(&clustersLength,sizeof(int),1,diskImage);

	if(!options.beQuiet)
	{
		fprintf(stderr,"[INFO:] files: %d; clusters: %d\n",fileTableLength,clustersLength);
	}
	
	FileTableEntry * fileTableEntries = 
		(FileTableEntry *)malloc(fileTableLength*sizeof(FileTableEntry));
	FileTableEntry * lostEntries;

	Cluster * clusters;	
	

	/* read file table */
	for(int i = 0; i < fileTableLength; ++i)
	{
		char tmpBuf[255];
		
		fread(&(fileTableEntries[i].namePath),sizeof(char),16,diskImage);
		fread(&(fileTableEntries[i].firstCluster),sizeof(int),1,diskImage);
		
		if(ferror(diskImage))
		{
			fprintf(stderr,"[ERROR:] error reading file at pos %d\n",ftell(diskImage));
			return EXIT_FAILURE;
		}
		
		if(feof(diskImage))
		{
			fprintf(stderr,"[ERROR:] unexpected end of file at pos %d\n",ftell(diskImage));
			return EXIT_FAILURE;
		}
	}
	
	/* read rest of the file */

	clusters = (Cluster *)malloc(sizeof(Cluster)*clustersLength);

	fread(clusters,sizeof(Cluster),clustersLength,diskImage);

	if(ferror(diskImage))
	{
		fprintf(stderr,"[ERROR:] error reading file at pos %d\n",ftell(diskImage));
		return EXIT_FAILURE;
	}

	fclose(diskImage);
	
	int additionalEntries = 0;

	char * fileStatus = (char *)calloc(fileTableLength,sizeof(char));
	char * clusterStatus = (char *)calloc(clustersLength,sizeof(char));

if(!options.noProcess) {
	fprintf(stdout,"[SCANNER:] scanning disk image\n");

	for(int i = 0; i < fileTableLength; ++i)
	{
		if(fileTableEntries[i].firstCluster > clustersLength-1)
		{
			fprintf(stdout,
				"[SCANNER:] cluster %d is outside of bounds, marking file entry %s for deletion\n",
				fileTableEntries[i].firstCluster,fileTableEntries[i].namePath);
			fileStatus[i] = DELETE;
		}
		else
		{
			int addr = fileTableEntries[i].firstCluster;
			int nextaddr = 0;
			int loop = true;

			while(loop)
			{
				if(clusterStatus[addr] != WALKED)
				{
					clusterStatus[addr] = WALKED;

					if(clusters[addr].nextCluster == 0xFFFFFFFF)
					{
						loop = false;
					}
					else
					{
						nextaddr = clusters[addr].nextCluster;
						if(addr == clustersLength-1)
						{
							fprintf(stdout,
								"[SCANNER:] can not find end of file %s, writing EOF to cluster %d\n",
								fileTableEntries[i].namePath,addr);
							clusters[addr].nextCluster = 0xFFFFFFFF;
							fileStatus[i] = FIXED;
							loop = false;
						}
						else if(clusters[addr].nextCluster > clustersLength-1)
						{							
							fprintf(stdout,
								"[SCANNER:] cluster %d is outside of bounds, writing EOF to cluster %d\n",
								clusters[addr].nextCluster,addr);
							clusters[addr].nextCluster = 0xFFFFFFFF;
						}
						else if(clusterStatus[nextaddr] == WALKED)
						{
							fprintf(stdout,
								"[SCANNER:] intersection: file %s intersects with file %s, writing EOF to cluster %d\n",
								fileTableEntries[clusters[addr].entryNo].namePath,
								fileTableEntries[clusters[nextaddr].entryNo].namePath,addr);
							clusters[addr].nextCluster = 0xFFFFFFFF;
							fileStatus[i] = FIXED;
							loop = false;
						} 
					}

					if(fileStatus[i] == 0) fileStatus[i] = CORRECT;


					addr = nextaddr;
				}
				else
				{
					fprintf(stdout,
						"[SCANNER:] intersection: file %s intersects with file %s,\n marking file %s for deletion\n",
						fileTableEntries[i].namePath,
						fileTableEntries[clusters[addr].entryNo].namePath,
						fileTableEntries[i].namePath);
					fileStatus[i] = DELETE;
					loop = false;
				}
			} //end loop
		}
	}
	
	fprintf(stdout,"[SCANNER:] searching for lost clusters\n");	


	if(options.fixLost)
	{ 
		lostEntries = (FileTableEntry *)malloc(fileTableLength*sizeof(FileTableEntry));
	}

	for(int i = 0; i < clustersLength; ++i)
	{
		if(clusterStatus[i] != WALKED)
		{
			++ additionalEntries;
			fprintf(stdout,
				"[SCANNER:] found cluster %d not associated with file\n", i);
			if(options.fixLost)
			{
				int addr = i;
				int next = clusters[i].nextCluster;
				int loop = true;

				while(loop)
				{
					clusters[addr].entryNo = fileTableLength+additionalEntries;
					clusterStatus[addr] = WALKED;
					next = clusters[addr].nextCluster;

					if(next == 0xFFFFFFFF)
					{
						fprintf(stderr,"0xFFFFFFFF\n");
						loop = false;
					}
					else
					{
						if(next == WALKED || next == clustersLength-1)
						{
							loop = false;
							clusters[addr].nextCluster = 0xFFFFFFFF;
						}
						addr = next;
					}

				}
				sprintf(lostEntries[additionalEntries-1].namePath,"RECOVERY-%05d",additionalEntries-1);
				lostEntries[additionalEntries-1].firstCluster = i;
				fprintf(stdout,"[SCANNER:] created file %s for cluster %i\n", lostEntries[additionalEntries-1].namePath, additionalEntries-1,i);
			}
			else
			{
				clusterStatus[i] = LOST;
			}
		}
	}
} //!options.noProcess

	FILE * output;
	
	fprintf(stdout,"[INFO:] creating output file\n");

	if(!options.niceTable)
	{
		output = fopen(outputFile,"wb");
		int totalLength = fileTableLength+additionalEntries;
		fwrite(&totalLength,sizeof(int),1,output);
		fwrite(&clustersLength,sizeof(int),1,output);


		for(int i = 0; i < fileTableLength; ++i)
		{
			if(fileStatus[i] == DELETE) continue;
			fwrite(fileTableEntries[i].namePath,sizeof(char),sizeof(fileTableEntries[i].namePath),output);
			fwrite(&fileTableEntries[i].firstCluster,sizeof(int),1,output);
		}
		if(options.fixLost)
		{ 
			for(int i = 0; i < additionalEntries; ++i)
			{
				fprintf(stderr,"%s\n",lostEntries[i].namePath);

				fwrite(lostEntries[i].namePath,sizeof(char),sizeof(lostEntries[i].namePath),output);
				fwrite(&lostEntries[i].firstCluster,sizeof(int),1,output);
			}
		}
		fwrite(clusters,sizeof(Cluster),clustersLength,output);
	}
	else
	{
		output = fopen(outputFile,"w");
		fprintf(output,"\"id\";\"filename\";\"firstcluster\";;\"cluster\";\"nextcluster\"\n");
		for(int i = 0; i < fileTableLength + additionalEntries || i < clustersLength; ++i)
		{
			bool files = i < fileTableLength;
			bool filesa = (i - fileTableLength) < additionalEntries && options.fixLost;
			bool cluster = i < clustersLength;

			fprintf(output,"\"%d\";\"%s\";\"%d\";;\"%d\";\"%d\"\n",
				files || filesa ? i : 0,
				files ? fileTableEntries[i].namePath : filesa ? lostEntries[i-fileTableLength].namePath : "",
				files ? fileTableEntries[i].firstCluster : filesa ? lostEntries[i-fileTableLength].firstCluster : 0,
				cluster ? i : 0,
				cluster ? clusters[i].nextCluster : 0);
		}
	}

	return EXIT_SUCCESS;
}
