/**
 * \file emb-file.c
 */

#include "embroidery_internal.h"

#include <stdarg.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <math.h>

static unsigned int sizeOfDirectoryEntry = 128;
const unsigned int sizeOfFatEntry = sizeof(unsigned int);
static const unsigned int sizeOfDifatEntry = 4;
static const unsigned int sizeOfChainingEntryAtEndOfDifatSector = 4;

/* Returns an EmbColor. It is created on the stack. */
EmbColor embColor_make(unsigned char r, unsigned char g, unsigned char b)
{
    EmbColor stackColor;
    stackColor.r = r;
    stackColor.g = g;
    stackColor.b = b;
    return stackColor;
}

/* Returns a pointer to an EmbColor. It is created on the heap. The caller is responsible for freeing the allocated memory. */
EmbColor* embColor_create(unsigned char r, unsigned char g, unsigned char b)
{
    EmbColor* heapColor = (EmbColor*)malloc(sizeof(EmbColor));
    if(!heapColor) { embLog_error("emb-color.c embColor_create(), cannot allocate memory for heapColor\n"); return 0; }
    heapColor->r = r;
    heapColor->g = g;
    heapColor->b = b;
    return heapColor;
}

/* Converts a 6 digit hex string (I.E. "00FF00") into an EmbColor and returns it. */
EmbColor embColor_fromHexStr(char* val)
{
    EmbColor color;
    char r[3];
    char g[3];
    char b[3];

    r[0] = val[0];
    r[1] = val[1];
    r[2] = 0;

    g[0] = val[2];
    g[1] = val[3];
    g[2] = 0;

    b[0] = val[4];
    b[1] = val[5];
    b[2] = 0;

    color.r = (unsigned char)strtol(r, 0, 16);
    color.g = (unsigned char)strtol(g, 0, 16);
    color.b = (unsigned char)strtol(b, 0, 16);
    return color;
}


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

/* This file contains wrapper functions around Keith Pomakis' HashTable Library */

EmbHash* embHash_create(void) { return HashTableCreate(1); }
void embHash_free(EmbHash* hash) { HashTableDestroy(hash); hash = 0; }

int embHash_contains(const EmbHash* hash, const void* key) { return HashTableContainsKey(hash, key); }
int embHash_insert(EmbHash* hash, const void* key, void* value) { return HashTablePut(hash, key, value); }
void* embHash_value(const EmbHash* hash, const void* key) { return HashTableGet(hash, key); }
void embHash_remove(EmbHash* hash, const void *key) { HashTableRemove(hash, key); }
void embHash_clear(EmbHash* hash) { HashTableRemoveAll(hash); }
int embHash_empty(const EmbHash* hash) { return HashTableIsEmpty(hash); }
long embHash_count(const EmbHash* hash) { return HashTableSize(hash); }
void embHash_rehash(EmbHash* hash, long numOfBuckets) { HashTableRehash(hash, numOfBuckets); }

/**************************************************/
/* EmbHoop                                        */
/**************************************************/

double embHoop_width(EmbHoop hoop)
{
    return hoop.width;
}

double embHoop_height(EmbHoop hoop)
{
    return hoop.height;
}


/* printf() abstraction. Uses Serial.print() on ARDUINO */
void embLog_print(const char* format, ...)
{
    /* TODO: log debug message in struct for later use */

#ifdef ARDUINO /* ARDUINO */
    char buff[256];
    va_list args;
    va_start(args, format);
    vsprintf(buff, format, args);
    va_end(args);
    inoLog_serial(buff);
#else /* ARDUINO */
    va_list args;
    va_start(args, format);
    vprintf(format, args);
    va_end(args);
#endif /* ARDUINO */
}

/* serious errors */
void embLog_error(const char* format, ...)
{
    /* TODO: log debug message in struct for later use */

#ifdef ARDUINO /* ARDUINO */
    char buff[256];
    va_list args;
    va_start(args, format);
    vsprintf(buff, format, args);
    va_end(args);
    inoLog_serial(strcat("ERROR: ", buff));
#else /* ARDUINO */
    va_list args;
    va_start(args, format);
    printf("ERROR: ");
    vprintf(format, args);
    va_end(args);
#endif /* ARDUINO */
}


EmbStitchList* embStitchList_create(EmbStitch data)
{
    EmbStitchList* heapStitchList = (EmbStitchList*)malloc(sizeof(EmbStitchList));
    if(!heapStitchList) { embLog_error("emb-stitch.c embStitchList_create(), cannot allocate memory for heapStitchList\n"); return 0; }
    heapStitchList->stitch = data;
    heapStitchList->next = 0;
    return heapStitchList;
}

