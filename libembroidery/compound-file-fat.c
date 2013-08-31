#include <stdio.h>
#include <stdlib.h>
#include "compound-file.h"
#include "helpers-binary.h"

const unsigned int sizeOfFatEntry = sizeof(unsigned int);

bcf_file_fat *bcfFileFat_create(const unsigned int sectorSize)
{
    bcf_file_fat *fat = (bcf_file_fat *)malloc(sizeof(bcf_file_fat));
    fat->numberOfEntriesInFatSector = sectorSize / sizeOfFatEntry;
    fat->fatEntryCount = 0;
    return fat;
}

void loadFatFromSector(bcf_file_fat *fat, FILE *file)
{
    unsigned int i;
    unsigned int currentNumberOfFatEntries = fat->fatEntryCount;
    unsigned int newSize = currentNumberOfFatEntries + fat->numberOfEntriesInFatSector;
    for (i = currentNumberOfFatEntries; i < newSize; ++i )
    {
        unsigned int fatEntry = binaryReadUInt32(file);
        fat->fatEntries[i] = fatEntry;
    }
    fat->fatEntryCount = newSize;
}

void bcf_file_fat_free(bcf_file_fat *fat)
{
    free(fat);
}

/* kate: bom off; indent-mode cstyle; indent-width 4; replace-trailing-space-save on; */
