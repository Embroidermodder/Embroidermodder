# CAD Command Overview

A _CAD command_ in Embroidermodder is referred to as an action and the data needed for it
to function is stored in the `command_data`. For a regular user who wishes to use
more shortcuts and macros: a list of commands can be fed into the prompt similar to
a `GCODE` file.

## CAD Command Table

These are available on all platorms but with various levels of support and applicability.

| ID | Name | Arguments  | Description                                            |
|----|------|------------|--------------------------------------------------------|
| 0  | `NEW`  | none       | Create a new EmbPattern with a new tab in the GUI.     |
| 1  | `OPEN` | char array | Open an EmbPattern with the supplied filename in argument 0. |
| 2  | `SAVE` | char array | Save the current loaded EmbPattern to the supplied filename in argument 0. |
| 3 | `SCALE` | selected objects, 1 float | Scale all selected objects by the number supplied, without selection scales the entire design. |
| 4 | `CIRCLE` | mouse co-ords | Adds a circle to the design based on the supplied numbers, converts to stitches on save for stitch only formats. |
| 5 | `OFFSET` | mouse co-ords | Shifts the selected objects by the amount given by the mouse co-ordinates. |
| 6 | `EXTEND` | | |
| 7 | `TRIM` | selected stitches | Sets the flag for the stitches selected to `TRIM`. |
| 8 | `break_at_point` | | |
| 9 | `break_2_points` | | |
| 10 | `FILLET` | | |
| 11 | `STAR` | mouse co-ords | Adds a star to the vector layer at the mouse co-ords. |
| 12 | `TEXTSINGLE` | | |
| 13 | `CHAMFER` | | |
| 14 | `SPLIT` | A selected line in the vector layer. Otherwise 2 mouse co-ords in sequence. | Seperates stitch blocks by cutting threads between those either side of a line supplied by the user then rerouting the stitches. |
| 15 | `AREA` | Selected objects | |
| 16 | `TIME` | None | Prints the current time as ISO formatted UTC to the console. |
| 17 | `PICKADD` | None | |
| 16 | `ZOOMFACTOR` | float | Sets the zoom factor to the supplied argument. |
| 17 | `PRODUCT` | None | Prints the name and build code of the running version of Embroidermodder to disambiguate between forks and build versions. The string may only start with \texttt{embroidermodder} if the build is from the official Embroidermodder project under the terms of the license. |
| 18 | `PROGRAM/PROG` | | |
| 19 | `ZOOMWINDOW` | | |
| 20 | \indext{DIVIDE} | | |
| 21 | \indext{FIND} | int | Select the stitch at the index given and center the view on it. |
| 22 | \indext{RECORD} | None | Start recording all user input that passes through the actuator (i.e. actions from this table, with all the data passed as arguments) for forming macros. Stop recording if RECORD/PLAYBACK/END is issued. |
| 23 | \indext{PLAYBACK} | None | |
| 24 | \indext{ROTATE} | | |
| 25 | \indext{RGB} | | |
| 26 | \indext{move} | | |
| 27 | \indext{grid} | | |
| 28 | \indext{griphot} | | |
| 29 | \indext{gripcolor} | | |
| 30 | \indext{gripcool} | | |
| 31 | \indext{gripsize} | | |
| 32 | \indext{highlight} | | |
| 33 | \indext{units} | | |
| 34 | \indext{locatepoint} | | |
| 35 | \indext{distance} | | |
| 36 | `ARC` | | |
| 37 | `ELLIPSE` | | |
| 38 | `ARRAY` | | |
| 39 | `POINT` | | |
| 40 | `POLYLINE` | | |
| 41 | `POLYGON` | | |
| 42 | `rectangle` | | |
| 43 | `line` | | |
| 44 | `arc-rt` | | |
| 45 | `dolphin` | | |
| 46 | `heart` | | |

## Actions

### ABOUT

| index | arguments | flags |
|------|------|------|
| 0 | none | |

### ADD-ARC

| index | arguments | flags |
|------|------|------|
| 1 | mouse co-ords | |

### ADD-CIRCLE

| index | arguments | flags |
|------|------|------|
| 2 | mouse co-ords | |

### ADD-DIM-LEADER

| index | arguments | flags |
|------|------|------|
| 3 | none | |

### ADD-ELLIPSE

| index | arguments | flags |
|------|------|------|
4 & none & 

### ADD-GEOMETRY

| index | arguments | flags |
|------|------|------|
5 & none & 

### ADD-HORIZONTAL-DIMENSION

| index | arguments | flags |
|------|------|------|
6 & none & 

### ADD-IMAGE

| index | arguments | flags |
|------|------|------|
7 & none & 

### ADD-INFINITE-LINE