EmbStitchList* embStitchList_add(EmbStitchList* pointer, EmbStitch data)
{
    if(!pointer) { embLog_error("emb-stitch.c embStitchList_add(), pointer argument is null\n"); return 0; }
    if(pointer->next) { embLog_error("emb-stitch.c embStitchList_add(), pointer->next should be null\n"); return 0; }
    pointer->next = (EmbStitchList*)malloc(sizeof(EmbStitchList));
    if(!pointer->next) { embLog_error("emb-stitch.c embStitchList_add(), cannot allocate memory for pointer->next\n"); return 0; }
    pointer = pointer->next;
    pointer->stitch = data;
    pointer->next = 0;
    return pointer;
}

EmbStitch embStitchList_getAt(EmbStitchList* pointer, int num)
{
    /* TODO: pointer safety */
    int i;
    for(i = 0; i < num; i++)
    {
        pointer = pointer->next;
    }
    return pointer->stitch;
}

/* TODO: Add a default parameter to handle returning count based on stitch flags. Currently, it includes JUMP and TRIM stitches, maybe we just want NORMAL stitches only or vice versa */
int embStitchList_count(EmbStitchList* pointer)
{
    int i = 1;
    if(!pointer) return 0;
    while(pointer->next)
    {
        pointer = pointer->next;
        i++;
    }
    return i;
}

int embStitchList_empty(EmbStitchList* pointer)
{
    if(!pointer)
        return 1;
    return 0;
}

void embStitchList_free(EmbStitchList* pointer)
{
    EmbStitchList* tempPointer = pointer;
    EmbStitchList* nextPointer = 0;
    while(tempPointer)
    {
        nextPointer = tempPointer->next;
        free(tempPointer);
        tempPointer = nextPointer;
    }
    pointer = 0;
}


int embThread_findNearestColor(EmbColor color, EmbThreadList* colors)
{
    double currentClosestValue = 9999999;
    int closestIndex = -1;
    int red = color.r;
    int green = color.g;
    int blue = color.b;
    int i = 0;
    EmbThreadList* currentThreadItem = colors;

    while(currentThreadItem != NULL)
    {
        int deltaRed;
        int deltaBlue;
        int deltaGreen;
        double dist;
        EmbColor c;
        c = currentThreadItem->thread.color;

        deltaRed = red - c.r;
        deltaBlue = green - c.g;
        deltaGreen = blue - c.b;

        dist = sqrt((double)(deltaRed * deltaRed) + (deltaBlue * deltaBlue) + (deltaGreen * deltaGreen));
        if(dist <= currentClosestValue)
        {
            currentClosestValue = dist;
            closestIndex = i;
        }
        currentThreadItem = currentThreadItem->next;
        i++;
    }
    return closestIndex;
}

int embThread_findNearestColorInArray(EmbColor color, EmbThread* colorArray, int count)
{
    double currentClosestValue = 9999999;
    int closestIndex = -1;
    int red = color.r;
    int green = color.g;
    int blue = color.b;
    int i = 0;
    for(i = 0; i < count; i++)
    {
        int deltaRed;
        int deltaBlue;
        int deltaGreen;
        double dist;
        EmbColor c;
        c = colorArray[i].color;

        deltaRed = red - c.r;
        deltaBlue = green - c.g;
        deltaGreen = blue - c.b;

        dist = sqrt((double)(deltaRed * deltaRed) + (deltaBlue * deltaBlue) + (deltaGreen * deltaGreen));
        if(dist <= currentClosestValue)
        {
            currentClosestValue = dist;
            closestIndex = i;
        }
    }
    return closestIndex;
}

EmbThread embThread_getRandom(void)
{
    EmbThread c;
    c.color.r = rand()%256;
    c.color.g = rand()%256;
    c.color.b = rand()%256;
    c.description = "random";
    c.catalogNumber = "";
    return c;
}

EmbThreadList* embThreadList_create(EmbThread data)
{
    EmbThreadList* heapThreadList = (EmbThreadList*)malloc(sizeof(EmbThreadList));
    if(!heapThreadList) { embLog_error("emb-thread.c embThreadList_create(), cannot allocate memory for heapThreadList\n"); return 0; }
    heapThreadList->thread = data;
    heapThreadList->next = 0;
    return heapThreadList;
}

EmbThreadList* embThreadList_add(EmbThreadList* pointer, EmbThread data)
{
    if(!pointer) { embLog_error("emb-thread.c embThreadList_add(), pointer argument is null\n"); return 0; }
    if(pointer->next) { embLog_error("emb-thread.c embThreadList_add(), pointer->next should be null\n"); return 0; }
    pointer->next = (EmbThreadList*)malloc(sizeof(EmbThreadList));
    if(!pointer->next) { embLog_error("emb-thread.c embThreadList_add(), cannot allocate memory for pointer->next\n"); return 0; }
    pointer = pointer->next;
    pointer->thread = data;
    pointer->next = 0;
    return pointer;
}

