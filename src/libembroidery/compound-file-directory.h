/*! @file compound-file-directory.h */
#ifndef COMPOUND_FILE_DIRECTORY_H
#define COMPOUND_FILE_DIRECTORY_H

#include "emb-time.h"
#include "emb-file.h"

#include "api-start.h"
#ifdef __cplusplus
extern "C" {
#endif

typedef struct _bcf_directory_entry
{
    char                         directoryEntryName[32];
    unsigned short               directoryEntryNameLength;
    unsigned char                objectType;
    unsigned char                colorFlag;
    unsigned int                 leftSiblingId;
    unsigned int                 rightSiblingId;
    unsigned int                 childId;
    unsigned char                CLSID[16];
    unsigned int                 stateBits;
    EmbTime                      creationTime;
    EmbTime                      modifiedTime;
    unsigned int                 startingSectorLocation;
    unsigned long                streamSize; /* should be long long but in our case we shouldn't need it, and hard to support on c89 cross platform */
    unsigned int                 streamSizeHigh; /* store the high int of streamsize */
    struct _bcf_directory_entry* next;
} bcf_directory_entry;

typedef struct _bcf_directory
{
    bcf_directory_entry* dirEntries;
    unsigned int         maxNumberOfDirectoryEntries;
    /* TODO: possibly add a directory tree in the future */

} bcf_directory;

extern EMB_PRIVATE bcf_directory_entry* EMB_CALL CompoundFileDirectoryEntry(EmbFile* file);
extern EMB_PRIVATE bcf_directory* EMB_CALL CompoundFileDirectory(const unsigned int maxNumberOfDirectoryEntries);
extern EMB_PRIVATE void EMB_CALL readNextSector(EmbFile* file, bcf_directory* dir);
extern EMB_PRIVATE void EMB_CALL bcf_directory_free(bcf_directory* dir);

#ifdef __cplusplus
}
#endif /* __cplusplus */
#include "api-stop.h"

#endif /* COMPOUND_FILE_DIRECTORY_H */

/* kate: bom off; indent-mode cstyle; indent-width 4; replace-trailing-space-save on; */
