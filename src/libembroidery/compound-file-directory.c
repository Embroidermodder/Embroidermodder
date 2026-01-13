#include "compound-file-directory.h"
#include "compound-file-common.h"
#include "emb-logging.h"
#include "helpers-binary.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

static void parseDirectoryEntryName(EmbFile* file, bcf_directory_entry* dir)
{
    int i;
    unsigned short unicodechar;
    for(i = 0; i < 32; ++i)
    {
        unicodechar = binaryReadUInt16(file);
        if(unicodechar != 0x0000)
        {
            dir->directoryEntryName[i] = (char)unicodechar;
        }
    }
}

static void readCLSID(EmbFile* file, bcf_directory_entry* dir)
{
    int i;
    unsigned char scratch;
    const int guidSize = 16;
    for(i = 0; i < guidSize; ++i)
    {
        scratch = binaryReadByte(file);
        dir->CLSID[i] = scratch;
    }
}

bcf_directory* CompoundFileDirectory(const unsigned int maxNumberOfDirectoryEntries)
{
    bcf_directory* dir = (bcf_directory*)malloc(sizeof(bcf_directory));
    if(!dir) { embLog_error("compound-file-directory.c CompoundFileDirectory(), cannot allocate memory for dir\n"); } /* TODO: avoid crashing. null pointer will be accessed */
    dir->maxNumberOfDirectoryEntries = maxNumberOfDirectoryEntries;
    dir->dirEntries = 0;
    return dir;
}

EmbTime parseTime(EmbFile* file)
{
    EmbTime returnVal;
    unsigned int ft_low, ft_high;
    /*embTime_time(&returnVal); TODO: use embTime_time() rather than time(). */
    ft_low = binaryReadInt32(file);
    ft_high = binaryReadInt32(file);
    /* TODO: translate to actual date time */
    return returnVal;
}

bcf_directory_entry* CompoundFileDirectoryEntry(EmbFile* file)
{
    bcf_directory_entry* dir = (bcf_directory_entry*)malloc(sizeof(bcf_directory_entry));
    if(!dir) { embLog_error("compound-file-directory.c CompoundFileDirectoryEntry(), cannot allocate memory for dir\n"); } /* TODO: avoid crashing. null pointer will be accessed */
    memset(dir->directoryEntryName, 0, 32);
    parseDirectoryEntryName(file, dir);
    dir->next = 0;
    dir->directoryEntryNameLength = binaryReadUInt16(file);
    dir->objectType = (unsigned char)binaryReadByte(file);
    if( (dir->objectType != ObjectTypeStorage) &&
        (dir->objectType != ObjectTypeStream) &&
        (dir->objectType != ObjectTypeRootEntry))
    {
        embLog_error("compound-file-directory.c CompoundFileDirectoryEntry(), unexpected object type: %d\n", dir->objectType);
        return 0;
    }
    dir->colorFlag = (unsigned char)binaryReadByte(file);
    dir->leftSiblingId = binaryReadUInt32(file);
    dir->rightSiblingId = binaryReadUInt32(file);
    dir->childId = binaryReadUInt32(file);
    readCLSID(file, dir);
    dir->stateBits = binaryReadUInt32(file);
    dir->creationTime = parseTime(file);
    dir->modifiedTime = parseTime(file);
    dir->startingSectorLocation = binaryReadUInt32(file);
    dir->streamSize = binaryReadUInt32(file); /* This should really be __int64 or long long, but for our uses we should never run into an issue */
    dir->streamSizeHigh = binaryReadUInt32(file); /* top portion of int64 */
    return dir;
}

void readNextSector(EmbFile* file, bcf_directory* dir)
{
    unsigned int i;
    for(i = 0; i < dir->maxNumberOfDirectoryEntries; ++i)
    {
        bcf_directory_entry* dirEntry = CompoundFileDirectoryEntry(file);
        bcf_directory_entry* pointer = dir->dirEntries;
        if(!pointer)
        {
            dir->dirEntries = dirEntry;
        }
        else
        {
            while(pointer)
            {
                if(!pointer->next)
                {
                    pointer->next = dirEntry;
                    break;
                }
                pointer = pointer->next;
            }
        }
    }
}

void bcf_directory_free(bcf_directory* dir)
{
    bcf_directory_entry* pointer = dir->dirEntries;
    bcf_directory_entry* entryToFree = 0;
    while(pointer)
    {
        entryToFree = pointer;
        pointer = pointer->next;
        free(entryToFree);
        entryToFree = 0;
    }
    if(dir)
    {
        free(dir);
        dir = 0;
    }
}

/* kate: bom off; indent-mode cstyle; indent-width 4; replace-trailing-space-save on; */
