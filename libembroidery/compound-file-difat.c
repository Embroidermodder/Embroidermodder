#include "compound-file-difat.h"
#include "compound-file-common.h"
#include "emb-logging.h"
#include "helpers-binary.h"
#include <stdlib.h>

static const unsigned int sizeOfDifatEntry = 4;
static const unsigned int sizeOfChainingEntryAtEndOfDifatSector = 4;

bcf_file_difat* bcf_difat_create(EmbFile* file, unsigned int fatSectors, const unsigned int sectorSize)
{
    unsigned int i;
    bcf_file_difat* difat = 0;
    unsigned int sectorRef;

    difat = (bcf_file_difat*)malloc(sizeof(bcf_file_difat));
    if(!difat) { embLog_error("compound-file-difat.c bcf_difat_create(), cannot allocate memory for difat\n"); } /* TODO: avoid crashing. null pointer will be accessed */

    difat->sectorSize = sectorSize;
    if(fatSectors > NumberOfDifatEntriesInHeader)
    {
        fatSectors = NumberOfDifatEntriesInHeader;
    }

    for(i = 0; i < fatSectors; ++i)
    {
        sectorRef = binaryReadUInt32(file);
        difat->fatSectorEntries[i] = sectorRef;
    }
    difat->fatSectorCount = fatSectors;
    for(i = fatSectors; i < NumberOfDifatEntriesInHeader; ++i)
    {
        sectorRef = binaryReadUInt32(file);
        if(sectorRef != CompoundFileSector_FreeSector)
        {
            embLog_error("compound-file-difat.c bcf_difat_create(), Unexpected sector value %x at DIFAT[%d]\n", sectorRef, i);
        }
    }
    return difat;
}

unsigned int numberOfEntriesInDifatSector(bcf_file_difat* fat)
{
    return (fat->sectorSize - sizeOfChainingEntryAtEndOfDifatSector ) / sizeOfDifatEntry;
}

unsigned int readFullSector(EmbFile* file, bcf_file_difat* bcfFile, unsigned int* numberOfDifatEntriesStillToRead)
{
    unsigned int i;
    unsigned int sectorRef;
    unsigned int nextDifatSectorInChain;
    unsigned int entriesToReadInThisSector = 0;
    if(*numberOfDifatEntriesStillToRead > numberOfEntriesInDifatSector(bcfFile))
    {
        entriesToReadInThisSector = numberOfEntriesInDifatSector(bcfFile);
        *numberOfDifatEntriesStillToRead -= entriesToReadInThisSector;
    }
    else
    {
        entriesToReadInThisSector = *numberOfDifatEntriesStillToRead;
        *numberOfDifatEntriesStillToRead = 0;
    }

    for(i = 0; i < entriesToReadInThisSector; ++i)
    {
        sectorRef = binaryReadUInt32(file);
        bcfFile->fatSectorEntries[bcfFile->fatSectorCount]= sectorRef;
        bcfFile->fatSectorCount++;
    }
    for(i = entriesToReadInThisSector; i < numberOfEntriesInDifatSector(bcfFile); ++i)
    {
        sectorRef = binaryReadUInt32(file);
        if(sectorRef != CompoundFileSector_FreeSector)
        {
            embLog_error("compound-file-difat.c readFullSector(), Unexpected sector value %x at DIFAT[%d]]\n", sectorRef, i);
        }
    }
    nextDifatSectorInChain = binaryReadUInt32(file);
    return nextDifatSectorInChain;
}

void bcf_file_difat_free(bcf_file_difat* difat)
{
    free(difat);
    difat = 0;
}

/* kate: bom off; indent-mode cstyle; indent-width 4; replace-trailing-space-save on; */