EmbThread embThreadList_getAt(EmbThreadList* pointer, int num)
{
    /* TODO: pointer safety */
    int i = 0;
    for(i = 0; i < num; i++)
    {
        if(pointer->next)
        {
            pointer = pointer->next;
        }
    }
    return pointer->thread;
}

int embThreadList_count(EmbThreadList* pointer)
{
    int i = 1;
    if(!pointer) return 0;
    while(pointer->next)
    {
        pointer = pointer->next;
        i++;
    }
    return i;
}

int embThreadList_empty(EmbThreadList* pointer)
{
    if(!pointer)
        return 1;
    return 0;
}

void embThreadList_free(EmbThreadList* pointer)
{
    EmbThreadList* tempPointer = pointer;
    EmbThreadList* nextPointer = 0;
    while(tempPointer)
    {
        nextPointer = tempPointer->next;
        free(tempPointer);
        tempPointer = nextPointer;
    }
	pointer = 0;
}

#ifdef ARDUINO
/*TODO: arduino embTime includes */
#else
#include <time.h>
#endif

void embTime_initNow(EmbTime* t)
{
#ifdef ARDUINO
/*TODO: arduino embTime_initNow */
#else
    time_t rawtime;
    struct tm* timeinfo;
    time(&rawtime);
    timeinfo = localtime(&rawtime);

    t->year   = timeinfo->tm_year;
    t->month  = timeinfo->tm_mon;
    t->day    = timeinfo->tm_mday;
    t->hour   = timeinfo->tm_hour;
    t->minute = timeinfo->tm_min;
    t->second = timeinfo->tm_sec;
#endif /* ARDUINO */
}

EmbTime embTime_time(EmbTime* t)
{
#ifdef ARDUINO
/*TODO: arduino embTime_time */
#else

int divideByZero = 0;
divideByZero = divideByZero/divideByZero; /*TODO: wrap time() from time.h and verify it works consistently */

#endif /* ARDUINO */
}

/*! Initializes and returns an EmbSettings */
EmbSettings embSettings_init(void)
{
    EmbSettings settings;
    settings.dstJumpsPerTrim = 6;
    settings.home = embPoint_make(0.0, 0.0);
    return settings;
}

/*! Returns the home position stored in (\a settings) as an EmbPoint (\a point). */
EmbPoint embSettings_home(EmbSettings* settings)
{
    return settings->home;
}

/*! Sets the home position stored in (\a settings) to EmbPoint (\a point). You will rarely ever need to use this. */
void embSettings_setHome(EmbSettings* settings, EmbPoint point)
{
    settings->home = point;
}

/**************************************************/
/* EmbFlagList                                    */
/**************************************************/

EmbFlagList* embFlagList_create(EmbFlag data)
{
    EmbFlagList* heapFlagList = (EmbFlagList*)malloc(sizeof(EmbFlagList));
    if(!heapFlagList) { embLog_error("emb-flag.c embFlagList_create(), cannot allocate memory for heapFlagList\n"); return 0; }
    heapFlagList->flag = data;
    heapFlagList->next = 0;
    return heapFlagList;
}

EmbFlagList* embFlagList_add(EmbFlagList* pointer, EmbFlag data)
{
    if(!pointer) { embLog_error("emb-flag.c embFlagList_add(), pointer argument is null\n"); return 0; }
    if(pointer->next) { embLog_error("emb-flag.c embFlagList_add(), pointer->next should be null\n"); return 0; }
    pointer->next = (EmbFlagList*)malloc(sizeof(EmbFlagList));
    if(!pointer->next) { embLog_error("emb-flag.c embFlagList_add(), cannot allocate memory for pointer->next\n"); return 0; }
    pointer = pointer->next;
    pointer->flag = data;
    pointer->next = 0;
    return pointer;
}

int embFlagList_count(EmbFlagList* pointer)
{
    int i = 1;
    if(!pointer) return 0;
    while(pointer->next)
    {
        pointer = pointer->next;
        i++;
    }
    return i;
}

int embFlagList_empty(EmbFlagList* pointer)
{
    if(!pointer)
        return 1;
    return 0;
}

void embFlagList_free(EmbFlagList* pointer)
{
    EmbFlagList* tempPointer = pointer;
    EmbFlagList* nextPointer = 0;
    while(tempPointer)
    {
        nextPointer = tempPointer->next;
        free(tempPointer);
        tempPointer = nextPointer;
    }
    pointer = 0;
}
