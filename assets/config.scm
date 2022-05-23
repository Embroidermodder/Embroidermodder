; Embroidermodder 2.
; Copyright 2013-2022 The Embroidermodder Team
; Distributed under the terms of the zlib license.
; https://www.libembroidery.org
; ------------------------------------------------------------
; Configuration and data specific to the GUI.
;

(define (title) "Embroidermodder")
(define (version) "2.0.0-alpha")
(define (git-build-hash) "92403820fc2c6be46b884b6500e01a239363fc82")
(define (interface-font) "assets/fonts/source-code-pro/TTF/SourceCodePro-Black.ttf")
(define (text-font) "assets/fonts/source-sans/TTF/SourceSans3-Regular.ttf")
(define (n-tabs) 0)
(define (tab-index) 0)
(define (n-patterns) 0)
(define (real-render) 0)
(define (max-string-length) 500)
(define (debug-mode) 1)
(define (window-offset-x) 100)
(define (window-offset-y) 100)
(define (window-width) 640)
(define (window-height) 480)
(define (max-distance) 1000000.0)
(define (general-icon-size) 16)
(define (background-color) "446688")
(define (interface-color) "6688AA")
(define (button-background-color) "88AAFF")
(define (color-mode) 0)
(define (general-mdi-bg-use-logo) 0)
(define (general-mdi-bg-use-texture) 0)
(define (general-mdi-bg-use-color) 0)
(define (selection-coolgrip-color) "FFFFFF")
(define (selection-hotgrip-color) "FFFFFF")

(define (application-folder) "~/.embroidermodder2/")
(define (printing-default-device) 0)
(define (ruler-color) "FFFFFF")
(define (qsnap-locator-color) "FFFFFF")
(define (grid-type) "cartesian")
(define (selection-coolgrip-color) "FFFFFF")
(define (selection-hotgrip-color) "FFFFFF")
(define (translation-table)
  (vector
    "default"
    "french"
    "german"
    "spanish"))

;mdi-window
;
;    QToolBar* toolbar[10]
;    QMenu* menu[10]
;    StatusBarButton* status-bar[8]
;    QToolButton* toolButton[PROPERTY-EDITORS]
;    QLineEdit* lineEdit[LINEEDIT-PROPERTY-EDITORS]
;    QComboBox* comboBox[COMBOBOX-PROPERTY-EDITORS];
;
;    QString opensave-custom-filter
;
;
;    paste-delta
;    EmbVector scene-press-point
;    QPoint press-point
;    EmbVector sceneMovePoint
;    QPoint movePoint
;    EmbVector sceneReleasePoint
;    QPoint releasePoint
;    EmbVector sceneGripPoint
;
;    Color ruler-color;
;
;    QPoint  viewMousePoint;
;    sceneMousePoint;
;    unsigned int qsnap-locator-color;
;    unsigned int grip-color-cool;
;    unsigned int grip-color-hot;
;    unsigned int crosshair-color;
;    int precision-angle;
;    int precision-length;
;
;    QLabel* statusBarMouseCoord
;
; Used when checking if fields vary
;    QString field-old-text
;    QString field-new-text
;    QString field-varies-text
;    QString field-yes-text
;    QString field-no-text
;    QString field-on-text
;    QString field-off-text
;
;    QToolButton* toolButtonArcClockwise
;    QComboBox* comboBoxArcClockwise
;
;    Qgroup-box* group-boxGeometry[32]
;    Qgroup-box* group-boxGeneral
;    Qgroup-box* group-boxMiscArc
;    Qgroup-box* group-boxMiscPath
;    Qgroup-box* group-boxMiscPolyline
;    Qgroup-box* group-boxTextTextSingle
;    Qgroup-box* group-boxMiscTextSingle


(define (general-language) "default")
(define (general-icon-theme) "default")
(define (general-mdi-bg-logo) "assets/icons/logo.png")
(define (general-mdi-bg-texture) "assets/icons/texture.png")
(define (general-mdi-bg-color) "FFFFFF")
(define (general-current-tip) 0)
(define (general-tip-of-the-day) 0)
(define (general-system-help-browser) 1)
(define (general-check-for-updates) 0)

