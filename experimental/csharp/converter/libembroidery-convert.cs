class LibEmbroideryConvert
{
    public static void usage()
    {
        System.Console.WriteLine(" _____________________________________________________________________________ ");
        System.Console.WriteLine("|          _   _ ___  ___ _____ ___  ___   __  _ ___  ___ ___   _ _           |");
        System.Console.WriteLine("|         | | | | _ \\| __|     | _ \\| _ \\ /  \\| |   \\| __| _ \\ | | |          |");
        System.Console.WriteLine("|         | |_| | _ <| __| | | | _ <|   /| () | | |) | __|   / |__ |          |");
        System.Console.WriteLine("|         |___|_|___/|___|_|_|_|___/|_|\\_\\\\__/|_|___/|___|_|\\_\\|___|          |");
        System.Console.WriteLine("|                    ___  __  ___ _ _    _ ___ ___  _____                     |");
        System.Console.WriteLine("|                   / __|/  \\|   | | \\  / | __| _ \\|_   _|                    |");
        System.Console.WriteLine("|                  ( (__| () | | | |\\ \\/ /| __|   /  | |                      |");
        System.Console.WriteLine("|                   \\___|\\__/|_|___| \\__/ |___|_|\\_\\ |_|                      |");
        System.Console.WriteLine("|                                                                             |");
        System.Console.WriteLine("|                               * C# Version *                                |");
        System.Console.WriteLine("|_____________________________________________________________________________|");
        System.Console.WriteLine("|                                                                             |");
        System.Console.WriteLine("| Usage: libembroidery-convert fileToRead filesToWrite ...                    |");
        System.Console.WriteLine("|_____________________________________________________________________________|");
        System.Console.WriteLine("                                                                               ");
        System.Console.WriteLine(" _____________________________________________________________________________ ");
        System.Console.WriteLine("|                                                                             |");
        System.Console.WriteLine("| List of Formats                                                             |");
        System.Console.WriteLine("|_____________________________________________________________________________|");
        System.Console.WriteLine("|                                                                             |");
        System.Console.WriteLine("| 'S' = Yes, and is considered stable.                                        |");
        System.Console.WriteLine("| 'U' = Yes, but may be unstable.                                             |");
        System.Console.WriteLine("| ' ' = No.                                                                   |");
        System.Console.WriteLine("|_____________________________________________________________________________|");
        System.Console.WriteLine("|        |       |       |                                                    |");
        System.Console.WriteLine("| Format | Read  | Write | Description                                        |");
        System.Console.WriteLine("|________|_______|_______|____________________________________________________|");
        System.Console.WriteLine("|        |       |       |                                                    |");

        //System.Console.WriteLine("|  %-4s  |   %s   |   %s   |  %-49s |", "TODO", " ", " ", "TODO"); //TODO: finish this.

        System.Console.WriteLine("|________|_______|_______|____________________________________________________|");
        System.Console.WriteLine("|        |       |       |                                                    |");
        //System.Console.WriteLine("| Total: |  %3d  |  %3d  |                                                    |", numReaders, numWriters); //TODO: finish this.
        System.Console.WriteLine("|________|_______|_______|____________________________________________________|");
        System.Console.WriteLine("|                                                                             |");
        System.Console.WriteLine("|                         http://embroidermodder.org                          |");
        System.Console.WriteLine("|_____________________________________________________________________________|");
        System.Console.WriteLine("");
    }

    public static int Main(string[] args)
    {
        EmbPattern p;
        int successful;

        if(args == null || args.Length < 2)
        {
            usage();
            return 1;
        }

        p = libembroidery.embPattern_create();
        successful = libembroidery.embPattern_read(p, args[0]);
        successful = libembroidery.embPattern_write(p, args[1]); //TODO: finish this. add looping to output multiple formats
        libembroidery.embPattern_free(p);

        return 0;
    }
}

/* kate: bom off; indent-mode cstyle; indent-width 4; replace-trailing-space-save on; */
