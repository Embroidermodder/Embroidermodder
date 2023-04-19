/**
 * \file emb-file.c
 */

#include "embroidery.h"
#include <stdarg.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <math.h>

static unsigned int sizeOfDirectoryEntry = 128;
const unsigned int sizeOfFatEntry = sizeof(unsigned int);
static const unsigned int sizeOfDifatEntry = 4;
static const unsigned int sizeOfChainingEntryAtEndOfDifatSector = 4;

EmbFile* embFile_open(const char* fileName, const char* mode)
{
#ifdef ARDUINO
    return inoFile_open(fileName, mode);
#else
    EmbFile* eFile = 0;
    FILE* oFile = fopen(fileName, mode);
    if(!oFile)
        return 0;

    eFile = (EmbFile*)malloc(sizeof(EmbFile));
    if(!eFile)
    {
        fclose(oFile);
        return 0;
    }

    eFile->file = oFile;
    return eFile;
#endif
}

int embFile_close(EmbFile* stream)
{
#ifdef ARDUINO
    return inoFile_close(stream);
#else /* ARDUINO */
    int retVal = fclose(stream->file);
    free(stream);
    stream = 0;
    return retVal;
#endif /* ARDUINO */
}

int embFile_eof(EmbFile* stream)
{
#ifdef ARDUINO
    return inoFile_eof(stream);
#else /* ARDUINO */
    return feof(stream->file);
#endif /* ARDUINO */
}

int embFile_getc(EmbFile* stream)
{
#ifdef ARDUINO
    return inoFile_getc(stream);
#else /* ARDUINO */
    return fgetc(stream->file);
#endif /* ARDUINO */
}

size_t embFile_read(void* ptr, size_t size, size_t nmemb, EmbFile* stream)
{
#ifdef ARDUINO
    return 0; /* ARDUINO TODO: SD File read() doesn't appear to return the same way as fread(). This will need work. */
#else /* ARDUINO */
    return fread(ptr, size, nmemb, stream->file);
#endif /* ARDUINO */
}

size_t embFile_write(const void* ptr, size_t size, size_t nmemb, EmbFile* stream)
{
#ifdef ARDUINO
    return 0; /* ARDUINO TODO: Implement inoFile_write. */
#else /* ARDUINO */
    return fwrite(ptr, size, nmemb, stream->file);
#endif /* ARDUINO */
}

int embFile_seek(EmbFile* stream, long offset, int origin)
{
#ifdef ARDUINO
    return inoFile_seek(stream, offset, origin);
#else /* ARDUINO */
    return fseek(stream->file, offset, origin);
#endif /* ARDUINO */
}

long embFile_tell(EmbFile* stream)
{
#ifdef ARDUINO
    return inoFile_tell(stream);
#else /* ARDUINO */
    return ftell(stream->file);
#endif /* ARDUINO */
}

EmbFile* embFile_tmpfile(void)
{
#ifdef ARDUINO
    return inoFile_tmpfile();
#else
    EmbFile* eFile = 0;
    FILE* tFile = tmpfile();
    if(!tFile)
        return 0;

    eFile = (EmbFile*)malloc(sizeof(EmbFile));
    if(!eFile)
    {
        fclose(tFile);
        return 0;
    }

    eFile->file = tFile;
    return eFile;
#endif
}

int embFile_putc(int ch, EmbFile* stream)
{
#ifdef ARDUINO
    return inoFile_putc(ch, stream);
#else /* ARDUINO */
    return fputc(ch, stream->file);
#endif /* ARDUINO */
}

int embFile_printf(EmbFile* stream, const char* format, ...)
{
#ifdef ARDUINO /* ARDUINO */
    char buff[256];
    va_list args;
    va_start(args, format);
    vsprintf(buff, format, args);
    va_end(args);
    return inoFile_printf(stream, buff);
#else /* ARDUINO */
    int retVal;
    va_list args;
    va_start(args, format);
    retVal = vfprintf(stream->file, format, args);
    va_end(args);
    return retVal;
#endif /* ARDUINO */
}

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