; QSnap Properties
(define (qsnap-locator-color) "FFFFFF")
(define (qsnap-endpoint) 0)
(define (qsnap-midpoint) 0)
(define (qsnap-center) 0)
(define (qsnap-node) 0)
(define (qsnap-quadrant) 0)
(define (qsnap-intersection) 0)
(define (qsnap-extension) 0)
(define (qsnap-insertion) 0)
(define (qsnap-perpendicular) 0)
(define (qsnap-tangent) 0)
(define (qsnap-nearest) 0)
(define (qsnap-apparent) 0)
(define (qsnap-parallel) 0)
(define (qsnap-enabled) 0)
(define (qsnap-locator-size) 16)
(define (qsnap-aperture-size) 16)
(define (qsnap-active) 0)
(define (qsnap-toggle) 0)

; Text Settings
(define (text-style-bold) 0)
(define (text-style-italic) 0)
(define (text-style-underline) 0)
(define (text-style-strikethrough) 0)

; Line Weight Settings
(define (lwt-show-lwt) 0)
(define (lwt-real-render) 0)
(define (lwt-default-lwt) 1.0)

; Ruler Settings
(define (ruler-metric) 1)
(define (ruler-show-on-load) 1)
(define (ruler-pixel-size) 30)

(define (tab-index) 0)

; Grid Settings
(define (grid-size-radius) 10.0)
(define (grid-center-on-origin) 0)
(define (grid-center) (vector 0.5 0.5))
(define (grid-size) (vector 10.0 10.0))
(define (grid-spacing) (vector 10.0 10.0))
(define (grid-size-radius) 10.0)
(define (grid-spacing-radius) 10.0)
(define (grid-spacing-angle) 10.0)
(define (grid-show-on-load) 0)
(define (grid-show-origin) 1)
(define (grid-color-match-crosshair) 1)
(define (grid-load-from-file) 1)
(define (grid-color) "FFFFFF")

(define (symbol-scale) 0.01)

(define (display-use-opengl) 1)
(define (display-renderhint-aa) 1)
(define (display-renderhint-text-aa) 1)
(define (display-renderhint-smooth-pix) 1)
(define (display-renderhint-high-aa) 1)
(define (display-renderhint-noncosmetic) 1)
(define (display-show-scrollbars) 1)
(define (display-scrollbar-widget-num) 1)
(define (display-zoomscale-in) 1)
(define (display-zoomscale-out) 1)
(define (display-selectbox-alpha) 1)
(define (display-crosshair-percent) 10)

(define (opensave-open-format) "csv")
(define (opensave-save-format) "csv")
(define (opensave-recent-directory) "~")
(define (opensave-recent-files)
  (vector "new-file.csv"))
(define (opensave-open-thumbnail) "assets/icon/open_file_16.png")
(define (opensave-save-thumbnail) "assets/icon/save_file_16.png")
(define (opensave-recent-max-files) 10)
(define (opensave-trim-dst-num-jumps) 6)

(define (printing-use-last-device) 0)
(define (printing-disable-bg) 0)

; Selection Settings
(define (selection-mode-pickfirst) 0)
(define (selection-mode-pickadd) 0)
(define (selection-mode-pickdrag) 0)
(define (selection-grip-size) 16)
(define (selection-pickbox-size) 16)

(define (grip-size) 16)
(define (pick-box-size) 40)
(define (crosshair-size) 16)
(define (shift-key-pressed-state) 0)
(define (gripping-active) 0)
(define (rapid-move-active) 0)
(define (previewActive) 0)
(define (pastingActive) 0)
(define (movingActive) 0)
(define (selectingActive) 0)
(define (zoomWindowActive) 0)

; Panning Settings
(define (panning-real-time-active) 0)
(define (panning-point-active) 0)
(define (panning-active) 0)

(define (display-crosshair-color) "FFFFFF")
(define (display-bg-color) "FFFFFF")
(define (display-selectbox-left-color) "FFFFFF")
(define (display-selectbox-left-fill) "FFFFFF")
(define (display-selectbox-right-color) "FFFFFF")
(define (display-selectbox-right-fill) "FFFFFF")
(define (display-units) "Scientific")

(define (shortcuts)
  (vector
    (vector "new_file" "Ctrl+N")
    (vector "open_file" "Ctrl+O")
    (vector "save_file" "Ctrl+S")
    (vector "save_file_as" "Ctrl+Shift+S")
    (vector "print" "Ctrl+P")
    (vector "design_details" "Ctrl+D")
    (vector "exit" "Ctrl+Q")
    (vector "cut" "Ctrl+X")
    (vector "copy" "Ctrl+C")
    (vector "paste" "Ctrl+V")
    (vector "undo" "Ctrl+Z")
    (vector "redo" "Ctrl+Shift+Z")
    (vector "help" "F1")
    (vector "about" "F2")
    (vector "fullscreen" "F11")))

