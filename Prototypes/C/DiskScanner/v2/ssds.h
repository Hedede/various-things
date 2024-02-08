#ifndef __SSDS_H__
#define __SSDS_H__

typedef struct
{
	unsigned beQuiet : 1;
	unsigned niceTable : 1;
	unsigned noProcess : 1;
	unsigned fixLost : 1;
} Options; 

#endif __SSDS_H__