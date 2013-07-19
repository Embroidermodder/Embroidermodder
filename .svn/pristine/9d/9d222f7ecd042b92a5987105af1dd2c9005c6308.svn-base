#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "emb-reader-writer.h"

static const int formatCount = 53;
static const char* const formats[] = {
".10o", "U", " ", " Mitsubishi Embroidery Format                     ",
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
".jef", "U", "U", " Janome Embroidery Format                         ",
".ksm", "U", "U", " Pfaff Embroidery Format                          ",
".pcd", "U", "U", " Pfaff Embroidery Format                          ",
".pcm", "U", " ", " Pfaff Embroidery Format                          ",
".pcq", " ", " ", " Pfaff Embroidery Format                          ",
".pcs", "U", "U", " Pfaff Embroidery Format                          ",
".pec", "U", "U", " Brother Embroidery Format                        ",
".pel", " ", " ", " Brother Embroidery Format                        ",
".pem", " ", " ", " Brother Embroidery Format                        ",
".pes", "U", "U", " Brother Embroidery Format                        ",
".phb", " ", " ", " Brother Embroidery Format                        ",
".phc", " ", " ", " Brother Embroidery Format                        ",
".rgb", "U", "U", " RGB Embroidery Format                            ",
".sew", "U", " ", " Janome Embroidery Format                         ",
".shv", "U", " ", " Husqvarna Viking Embroidery Format               ",
".sst", " ", " ", " Sunstar Embroidery Format                        ",
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
};

void usage()
{
    int i = 0;

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
    for(i = 0; i < formatCount; i++)
    {
        printf("|  %s  |   %s   |   %s   | %s |\n", formats[i*4], formats[i*4+1], formats[i*4+2], formats[i*4+3]);
    }
    printf("|________|_______|_______|____________________________________________________|\n");
    printf("|                                                                             |\n");
    printf("|                   http://embroidermodder.sourceforge.net                    |\n");
    printf("|_____________________________________________________________________________|\n");
    printf("\n");
}

int main(int argc, const char* argv[])
{
    EmbPattern* p = 0;
    EmbReaderWriter* reader = 0;
    EmbReaderWriter* writer = 0;
    int readSuccessful, writeSuccessful, i;
    if(argc < 3)
    {
        usage();
        exit(0);
    }

    p = embPattern_create();
    if(!p) { printf("Could not allocate memory for embroidery pattern\n"); exit(1); }

    /* Read */
    readSuccessful = 0;
    reader = embReaderWriter_getByFileName(argv[1]);
    if(!reader)
    {
        readSuccessful = 0;
        printf("Unsupported read file type: %s\n", argv[1]);
    }
    else
    {
        readSuccessful = reader->reader(p, argv[1]);
        if(!readSuccessful) printf("Reading file was unsuccessful: %s\n", argv[1]);
    }
    free(reader);
    if(!readSuccessful)
    {
        embPattern_free(p);
        exit(1);
    }

    /* Write */
    i = 2;
    for(i = 2; i < argc; i++)
    {
        writer = embReaderWriter_getByFileName(argv[i]);
        if(!writer)
        {
            printf("Unsupported write file type: %s\n", argv[i]);
        }
        else
        {
            writeSuccessful = writer->writer(p, argv[i]);
            if(!writeSuccessful) printf("Writing file %s was unsuccessful\n", argv[i]);
        }
        free(writer);
    }

    embPattern_free(p);
    return 0;
}

/* kate: bom off; indent-mode cstyle; indent-width 4; replace-trailing-space-save on; */
