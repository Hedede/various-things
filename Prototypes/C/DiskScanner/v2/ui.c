#include "stdlib.h"
#include "stdbool.h"
#include "string.h"
#include "stdio.h"
#include "stdint.h"

#include <Windows.h>

#include "mbr.h"
#include "scanner.h"

#include "ui.h"

typedef enum  
{
	STATE_GET_ARG = 0,
	STATE_GET_FILENAME = 1,
	STATE_GET_NUMBER = 2
} EParserState;

typedef struct
{
	int drive;
	int partition;
	char * outputFile;
	unsigned scanImage : 1;
	unsigned niceTable : 1;
	unsigned outputResult : 1;
	unsigned beQuiet : 1;
	unsigned noScan : 1;
	unsigned fixLost : 1;
} Options; 

bool getFileNameArg(char *out, char *name)
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

bool parseArguments(int argc, char *argv[], Options* options)
{
	EParserState parserState = STATE_GET_ARG;
	void * target;
	
	for (int i = 1; i < argc; ++i)
	{
		if(parserState == STATE_GET_ARG)
		{
			if(strcmp(argv[i], "--help") == 0)
			{
				fprintf(stdout,"usage: ssds <args>\n");
				fprintf(stdout," --help                      display this message\n");
				fprintf(stdout," --drive or -d   <number>    scan disk drive <number>\n");
				fprintf(stdout," --out   or -o   <filename>  name of output file\n");
				fprintf(stdout," --quiet or -q               be more quiet\n");
				fprintf(stdout," --part  or -p   <number>    select desired partition\n");
				fprintf(stdout," --table                     output to CSV table\n");
				fprintf(stdout," --noscan                    don't process input\n");
				fprintf(stdout," --fixlost                   create files for lost clusters\n");
				return false;
			} 
			else if(strcmp(argv[i], "-i") == 0 || strcmp(argv[i], "--image") == 0)
			{
				fprintf(stderr,"[WARNING:] argument %s is deprecated, ignoring\n",argv[i]);
				/*parserState = STATE_GET_FILENAME;
				target = &options->fileName;
				options->scanImage = true;*/
			}
			else if(strcmp(argv[i], "-o") == 0 || strcmp(argv[i], "--out") == 0)
			{
				parserState = STATE_GET_FILENAME;
				target = options->outputFile;
			}
			else if(strcmp(argv[i], "-q") == 0 || strcmp(argv[i], "--quiet") == 0)
			{
				options->beQuiet = true;
			}
			else if(strcmp(argv[i], "-d") == 0 || strcmp(argv[i], "--drive") == 0)
			{
				parserState = STATE_GET_NUMBER;
				target = &options->drive;
			}
			else if(strcmp(argv[i], "-p") == 0 || strcmp(argv[i], "--part") == 0)
			{
				parserState = STATE_GET_NUMBER;
				target = &options->partition;
			}
			else if(strcmp(argv[i], "--table") == 0)
			{
				options->niceTable = true;
			}
			else if(strcmp(argv[i], "--noscan") == 0)
			{
				options->noScan = true;
			}
			else if(strcmp(argv[i], "--fixlost") == 0)
			{
				options->fixLost = true;
			}
			else 
			{
				if(!options->beQuiet)
				{
					fprintf(stderr,"[WARNING:] unknown argument %s, ignoring\n",argv[i]);
				}
			}
		}
		else if(parserState == STATE_GET_FILENAME)
		{
			if(strlen(target) == 0)
			{
				if(!getFileNameArg((char *)target,argv[i])) continue;
			}
			else
			{
				if(!options->beQuiet) 
				{
					fprintf(stderr,
					"[WARNING:] duplicate %s argument, ignoring\n",argv[i-1]);
				}
			}
			
			parserState = STATE_GET_ARG;
		}
		else if(parserState == STATE_GET_NUMBER)
		{
			if(*(int *)target == -1)
			{
				if(!sscanf(argv[i],"%1d",(int *)target)) continue;
			}
			else
			{
				if(!options->beQuiet) 
				{
					fprintf(stderr, 
					"[WARNING:] duplicate %s argument, ignoring\n",argv[i-1]);
				}
			}
			
			parserState = STATE_GET_ARG;
		}
	}
	
	return true;
}

void printLine(char * line)
{
	fprintf(stdout,line);
}

int main(int argc, char *argv[])
{
    char buffer[128];
	//char fileName[256] = "";
	//char outputFile[256] = "";
	
	Options options = 
	{
		.drive =			-1, 
		.partition =		-1, 
		.outputFile =		"",
		.scanImage = 		false,
		.outputResult = 	false,
		.beQuiet = 			false,
		.noScan = 			false,
		.fixLost = 			false,
		.niceTable = 		false
	};
	
	if(!parseArguments(argc,argv,&options))
	{
		return EXIT_SUCCESS;
	}
	
	if(options.drive >= 0)
	{
		HANDLE hDisk = OpenDrive(options.drive);
		if(hDisk == INVALID_HANDLE_VALUE)
		{
			fprintf(stderr,"[ERROR:] error opening drive %1d",options.drive);
			return EXIT_FAILURE;
		}
		
		Mbr mbr;
	
		ReadMBR(hDisk,&mbr);
		AnalyzeMBR(hDisk,&mbr);
		
		if(getPartitionsNumber() > 1)
		{
			while(options.partition < 1 || options.partition > getPartitionsNumber())
			{
				fprintf(stderr, "[WARNING:] invalid partition, please enter partition number between 1 and %d\n",getPartitionsNumber());
				fprintf(stderr, "> ");
				fscanf(stdin,"%d",&options.partition);
			}
		}
		else if(getPartitionsNumber() == 0)
		{
			fprintf(stderr, "[SCANNER:] found no scannable partitions");
			return EXIT_SUCCESS;
		}
		else
		{
			options.partition = getPartitionsNumber();
		}
		
		-- options.partition;
		
		ScannablePartition * part = getPartition(options.partition);
		
		scanPartition(hDisk,options.partition);
	}
	
	return EXIT_SUCCESS;
}