(define (tips)
  (vector
    "we need more tips?"
    "you can change the color of the display through settings?"
    "you can hide the scrollbars to increase the viewable area through settings?"
    "you can change the icon size for increased visibility?"
    "you can toggle the grid on and off by pressing the button in the statusbar?"
    "the grid size can be changed to match your hoop size through settings?"
    "the crosshair size is based on a percentage of your screen size? Setting it to 100 may help you visually line things up better."
    "you can pan by pressing the middle mouse button and dragging your mouse across the screen?"
    "you can open and edit multiple designs simultaneously?"
    "that many embroidery machines support the .dst format?"
    "that you can zoom in and out using your mouse wheel?"
    "that you can use circular and isometric grids?"
    "about our command line format converter?"
    "that you can use the 'DAY' and 'NIGHT' commands to quickly switch the  view colors to commonly used white or black?"
    "that you can quickly change the background, crosshair and grid colors using the 'RGB' command?"
    "END"))

(define (welcome-message)
  (vector
    " _____________________________________________________________________________"
    "|                                                                             |"
    "|                         EMBROIDERMODDER 2.0.0-alpha                         |"
    "|                         http://embroidermodder.org                          |"
    "|_____________________________________________________________________________|"
    "Usage: embroidermodder [options] files ..."
    ""
    "Options:"
    "-d, --debug      Print lots of debugging information."
    "-h, --help       Print self message and exit."
    "-v, --version    Print the version number of embroidermodder and exit."))

; In the order (icons, labels, value)
(define (thread_weights)
  (vector
    (vector "assets/icons/lineweightbylayer.png" "ByLayer" -2.0)
    (vector "assets/icons/lineweightbyblock.png" "ByBlock" -1.0)
    (vector "assets/icons/lineweightdefault.png" "Default" 0.0)
    (vector "assets/icons/lineweight01.png" "0.00 mm" 0.0)
    (vector "assets/icons/lineweight02.png" "0.05 mm" 0.05)
    (vector "assets/icons/lineweight03.png" "0.15 mm" 0.15)
    (vector "assets/icons/lineweight04.png" "0.20 mm" 0.2)
    (vector "assets/icons/lineweight05.png" "0.25 mm" 0.25)
    (vector "assets/icons/lineweight06.png" "0.30 mm" 0.3)
    (vector "assets/icons/lineweight07.png" "0.35 mm" 0.35)
    (vector "assets/icons/lineweight08.png" "0.40 mm" 0.4)
    (vector "assets/icons/lineweight09.png" "0.45 mm" 0.45)
    (vector "assets/icons/lineweight10.png" "0.50 mm" 0.5)
    (vector "assets/icons/lineweight11.png" "0.55 mm" 0.55)
    (vector "assets/icons/lineweight12.png" "0.60 mm" 0.6)
    (vector "assets/icons/lineweight13.png" "0.65 mm" 0.65)
    (vector "assets/icons/lineweight14.png" "0.70 mm" 0.7)
    (vector "assets/icons/lineweight15.png" "0.75 mm" 0.75)
    (vector "assets/icons/lineweight16.png" "0.80 mm" 0.8)
    (vector "assets/icons/lineweight17.png" "0.85 mm" 0.85)
    (vector "assets/icons/lineweight18.png" "0.90 mm" 0.9)
    (vector "assets/icons/lineweight19.png" "0.95 mm" 0.95)
    (vector "assets/icons/lineweight20.png" "1.00 mm" 1.0)
    (vector "assets/icons/lineweight21.png" "1.05 mm" 1.05)
    (vector "assets/icons/lineweight22.png" "1.10 mm" 1.1)
    (vector "assets/icons/lineweight23.png" "1.15 mm" 1.15)
    (vector "assets/icons/lineweight24.png" "1.20 mm" 1.2)))

; This will parse correctly but may not
; run and could cause crashes.
;
; Use 2 space indentation and stack up trailing parentheses.

(define (usage)
  (debug-message "Usage Message"))

(define (rgb red green blue)
  (+
    blue
    (* 256 green)
    (* 256 256 red)))

; These toggle functions could be made by a macro.
(define (toggle-polar)
   (let*
      (debug-message "StatusBarButton togglePolar()")
      (define (show-polar) (not show-polar))))

(define (toggle-snap)
   (let*
      (debug-message "StatusBarButton toggleQSnap()")
      (define (snap-mode) (not snap-mode))))))

