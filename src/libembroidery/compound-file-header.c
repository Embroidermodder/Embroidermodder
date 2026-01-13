#include "compound-file-header.h"
#include "helpers-binary.h"
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

/* kate: bom off; indent-mode cstyle; indent-width 4; replace-trailing-space-save on; */
