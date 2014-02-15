program libembroidery_convert;

{$IFDEF FPC}
  {$MODE Delphi}
{$ENDIF}

{$APPTYPE CONSOLE}

uses
  SysUtils, libembroidery;

procedure usage();
var
  i : integer;
  hformats : PEmbFormatList;
  curent : PEmbFormat;
  ext, description: PChar;
  stateReader, stateWriter : char;
  formatType : integer;
begin
  hformats := embFormatList_create();
  curent  := hformats.firstFormat;


    i := 0;
    writeln(' _____________________________________________________________________________ ');
    writeln('|          _   _ ___  ___ _____ ___  ___   __  _ ___  ___ ___   _ _           |');
    writeln('|         | | | | _ \| __|     | _ \| _ \ /  \| |   \| __| _ \ | | |          |');
    writeln('|         | |_| | _ <| __| | | | _ <|   /| () | | |) | __|   / |__ |          |');
    writeln('|         |___|_|___/|___|_|_|_|___/|_|\_\\__/|_|___/|___|_|\_\|___|          |');
    writeln('|                    ___  __  ___ _ _    _ ___ ___  _____                     |');
    writeln('|                   / __|/  \|   | | \  / | __| _ \|_   _|                    |');
    writeln('|                  ( (__| () | | | |\ \/ /| __|   /  | |                      |');
    writeln('|                   \___|\__/|_|___| \__/ |___|_|\_\ |_|                      |');
    writeln('|_____________________________________________________________________________|');
    writeln('|                                                                             |');
    writeln('| Usage: libembroidery_convert fileToRead filesToWrite ...                    |');
    writeln('|_____________________________________________________________________________|');
    writeln('                                                                               ');
    writeln(' _____________________________________________________________________________ ');
    writeln('|                                                                             |');
    writeln('| List of Formats                                                             |');
    writeln('|_____________________________________________________________________________|');
    writeln('|                                                                             |');
    writeln('| "S" = Yes, and is considered stable.                                        |');
    writeln('| "U" = Yes, but may be unstable.                                             |');
    writeln('| " " = No.                                                                   |');
    writeln('|_____________________________________________________________________________|');
    writeln('|        |       |       |                                                    |');
    writeln('| Format | Read  | Write | Description                                        |');
    writeln('|________|_______|_______|____________________________________________________|');
    writeln('|        |       |       |                                                    |');
 
    while curent <> nil do
    begin
      embFormat_info( curent.extension, ext, description, stateReader, stateWriter, formatType ); 
      writeln(format('|  %-4s  |   %s   |   %s   |  %-49s |', [ curent^.extension, stateReader, stateWriter, description]) );
      curent := curent^.next;
    end;

    writeln('|________|_______|_______|____________________________________________________|');
    writeln('|                                                                             |');
    writeln('|                   http://embroidermodder.github.io                          |');
    writeln('|_____________________________________________________________________________|');
    writeln('');
  embFormatList_free(hformats);
end;

(* TODO: Add capability for converting multiple files of various types to a single format. Currently, we only convert a single file to multiple formats. *)

(*! Developers incorporating libembroidery into another project should use the SHORT_WAY of using libembroidery. It uses
 *  convenience functions and is approximately 20 lines shorter than the long way.
 *
 *  Developers who are interested improving libembroidery or using it to its fullest extent should use the LONG_WAY.
 *  It essentially does the same work the convenience function do.
 *
 *  Both methods are acceptable and it is up to you which to choose. Both will stay here for regression testing.
 *)

{$define SHORT_WAY}

var
  P : PEmbPattern;
  i, successful : Integer;
begin
  { TODO -oUser -cConsole Main : Insert code here }

{$ifdef SHORT_WAY}
    fillchar(p,sizeof(TEmbPattern), 0);
    successful := 0; i := 0;
    if paramcount < 2 then
    begin
        usage();
        exit;
    end;

    p := embPattern_create();
    //if (p) then  raise excepion. 'libembroidery-convert-main.c main(), cannot allocate memory for p'); exit(1); }

    successful := embPattern_read(p, pchar(paramstr(1)) );
    if successful = 0 then
    begin
        writeln('libembroidery-convert-main.c main(), reading file %s was unsuccessful', paramstr(1));
        //embPattern_free(p);
        exit;
    end;
(*
    if(embFormat_type(argv[1]) == EMBFORMAT_OBJECTONLY && argc == 3) // TODO: fix this to work when writing multiple files 
    {
        if(embFormat_type(argv[2]) == EMBFORMAT_STITCHONLY)
            embPattern_movePolylinesToStitchList(p);
    }

    i = 2;
    for(i = 2; i < argc; i++)
    {     *)
        //successful = embPattern_write(p, argv[i]);
        successful := embPattern_write(p, pchar(paramstr(2)));
        if  successful = 0 then
        begin
            //embLog_error('libembroidery-convert-main.c main(), writing file %s was unsuccessful', argv[i]);
            writeln('libembroidery-convert-main.c main(), writing file %s was unsuccessful', paramstr(2));
            exit;
        end;

        writeln(format('libembroidery-convert, writing file %s was successful', [paramstr(2)]));

        (*
    }

    embPattern_free(p);
    return 0;
{$else} //LONG_WAY 
    EmbPattern* p = 0;
    EmbReaderWriter* reader = 0, *writer = 0;
    int successful = 0, i = 0;
    if(argc < 3)
    {
        usage();
        exit(0);
    }

    p = embPattern_create();
    if(!p) { embLog_error('libembroidery-convert-main.c main(), cannot allocate memory for p'); exit(1); }

    successful = 0;
    reader = embReaderWriter_getByFileName(argv[1]);
    if(!reader)
    {
        successful = 0;
        embLog_error('libembroidery-convert-main.c main(), unsupported read file type: %s', argv[1]);
    }
    else
    {
        successful = reader->reader(p, argv[1]);
        if(!successful) embLog_error('libembroidery-convert-main.c main(), reading file was unsuccessful: %s', argv[1]);
    }
    free(reader);
    if(!successful)
    {
        embPattern_free(p);
        exit(1);
    }

    if(embFormat_type(argv[1]) == EMBFORMAT_OBJECTONLY && argc == 3) // TODO: fix this to work when writing multiple files 
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
            embLog_error('libembroidery-convert-main.c main(), unsupported write file type: %s', argv[i]);
        }
        else
        {
            successful = writer->writer(p, argv[i]);
            if(!successful)
                embLog_error('libembroidery-convert-main.c main(), writing file %s was unsuccessful', argv[i]);
        }
        free(writer);
    }

    embPattern_free(p);
    return 0;
    *)
{$endif} //SHORT_WAY 

end.
