#!/usr/bin/env python
import sys
from libembroidery import *

def usage():

    formatList = 0
    curFormat = 0
    extension =  ' '
    description = ' '
    readerState = ' '
    writerState = ' '
    numReaders = 0
    numWriters = 0
    print(" _____________________________________________________________________________ ")
    print("|          _   _ ___  ___ _____ ___  ___   __  _ ___  ___ ___   _ _           |")
    print("|         | | | | _ \\| __|     | _ \\| _ \\ /  \\| |   \\| __| _ \\ | | |          |")
    print("|         | |_| | _ <| __| | | | _ <|   /| () | | |) | __|   / |__ |          |")
    print("|         |___|_|___/|___|_|_|_|___/|_|\\_\\\\__/|_|___/|___|_|\\_\\|___|          |")
    print("|                    ___  __  ___ _ _    _ ___ ___  _____                     |")
    print("|                   / __|/  \\|   | | \\  / | __| _ \\|_   _|                    |")
    print("|                  ( (__| () | | | |\\ \\/ /| __|   /  | |                      |")
    print("|                   \\___|\\__/|_|___| \\__/ |___|_|\\_\\ |_|                      |")
    print("|                                                                             |")
    print("|                             * Python Version *                              |")
    print("|_____________________________________________________________________________|")
    print("|                                                                             |")
    print("| Usage: python libembroidery-convert.py fileToRead filesToWrite ...          |")
    print("|_____________________________________________________________________________|")
    print("                                                                               ")
    print(" _____________________________________________________________________________ ")
    print("|                                                                             |")
    print("| List of Formats                                                             |")
    print("|_____________________________________________________________________________|")
    print("|                                                                             |")
    print("| 'S' = Yes, and is considered stable.                                        |")
    print("| 'U' = Yes, but may be unstable.                                             |")
    print("| ' ' = No.                                                                   |")
    print("|_____________________________________________________________________________|")
    print("|        |       |       |                                                    |")
    print("| Format | Read  | Write | Description                                        |")
    print("|________|_______|_______|____________________________________________________|")
    print("|        |       |       |                                                    |")


    formatList = embFormatList_create();
    if(not formatList):
        print("libembroidery-convert.py usage(), cannot allocate memory for formatList")
        return
    curFormat = formatList;
    while(curFormat):
        extension = embFormat_extension(curFormat);
        description = embFormat_description(curFormat);
        readerState = embFormat_readerState(curFormat);
        writerState = embFormat_writerState(curFormat);

        if(readerState != ' '): numReaders += 1
        if(writerState != ' '): numWriters += 1
        print("|  %-4s  |   %c   |   %c   |  %-49s |" % (extension, readerState, writerState, description))

        curFormat = curFormat.next;

    embFormatList_free(formatList);
    formatList = 0;

    print("|________|_______|_______|____________________________________________________|")
    print("|        |       |       |                                                    |")
    print("| Total: |  %3d  |  %3d  |                                                    |" % (numReaders, numWriters))
    print("|________|_______|_______|____________________________________________________|")
    print("|                                                                             |")
    print("|                         http://embroidermodder.org                          |")
    print("|_____________________________________________________________________________|")
    print("")

if __name__ == "__main__":
    argc = len(sys.argv)

    if(argc < 3):
        usage()
        exit(0)

    p = embPattern_create();
    if(not p):
        print("libembroidery-convert.py main(), cannot allocate memory for p")
        exit(1)

    successful = embPattern_read(p, sys.argv[1])
    if(not successful):
        print("libembroidery-convert.py main(), reading file %s was unsuccessful" % (sys.argv[1]))
        embPattern_free(p)
        exit(1)

    formatType = embFormat_typeFromName(sys.argv[1])
    if(formatType == EMBFORMAT_OBJECTONLY and argc == 3):
        formatType = embFormat_typeFromName(sys.argv[2])
        if(formatType == EMBFORMAT_STITCHONLY):
            embPattern_movePolylinesToStitchList(p)

    for i in range(2, argc):
        successful = embPattern_write(p, sys.argv[i])
        if(not successful):
            print("libembroidery-convert.py main(), writing file %s was unsuccessful" % (sys.argv[i]))

    embPattern_free(p)

# kate: bom off; indent-mode python; indent-width 4; replace-trailing-space-save on;
