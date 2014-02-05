unit libembroidery;
{**************************************************************************}
{                                                                          }
{    This C DLL header file first (automatic) conversion generated by:     }
{    HeadConv 4.0 (c) 2000 by Bob Swart (aka Dr.Bob - www.drbob42.com)     }
{      Final Delphi-Jedi (Darth) command-line units edition                }
{                                                                          }
{    Generated Date: 25/01/2014                                            }
{    Generated Time: 04:24:18                                              }
{                                                                          }
{**************************************************************************}

interface
uses
{$IFDEF FPC}
  dynlibs;
{$ELSE}
  {$IFDEF WIN32}
    Windows;
  {$ELSE}
    //Dos, Delphi 1, Delphi 2
    Wintypes, WinProcs;
  {$ENDIF}
{$ENDIF}  

{.$DEFINE EMBOBJECT}
{.$DEFINE EMBOBJECTS_PREV}
{$IFNDEF FPC}
  {$IFNDEF MSDOS}
    {$DEFINE SetErrorMode}
  {$ENDIF}
{$ENDIF}

type

  TEmbColor = record
    r: Byte;
    g: Byte;
    b: Byte;
  end {EmbColor_};
  
  TEmbPoint = record
    xx: Double;   {= absolute position (not relative) }
    yy: Double;   {= positive is up, units are in mm }
  end {EmbPoint_};

  PEmbPointList = ^TEmbPointList;
  TEmbPointList = record
    point: TEmbPoint;
    next: PEmbPointList;
  end {EmbPointList_};

  TEmbPointObject = record
    point: TEmbPoint;
	  // Properties
    lineType: Integer;
    color: TEmbColor;
  end {EmbPointObject_};

  PEmbPointObjectList = ^TEmbPointObjectList;
  TEmbPointObjectList = record
    pointObj: TEmbPointObject;
    next: PEmbPointObjectList;
  end {EmbPointObjectList_};


  TEmbSettings = record
    dstJumpsPerTrim: Word;
    home: TEmbPoint;
  end {EmbSettings_};

  TEmbHoop = record
    width: Double;
    height: Double;
  end {EmbHoop_};

  TEmbStitch = record
    flags: Integer;   {= uses codes defined above }
    xx: Double;       {= absolute position (not relative) }
    yy: Double;       {= positive is up, units are in mm }
    color: Integer;
  end {EmbStitch_};

  PEmbStitchList = ^TEmbStitchList;
  TEmbStitchList = record
    stitch: TEmbStitch;
    next: PEmbStitchList;
  end;


  TEmbThread = record
    color: TEmbColor;
    description: PCHAR;
    catalogNumber: PCHAR;
  end {EmbThread_};

  PEmbThreadList = ^TEmbThreadList;
  TEmbThreadList = record
    thread: TEmbThread;
    next: PEmbThreadList;
  end {EmbThreadList_};


  TEmbArc = record
    startX: Double; {= absolute position (not relative) }
    startY: Double;
    midX: Double;   {= absolute position (not relative) }
    midY: Double;
    endX: Double;   {= absolute position (not relative) }
    endY: Double;
  end {EmbArc_};

  TEmbArcObject = record
    arc: TEmbArc;  {+// Properties*/ }
    lineType: Integer;
    color: TEmbColor;
  end {EmbArcObject_};


  PEmbArcObjectList = ^TEmbArcObjectList;
  TEmbArcObjectList = record
    arcObj: TEmbArcObject;
    next: PEmbArcObjectList;
  end {EmbArcObjectList_};



  TEmbCircle = record
    centerX: Double;
    centerY: Double;
    radius: Double;
  end {EmbCircle_};

  TEmbCircleObject = record
    circle: TEmbCircle;

    // Properties
    lineType: Integer;
    color: TEmbColor;
  end {EmbCircleObject_};

  PEmbCircleObjectList = ^TEmbCircleObjectList;
  TEmbCircleObjectList = record
    circleObj : TEmbCircleObject;
    next : PEmbCircleObjectList;
  end;

  TEmbEllipse = record
    centerX: Double;
    centerY: Double;
    radiusX: Double;
    radiusY: Double;
  end {EmbEllipse_};

  TEmbEllipseObject = record
    ellipse: TEmbEllipse;
    rotation: Double;
    //Properties
    lineType: Integer;
    color: TEmbColor;
  end {EmbEllipseObject_};

  PEmbEllipseObjectList = ^TEmbEllipseObjectList;
  TEmbEllipseObjectList = record
    EllipseObj : TEmbEllipseObject;
    next : PEmbEllipseObjectList;
  end;


  TEmbLine = record
    x1: Double;
    y1: Double;
    x2: Double;
    y2: Double;
  end {EmbLine_};

  TEmbLineObject = record
    line: TEmbLine;

    // Properties
    lineType: Integer;
    color: TEmbColor;
  end {EmbLineObject_};


  PEmbLineObjectList = ^TEmbLineObjectList;
  TEmbLineObjectList = record
    LineObj : TEmbLineObject;
    next : PEmbLineObjectList;
  end;

type
  TEmbFlag = Integer;

  PEmbFlagList = ^TEmbFlagList;
  TEmbFlagList = record
    flag: Integer;
    next: PEmbFLAGList;
  end {EmbFlagList_};

  TEmbPathObject = record
    pointList: PEmbPointList;
    flagList: PEmbFLAGList;
    // Properties
    lineType: Integer;
    color: TEmbColor;
  end {EmbPathObject_};
  PEmbPathObject = ^TEmbPathObject;

  PEmbPathObjectList = ^TEmbPathObjectList;
  TEmbPathObjectList = record
    pathObj: PEmbPathObject;
    next: PEmbPathObjectList;
  end {EmbPathObjectList_};

  TEmbPolygonObject = record
    pointList: PEmbPointList;
    // Properties
    lineType: Integer;
    color: TEmbColor;
  end {EmbPolygonObject_};
  PEmbPolygonObject = TEmbPolygonObject;


  PEmbPolygonObjectList = ^TEmbPolygonObjectList;
  TEmbPolygonObjectList = record
    polygonObj: PEmbPolygonObject;
    next: PEmbPolygonObjectList;
  end {EmbPolygonObjectList_};


  TEmbPolylineObject = record
    pointList: PEmbPointList;
    // Properties
    lineType: Integer;
    color: TEmbColor;
  end {EmbPolylineObject_};
  PEmbPolylineObject= ^TEmbPolylineObject;

  PEmbPolylineObjectList = ^TEmbPolylineObjectList;
  TEmbPolylineObjectList = record
    polylineObj: PEmbPolylineObject;
    next: PEmbPolylineObjectList;
  end {EmbPolylineObjectList_};


  TEmbRect = record
    top: Double;
    left: Double;
    bottom: Double;
    right: Double;
  end {EmbRect_};

  TEmbRectObject = record
    rect: TEmbRect;
    rotation: Double;
    radius: Double;
    // Properties
    lineType: Integer;
    color: TEmbColor;
  end {EmbRectObject_};


  PEmbRectObjectList = ^TEmbRectObjectList;
  TEmbRectObjectList = record
    rectObj: TEmbRectObject;
    next: PEmbRectObjectList;
  end {EmbRectObjectList_};


  TEmbBezier = record
    startX: Double;
    startY: Double;
    control1X: Double;
    control1Y: Double;
    control2X: Double;
    control2Y: Double;
    endX: Double;
    endY: Double;
  end {EmbBezier_};

  PEmbSplineObject = ^TEmbSplineObject;
  TEmbSplineObject = record
    bezier: TEmbBezier;
    next: PEmbSplineObject;
    // Properties
    lineType: Integer;
    color: TEmbColor;
  end {EmbSplineObject_};

{ A list of bezier curves is a B-spline }
  PEmbSplineObjectList = ^TEmbSplineObjectList;
  TEmbSplineObjectList = record
    splineObj: TEmbSplineObject;
    next: PEmbSplineObjectList;
  end {EmbSplineObjectList_};

{$IFDEF EMBOBJECT}
type
  PEmbObject = ^TEmbObject;
  TEmbObject = record
    kind: Char;
    pointList: PEmbPointList;

    // Properties*/
    lineType: Integer;
    color: TEmbColor;
  end {};

  PEmbObjectList = ^TEmbObjectList;
  TEmbObjectList = record
    objectObj: PEmbObject;
    {$IFDEF EMBOBJECTS_PREV}
    prev: PEmbObjectList;
    {$ENDIF}
    next: PEmbObjectList;
    child: PEmbObjectList;
  end {EmbObjectList_};
{$ENDIF}

  TEmbPattern = record
    settings: TEmbSettings;
    hoop: TEmbHoop;
    stitchList: PEmbStitchList;
    threadList: PEmbThreadList;
    arcObjList: PEmbArcObjectList;
    circleObjList: PEmbCircleObjectList;
    ellipseObjList: PEmbEllipseObjectList;
    lineObjList: PEmbLineObjectList;
    pathObjList: PEmbPathObjectList;
    pointObjList: PEmbPointObjectList;
    polygonObjList: PEmbPolygonObjectList;
    polylineObjList: PEmbPolylineObjectList;
    rectObjList: PEmbRectObjectList;
    splineObjList: PEmbSplineObjectList;
    lastStitch: PEmbStitchList;
    lastThread: PEmbThreadList;
    lastArcObj: PEmbArcObjectList;
    lastCircleObj: PEmbCircleObjectList;
    lastEllipseObj: PEmbEllipseObjectList;
    lastLineObj: PEmbLineObjectList;
    lastPathObj: PEmbPathObjectList;
    lastPointObj: PEmbPointObjectList;
    lastPolygonObj: PEmbPolygonObjectList;
    lastPolylineObj: PEmbPolylineObjectList;
    lastRectObj: PEmbRectObjectList;
    lastSplineObj: PEmbSplineObjectList;
    
    currentColorIndex: Integer;
    lastX: Double;
    lastY: Double;
  end {EmbPattern_};
  PEmbPattern = ^TEmbPattern;


var
  embPattern_create:  function(): PEmbPattern cdecl  {$IFDEF WIN32} stdcall {$ENDIF};
  embPattern_read:    function(pattern: PEmbPattern;
            const fileName: PChar): Integer cdecl  {$IFDEF WIN32} stdcall {$ENDIF};

  embPattern_write:   function(const pattern: PEmbPattern;
             const fileName: PChar): Integer cdecl  {$IFDEF WIN32} stdcall {$ENDIF};

  embFormat_type:     function(const fileName: PChar): Integer cdecl  {$IFDEF WIN32} stdcall {$ENDIF};

{$IFDEF EMBOBJECT}
 embObject_create:   function (kind: Char;
					  points: PEmbPointList;
					  color: TEmbColor;
					  lineType: Integer): PEmbObject cdecl  {$IFDEF WIN32} stdcall {$ENDIF};
 embObject_free:     procedure (pointer: PEmbObject) cdecl  {$IFDEF WIN32} stdcall {$ENDIF};
 embObjectList_create: function (data: PEmbObject): PEmbObjectList cdecl  {$IFDEF WIN32} stdcall {$ENDIF};
 embObjectList_add:  function (pointer: PEmbObjectList;
						 data: PEmbObject): PEmbObjectList cdecl  {$IFDEF WIN32} stdcall {$ENDIF};
 embObjectList_count:  function (pointer: PEmbObjectList): Integer cdecl  {$IFDEF WIN32} stdcall {$ENDIF};
 embObjectList_empty:  function (pointer: PEmbObjectList): Integer cdecl  {$IFDEF WIN32} stdcall {$ENDIF};
 embObjectList_free:   procedure (pointer: PEmbObjectList) cdecl  {$IFDEF WIN32} stdcall {$ENDIF};
{$ENDIF}


var
  DLLLoaded: Boolean { is DLL (dynamically) loaded already? }
    {$IFDEF WIN32} = False {$ENDIF};

const
{$IFDEF win32}
  emblib = 'libembroidery.dll';
{$ELSE}
  {$IFDEF darwin}
    emblib = 'libembroidery';
    {$linklib libembroidery}
  {$ELSE}
    emblib = 'libembroidery.so';
  {$ENDIF}
{$ENDIF}
    
implementation

uses
  SysUtils;
var
  SaveExit: pointer;
  DLLHandle: THandle;
{$IFDEF SetErrorMode}
  ErrorMode: Integer;
{$ENDIF}

  procedure NewExit; far;
  begin
    ExitProc := SaveExit;
    FreeLibrary(DLLHandle)
  end {NewExit};

procedure LoadDLL;
begin
  if DLLLoaded then Exit;
{$IFDEF SetErrorMode}
  ErrorMode := SetErrorMode($8000{SEM_NoOpenFileErrorBox});
{$ENDIF}
  DLLHandle := LoadLibrary(emblib);
  if DLLHandle >= 32 then
  begin
    DLLLoaded := True;
    SaveExit := ExitProc;
    ExitProc := @NewExit;

    @embPattern_create := GetProcAddress(DLLHandle,'embPattern_create');
  {$IFDEF WIN32}
    //I need to sure we are using correct lib, but this context doesn't work:
    //Assert(assigned(@embPattern_read));
  {$ENDIF}

    @embPattern_read := GetProcAddress(DLLHandle,'embPattern_read');
    @embPattern_write := GetProcAddress(DLLHandle,'embPattern_write');
    @embFormat_type := GetProcAddress(DLLHandle,'embFormat_type');

{$IFDEF EMBOBJECT}
    @embObject_create   := GetProcAddress(DLLHandle,'embObject_create');
    @embObject_free     := GetProcAddress(DLLHandle,'embObject_free');
    @embObjectList_create := GetProcAddress(DLLHandle,'embObjectList_create');
    @embObjectList_add    := GetProcAddress(DLLHandle,'embObjectList_add');
    @embObjectList_count  := GetProcAddress(DLLHandle,'embObjectList_count');
    @embObjectList_empty  := GetProcAddress(DLLHandle,'embObjectList_empty');
    @embObjectList_free   := GetProcAddress(DLLHandle,'embObjectList_free');
{$ENDIF}  


  end
  else
  begin
    DLLLoaded := False;
    raise Exception.Create( emblib +' could not be loaded !!' );
  end;
{$IFDEF SetErrorMode}
  SetErrorMode(ErrorMode)
{$ENDIF}
end {LoadDLL};

initialization
  LoadDLL;
end.