| index | arguments | flags |
|------|------|------|
8 & none & 

### ADD-LINE

| index | arguments | flags |
|------|------|------|
| 9 | none | |

### ADD-PATH

| index | arguments | flags |
|------|------|------|
| 10 | none | |

### ADD-POINT

\index{ADD-POINT}

index 11

### ADD-POLYGON

\index{ADD-POLYGON}

index 12

### ADD-POLYLINE}
\index{ADD-POLYLINE}

index 13

### ADD-RAY}
\index{ADD-RAY}

index 14

### ADD-RECTANGLE}
\index{ADD-RECTANGLE}

index 15

### ADD-REGULAR-POLYGON}
\index{ADD-REGULAR-POLYGON}

index 16

### ADD-ROUNDED-RECTANGLE}
\index{action}

index 17

### ADD-RUBBER}
\index{ADD-RUBBER}

index 18

### ADD-SLOT}
\index{action}

index 19

### ADD-TEXT-MULTI}
\index{action}

index 20

### ADD-TEXT-SINGLE}
\index{action}

index 21

### ADD-TO-SELECTION}
\index{action}

index 22

### ADD-TRIANGLE}
\index{action}

index 23

### ADD-VERTICAL-DIMENSION}
\index{action}

index 24

### ALERT}
\index{action}

index 25

### ALLOW-RUBBER}
\index{action}

index 26

### APPEND-HISTORY}
\index{action}

index 27

### CALCULATE-ANGLE}
\index{action}

index 28

### CALCULATE-DISTANCE}
\index{action}

index 29

### CHANGELOG}
\index{action}

index 30

### CLEAR-RUBBER}
\index{action}

index 31

### CLEAR-SELECTION}
\index{action}

index 32

### COPY}
\index{action}

index 33

### COPY-SELECTED}
\index{action}

index 34

### CUT}
\index{action}

index 35

### CUT-SELECTED}
\index{action}

index 36

### DAY}
\index{action}

index 37

### DEBUG}
\index{action}

index 38

### DELETE-SELECTED}
\index{action}

index 39

### DESIGN-DETAILS}
\index{action}

index 40

### DO-NOTHING}
\index{action}

index 41

### END}
\index{action}

index 42

### ERROR}
\index{action}

index 43

### HELP}
\index{action}

index 44

### ICON}
\index{action}

index 45

### INIT}
\index{action}

index 46

### MESSAGEBOX}
\index{action}

index 47, 3 char arrays deliminated by quotes Example Call

### MIRROR-SELECTED}
\index{action}

index 48

### MOUSE-X}
\index{action}

index 49

### MOUSE-Y}
\index{action}

index 50

### MOVE-SELECTED}
\index{action}

index 51

### NEW}
\index{action}

index 52

### NIGHT}
\index{action}

index 53

### NUM-SELECTED}
\index{action}

index 54

### OPEN}
\index{action}

index 55

### PAN}
\index{action}

index 56

### PASTE}
\index{PASTE}

index 57

### PASTE-SELECTED}
\index{PASTE-SELECTED}

index 58

### PERPENDICULAR-DISTANCE}
\index{PERPENDICULAR-DISTANCE}

index 59

### PLATFORM}
\index{PLATFORM}

index 60

### PREVIEW-OFF}
\index{PREVIEW-OFF}

index 61

### PREVIEW-ON}
\index{PREVIEW-ON}

index 62

### PRINT}
\index{PRINT}

index 63

### PRINT-AREA}
\index{PRINT-AREA}

index 64

### QSNAP-X}
\index{QSNAP-X}

index 65

### QSNAP-Y}
\index{QSNAP-Y}

index 66

### EXIT}
\index{EXIT}

 index 67

### REDO}
\index{REDO}

index 68

### ROTATE-SELECTED}
\index{ROTATE-SELECTED}

index 69

### RUBBER}
\index{RUBBER}

index 70

### SCALE-SELECTED}
\index{SCALE-SELECTED}

index 71

### SELECT-ALL}
\index{SELECT-ALL}

index 72

### SETTINGS-DIALOG}
\index{action}

index 73

### SET-BACKGROUND-COLOR}
\index{action}

index 74

### SET-CROSSHAIR-COLOR}
\index{action}

index 75

### SET-CURSOR-SHAPE}
\index{action}

index 76

### SET-GRID-COLOR}
\index{action}

index 77

### SET-PROMPT-PREFIX}
\index{action}

index 78

### SET-RUBBER-FILTER}
\index{action}

index 79

### SET-RUBBER-MODE}
\index{action}

index 80

### SET-RUBBER-POINT}
\index{action}

index 81

### SET-RUBBER-TEXT}
\index{action}

index 82

### SPARE-RUBBER}
\index{action}

index 83

