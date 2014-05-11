#!/usr/bin/lua

require "libembroidery"

printf = function(s,...) return io.write(s:format(...)) end

function usage()
    local formatList
    local curFormat
    local extension
    local description
    local readerState
    local writerState
    local formatType
    local numReaders = 0
    local numWriters = 0

    printf(" _____________________________________________________________________________ \n");
    printf("|          _   _ ___  ___ _____ ___  ___   __  _ ___  ___ ___   _ _           |\n");
    printf("|         | | | | _ \\| __|     | _ \\| _ \\ /  \\| |   \\| __| _ \\ | | |          |\n");
    printf("|         | |_| | _ <| __| | | | _ <|   /| () | | |) | __|   / |__ |          |\n");
    printf("|         |___|_|___/|___|_|_|_|___/|_|\\_\\\\__/|_|___/|___|_|\\_\\|___|          |\n");
    printf("|                    ___  __  ___ _ _    _ ___ ___  _____                     |\n");
    printf("|                   / __|/  \\|   | | \\  / | __| _ \\|_   _|                    |\n");
    printf("|                  ( (__| () | | | |\\ \\/ /| __|   /  | |                      |\n");
    printf("|                   \\___|\\__/|_|___| \\__/ |___|_|\\_\\ |_|                      |\n");
    printf('|                                                                             |\n');
    printf('|                               * Lua Version *                               |\n');
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

    formatList = libembroidery.embFormatList_create();
    if(formatList == 0) then
        embLog_error("libembroidery-convert-main.c usage(), cannot allocate memory for formatList\n"); do return end
    end

    curFormat = formatList;
    while curFormat do
        printf("|  %-4s  |   %s   |   %s   |  %-49s |\n", "TODO", " ", " ", "TODO") --TODO: remove this line when embFormat part below is completed
        --if(libembroidery.embFormat_info(curFormat->extension, &extension, &description, &readerState, &writerState, &formatType)) then --TODO: See here: http://www.lua.org/pil/27.2.html
            --numReaders += readerState != ' '? 1 : 0;
            --numWriters += writerState != ' '? 1 : 0;
            --printf("|  %-4s  |   %c   |   %c   |  %-49s |\n", extension, readerState, writerState, description)
        --end
        curFormat = curFormat.next;
    end
    libembroidery.embFormatList_free(formatList);
    formatList = 0

    printf("|________|_______|_______|____________________________________________________|\n");
    printf("|        |       |       |                                                    |\n");
    printf("| Total: |  %3d  |  %3d  |                                                    |\n", numReaders, numWriters);
    printf("|________|_______|_______|____________________________________________________|\n");
    printf("|                                                                             |\n");
    printf("|                   http://embroidermodder.github.io                          |\n");
    printf("|_____________________________________________________________________________|\n");
    printf("\n");
end

function main()
    local p
    local successful

    if(#arg < 2) then
        usage()
        os.exit()
    end

    p = libembroidery.embPattern_create()
    successful = libembroidery.embPattern_read(p,arg[1])
    successful = libembroidery.embPattern_write(p,arg[2]) --TODO: capability to write out more than 1 file at a time
    libembroidery.embPattern_free(p)
end

main()

-- kate: bom off; indent-mode cstyle; indent-width 4; replace-trailing-space-save on;
