unit uMain;

interface

uses
  Windows, Messages, SysUtils, Variants, Classes, Graphics, Controls, Forms,
  Dialogs, libembroidery, StdCtrls, ExtDlgs, ExtCtrls, VirtualTrees;

type
  TForm1 = class(TForm)
    mmo1: TMemo;
    dlgOpen1: TOpenDialog;
    OpenPictureDialog1: TOpenPictureDialog;
    Panel1: TPanel;
    Button1: TButton;
    btnEmbOpen: TButton;
    vtOpen: TVirtualStringTree;
    spl1: TSplitter;
    vtExts: TVirtualStringTree;
    spl2: TSplitter;
    pnlOutput: TPanel;
    pnlOutputConfig: TPanel;
    vtResult: TVirtualStringTree;
    rgAdditionalDir: TRadioGroup;
    grpTargetDir: TGroupBox;
    rbInPlace: TRadioButton;
    rbOutputDifferentDir: TRadioButton;
    edtOutputDifferentDir: TEdit;
    GroupBox1: TGroupBox;
    edtPrefix: TEdit;
    edtSuffix: TEdit;
    lblPrefix: TLabel;
    lblSuffix: TLabel;
    procedure FormCreate(Sender: TObject);
    procedure btnEmbOpenClick(Sender: TObject);
    procedure vtOpenGetText(Sender: TBaseVirtualTree; Node: PVirtualNode;
      Column: TColumnIndex; TextType: TVSTTextType;
      var CellText: WideString);
    procedure vtExtsGetText(Sender: TBaseVirtualTree;
      Node: PVirtualNode; Column: TColumnIndex; TextType: TVSTTextType;
      var CellText: WideString);
    procedure vtExtsInitNode(Sender: TBaseVirtualTree; ParentNode,
      Node: PVirtualNode; var InitialStates: TVirtualNodeInitStates);
    procedure vtOpenGetNodeDataSize(Sender: TBaseVirtualTree;
      var NodeDataSize: Integer);
    procedure vtExtsGetNodeDataSize(Sender: TBaseVirtualTree;
      var NodeDataSize: Integer);
    procedure vtExtsChecked(Sender: TBaseVirtualTree;
      Node: PVirtualNode);
    procedure vtResultGetNodeDataSize(Sender: TBaseVirtualTree;
      var NodeDataSize: Integer);
    procedure vtResultGetText(Sender: TBaseVirtualTree; Node: PVirtualNode;
      Column: TColumnIndex; TextType: TVSTTextType;
      var CellText: WideString);
    procedure edtOutputDifferentDirChange(Sender: TObject);
    procedure RebuildOutputResult(Sender: TObject);
    procedure vtExtsCompareNodes(Sender: TBaseVirtualTree; Node1,
      Node2: PVirtualNode; Column: TColumnIndex; var Result: Integer);
    procedure vtExtsColumnClick(Sender: TBaseVirtualTree;
      Column: TColumnIndex; Shift: TShiftState);
  private
    { Private declarations }
    FInputFiles : TStrings;
    //FOutputExts : TStrings;
    procedure AddOpenFile(AFilePath: string);
    procedure FillExts;
  public
    { Public declarations }
  end;

var
  Form1: TForm1;

implementation

uses libembroidery_formats;

{$R *.dfm}

type
  PData = ^TData;
  TData = record
    processed : boolean;
    Txt : string;
  end;

  PEmbExt = ^TEmbExt;
  TEmbExt = record
    Ext : string;
    Desc : string;
  end;

  PResult = ^TResult;
  TResult = record
    Txt : string;
    Source : string;
    SourcePath : string;
    processed : boolean;
  end;

procedure TForm1.FormCreate(Sender: TObject);
begin
  //mmo1.Lines.Add( EmbReadersFilter() );
  FInputFiles := TStringList.Create;
  dlgOpen1.Filter := EmbReadersFilter();
  FillExts();
  pnlOutput.Align := alClient;
end;

procedure TForm1.FillExts();
var
  LExt : PEmbExt;
  function AddNew(Parent: PVirtualNode; AExt, ADesc: string):PVirtualNode;
  begin
    result := vtExts.AddChild(Parent);
    LExt := vtExts.GetNodeData(result);
    LExt^.Ext := AExt;
    LExt^.Desc := ADesc;
  end;
var i : integer;
  LExtList : TStringList;
begin
  LExtList := TStringList.Create;
  EmbFillWriter(LExtList);
  //vtOutExt.RootNodeCount := LExtList.Count;
  for i := 0 to LExtList.Count -1 do
  begin
    AddNew(vtExts.RootNode, LExtList.Names[i], LExtList.ValueFromIndex[i]);
  end;
  LExtList.Free;
  vtExts.OnCompareNodes := vtExtsCompareNodes;
  vtExts.Sort(nil,1, sdAscending );
end;



