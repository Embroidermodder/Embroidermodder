/*! @file compound-file-fat.h */
#ifndef COMPOUND_FILE_FAT_H
#define COMPOUND_FILE_FAT_H

#include "emb-file.h"

#include "api-start.h"
#ifdef __cplusplus
extern "C" {
#endif

typedef struct _bcf_file_fat
{
    int          fatEntryCount;
    unsigned int fatEntries[255]; /* maybe make this dynamic */
    unsigned int numberOfEntriesInFatSector;
} bcf_file_fat;

extern EMB_PRIVATE bcf_file_fat* EMB_CALL bcfFileFat_create(const unsigned int sectorSize);
extern EMB_PRIVATE void EMB_CALL loadFatFromSector(bcf_file_fat* fat, EmbFile* file);
extern EMB_PRIVATE void EMB_CALL bcf_file_fat_free(bcf_file_fat* fat);

#ifdef __cplusplus
}
#endif /* __cplusplus */
#include "api-stop.h"

#endif /* COMPOUND_FILE_FAT_H */

/* kate: bom off; indent-mode cstyle; indent-width 4; replace-trailing-space-save on; */
