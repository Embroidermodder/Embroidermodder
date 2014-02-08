unit dllExportList;

interface

uses Classes, Windows;

procedure ListDLLExports(const FileName: string; List: TStrings);

implementation
uses
    {$IFDEF FPC}
  imagehlp
  {$ELSE}
  ImageHlp
  {$ENDIF}
  ;

procedure ListDLLExports(const FileName: string; List: TStrings);
type
  TDWordArray = array [0..$FFFFF] of CARDINAL;
var
  imageinfo: LoadedImage;
  pExportDirectory: PImageExportDirectory;
  dirsize: Cardinal;
  pDummy: PImageSectionHeader;
  i: Cardinal;
  pNameRVAs: ^TDWordArray;
  Name: string;
begin
  List.Clear;
  if MapAndLoad(PChar(FileName), nil, @imageinfo, True, True) then
  begin
    try
      pExportDirectory := ImageDirectoryEntryToData(imageinfo.MappedAddress,
        False, IMAGE_DIRECTORY_ENTRY_EXPORT, dirsize);
      if (pExportDirectory <> nil) then
      begin
        pNameRVAs := ImageRvaToVa(imageinfo.FileHeader, imageinfo.MappedAddress,
          DWORD(pExportDirectory^.AddressOfNames), pDummy);
        for i := 0 to pExportDirectory^.NumberOfNames - 1 do
        begin
          Name := PChar(ImageRvaToVa(imageinfo.FileHeader, imageinfo.MappedAddress,
            pNameRVAs^[i], pDummy));
          List.Add(Name);
        end;
      end;
    finally
      UnMapAndLoad(@imageinfo);
    end;
  end;
end;
end.
