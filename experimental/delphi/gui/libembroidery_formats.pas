unit libembroidery_formats;
{see: http://www.swissdelphicenter.ch/torry/showcode.php?id=1133}
interface

uses
    libembroidery, Classes, Windows, Messages, SysUtils, Variants;



function EmbReadersFilter(): string ;
procedure EmbFillWriter(AList: TStrings);

implementation

uses
  dllExportList;

var
  GReaders : TStrings = nil;
  GWriters : TStrings = nil;

function NameOfExt(ext: string; ColumnU: integer; defaultName: string = ''): string;
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
end;

procedure ScanDLL_AssumedReaderOrWriterEXT;
var
  LList : TStrings;
  s,ext,name : string;
  i : integer;
begin
  GReaders := TStringList.Create;
  GWriters := TStringList.Create;
  LList := TStringList.Create;

  try
    ListDLLExports(emblib, LList);
    for i := 0 to LList.Count - 1 do
    begin
      s := LList[i];
      //READ
      if (length(s) <= 7) and(copy(s,1,4) = 'read') then
      begin
        ext := lowerCase( copy(s, 5, length(s)) );
        if GReaders.IndexOfName(ext) < 0 then
        begin
          {$IFNDEF EMBFORMAT_SUPPORTEDONLY}
          GReaders.Values[ext] := NameOfExt(ext,1,'Embroidery Format');
          {$ELSE}
          name := NameOfExt(ext,1);
          if name <> '' then
            GReaders.Values[ext] := name;
          {$ENDIF}
        end;
      end

      //WRITE
      else if (length(s) <= 8) and (copy(s,1,5) = 'write') then
      begin
        ext := lowerCase( copy(s, 6, length(s)) );
        if GWriters.IndexOfName(ext) < 0 then
        begin
          {$IFNDEF EMBFORMAT_SUPPORTEDONLY}
          GWriters.Values[ext] := NameOfExt(ext,2,'Embroidery Format');
          {$ELSE}
          name := NameOfExt(ext,1);
          if name <> '' then
            GWriters.Values[ext] := name;
          {$ENDIF}
        end;

      end;

    end;
  finally
    LList.Free
  end;
end;

procedure EmbFillWriter(AList: TStrings);
begin
  if not assigned(GReaders) then
     ScanDLL_AssumedReaderOrWriterEXT;
  AList.Assign(GWriters);
end;

function EmbReadersFilter(): string ;
var i,j : integer;
  LList : TStrings;
  all,s : string;
begin
  if not assigned(GReaders) then
     ScanDLL_AssumedReaderOrWriterEXT;
  result := '';
  all := '';
  {$IFNDEF EMBSORTEDFILTER}
  for i := 0 to GReaders.Count-1 do
  begin
    if result <> '' then
      result := result + '|';
    result := result + format('%s  (*.%1:s)|*.%1:s',[GReaders.ValueFromIndex[i],GReaders.Names[i] ]);
    if all <> '' then
      all := all + ';';
    all := all + format('*.%s', [GReaders.Names[i]]);
  end;
  result := format('%s  (%1:s)|*.%1:s',['All embroidery format',all ]) +'|'+ result;

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
      if result <> '' then
        result := result + '|';
      result := result + LList1[i];
    end;
  finally
    LList1.Free;
  end;

  {$ENDIF}

end;
     

end.