; Strings that don't need to be easily editable.
;
;#define DATA_TYPE_END_MARKER -1
;
;char preview_modes[][100] = {
;    "null",
;    "move",
;    "rotate",
;    "scale"
;);
;
;char comboboxes[][100] = {
;    "ARC-CLOCKWISE",
;    "GENERAL-LAYER",
;    "GENERAL-COLOR",
;    "GENERAL-LINE-TYPE",
;    "GENERAL-LINE-WEIGHT",
;    "TEXT-SINGLE-FONT",
;    "TEXT-SINGLE-JUSTIFY"
;);
;
;Property circle_geometry_properties[] = {
;    {"Circle", "system" DATA_TYPE_TITLE),
;    {"Geometry", "system" DATA_TYPE_GROUP_BOX),
;    {"Center X", "user" DATA_TYPE_DOUBLE),
;    {"Center Y", "user" DATA_TYPE_DOUBLE),
;    {"Radius", "user" DATA_TYPE_DOUBLE),
;    {"Diameter", "system" DATA_TYPE_DOUBLE),
;    {"Area", "system" DATA_TYPE_DOUBLE),
;    {"Circumference", "system" DATA_TYPE_DOUBLE),
;    {"END", "system" DATA_TYPE_END_MARKER)
;);
;
;Property ellipse_properties[] = {
;    {"Ellipse", "system" DATA_TYPE_TITLE),
;    {"Geometry", "system" DATA_TYPE_GROUP_BOX),
;    {"Center X", "user" DATA_TYPE_DOUBLE),
;    {"Center Y", "user" DATA_TYPE_DOUBLE),
;    {"Radius Major", "user" DATA_TYPE_DOUBLE),
;    {"Radius Minor", "user" DATA_TYPE_DOUBLE),
;    {"Diameter Major", "user" DATA_TYPE_DOUBLE),
;    {"Diameter Minor", "user" DATA_TYPE_DOUBLE),
;    {"END", "system" DATA_TYPE_END_MARKER)
;);
;
;Property image_properties[] = {
; (vector
;    {"Image", "system" DATA_TYPE_TITLE),
;    {"Geometry", "system" DATA_TYPE_GROUP_BOX),
;    {"Position X", "user" DATA_TYPE_DOUBLE),
;    {"Position Y", "user" DATA_TYPE_DOUBLE),
;    {"Width", "user" DATA_TYPE_DOUBLE),
;    {"Height", "user" DATA_TYPE_DOUBLE),
;    {"Misc", "system" DATA_TYPE_GROUP_BOX),
;    {"Name", "user" DATA_TYPE_STRING),
;    {"Path", "user" DATA_TYPE_STRING),
;    {"END", "system" DATA_TYPE_END_MARKER)
;))
;
;Property infinite_line_properties[] = {
;    {"Infinite Line", "system" DATA_TYPE_TITLE),
;    {"Geometry", "system" DATA_TYPE_GROUP_BOX),
;    {"Position X1", "user" DATA_TYPE_DOUBLE),
;    {"Position Y1", "user" DATA_TYPE_DOUBLE),
;    {"Position X2", "user" DATA_TYPE_DOUBLE),
;    {"Position Y2", "user" DATA_TYPE_DOUBLE),
;    {"Vector X", "system" DATA_TYPE_DOUBLE),
;    {"Vector Y", "system" DATA_TYPE_DOUBLE),
;    {"END", "system" DATA_TYPE_END_MARKER)
;);
;
;/*
;Arc
;Geometry
;Center X,double,user
;Center Y,double,user
;Radius,double,user
;Start Angle,double,user
;End Angle,double,user
;Start X,double,system
;Start Y,double,system
;End X,double,system
;End Y,double,system
;Area,double,system
;Length,double,system
;Chord,double,system
;Included Angle,double,system
;END,END,END
;*/
;
;char *settings_tabs[MAX_STRING_LENGTH] = {
;    "General",
;    "Files/Path",
;    "Display",
;    "Open/Save",
;    "Printing",
;    "Snap",
;    "Grid/Ruler",
;    "Ortho/Polar",
;    "QuickSnap",
;    "QuickTrack",
;    "LineWeight",
;    "Selection",
;    "END"
;);
;
;/* "Rect"
; * "GroupBox" "Geometry"
; */
;char *geometry_rect_properties[MAX_STRING_LENGTH] = {
;    "Corner X1,double,user",
;    "Corner Y1,double,user",
;    "Corner X2,double,user",
;    "Corner Y2,double,user",
;    "Corner X3,double,user",
;    "Corner Y3,double,user",
;    "Corner X4,double,user",
;    "Corner Y4,double,user",
;    "Height,double,user",
;    "Width,double,user",
;    "Area,double,system",
;    "END"
;);
;
;char *arc_properties[MAX_STRING_LENGTH] = {
;    "Center X,double,user",
;    "Center Y,double,user",
;    "Radius,double,user",
;    "Start Angle,double,user",
;    "End Angle,double,user",
;    "Start X,double,system",
;    "Start Y,double,system",
;    "End X,double,system",
;    "End Y,double,system",
;    "Area,double,system",
;    "Length,double,system",
;    "Chord,double,system",
;    "Included Angle,double,system",
;    "END"
;);
;
;char *details_labels[MAX_STRING_LENGTH] = {
;    "Total Stitches:",
;    "Real Stitches:",
;    "Jump Stitches:",
;    "Trim Stitches:",
;    "Total Colors:",
;    "Color Changes:",
;    "Left:",
;    "Top:",
;    "Right:",
;    "Bottom:",
;    "Width:",
;    "Height:",
;    "END"
;);
;
;char *statusbar_labels[MAX_STRING_LENGTH] = {
;    "SNAP",
;    "GRID",
;    "RULER",
;    "ORTHO",
;    "POLAR",
;    "QSNAP",
;    "QTRACK",
;    "LWT",
;    "END"
;);
;
;(define (obj-names)
;  (vector
;    "Null"
;    "Unknown"
;    "Base"
;    "Arc"
;    "Block"
;    "Circle"
;    "Aligned Dimension"
;    "Angular Dimension"
;    "Arc Length Dimension"
;    "Diameter Dimension"
;    "Leader Dimension"
;    "Linear Dimension"
;    "Ordinate Dimension"
;    "Radius Dimension"
;    "Ellipse"
;    "Elliptical Arc"
;    "Rubber"
;    "Grid"
;    "Hatch"
;    "Image"
;    "Infinite Line"
;    "Line"
;    "Path"
;    "Point"
;    "Polygon"
;    "Polyline"
;    "Ray"
;    "Rectangle"
;    "Slot"
;    "Spline"
;    "Multi Line Text"
;    "Single Line Text"
;    "Unknown"
;    "END"
;  )
;)
;
;(define (text-single-properties)
;  (vector
;    "Text"
;    "Text Single"
;    "Contents" "string" "user"
;    "Font" "dropdown" "user"
;    "Justify" "dropdown" "user"
;    "Height" "double" "system"
;    "Rotation" "double" "system"
;    "Backward" "int" "user"
;    "Upside Down" "int" "user"
;    "Position X" "double" "system"
;    "Position Y" "double" "system"
;    "END" "END" "END"
;  )
;)