bcf_file_fat* bcfFileFat_create(const unsigned int sectorSize)
{
    bcf_file_fat* fat = (bcf_file_fat*)malloc(sizeof(bcf_file_fat));
    if(!fat) { embLog_error("compound-file-fat.c bcfFileFat_create(), cannot allocate memory for fat\n"); } /* TODO: avoid crashing. null pointer will be accessed */
    fat->numberOfEntriesInFatSector = sectorSize / sizeOfFatEntry;
    fat->fatEntryCount = 0;
    return fat;
}

void loadFatFromSector(bcf_file_fat* fat, EmbFile* file)
{
    unsigned int i;
    unsigned int currentNumberOfFatEntries = fat->fatEntryCount;
    unsigned int newSize = currentNumberOfFatEntries + fat->numberOfEntriesInFatSector;
    for(i = currentNumberOfFatEntries; i < newSize; ++i)
    {
        unsigned int fatEntry = binaryReadUInt32(file);
        fat->fatEntries[i] = fatEntry;
    }
    fat->fatEntryCount = newSize;
}

void bcf_file_fat_free(bcf_file_fat* fat)
{
    free(fat);
    fat = 0;
}

/* kate: bom off; indent-mode cstyle; indent-width 4; replace-trailing-space-save on; */
#include "embroidery.h"
#include <string.h>
#include <stdio.h>

bcf_file_header bcfFileHeader_read(EmbFile* file)
{
    bcf_file_header header;
    binaryReadBytes(file, header.signature, 8);
    binaryReadBytes(file, header.CLSID, 16);
    header.minorVersion = (unsigned short)binaryReadUInt16(file);
    header.majorVersion = (unsigned short)binaryReadUInt16(file);
    header.byteOrder = (unsigned short)binaryReadUInt16(file);
    header.sectorShift = (unsigned short)binaryReadUInt16(file);
    header.miniSectorShift = (unsigned short)binaryReadUInt16(file);
    header.reserved1 = (unsigned short)binaryReadUInt16(file);
    header.reserved2 = (unsigned int)binaryReadUInt32(file);
    header.numberOfDirectorySectors = (unsigned int)binaryReadUInt32(file);
    header.numberOfFATSectors = (unsigned int)binaryReadUInt32(file);
    header.firstDirectorySectorLocation = (unsigned int)binaryReadUInt32(file);
    header.transactionSignatureNumber = (unsigned int)binaryReadUInt32(file);
    header.miniStreamCutoffSize = (unsigned int)binaryReadUInt32(file);
    header.firstMiniFATSectorLocation = (unsigned int)binaryReadUInt32(file);
    header.numberOfMiniFatSectors = (unsigned int)binaryReadUInt32(file);
    header.firstDifatSectorLocation = (unsigned int)binaryReadUInt32(file);
    header.numberOfDifatSectors = (unsigned int)binaryReadUInt32(file);
    return header;
}
static const int supportedMinorVersion = 0x003E;
static const int littleEndianByteOrderMark = 0xFFFE;

int bcfFileHeader_isValid(bcf_file_header header)
{
    if(memcmp(header.signature, "\xd0\xcf\x11\xe0\xa1\xb1\x1a\xe1", 8) != 0)
    {
        printf("bad header signature\n");
        return 0;
    }
    return 1;
}


static unsigned int sectorSize(bcf_file* bcfFile)
{
    /* version 3 uses 512 byte */
    if(bcfFile->header.majorVersion == 3)
    {
        return 512;
    }
    else if(bcfFile->header.majorVersion == 4)
    {
        return 4096;
    }
    return 4096;
}

static int haveExtraDIFATSectors(bcf_file* file)
{
    return (int)(numberOfEntriesInDifatSector(file->difat) > 0);
}

static int seekToOffset(EmbFile* file, const unsigned int offset)
{
    return embFile_seek(file, offset, SEEK_SET);
}

static int seekToSector(bcf_file* bcfFile, EmbFile* file, const unsigned int sector)
{
    unsigned int offset = sector * sectorSize(bcfFile) + sectorSize(bcfFile);
    return seekToOffset(file, offset);
}

