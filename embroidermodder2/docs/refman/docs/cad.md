
\newcommand{\indexi}[1]{\index{#1}#1}
\newcommand{\indext}[1]{\index{#1}\texttt{#1}}


A CAD command\indext{CAD}\indext{command} in Embroidermodder is referred to as
an \emph{action}\indext{action} and the data needed for it to function is stored
in the \indext{``action_table``}. For a regular user who wishes to use
more shortcuts and macros

\begin{longtable}{p{1cm} p{2.5cm} p{2cm} p{7cm}}
\caption{Overview of the CAD commands available on all platorms but with
various levels of support and applicability.}
\label{tab:command-table} \\
\textbf{ID} &
\textbf{name} &
\textbf{arguments} &
\textbf{description} \\

0 &
\indext{NEW} &
none &
Create a new EmbPattern with a new tab in the GUI. \\

1 &
\indext{OPEN} &
char array &
Open an EmbPattern with the supplied filename `fname'. \\

2 &
\indext{SAVE} &
char array &
Save the current loaded EmbPattern to the supplied filname `fname'. \\

3 &
\indext{SCALE} &
selected objects, 1 float &
Scale all selected objects by the number supplied, without selection scales the
entire design. \\

4 &
\indext{CIRCLE} &
mouse co-ords &
Adds a circle to the design based on the supplied numbers, converts to stitches
on save for stitch only formats. \\

5 &
\indext{OFFSET} &
mouse co-ords &
Shifts the selected objects by the amount given by the mouse co-ordinates. \\

6 &
\indext{EXTEND} &
 &
\\

7 &
\indext{TRIM} & 
selected stitches &
Sets the flag for the stitches selected to \texttt{TRIM}. \\

8 &
\indext{``break_at_point``} & 
&
\\

9 &
\indext{``break_2_points``} &
&
\\

10 &
\indext{FILLET} &
&
\\

11 &
\indext{STAR} &
mouse co-ords &
Adds a star to the vector layer at the mouse co-ords. \\

12 &
\indext{textsingle} &
&
\\

13 &
\indext{CHAMFER} &
&
\\

14 &
\indext{SPLIT} &
A selected line in the vector layer. Otherwise 2 mouse co-ords in sequence. &
Seperates stitch blocks by cutting threads between those either side of a line
supplied by the user then rerouting the stitches. \\

15 &
\indext{AREA} &
Selected objects &
\\

16 &
\indext{TIME} &
None &
Prints the current time as ISO formatted UTC to the console. \\

17 &
\indext{PICKADD} & 
None &
\\

16 &
\indext{ZOOMFACTOR} & 
float &
Sets the zoom factor to the supplied argument. \\

17 &
\indext{PRODUCT} & 
None &
Prints the name and build code of the running version of Embroidermodder
to disambiguate between forks and build versions. The string may only
start with \texttt{embroidermodder} if the build is from the official Embroidermodder
project under the terms of the license. \\

18 &
\indext{PROGRAM/PROG} &
&
\\

19 &
\indext{ZOOMWINDOW} &
&
\\

20 &
\indext{DIVIDE} &
&
\\

21 &
\indext{FIND} &
int &
Select the stitch at the index given and center the view on it. \\

22 &
\indext{RECORD} &
None &
Start recording all user input that passes through the actuator
(i.e. actions from this table, with all the data passed as arguments)
for forming macros. Stop recording if RECORD/PLAYBACK/END is issued. \\

23 &
\indext{PLAYBACK} &
None &
\\

24 &
\indext{ROTATE} &
&
\\

25 &
\indext{RGB} &
&
\\

26 &
\indext{move} &
&
\\

27 &
\indext{grid} &
&
\\

28 &
\indext{griphot} &
&
\\

29 &
\indext{gripcolor} &
&
\\

30 &
\indext{gripcool} &
&
\\

31 &
\indext{gripsize} &
&
\\

32 &
\indext{highlight} &
&
\\

33 &
\indext{units} &
&
\\

34 &
\indext{locatepoint} &
&
\\

35 &
\indext{distance} &
  &
\\

36 &
 \indext{ARC} &
 &
\\

37 &
\indext{ELLIPSE} &
 &
\\

38 &
\indext{ARRAY} &
 &
\\

39 &
\indext{POINT} &
 &
\\

40 &
\indext{POLYLINE} &
 &
\\

41 &
\indext{POLYGON} &
 &
\\

42 &
\indext{rectangle} &
 &
\\

43 &
\indext{line} &
 &
\\

44 &
\indext{arc-rt} &
 &
\\

45 &
\indext{dolphin} &
&
\\

46 &
\indext{heart} &
&

\end{longtable}