procedure TForm1.btnEmbOpenClick(Sender: TObject);
var i : integer;
begin
  if dlgOpen1.Filter = '' then
    dlgOpen1.Filter := EmbReadersFilter();
    //self.mmo1.Lines.Assign(GReaders);

  if dlgOpen1.Execute then
  begin
    for i := 0 to dlgOpen1.Files.Count -1 do
    begin
      AddOpenFile(dlgOpen1.Files[i]);
    end;
    //vtOpen.RootNodeCount := FInputFiles.Count;
  end;
  vtOpen.FullExpand();
end;

procedure TForm1.AddOpenFile(AFilePath: string);
var
  Data : PData;
  function AddNew(Parent: PVirtualNode; AText: string):PVirtualNode;
  begin
    result := vtOpen.AddChild(Parent);
    data := vtOpen.GetNodeData(result);
    data^.Txt := Atext;
    data^.processed := False;
  end;

var
  LFile, LPath : string;
  Parent, Node : PVirtualNode;
  i : integer;
begin
  LFile := ExtractFileName(AFilePath);
  LPath := ExtractFilePath(AFilePath);

  //find parent
  Parent := vtOpen.RootNode.FirstChild;
  while Parent <> nil do
  begin
    Data := vtOpen.GetNodeData(Parent);
    if Data^.Txt = LPath then
    begin
      break; //found.
    end
    else
    begin
      Parent := Parent.NextSibling;
    end;
  end;
  if Not Assigned(Parent) then
  begin
    //append parent to root
    Parent := AddNew(vtOpen.RootNode, LPath);
  end;

  //append child to parent
  AddNew(Parent, LFile);
end;


procedure TForm1.vtOpenGetText(Sender: TBaseVirtualTree;
  Node: PVirtualNode; Column: TColumnIndex; TextType: TVSTTextType;
  var CellText: WideString);
var
  Data : PData;
begin
  Data := vtOpen.GetNodeData(Node);
  CellText := Data^.Txt;// FInputFiles[Node^.index];
end;

procedure TForm1.vtExtsGetText(Sender: TBaseVirtualTree;
  Node: PVirtualNode; Column: TColumnIndex; TextType: TVSTTextType;
  var CellText: WideString);
var
  Ext : PEmbExt;
begin
  Ext := vtExts.GetNodeData(Node);
  if assigned(Ext) then
  case Column of
    0 : CellText := Ext^.Ext;
    1 : CellText := Ext^.Desc;
  end;
end;

procedure TForm1.vtExtsInitNode(Sender: TBaseVirtualTree; ParentNode,
  Node: PVirtualNode; var InitialStates: TVirtualNodeInitStates);
begin
  Node^.CheckType := ctCheckBox;
end;

procedure TForm1.vtOpenGetNodeDataSize(Sender: TBaseVirtualTree;
  var NodeDataSize: Integer);
begin
  NodeDataSize := sizeof(TData);
end;

procedure TForm1.vtExtsGetNodeDataSize(Sender: TBaseVirtualTree;
  var NodeDataSize: Integer);
begin
  NodeDataSize := sizeof(TEmbExt);
end;

procedure TForm1.vtExtsChecked(Sender: TBaseVirtualTree;
  Node: PVirtualNode);
begin
  RebuildOutputResult(nil);
end;

