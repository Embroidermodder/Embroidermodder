/*! @file compound-file-difat.h */
#ifndef COMPOUND_FILE_DIFAT_H
#define COMPOUND_FILE_DIFAT_H

#include "emb-file.h"

#include "api-start.h"
#ifdef __cplusplus
extern "C" {
#endif

/* double-indirection file allocation table references */
typedef struct _bcf_file_difat
{
    unsigned int fatSectorCount;
    unsigned int fatSectorEntries[109];
    unsigned int sectorSize;
} bcf_file_difat;

extern EMB_PRIVATE bcf_file_difat* EMB_CALL bcf_difat_create(EmbFile* file, unsigned int fatSectors, const unsigned int sectorSize);
extern EMB_PRIVATE unsigned int EMB_CALL readFullSector(EmbFile* file, bcf_file_difat* bcfFile, unsigned int* numberOfDifatEntriesStillToRead);
extern EMB_PRIVATE unsigned int EMB_CALL numberOfEntriesInDifatSector(bcf_file_difat* fat);
extern EMB_PRIVATE void EMB_CALL bcf_file_difat_free(bcf_file_difat* difat);

/*! Constant representing the number of Double Indirect FAT entries in a single header */
static const unsigned int NumberOfDifatEntriesInHeader = 109;

#ifdef __cplusplus
}
#endif /* __cplusplus */
#include "api-stop.h"

#endif /* COMPOUND_FILE_DIFAT_H */

/* kate: bom off; indent-mode cstyle; indent-width 4; replace-trailing-space-save on; */
