program embroidery_converter;

{$APPTYPE CONSOLE}

uses
  SysUtils, libembroidery;

const
  formatCount = 59;
  formats : array[0..formatCount*4-1] of string = (
'.10o', 'U', ' ', ' Toyota Embroidery Format                         ',
'.100', 'U', ' ', ' Toyota Embroidery Format                         ',
'.art', ' ', ' ', ' Bernina Embroidery Format                        ',
'.bmc', ' ', ' ', ' Bitmap Cache Embroidery Format                   ',
'.bro', 'U', ' ', ' Bits & Volts Embroidery Format                   ',
'.cnd', ' ', ' ', ' Melco Embroidery Format                          ',
'.col', 'U', 'U', ' Embroidery Thread Color Format                   ',
'.csd', 'U', ' ', ' Singer Embroidery Format                         ',
'.csv', 'U', 'U', ' Comma Separated Values                           ',
'.dat', 'U', ' ', ' Barudan Embroidery Format                        ',
'.dem', ' ', ' ', ' Melco Embroidery Format                          ',
'.dsb', 'U', ' ', ' Barudan Embroidery Format                        ',
'.dst', 'U', 'U', ' Tajima Embroidery Format                         ',
'.dsz', 'U', ' ', ' ZSK USA Embroidery Format                        ',
'.dxf', ' ', ' ', ' Drawing Exchange Format                          ',
'.edr', 'U', 'U', ' Embird Embroidery Format                         ',
'.emd', 'U', ' ', ' Elna Embroidery Format                           ',
'.exp', 'U', 'U', ' Melco Embroidery Format                          ',
'.exy', 'U', ' ', ' Eltac Embroidery Format                          ',
'.eys', ' ', ' ', ' Sierra Expanded Embroidery Format                ',
'.fxy', 'U', ' ', ' Fortron Embroidery Format                        ',
'.gnc', ' ', ' ', ' Great Notions Embroidery Format                  ',
'.gt ', 'U', ' ', ' Gold Thread Embroidery Format                    ',
'.hus', 'U', 'U', ' Husqvarna Viking Embroidery Format               ',
'.inb', 'U', ' ', ' Inbro Embroidery Format                          ',
'.inf', 'U', 'U', ' Embroidery Color Format                          ',
'.jef', 'U', 'U', ' Janome Embroidery Format                         ',
'.ksm', 'U', 'U', ' Pfaff Embroidery Format                          ',
'.max', 'U', ' ', ' Pfaff Embroidery Format                          ',
'.mit', 'U', ' ', ' Mitsubishi Embroidery Format                     ',
'.new', 'U', ' ', ' Ameco Embroidery Format                          ',
'.ofm', 'U', ' ', ' Melco Embroidery Format                          ',
'.pcd', 'U', 'U', ' Pfaff Embroidery Format                          ',
'.pcm', 'U', ' ', ' Pfaff Embroidery Format                          ',
'.pcq', 'U', 'U', ' Pfaff Embroidery Format                          ',
'.pcs', 'U', 'U', ' Pfaff Embroidery Format                          ',
'.pec', 'U', 'U', ' Brother Embroidery Format                        ',
'.pel', ' ', ' ', ' Brother Embroidery Format                        ',
'.pem', ' ', ' ', ' Brother Embroidery Format                        ',
'.pes', 'U', 'U', ' Brother Embroidery Format                        ',
'.phb', 'U', ' ', ' Brother Embroidery Format                        ',
'.phc', 'U', ' ', ' Brother Embroidery Format                        ',
'.plt', 'U', 'U', ' AutoCAD plot drawing                             ',
'.rgb', 'U', 'U', ' RGB Embroidery Format                            ',
'.sew', 'U', ' ', ' Janome Embroidery Format                         ',
'.shv', 'U', ' ', ' Husqvarna Viking Embroidery Format               ',
'.sst', 'U', ' ', ' Sunstar Embroidery Format                        ',
'.stx', 'U', ' ', ' Data Stitch Embroidery Format                    ',
'.svg', 'U', 'U', ' Scalable Vector Graphics                         ',
'.t09', 'U', ' ', ' Pfaff Embroidery Format                          ',
'.tap', 'U', ' ', ' Happy Embroidery Format                          ',
'.thr', 'U', 'U', ' ThredWorks Embroidery Format                     ',
'.txt', ' ', 'U', ' Text File                                        ',
'.u00', 'U', ' ', ' Barudan Embroidery Format                        ',
'.u01', ' ', ' ', ' Barudan Embroidery Format                        ',
'.vip', 'U', ' ', ' Pfaff Embroidery Format                          ',
'.vp3', 'U', ' ', ' Pfaff Embroidery Format                          ',
'.xxx', 'U', 'U', ' Singer Embroidery Format                         ',
'.zsk', 'U', ' ', ' ZSK USA Embroidery Format                        '
);




procedure usage();
var
  i : integer;
begin
    i := 0;

    writeln(' _____________________________________________________________________________ ');
    writeln('|                ___ _____ ___  ___   __  _ ___  ___ ___   _ _                |');
    writeln('|               | __|     | _ \| _ \ /  \| |   \| __| _ \ | | |               |');
    writeln('|               | __| | | | _ <|   /| () | | |) | __|   / |__ |               |');
    writeln('|               |___|_|_|_|___/|_|\_\\__/|_|___/|___|_|\_\|___|               |');
    writeln('|                    ___  __  ___ _ _    _ ___ ___  _____                     |');
    writeln('|                   / __|/  \|   | | \  / | __| _ \|_   _|                    |');
    writeln('|                  ( (__| () | | | |\ \/ /| __|   /  | |                      |');
    writeln('|                   \___|\__/|_|___| \__/ |___|_|\_\ |_|                      |');
    writeln('|_____________________________________________________________________________|');
    writeln('|                                                                             |');
    writeln('| Usage: embroidery-convert fileToRead filesToWrite ...                       |');
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
    for i := 0 to formatCount-1 do
    begin
        writeln(format('|  %s  |   %s   |   %s   | %s |', [ formats[i*4], formats[i*4+1], formats[i*4+2], formats[i*4+3] ] ));
    end;
    writeln('|________|_______|_______|____________________________________________________|');
    writeln('|                                                                             |');
    writeln('|                   http://embroidermodder.sourceforge.net                    |');
    writeln('|_____________________________________________________________________________|');
    writeln('');
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
    fillchar(p,sizeof(embPattern), 0);
    successful := 0; i := 0;
    if paramcount < 2 then
    begin
        usage();
        writeln('paramcount=',paramcount);
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

(* kate: bom off; indent-mode cstyle; indent-width 4; replace-trailing-space-save on; *)
