/*
* Header file for using yaffs in an application via
* a direct interface.
*/


#ifndef __YAFFSFS_H__
#define __YAFFSFS_H__

#include "yaffs_trace.h"

struct yaffs_dev_config {
    char*	name;
    unsigned	nBytesPerChunk;
    unsigned	nChunksPerBlock;
    unsigned	nReservedBlocks;
    unsigned	startBlock;
    unsigned	endBlock;
    unsigned	nShortOpCaches;
    unsigned	isYaffs2;
    void*	genericDevice;
    void*	writeChunkWithTagsToNAND;
    void*	readChunkWithTagsFromNAND;
    void*	eraseBlockInNAND;
    void*	initialiseNAND;
    void*	markNANDBlockBad;
    void*	queryNANDBlock;
    void*	writeChunkToNAND;
    void*	readChunkFromNAND;
    void*	chunkFreeInNAND;
    void*	gcControl;
    unsigned	nCheckpointReservedBlocks;
};

extern int yaffsDebug;

int yaffs_initialise(struct yaffs_dev_config *configList[]);
int yaffs_StartUp(void);
int yaffsMount(char* mnt, char* dev, int flags);
int yaffsFormat(char* dev, int force);
const char* yaffsGetDevName(void* dev);
void* yaffsGetDevContext(void* dev);

#endif