(define (block-properties)
  (vector
    "Block"
    "Block"
    "Position X" "double" "system"
    "Position Y" "double" "system"
    "END" "END" "END"))

(define (line-properties)
  (vector
    "Line"
    "Line"
    "Start X" "double" "user"
    "Start Y" "double" "user"
    "End X" "double" "user"
    "End Y" "double" "user"
    "Delta X" "double" "system"
    "Delta Y" "double" "system"
    "Angle" "double" "system"
    "Length" "double" "system"
    "END" "END" "END"))

(define (polygon-properties)
  (vector
    "Polygon"
    "Polygon"
    "Center X" "double" "user"
    "Center Y" "double" "user"
    "Radius Vertex" "double" "system"
    "Radius Side" "double" "system"
    "Diameter Vertex" "double" "system"
    "Diameter Side" "double" "system"
    "Interior Angle" "double" "system"
    "END" "END" "END"))

(define (point-properties)
  (vector
    "Point"
    "Point"
    "Point X" "double" "user"
    "Point Y" "double" "user"
    "END" "END" "END"))

(define (general-general-properties)
  (vector
    "General"
    "General"
    "Layer" "dropdown" "user"
    "Color" "dropdown" "user"
    "Line Type" "dropdown" "user"
    "Line Weight" "dropdown" "user"
    "END" "END" "END"))

(define (misc-arc-properties)
  (vector
    "Misc"
    "Arc"
    "Clockwise" "int" "system"
    "END" "END" "END"
  )
)

(define (misc-image-properties)
  (vector
    "Misc"
    "Image"
    "Name" "string" "system"
    "Path" "string" "system"
    "END" "END" "END"
  )
)

(define (misc-path-properties)
  (vector
    "Misc"
    "Path"
    "Closed" "int" "user"
    "END" "END" "END"
  )
)

(define (misc-polyline-properties)
  (vector
    "Misc"
    "Polyline"
    "Closed" "int" "user"
    "END" "END" "END"
  )
)

