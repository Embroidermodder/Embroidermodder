unit libembroidery_formats;

interface

uses
    libembroidery, Classes, Windows, Messages, SysUtils, Variants;



function EmbReadersFilter(): string ;
procedure EmbFillWriter(AList: TStrings);

implementation

var
  GReaders : TStrings = nil;
  GWriters : TStrings = nil;

(*function NameOfExt(ext: string; ColumnU: integer; defaultName: string = ''): string;
var i : integer;
begin
  result := defaultName;
  for i := 0 to EmbFormatSupportedCount-1 do
  begin
    if EmbFormatSupportedFiles[i][0] = ext then //MATCH
    begin
      {$IFNDEF EMBFORMAT_SUPPORTEDONLY}
      result := EmbFormatSupportedFiles[i][3];
      {$ELSE}
      if trim(EmbFormatSupportedFiles[i][ColumnU]) <> '' then
        result := EmbFormatSupportedFiles[i][3];

      {$ENDIF}
      break;
    end;
  end;
end;*)

procedure build_RW;
var
  s,ext,name : string;
  i : integer;

  hformats : PEmbFormatList;
  current : PEmbFormat;
  pext, description: PChar;
  stateReader, stateWriter : char;
  formatType : integer;
begin
  hformats := embFormatList_create();
  current  := hformats.firstFormat;
  //embFormat_info( current.extension, pext, description, stateReader, stateWriter, formatType );
  //embFormatList_free(hformats); exit;
  GReaders := TStringList.Create;
  GWriters := TStringList.Create;


  try
    while current <> nil do
    begin
      embFormat_info( current.extension, pext, description, stateReader, stateWriter, formatType );
      //READ
      if stateReader <> ' ' then
      begin
        ext := string(pext);
        if GReaders.IndexOfName(ext) < 0 then
        begin
          GReaders.Values[ext] := string(description);
          (*{$IFNDEF EMBFORMAT_SUPPORTEDONLY}
          GReaders.Values[ext] := NameOfExt(ext,1,'Embroidery Format');
          {$ELSE}
          name := NameOfExt(ext,1);
          if name <> '' then
            GReaders.Values[ext] := name;
          {$ENDIF}*)
        end;
      end;

      //WRITE
      if stateWriter <> ' ' then
      begin
        ext := string(pext);
        if GWriters.IndexOfName(ext) < 0 then
        begin
          GWriters.Values[ext] := string(description);
          (*{$IFNDEF EMBFORMAT_SUPPORTEDONLY}
          GWriters.Values[ext] := NameOfExt(ext,2,'Embroidery Format');
          {$ELSE}
          name := NameOfExt(ext,1);
          if name <> '' then
            GWriters.Values[ext] := name;
          {$ENDIF}*)
        end;

      end;
      current := current^.next;
    end;
  finally
  
    {pext := 'a';
    description := 'z';
    stateReader := '0';
    stateWriter := 'z';
    embFormatList_free(hformats);}
    
  end;

    
end;

procedure EmbFillWriter(AList: TStrings);
begin
  if not assigned(GWriters) then
     build_RW;
  if Assigned(GWriters) then
  AList.Assign(GWriters);
end;

function EmbReadersFilter(): string ;
var i,j : integer;
  LList : TStrings;
  pool,allExt,s : string;
begin
  if not assigned(GReaders) then
  begin
     build_RW;
  end;
  
  pool := '';
  allExt := '';
  {$IFNDEF EMBSORTEDFILTER}
  if assigned(GReaders) then
  for i := 0 to GReaders.Count-1 do
  begin
    if pool <> '' then
      pool := pool + '|';
    pool := pool + format('%s  (*%1:s)|*%1:s',[GReaders.ValueFromIndex[i],GReaders.Names[i] ]);
    if allExt <> '' then
      allExt := allExt + ';';
    allExt := allExt + format('*%s', [GReaders.Names[i]]);
  end;
  pool := format('%s  (%1:s)|*%1:s',['All embroidery format',allExt ]) +'|'+ pool;

  {$ELSE}



  LList1 := TStringList.Create;
  try
    for i := 0 to GReaders.Count-1 do
    begin
      LList1.Add( format('%s (*.%1:s)|*.%1:s',[GReaders.ValueFromIndex[i],GReaders.Names[i] ]) );
    end;
    TStringList(LList1).Sort;
    for i := 0 to LList1.Count-1 do
    begin
      if pool <> '' then
        pool := pool + '|';
      pool := pool + LList1[i];
    end;
  finally
    LList1.Free;
  end;

  {$ENDIF}
  result := pool;
end;
     

end.