static void parseDIFATSectors(EmbFile* file, bcf_file* bcfFile)
{
    unsigned int numberOfDifatEntriesStillToRead = bcfFile->header.numberOfFATSectors - NumberOfDifatEntriesInHeader;
    unsigned int difatSectorNumber = bcfFile->header.firstDifatSectorLocation;
    while((difatSectorNumber != CompoundFileSector_EndOfChain) &&
          (numberOfDifatEntriesStillToRead > 0 ))
    {
        seekToSector(bcfFile, file, difatSectorNumber);
        difatSectorNumber = readFullSector(file, bcfFile->difat, &numberOfDifatEntriesStillToRead);
    }
}

int bcfFile_read(EmbFile* file, bcf_file* bcfFile)
{
    unsigned int i, numberOfDirectoryEntriesPerSector, directorySectorToReadFrom;

    bcfFile->header = bcfFileHeader_read(file);
    if(!bcfFileHeader_isValid(bcfFile->header))
    {
        printf( "Failed to parse header\n" );
        return 0;
    }

    bcfFile->difat = bcf_difat_create(file, bcfFile->header.numberOfFATSectors, sectorSize(bcfFile));
    if(haveExtraDIFATSectors(bcfFile))
    {
        parseDIFATSectors(file, bcfFile);
    }

    bcfFile->fat = bcfFileFat_create(sectorSize(bcfFile));
    for(i = 0; i < bcfFile->header.numberOfFATSectors; ++i)
    {
        unsigned int fatSectorNumber = bcfFile->difat->fatSectorEntries[i];
        seekToSector(bcfFile, file, fatSectorNumber);
        loadFatFromSector(bcfFile->fat, file);
    }

    numberOfDirectoryEntriesPerSector = sectorSize(bcfFile) / sizeOfDirectoryEntry;
    bcfFile->directory = CompoundFileDirectory(numberOfDirectoryEntriesPerSector);
    directorySectorToReadFrom = bcfFile->header.firstDirectorySectorLocation;
    while(directorySectorToReadFrom != CompoundFileSector_EndOfChain)
    {
        seekToSector(bcfFile, file, directorySectorToReadFrom);
        readNextSector(file, bcfFile->directory);
        directorySectorToReadFrom = bcfFile->fat->fatEntries[directorySectorToReadFrom];
    }

    return 1;
}

EmbFile* GetFile(bcf_file* bcfFile, EmbFile* file, char* fileToFind)
{
    int filesize, sectorSize, currentSector, sizeToWrite, currentSize, totalSectors, i;
    char* input = 0;
    EmbFile* fileOut = embFile_tmpfile();
    bcf_directory_entry* pointer = bcfFile->directory->dirEntries;
    while(pointer)
    {
        if(strcmp(fileToFind, pointer->directoryEntryName) == 0)
        break;
        pointer = pointer->next;
    }
    filesize = pointer->streamSize;
    sectorSize = bcfFile->difat->sectorSize;
    input = (char*)malloc(sectorSize);
    if(!input) { embLog_error("compound-file.c GetFile(), cannot allocate memory for input\n"); } /* TODO: avoid crashing. null pointer will be accessed */
    currentSize = 0;
    currentSector = pointer->startingSectorLocation;
    totalSectors = (int)ceil((float)filesize / sectorSize);
    for(i = 0; i < totalSectors; i++)
    {
        seekToSector(bcfFile, file, currentSector);
        sizeToWrite = filesize - currentSize;
        if(sectorSize < sizeToWrite)
        {
            sizeToWrite = sectorSize;
        }
        embFile_read(input, 1, sizeToWrite, file);
        embFile_write(input, 1, sizeToWrite, fileOut);
        currentSize += sizeToWrite;
        currentSector = bcfFile->fat->fatEntries[currentSector];
    }
    free(input);
    input = 0;
    return fileOut;
}

void bcf_file_free(bcf_file* bcfFile)
{
    bcf_file_difat_free(bcfFile->difat);
    bcfFile->difat = 0;
    bcf_file_fat_free(bcfFile->fat);
    bcfFile->fat = 0;
    bcf_directory_free(bcfFile->directory);
    bcfFile->directory = 0;
    free(bcfFile);
    bcfFile = 0;
}