(define (misc-text-single-properties)
  (vector
    "Misc"
    "Text Single"
    "Backward" "int" "user"
    "Upside down" "int" "user"
    "END" "END" "END"
  )
)

(define (geometry-circle-properties)
  (vector
    "Geometry"
    "Circle"
    "Center X" "double" "user"
    "Center Y" "double" "user"
    "Radius" "double" "user"
    "Diameter" "double" "user"
    "Area" "double" "user"
    "Circumference" "double" "user"
    "END" "END" "END"
  )
)

(define (geometry-image-properties)
  (vector
    "Geometry"
    "Image"
    "Position X" "double" "user"
    "Position Y" "double" "user"
    "Width" "double" "user"
    "Height" "double" "user"
    "END" "END" "END"
  )
)

(define (geometry-infinite-line-properties)
  (vector
    "Geometry"
    "Infinite Line"
    "Start X" "double" "user"
    "Start Y" "double" "user"
    "2nd X" "double" "user"
    "2nd Y" "double" "user"
    "Vector X" "double" "system"
    "Vector Y" "double" "system"
    "END" "END" "END"
  )
)

(define (GroupBox10)
  (vector
    "Geometry"
    "Line"
    "Start X" "double" "user"
    "Start Y" "double" "user"
    "End X" "double" "user"
    "End Y""double" "user"
    "Delta X" "double" "system"
    "Delta Y" "double" "system"
    "Angle X" "double" "system"
    "Length Y""double" "system"
    "END" "END" "END"
  )
)

(define (GroupBox9)
  (vector
    "Geometry"
    "Path"
    "Vertex #" "int" "user"
    "Vertex X" "double" "user"
    "Vertex Y" "double" "user"
    "Area" "double" "system"
    "Length" "double" "system"
    "END" "END" "END"
  )
)

(define (GroupBox8)
  (vector
    "Misc"
    "Path"
    "Closed" "int" "user"
    "END" "END" "END"
  )
)

(define (geometry-polygon-properties)
  (vector
    "Geometry"
    "Polygon"
    "Center X" "double" "user"
    "Center Y" "double" "user"
    "Vertex Radius" "double" "user"
    "Side Radius" "double" "user"
    "Vertex Diameter" "double" "user"
    "Side Diameter" "double" "user"
    "Interior Angle" "double" "system"
    "END" "END" "END"
  )
)

(define (geometry-polyline-properties)
  (vector
    "Geometry"
    "Polyline"
    "Vertex #" "int" "user"
    "Vertex X" "double" "user"
    "Vertex Y" "double" "user"
    "Area" "double" "system"
    "Length" "double" "system"
    "END" "END" "END"
  )
)

(define (geometry-ray-properties)
  (vector
    "Geometry"
    "Ray"
    "Start X" "double" "user"
    "Start Y" "double" "user"
    "2nd X" "double" "user"
    "2nd Y" "double" "user"
    "Vector X" "double" "system"
    "Vector Y" "double" "system"
    "END" "END" "END"
  )
)

(define (geometry-text-multi-properties)
  (vector
    "Geometry"
    "Text Multi"
    "Position X" "double" "user"
    "Position Y" "double" "user"
    "END" "END" "END"
  )
)

;(define GroupBox3[] =
;  (vector
;    "Text"
;    "Text Single"
;    "Contents" "string" "user"
;    "Font" "dropdown" "user"
;    "Justify" "dropdown" "user"
;    "Height" "double" "user"
;    "Rotation" "double" "user"
;    "END" "END" "END"
;  )
;)

;(define GroupBox2[] =
;  (vector
;    "Geometry"
;    "Text Single"
;    "Position X" "double" "user"
;    "Position Y" "double" "user"
;    "END" "END" "END"
;  )
;)

;(define GroupBox1[] =
;  (vector
;    "Misc"
;    "Text Single"
;    "Backward" "int" "user"
;    "Upside Down" "int" "user"
;    "END" "END" "END"
;  )
;)

;(define General1[] =
;  (vector
;    "Layer (Toolbutton" " combobox)"
;    "Color (toolbutton" " combobox)"
;    "LineType (toolbutton" " combobox)"
;    "LineWeight (toolbutton" " combobox)"
;    "END" "END" "END"
;  )
;)

;(define Path[] =
;  (vector
;    "VertexNum (toolbutton" " combobox)"
;    "VertexX (toolbutton" " lineedit)"
;    "VertexY (toolbutton" " lineedit)"
;    "Area (toolbutton" " lineedit)"
;    "Length (toolbutton" " lineedit)"
;    "Closed (toolbutton" " combobox)"
;    "END" "END" "END"
;  )
;)

