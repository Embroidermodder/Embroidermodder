#ifndef COMPOUND_FILE_DIFAT_H
#define COMPOUND_FILE_DIFAT_H

#ifdef __cplusplus
extern "C" {
#endif

#include <stdio.h> /* TODO: replace this with "emb-file.h" when FILE is ported to EmbFile */

/* double-indirection file allocation table references */
typedef struct _bcf_file_difat
{
    unsigned int fatSectorCount;
    unsigned int fatSectorEntries[109];
    unsigned int sectorSize;
} bcf_file_difat;

bcf_file_difat *bcf_difat_create(FILE* file, unsigned int fatSectors, const unsigned int sectorSize);
unsigned int readFullSector(FILE* file, bcf_file_difat* bcfFile, unsigned int* numberOfDifatEntriesStillToRead);
unsigned int numberOfEntriesInDifatSector(bcf_file_difat* fat);
void bcf_file_difat_free(bcf_file_difat* difat);

/**
  Constant representing the number of Double Indirect FAT entries in a single header
*/
static const unsigned int NumberOfDifatEntriesInHeader = 109;

#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif /* COMPOUND_FILE_DIFAT_H */

/* kate: bom off; indent-mode cstyle; indent-width 4; replace-trailing-space-save on; */