procedure TForm1.RebuildOutputResult(Sender: TObject);
var LExts : array of string;

  procedure ScanExts();
  var Node : PVirtualNode;
    i : integer;
    R : PEmbExt;
  begin
    SetLength(LExts,0);
    Node := vtExts.RootNode;
    repeat
      Node := vtExts.GetNextChecked(Node, csCheckedNormal, true);
      if assigned(Node) then
      begin
        i := Length(LExts);
        SetLength(LExts, i+1);
        R := vtExts.GetNodeData(Node);
        LExts[i] := '.'+ R^.Ext;
      end;
    until Node = nil;

  end;

  var
    RData : PResult;
  function AddNew(Parent: PVirtualNode; AText: string; ASource: string=''; APath: string='';
    AllowPrefixSuffix: boolean=True):PVirtualNode;
  var
    ext, fn : string;
  begin
    result := vtResult.AddChild(Parent);
    RData := vtResult.GetNodeData(result);
    if AllowPrefixSuffix and ((edtSuffix.Text <> EmptyStr) or (edtPrefix.Text <> EmptyStr)) then
    begin
      fn := ExtractFileName(AText);
      ext := ExtractFileExt(AText);
      fn := copy(fn, 1, length(fn)-length(ext) );
      Atext := edtPrefix.Text + fn + edtSuffix.Text + ext;
    end;
    RData^.Txt := Atext;
    RData^.processed := False;
    RData^.Source := ASource;
    RData^.SourcePath := APath;
  end;

  function GetPathNodeHasText(RootPathNode:PVirtualNode; AText:string): PVirtualNode ;
  var DstChild : PVirtualNode;
    i : integer;
  begin
    //find parent
    DstChild := RootPathNode.FirstChild;//vtResult.RootNode.FirstChild;
    while DstChild <> nil do
    begin
      RData := vtResult.GetNodeData(DstChild);
      if RData^.Txt = AText then
      begin
        break; //found.
      end
      else
      begin
        DstChild := DstChild.NextSibling;
      end;
    end;
    if Not Assigned(DstChild) then
    begin
      //append parent to root
      DstChild := AddNew(RootPathNode, AText, EmptyStr, EmptyStr, False);
    end;
    result := DstChild;
  end;

  procedure AddNewResultFiles(ASrcFile, ASrcPath: string);
  var DstParent, SubParent : PVirtualNode;
    i : integer;
    ext,ext2, fn : string;
  begin
    //find parent
    if rbOutputDifferentDir.Checked then
      DstParent := GetPathNodeHasText(vtResult.RootNode, edtOutputDifferentDir.Text)
    else
      DstParent := GetPathNodeHasText(vtResult.RootNode, ASrcPath);

    case rgAdditionalDir.ItemIndex of
      0 : //Source Filename
          begin
            for i := 0 to length(LExts) -1 do
            begin
              fn := ExtractFileName(ASrcFile);
              ext := ExtractFileExt(ASrcFile);
              fn := copy(fn, 1, length(fn)-length(ext) );
              SubParent := GetPathNodeHasText(DstParent, fn);
              AddNew(SubParent, ChangeFileExt(ASrcFile, LExts[i]), ASrcFile, ASrcPath );
            end;
          end;
      1 : //Source File Extension
          begin
            for i := 0 to length(LExts) -1 do
            begin
              ext := ExtractFileExt(ASrcFile);
              ext := copy(ext, 2, length(ext));
              SubParent := GetPathNodeHasText(DstParent, ext);
              AddNew(SubParent, ChangeFileExt(ASrcFile, LExts[i]), ASrcFile, ASrcPath );
            end;
          end;
      2 : //Destination File Extension
          begin
            for i := 0 to length(LExts) -1 do
            begin
              ext := ExtractFileExt(LExts[i]);
              if pos('.',ext) = 1 then
                ext := copy(ext, 2, length(ext));
              SubParent := GetPathNodeHasText(DstParent, ext);
              AddNew(SubParent, ChangeFileExt(ASrcFile, LExts[i]), ASrcFile, ASrcPath );
            end;
          end;
      else //None
          begin
            //append child to parent
            for i := 0 to length(LExts) -1 do
            begin
              AddNew(DstParent, ChangeFileExt(ASrcFile, LExts[i]), ASrcFile, ASrcPath );
            end;
          end;
    end;


  end;

var
  SrcParent, SrcNode, RNode : PVirtualNode;
  R : PResult;
  ParentData, Data : PData;

begin
  //result count = checked.count * open.count
  vtResult.BeginUpdate;
  try
    vtResult.Clear;
    ScanExts();
    SrcParent := vtOpen.RootNode.FirstChild;
    while SrcParent <> nil do
    begin
      ParentData := vtOpen.GetNodeData(SrcParent);

      SrcNode := SrcParent.FirstChild;
      while SrcNode <> nil do
      begin
        Data := vtOpen.GetNodeData(SrcNode);
        AddNewResultFiles(Data^.Txt, ParentData^.Txt);
        SrcNode := SrcNode.NextSibling;
      end;
      
      SrcParent := SrcParent.NextSibling;
    end;
  finally
    vtResult.FullExpand();
    vtResult.EndUpdate;
  end;
end;

procedure TForm1.vtResultGetNodeDataSize(Sender: TBaseVirtualTree;
  var NodeDataSize: Integer);
begin
  NodeDataSize := sizeOf(TResult);
end;

procedure TForm1.vtResultGetText(Sender: TBaseVirtualTree;
  Node: PVirtualNode; Column: TColumnIndex; TextType: TVSTTextType;
  var CellText: WideString);
var Data : PResult;
begin
  data := vtResult.GetNodeData(Node);
  case column of
    0 : CellText := data^.Txt;
    1 : CellText := data^.Source;
    2 : CellText := data^.SourcePath;
  end;
end;

procedure TForm1.edtOutputDifferentDirChange(Sender: TObject);
begin
  rbOutputDifferentDir.Checked := true;
  RebuildOutputResult(nil);
end;

procedure TForm1.vtExtsCompareNodes(Sender: TBaseVirtualTree; Node1,
  Node2: PVirtualNode; Column: TColumnIndex; var Result: Integer);
var
  x1,x2 : PEmbExt;
begin
  result := 0;
  x1 := vtExts.GetNodeData(Node1);
  x2 := vtExts.GetNodeData(Node2);
  if assigned(x1) and assigned(x2) then
  case Column of
    0 : result := CompareText(x1^.Ext, x2^.Ext) ;
    1 : result := CompareText(x1^.Desc, x2^.Desc);
  end;


end;

procedure TForm1.vtExtsColumnClick(Sender: TBaseVirtualTree;
  Column: TColumnIndex; Shift: TShiftState);
begin
{  if vtExts.Header.SortColumn = column then
  begin
    vtExts.Header.SortDirection := pred(vtExts.Header.SortDirection);
  end
  else
  begin
    vtExts.Header.SortColumn := Column;
  end;}
end;

end.