;(define (Polyline)
;  (vector
;    "Polyline"
;    "VertexNum (toolbutton" " combobox)"
;    "VertexX (toolbutton" " lineedit)"
;    "VertexY (toolbutton" " lineedit)"
;    "Area (toolbutton" " lineedit)"
;    "Length (toolbutton" " lineedit)"
;    "Closed (toolbutton" " combobox)"
;    "END" "END" "END"
;  )
;)

;(define Ray)
;    "X1 (toolbutton" " lineedit)"
;    "Y1 (toolbutton" " lineedit)"
;    "X2 (toolbutton" " lineedit)"
;    "Y2 (toolbutton" " lineedit)"
;    "VectorX (toolbutton" " lineedit)"
;    "VectorY (toolbutton" " lineedit)"
;    "END" "END" "END"
;  )
;)

;(define TextMulti)
;    "X (toolbutton" " lineedit)"
;    "Y (toolbutton" " lineedit)"
;    "END" "END" "END"
;  )
;)

;char error-msg[MAX-STRING-LENGTH];
;char uname-string[MAX-STRING-LENGTH];
;char current-file-name[MAX-STRING-LENGTH];
;char *arc-properties[MAX-STRING-LENGTH];
;char *details-labels[MAX-STRING-LENGTH];
;char *statusbar-labels[MAX-STRING-LENGTH];
(define (preview-active) 0)
(define (moving-active) 0)
(define (panning-active) 0)
(define (rapid-move-active) 0)
;(define (pasting-active = 0;
;(define (selecting-active = 0;
;(define (zoom-window-active = 0;
;(define (gripping-active = 0;
;int general-tip-of-the-day = 0;
;int ruler-metric = 1;
;int display-use-open-gl = 0;
;int display-renderhint-aa = 0;
;int display-renderhint-text-aa = 0;
;int display-renderhint-smooth-pix = 0;
;int display-renderhint-high-aa = 0;
;int display-renderhint-noncosmetic = 0;
;int display-show-scrollbars = 1;
;int display-scrollbar-widget-num = 0;
;float display-zoom-scale-action-in = 0.0;
;float display-zoom-scale-action-out = 0.0;
;unsigned char display-selectbox-alpha = 0;
;unsigned char display-crosshair-percent = 0;
;double zoom-in-limit = 0.00001;
;double zoom-out-limit = 10000.0;
;EmbVector grid-center;
;EmbVector grid-size;
;EmbVector grid-spacing;
;float grid-size-radius;
;float grid-spacing-radius;
;float grid-spacing-angle;
;int ruler-show-on-load;
;int general-system-help-browser;
;int general-check-for-updates;
;int opensave-open-thumbnail;
;int opensave-save-thumbnail;
;unsigned char opensave-recent-max-files;
;unsigned char opensave-trim-dst-num-jumps;
;int printing-use-last-device;
;int printing-disable-bg;
;int grid-show-on-load;
;int grid-show-origin;
;int grid-color-match-crosshair;
;int grid-load-from-file;
;int qsnap-enabled;
;int qSnapToggle;?
;char general-language[MAX-STRING-LENGTH];
;char general-icon-theme[MAX-STRING-LENGTH];
;char general-mdi-bg-logo[MAX-STRING-LENGTH];
;char general-mdi-bg-texture[MAX-STRING-LENGTH];
;unsigned int general-mdi-bg-color;
;unsigned short general-current-tip;
;unsigned int display-crosshair-color;
;unsigned int display-bg-color;
;unsigned int display-selectbox-left-color;
;unsigned int display-selectbox-left-fill;
;unsigned int display-selectbox-right-color;
;unsigned int display-selectbox-right-fill;
;char display-units[MAX-STRING-LENGTH];
;char opensave-open-format[MAX-STRING-LENGTH];
;char opensave-save-format[MAX-STRING-LENGTH];
;char opensave-recent-directory[MAX-STRING-LENGTH];
;char printing-default-device[MAX-STRING-LENGTH];
;unsigned int grid-color;
;unsigned int ruler-color;
;unsigned int qsnap-locator-color;
;char grid-type[MAX-STRING-LENGTH];
;unsigned int selection-coolgrip-color;
;unsigned int selection-hotgrip-color;
;unsigned char qsnap-locator-size;
;unsigned char qsnap-aperture-size;
;int lwt-show-lwt;
;(define (lwt-real-render) 1)
;(define (lwt-default-lwt) 1.0)
;int selection-mode-pickfirst;
;int selection-mode-pickadd;
;int selection-mode-pickdrag;
;unsigned char selection-grip-size;
;unsigned char selection-pickbox-size;
;text-properties text-style;
(define (ruler-pixel-size) 16)
(define (grip-size) 16)
(define (pick-box-size) 16)
(define (crosshair-size) 16)

;int qsnap-mode[20];
;int state[20];
;int toggle-status[20];
;char prefix[100];
;int shift-key-pressed-state;

(define (running) 1)
(define (testing) 0)

; Should be a ring buffer.
(define (undo-history) (vector ""))
(define (undo-history-position) 0)
(define (undo-history-max) 500)

(define (text-cursor) (vector 0 0))
(define (text-display) (vector ""))

;/* Variable data */
;char obj_names[MAX_OBJECTS][MAX_STRING_LENGTH];
;char details_label_text[12][MAX_STRING_LENGTH];
;
;int dialog_setting_int[100];
;double dialog_setting_double[100];
;
;/*
;char text_font[MAX_STRING_LENGTH];
;preview = settings.copy();
;dialog = settings.copy();
;accept = settings.copy();
;char opensave_custom_filter[MAX_STRING_LENGTH]; */
;
;char toolbar_entries[MAX_TOOLBARS][MAX_TOOLBAR_ENTRIES][MAX_STRING_LENGTH];
;char menu_entries[MAX_TOOLBARS][MAX_TOOLBAR_ENTRIES][MAX_STRING_LENGTH];
;char statusbar_label[MAX_MENU_LENGTH][MAX_STRING_LENGTH];

; Settings Editor
; ---------------
;
;Setting
;grid_ruler_misc_settings[] = {;
;    ;{
;        "Initially show grid when loading a file",
;        INT_GRID_SHOW_ON_LOAD,
;        "int",
;        0,
;        1,
;        0,
;        0,
;        ALIGN_LEFT
 ;   ),
  ;  {
;        "Show the origin when the grid is enabled",
;        INT_GRID_SHOW_ORIGIN,
;        "int",
;        0,
;        1,
;        1,
;        0,
;        ALIGN_LEFT
;    ),
;    {
;        "END",
;        -1,
;        -1,
;        -1,
;        -1,
;        -1,
;        -1,
;        -1
;    )
;);
;
;Setting
;grid_ruler_color_settings[] = {
;    {
;        "Match grid color to crosshair color",
;        INT_GRID_COLOR_MATCH_CROSSHAIR,
;        "int",
;        0,
;        1,
;        0,
;        0,
;        ALIGN_LEFT
;    ),
 ;   {
    ;    "END",
;        -1,
;        -1,
;        -1,
;        -1,
;        -1,
;        -1,
;        -1
;    )
;)
;
;(define (grid_ruler_boxes)
;SettingBox
;grid_ruler_boxes[] = {
;    (vector
;          "Grid Misc",
;        grid_ruler_misc_settings
;    ),
;    (vector
;        "Grid Color",
;        grid_ruler_color_settings
;    )
;);
;
;(define 
;SettingsTab grid_ruler_settings = {
;    "Grid/Ruler",
;    2,
;    grid_ruler_boxes
;);
; */

; ------------------------------------------------------------
; Toolbars
(define (file-toolbar)
  (vector
    "new"
    "open"
    "save"
    "save_as"
    "print"
    "design_details"
    "help"
    "END"))

(define (edit-toolbar)
  (vector
    "undo"
    "redo"
    "cut"
    "copy"
    "paste"
    "END"))

(define (view-toolbar)
  (vector
    "day"
    "night"
    "END"))

(define (zoom-toolbar)
  (vector
    "zoom_window"
    "zoom_dynamic"
    "zoom_scale"
    "zoom_center"
    "zoom_in"
    "zoom_out"
    "zoom_selected"
    "zoom_all"
    "zoom_extents"
    "END"))

(define (pan-toolbar)
  (vector
    "pan_real_time"
    "pan_point"
    "pan_left"
    "pan_right"
    "pan_up"
    "pan_down"
    "END"))

(define (icon-toolbar)
  (vector
    "icon16"
    "icon24"
    "icon32"
    "icon48"
    "icon64"
    "icon128"
    "END"))

(define (help-toolbar)
  (vector
    "help"
    "changelog"
    "about"
    "whats_this"
    "END"))

(define (layer-toolbar)
  (vector
    "END"))

(define (text-toolbar)
  (vector
    "END"))

(define (propertes-toolbar)
  (vector
    "END"))

(define (other-toolbar)
  (vector
    "END"))


