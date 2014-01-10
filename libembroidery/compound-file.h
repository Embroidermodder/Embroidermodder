#ifndef COMPOUND_FILE_H
#define COMPOUND_FILE_H

#ifdef __cplusplus
extern "C" {
#endif

#include "compound-file-difat.h"
#include "compound-file-directory.h"
#include "compound-file-fat.h"
#include "compound-file-header.h"
#include <stdio.h> /* TODO: replace this with "emb-file.h" when FILE is ported to EmbFile */

typedef struct _bcf_file
{
    bcf_file_header header;   /*! The header for the CompoundFile */
    bcf_file_difat* difat;    /*! The "Double Indirect FAT" for the CompoundFile */
    bcf_file_fat* fat;        /*! The File Allocation Table for the Compound File */
    bcf_directory* directory; /*! The directory for the CompoundFile */
} bcf_file;

int bcfFile_read(FILE* file, bcf_file* bcfFile);
FILE* GetFile(bcf_file* bcfFile, FILE* file, char* fileToFind);
void bcf_file_free(bcf_file* bcfFile);

#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif /* COMPOUND_FILE_H */

/* kate: bom off; indent-mode cstyle; indent-width 4; replace-trailing-space-save on; */
