program embconv;

uses
  Forms,
  uMain in 'uMain.pas' {Form1},
  libembroidery_formats in 'libembroidery_formats.pas',
  dllExportList in 'dllExportList.pas';

{$R *.res}

begin
  Application.Initialize;
  Application.CreateForm(TForm1, Form1);
  Application.Run;
end.
