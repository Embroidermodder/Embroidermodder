#include "emb-reader-writer.h"
#include "emb-logging.h"
#include "formats.h"
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

/*static const int formatCount = 59;
static const char* const formats[] = {
".10o", "U", " ", " Toyota Embroidery Format                         ",
".100", "U", " ", " Toyota Embroidery Format                         ",
".art", " ", " ", " Bernina Embroidery Format                        ",
".bmc", " ", " ", " Bitmap Cache Embroidery Format                   ",
".bro", "U", " ", " Bits & Volts Embroidery Format                   ",
".cnd", " ", " ", " Melco Embroidery Format                          ",
".col", "U", "U", " Embroidery Thread Color Format                   ",
".csd", "U", " ", " Singer Embroidery Format                         ",
".csv", "U", "U", " Comma Separated Values                           ",
".dat", "U", " ", " Barudan Embroidery Format                        ",
".dem", " ", " ", " Melco Embroidery Format                          ",
".dsb", "U", " ", " Barudan Embroidery Format                        ",
".dst", "U", "U", " Tajima Embroidery Format                         ",
".dsz", "U", " ", " ZSK USA Embroidery Format                        ",
".dxf", " ", " ", " Drawing Exchange Format                          ",
".edr", "U", "U", " Embird Embroidery Format                         ",
".emd", "U", " ", " Elna Embroidery Format                           ",
".exp", "U", "U", " Melco Embroidery Format                          ",
".exy", "U", " ", " Eltac Embroidery Format                          ",
".eys", " ", " ", " Sierra Expanded Embroidery Format                ",
".fxy", "U", " ", " Fortron Embroidery Format                        ",
".gnc", " ", " ", " Great Notions Embroidery Format                  ",
".gt ", "U", " ", " Gold Thread Embroidery Format                    ",
".hus", "U", "U", " Husqvarna Viking Embroidery Format               ",
".inb", "U", " ", " Inbro Embroidery Format                          ",
".inf", "U", "U", " Embroidery Color Format                          ",
".jef", "U", "U", " Janome Embroidery Format                         ",
".ksm", "U", "U", " Pfaff Embroidery Format                          ",
".max", "U", " ", " Pfaff Embroidery Format                          ",
".mit", "U", " ", " Mitsubishi Embroidery Format                     ",
".new", "U", " ", " Ameco Embroidery Format                          ",
".ofm", "U", " ", " Melco Embroidery Format                          ",
".pcd", "U", "U", " Pfaff Embroidery Format                          ",
".pcm", "U", " ", " Pfaff Embroidery Format                          ",
".pcq", "U", "U", " Pfaff Embroidery Format                          ",
".pcs", "U", "U", " Pfaff Embroidery Format                          ",
".pec", "U", "U", " Brother Embroidery Format                        ",
".pel", " ", " ", " Brother Embroidery Format                        ",
".pem", " ", " ", " Brother Embroidery Format                        ",
".pes", "U", "U", " Brother Embroidery Format                        ",
".phb", "U", " ", " Brother Embroidery Format                        ",
".phc", "U", " ", " Brother Embroidery Format                        ",
".plt", "U", "U", " AutoCAD plot drawing                             ",
".rgb", "U", "U", " RGB Embroidery Format                            ",
".sew", "U", " ", " Janome Embroidery Format                         ",
".shv", "U", " ", " Husqvarna Viking Embroidery Format               ",
".sst", "U", " ", " Sunstar Embroidery Format                        ",
".stx", "U", " ", " Data Stitch Embroidery Format                    ",
".svg", "U", "U", " Scalable Vector Graphics                         ",
".t09", "U", " ", " Pfaff Embroidery Format                          ",
".tap", "U", " ", " Happy Embroidery Format                          ",
".thr", "U", "U", " ThredWorks Embroidery Format                     ",
".txt", " ", "U", " Text File                                        ",
".u00", "U", " ", " Barudan Embroidery Format                        ",
".u01", " ", " ", " Barudan Embroidery Format                        ",
".vip", "U", " ", " Pfaff Embroidery Format                          ",
".vp3", "U", " ", " Pfaff Embroidery Format                          ",
".xxx", "U", "U", " Singer Embroidery Format                         ",
".zsk", "U", " ", " ZSK USA Embroidery Format                        "
};*/

void usage(void)
{
    EmbFormatList* formatList;
    EmbFormat* cur;    
    char* extension = NULL;
    char* description = NULL;
    char readerState;
    char writerState;
    int type;
    int readers = 0;
    int writers = 0;
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
    /*for(i = 0; i < formatCount; i++)
    {
        printf("|  %s  |   %s   |   %s   | %s |\n", formats[i*4], formats[i*4+1], formats[i*4+2], formats[i*4+3]);
    }*/


    formatList = embFormatList_create();
    cur = formatList->firstFormat;
    while (cur != NULL) {
        if (embFormat_info(cur->extension, &extension, &description, &readerState, &writerState, &type)){
            readers += readerState != ' '? 1 : 0;
            writers += writerState != ' '? 1 : 0;
            printf("|  %-4s  |   %c   |   %c   |  %-49s |\n", extension, readerState, writerState, description);
        }
        cur = cur->next;
    }
    embFormatList_free(formatList);

    printf("|        |       |       |                                                    |\n");
    printf("|________|_______|_______|____________________________________________________|\n");
    printf("|        |       |       |                                                    |\n");
    printf("| Total: |  %3d  |  %3d  |                                                    |\n",
           readers, writers);
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
