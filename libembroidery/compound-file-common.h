#ifndef COMPOUND_FILE_COMMON_H
#define COMPOUND_FILE_COMMON_H

#ifdef __cplusplus
extern "C" {
#endif

/**
  Type of sector
*/
enum CompoundFileSectorTypes
{
    CompoundFileSector_MaxRegSector = 0xFFFFFFFA,
    CompoundFileSector_DIFAT_Sector = 0xFFFFFFFC,
    CompoundFileSector_FAT_Sector   = 0xFFFFFFFD,
    CompoundFileSector_EndOfChain   = 0xFFFFFFFE,
    CompoundFileSector_FreeSector   = 0xFFFFFFFF
};

/**
  Type of directory object
*/
enum DirectoryObjectType
{
    ObjectTypeUnknown   = 0x00, /*!< Probably unallocated    */
    ObjectTypeStorage   = 0x01, /*!< a directory type object */
    ObjectTypeStream    = 0x02, /*!< a file type object      */
    ObjectTypeRootEntry = 0x05  /*!< the root entry          */
};

/**
  Special values for Stream Identifiers
*/
enum CompoundFileStreamIds
{
    CompoundFileStreamId_MaxRegularStreamId = 0xFFFFFFFA, /*!< All real stream Ids are less than this */
    CompoundFileStreamId_NoStream           = 0xFFFFFFFF  /*!< There is no valid stream Id            */
};

#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif /* COMPOUND_FILE_COMMON_H */

/* kate: bom off; indent-mode cstyle; indent-width 4; replace-trailing-space-save on; */