### TIP-OF-THE-DAY}
\index{action}

index 84

### TODO}
\index{action}

 index 85

### UNDO}
\index{action}

 index 86

### VERSION}
\index{action}

index 87

### VULCANIZE}
\index{action}

index 88

### WHATS-THIS}
\index{action}

index 89

### WINDOW-CLOSE}
\index{action}

index 90

### WINDOW-CLOSE-ALL}
\index{action}

index 91

### WINDOW-TILE}
\index{action}

index 92

### WINDOW-CASCADE}
\index{action}

index 93

### WINDOW-NEXT}
\index{action}

index 94

### WINDOW-PREVIOUS}
\index{action}

index 95

### ZOOM}
\index{action}

 index 96

### ZOOM-IN}
\index{action}

index 97

### TEST}
\index{action}

 index 98

### SLEEP}
\index{action}

index 99

### LAYER-EDITOR}
\index{action}

index 100

### MAKE-LAYER-CURRENT}
\index{action}

index 101

### TEXT-BOLD}
\index{action}

index 102

### TEXT-ITALIC}
\index{action}

index 103

### TEXT-UNDERLINE}
\index{action}

index 104

### TEXT-STRIKEOUT}
\index{action}

index 105

### TEXT-OVERLINE}
\index{action}

index 106

### LAYER-PREVIOUS}
\index{action}

index 107

### ICON16}
\index{action}

index 108

### ICON24}
\index{action}

index 109

### ICON32}
\index{action}

index 110

### ICON48}
\index{action}

index 111

### ICON64}
\index{action}

index 112

### ICON128}
\index{action}

index 113

### SAVE}
\index{action}

index 114

### SAVEAS}
\index{action}

index 115

### PAN-REAL-TIME}
\index{action}

index 116

### PAN-POINT}
\index{action}

index 117

### PAN-LEFT}
\index{action}

index 118

### PAN-RIGHT}
\index{action}

index 119

### PAN-UP}
\index{action}

index 120

### PAN-DOWN}
\index{action}

index 121

### ZOOM-REAL-TIME}
\index{action}

index 122

### ZOOM-PREVIOUS}
\index{action}

index 123

### ZOOM-WINDOW}
\index{action}

index 124

### ZOOM-DYNAMIC}
\index{action}

index 125

### ZOOM-OUT}
\index{action}

index 126

### ZOOM-EXTENTS}
\index{action}

index 127

### LAYERS}
\index{action}

index 128

### LAYER-SELECTOR}
\index{action}

index 129

### TREBLECLEF}
\index{action}

index 130

### COLOR-SELECTOR}
\index{action}

index 131

### LINE-TYPE-SELECTOR}
\index{action}

index 132

### LINE-WEIGHT-SELECTOR}
\index{action}

index 133

### ZOOM-SCALE}
\index{action}

index 134

### ZOOM-CENTER}
\index{action}

index 135

### HIDE-ALL-LAYERS}
\index{action}

index 136

### ZOOM-SELECTED}
\index{action}

index 137

### ZOOM-ALL}
\index{action}

index 138

### ADD-HEART}
\index{action}

index 139

### ADD-SINGLE-LINE-TEXT}
\index{action}

index 140

### SHOW-ALL-LAYERS}
\index{action}

index 141

### FREEZE-ALL-LAYERS}
\index{action}

index 142

### THAW-ALL-LAYERS}
\index{action}

index 143

### LOCK-ALL-LAYERS}
\index{action}

index 144

### UNLOCK-ALL-LAYERS}
\index{UNLOCK-ALL-LAYERS}

index 145

### ADD-DOLPHIN}
\index{ADD-DOLPHIN}

index 146

### ADD-DISTANCE}
\index{ADD-DISTANCE}

index 147

### LOCATE-POINT}
\index{LOCATE-POINT}

index 148

### QUICKSELECT}
\index{QUICKSELECT}

index 149

### SPELLCHECK}
\index{SPELLCHECK}

index 150

### DISTANCE}
\index{DISTANCE}

index 151

### MOVE}
\index{MOVE}

index 152

### QUICKLEADER}
\index{QUICKLEADER}

index 153

### RGB}
\index{RGB}

 index 154

### ROTATE}
\index{ROTATE}

index 155

### SANDBOX}
\index{SANDBOX}

index 156

### ADD-SNOWFLAKE}
\index{ADD-SNOWFLAKE}

index 157

### ADD-STAR}
\index{ADD-STAR}

index 158

### DELETE}
\index{DELETE}

index 159

### SCALE}
\index{SCALE}

index 160

### SINGLE-LINE-TEXT}
\index{SINGLE-LINE-TEXT}

index 161

### SYSWINDOWS}
\index{SYSWINDOWS}

index 162
