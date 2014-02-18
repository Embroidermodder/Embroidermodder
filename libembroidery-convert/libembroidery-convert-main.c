#include "emb-reader-writer.h"
#include "emb-logging.h"
#include "emb-format.h"
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

void usage(void)
{
    EmbFormatList* formatList = 0;
    EmbFormatList* curFormat = 0;
    char* extension = 0;
    char* description = 0;
    char readerState;
    char writerState;
    int type;
    int numReaders = 0;
    int numWriters = 0;
    printf(" _____________________________________________________________________________ \n");
    printf("|          _   _ ___  ___ _____ ___  ___   __  _ ___  ___ ___   _ _           |\n");
    printf("|         | | | | _ \\| __|     | _ \\| _ \\ /  \\| |   \\| __| _ \\ | | |          |\n");
    printf("|         | |_| | _ <| __| | | | _ <|   /| () | | |) | __|   / |__ |          |\n");
    printf("|         |___|_|___/|___|_|_|_|___/|_|\\_\\\\__/|_|___/|___|_|\\_\\|___|          |\n");
    printf("|                    ___  __  ___ _ _    _ ___ ___  _____                     |\n");
    printf("|                   / __|/  \\|   | | \\  / | __| _ \\|_   _|                    |\n");
    printf("|                  ( (__| () | | | |\\ \\/ /| __|   /  | |                      |\n");
    printf("|                   \\___|\\__/|_|___| \\__/ |___|_|\\_\\ |_|                      |\n");
    printf("|_____________________________________________________________________________|\n");
    printf("|                                                                             |\n");
    printf("| Usage: libembroidery-convert fileToRead filesToWrite ...                    |\n");
    printf("|_____________________________________________________________________________|\n");
    printf("                                                                               \n");
    printf(" _____________________________________________________________________________ \n");
    printf("|                                                                             |\n");
    printf("| List of Formats                                                             |\n");
    printf("|_____________________________________________________________________________|\n");
    printf("|                                                                             |\n");
    printf("| 'S' = Yes, and is considered stable.                                        |\n");
    printf("| 'U' = Yes, but may be unstable.                                             |\n");
    printf("| ' ' = No.                                                                   |\n");
    printf("|_____________________________________________________________________________|\n");
    printf("|        |       |       |                                                    |\n");
    printf("| Format | Read  | Write | Description                                        |\n");
    printf("|________|_______|_______|____________________________________________________|\n");
    printf("|        |       |       |                                                    |\n");

    formatList = embFormatList_create();
    if(!formatList) { embLog_error("libembroidery-convert-main.c usage(), cannot allocate memory for formatList\n"); return; }
    curFormat = formatList;
    while(curFormat)
    {
        if(embFormat_info(curFormat->extension, &extension, &description, &readerState, &writerState, &type))
        {
            numReaders += readerState != ' '? 1 : 0;
            numWriters += writerState != ' '? 1 : 0;
            printf("|  %-4s  |   %c   |   %c   |  %-49s |\n", extension, readerState, writerState, description);
        }
        curFormat = curFormat->next;
    }
    embFormatList_free(formatList);
    formatList = 0;

    printf("|________|_______|_______|____________________________________________________|\n");
    printf("|        |       |       |                                                    |\n");
    printf("| Total: |  %3d  |  %3d  |                                                    |\n", numReaders, numWriters);
    printf("|________|_______|_______|____________________________________________________|\n");
    printf("|                                                                             |\n");
    printf("|                   http://embroidermodder.github.io                          |\n");
    printf("|_____________________________________________________________________________|\n");
    printf("\n");
}

/* TODO: Add capability for converting multiple files of various types to a single format. Currently, we only convert a single file to multiple formats. */

/*! Developers incorporating libembroidery into another project should use the SHORT_WAY of using libembroidery. It uses
 *  convenience functions and is approximately 20 lines shorter than the long way.
 *
 *  Developers who are interested improving libembroidery or using it to its fullest extent should use the LONG_WAY.
 *  It essentially does the same work the convenience function do.
 *
 *  Both methods are acceptable and it is up to you which to choose. Both will stay here for regression testing.
 */

#define SHORT_WAY

int main(int argc, const char* argv[])
{
#ifdef SHORT_WAY
    EmbPattern* p = 0;
    int successful = 0, i = 0;
    if(argc < 3)
    {
        usage();
        exit(0);
    }

    p = embPattern_create();
    if(!p) { embLog_error("libembroidery-convert-main.c main(), cannot allocate memory for p\n"); exit(1); }

    successful = embPattern_read(p, argv[1]);
    if(!successful)
    {
        embLog_error("libembroidery-convert-main.c main(), reading file %s was unsuccessful\n", argv[1]);
        embPattern_free(p);
        exit(1);
    }

    if(embFormat_type(argv[1]) == EMBFORMAT_OBJECTONLY && argc == 3) /* TODO: fix this to work when writing multiple files */
    {
        if(embFormat_type(argv[2]) == EMBFORMAT_STITCHONLY)
            embPattern_movePolylinesToStitchList(p);
    }

    i = 2;
    for(i = 2; i < argc; i++)
    {
        successful = embPattern_write(p, argv[i]);
        if(!successful)
            embLog_error("libembroidery-convert-main.c main(), writing file %s was unsuccessful\n", argv[i]);
    }

    embPattern_free(p);
    return 0;
#else /* LONG_WAY */
    EmbPattern* p = 0;
    EmbReaderWriter* reader = 0, *writer = 0;
    int successful = 0, i = 0;
    if(argc < 3)
    {
        usage();
        exit(0);
    }

    p = embPattern_create();
    if(!p) { embLog_error("libembroidery-convert-main.c main(), cannot allocate memory for p\n"); exit(1); }

    successful = 0;
    reader = embReaderWriter_getByFileName(argv[1]);
    if(!reader)
    {
        successful = 0;
        embLog_error("libembroidery-convert-main.c main(), unsupported read file type: %s\n", argv[1]);
    }
    else
    {
        successful = reader->reader(p, argv[1]);
        if(!successful) embLog_error("libembroidery-convert-main.c main(), reading file was unsuccessful: %s\n", argv[1]);
    }
    free(reader);
    if(!successful)
    {
        embPattern_free(p);
        exit(1);
    }

    if(embFormat_type(argv[1]) == EMBFORMAT_OBJECTONLY && argc == 3) /* TODO: fix this to work when writing multiple files */
    {
        if(embFormat_type(argv[2]) == EMBFORMAT_STITCHONLY)
            embPattern_movePolylinesToStitchList(p);
    }

    i = 2;
    for(i = 2; i < argc; i++)
    {
        writer = embReaderWriter_getByFileName(argv[i]);
        if(!writer)
        {
            embLog_error("libembroidery-convert-main.c main(), unsupported write file type: %s\n", argv[i]);
        }
        else
        {
            successful = writer->writer(p, argv[i]);
            if(!successful)
                embLog_error("libembroidery-convert-main.c main(), writing file %s was unsuccessful\n", argv[i]);
        }
        free(writer);
    }

    embPattern_free(p);
    return 0;
#endif /* SHORT_WAY */

}

/* kate: bom off; indent-mode cstyle; indent-width 4; replace-trailing-space-save on; */
