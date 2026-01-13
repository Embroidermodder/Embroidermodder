/*! @file compound-file.h */
#ifndef COMPOUND_FILE_H
#define COMPOUND_FILE_H

#include "compound-file-difat.h"
#include "compound-file-directory.h"
#include "compound-file-fat.h"
#include "compound-file-header.h"
#include "emb-file.h"

#include "api-start.h"
#ifdef __cplusplus
extern "C" {
#endif

typedef struct _bcf_file
{
    bcf_file_header header;   /*! The header for the CompoundFile */
    bcf_file_difat* difat;    /*! The "Double Indirect FAT" for the CompoundFile */
    bcf_file_fat* fat;        /*! The File Allocation Table for the Compound File */
    bcf_directory* directory; /*! The directory for the CompoundFile */
} bcf_file;

extern EMB_PRIVATE int EMB_CALL bcfFile_read(EmbFile* file, bcf_file* bcfFile);
extern EMB_PRIVATE EmbFile* EMB_CALL GetFile(bcf_file* bcfFile, EmbFile* file, char* fileToFind);
extern EMB_PRIVATE void EMB_CALL bcf_file_free(bcf_file* bcfFile);

#ifdef __cplusplus
}
#endif /* __cplusplus */
#include "api-stop.h"

#endif /* COMPOUND_FILE_H */

/* kate: bom off; indent-mode cstyle; indent-width 4; replace-trailing-space-save on; */
