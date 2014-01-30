/*! @file compound-file-common.h */
#ifndef COMPOUND_FILE_COMMON_H
#define COMPOUND_FILE_COMMON_H

#include "api-start.h"
#ifdef __cplusplus
extern "C" {
#endif

/**
  Type of sector
*/
#define CompoundFileSector_MaxRegSector 0xFFFFFFFA
#define CompoundFileSector_DIFAT_Sector 0xFFFFFFFC
#define CompoundFileSector_FAT_Sector   0xFFFFFFFD
#define CompoundFileSector_EndOfChain   0xFFFFFFFE
#define CompoundFileSector_FreeSector   0xFFFFFFFF

/**
  Type of directory object
*/
#define ObjectTypeUnknown   0x00 /*!< Probably unallocated    */
#define ObjectTypeStorage   0x01 /*!< a directory type object */
#define ObjectTypeStream    0x02 /*!< a file type object      */
#define ObjectTypeRootEntry 0x05 /*!< the root entry          */

/**
  Special values for Stream Identifiers
*/
#define CompoundFileStreamId_MaxRegularStreamId 0xFFFFFFFA /*!< All real stream Ids are less than this */
#define CompoundFileStreamId_NoStream           0xFFFFFFFF /*!< There is no valid stream Id            */

#ifdef __cplusplus
}
#endif /* __cplusplus */
#include "api-stop.h"

#endif /* COMPOUND_FILE_COMMON_H */

/* kate: bom off; indent-mode cstyle; indent-width 4; replace-trailing-space-save on; */

