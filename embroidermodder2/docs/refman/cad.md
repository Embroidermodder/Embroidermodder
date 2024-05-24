# CAD Commands

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
