/* 
 *  Embroidermodder 2.
 *
 *  ------------------------------------------------------------
 *
 *  Copyright 2013-2022 The Embroidermodder Team
 *  Embroidermodder 2 is Open Source Software.
 *  See LICENSE for licensing terms.
 *
 *  ------------------------------------------------------------
 *
 *  Unsorted.
 */

#include "em2.h"

const char *symbols_docstring =
    "Symbols use the SVG path syntax.\n" \
    "\n" \
    "In theory, we could combine the icons and symbols systems,\n" \
    "since they could be rendered once and stored as icons in Qt.\n" \
    "(Or as textures in FreeGLUT.)\n" \
    "\n" \
    "Also we want to render the patterns themselves using SVG\n" \
    "syntax, so it would save on repeated work overall.\n";

#if 0
keys:
    obj-type=int=See OBJ-TYPE-VALUES
    obj-name=str=\USER\, \DEFINED\, \STRINGS\, etc...
    OBJ-LAYER=value type {int=0-255
    OBJ-COLOR=TODO=Use color chart in formats/format-dxf.h for this
    OBJ-LTYPE=int=See OBJ-LTYPE-VALUES
    OBJ-LWT=int=[-2, 27]
    OBJ-RUBBER=int=See OBJ-RUBBER-VALUES

origin-string=|
    M 0.0 0.5
    A -0.5 -0.5 1.0 1.0 90.0 360.0
    A -0.5 -0.5 1.0 1.0 90.0 -360.0
    L 0.0 -0.5
    A -0.5 -0.5 1.0 1.0 270.0 90.0
    L -0.5 0.0
    A -0.5 -0.5 1.0 1.0 180.0 -90.0
    Z
tosort:
    to-translate=|
        path-symbol icon-two[] = {
            {PATHS-MOVETO, 0 -0.75
            A {0.45, 1.00, 0.50, 180.00, -216.87
            L 0 0.0
            L {0.50, 0.0
        ]
        
        path-symbol icon-three[] = {
            {PATHS-ARCMOVETO, 0 -0.50, 0.50, 0.50, 195.00
            A 0 -0.50, 0.50, 195.00, 255.00
            A 0 -0.50, 0.50, 270.00, 255.00
        ]
        
        path-symbol icon-five[] = {
            M 50 0 L 0 0 L 0 50 L 25 50 A 0.0, -0.5 0.5 0.5 90.0 -180.0 L 0 0
        ]
        
        path-symbol icon-six[] = {
            path.addEllipse(Vector(x+0.25*xs, y-0.25*ys), 0.25*xs, 0.25*ys)
            M 0 75 L 0 25
            path.arcTo(x+0.00*xs, y-1.00*ys, 0.50*xs, 0.50*ys, 180.00, -140.00)
        ]
        
        path-symbol icon-eight[] = {
            path.add-ellipse(Vector(x+0.25*xs, y-0.25*ys), 0.25*xs, 0.25*ys)
            path.add-ellipse(Vector(x+0.25*xs, y-0.75*ys), 0.25*xs, 0.25*ys)
        ]
        
        path-symbol icon-nine[] = {
            path.add-ellipse(Vector(x+0.25*xs, y-0.75*ys), 0.25*xs, 0.25*ys)
            M 0.50*xs, y-0.75*ys)
            L x+0.50*xs, y-0.25*ys)
            path.arcTo(x+0.00*xs, y-0.50*ys, 0.50*xs, 0.50*ys, 0.00, -140.00)
        ]

settings[to-add-to-property-editor] = 
    
    toolbar = ToolBar[10]
    menu = Menu[10]
    status-bar = toolButton = [
        tk.Button() for i in range(8)
    ]
    toolButton = [
        tk.Button() for i in range(PROPERTY-EDITORS)
    ]
    lineEdit = [
        tk.LineEdit() for i in range(LINEEDIT-PROPERTY-EDITORS)
    ]
    comboBox = [
        tk.ComboBox() for i in range(COMBOBOX-PROPERTY-EDITORS)
    ]

    opensave-recent-list-of-files = []
    opensave-custom-filter = 

    toolButtonTextSingleHeight =
    toolButtonTextSingleRotation = tk.Button()

    text-single-editors = {
        contents={
            entry=tk.LineEdit(),
            toolbutton=tk.Button()
    },
        font=[tk.FontComboBox(), tk.Button()],
        justify=[tk.ComboBox(), tk.Button()],
        height=[tk.LineEdit(), tk.Button()],
        rotation=[tk.LineEdit(), tk.Button()]
    }

    EmbVector paste_delta;
    EmbVector scene_press_point;
    Point press_point;
    EmbVector scene_move_point;
    Point move_point;
    EmbVector scene_release_point;
    Point releasePoint;
    EmbVector sceneGripPoint;

    Color ruler_color;

    Point  viewMousePoint;
    EmbVector sceneMousePoint;
    unsigned int snapLocatorColor;
    unsigned int gripColorCool;
    unsigned int gripColorHot;
    unsigned int crosshairColor;
    int precisionAngle;
    int precisionLength;

    Label status-bar-mouse-coord;

 Used when checking if fields vary
    field-old-text = ;
    field-new-text = ;
    field-varies-text = ;
    field-yes-text = ;
    field-no-text = ;
    field-on-text = ;
    fieldOffText = ;

    ToolButton tool-button-arc-clockwise
    ComboBox combobox-arc-clockwise;

    groupbox groupboxGeometry[32];
    groupbox groupboxGeneral;
    groupbox groupboxMiscArc;
    groupbox groupboxMiscPath;
    groupbox groupboxMiscPolyline;
    groupbox groupboxTextTextSingle;
    groupbox groupboxMiscTextSingle;

#endif

/*
Help {
    Help
    Opens the packaged help.

    Changelog
    Opens a log of what has recently changed.

    About
    Opens the about this software dialog.

    What's This?
    Details of the current design.
}
*/

void
draw_background(void)
/* int scene, int painter, EmbRect rect) */
{
    /*
    painter.fill-rect(rect, backgroundBrush());

    # HACK a = rect.intersects(grid-path.controlPointRect()
    a = 1;
    if (self.gscene.property(enable-grid) and a) {
        grid-pen = Pen (grid-color);
        grid-pen.set_join-style("MiterJoin");
        grid-pen.set_cosmetic(1);
        painter.set_pen(grid-pen);
        painter.draw-path(grid-path);
        painter.draw-path(self.origin-path);
        painter.fill-path(self.origin-path, grid-color);
    }
    */
}

void
draw_crosshair(void)
{
    /* painter.setBrush(Qt-NoBrush); */
    /* crosshairPen = QPen (Color-fromRgb(crosshair-color))
    crosshairPen.set_cosmetic(1)
    painter.set_pen(crosshairPen)

    start = map-to-scene(view-mouse-point.x(), view-mouse-point.y()-settings.crosshair-size)
    end = map-to-scene(view-mouse-point.x(), view-mouse-point.y()+settings.crosshair-size)
    painter.draw-line(Line(start, end))

    start = map-to-scene(view-mouse-point.x()-settings.crosshair-size, view-mouse-point.y())
    end = map-to-scene(view-mouse-point.x()+settings.crosshair-size, view-mouse-point.y())
    painter.drawLine(Line(start, end))

    top-left = map-to-scene(view-mouse-point.x()-settings.selection-pickbox-size,
        view-mouse-point.y()-settings.selection-pickbox-size)
    bottom-right = map-to-scene(view-mouse-point.x()+settings.selection-pickbox-size,
        view-mouse-point.y()+settings.selection-pickbox-size)
    painter.drawRect(Rect(top-left, bottom-right))

    p = QPixmap-grabWindow(winId());
    p.save(QString("test.bmp"), "bmp"); */
}

/* TODO: and findClosestSnapPoint == 1 */
void
draw_closest_qsnap(int view)
{
    printf("%d\n", view);
    /*
    qsnap-pen = Pen(Color(qsnapLocator-color));
    qsnap-pen.set_width(2);
    qsnap-pen.set_join-style(Qt-MiterJoin);
    qsnap-pen.set_cosmetic(1);
    painter.set_pen(qsnap-pen);
    qsnap-offset = Vector(qsnap-locator-size, qsnap-locator-size"]);

    apertureSnapPoints = [];
    apertureitem-list = items(
        view-mouse-point.x()-qsnap-aperture-size,
        view-mouse-point.y()-qsnap-aperture-size,
        qsnap-aperture-size"]*2,
        qsnap-aperture-size"]*2);
    for (item in apertureitem-list) {
        if (item.type != "Unknown") {
            tempitem-obj = item
            if (tempitem-obj) {
                aperture-snap-points += [ tempitem-obj.mouseSnapPoint(scene-mouse-point) ]
            }
        }
    }
    */

    /* TODO: Check for intersection snap points and add them to the list. */ /*
    for (asp in aperture-snap-points) {
        p1 = map_from_scene(asp) - qsnap_offset;
        q1 = map_from_scene(asp) + qsnap_offset;
        painter.drawRect(Rect(map-to-scene(p1), map-to-scene(q1)));
    }
    */
}

#if 0
/* Draw grip points for all selected objects. */
void
draw_foreground(int view, int painter EmbRect rect)
{
    grip-pen = Pen(rgb=grip-color-cool);
    grip-pen.set_width(2);
    grip-pen.set_join-style("MiterJoin");
    grip-pen.set_cosmetic(1);
    painter.set_pen(grip-pen);
    grip-offset = Vector(
        selection-grip-size, selection-grip-size"]
    );

    selected-grip-points = [];
    selecteditem-list = self.gscene.selected-items()
    if (selecteditem-list.size() <= 100) {
        for (item in selecteditem-list) {
            if (item.type != "Unknown") {
                tempBase-obj = item
                if (tempBase-obj) {
                    selected-grip-points = tempBase-obj.all-grip-points();
                }

                for (ssp in selected-grip-points) {
                    p1 = map-from-scene(ssp) - grip-offset;
                    q1 = map-from-scene(ssp) + grip-offset;
                    rect = Rect(map-to-scene(p1), map-to-scene(q1));

                    if (ssp == sceneGripPoint) {
                        painter.fill-rect(rect, rgb=grip-color-hot);
                    }
                    else {
                        painter.draw-rect(rect);
                    }
                }
            }
        }
    }

    if (!mainwnd->selecting_active) {
        draw_closest_qsnap(self);
        draw_crosshair(self);
    }

    if (mainwnd->show_ruler) {
        draw_rulers();
    }
}

/* Should create a label widget under the new system. */
void
create_ruler_text_path(EmbVector position, char *str, double height)
{
    /*
    int i;
    path = Path()

    x-scale = height
    y-scale = height
    pos = Vector(x, y)
    scale = Vector(0.01*height, 0.01*height)

    for (i=0 i<strlen(str) i++) {
        
        pos[0] = x
    }
    return path;
    */
}

void
create_grid(scheme *sc, int gridType)
{
    EmbPath grid-path;
    int enable_grid = 1;
    switch (gridType) {
    case RECTANGULAR_GRID:
        createGridrect();
        break;
    case CIRCULAR_GRID:
        createGridpolar();
        break;
    case ISOMETRIC_GRID:
        createGridiso();
        break;
    default:
        break;
    }
    create-origin();
    
    /* EXPERIMENT
     * Tagging experiments with the path system
     * to the origin.
     */
    EmbVector origin_position, origin_scale;
    origin_position.x = 10.0;
    origin_position.y = 0.0;
    origin_scale.x = 1.0;
    origin_scale.y = 1.0;
    /* self.origin-path.add-list-to-path(origin-string, position, scale); */
    update_current_scene();
}

/* TODO: Make Origin Customizable. */
void
create_origin(scheme *sc)
{
    /* self.origin-path = Path(); */ /*

    if ((sc, "grid-show-origin")) { */
        /* self.origin-path.addEllipse(Vector(0,0), 0.5, 0.5) */
        /* TODO: Make Origin Customizable. */ /*
        position = [0.0, 0.0];
        scale = [1.0, 1.0];
        self.origin-path.add-list-to-path(origin-string, position, scale)
    } */
}

void
createGridrect(scheme *sc)
{
    EmbVector point1, point2;
    /*
    y_spacing = grid_spacing.y;

    gr = Rect(0, 0, gridsize.x, -gridsize.y); */
    /* Ensure the loop will work correctly with negative numbers. */ /*
    point1.x = min(gr.left(), gr.right());
    point1.y = min(gr.top(), gr.bottom());
    point2.x = max(gr.left(), gr.right());
    point2.y = max(gr.top(), gr.bottom());

    grid-path = Path();
    grid-path.add-rect(gr);
    double gx, gy;
    for (gx=point1.x; gx<point2.x; gx+=grid_spacing.x) {
        for (gx=point1.y; gy<point2.y; gy+=grid_spacing.y) {
            grid-path.move-to(point1-x, gy);
            grid-path.line-to(point2-x, gy);
            grid-path.move-to(gx, point1-y);
            grid-path.line-to(gx, point2-y);
        }
    } */

    /* Center the Grid */ /*
    grid-rect = grid-path.bounding-rect();
    bx = gridrect.width() / 2.0;
    by = -gridrect.height() / 2.0;
    center = Vector(gridcenter.x, -gridcenter.y);
    delta.x = center.x - bx;
    delta.y = center.y - by;

    if (get_int("grid-center-on-origin")) {
        grid-path.translate(-bx, -by);
    }
    else {
        grid-path.translate(delta-x, delta-y);
    }
    */
}

void
createGridpolar(void)
{
    double r, ang;
    /*
    rad-spacing = grid-spacing-radius"]
    ang-spacing = grid-spacing-angle"]

    rad = grid-size-radius"]

    grid-path = Path()
    grid-path.addEllipse(Vector(0,0), rad, rad)
    for (r=0.0 r < rad r+=rad-spacing) {
        grid-path.addEllipse(Vector(0,0), r, r)
    }

    for (ang=0.0 ang<360.0 ang+=ang-spacing) {
        grid-path.move-to(0,0)
        grid-path.line-to(Line-from-polar(rad, ang).p2())
    }

    if (not grid-center-on-origin"]) {
        grid-path.translate(settings.grid-center.x, -settings.grid-center.y)
    }
    */
}

void
createGridiso(void)
{
    /* Ensure the loop will work correctly with negative numbers */
    /* isoW = fabs(gridsize.x);
    isoH = fabs(gridsize.y);

    p1 = Vector(0, 0);
    p2 = Line-from-polar(isoW, 30).p2();
    p3 = Line-from-polar(isoH, 150).p2();
    p4 = p2 + p3;

    grid-path = Path();
    grid-path.moveto(p1);
    grid-path.lineto(p2);
    grid-path.lineto(p4);
    grid-path.lineto(p3);
    grid-path.lineto(p1);

    double x, y;
    for (x=0.0; x<isoW; x+=gridspacing.x) {
        for (y=0.0; y<isoH; y+=gridspacing.y) {
            px = Line-from-polar(x, 30).p2();
            py = Line-from-polar(y, 150).p2();

            grid-path.moveto(px);
            grid-path.lineto(px+p3);
            grid-path.moveto(py);
            grid-path.lineto(py+p2);
        }
    } */

    /* Center the Grid */
    /* gridrect = grid-path.bounding-rect();*/
    /* bx is unused */ /*
    by = -gridrect.height()/2.0;
    cx = gridcenter.x;
    cy = -gridcenter.y;

    if (grid-center-on-origin) {
        grid-path.translate(0, -by);
    }
    else {
        grid-path.translate(0, -by);
        grid-path.translate(cx, cy);
    }
    */
}

/*
 * NOTE:
 * This native is different than the rest in that the Y+ is down
 * (scripters need not worry about this).
 */

/*
 * TODO: Save a Brother PEL image (An 8bpp, 130x113 pixel
 *     monochromatic? bitmap image) Why 8bpp when only 1bpp is needed?
 *
 * TODO: Should BMC be limited to ~32KB or is this a mix up with Bitmap Cache?
 * TODO: Is there/should there be other embedded data in the bitmap
 *     besides the image itself?
 * NOTE: Can save a Singer BMC image (An 8bpp, 130x113 pixel colored
 *     bitmap image)
 * TODO: figure out how to center the image, right now it just plops
 *     it to the left side.
*/
void
saveBMC(void/* MdiWindow *subwindow */)
{
    /*  
    img = QImage(150, 150, "QImage-Format-ARGB32-Premultiplied")
    img.fill(qRgb(255,255,255))
    extents = gscene.itemsBoundingRect()

    painter = QPainter(img)
    targetRect = Rect(0,0,150,150)
    if (printing-disable-bg) { */
        /* TODO: Make BMC background into it's own setting? */ /*
        brush = gscene.backgroundBrush()
        gscene.setBackgroundBrush(Qt-NoBrush)
        scene_update()
        gscene.render(painter, targetRect, extents, "Qt-KeepAspectRatio")
        gscene.setBackgroundBrush(brush)
    }
    else {
        scene_update()
        gscene.render(painter, targetRect, extents, "Qt-KeepAspectRatio")
    }
    img.convertToFormat(QImage-Format-Indexed8, Qt-ThresholdDither|Qt-AvoidDither).save("test.bmc", "BMP")
    */
}

void
create_settings_widget(setting)
{
    /* translate(setting.description),
        int-dialog[setting.index] = int-setting[setting.index] */
    if (setting.type[0] == 'i') { /* Int */
        return
    }
    if (setting.type[1] == 'o') { /* dOuble */
        return
    }
    if (setting.type[1] == 'r') { /* dRopdown */
        return
    }
    if (setting.type[0] == 's') { /* String */
        return
    }
    /* Error. */
}
#endif

void
create_settings_box(int box)
{
    int i;
    /*
    debug_message(box.title);
    for (i=0; box.settings[i].type >= 0 i++) {
        create_settings_widget(box.settings[i]);
    }
    */
}

void
create_settings_tab(int tab)
{
    int i;
    /*
    debug_message(tab.title);
    for (i=0 i<tab.n-boxes i++) {
        create_settings_box(tab.box[i]);
    }
    */
}


/* 
class settings-dialog-action()
    tab-widget* tab-widget
    dialogButtonBox* buttonBox
*/

/* showTab is the tab index, use the TAB-GENERAL style defines.
 */
void
settings_dialog_init(int showTab)
{
    /* mw = mw;
    accept = copy();
    dialog = copy();
    preview = copy();

    window = tk.Tk();
    window.size(750,550);
    tab-widget = tk.tab-widget(window); */

    /* TODO: Add icons to tabs */
    /* tab-widget.add-tab(create-tab-general(), translate("General"))
    tab-widget.add-tab(createTabFilesPaths(), translate("Files/Paths"))
    tab-widget.add-tab(createTabDisplay(), translate("Display"))
    tab-widget.add-tab(createTabOpenSave(), translate("Open/Save"))
    tab-widget.add-tab(createTabPrinting(), translate("Printing"))
    tab-widget.add-tab(createTabSnap(), translate("Snap"))
    tab-widget.add-tab(createTabGridRuler(), translate("Grid/Ruler"))
    tab-widget.add-tab(createTabOrthoPolar(), translate("Ortho/Polar"))
    tab-widget.add-tab(createTabQuickSnap(), translate("QuickSnap"))
    tab-widget.add-tab(createTabQuickTrack(), translate("QuickTrack"))
    tab-widget.add-tab(createTabLineWeight(), translate("LineWeight"))
    tab-widget.add-tab(createTabSelection(), translate("Selection"))

    for i in range(12)
        if showTab == settings-tab-label[i]:
            tab-widget.setCurrentIndex(i)

    buttonBox = tk.DialogButtonBox(QDialogButtonBox-Ok | tk.DialogButtonBox-Cancel)

    # connect(buttonBox, SIGNAL(accepted()), this, SLOT(acceptChanges()))
    # connect(buttonBox, SIGNAL(rejected()), this, SLOT(rejectChanges()))

    vbox-layout-main = tk.VBoxLayout()
    vbox-layout-main.addWidget(tab-widget)
    vbox-layout-main.addWidget(buttonBox)
    setLayout(vbox-layout-main)

    window.title(translate("Settings"))
    window.setOverrideCursor("ArrowCursor") */
}

void
create_tab_general(void)
{
    /* widget = tk.Widget(window);

    #Language
    groupboxLanguage = tk.groupbox(translate("Language"), widget);

    labelLanguage = tk.Label(translate("Language (Requires Restart)"), groupboxLanguage);
    comboboxLanguage = tk.combobox(groupboxLanguage);
    to-lower(dialog-general-language, general-language);
    comboboxLanguage.add-item("Default");
    comboboxLanguage.add-item("System");
    comboboxLanguage.insertSeparator(2);
    trDir = tk.App.applicationDirPath();
    trDir.cd("translations");
    for dirName in trDir.entryList(QDir-Dirs | tk.Dir-NoDotAndDotDot)
        dirName[0] = dirName[0].toUpper();
        comboboxLanguage.add-item(dirName);

    current = dialog-general-language;
    current[0] = current[0].toUpper();
    comboboxLanguage.setCurrentIndex(comboboxLanguage.findText(current));
    #connect(comboboxLanguage, SIGNAL(current-index-changed("")), this, SLOT(comboboxLanguageCurrent-index-changed("")));

    vboxLayoutLanguage = tk.VBoxLayout(groupboxLanguage);
    vboxLayoutLanguage.addWidget(labelLanguage);
    vboxLayoutLanguage.addWidget(comboboxLanguage);
    groupboxLanguage.setLayout(vboxLayoutLanguage);

    #Icons
    groupboxIcon = tk.groupbox(translate("Icons"), widget);

    labelIconTheme = tk.Label(translate("Icon Theme"), groupboxIcon);
    comboboxIconTheme = tk.combobox(groupboxIcon);
    dir = tk.App.applicationDirPath();
    dir.cd("icons");
    dialog-general-icon-theme = general-icon-theme;
    for dirName in dir.entryList(QDir-Dirs | tk.Dir-NoDotAndDotDot)
        comboboxIconTheme.add-item(load-icon(theme-xpm), dirName);

    comboboxIconTheme.setCurrentIndex(comboboxIconTheme.findText(dialog-general-icon-theme));
    #connect(comboboxIconTheme, SIGNAL(current-index-changed("")), this, SLOT(comboboxIconThemeCurrent-index-changed("")));

    labelIconSize = tk.Label(translate("Icon Size"), groupboxIcon);
    combobox-icon-size = tk.combobox(groupboxIcon);
    combobox-icon-size.add-item(load-icon("icon16-xpm"), "Very Small", 16);
    combobox-icon-size.add-item(load-icon("icon24-xpm"), "Small", 24);
    combobox-icon-size.add-item(load-icon("icon32-xpm"), "Medium", 32);
    combobox-icon-size.add-item(load-icon("icon48-xpm"), "Large", 48);
    combobox-icon-size.add-item(load-icon("icon64-xpm"), "Very Large", 64);
    combobox-icon-size.add-item(load-icon("icon128-xpm"), "I'm Blind", 128);
    dialog-general-icon-size = general-icon-size;
    combobox-icon-size.setCurrentIndex(combobox-icon-size.findData(dialog-general-icon-size));
    #connect(combobox-icon-size, SIGNAL(current-index-changed(int)), this, SLOT(combobox-icon-sizeCurrent-index-changed(int)));

    vbox-layout-icon = tk.QVBoxLayout(groupboxIcon);
    vbox-layout-icon.addWidget(labelIconTheme);
    vbox-layout-icon.addWidget(comboboxIconTheme);
    vbox-layout-icon.addWidget(labelIconSize);
    vbox-layout-icon.addWidget(combobox-icon-size);
    groupboxIcon.setLayout(vbox-layout-icon);

    #Mdi Background
    groupboxMdiBG = tk.groupbox(window, text=translate("Background"));

    checkbox-mdi-bg-use-logo = tk.checkbox(groupboxMdiBG, text=translate("Use Logo"));
    dialog-general-mdi-bg-use-logo = general-mdi-bg-use-logo;
    preview["general-mdi-bg-use-logo = dialog-general-mdi-bg-use-logo;
    checkbox-mdi-bg-use-logo.setChecked(preview["general-mdi-bg-use-logo);
    #connect(checkbox-mdi-bg-use-logo, SIGNAL(-state-changed(int)), this, SLOT(checkboxGeneralMdiBGUseLogo-state-changed(int)));

    button-mdi-bg-logo = tk.Button(groupboxMdiBG, text=translate("Choose"));
    button-mdi-bg-logo.setEnabled(dialog-general-mdi-bg-use-logo);
    dialog-general-mdi-bg-logo = general-mdi-bg-logo;
    accept["general-mdi-bg-logo = dialog-general-mdi-bg-logo;
    #connect(button-mdi-bg-logo, SIGNAL(clicked()), this, SLOT(chooseGeneralMdiBackgroundLogo()));
    #connect(checkbox-mdi-bg-use-logo, SIGNAL(toggled(int)), button-mdi-bg-logo, SLOT(setEnabled(int)));

    checkbox-mdi-bg-use-texture = tk.checkbox(translate("Use Texture"), groupboxMdiBG);
    dialog-general-mdi-bg-use-texture = general-mdi-bg-use-texture;
    preview["general-mdi-bg-use-texture = dialog-general-mdi-bg-use-texture;
    checkbox-mdi-bg-use-texture.setChecked(preview["general-mdi-bg-use-texture);
    #connect(checkbox-mdi-bg-use-texture, SIGNAL(-state-changed(int)), this, SLOT(checkboxGeneralMdiBGUseTexture-state-changed(int)));

    buttonMdiBGTexture = tk.Button(groupboxMdiBG, text=translate("Choose"))
    buttonMdiBGTexture.setEnabled(dialog-general-mdi-bg-use-texture)
    dialog-general-mdi-bg-texture = general-mdi-bg-texture
    accept["general-mdi-bg-texture = dialog-general-mdi-bg-texture
    #connect(buttonMdiBGTexture, SIGNAL(clicked()), this, SLOT(chooseGeneralMdiBackgroundTexture()))
    #connect(checkbox-mdi-bg-use-texture, SIGNAL(toggled(int)), buttonMdiBGTexture, SLOT(setEnabled(int)))

    checkboxMdiBGUseColor = tk.checkbox(translate("Use Color"), groupboxMdiBG)
    dialog-general-mdi-bg-use-color = general-mdi-bg-use-color
    preview["general-mdi-bg-use-color = dialog-general-mdi-bg-use-color
    checkboxMdiBGUseColor.setChecked(preview.general-mdi-bg-use-color)
    #connect(checkboxMdiBGUseColor, SIGNAL(-state-changed(int)), this, SLOT(checkboxGeneralMdiBGUseColor-state-changed(int)))

    buttonMdiBGColor = PushButton(translate("Choose"), groupboxMdiBG)
    buttonMdiBGColor.setEnabled(dialog-general-mdi-bg-use-color)
    dialog-general-mdi-bg-color = general-mdi-bg-color
    preview["general-mdi-bg-color = dialog-general-mdi-bg-color
    accept["general-mdi-bg-color = dialog-general-mdi-bg-color
    mdiBGPix = Image(16,16)
    mdiBGPix.fill(Color(preview.general-mdi-bg-color))
    buttonMdiBGColor.setIcon(QIcon(mdiBGPix))
    #connect(buttonMdiBGColor, SIGNAL(clicked()), this, SLOT(chooseGeneralMdiBackgroundColor()))
    #connect(checkboxMdiBGUseColor, SIGNAL(toggled(int)), buttonMdiBGColor, SLOT(setEnabled(int)))

    gridLayoutMdiBG = tk.GridLayout(widget)
    gridLayoutMdiBG.addWidget(checkbox-mdi-bg-use-logo, 0, 0, "Align Left")
    gridLayoutMdiBG.addWidget(button-mdi-bg-logo, 0, 1, "Align Right")
    gridLayoutMdiBG.addWidget(checkbox-mdi-bg-use-texture, 1, 0, "Align Left")
    gridLayoutMdiBG.addWidget(buttonMdiBGTexture, 1, 1, "Align Right")
    gridLayoutMdiBG.addWidget(checkboxMdiBGUseColor, 2, 0, "Align Left")
    gridLayoutMdiBG.addWidget(buttonMdiBGColor, 2, 1, "Align Right")
    groupboxMdiBG.setLayout(gridLayoutMdiBG)

    #Tips
    groupboxTips = tk.groupbox(translate("Tips"), widget)

    checkboxTipOfTheDay = tk.checkbox(translate("Show Tip of the Day on startup"), groupboxTips)
    dialog-general-tip-of-the-day = general-tip-of-the-day
    checkboxTipOfTheDay.setChecked(dialog-general-tip-of-the-day)
    #connect(checkboxTipOfTheDay, SIGNAL(-state-changed(int)), this, SLOT(checkboxTipOfTheDay-state-changed(int)))

    vboxLayoutTips = tk.VBoxLayout(groupboxTips)
    vboxLayoutTips.addWidget(checkboxTipOfTheDay)
    groupboxTips.setLayout(vboxLayoutTips)

    #Help Browser
    groupboxHelpBrowser = tk.groupbox(translate("Help Browser"), widget)

    radio-button-system-help-browser = tk.RadioButton(translate("System"), groupboxHelpBrowser)
    radio-button-system-help-browser.setChecked(general-system-help-browser)
    radio-button-custom-help-browser = tk.RadioButton(translate("Custom"), groupboxHelpBrowser)
    radio-button-custom-help-browser.setChecked(!general-system-help-browser)
    radio-button-custom-help-browser.setEnabled(0) TODO: finish this */

    /*
    vboxLayoutHelpBrowser = tk.VBoxLayout(groupboxHelpBrowser)
    vboxLayoutHelpBrowser.addWidget(radio-button-system-help-browser)
    vboxLayoutHelpBrowser.addWidget(radio-button-custom-help-browser)
    groupboxHelpBrowser.setLayout(vboxLayoutHelpBrowser)
*/
    /* Widget Layout */ /*
    vbox-layout-main = tk.VBoxLayout(widget)
    vbox-layout-main.addWidget(groupboxLanguage)
    vbox-layout-main.addWidget(groupboxIcon)
    vbox-layout-main.addWidget(groupboxMdiBG)
    vbox-layout-main.addWidget(groupboxTips)
    vbox-layout-main.addWidget(groupboxHelpBrowser)
    vbox-layout-main.addStretch(1)
    widget.setLayout(vbox-layout-main)

    scroll-area = tk.scroll-area(this)
    scroll-area.setWidgetResizable(1)
    scroll-area.setWidget(widget)
    return scroll-area */
}

void
create_tab_files_paths(void)
{
    /* widget = tk.Widget(this)

    scroll-area = tk.scroll-area(this)
    scroll-area.setWidgetResizable(1)
    scroll-area.setWidget(widget)
    return scroll-area */
}

void
create_tab_display(void)
{
    /*
    widget = tk.Widget(this)

    #Rendering
    #TODO: Review OpenGL and Rendering settings for future inclusion
    #
    groupboxRender = tk.groupbox(translate("Rendering"), widget)

    checkboxUseOpenGL = tk.checkbox(translate("Use OpenGL"), groupboxRender)
    int-dialog[INT-DISPLAY-USE-OPENGL] = int-setting[INT-DISPLAY-USE-OPENGL]
    checkboxUseOpenGL.setChecked(int-dialog[INT-DISPLAY-USE-OPENGL)
    #connect(checkboxUseOpenGL, SIGNAL(-state-changed(int)), this, SLOT(checkboxUseOpenGL-state-changed(int)))

    checkboxRenderHintAA = tk.checkbox(translate("Antialias"), groupboxRender)
    dialog-display-renderhint-aa = display-render-hint-aa
    checkboxRenderHintAA.setChecked(dialog-display-renderhint-aa)
    #connect(checkboxRenderHintAA, SIGNAL(-state-changed(int)), this, SLOT(checkboxRenderHintAA-state-changed(int)))

    checkboxRenderHintTextAA = tk.checkbox(translate("Antialias Text"), groupboxRender)
    dialog-display-renderhint-text-aa = display-render-hint-text-aa
    checkboxRenderHintTextAA.setChecked(dialog-display-renderhint-text-aa)
    #connect(checkboxRenderHintTextAA, SIGNAL(-state-changed(int)), this, SLOT(checkboxRenderHintTextAA-state-changed(int)))

    checkboxRenderHintSmoothPix = tk.checkbox(translate("Smooth Pixmap"), groupboxRender)
    dialog-display-renderhint-smooth-pix = display-render-hint-smooth-pix
    checkboxRenderHintSmoothPix.setChecked(dialog-display-renderhint-smooth-pix)
    #connect(checkboxRenderHintSmoothPix, SIGNAL(-state-changed(int)), this, SLOT(checkboxRenderHintSmoothPix-state-changed(int)))

    checkboxRenderHintHighAA = tk.checkbox(translate("High Quality Antialiasing (OpenGL)"), groupboxRender)
    dialog-display-renderhint-high-aa = display-render-hint-high-aa
    checkboxRenderHintHighAA.setChecked(dialog-display-renderhint-high-aa)
    #connect(checkboxRenderHintHighAA, SIGNAL(-state-changed(int)), this, SLOT(checkboxRenderHintHighAA-state-changed(int)))

    checkboxRenderHintNonCosmetic = tk.checkbox(translate("Non Cosmetic"), groupboxRender)
    dialog-display-renderhint-noncosmetic = display-render-hint-non-cosmetic
    checkboxRenderHintNonCosmetic.setChecked(dialog-display-renderhint-noncosmetic)
    #connect(checkboxRenderHintNonCosmetic, SIGNAL(-state-changed(int)), this, SLOT(checkboxRenderHintNonCosmetic-state-changed(int)))

    vboxLayoutRender = tk.VBoxLayout(groupboxRender)
    vboxLayoutRender.addWidget(checkboxUseOpenGL)
    vboxLayoutRender.addWidget(checkboxRenderHintAA)
    vboxLayoutRender.addWidget(checkboxRenderHintTextAA)
    vboxLayoutRender.addWidget(checkboxRenderHintSmoothPix)
    vboxLayoutRender.addWidget(checkboxRenderHintHighAA)
    vboxLayoutRender.addWidget(checkboxRenderHintNonCosmetic)
    groupboxRender.setLayout(vboxLayoutRender)

    #ScrollBars
    groupboxScrollBars = tk.groupbox(translate("ScrollBars"), widget)

    checkboxShowScrollBars = tk.checkbox(translate("Show ScrollBars"), groupboxScrollBars)
    dialog-display-show-scrollbars = display-show-scrollbars
    preview.display-show-scrollbars = dialog-display-show-scrollbars
    checkboxShowScrollBars.setChecked(preview.display-show-scrollbars)
    #connect(checkboxShowScrollBars, SIGNAL(-state-changed(int)), this, SLOT(checkboxShowScrollBars-state-changed(int)))

    labelScrollBarWidget = tk.Label(translate("Perform action when clicking corner widget"), groupboxScrollBars)
    comboboxScrollBarWidget = tk.combobox(groupboxScrollBars)
    dialog-display-scrollbar-widget-num = display-scrollbar-widget-num
    numActions = actionHash.size();
    for (i=0; i<numActions; i++) {
        action = actionHash.value(i);
        if (action) {
            comboboxScrollBarWidget.add-item(action.icon(), action.text().replace("&", ""));
        }
    }

    comboboxScrollBarWidget.setCurrentIndex(dialog-display-scrollbar-widget-num)
    #connect(comboboxScrollBarWidget, SIGNAL(current-index-changed(int)), this, SLOT(comboboxScrollBarWidgetCurrent-index-changed(int)))

    vboxLayoutScrollBars = tk.VBoxLayout(groupboxScrollBars)
    vboxLayoutScrollBars.addWidget(checkboxShowScrollBars)
    vboxLayoutScrollBars.addWidget(labelScrollBarWidget)
    vboxLayoutScrollBars.addWidget(comboboxScrollBarWidget)
    groupboxScrollBars.setLayout(vboxLayoutScrollBars)

    #Colors
    groupboxColor = tk.groupbox(translate("Colors"), widget)

    labelCrossHairColor = tk.Label(translate("Crosshair Color"), groupboxColor);
    buttonCrossHairColor = PushButton(translate("Choose"), groupboxColor);
    dialog-display-crosshair-color = display-crosshair-color;
    preview.display-crosshair-color = dialog-display-crosshair-color;
    accept.display-crosshair-color = dialog-display-crosshair-color;
    crosshairPix = (16,16);
    crosshairPix.fill(Color(preview.display-crosshair-color));
    buttonCrossHairColor.setIcon(QIcon(crosshairPix));
    #connect(buttonCrossHairColor, SIGNAL(clicked()), this, SLOT(chooseDisplayCrossHairColor()));

    labelBGColor = tk.Label(translate("Background Color"), groupboxColor);
    buttonBGColor = PushButton(translate("Choose"), groupboxColor);
    dialog-display-bg-color = display-bg-color;
    preview.display-bg-color = dialog-display-bg-color;
    accept.display-bg-color = dialog-display-bg-color;
    bgPix = Image(16,16);
    bgPix.fill(Color(preview.display-bg-color));
    buttonBGColor.setIcon(QIcon(bgPix));
    #connect(buttonBGColor, SIGNAL(clicked()), this, SLOT(chooseDisplayBackgroundColor()));

    labelSelectBoxLeftColor = tk.Label(translate("Selection Box Color (Crossing)"), groupboxColor);
    buttonSelectBoxLeftColor = PushButton(translate("Choose"), groupboxColor);
    dialog-display-selectbox-left-color = display-selectbox-left-color;
    preview["display-selectbox-left-color = dialog-display-selectbox-left-color;
    accept["display-selectbox-left-color = dialog-display-selectbox-left-color;
    sBoxLCPix = Image(16,16);
    sBoxLCPix.fill(Color(preview["display-selectbox-left-color));
    buttonSelectBoxLeftColor.setIcon(QIcon(sBoxLCPix));
    #connect(buttonSelectBoxLeftColor, SIGNAL(clicked()), this, SLOT(chooseDisplaySelectBoxLeftColor()));

    labelSelectBoxLeftFill = tk.Label(translate("Selection Box Fill (Crossing)"), groupboxColor);
    buttonSelectBoxLeftFill = PushButton(translate("Choose"), groupboxColor);
    dialog-display-selectbox-left-fill = display-selectbox-left-fill;
    preview["display-selectbox-left-fill = dialog-display-selectbox-left-fill;
    accept["display-selectbox-left-fill = dialog-display-selectbox-left-fill;
    sBoxLFPix = Image(16,16);
    sBoxLFPix.fill(Color(preview["display-selectbox-left-fill));
    buttonSelectBoxLeftFill.setIcon(QIcon(sBoxLFPix));
    #connect(buttonSelectBoxLeftFill, SIGNAL(clicked()), this, SLOT(chooseDisplaySelectBoxLeftFill()));

    labelSelectBoxRightColor = tk.Label(translate("Selection Box Color (Window)"), groupboxColor);
    buttonSelectBoxRightColor = tk.PushButton(translate("Choose"), groupboxColor);
    dialog-display-selectbox-right-color = display-selectbox-right-color;
    preview["display-selectbox-right-color = dialog-display-selectbox-right-color;
    accept["display-selectbox-right-color = dialog-display-selectbox-right-color;
    sBoxRCPix = Image(16,16);
    sBoxRCPix.fill(Color(preview["display-selectbox-right-color));
    buttonSelectBoxRightColor.setIcon(QIcon(sBoxRCPix));
    #connect(buttonSelectBoxRightColor, SIGNAL(clicked()), this, SLOT(chooseDisplaySelectBoxRightColor()));

    labelSelectBoxRightFill = tk.Label(translate("Selection Box Fill (Window)"), groupboxColor);
    buttonSelectBoxRightFill = PushButton(translate("Choose"), groupboxColor);
    dialog-display-selectbox-right-fill = display-selectbox-right-fill;
    preview.display-selectbox-right-fill = dialog-display-selectbox-right-fill;
    accept.display-selectbox-right-fill = dialog-display-selectbox-right-fill;
    sBoxRFPix = Image(16,16);
    sBoxRFPix.fill(Color(preview.display-selectbox-right-fill));
    buttonSelectBoxRightFill.setIcon(QIcon(sBoxRFPix));
    #connect(buttonSelectBoxRightFill, SIGNAL(clicked()), this, SLOT(chooseDisplaySelectBoxRightFill()));

    labelSelectBoxAlpha = tk.Label(translate("Selection Box Fill Alpha"), groupboxColor);
    spinboxSelectBoxAlpha = tk.spinbox(groupboxColor);
    spinboxSelectBoxAlpha.setRange(0, 255);
    dialog-display-selectbox-alpha = display-selectbox-alpha;
    preview.display-selectbox-alpha = dialog-display-selectbox-alpha;
    spinboxSelectBoxAlpha.setValue(preview.display-selectbox-alpha);
    #connect(spinboxSelectBoxAlpha, SIGNAL(-value-changed(int)), this, SLOT(spinboxDisplaySelectBoxAlpha-value-changed(int)));

    gridLayoutColor = tk.GridLayout(widget);
    gridLayoutColor.addWidget(labelCrossHairColor, 0, 0, "Align Left");
    gridLayoutColor.addWidget(buttonCrossHairColor, 0, 1, "Align Right");
    gridLayoutColor.addWidget(labelBGColor, 1, 0, "Align Left");
    gridLayoutColor.addWidget(buttonBGColor, 1, 1, "Align Right");
    gridLayoutColor.addWidget(labelSelectBoxLeftColor, 2, 0, "Align Left");
    gridLayoutColor.addWidget(buttonSelectBoxLeftColor, 2, 1, "Align Right");
    gridLayoutColor.addWidget(labelSelectBoxLeftFill, 3, 0, "Align Left");
    gridLayoutColor.addWidget(buttonSelectBoxLeftFill, 3, 1, "Align Right");
    gridLayoutColor.addWidget(labelSelectBoxRightColor, 4, 0, "Align Left");
    gridLayoutColor.addWidget(buttonSelectBoxRightColor, 4, 1, "Align Right");
    gridLayoutColor.addWidget(labelSelectBoxRightFill, 5, 0, "Align Left");
    gridLayoutColor.addWidget(buttonSelectBoxRightFill, 5, 1, "Align Right");
    gridLayoutColor.addWidget(labelSelectBoxAlpha, 6, 0, "Align Left");
    gridLayoutColor.addWidget(spinboxSelectBoxAlpha, 6, 1, "Align Right");
    groupboxColor.setLayout(gridLayoutColor);

    #Zoom
    groupboxZoom = tk.groupbox(translate("Zoom"), widget);

    labelzoom-scale-actionIn = tk.Label(translate("Zoom In Scale"), groupboxZoom);
    spinboxzoom-scale-actionIn = tk.Doublespinbox(groupboxZoom);
    dialog-display-zoom-scale-action-in = display-zoom-scale-action-in;
    spinboxzoom-scale-actionIn.setValue(dialog-display-zoom-scale-action-in);
    spinboxzoom-scale-actionIn.setSingleStep(0.01);
    spinboxzoom-scale-actionIn.setRange(1.01, 10.00);
    #connect(spinboxzoom-scale-actionIn, SIGNAL(-value-changed(double)), this, SLOT(spinboxzoom-scale-actionIn-value-changed(double)));

    labelzoom-scale-actionOut = tk.Label(translate("Zoom Out Scale"), groupboxZoom)
    spinboxzoom-scale-actionOut = tk.Doublespinbox(groupboxZoom)
    dialog-display-zoom-scale-action-out = display-zoom-scale-action-out
    spinboxzoom-scale-actionOut.setValue(dialog-display-zoom-scale-action-out)
    spinboxzoom-scale-actionOut.setSingleStep(0.01)
    spinboxzoom-scale-actionOut.setRange(0.01, 0.99)
    #connect(spinboxzoom-scale-actionOut, SIGNAL(-value-changed(double)), this, SLOT(spinboxzoom-scale-actionOut-value-changed(double)))

    gridLayoutZoom = tk.GridLayout(groupboxZoom)
    gridLayoutZoom.addWidget(labelzoom-scale-actionIn, 0, 0, "Align Left")
    gridLayoutZoom.addWidget(spinboxzoom-scale-actionIn, 0, 1, "Align Right")
    gridLayoutZoom.addWidget(labelzoom-scale-actionOut, 1, 0, "Align Left")
    gridLayoutZoom.addWidget(spinboxzoom-scale-actionOut, 1, 1, "Align Right")
    groupboxZoom.setLayout(gridLayoutZoom)

    #Widget Layout
    vbox-layout-main = tk.VBoxLayout(widget)
    #vbox-layout-main.addWidget(groupboxRender)
    # TODO: Review OpenGL and Rendering settings for future inclusion
    vbox-layout-main.addWidget(groupboxScrollBars)
    vbox-layout-main.addWidget(groupboxColor)
    vbox-layout-main.addWidget(groupboxZoom)
    vbox-layout-main.addStretch(1)
    widget.setLayout(vbox-layout-main)

    scroll-area = tk.scroll-area(this)
    scroll-area.setWidgetResizable(1)
    scroll-area.setWidget(widget)
    return scroll-area
    */
}

/* TODO: finish open/save options */
void
createTabOpenSave(void)
{
    /*
    widget = tk.Widget(this)

    #Custom Filter
    groupboxCustomFilter = tk.groupbox(translate("Custom Filter"), widget)
    groupboxCustomFilter.setEnabled(0) #TODO: Fixup custom filter

    buttonCustomFilterSelectAll = PushButton(translate("Select All"), groupboxCustomFilter)
    #connect(buttonCustomFilterSelectAll, SIGNAL(clicked()), this, SLOT(buttonCustomFilterSelectAllClicked()))
    buttonCustomFilterClearAll = PushButton("Clear All", groupboxCustomFilter)
    #connect(buttonCustomFilterClearAll, SIGNAL(clicked()), this, SLOT(buttonCustomFilterClearAllClicked()))
    gridLayoutCustomFilter = tk.GridLayout(groupboxCustomFilter)

    for i in range(numberOfFormats)
        c = checkbox(formatTable[i].extension, groupboxCustomFilter)
        c.setChecked(opensave-custom-filter.contains(QString("*") + formatTable[i].extension, tk.t-CaseInsensitive))
        #connect(c, SIGNAL(-state-changed(int)), this, SLOT(checkboxCustomFilter-state-changed(int)))
        #connect(this, SIGNAL(buttonCustomFilterSelectAll(int)), c, SLOT(setChecked(int)))
        #connect(this, SIGNAL(buttonCustomFilterClearAll(int)), c, SLOT(setChecked(int)))
        gridLayoutCustomFilter.addWidget(c, i%10, i/10, "Align Left")

    gridLayoutCustomFilter.addWidget(buttonCustomFilterSelectAll, 0, 7, "Align Left")
    gridLayoutCustomFilter.addWidget(buttonCustomFilterClearAll, 1, 7, "Align Left")
    gridLayoutCustomFilter.setColumnStretch(7,1)
    groupboxCustomFilter.setLayout(gridLayoutCustomFilter)

    if opensave-custom-filter.contains("supported", tk.t-CaseInsensitive)
        buttonCustomFilterSelectAllClicked()

    # Opening
    groupboxOpening = tk.groupbox(translate("File Open"), widget)

    comboboxOpenFormat = tk.combobox(groupboxOpening);

    checkboxOpenThumbnail = tk.checkbox(translate("Preview Thumbnails"), groupboxOpening);
    checkboxOpenThumbnail.setChecked(0);

    # TODO: Add a button to clear the recent history.

    labelRecentMaxFiles = tk.Label(translate("Number of recently accessed files to show"), groupboxOpening);
    spinboxRecentMaxFiles = tk.spinbox(groupboxOpening);
    spinboxRecentMaxFiles.setRange(0, 10);
    dialog-opensave-recent-max-files = opensave-recent-max-files;
    spinboxRecentMaxFiles.setValue(dialog-opensave-recent-max-files);
    #connect(spinboxRecentMaxFiles, SIGNAL(-value-changed(int)), this, SLOT(spinboxRecentMaxFiles-value-changed(int)));

    frameRecent = tk.Frame(groupboxOpening);
    gridLayoutRecent = tk.GridLayout(frameRecent);
    gridLayoutRecent.addWidget(labelRecentMaxFiles, 0, 0, "AlignLeft");
    gridLayoutRecent.addWidget(spinboxRecentMaxFiles, 0, 1, "Align Right");
    frameRecent.setLayout(gridLayoutRecent);

    vboxLayoutOpening = tk.VBoxLayout(groupboxOpening);
    vboxLayoutOpening.addWidget(comboboxOpenFormat);
    vboxLayoutOpening.addWidget(checkboxOpenThumbnail);
    vboxLayoutOpening.addWidget(frameRecent);
    groupboxOpening.setLayout(vboxLayoutOpening);

    #Saving
    groupboxSaving = tk.groupbox(translate("File Save"), widget);

    comboboxSaveFormat = tk.combobox(groupboxSaving);

    checkboxSaveThumbnail = tk.checkbox(translate("Save Thumbnails"), groupboxSaving);
    checkboxSaveThumbnail.setChecked(0);

    checkboxAutoSave = tk.checkbox(translate("AutoSave"), groupboxSaving);
    checkboxAutoSave.setChecked(0);

    vboxLayoutSaving = tk.VBoxLayout(groupboxSaving);
    vboxLayoutSaving.addWidget(comboboxSaveFormat);
    vboxLayoutSaving.addWidget(checkboxSaveThumbnail);
    vboxLayoutSaving.addWidget(checkboxAutoSave);
    groupboxSaving.setLayout(vboxLayoutSaving);

    #Trimming
    groupboxTrim = tk.groupbox(translate("Trimming"), widget);

    labelTrimDstNumJumps = tk.Label(translate("DST Only: Minimum number of jumps to trim"), groupboxTrim);
    spinboxTrimDstNumJumps = tk.spinbox(groupboxTrim);
    spinboxTrimDstNumJumps.setRange(1, 20);
    dialog-opensave-trim-dst-num-jumps = opensave-trim-dst-num-jumps;
    spinboxTrimDstNumJumps.setValue(dialog-opensave-trim-dst-num-jumps);
    #connect(spinboxTrimDstNumJumps, SIGNAL(-value-changed(int)), this, SLOT(spinboxTrimDstNumJumps-value-changed(int)));

    frameTrimDstNumJumps = tk.Frame(groupboxTrim);
    gridLayoutTrimDstNumJumps = tk.GridLayout(frameTrimDstNumJumps);
    gridLayoutTrimDstNumJumps.addWidget(labelTrimDstNumJumps, 0, 0, "Align Left");
    gridLayoutTrimDstNumJumps.addWidget(spinboxTrimDstNumJumps, 0, 1, "Align Right");
    frameTrimDstNumJumps.setLayout(gridLayoutTrimDstNumJumps);

    vboxLayoutTrim = tk.VBoxLayout(groupboxTrim);
    vboxLayoutTrim.addWidget(frameTrimDstNumJumps);
    groupboxTrim.setLayout(vboxLayoutTrim);

    #Widget Layout
    vbox-layout-main = tk.VBoxLayout(widget);
    vbox-layout-main.addWidget(groupboxCustomFilter);
    vbox-layout-main.addWidget(groupboxOpening);
    vbox-layout-main.addWidget(groupboxSaving);
    vbox-layout-main.addWidget(groupboxTrim);
    vbox-layout-main.addStretch(1);
    widget.setLayout(vbox-layout-main);

    scroll-area = tk.scroll-area(this);
    scroll-area.setWidgetResizable(1);
    scroll-area.setWidget(widget);
    return scroll-area;
    */
}

void
create_tab_printing(void)
{
    /*
    widget = tk.Widget(this);

    # Default Printer
    groupboxDefaultPrinter = tk.groupbox(translate("Default Printer"), widget);

    radioButtonUseSame = tk.RadioButton(translate("Use as default device"), groupboxDefaultPrinter);
    radioButtonUseSame.setChecked(!printing-use-last-device);
    radioButtonUseLast = tk.RadioButton(translate("Use last used device"), groupboxDefaultPrinter);
    radioButtonUseLast.setChecked(printing-use-last-device);

    comboboxDefaultDevice = tk.combobox(groupboxDefaultPrinter);
    listAvailPrinters = tk.PrinterInfo-availablePrinters();
    for info in listAvailPrinters:
        comboboxDefaultDevice.add-item(load-icon(print-xpm), info.printerName());

    vboxLayoutDefaultPrinter = tk.VBoxLayout(groupboxDefaultPrinter);
    vboxLayoutDefaultPrinter.addWidget(radioButtonUseSame);
    vboxLayoutDefaultPrinter.addWidget(comboboxDefaultDevice);
    vboxLayoutDefaultPrinter.addWidget(radioButtonUseLast);
    groupboxDefaultPrinter.setLayout(vboxLayoutDefaultPrinter);

    # Save Ink
    groupboxSaveInk = tk.groupbox(translate("Save Ink"), widget);

    checkboxDisableBG = tk.checkbox(translate("Disable Background"), groupboxSaveInk);
    dialog-printing-disable-bg = printing-disable-bg;
    checkboxDisableBG.setChecked(dialog-printing-disable-bg);
    #connect(checkboxDisableBG, SIGNAL(-state-changed(int)), this, SLOT(checkboxDisableBG-state-changed(int)));

    vboxLayoutSaveInk = tk.VBoxLayout(groupboxSaveInk);
    vboxLayoutSaveInk.addWidget(checkboxDisableBG);
    groupboxSaveInk.setLayout(vboxLayoutSaveInk);

    #Widget Layout
    vbox-layout-main = tk.VBoxLayout(widget);
    vbox-layout-main.addWidget(groupboxDefaultPrinter);
    vbox-layout-main.addWidget(groupboxSaveInk);
    vbox-layout-main.addStretch(1);
    widget.setLayout(vbox-layout-main);

    scroll-area = tk.scroll-area(this);
    #scroll-area.setWidgetResizable(1);
    scroll-area.setWidget(widget);
    return scroll-area;
    */
}

void
create_tab_snap(void)
{
    /*
    widget = tk.Widget(this);

    #TODO: finish this

    scroll-area = tk.scroll-area(this);
    scroll-area.setWidgetResizable(1);
    scroll-area.setWidget(widget);
    return scroll-area;
    */
}

#if 0
void
createTabGridRuler(void)
{
    create-settings-tab(grid-ruler-settings);
    /*
    widget = tk.Widget(this);

    #Grid Color
    groupboxGridColor = tk.groupbox(translate("Grid Color"), widget);

    labelGridColor = tk.Label(translate("Grid Color"), groupboxGridColor);
    labelGridColor.setObjectName("labelGridColor");
    buttonGridColor = PushButton(translate("Choose"), groupboxGridColor);
    buttonGridColor.setObjectName("buttonGridColor");
    if dialogGridcolor-match-crosshair:
        dialogGridcolor = display-crosshair-color;
    else {
        dialogGridcolor = grid-color;
    preview.grid-color = dialogGridcolor;
    accept.grid-color = dialogGridcolor;
    gridPix = Image(16,16);
    gridPix.fill(Color(preview.grid-color));
    buttonGridColor.setIcon(QIcon(gridPix));
    #connect(buttonGridColor, SIGNAL(clicked()), this, SLOT(chooseGridColor()));

    labelGridColor.setEnabled(!dialogGridcolor-match-crosshair);
    buttonGridColor.setEnabled(!dialogGridcolor-match-crosshair);

    gridLayoutGridColor = tk.GridLayout(widget);
    gridLayoutGridColor.addWidget(checkboxGridColorMatchCrossHair, 0, 0, "Align Left");
    gridLayoutGridColor.addWidget(labelGridColor, 1, 0, "Align Left");
    gridLayoutGridColor.addWidget(buttonGridColor, 1, 1, "Align Right");
    groupboxGridColor.setLayout(gridLayoutGridColor);

    #Grid Geometry
    groupboxGridGeom = tk.groupbox(translate("Grid Geometry"), widget);

    checkboxGridload-from-file = tk.checkbox(translate("Set grid size from opened file"), groupboxGridGeom);
    dialogGridload-from-file = grid-load-from-file;
    checkboxGridload-from-file.setChecked(dialogGridload-from-file);
    #connect(checkboxGridload-from-file, SIGNAL(-state-changed(int)), this, SLOT(checkboxGridload-from-file-state-changed(int)))

    labelGridtype = tk.Label(translate("Grid Type"), groupboxGridGeom)
    labelGridtype.setObjectName("labelGridtype")
    comboboxGridtype = tk.combobox(groupboxGridGeom)
    comboboxGridtype.setObjectName("comboboxGridtype")
    comboboxGridtype.add-item("Rectangular")
    comboboxGridtype.add-item("Circular")
    comboboxGridtype.add-item("Isometric")
    strcpy(dialogGridtype, grid-type)
    comboboxGridtype.setCurrentIndex(comboboxGridtype.findText(dialogGridtype))
    # #connect(comboboxGridtype, SIGNAL(current-index-changed("")), this, SLOT(comboboxGridtypeCurrent-index-changed("")))

    checkboxGridcenter-on-origin = tk.checkbox(translate("Center the grid on the origin"), groupboxGridGeom)
    checkboxGridcenter-on-origin.setObjectName("checkboxGridcenter-on-origin")
    dialogGridcenter-on-origin = grid-center-on-origin
    checkboxGridcenter-on-origin.setChecked(dialogGridcenter-on-origin)
    #connect(checkboxGridcenter-on-origin, SIGNAL(-state-changed(int)), this, SLOT(checkboxGridcenter-on-origin-state-changed(int)))

    labelGridcenter-x = tk.Label(translate("Grid Center X"), groupboxGridGeom);
    labelGridcenter-x.setObjectName("labelGridcenter-x");
    spinboxGridCenterX = tk.Doublespinbox(groupboxGridGeom);
    spinboxGridCenterX.setObjectName("spinboxGridCenterX");
    dialogGridcenter.x = grid-center.x;
    spinboxGridCenterX.setSingleStep(1.000);
    spinboxGridCenterX.setRange(-1000.000, 1000.000);
    spinboxGridCenterX.setValue(dialogGridcenter.x);
    #connect(spinboxGridCenterX, SIGNAL(-value-changed(double)), this, SLOT(spinboxGridCenterX-value-changed(double)));

    labelGridcenter-y = tk.Label(translate("Grid Center Y"), groupboxGridGeom);
    labelGridcenter-y.setObjectName("labelGridcenter-y");
    spinboxGridcenter-y = tk.Doublespinbox(groupboxGridGeom);
    spinboxGridcenter-y.setObjectName("spinboxGridcenter-y");
    dialogGridcenter.y = grid-center.y;
    spinboxGridcenter-y.setSingleStep(1.000);
    spinboxGridcenter-y.setRange(-1000.000, 1000.000);
    spinboxGridcenter-y.setValue(dialogGridcenter.y);
    #connect(spinboxGridcenter-y, SIGNAL(-value-changed(double)), this, SLOT(spinboxGridcenter-y-value-changed(double)));

    labelGridsizeX = tk.Label(translate("Grid Size X"), groupboxGridGeom);
    labelGridsizeX.setObjectName("labelGridsizeX");
    spinboxGridsizeX = tk.Doublespinbox(groupboxGridGeom);
    spinboxGridsizeX.setObjectName("spinboxGridsizeX");
    dialogGridsize.x = grid-size.x;
    spinboxGridsizeX.setSingleStep(1.000);
    spinboxGridsizeX.setRange(1.000, 1000.000);
    spinboxGridsizeX.setValue(dialogGridsize.x);
    #connect(spinboxGridsizeX, SIGNAL(-value-changed(double)), this, SLOT(spinboxGridsizeX-value-changed(double)));

    labelGridsizeY = tk.Label(translate("Grid Size Y"), groupboxGridGeom);
    labelGridsizeY.setObjectName("labelGridsizeY");
    spinboxGridsizeY = tk.Doublespinbox(groupboxGridGeom);
    spinboxGridsizeY.setObjectName("spinboxGridsizeY");
    dialogGridsize.y = grid-size.y;
    spinboxGridsizeY.setSingleStep(1.000);
    spinboxGridsizeY.setRange(1.000, 1000.000);
    spinboxGridsizeY.setValue(dialogGridsize.y);
    #connect(spinboxGridsizeY, SIGNAL(-value-changed(double)), this, SLOT(spinboxGridsizeY-value-changed(double)));

    labelGridSpacingX = tk.Label(translate("Grid Spacing X"), groupboxGridGeom);
    labelGridSpacingX.setObjectName("labelGridSpacingX");
    spinboxGridSpacingX = tk.Doublespinbox(groupboxGridGeom);
    spinboxGridSpacingX.setObjectName("spinboxGridSpacingX");
    dialogGridspacing.x = grid-spacing.x;
    spinboxGridSpacingX.setSingleStep(1.000);
    spinboxGridSpacingX.setRange(0.001, 1000.000);
    spinboxGridSpacingX.setValue(dialogGridspacing.x);
    #connect(spinboxGridSpacingX, SIGNAL(-value-changed(double)), this, SLOT(spinboxGridSpacingX-value-changed(double)));

    labelGridSpacingY = tk.Label(translate("Grid Spacing Y"), groupboxGridGeom);
    labelGridSpacingY.setObjectName("labelGridSpacingY");
    spinboxGridSpacingY = tk.Doublespinbox(groupboxGridGeom);
    spinboxGridSpacingY.setObjectName("spinboxGridSpacingY");
    dialogGridspacing.y = grid-spacing.y;
    spinboxGridSpacingY.setSingleStep(1.000);
    spinboxGridSpacingY.setRange(0.001, 1000.000);
    spinboxGridSpacingY.setValue(dialogGridspacing.y);
    #connect(spinboxGridSpacingY, SIGNAL(-value-changed(double)), this, SLOT(spinboxGridSpacingY-value-changed(double)));

    labelGridsizeRadius = tk.Label(translate("Grid Size Radius"), groupboxGridGeom);
    labelGridsizeRadius.setObjectName("labelGridsizeRadius");
    spinboxGridsizeRadius = tk.Doublespinbox(groupboxGridGeom);
    spinboxGridsizeRadius.setObjectName("spinboxGridsizeRadius");
    dialogGridsize-radius = grid-size-radius;
    spinboxGridsizeRadius.setSingleStep(1.000);
    spinboxGridsizeRadius.setRange(1.000, 1000.000);
    spinboxGridsizeRadius.setValue(dialogGridsize-radius);
    #connect(spinboxGridsizeRadius, SIGNAL(-value-changed(double)), this, SLOT(spinboxGridsizeRadius-value-changed(double)));

    labelGridSpacingRadius = tk.Label(translate("Grid Spacing Radius"), groupboxGridGeom);
    labelGridSpacingRadius.setObjectName("labelGridSpacingRadius");
    spinboxGridSpacingRadius = tk.Doublespinbox(groupboxGridGeom);
    spinboxGridSpacingRadius.setObjectName("spinboxGridSpacingRadius");
    dialogGridspacing-radius = grid-spacing-radius;
    spinboxGridSpacingRadius.setSingleStep(1.000);
    spinboxGridSpacingRadius.setRange(0.001, 1000.000);
    spinboxGridSpacingRadius.setValue(dialogGridspacing-radius);
    #connect(spinboxGridSpacingRadius, SIGNAL(-value-changed(double)), this, SLOT(spinboxGridSpacingRadius-value-changed(double)));

    labelGridSpacingAngle = tk.Label(translate("Grid Spacing Angle"), groupboxGridGeom);
    labelGridSpacingAngle.setObjectName("labelGridSpacingAngle");
    spinboxGridSpacingAngle = tk.Doublespinbox(groupboxGridGeom);
    spinboxGridSpacingAngle.setObjectName("spinboxGridSpacingAngle");
    dialogGridspacing-angle = grid-spacing-angle;
    spinboxGridSpacingAngle.setSingleStep(1.000);
    spinboxGridSpacingAngle.setRange(0.001, 1000.000);
    spinboxGridSpacingAngle.setValue(dialogGridspacing-angle);
    #connect(spinboxGridSpacingAngle, SIGNAL(-value-changed(double)), this, SLOT(spinboxGridSpacingAngle-value-changed(double)));

    labelGridtype.setEnabled(!dialogGridloadFromFile);
    comboboxGridtype.setEnabled(!dialogGridloadFromFile);
    checkboxGridcenter-on-origin.setEnabled(!dialogGridload-from-file);
    labelGridcenter-x.setEnabled(!dialogGridload-from-file);
    spinboxGridCenterX.setEnabled(!dialogGridload-from-file);
    labelGridcenter-y.setEnabled(!dialogGridload-from-file);
    spinboxGridcenter-y.setEnabled(!dialogGridload-from-file);
    labelGridsizeX.setEnabled(!dialogGridload-from-file);
    spinboxGridsizeX.setEnabled(!dialogGridload-from-file);
    labelGridsizeY.setEnabled(!dialogGridload-from-file);
    spinboxGridsizeY.setEnabled(!dialogGridload-from-file);
    labelGridSpacingX.setEnabled(!dialogGridload-from-file);
    spinboxGridSpacingX.setEnabled(!dialogGridload-from-file);
    labelGridSpacingY.setEnabled(!dialogGridload-from-file);
    spinboxGridSpacingY.setEnabled(!dialogGridload-from-file);
    labelGridsizeRadius.setEnabled(!dialogGridload-from-file);
    spinboxGridsizeRadius.setEnabled(!dialogGridload-from-file);
    labelGridSpacingRadius.setEnabled(!dialogGridload-from-file);
    spinboxGridSpacingRadius.setEnabled(!dialogGridload-from-file);
    labelGridSpacingAngle.setEnabled(!dialogGridload-from-file);
    spinboxGridSpacingAngle.setEnabled(!dialogGridload-from-file);

    int visibility = 0;
    if (dialogGridtype == "Circular") {
        visibility = 1;
    }
    labelGridsizeX.visible = !visibility;
    spinboxGridsizeX.visible = !visibility;
    labelGridsizeY.visible = !visibility;
    spinboxGridsizeY.visible = !visibility;
    labelGridSpacingX.visible = !visibility;
    spinboxGridSpacingX.visible = !visibility;
    labelGridSpacingY.visible = !visibility;
    spinboxGridSpacingY.visible = !visibility;
    labelGridsizeRadius.visible = visibility;
    spinboxGridsizeRadius.visible = visibility;
    labelGridSpacingRadius.visible = visibility;
    spinboxGridSpacingRadius.visible = visibility;
    labelGridSpacingAngle.visible = visibility;
    spinboxGridSpacingAngle.visible = visibility;

    gridLayoutGridGeom = tk.GridLayout(groupboxGridGeom);
    addWidget(gridLayoutGridGeom, checkboxGridload-from-file, 0, 0, "Align Left");
    addWidget(gridLayoutGridGeom, labelGridtype, 1, 0, "Align Left");
    addWidget(gridLayoutGridGeom, comboboxGridtype, 1, 1, "Align Right");
    addWidget(gridLayoutGridGeom, checkboxGridcenter-on-origin, 2, 0, "Align Left");
    addWidget(gridLayoutGridGeom, labelGridcenter-x, 3, 0, "Align Left");
    addWidget(gridLayoutGridGeom, spinboxGridCenterX, 3, 1, "Align Right");
    addWidget(gridLayoutGridGeom, labelGridcenter-y, 4, 0, "Align Left");
    addWidget(gridLayoutGridGeom, spinboxGridcenter-y, 4, 1, "Align Right");
    addWidget(gridLayoutGridGeom, labelGridsizeX, 5, 0, "Align Left");
    addWidget(gridLayoutGridGeom, spinboxGridsizeX, 5, 1, "Align Right");
    addWidget(gridLayoutGridGeom, labelGridsizeY, 6, 0, "Align Left");
    addWidget(gridLayoutGridGeom, spinboxGridsizeY, 6, 1, "Align Right");
    addWidget(gridLayoutGridGeom, labelGridSpacingX, 7, 0, "Align Left");
    addWidget(gridLayoutGridGeom, spinboxGridSpacingX, 7, 1, "Align Right");
    addWidget(gridLayoutGridGeom, labelGridSpacingY, 8, 0, "Align Left");
    addWidget(gridLayoutGridGeom, spinboxGridSpacingY, 8, 1, "Align Right");
    addWidget(gridLayoutGridGeom, labelGridsizeRadius, 9, 0, "Align Left");
    addWidget(gridLayoutGridGeom, spinboxGridsizeRadius, 9, 1, "Align Right");
    addWidget(gridLayoutGridGeom, labelGridSpacingRadius, 10, 0, "Align Left");
    addWidget(gridLayoutGridGeom, spinboxGridSpacingRadius, 10, 1, "Align Right");
    addWidget(gridLayoutGridGeom, labelGridSpacingAngle, 11, 0, "Align Left");
    addWidget(gridLayoutGridGeom, spinboxGridSpacingAngle, 11, 1, "Align Right");
    groupboxGridGeom.setLayout(gridLayoutGridGeom);

    #Ruler Misc
    groupboxRulerMisc = tk.groupbox(translate("Ruler Misc"), widget);

    checkboxRulerShowOnLoad = tk.checkbox(translate("Initially show ruler when loading a file"), groupboxRulerMisc);
    dialog-ruler-show-on-load = ruler-show-on-load;
    checkboxRulerShowOnLoad.setChecked(dialog-ruler-show-on-load);
    #connect(checkboxRulerShowOnLoad, SIGNAL(-state-changed(int)), this, SLOT(checkboxRulerShowOnLoad-state-changed(int)));

    labelRulerMetric = tk.Label(translate("Ruler Units"), groupboxRulerMisc);
    comboboxRulerMetric = tk.combobox(groupboxRulerMisc);
    comboboxRulerMetric.add-item("Imperial", 0);
    comboboxRulerMetric.add-item("Metric", 1);
    dialog-ruler-metric = ruler-metric;
    comboboxRulerMetric.setCurrentIndex(comboboxRulerMetric.findData(dialog-ruler-metric));
    #connect(comboboxRulerMetric, SIGNAL(current-index-changed(int)), this, SLOT(comboboxRulerMetricCurrent-index-changed(int)));

    gridLayoutRulerMisc = tk.GridLayout(widget);
    gridLayoutRulerMisc.addWidget(checkboxRulerShowOnLoad, 0, 0, "Align Left");
    gridLayoutRulerMisc.addWidget(labelRulerMetric, 1, 0, "Align Left");
    gridLayoutRulerMisc.addWidget(comboboxRulerMetric, 1, 1, "Align Right");
    groupboxRulerMisc.setLayout(gridLayoutRulerMisc);

    #Ruler Color
    groupboxRulerColor = tk.groupbox(translate("Ruler Color"), widget);

    labelRulerColor = tk.Label(translate("Ruler Color"), groupboxRulerColor);
    labelRulerColor.setObjectName("labelRulerColor");
    buttonRulerColor = PushButton(translate("Choose"), groupboxRulerColor);
    buttonRulerColor.setObjectName("buttonRulerColor");
    dialog-ruler-color = ruler-color;
    preview.ruler-color = dialog-ruler-color;
    accept.ruler-color = dialog-ruler-color;
    rulerPix = Image(16,16);
    rulerPix.fill(Color(preview.ruler-color));
    buttonRulerColor.setIcon(QIcon(rulerPix));
    #connect(buttonRulerColor, SIGNAL(clicked()), this, SLOT(chooseRulerColor()));

    gridLayoutRulerColor = tk.GridLayout(widget);
    gridLayoutRulerColor.addWidget(labelRulerColor, 1, 0, "Align Left");
    gridLayoutRulerColor.addWidget(buttonRulerColor, 1, 1, "Align Right");
    groupboxRulerColor.setLayout(gridLayoutRulerColor);

    #Ruler Geometry
    groupboxRulerGeom = tk.groupbox(translate("Ruler Geometry"), widget);

    labelRulerPixelSize = tk.Label(translate("Ruler Pixel Size"), groupboxRulerGeom);
    labelRulerPixelSize.setObjectName("labelRulerPixelSize");
    spinboxRulerPixelSize = tk.Doublespinbox(groupboxRulerGeom);
    spinboxRulerPixelSize.setObjectName("spinboxRulerPixelSize");
    dialog-ruler-pixel-size = ruler-pixel-size;
    spinboxRulerPixelSize.setSingleStep(1.000);
    spinboxRulerPixelSize.setRange(20.000, 100.000);
    spinboxRulerPixelSize.setValue(dialog-ruler-pixel-size);
    #connect(spinboxRulerPixelSize, SIGNAL(-value-changed(double)), this, SLOT(spinboxRulerPixelSize-value-changed(double)));

    gridLayoutRulerGeom = tk.GridLayout(groupboxRulerGeom);
    gridLayoutRulerGeom.addWidget(labelRulerPixelSize, 0, 0, "Align Left");
    gridLayoutRulerGeom.addWidget(spinboxRulerPixelSize, 0, 1, "Align Right");
    groupboxRulerGeom.setLayout(gridLayoutRulerGeom);

    /* Widget Layout */
    vbox-layout-main = tk.VBoxLayout(widget);
    vbox-layout-main.addWidget(groupboxGridMisc);
    vbox-layout-main.addWidget(groupboxGridColor);
    vbox-layout-main.addWidget(groupboxGridGeom);
    vbox-layout-main.addWidget(groupboxRulerMisc);
    vbox-layout-main.addWidget(groupboxRulerColor);
    vbox-layout-main.addWidget(groupboxRulerGeom);
    vbox-layout-main.addStretch(1);
    widget.setLayout(vbox-layout-main);

    scroll-area = tk.scroll-area(this);
    scroll-area.setWidgetResizable(1);
    scroll-area.setWidget(widget);
    return scroll-area;
}
#endif

void
createTabOrthoPolar(void)
{
    /*
    widget = tk.Widget(this);

    #TODO: finish this

    scroll-area = tk.scroll-area(this);
    scroll-area.setWidgetResizable(1);
    scroll-area.setWidget(widget);
    return scroll-area;
    */
}

/* Originally a macro for constructing checkboxes.
 */
#if 0
void
make_checkbox(char *label, int checked, char *icon, f, x, y)
{
    c = tk.checkbox(translate(label), groupboxQSnapLoc);
    c.setChecked(checked);
    c.setIcon(load-icon(icon));
    #connect(c, SIGNAL(-state-changed(int)), this, SLOT(f(int)));
    #connect(this, SIGNAL(buttonQSnapSelectAll(int)), c, SLOT(setChecked(int)));
    #connect(this, SIGNAL(buttonQSnapClearAll(int)), c, SLOT(setChecked(int)));
    gridLayoutQSnap.addWidget(c, x, y, "Align Left");
    dialog-checked = checked;
}

void
createTabQuickSnap(void)
{
    widget = tk.Widget(this);

    #QSnap Locators
    groupboxQSnapLoc = tk.groupbox(translate("Locators Used"), widget);
    buttonQSnapSelectAll = PushButton(translate("Select All"), groupboxQSnapLoc);
    buttonQSnapClearAll = PushButton(translate("Clear All"), groupboxQSnapLoc);
    gridLayoutQSnap = tk.GridLayout(groupboxQSnapLoc);

    #connect(buttonQSnapSelectAll, SIGNAL(clicked()), this, SLOT(buttonQSnapSelectAllClicked()));
    #connect(buttonQSnapClearAll, SIGNAL(clicked()), this, SLOT(buttonQSnapClearAllClicked()));

    make-checkbox("Endpoint", tk.snap-endpoint, locator-snaptoendpoint-xpm, checkboxQSnapEndPoint-state-changed, 0, 0);
    make-checkbox("Midpoint", tk.snap-midpoint, locator-snaptomidpoint-xpm, checkboxQSnapMidPoint-state-changed, 1, 0);
    make-checkbox("Center", tk.snap-center, locator-snaptocenter-xpm, checkboxQSnapCenter-state-changed, 2, 0);
    make-checkbox("Node", tk.snap-node, locator-snaptonode-xpm, checkboxQSnapNode-state-changed, 3, 0);
    make-checkbox("Quadrant", tk.snap-quadrant, locator-snaptoquadrant-xpm, checkboxQSnapQuadrant-state-changed, 4, 0);
    make-checkbox("Intersection", tk.snap-intersection, locator-snaptointersection-xpm, checkboxQSnapIntersection-state-changed, 5, 0);
    make-checkbox("Extension", tk.snap-extension, locator-snaptoextension-xpm, checkboxQSnapExtension-state-changed, 6, 0);
    make-checkbox("Insertion", tk.snap-insertion, locator-snaptoinsert-xpm, checkboxQSnapInsertion-state-changed, 0, 1);
    make-checkbox("Perpendicular", tk.snap-perpendicular, locator-snaptoperpendicular-xpm, checkboxQSnapPerpendicular-state-changed, 1, 1);
    make-checkbox("Tangent", tk.snap-tangent, locator-snaptotangent-xpm, checkboxQSnapTangent-state-changed, 2, 1);
    make-checkbox("Nearest", tk.snap-nearest, locator-snaptonearest-xpm, checkboxQSnapNearest-state-changed, 3, 1);
    make-checkbox("Apparent Intersection", tk.snap-apparent, locator-snaptoapparentintersection-xpm, checkboxQSnapApparentIntersection-state-changed, 4, 1);
    make-checkbox("Parallel", tk.snap-parallel, locator-snaptoparallel-xpm, checkboxQSnapParallel-state-changed, 5, 1);

    gridLayoutQSnap.addWidget(buttonQSnapSelectAll, 0, 2, "Align Left");
    gridLayoutQSnap.addWidget(buttonQSnapClearAll, 1, 2, "Align Left");
    gridLayoutQSnap.setColumnStretch(2,1);
    groupboxQSnapLoc.setLayout(gridLayoutQSnap);

    #QSnap Visual Config
    groupboxQSnapVisual = tk.groupbox(translate("Visual Configuration"), widget);

    labelQSnapLocColor = tk.Label(translate("Locator Color"), groupboxQSnapVisual);
    comboboxQSnapLocColor = tk.combobox(groupboxQSnapVisual);
    addColorsTocombobox(comboboxQSnapLocColor);
    dialog-qsnap-locator-color = qsnap-locator-color;
    comboboxQSnapLocColor.setCurrentIndex(comboboxQSnapLocColor.findData(dialog-qsnap-locator-color));
    #connect(comboboxQSnapLocColor, SIGNAL(current-index-changed(int)), this, SLOT(comboboxQSnapLocatorColorCurrent-index-changed(int)));

    labelQSnapLocSize = tk.Label(translate("Locator Size"), groupboxQSnapVisual);
    sliderQSnapLocSize = tk.Slider(Qt-Horizontal, groupboxQSnapVisual);
    sliderQSnapLocSize.setRange(1,20);
    dialog-qsnap-locator-size = qsnap-locator-size;
    sliderQSnapLocSize.setValue(dialog-qsnap-locator-size);
    #connect(sliderQSnapLocSize, SIGNAL(-value-changed(int)), this, SLOT(sliderQSnapLocatorSize-value-changed(int)));

    vboxLayoutQSnapVisual = tk.VBoxLayout(groupboxQSnapVisual);
    vboxLayoutQSnapVisual.addWidget(labelQSnapLocColor);
    vboxLayoutQSnapVisual.addWidget(comboboxQSnapLocColor);
    vboxLayoutQSnapVisual.addWidget(labelQSnapLocSize);
    vboxLayoutQSnapVisual.addWidget(sliderQSnapLocSize);
    groupboxQSnapVisual.setLayout(vboxLayoutQSnapVisual);

    #QSnap Sensitivity Config
    groupboxQSnapSensitivity = tk.groupbox(translate("Sensitivity"), widget);

    labelQSnapApertureSize = tk.Label(translate("Aperture Size"), groupboxQSnapSensitivity);
    sliderQSnapApertureSize = tk.Slider(Qt-Horizontal, groupboxQSnapSensitivity);
    sliderQSnapApertureSize.setRange(1,20);
    dialog-qsnap-aperture-size = qsnap-aperture-size;
    sliderQSnapApertureSize.setValue(dialog-qsnap-aperture-size);
    #connect(sliderQSnapApertureSize, SIGNAL(-value-changed(int)), this, SLOT(sliderQSnapApertureSize-value-changed(int)));

    vboxLayoutQSnapSensitivity = tk.VBoxLayout(groupboxQSnapSensitivity);
    vboxLayoutQSnapSensitivity.addWidget(labelQSnapApertureSize);
    vboxLayoutQSnapSensitivity.addWidget(sliderQSnapApertureSize);
    groupboxQSnapSensitivity.setLayout(vboxLayoutQSnapSensitivity);

 Widget Layout
    vbox-layout-main = tk.VBoxLayout(widget);
    vbox-layout-main.addWidget(groupboxQSnapLoc);
    vbox-layout-main.addWidget(groupboxQSnapVisual);
    vbox-layout-main.addWidget(groupboxQSnapSensitivity);
    vbox-layout-main.addStretch(1);
    widget.setLayout(vbox-layout-main);

    scroll-area = tk.scroll-area(this);
    scroll-area.setWidgetResizable(1);
    scroll-area.setWidget(widget);
    return scroll-area;
}
*/

/* TODO: finish this */
void
create_tab_quick_track(void)
{
    /*
    widget = tk.Widget(this);

    scroll-area = tk.scroll-area(this);
    scroll-area.setWidgetResizable(1);
    scroll-area.setWidget(widget);
    return scroll-area;
    */
}

/* TODO: finish this */
void
create-tab-line-weight)
{
    widget = tk.Widget(this)

 Misc
    groupboxLwtMisc = tk.groupbox(translate("LineWeight Misc"), widget);

    s = (active-scene);

    checkboxShowLwt = tk.checkbox(translate("Show LineWeight"), groupboxLwtMisc);
    if s:
        dialog-lwt-show-lwt = s.property("ENABLE-LWT");
    else
        dialog-lwt-show-lwt = lwt-show-lwt;

    preview.lwt-show-lwt = dialog-lwt-show-lwt;
    checkboxShowLwt.setChecked(preview.lwt-show-lwt);
    #connect(checkboxShowLwt, SIGNAL(-state-changed(int)), this, SLOT(checkboxLwtShowLwt-state-changed(int)));

    checkboxRealRender = tk.checkbox(translate("RealRender"), groupboxLwtMisc);
    checkboxRealRender.setObjectName("checkboxRealRender");
    if s:
        dialog-lwt-real-render = s.property("ENABLE-REAL");
    else {
        dialog-lwt-real-render = lwt-real-render;

    preview.lwt-real-render = dialog-lwt-real-render;
    checkboxRealRender.setChecked(preview.lwt-real-render);
    #connect(checkboxRealRender, SIGNAL(-state-changed(int)), this, SLOT(checkboxLwtRealRender-state-changed(int)));
    checkboxRealRender.setEnabled(dialog-lwt-show-lwt);

    labelDefaultLwt = tk.Label(translate("Default weight"), groupboxLwtMisc);
    labelDefaultLwt.setEnabled(0) # TODO: remove later;
    comboboxDefaultLwt = tk.combobox(groupboxLwtMisc);
    dialog-lwt-default-lwt = lwt-default-lwt;
    # TODO: populate the combobox and set the initial value;
    comboboxDefaultLwt.add-item("".setNum(dialog-lwt-default-lwt, 'F', 2).append(" mm"), dialog-lwt-default-lwt);
    comboboxDefaultLwt.setEnabled(0); # TODO: remove later

    vboxLayoutLwtMisc = tk.VBoxLayout(groupboxLwtMisc);
    vboxLayoutLwtMisc.addWidget(checkboxShowLwt);
    vboxLayoutLwtMisc.addWidget(checkboxRealRender);
    vboxLayoutLwtMisc.addWidget(labelDefaultLwt);
    vboxLayoutLwtMisc.addWidget(comboboxDefaultLwt);
    groupboxLwtMisc.setLayout(vboxLayoutLwtMisc);

    #Widget Layout
    vbox-layout-main = tk.VBoxLayout(widget);
    vbox-layout-main.addWidget(groupboxLwtMisc);
    vbox-layout-main.addStretch(1);
    widget.setLayout(vbox-layout-main);

    scroll-area = tk.scroll-area(this);
    scroll-area.setWidgetResizable(1);
    scroll-area.setWidget(widget);
    return scroll-area;
}
*/

void
create-tab-selection(void)
{
    widget = tk.Widget(this);

 Selection Modes
    groupboxSelectionModes = tk.groupbox(translate("Modes"), widget);

    checkboxSelectionModePickFirst = tk.checkbox(translate("Allow Preselection (PickFirst)"), groupboxSelectionModes);
    dialog-selection-mode-pickfirst = selection-mode-pickfirst;
    checkboxSelectionModePickFirst.setChecked(dialog-selection-mode-pickfirst);
    checkboxSelectionModePickFirst.setChecked(1) checkboxSelectionModePickFirst.setEnabled(0); # TODO: Remove this line when Post-selection is available
    #connect(checkboxSelectionModePickFirst, SIGNAL(-state-changed(int)), this, SLOT(checkboxSelectionModePickFirst-state-changed(int)));

    checkboxSelectionModePickAdd = tk.checkbox(translate("Add to Selection (PickAdd)"), groupboxSelectionModes);
    dialog-selection-mode-pickadd = selection-mode-pickadd;
    checkboxSelectionModePickAdd.setChecked(dialog-selection-mode-pickadd);
    #connect(checkboxSelectionModePickAdd, SIGNAL(-state-changed(int)), this, SLOT(checkboxSelectionModePickAdd-state-changed(int)));

    checkboxSelectionModePickDrag = tk.checkbox(translate("Drag to Select (PickDrag)"), groupboxSelectionModes);
    dialog-selection-mode-pickdrag = selection-mode-pickdrag;
    checkboxSelectionModePickDrag.setChecked(dialog-selection-mode-pickdrag);
    checkboxSelectionModePickDrag.setChecked(0) checkboxSelectionModePickDrag.setEnabled(0) #TODO: Remove this line when this functionality is available
    #connect(checkboxSelectionModePickDrag, SIGNAL(-state-changed(int)), this, SLOT(checkboxSelectionModePickDrag-state-changed(int)));

    vboxLayoutSelectionModes = tk.VBoxLayout(groupboxSelectionModes);
    vboxLayoutSelectionModes.addWidget(checkboxSelectionModePickFirst);
    vboxLayoutSelectionModes.addWidget(checkboxSelectionModePickAdd);
    vboxLayoutSelectionModes.addWidget(checkboxSelectionModePickDrag);
    groupboxSelectionModes.setLayout(vboxLayoutSelectionModes);

    #Selection Colors
    groupboxSelectionColors = tk.groupbox(translate("Colors"), widget);

    labelCoolGripColor = tk.Label(translate("Cool Grip (Unselected)"), groupboxSelectionColors);
    comboboxCoolGripColor = tk.combobox(groupboxSelectionColors);
    addColorsTocombobox(comboboxCoolGripColor);
    dialog-selection-coolgrip-color = selection-coolgrip-color;
    comboboxCoolGripColor.setCurrentIndex(comboboxCoolGripColor.findData(dialog-selection-coolgrip-color));
    #connect(comboboxCoolGripColor, SIGNAL(current-index-changed(int)), this, SLOT(comboboxSelectionCoolGripColorCurrent-index-changed(int)));

    labelHotGripColor = tk.Label(translate("Hot Grip (Selected)"), groupboxSelectionColors);
    comboboxHotGripColor = tk.combobox(groupboxSelectionColors);
    addColorsTocombobox(comboboxHotGripColor);
    dialog-selection-hotgrip-color = selection-hotgrip-color;
    comboboxHotGripColor.setCurrentIndex(comboboxHotGripColor.findData(dialog-selection-hotgrip-color));
    #connect(comboboxHotGripColor, SIGNAL(current-index-changed(int)), this, SLOT(comboboxSelectionHotGripColorCurrent-index-changed(int)));

    vboxLayoutSelectionColors = tk.VBoxLayout(groupboxSelectionColors);
    addWidget(vboxLayoutSelectionColors, labelCoolGripColor);
    addWidget(vboxLayoutSelectionColors, comboboxCoolGripColor);
    addWidget(vboxLayoutSelectionColors, labelHotGripColor);
    addWidget(vboxLayoutSelectionColors, comboboxHotGripColor);
    groupboxSelectionColors.layout = vboxLayoutSelectionColors;

    #Selection Sizes
    groupboxSelectionSizes = tk.groupbox(translate("Sizes"), widget);

    labelSelectionGripSize = tk.Label(translate("Grip Size"), groupboxSelectionSizes);
    sliderSelectionGripSize = tk.Slider(Qt-Horizontal, groupboxSelectionSizes);
    sliderSelectionGripSize.setRange(1,20);
    dialog-selection-grip-size = selection-grip-size;
    sliderSelectionGripSize.setValue(dialog-selection-grip-size);
    #connect(sliderSelectionGripSize, SIGNAL(-value-changed(int)), this, SLOT(sliderSelectionGripSize-value-changed(int)));

    labelSelectionPickBoxSize = tk.Label(translate("Pickbox Size"), groupboxSelectionSizes);
    sliderSelectionPickBoxSize = tk.Slider(Qt-Horizontal, groupboxSelectionSizes);
    sliderSelectionPickBoxSize.setRange(1,20);
    dialog-selection-pickbox-size = selection-pickbox-size;
    sliderSelectionPickBoxSize.setValue(dialog-selection-pickbox-size);
    #connect(sliderSelectionPickBoxSize, SIGNAL(-value-changed(int)), this, SLOT(sliderSelectionPickBoxSize-value-changed(int)));

    vboxLayoutSelectionSizes = tk.VBoxLayout(groupboxSelectionSizes);
    vboxLayoutSelectionSizes.addWidget(labelSelectionGripSize);
    vboxLayoutSelectionSizes.addWidget(sliderSelectionGripSize);
    vboxLayoutSelectionSizes.addWidget(labelSelectionPickBoxSize);
    vboxLayoutSelectionSizes.addWidget(sliderSelectionPickBoxSize);
    groupboxSelectionSizes.setLayout(vboxLayoutSelectionSizes);

 Widget Layout
    vbox-layout-main = tk.VBoxLayout(widget);
    vbox-layout-main.addWidget(groupboxSelectionModes);
    vbox-layout-main.addWidget(groupboxSelectionColors);
    vbox-layout-main.addWidget(groupboxSelectionSizes);
    vbox-layout-main.addStretch(1);
    widget.setLayout(vbox-layout-main);

    scroll-area = tk.scroll-area(this);
    scroll-area.setWidgetResizable(1);
    scroll-area.setWidget(widget);
    return scroll-area;
}

/* Action functions.
 */
void
create_line_edit(int type, int user_editable)
{
    debug_message "create-tool-button({type}, {user-editable})"));
}

void
create_tool_button(int type, char *label)
{
    debug_message("create-tool-button({type}, {label})");
}
#endif

void
line_action(void)
{
    debug_message("line-action()");
}

void
distance_action(void)
{
    debug_message("distance-action()");
}

void
dolphin_action(void)
{
    debug_message("dolphin-action()");
}

void
ellipse_action(void)
{
    debug_message("ellipse-action()");
}


/* .
 */
void
delete_selected(void)
{
    /*
    item-list = gscene.selected-items();
    numSelected = item-list.size();

    for (i in range(len(item-list))) {
        if (item-list[i].data(OBJ-TYPE) != OBJ-TYPE-NULL) {
            base = item-list[i];
            if (base) {
                debug_message(".");
            }
        }
    }
    */
}

#if 0
void
create_object_list(int *list)
{
    /*copy-list = []

    for (item in list-) {
        if (!item) {
            continue
        }

        if (item.type in -Arc", "Circle", "DimLeader", "Ellipse", "Line",
                            "Path", "Point", "Polygon", "Polyline", "Rect", "Text Single) {
            copy-list += [item.copy()]

        else {
            if (item.type in -Block", "DimAligned", "DimAngular", "DimArcLength",
                            "DimDiameter", "DimLinear", "DimOrdinate", "DimRadius",
                            "Ellipse Arc", "Image", "Infinite Line", "Ray) {
                debug_message("TODO: %s" % item.type)
            }
        }
    }

    return copy-list */
}

void
move_selected(EmbVector delta)
{
    item-list = gscene.selected-items();
    num-selected = item-list.size();

    for (item in item-list) {
        if (item) {
            debug_message(" . ");
        }
    }

    /* Always clear the selection after a move. */
    /* gscene.clear-selection(); */
}

void
move_action(void)
{
    debug_message(" . ");
}

/*
 *  To make the undo history easier to manage we use a dict for
 *  keeping all the action information together.
 */
/*
 * settings = load-data("config.json");
 * icons = load-data("icons.json");
 * designs = load-data("designs.json");
 */
void
settings_dialog(int showTab)
{
    /*
    dialog = settings-dialog-action(showTab);
    dialog-mainloop();
    */
}

void
draw_icon(void)
{
    /*
    Would work on lists like this:

    "about": [
        "arc 0 0 128 128 1 -1 black 3",
        "arc 0 0 128 128 -2 2 black 3",
        "arc 20 20 108 108 40 -40 black 3"
    ];
    out = Image.new("RGB", (128, 128), (255, 255, 255));
    draw = ImageDraw.Draw(out);
    for (line in code) {
        cmd = line.split(" ");
        if (cmd[0] == "arc") {
            box = (int(cmd[1]), int(cmd[2]), int(cmd[3]), int(cmd[4]));
            start = int(cmd[5]);
            end = int(cmd[6]);
            draw.arc(box, start, end, fill=cmd[7], width=int(cmd[8]));
        }
    }
    return out;
    */
    /*  return "self function is overridden." */
}

void
check_for_updates(void)
{
    debug_message("check-for-updates()");
    /* TODO: Check website for versions, commands, etc... */
}

void
select_all(void)
{
    debug_message("selectAll()");
    gview = active-view();
    if (gview) {
        gview.selectAll();
    }

    allPath = Path();
    allPath.add-rect(gscene.scene-rect());
    gscene.setSelectionArea(allPath, "ReplaceSelection", "intersects-item-shape", transform());
}

void
design_details(void)
{
    int scene = active_scene();
    if (scene) {
        dialog = details-dialog-init(scene, self);
        dialog_exec();
    }
}

void
button_tip_of_the_day_clicked(int button)
{
    debug_message("button-tip-of-the-day-clicked()");
    if (button == 0) {
        if (general-current-tip > 0) {
            general-current-tip--;
        }
        else {
            general-current-tip = n-tips-1;
        }
        /* setText(tips[general-current-tip]); */
        return;
    }
    if (button == 1) {
        general-current-tip++
        if (general-current-tip >= n-tips) {
            general-current-tip = 0;
        }
        /* setText(tips[general-current-tip]); */
        return;
    }
    /* close dialog */
}

void
icon_resize(int icon-size)
{
    seticon-size(icon-size, icon-size);
    layer-selector.seticon-size(icon-size*4, icon-size);
    color-selector.seticon-size(icon-size, icon-size);
    linetype-selector.seticon-size(icon-size*4, icon-size);
    lineweightSelector.seticon-size(icon-size*4, icon-size);
    #set the minimum combobox width so the text is always readable
    layer-selector.set_minimum-width(icon-size*4);
    color-selector.set_minimum-width(icon-size*2);
    linetype-selector.set_minimum-width(icon-size*4);
    lineweightSelector.set_minimum-width(icon-size*4);

    #TODO: low-priority:
    #open app with icon-size set to 128. resize the icons to a smaller size.

    general_icon_size = icon_size;
}

void
active_mdi_window(void)
{
    debug_message("activemdi-window()");
    /*
    mdi-win = mdi-area.active-sub-window();
    return mdi-win;
    */
}

/* Missing function from Qt.
 *
 * Might need to be in View scope not Window.
 */
void
set_pen(void)
{
    debug_message("set_pen");
}

/* This may need to be a method of Main Window.
 */
void
clear_selection(void)
{
    debug_message("clear-selection");
    /* gscene.clear-selection();
    prompt = ""; */
}

/* This function intentionally does nothing. */
void
do_nothing(void)
{
    debug_message("do-nothing()");
}

void
close_toolbar(int action)
{
    if (action.object-name() == "toolbarclose") {
        tb = sender();
        if (tb) {
            debug_message("%s closed.", str(tb.object-name()));
            tb.hide();
        }
    }
}


void
layer-manager-action(void)
{
    debug_message("layerManager()")
    debug_message("Implement layerManager.")
    /* #LayerManager layman( self,  self)
     * #layman.exec()
}

void
layer_previous_action(void)
{
    debug_message("layerPrevious()");
    debug_message("Implement layerPrevious.");
}

void
layer_selector(void)
{
    debug_message("exit-action()");
}

void
line_weight_selector(void)
{
    debug_message("line-weight-selector-action()");
}

/* Open line type selector dialog-
 */
void
line_type_selector(void)
{
    debug_message("line-type-selector-action()");
}

/* Open color selector dialog-
 */
void
color-selector(void)
{
    debug_message("color-selector-action()");
}

void
layer_selector_action(void)
{
    debug_message("color-selector-action()");
}

void
setBackgroundColor(void)
{

}

void
setGridColor(void)
{
    
}

void
setCrossHairColor(void)
{

}

/* Whenever the code happens across a todo call,
 * write it in a log file.
 */
void
alert(char *title, char *message)
{
    if (debug-mode) {
        FILE *f;
        f = fopen("alert.txt", "a");
        fprintf(f, "%s\n%s\n", title, message);
        fclose(f);
    }
}

 Should we need to add this to an error report.

void
report-platform(void)
{
    /* print(os.uname()) */
}

void
scale-action(void)
{
    debug_message(".");
}

void
get_file_separator(void)
{
    debug_message("getFileSeparator()");
    /* return my-file-separator */
}

void
active-view(void)
{
    debug_message("active-view()");
    mdi-win = mdi-area.active-sub-window();
    if (mdi-win) {
        v = mdi-win.getView();
        return v;
    }
}


/* replace with a variable in Window? */
int
active-scene(void)
{
    debug_message("active-scene()");
    mdi-win = mdi-area.active-sub-window();
    if (mdi-win) {
        return mdi-win.getScene();
    }
}

void
update_all_view_scroll_bars(int val)
{
    windowList = mdi-area.sub-window-list();
    for (mdi-win in windowList) {
        mdi-win.showViewScrollBars(val);
    }
}

void
update_all_view_cross_hair_colors(EmbColor color)
{
    windowList = mdi-area.sub-window-list();
    for (mdi-win in windowList) {
        mdi-win.setViewCrossHairColor(color);
    }
}

void
updateAllViewBackgroundColors color)
    windowList = mdi-area.sub-window-list()
    for (mdi-win in windowList) {
        mdi-win.setViewBackgroundColor(color)
    }
}


void
checkbox setting checked)
  (dialog-setting-int[setting] = checked))


void
spinbox setting value)
    dialog-setting-double[setting] = value
}
#endif

/*
 * This is the heart of the program, we're working on 
 * replacing the Qt reliance, so these functions
 * and data represent the eventual core of the program.
 *
 * The widget system is created here, but it is built
 * on top of the SVG system created in libembroidery.
 * So a widget is an svg drawing, with a position to draw
 * it in relative to its parent. The widgets
 * form a tree rooted at the global variable called root.
 *
 * TODO: Set What's self Context Help to statusTip for now
 * so there is some infos there.
 *
 * Make custom What's self Context Help popup with more
 * descriptive help than just the status bar/tip one
 * liner (short but not real long) with a hyperlink
 * in the custom popup at the bottom to open full help file
 * description. Ex: like wxPython AGW's SuperToolTip.
 */
#if 0
void
main_window_init(void)
{
    /* To stop the garbage collector stealing our icons. */
    /* tkimg = {}

    root = tk.Tk();
    root.title(title);
    root.minsize(width, height);
    build-menu-bar();
    build-button-grid();
    undo-history = [];
    undo-history-position = 0;
    opensave-recent-list-of-files = [];
    opensave-custom-filter = "";
    current-path = "";

    num-docs = 0;
    tab-index = 0;

    lang = general-language;
    debug_message("language: %s" % lang);
    */
    /* This is a View() instance. */
    /* canvas = tk.Canvas(root, bg="#FFFFFF",
                            width=500, height=400)
    canvas.grid(row=4, column=0, columnspan=20, rowspan=2, sticky="W")
*/
    /* Use PropertyEditor
     * need to make a tk.Entry test
     */
   /* property-editor = tk.Label(root, text="Property Editor",
                                    bg="#FFFFFF")
    property-editor.grid(row=4, column=21, columnspan=5, sticky="NE")

    undo-history-editor = tk.Label(root, text="Undo History",
        bg="#FFFFFF");
    undo-history-editor.grid(row=5, column=21, columnspan=5, 
        sticky="NE");

    message-bar = tk.Label(root,
        text=time.strftime("%d %B %Y"),
        bg="#BBBBBB");
    message-bar.set_text("test");
    message-bar.grid(row=6, column=0, columnspan=20, sticky="SW");
    message-bar-tip = Tooltip(
        message-bar,
        "*Message Bar*\nShows current state of the program, useful for bug checking information.");

    return

    for (i=0; i<nFolders; i++) {
        current-path = application-folder + folders[i];
        if (!exists(current-path) {
            critical(translate("Path Error"), translate("Cannot locate: ") + current-path);
        }
    }

    if (lang == "system") {
        lang = QLocale-system().languageToString(QLocale-system().language()).lower();
    }

    /* Load translations provided by Qt
     * - self covers dialog buttons and other common things. */
    translatorQt = ""
    translatorQt.load("" + QLocale-system().name(), QLibraryInfo-location(QLibraryInfo-TranslationsPath)) */
    /* TODO: ensure self always loads, ship a copy of self with the app. */
    /* qApp.installTranslator(translatorQt) */

 Selectors
    layer-selector = ComboBox();
    color-selector = ComboBox();
    linetype-selector = ComboBox();
    lineweightSelector = ComboBox();
    textFontSelector = FontComboBox();
    text-size-selector = ComboBox();

    (define shift-key-pressed-state 0);

    set_windowIcon(app.png);
 Require Minimum WVGA
    set_minimum-size(800, 480);

    load-formats();

 create the mdi-area
    vbox = Frame(void);
    layout = VBoxLayout(vbox);
    layout.setContentsMargins(Margins());
    vbox.setFrameStyle(Frame-StyledPanel | Frame-Sunken);
    mdi-area = mdi-area(vbox);
    mdi-area.useBackgroundLogo(general-mdi-bg-use-logo);
    mdi-area.useBackgroundTexture(general-mdi-bg-use-texture);
    mdi-area.useBackgroundColor(general-mdi-bg-use-color);
    mdi-area.setBackgroundLogo(general-mdi-bg-logo);
    mdi-area.setBackgroundTexture(general-mdi-bg-texture);
    mdi-area.setBackgroundColor(Color(general-mdi-bg-color));
    mdi-area.setViewMode("TabbedView");
    mdi-area.setHorizontalScrollBarPolicy("ScrollBarAsNeeded");
    mdi-area.setVerticalScrollBarPolicy("ScrollBarAsNeeded");
    mdi-area.setActivationOrder("ActivationHistoryOrder");
    layout.addWidget(mdi-area);
    setCentralWidget(vbox);

    setDockOptions(QAnimatedDocks | QAllowTabbedDocks | QVerticalTabs);
 TODO: Load these from settings
    tabifyDockWidget(dockPropEdit, dockUndoEdit);
 TODO: load this from settings

    statusbar = StatusBar(root);
    setStatusBar(statusbar);

    for (i=0 i<action-list.keys(void) i++) {
        Icon = action-list[i].icon;
        ACTION = Action(Icon, action-list[i].menu-name, self);

        if (len(action-list[i].shortcut)>0) {
            ACTION.setShortcut(QKeySequence(action-list[i].shortcut));
        }

        ACTION.set_status-tip(action-list[i].description);
        ACTION.set_object-name(action-list[i].abbreviation);
        ACTION.setWhatsself(action-list[i].description);

        connect(ACTION, SIGNAL(triggered()), self, SLOT(actions()));
        action-hash.insert(i, ACTION);
    }

    action-hash.value("window-close").setEnabled(n-docs > 0);
    action-hash.value("design-details").setEnabled(n-docs > 0);

    menu-FILE.add-menu(menu[RECENT-MENU]);
    /* Do not allow the Recent Menu to be torn off.
     * It's a pain in the ass to maintain.
     */
    menu[RECENT-MENU].set_tear-off-enabled(0);

    debug_message("createWindowMenu()");
    menu-bar().add-menu(menu-WINDOW);
    connect(menu-WINDOW, SIGNAL(aboutToShow()), self, SLOT(window-menu-about-to-show()));
    /* Do not allow the Window Menu to be torn off.
     * It's a pain in the ass to maintain.
     */
    menu-WINDOW.set_tear-off-enabled(0);

    for i in range(N-TOOLBARS) {
        message = "creating %s\n" % toolbar-label[i];
        debug_message(message);

        toolbar[i].set_object-name(toolbar-label[i]);

        for (j in toolbars[i]) {
            if toolbars[i][j] >= 0:
                toolbar[i].add-action(action-hash.value(toolbars[i][j]));
            else {
                toolbar[i].add-separator();
            }
        }
        /*  connect(toolbar[i], SIGNAL(topLevelChanged(int)), self, SLOT(floating-changed-toolbar(int)));
    }

    debug_message("createLayerToolbar()");

    toolbar-LAYER.set_object-name("toolbarLayer");
    toolbar-LAYER.add-action(action-hash.value("make-layer-current"));
    toolbar-LAYER.add-action(action-hash.value("layers"));

    #TODO: Create layer pixmaps by concatenating several icons
    layer-selector.add-item("linetypebylayer.png", "0");
    layer-selector.add-item("linetypebylayer.png", "1");
    layer-selector.add-item("linetypebylayer.png", "2");
    layer-selector.add-item("linetypebylayer.png", "3");
    layer-selector.add-item("linetypebylayer.png", "4");
    layer-selector.add-item("linetypebylayer.png", "5");
    layer-selector.add-item("linetypebylayer.png", "6");
    layer-selector.add-item("linetypebylayer.png", "7");
    layer-selector.add-item("linetypebylayer.png", "8");
    layer-selector.add-item("linetypebylayer.png", "9");
    toolbar-LAYER.addWidget(layer-selector);
    #connect(layer-selector, SIGNAL(currentIndexChanged(int)), self, SLOT(layer-selectorIndexChanged(int)));

    toolbar-LAYER.add-action(action-hash.value("layer-previous"));

    #connect(toolbar-LAYER, SIGNAL(topLevelChanged(int)), self, SLOT(floating-changed-toolbar(int)));

    debug_message("createPropertiesToolbar()");

    toolbar-PROPERTIES.set_object-name("toolbar-properties");

    color-selector.set_focusProxy(menu-FILE);
 NOTE: Qt4.7 wont load icons without an extension...
    color-selector.add-item("colorbylayer.png", "ByLayer");
    color-selector.add-item("colorbyblock.png", "ByBlock");
    color-selector.add-item("colorred.png", translate("Red"), (255, 0, 0));
    color-selector.add-item("coloryellow.png", translate("Yellow"), (255,255, 0));
    color-selector.add-item("colorgreen.png", translate("Green"), (0, 255, 0));
    color-selector.add-item("colorcyan.png", translate("Cyan"), (0,255,255));
    color-selector.add-item("colorblue.png", translate("Blue"), (0, 0,255));
    color-selector.add-item("colormagenta.png", translate("Magenta"), (255, 0,255));
    color-selector.add-item("colorwhite.png", translate("White"), (255,255,255));
    color-selector.add-item("colorother.png", translate("Other..."))
    toolbar-PROPERTIES.addWidget(color-selector)
    #connect(color-selector, SIGNAL(currentIndexChanged(int)), self, SLOT(color-selectorIndexChanged(int)))

    toolbar-PROPERTIES.add-separator()
    linetype-selector.set_focusProxy(menu-FILE)
    linetype-selector.add-item("linetypebylayer.png", "ByLayer")
    linetype-selector.add-item("linetypebyblock.png", "ByBlock")
    linetype-selector.add-item("linetypecontinuous.png", "Continuous")
    linetype-selector.add-item("linetypehidden.png", "Hidden")
    linetype-selector.add-item("linetypecenter.png", "Center")
    linetype-selector.add-item("linetypeother.png", "Other...")
    toolbar-PROPERTIES.addWidget(linetype-selector)
    #connect(linetype-selector, SIGNAL(currentIndexChanged(int)), self, SLOT(linetype-selectorIndexChanged(int)))

    toolbar-PROPERTIES.add-separator()
    lineweightSelector.set_focusProxy(menu-FILE)
    #NOTE: Qt4.7 wont load icons without an extension...
    #TODO: Thread weight is weird. See http://en.wikipedia.org/wiki/Thread-(yarn)#Weight
    for line in thread-weights:
        lineweightSelector.add-item(line[0], line[1], line[2])
    lineweightSelector.setMinimumContentsLength(8)
    #Prevent dropdown text readability being squish...d.
    toolbar-PROPERTIES.addWidget(lineweightSelector)
    #connect(lineweightSelector, SIGNAL(currentIndexChanged(int)), self, SLOT(lineweightSelectorIndexChanged(int)))

    #connect(toolbar-PROPERTIES, SIGNAL(topLevelChanged()), self, SLOT(floating-changed-toolbar()))

    debug_message("createTextToolbar()")
    toolbar-TEXT.set_object-name("toolbarText")
    toolbar-TEXT.addWidget(textFontSelector)
    textFontSelector.setCurrentFont(Font(text-font))
    #connect(textFontSelector, SIGNAL(currentFontChanged()), self, SLOT(textFontSelectorCurrentFontChanged()))

    #TODO: SEGFAULTING FOR SOME REASON
    toolbar-TEXT.add-action(action-hash.value("text-bold"))
    action-hash.value("text-bold").set_checked(text-style-bold)
    toolbar-TEXT.add-action(action-hash.value("text-italic"))
    action-hash.value("text-italic").set_checked(text-style-italic)
    toolbar-TEXT.add-action(action-hash.value("text-underline"))
    action-hash.value("text-underline").set_checked(text-style-underline)
    toolbar-TEXT.add-action(action-hash.value("text-strikeout"))
    action-hash.value("text-strikeout").set_checked(text-style-strikeout)
    toolbar-TEXT.add-action(action-hash.value("text-overline"))
    action-hash.value("text-overline").set_checked(text-style-overline)

    text-size-selector.set_focusProxy(menu-FILE)
    sizes = [6, 8, 9, 10, 11, 12, 14, 18, 24, 30, 36, 48, 60, 72]
    for size in sizes:
        text-size-selector.add-item(str(size)+" pt", size)
    setTextSize(text-size)
    toolbar-TEXT.addWidget(text-size-selector)
    #connect(text-size-selector, SIGNAL(currentIndexChanged(int)), self, SLOT(text-size-selectorIndexChanged(int)))

    #connect(toolbar-TEXT, SIGNAL(topLevelChanged(int)), self, SLOT(floating-changed-toolbar(int)))

    #Horizontal
    toolbar-VIEW.set_orientation("Horizontal")
    toolbar-ZOOM.set_orientation("Horizontal")
    toolbar-LAYER.set_orientation("Horizontal")
    toolbar-PROPERTIES.set_orientation("Horizontal")
    toolbar-TEXT.set_orientation("Horizontal")
    #Top
    add-toolbar-break("TopToolBarArea")
    add-toolbar("TopToolBarArea", toolbar-FILE)
    add-toolbar("Top toolbar area", toolbar-EDIT)
    add-toolbar("Top toolbar area", toolbar-HELP)
    add-toolbar("Top toolbar area", toolbar-ICON)
    add-toolbar-break("TopToolBarArea")
    add-toolbar("Top toolbar area", toolbar-ZOOM)
    add-toolbar("Top toolbar area", toolbar-PAN)
    add-toolbar("Top toolbar area", toolbar-VIEW)
    add-toolbar-break("TopToolBarArea")
    add-toolbar("Top toolbar area", toolbar-LAYER)
    add-toolbar("Top toolbar area", toolbar-PROPERTIES)
    add-toolbar-break("TopToolBarArea")
    add-toolbar("Top toolbar area", toolbar-TEXT)

    #zoomToolBar.setToolButtonStyle("ToolButtonTextOnly")

    icon-resize(get-int(main-window, "general-icon-size"))
    update-menu-toolbar-statusbar()

    #Show date in statusbar after it has been updated
    #TODO: Switch to ISO dates.

    date = time.currentDate()
    datestr = date.toString("MMMM d, yyyy")
    statusbar.showMessage(datestr)

    showNormal()

    if general-tip-of-the-day:
        tip-of-the-day-action() */
}

void
read-settings(void)
{
    debug_message("Reading settings...")

 This file needs to be read from the users home directory to ensure it is writable.
    pos = Vector(window-x, window-y)
    size = (window-width, window-height)

    layoutState = settings-file.value("LayoutState").toByteArray()
    if ! restoreState(layoutState) {
        debug_message("LayoutState NOT restored! Setting Default Layout...")
        #someToolBar.setVisible(1)

    settings = load-data("config.json")

 Sanitise data here
    window-x = clamp(0, window-x, 1000)
    window-y = clamp(0, window-y, 1000)

    move(pos)
    resize(size) */
})


void
rotate-action)
  debug_message("TODO"))


void
rotate-selected pos rot)
    item-list = gscene.selected-items()
    num-selected = item-list.size()
    for (item in item-list) {
        if (item) {
            debug_message(".")
        }
    }
 Always clear the selection after a rotate.
    gscene.clear-selection()
}


void
mirror-selected point1 point2)
    item-list = gscene.selected-items()
    num-selected = item-list.size()
    for (item in item-list) {
        if (item) {
            debug_message(".")
        }
    }
 Always clear the selection after a mirror.
    gscene.clear-selection()
}

void
scale-selected(EmbVector v, double factor)
    /* item-list = gscene.selected-items()
    num-selected = item-list.size()
    for (item in item-list) {
        if (item) {
            debug_message(".")
        }
    } */
    /*  Always clear the selection after a scale. */
    /* gscene.clear-selection() */
}

static int
num-selected(void)
    /* return gscene.selected-items().size() */
    return 0
}

void
show-scroll-bars(int val)
    if (val) {
        /* set_horizontal-scroll-bar-policy("ScrollBarAlwaysOn")
        set_vertical-scroll-bar-policy("ScrollBarAlwaysOn") */
    }
    else {
        /* set_horizontal-scroll-bar-policy("ScrollBarAlwaysOff")
        set_vertical-scroll-bar-policy("ScrollBarAlwaysOff") */
    }
}

void
set_cross-hair-color(MdiArea *mdi-area, EmbColor color)
    /* crosshair-color = color
    gscene.set_property("VIEW-COLOR-CROSSHAIR", color)
    scene_update()
    */
}

void
set_background-color(MdiArea *mdi-area, EmbColor color)
    /* set_background-brush(Color(color))
    gscene.set_property("VIEW-COLOR-BACKGROUND", color)
    scene_update()
    */
}

void
set_select-box-colors(
    MdiArea *mdi_area,
    EmbColor color_left,
    EmbColor fill_left,
    EmbColor color_right,
    EmbColor fill_right,
    double alpha)
{
    /* select-box.set_colors(color-left, fill-left, color-right, fill-right, alpha) */
}

void
update-all-view-select-box-colors(
    EmbColor colorL, EmbColor fillL,
    EmbColor colorR, EmbColor fillR,
    double alpha)
{
    /* windowList = mdi-area.sub-window-list()
    for (mdi-win in windowList) {
        mdi-win.setViewSelectBoxColors(colorL, fillL, colorR, fillR, alpha)
    } */
}
#endif

void
update_all_view_grid_colors(EmbColor color)
{
    /*
    windowList = mdi-area.sub-window-list();
    for (mdi-win in windowList) {
        mdi-win.setViewGridColor(color);
    }
    */
}

void
update_all_view_ruler_colors(EmbColor color)
{
    /*
    windowList = mdi-area.sub-window-list();
    for (mdi-win in windowList) {
        mdi-win.set_view-ruler-color(color);
    }
    */
}

void
update_pick_add_mode(int val)
{
    /*
    selection-mode-pickadd = val;
    dockPropEdit.update-pick-add-mode-button(val);
    */
}

void
pick_add_mode_toggled(void)
{
    /*
    val = !selection-mode-pickadd
    updatePickAddMode(val)
    */
}

#if 0
void
layer-selector-index-changed(int index)
{
    printf("layer-selectorIndexChanged(%d)", index)
}


/* Color selector index changed.
 */
void
color-selector-index-changed(int index)
    printf("color-selectorIndexChanged(%d)", index)
    /*
    comboBox = sender()
    newColor = Color()
    if comboBox:
        ok = 0
        #TODO: Handle ByLayer and ByBlock and Other...
        newColor, ok = comboBox.itemData(index).toUInt()
        if ! ok:
            warning(translate("Color Selector Conversion Error"), translate("<b>An error has occurred while changing colors.</b>"))

    else {
        warning(translate("Color Selector Pointer Error"), translate("<b>An error has occurred while changing colors.</b>"))

    mdi-win = mdi-area.active-sub-window()
    if (mdi-win) {
        mdi-win.currentColorChanged(newColor)
    }
}

void
linetype-selector-index-changed(int index)
{
    printf("linetype-selectorIndexChanged(%d)", index)
    actuator(ACTION-DO-NOTHING)
}

void
lineweight-selector-index-changed(int index)
{
    printf("lineweightSelectorIndexChanged(%d)", index)
    actuator(ACTION-DO-NOTHING)
}

/* Text font selector current font changed.
 */
void
text-fontSelectorCurrentFontChanged(char *font)
   /*  debug_message("textFontSelectorCurrentFontChanged()")
    textFontSelector.setCurrentFont(Font(font.family()))
    text-font = font.family().toLocal8Bit().constData() */
}

/* TODO: check that the toReal() conversion is ok.
 */
void
text-size-selectorIndexChanged(int index)
    /* debug_message("text-size-selectorIndexChanged(%d)", index)
    text-style.size = abs(text-size-selector.itemData(index).toReal()) */
}

void
text-font(void)
    /*return text-font */
}

void
setTextSize(int num)
    int index
    /*
    text-style.size = abs(num)
    index = text-size-selector.find-text("Custom", "MatchContains")
    if (index != -1) {
        text-size-selector.remove-item(index)
    }
    text-size-selector.add-item("Custom " + "".set_num(num, 'f', 2) + " pt", num)
    index = text-size-selector.find-text("Custom", "MatchContains")
    if (index != -1) {
        text-size-selector.setCurrentIndex(index)
    }*/
}

static int
getCurrentLayer(void)
    debug_message("getCurrentLayer")
    if (window == NULL) {
        debug_message("called without window initialised.")
        return -2
    }
    /*
    debug_message((char*)title)
    mdi-win = mdi-area.active-sub-window()
    if (mdi-win) {
        return mdi-win.getCurrentLayer()
    }
    */
    return 0
}

/* TODO: return color ByLayer.
 */
static int
get-current-color(void)
    debug_message("get-current-color")
    if (window == NULL) {
        debug_message("called without window initialised.")
        return -2
    }
    /*
    debug_message((char*)title)
    mdi-win = mdi-area.active-sub-window()
    if mdi-win:
        return mdi-win.get-current-color()
    */
    return 0
}

int
get_current_line_type(void)
{
    debug_message("get-current-line-type");
    if (window == NULL) {
        debug_message("called without window initialised.");
        return -2;
    }
    /*
    debug_message((char*)title);
    mdi-win = mdi-area.active-sub-window();
    if (mdi-win) {
        return mdi-win.get-current-line-type();
    }
    */
    return OBJ_LWT_BYLAYER;
}

int
get_current_line_weight(void)
{
    debug_message("get-current-line-weight");
    if (window == NULL) {
        debug_message("called without window initialised.");
        return -2;
    }

    debug_message(title);
    mdi-win = mdi-area.active-sub-window();
    if (mdi-win) {
        return mdi-win.get-current-line-weight();
    }

    return OBJ-LWT-BYLAYER;
}

void
calculate-angle(EmbVector point1, EmbVector point2)
{
    return Line(x1, -y1, x2, -y2).angle();
}

void
calculate-distance(EmbVector point1, EmbVector point2)
{
    return Line(x1, y1, x2, y2).length();
}

void
fill-menu(int menu-id)
{
    debug_message("MainWindow creating %s", menu-label[menu-id]);
    menu-bar().add-menu(menu[menu-id]);
    for (menu in menus[menu-id]) {
        if (menus[menu-id][i] >= 0) {
            menu[menu-id].add-action(action-hash.value(menus[menu-id][i]));
        }
        else {
            menu[menu-id].add-separator();
        }
    }
}

/*
 * This is currently causing a bug and is going to be replaced
 * with a libembroidery function.
 */
static double
native-perpendicular-distance(void)
{
    EmbLine line;
    EmbVector norm;
    /*
    line = Line(x1, y1, x2, y2);
    norm = line.normal();
    delta.x = point.x-x1;
    delta.y = point.y-y1;
    norm.translate(delta);
    iPoint = norm.intersects(line);
    return Line(point, iPoint).length();
    */
}

void
recent-menu-about-to-show(void)
{
    debug_message("recentMenuAboutToShow()");
    /*
    menu[RECENT-MENU].clear();

    recent-file-info = "";
    recent-value = "";
    for (i=0; i<len(opensave-recent-list-of-files); i++) {
        /* If less than the max amount of entries add to menu */
        if (i < opensave-recent-max-files) {
            recent-file-info = FileInfo(opensave-recent-list-of-files.at(i))
            if recent-file-info.exists() and valid-file-format(recent-file-info.fileName()) {
                recent-value.set_num(i+1)
                rAction = 0
                if recent-value.toInt() >= 1 and recent-value.toInt() <= 9) {
                    rAction = Action("&" + recent-value + " " + recent-file-info.fileName(), self);
                }
                elif recent-value.toInt() == 10) {
                    rAction = Action("1&0 "                  + recent-file-info.fileName(), self);
                }
                else {
                    rAction = Action(recent-value + " " + recent-file-info.fileName(), self);
                }
                rAction.set_checkable(0);
                rAction.set_data(opensave-recent-list-of-files.at(i));
                menu[RECENT-MENU].add-action(rAction);
                #connect(rAction, SIGNAL(triggered()), self, SLOT(openrecentfile()));
            }
        }
    }

    /* Ensure the list only has max amount of entries */
    while (opensave-recent-list-of-files.size() > opensave-recent-max-files) {
        opensave-recent-list-of-files.removeLast();
    }
}

void
window-menu-about-to-show)
{
    debug_message("window-menu-about-to-show()");

    menu-WINDOW.clear();
    menu-WINDOW.add-action(action-hash.value("window-close"));
    menu-WINDOW.add-action(action-hash.value("window-close-all"));
    menu-WINDOW.add-separator();
    menu-WINDOW.add-action(action-hash.value("window-cascade"));
    menu-WINDOW.add-action(action-hash.value("window-tile"));
    menu-WINDOW.add-separator();
    menu-WINDOW.add-action(action-hash.value("window-next"));
    menu-WINDOW.add-action(action-hash.value("window-previous"));

    menu-WINDOW.add-separator();
    windows = mdi-area.sub-window-list();
    for (i=0; i<len(windows); i++) {
        an-action = Action(windows[i].window-title(), self);
        an-action.set_checkable(1);
        an-action.set_data(i);
        menu-WINDOW.add-action(an-action);
        #connect(an-action, SIGNAL(toggled(int)), self, SLOT(windowMenuActivated(int)));
        an-action.set_checked(mdi-area.active-sub-window() == windows[i]);
    }
}

void
window-menu-activated(int *checked)
{
    int a-sender;
    debug_message("windowMenuActivated()");
    a-sender = sender();
    if (!a-sender) {
        return;
    }
    w = mdi-area.sub-window-list().at[a-sender.data().toInt()];
    if (w and checked) {
        w.set_focus();
    }
}

void
close_event(event)
{
    debug_message("MdiWindow closeEvent()");
    mdi-area.close-all-sub-windows();
    write-settings();
    event.accept();

    sendCloseMdiWin();
}

void
on_close_window(void)
{
    debug_message("onCloseWindow()");
    mdi-win = mdi-area.active-sub-window();
    if (mdi-win) {
        onClosemdi-win(mdi-win);
    }
}

void
on-close-mdi-win(void)
{
    int keep-maximized;
    debug_message("onClosemdi-win()");
    n-docs--;
    keep-maximized = 0;
    if (the-mdi-win) {
        keep-maximized = the-mdi-win.is-maximized();
    }

    mdi-area.remove-sub-window(the-mdi-win);
    the-mdi-win.delete-later();

    update-menu-toolbar-statusbar();
    window-menu-about-to-show();

    if (keep-maximized) {
        mdi-win = mdi-area.active-sub-window();
        if (mdi-win) {
            mdi-win.show-maximized();
        }
    }
}

void
resize-event e)
{
    debug_message("resizeEvent()");
    /* resizeEvent(e);
    statusBar().setSizeGripEnabled(!isMaximized()); */
}

void
update-menu-toolbar-statusbar)
{
    debug_message("updateMenuToolbarStatusbar()")
    /*
    action-enabled[ACTION-PRINT] = n-docs
    action-enabled[ACTION-WINDOW-CLOSE] = n-docs
    action-enabled[ACTION-DESIGN-DETAILS] = n-docs
    */

    if (n-docs) {
        /*
        #Toolbars
        for key in toolbar.keys() {
            toolbar[key].show()

        #DockWidgets
        dock-prop-edit.show()
        dock-undo-edit.show()

        #Menus
        menu-bar().clear()
        menu-bar().add-menu(menu-FILE)
        menu-bar().add-menu(menu-EDIT)
        menu-bar().add-menu(menu-VIEW)

        for (menu- in menuHash) {
            menu-bar().add-menu(menu-)
        }

        menu-bar().add-menu(menu-SETTINGS)
        menu-bar().add-menu(menu-WINDOW)
        menu-bar().add-menu(menu-HELP)

        menu-WINDOW.setEnabled(1)

        #Statusbar
        statusbar.clear-message()
        status-bar-mouse-coord.show()
        status-bar-SNAP.show()
        status-bar-GRID.show()
        status-bar-RULER.show()
        status-bar-ORTHO.show()
        status-bar-POLAR.show()
        status-bar-QSNAP.show()
        status-bar-QTRACK.show()
        status-bar-LWT.show()
        */
    }
    else {
        int i
        int toolbars-to-hide[] = {
            TOOLBAR-VIEW,
            TOOLBAR-ZOOM,
            TOOLBAR-PAN,
            TOOLBAR-ICON,
            TOOLBAR-HELP,
            TOOLBAR-LAYER,
            TOOLBAR-TEXT,
            TOOLBAR-PROPERTIES,
            -1
        }

        for (i=0 toolbars-to-hide[i]>=0 i++) {
            hide-toolbar(toolbars-to-hide[i]);
        }

        /* DockWidgets */
        dockPropEdit.hide();
        dockUndoEdit.hide();

        /* Menus */
        menu-bar().clear();
        menu-bar().add-menu(menu-FILE);
        menu-bar().add-menu(menu-EDIT);
        menu-bar().add-menu(menu-MENU);
        menu-bar().add-menu(menu-WINDOW);
        menu-bar().add-menu(menu-HELP);

        menu-WINDOW.setEnabled(0);

        /* Statusbar */
        statusbar.clear-message()
        status-bar-mouse-coord.hide()
        for (k=0 k<status-bar-n-keys k++) {
            status-bar[k].hide();
        }
    }
}

void
load-formats(void)
{
    int i, curFormat;
    char stable, unstable;
    char supported-readers[MAX-STRING-LENGTH];
    char individual-readers[MAX-STRING-LENGTH];
    char supported-writers[MAX-STRING-LENGTH];
    char individual-writers[MAX-STRING-LENGTH];

    supported-readers[0] = 0;
    individual-readers[0] = 0;
    supported-writers[0] = 0;
    individual-writers[0] = 0;

    strcat(supported-readers, "All Supported Files (");
    strcat(individual-readers, "All Files (*)");
    strcat(supported-writers, "All Supported Files (");
    strcat(individual-writers, "All Files (*)");

    /*
    supported-str = "";
    individual-str = "";

    #TODO: Stable Only (Settings Option)
    #stable = 'S';
    #unstable = 'S';

    #Stable + Unstable
    stable = 'S';
    unstable = 'U';

    */
    curFormat = 0
    for (i=0 i<numberOfFormats i++) {
        /*
        extension = format-table[i].extension;
        description = format-table[i].description;
        readerState = format-table[i].reader-state;
        writerState = format-table[i].writer-state;

        upper-ext = extension.upper()
        supported-str = "*" + upper-ext + " "
        individual-str = upper-ext.replace(".", "") + " - " + description + " (*" + extension + ")"
        if(readerState == stable or readerState == unstable) {
            #Exclude color file formats from open dialogs
            if(upper-ext != "COL" and upper-ext != "EDR" and upper-ext != "INF" and upper-ext != "RGB") {
                supported-readers.append(supported-str)
                individual-readers.append(individual-str)
            }
        }

        if (writerState == stable or writerState == unstable) {
            supported-writers.append(supported-str)
            individual-writers.append(individual-str)
        }
        */
    }

    strcat(supported-readers, ")")
    strcat(supported-writers, ")")

    /*
    format-filter-open = supported-readers + individual-readers
    format-filter-save = supported-writers + individual-writers
    */
    /* TODO: Fixup custom filter. */
    /*custom = custom-filter
    if custom.contains("supported", "CaseInsensitive") { */
        /* This will hide it. */ /*
        custom = ""
    }
    elif ! custom.contains("*", "CaseInsensitive") { */
        /* This will hide it. */ /*
        custom = ""
    }
    else {
        custom = "Custom Filter(" + custom + ")"
    }

    return translate(custom + supported + all) */
}

void
floating-changed-toolbar(int isFloating)
    /* tb = sender()
    if (tb) {
        if (isFloating) { */
            /* #TODO: Determine best suited close button on various platforms.
            #Style-SP-DockWidgetCloseButton
            #Style-SP-TitleBarCloseButton
            #Style-SP-DialogCloseButton */
/*
            ACTION = Action(tb.style().standard-icon("Style-SP-DialogCloseButton"), "Close", self)
            ACTION.set_status-tip("Close the " + tb.window-title() + " Toolbar")
            ACTION.set_object-name("toolbarclose")
            tb.add-action(ACTION)
            #connect(tb, SIGNAL(actionTriggered()), self, SLOT(close-toolbar()))
        }
        else {
            for (action in tb.actions()) {
                if (action.object-name() == "toolbarclose") {
                    tb.remove-action(action)
                    #disconnect(tb, SIGNAL(actionTriggered()), self, SLOT(close-toolbar()))
                    del action
                }
            }
        }
    } */
}


/*
 * Build the classic UI dropdown menus using the layout defined
 * on file in our 'layout.json'.
 */
void
build-menu-bar(void)
    debug_message("build-menu-bar")
    /*
    menu-layout = menu-bar
    menu-bar = tk.Menu(root)
    for (menu in menu-layout-order) {
        debug_message(menu)
        menu- = tk.Menu(menu-bar, tearoff=0)
        for (item in menu-layout[menu]-order) {
            debug_message(item)
            cmd = menu-layout[menu][item]
            menu-.add-command(
                label=translate(item),
                command=lambda: actuator(cmd)
            )
        }
        menu-bar.add-cascade(label=translate(menu), menu=menu-)
    }
    root.config(menu=menu-bar)
    */
}

/*
 * Create the toolbars in the order given by the "order" list.
 */
void
build-button-grid(void)
    debug_message("build-buttongrid")
    /* button-layout = toolbar
    for (toolbar in button-layout-order) {
        debug_message(toolbar)
        for (button in button-layout[toolbar]-order) {
            debug_message(button)
            B = button-layout[toolbar][button]
            tkimg[button] = load-image(B-icon)
            button = tk.Button(
                root,
                command=lambda: actuator(B-command),
                image=tkimg[button]
            )
            button.grid(row=B-row, column=B-column)
        }
    }
    */
}

void
application-event(SDL-Event event)
    /*
    if (event.type() == FileOpen) {
        open-files-selected(event.file())
        return 1
    }
    Fall through
    return application-event(event)
    */
}


void
mdi-area-init(MdiArea *area)
    area->tabs-closeable = 1
    area->use-logo = 0
    area->use-texture = 0
    area->use-color = 0
    area->bg-logo = 0
    area->bg-texture = 0
    area->bg-color = 0
}

void
mdi-area-use-background-logo(MdiArea *area, int use)
{
    area->use-logo = use;
    force-repaint();
}

void
mdi-area-use-background-texture(MdiArea *area, int use)
{
    area->use-texture = use;
    force-repaint();
}

void
mdi-area-use-background-color(MdiArea *area, int use)
{
    area->use-color = use;
    force-repaint();
}

void
mdi-area-set_background-logo(MdiArea *area, char *file-name)
{
    bg-logo.load(file-name);
    force-repaint();
}

void
mdi-area-set_background-texture(MdiArea *area, char *file-name)
{
    bg-texture.load(file-name);
    force-repaint();
}

void
mdi-area-set_background-color(MdiArea *area, EmbColor color)
{
    if (!color.is-valid()) {
        bg-color = background().color()
    }
    else {
        bg-color = color
    }

    force-repaint();
}

void
mdi-area-mouse-double-click-event(MdiArea *area, SDL-Event e)
    /*
    mw.open-file-action()
    */
}

void
mdi-area-paint-event(MdiArea *area, SDL-Event e)
    /* vport = viewport()
    rect = vport.rect()

    painter = Painter(vport)
    painter.setRenderHint(QPainter-SmoothPixmapTransform) */

    /* Always fill with a solid color first. */
    /* if (use-color) {
        painter.fillRect(rect, bg-color)
    }
    else {
        painter.fillRect(rect, background())
    } */

    /* Then overlay the texture. */
    /* if (use-texture) {
        bgBrush = QBrush(bg-texture)
        painter.fillRect(rect, bgBrush)
    } */

    /* Overlay the logo last. */
    /* if (use-logo) { */
        /* Center the pixmap */
        /* dx = (rect.width()-bg-logo.width())/2
        dy = (rect.height()-bg-logo.height())/2
        painter.drawPixmap(dx, dy, bg-logo.width(), bg-logo.height(), bg-logo)
    } */
}

void
mdi-area-cascade(void)
    /*
    cascadeSubWindows()
    zoom-extents-actionAllSubWindows()
    */
}

void
mdi-area-tile)
    tileSubWindows()
    zoom-extents-actionAllSubWindows()
}

void
mdi-area-zoom-extents-actionAllSubWindows)
    for (window in subWindowList()) {
        if (window) {
            v = window.getView()
            if (v) {
                v.recalculateLimits()
                v.zoom-extents-action()
            }
        }
    }
}

 HACK: Take that QMdiArea!
void
mdi-area-force-repaint)
    hack = size()
    resize(hack + QSize(1,1))
    resize(hack)
}

 NOTE: This function should be used to interpret various
 object types and save them as polylines for stitchOnly formats.

void
to-polyline(
    EmbPattern *pattern,
    EmbVector obj-pos,
    /*
    obj-path,
    layer,
    */
    EmbColor color,
    int line-type,
    double line-weight)
    /*
    startX = obj-pos.x()
    startY = obj-pos.y()
    point-list = []
    for i in range(obj-path.element-count()) {
        element = obj-path.element-at(i)
        a = Vector(0.0, 0.0)
        a.point.x = element.x + startX
        a.point.y = -(element.y + startY)
        point-list += [a]
    }

    poly-object = Polyline()
    poly-object.point-list = point-list
    poly-object.color = color
    poly-object.line-type = "solid"
    pattern.add-polyline(poly-object)
}


/*
class MdiWindow()*/
    /*
    MdiWindow(theIndex, mw, parent, wflags)
    ~MdiWindow()

    virtual QSize  sizeHint() const

    static void getCurrentFile()
    static void getShortCurrentFile()
    static void getView()
    static void getScene()
    static void getCurrentLayer()
    static void getCurrentColor()
    static void getCurrentline-type()
    static void getCurrentline-weight()
    static void setCurrentLayer(layer)
    static void set_current-color(color)
    static void setCurrentline-type(line-type)
    static void setCurrentline-weight(line-weight)
    static void design-details-action()
    static void sendCloseMdiWin(MdiWindow*)

    mwdow*    mw
    QGraphicsScene*    gscene
    QMdiArea*  mdiArea
    View*  gview
    int fileWasLoaded

    # QPrinter printer

    QString curFile
void setCurrentFile(file-name)
    QString fileExtension(file-name)

    int myIndex

    QString curLayer
    unsigned int curColor
    QString curline-type
    QString curline-weight

void closeEvent(e)
void mdi-onWindowActivated()
void mdi-currentLayerChanged(layer)
void mdi-currentColorChanged(color)
void mdi-currentline-typeChanged(type)
void mdi-currentline-weightChanged(weight)
void mdi-updateColorline-typeline-weight()
void mdi-showViewScrollBars(int val)
void mdi-setViewCrossHairColor(color)
void setViewBackgroundColor(color)
void setViewSelectBoxColors(unsigned int colorL, unsigned int fillL, unsigned int colorR, unsigned int fillR, int alpha)
void setViewGridColor(unsigned int color)
void set_view-ruler-color(unsigned int color)
void print()

void showViewScrollBars(int val)
void setViewCrossHairColor(color)
        return
void setViewBackgroundColor(color)
        return
void setViewSelectBoxColors(unsigned int colorL, unsigned int fillL, unsigned int colorR, unsigned int fillR, int alpha)
void setViewGridColor(unsigned int color)
void set_view-ruler-color(unsigned int color)

void print()
    */

/*
void
mdi-window-init(self, theIndex, mw, parent, wflags)
    mw = mw
    mdiArea = parent
    myIndex = theIndex
    fileWasLoaded = 0

    setAttribute(Qt-WA-DeleteOnClose)

    aName = ""
    curFile = aName.asprintf("Untitled%d.dst", myIndex)
    setWindowTitle(curFile)
    setWindowIcon(QIcon("icons/app.png"))

    gscene = QGraphicsScene(0,0,0,0, this)
    gview = View(mw, gscene, this)

    setWidget(gview)
}
*/

void
load-file-action(char *file-name)
    FILE *file
    EmbPattern *p
    debug_message("MdiWindow load-file()")
    /*
    tmpColor = get-current-color()

    file = open(file-name, "r")
    if (!file) {
        warning(translate("Error reading file"),
                translate("Cannot read file %1:\n%2.")
                .arg(file-name).arg(file.errorString()))
        return 0
    }

    mw.set_override-cursor(Qt-WaitCursor)

    ext = fileExtension(file-name)
    debug_message("ext: %s", qPrintable(ext))

    p = embPattern-create()
    if (!p) {
        printf("Could not allocate memory for embroidery pattern\n")
        exit(1)
    }

    if (!p.readAuto(file-name)) {
        debug_message("Reading file was unsuccessful: %s\n", file-name)
        mw.restore-override-cursor()
        message = translate("Reading file was unsuccessful: ") + file-name
        warning(this, translate("Error reading pattern"), message)
    }
    else {
        p.move-stitch-list-to-polylines()
        # TODO: Test more
        stitchCount = p.stitch-list.count
        path = Path()

        if (p.circles) {
            for i in range(len(p.circles))
                c = p.circles.circle[i].circle
                this-color = p.circles.circle[i].color
                set_current-color(qRgb(this-color.r, this-color.g, this-color.b))
                # NOTE: With natives, the Y+ is up and libembroidery Y+ is up, so inverting the Y is NOT needed.
                mw.nativeAddCircle(c.center.x, c.center.y, c.radius, 0, "RUBBER-OFF")
                # TODO: fill

        if p.ellipses:
            for i in range(len(p.ellipses))
                e = p.ellipses.ellipse[i].ellipse
                this-color = p.ellipses.ellipse[i].color
                set_current-color(qRgb(this-color.r, this-color.g, this-color.b))
                # NOTE: With natives, the Y+ is up and libembroidery Y+ is up, so inverting the Y is NOT needed.
                mw.nativeAddEllipse(e.centerX, e.centerY, e.radiusX, e.radiusY, 0, 0, OBJ-RUBBER-OFF)
                #TODO: rotation and fill

        if p.lines:
            for i in range(len(p.lines))
                li = p.lines.line[i].line
                this-color = p.lines.line[i].color
                set_current-color(qRgb(this-color.r, this-color.g, this-color.b))
                # NOTE: With natives, the Y+ is up and libembroidery Y+ is up, so inverting the Y is NOT needed.
                mw.nativeAddLine(li.start.x, li.start.y, li.end.x, li.end.y, 0, OBJ-RUBBER-OFF)
                #TODO: rotation

        if p.paths:
            # TODO: This is unfinished. It needs more work
            for i in range(p.paths.count)
                curpoint-list = p.paths.path[i].point-list
                pathPath = Path()
                this-color = p.paths.path[i].color
                if curpoint-list.count > 0:
                    pp = curpoint-list[0].point.point
                    pathPath.move-to(pp.x, -pp.y)
                    #NOTE: Qt Y+ is down and libembroidery Y+ is up, so inverting the Y is needed.

                for j in range(curpoint-list.count)
                    pp = curpoint-list[j].point.point
                    pathPath.line-to(pp.x, -pp.y)
                    #NOTE: Qt Y+ is down and libembroidery Y+ is up, so inverting the Y is needed.

                loadPen = Pen(qRgb(this-color.r, this-color.g, this-color.b))
                loadPen.set_widthF(0.35)
                loadPen.set_cap-style(Qt-RoundCap)
                loadPen.set_join-style(Qt-RoundJoin)

                obj = Path(0, 0, pathPath, loadPen.color().rgb())
                item.setObjectRubberMode(OBJ-RUBBER-OFF)
                mw.activeScene().addItem(obj)

        if p.points:
            for i in range(p.points.count)
                po = p.points.point[i].point
                this-color = p.points.point[i].color
                set_current-color(qRgb(this-color.r, this-color.g, this-color.b))
                # NOTE: With natives, the Y+ is up and libembroidery Y+ is up, so inverting the Y is NOT needed.
                mw.nativeAddPoint(po.x, po.y)

        if p.polygons:
            for i in range(p.polygons.count)
                curpoint-list = p.polygons.polygon[i].point-list
                polygonPath = Path()
                firstPo= 0
                startX = 0
                startY = 0
                x = 0
                y = 0
                this-color = p.polygons.polygon[i].color
                set_current-color(qRgb(this-color.r, this-color.g, this-color.b))
                for j in range(curpoint-list.count)
                    pp = curpoint-list.point[j].point
                    x = pp.x
                    y = -pp.y
                    #NOTE: Qt Y+ is down and libembroidery Y+ is up, so inverting the Y is needed.

                    if first-point:
                        polygonPath.lineTo(x,y)
                    else {
                        polygonPath.move-to(x,y)
                        firstPo= 1
                        startX = x
                        startY = y

                polygonPath.translate(-startX, -startY)
                mw.nativeAddPolygon(startX, startY, polygonPath, OBJ-RUBBER-OFF)

        # NOTE: Polylines should only contain NORMAL stitches.
        if p.polylines:
            for i in range(len(p.polylines))
                curpoint-list = p.polylines.polyline[i].point-list
                polylinePath = Path()
                firstPo = 0
                startX = 0
                startY = 0
                x = 0
                y = 0
                this-color = p.polylines.polyline[i].color
                set_current-color(qRgb(this-color.r, this-color.g, this-color.b))
                for j in range(curpoint-list.count)
                    pp = curpoint-list.point[j].point
                    x = pp.x
                    y = -pp.y
                    # NOTE: Qt Y+ is down and libembroidery Y+ is up, so inverting the Y is needed.
                    if first-point:
                        polylinePath.line-to(x,y)
                    else {
                        polylinePath.move-to(x,y)
                        firstPo= 1
                        startX = x
                        startY = y

                polylinePath.translate(-startX, -startY)
                mw.nativeAddPolyline(startX, startY, polylinePath, OBJ-RUBBER-OFF)

        if p.rects:
            for i in range(len(p.rects))
                r = p.rects.rect[i].rect
                this-color = p.rects.rect[i].color
                set_current-color(qRgb(this-color.r, this-color.g, this-color.b))
                # NOTE: With natives, the Y+ is up and libembroidery Y+ is up, so inverting the Y is NOT needed.
                mw.nativeAddRectangle(embRect-x(r), embRect-y(r), embRect-width(r), embRect-height(r), 0, 0, OBJ-RUBBER-OFF)
                # TODO: rotation and fill

        set_current-file(file-name)
        mw.statusbar.showMessage("File loaded.")
        stitches = ""
        stitches.setNum(stitchCount)

        if grid-load-from-file:
            #TODO: Josh, provide me a hoop size and/or grid spacing from the pattern.
            debug_message(".")

        mw.restore-override-cursor()

    p.free()

    # Clear the undo stack so it is not possible to undo past this point.
    undo-history-length = 0

    set_current-color(tmpColor)
    return 1
    */
}


void
set_current-file(char *file-name)
    /*
    curFile = QFileInfo(file-name).canonicalFilePath()
    setWindowModified(0)
    setWindowTitle(getShortCurrentFile())
    */
}

void
get-short-current-file(void)
    /*
    return QFileInfo(curFile).file-name() */
}

void
file-extension(char *file-name)
    /*
    return QFileInfo(file-name).suffix().toLower()
    */
}

void
on-window-activated(void/* mdi-window *subwindow */)
    debug_message("MdiWindow onWindowActivated()")
    /*
    mdi-win = w.mdi-window()
    if (mdi-win) {
        mdi-win.on-window-activated()
    }
    status-bar-SNAP.setChecked(gscene.property("ENABLE-SNAP"))
    status-bar-GRID.setChecked(gscene.property("ENABLE-GRID"))
    status-bar-RULER.setChecked(gscene.property("ENABLE-RULER"))
    status-bar[STATUS-ORTHO].setChecked(gscene.property("ENABLE-ORTHO"))
    status-bar[STATUS-POLAR].setChecked(gscene.property("ENABLE-POLAR"))
    status-bar[STATUS-QSNAP].setChecked(gscene.property("ENABLE-QSNAP"))
    status-bar[STATUS-QTRACK].setChecked(gscene.property("ENABLE-QTRACK"))
    status-bar[STATUS-LWT].setChecked(gscene.property("ENABLE-LWT"))
    #mw.prompt.setHistory(promptHistory)
    */
}

void
sizeHint()
    /*
    debug_message("MdiWindow sizeHint()")
    return QSize(450, 300)
    */
}

void
current-layer-changed(/*layer */)
    /* curLayer = layer */
}

void
current-color-changed(EmbColor color)
    /*
    curColor = color
    */
}

void
current-line-type-changed(int type)
     /*
    curline-type = type
    */
}

void
current-line-weight-changed(double weight)
{
    curline-weight = weight
}

void
update-color-line-type-line-weight(void)
{
    debug_message("update color line type weight")
}

void
show-view-scroll-bars(double val)
{
    /* gview.showScrollBars(val) */
}

void
set_view-cross-hair-color(EmbColor color)
{
    /* gview.setCrossHairColor(color) */
}

void
set_view-background-color(EmbColor color)
{
    /* gview.setBackgroundColor(color) */
}

void
set_view-select-box-colors(
    EmbColor colorL,
    EmbColor fillL,
    EmbColor colorR,
    EmbColor fillR,
    double alpha
}
     /* gview.setSelectBoxColors(colorL, fillL, colorR, fillR, alpha) */
}

void
set_viewGridcolor(EmbColor color)
    /* gview.setGridColor(color) */
}

void
set_view-ruler-color(EmbColor color)
    /* gview.setRulerColor(color) */
}

/*
 *  Property editor.
 *  Uses the configuration to define what data should be presented
 *  to the user and what can be edited by the user.
 *
 * for toolbars: modify and draw. Inquiry toolbar?
 *
 * TODO: associate the property editor with the function callbacks using
 * a function pointer.
 *
 */

/*
# property-editor-row property-editors[] = {

class PropertyEditor()
  */  /*
    PropertyEditor(iconDirectory = "", int pickAddMode = 1, QWidget* widgetToFocus = 0, QWidget* parent = 0, Qt-WindowFlags flags = Qt-Widget)
    ~PropertyEditor()

    Qgroupbox* creategroupbox-geometry(int obj-type)
    Qgroupbox*   creategroupboxMiscImage()
    Qgroupbox*   creategroupboxGeneral()
    Qgroupbox*   creategroupboxMiscArc()
    Qgroupbox*   creategroupboxMiscPath()
    Qgroupbox*   creategroupboxMiscPolyline()
    Qgroupbox*   creategroupboxTextTextSingle()
    Qgroupbox*   creategroupboxMiscTextSingle()

    QWidget* focusWidget

    QString  iconDir
    int  iconSize
    Qt-ToolButtonStyle propertyEditorButtonStyle

    int pickAdd

    QList<QGraphicsItem*> selectedItemList

    #Helper functions
    QToolButton*   createToolButton(const QString& iconName, const QString& txt)
    QLineEdit* createLineEdit(const QString& validatorType = "", int readOnly = false)
    QComboBox* create-combobox(int disable = false)
    Qfont-combobox* createfont-combobox(int disable = false)

void
updateLineEditStrIfVaries(QLineEdit* lineEdit, const QString& str)

void
updateLineEditNumIfVaries(QLineEdit* lineEdit, num, int useAnglePrecision)

void
updatefont-comboboxStrIfVaries(Qfont-combobox* font-combobox, const QString& str)

void
updateComboBoxStrIfVaries(QComboBox* comboBox, const QString& str, const QStringList& strList)

void
updateComboBoxintIfVaries(QComboBox* comboBox, int val, int yesOrNoText)

    QSignalMapper* signalMapper
void
mapSignal(QObject* fieldObj, const QString& name, QVariant value)

    QComboBox*   create-comboboxSelected()
    QToolButton* createToolButtonQSelect()
    QToolButton* createToolButtonPickAdd()

    #TODO: Alphabetic/Categorized TabWidget
    bool eventFilter(QObject *obj, QEvent *event)

void
pickAddModeToggled()

void
setSelectedItems(QList<QGraphicsItem*> itemList)

void
updatePickAddModeButton(int pickAddMode)

void
fieldEdited(QObject* fieldObj)

void
showGroups(int obj-type)

void
showOneType(int index)

void
hideAllGroups()

void
clear-all-fields()

void
togglePickAddMode(

*/
/*
void
property-editor-init(self, iconDirectory, pickAddMode, widgetToFocus, parent, flags)
{ */
    /* . */ /*
    iconDir = iconDirectory
    iconSize = 16
    propertyEditorButtonStyle = Qt-button-TextBesideIcon */
    /* TODO: Make customizable. */ /*
    setMinimumSize(100,100)

    pickAdd = pickAddMode

    precisionAngle = 0 */
    /* TODO: Load this from settings and provide function for updating from settings */ /*
    precisionLength = 4
    # TODO: Load this from settings and provide function for updating from settings

    signalMapper = tk.SignalMapper(root)

    field-old-text = ""
    field-new-text = ""
    field-varies-text = "*Varies*"
    fieldYesText = "Yes"
    fieldNoText = "No"
    fieldOnText = "On"
    fieldOffText = "Off"

    widgetMain = tk.Widget(root)

    widgetSelection = tk.Widget(root)
    hboxLayoutSelection = tk.HBoxLayout(this)
    hboxLayoutSelection.addWidget(create-comboboxSelected())
    hboxLayoutSelection.addWidget(createbutton-QSelect())
    hboxLayoutSelection.addWidget(createbutton-PickAdd())
    widgetSelection.setLayout(hboxLayoutSelection)

    for i in range(1, UNKNOWN-BASE)
        groupbox-geometry[i] = creategroupbox-geometry(i+BASE)

    scrollProperties = QScrollArea(this)
    widgetProperties = QWidget(this)
    vboxLayoutProperties = QVBoxLayout(this)
    vboxLayoutProperties.addWidget(creategroupboxGeneral())
    for i in range(1, UNKNOWN-BASE)
        vboxLayoutProperties.addWidget(groupbox-geometry[i+BASE])

    vboxLayoutProperties.addWidget(creategroupboxMiscArc())
    vboxLayoutProperties.addWidget(creategroupboxMiscImage())
    vboxLayoutProperties.addWidget(creategroupboxMiscPath())
    vboxLayoutProperties.addWidget(creategroupboxMiscPolyline())
    vboxLayoutProperties.addWidget(creategroupboxTextTextSingle())
    vboxLayoutProperties.addWidget(creategroupboxMiscTextSingle())
    vboxLayoutProperties.addStretch(1)
    widgetProperties.setLayout(vboxLayoutProperties)
    scrollProperties.setWidget(widgetProperties)
    scrollProperties.setWidgetResizable(1)

    vboxLayoutMain = QVBoxLayout(this)
    vboxLayoutMain.addWidget(widgetSelection)
    vboxLayoutMain.addWidget(scrollProperties)
    widgetMain.setLayout(vboxLayoutMain)

    setWidget(widgetMain)
    setWindowTitle(translate("Properties"))
    setAllowedAreas(Qt-LeftDockWidgetArea | Qt-RightDockWidgetArea)

    hideAllGroups()

    connect(signalMapper, SIGNAL(mapped()), this, SLOT(fieldEdited()))

    focusWidget = widgetToFocus
    this.installEventFilter(this)
}
*/

void
create-groupbox(int data)
    /*
    for (i=0  i++) {
        button = create-tool-button("blank", translate(label[i]))
        edit = create-line-edit("double", 0)
        add-to-form(button, edit)
    }
    */
}

void
event-filter(int obj, SDL-Event event)
    /*
    if (event.type() == "KeyPress") {
        key = event.key()
        if (Qt-Key-Escape) {
            if focusWidget:
                focusWidget.setFocus(Qt-OtherFocusReason)
            return 1
        }
        else {
            event.ignore()
        }
    }
    return QObject-eventFilter(obj, event)
    */
}

void
create-combobox-selected(void)
    /*
    comboBoxSelected = tk.ComboBox(this)
    comboBoxSelected.addItem(translate("No Selection"))
    return comboBoxSelected
    */
}

void
createbutton-QSelect(void)
    /*
    button-QSelect = tk.Button-(this)
    button-QSelect.setIcon(load-icon(quickselect-xpm))
    button-QSelect.setIconSize(QSize(iconSize, iconSize))
    button-QSelect.setText("QSelect")
    button-QSelect.setToolTip("QSelect") #TODO: Better Description
    button-QSelect.setbutton-Style(Qt-button-IconOnly)
    return button-QSelect
    */
}

void
createbutton-PickAdd(void)
/*
    #TODO: Set as PickAdd or PickNew based on settings
    button-PickAdd = Qbutton-(this)
    updatePickAddModeButton(pickAdd)
    connect(button-PickAdd, SIGNAL(clicked(int)), this, SLOT(togglePickAddMode()))
    return button-PickAdd */
}

void
updatePickAddModeButton(int pickAddMode)
/*
    pickAdd = pickAddMode
    if (pickAdd)
        button-PickAdd.setIcon(load-icon(pickadd-xpm))
        button-PickAdd.setIconSize(QSize(iconSize, iconSize))
        button-PickAdd.setText("PickAdd")
        button-PickAdd.setToolTip("PickAdd Mode - Add to current selection.\nClick to switch to PickNew Mode.")
        button-PickAdd.setbutton-Style(Qt-button-IconOnly)

    else {
        button-PickAdd.setIcon(load-icon(picknew-xpm))
        button-PickAdd.setIconSize(QSize(iconSize, iconSize))
        button-PickAdd.setText("PickNew")
        button-PickAdd.setToolTip("PickNew Mode - Replace current selection.\nClick to switch to PickAdd Mode.")
        button-PickAdd.setbutton-Style(Qt-button-IconOnly)
        */
}

void
toggle_pick_add_mode(void)
{
    #emit pickAddModeToggled()
    debug_message("not sure how to deal with emit yet")
}

void
set_selected-items(int *itemList)
/*
    selectedItemList = itemList
    #Hide all the groups initially, then decide which ones to show
    hideAllGroups()
    comboBoxSelected.clear()

    if itemList.isEmpty()
        comboBoxSelected.addItem(translate("No Selection"))
        return

    typeSet = {}
    numAll = itemList.size()
    numObjects = [0 for i in range(31)]
    numTypes = 0

    for item in itemList:
        if (!item:
            continue

        obj-type = item.type()
        typeSet.insert(obj-type)

        if obj-type > BASE and obj-type < UNKNOWN:
            if numObjects[obj-type-BASE] == 0:
                numTypes += 1
            numObjects[obj-type-BASE] += 1
        else {
            numObjects[UNKNOWN-BASE] += 1
*/
    /* Populate the selection comboBox
     * ==================================================
     */ /*
    if numTypes > 1:
        comboBoxSelected.addItem(translate("Varies") + " (" + "".setNum(numAll) + ")")
        connect(comboBoxSelected, SIGNAL(currentIndexChanged(int)), this, SLOT(showOneType(int)))

    for (i=0 i<31 i++) {
        if (numObjects[i] > 0) {
            combobox-str = translate(obj-names[i])
            combobox-str += " (" + "".setNum(numObjects[i]) + ")"
            comboBoxSelected.addItem(combobox-str, BASE+i)
        }
    } */

    /* Load Data into the fields
     * ==================================================
     * Clear fields first so if the selected data varies,
     * the comparison is simple.
     */ /*
    clear-all-fields()

    for item in itemList:
        if (!item:
            continue

        # TODO: load data into the General field
        if item.type == "Arc":
            p = obj.objectCenter()
            update-edit-NumIfVaries(edit-[ARC-CENTER-X], p.x(), 0)
            update-edit-NumIfVaries(edit-[ARC-CENTER-Y], -p.y(), 0)
            update-edit-NumIfVaries(edit-[ARC-RADIUS], obj.objectRadius(), 0)
            update-edit-NumIfVaries(edit-[ARC-START-ANGLE], obj.objectStartAngle(), 1)
            update-edit-NumIfVaries(edit-[ARC-END-ANGLE], obj.objectEndAngle(), 1)
            update-edit-NumIfVaries(edit-[ARC-START-X], obj.objectStartPoint().x(), 0)
            update-edit-NumIfVaries(edit-[ARC-START-Y], -obj.objectStartPoint().y(), 0)
            update-edit-NumIfVaries(edit-[ARC-END-X], obj.objectEndPoint().x(), 0)
            update-edit-NumIfVaries(edit-[ARC-END-Y], -obj.objectEndPoint().y(), 0)
            update-edit-NumIfVaries(edit-[ARC-AREA], obj.objectArea(), 0)
            update-edit-NumIfVaries(edit-[ARC-LENGTH], obj.objectArcLength(), 0)
            update-edit-NumIfVaries(edit-[ARC-CHORD], obj.objectChord(), 0)
            update-edit-NumIfVaries(edit-[ARC-INC-ANGLE], obj.objectIncludedAngle(), 1)
            updateComboBoxintIfVaries(comboBox[ARC-CLOCKWISE], obj.objectClockwise(), 1)

        elif item.type == "Block":
            debug_message("TODO: load block data")

        elif item.type == "Circle":
            p = obj.objectCenter()
            update-edit-NumIfVaries(edit-[CIRCLE-CENTER-X], p.x(), 0)
            update-edit-NumIfVaries(edit-[CIRCLE-CENTER-Y], -p.y(), 0)
            update-edit-NumIfVaries(edit-[CIRCLE-RADIUS], obj.objectRadius(), 0)
            update-edit-NumIfVaries(edit-[CIRCLE-DIAMETER], obj.objectDiameter(), 0)
            update-edit-NumIfVaries(edit-[CIRCLE-AREA], obj.objectArea(), 0)
            update-edit-NumIfVaries(edit-[CIRCLE-CIRCUMFERENCE], obj.objectCircumference(), 0)

        elif item.type == "DimAligned":
            debug_message("TODO: load aligned dimension data")

        elif item.type == "DimAngular":
            debug_message("TODO: load angular dimension data")

        elif item.type == "DimArcLength":
            debug_message("TODO: load arclength dimension data")

        elif item.type == DIMDIAMETER:
            debug_message("TODO: load diameter dimension data")

        elif item.type == DIMLEADER:
            debug_message("TODO: load leader dimension data")

        elif item.type == DIMLINEAR:
            debug_message("TODO: load linear dimension data")

        elif item.type == DIMORDINATE:
            debug_message("TODO: load ordinate dimension data")

        elif item.type == "DimRadius":
            debug_message("TODO: load radius dimension data")

        elif item.type == "Ellipse":
            p = obj.objectCenter()
            update-edit-NumIfVaries(edit-[ELLIPSE-CENTER-X], p.x(), 0)
            update-edit-NumIfVaries(edit-[ELLIPSE-CENTER-Y], -p.y(), 0)
            update-edit-NumIfVaries(edit-[ELLIPSE-RADIUS-MAJOR], obj.objectRadiusMajor(), 0)
            update-edit-NumIfVaries(edit-[ELLIPSE-RADIUS-MINOR], obj.objectRadiusMinor(), 0)
            update-edit-NumIfVaries(edit-[ELLIPSE-DIAMETER-MAJOR], obj.objectDiameterMajor(), 0)
            update-edit-NumIfVaries(edit-[ELLIPSE-DIAMETER-MINOR], obj.objectDiameterMinor(), 0)

        elif item.type == "Image":
            debug_message("TODO: load image data")

        elif item.type == "Infinite Line":
            debug_message("TODO: load infinite line data")

        elif item.type == "Line":
            update-edit-NumIfVaries(edit-[LINE-START-X], obj.objectEndPoint1().x(), 0)
            update-edit-NumIfVaries(edit-[LINE-START-Y], -obj.objectEndPoint1().y(), 0)
            update-edit-NumIfVaries(edit-[LINE-END-X], obj.objectEndPoint2().x(), 0)
            update-edit-NumIfVaries(edit-[LINE-END-Y], -obj.objectEndPoint2().y(), 0)
            update-edit-NumIfVaries(edit-[LINE-DELTA-X], obj.objectDeltaX(), 0)
            update-edit-NumIfVaries(edit-[LINE-DELTA-Y], -obj.objectDeltaY(), 0)
            update-edit-NumIfVaries(edit-[LINE-ANGLE], obj.objectAngle(), 1)
            update-edit-NumIfVaries(edit-[LINE-LENGTH], obj.objectLength(), 0)

        elif item.type == "Path":
            debug_message(".")

        elif item.type == "Point":
            update-edit-NumIfVaries(edit-[POINT-X], obj.objectX(), 0)
            update-edit-NumIfVaries(edit-[POINT-Y], -obj.objectY(), 0)

        elif item.type == "Polygon":
            debug_message(".")

        elif item.type == "Polyline":
            debug_message(".")

        elif item.type == "Ray":
            debug_message(".")

        elif item.type == RECTANGLE:
            corn1 = obj.objectTopLeft()
            corn2 = obj.objectTopRight()
            corn3 = obj.objectBottomLeft()
            corn4 = obj.objectBottomRight()

            update-edit-NumIfVaries(edit-[RECT-CORNER-X1], corn1.x(), 0)
            update-edit-NumIfVaries(edit-[RECT-CORNER-Y1], -corn1.y(), 0)
            update-edit-NumIfVaries(edit-[RECT-CORNER-X2], corn2.x(), 0)
            update-edit-NumIfVaries(edit-[RECT-CORNER-Y2], -corn2.y(), 0)
            update-edit-NumIfVaries(edit-[RECT-CORNER-X3], corn3.x(), 0)
            update-edit-NumIfVaries(edit-[RECT-CORNER-Y3], -corn3.y(), 0)
            update-edit-NumIfVaries(edit-[RECT-CORNER-X4], corn4.x(), 0)
            update-edit-NumIfVaries(edit-[RECT-CORNER-Y4], -corn4.y(), 0)
            update-edit-NumIfVaries(edit-[RECT-WIDTH], obj.objectWidth(), 0)
            update-edit-NumIfVaries(edit-[RECT-HEIGHT], -obj.objectHeight(), 0)
            update-edit-NumIfVaries(edit-[RECT-AREA], obj.objectArea(), 0)

        elif item.type == TEXTMULTI:
            debug_message(".")

        elif item.type == TEXTSINGLE:
            update-edit-StrIfVaries(edit-TextSingleContents, obj.objText)
            updatefont-comboboxStrIfVaries(comboBoxTextSingleFont, obj.objTextFont)
            updateComboBoxStrIfVaries(comboBoxTextSingleJustify, obj.objTextJustify, obj.objectTextJustifyList())
            update-edit-NumIfVaries(edit-TextSingleHeight, obj.obj-text.size, 0)
            update-edit-NumIfVaries(edit-TextSingleRotation, -obj.rotation(), 1)
            update-edit-NumIfVaries(edit-TextSingleX, obj.objectX(), 0)
            update-edit-NumIfVaries(edit-TextSingleY, -obj.objectY(), 0)
            updateComboBoxintIfVaries(comboBoxTextSingleBackward, obj.obj-text.backward, 1)
            updateComboBoxintIfVaries(comboBoxTextSingleUpsideDown, obj.obj-text.upsidedown, 1)

    # Only show fields if all objects are the same type
    #==================================================
    if numTypes == 1:
        for obj-type in typeSet:
            showGroups(obj-type)
            */
}

void
update-edit-StrIfVaries(int edit-, char *str)
/*
    field-old-text = edit-.text()
    field-new-text = str

    if field-old-text.isEmpty()
        edit-.setText(field-new-text)
    elif field-old-text != field-new-text:
        edit-.setText(field-varies-text)
        */
}

void
update-edit-NumIfVaries(int edit-, int num, int useAnglePrecision)
/*
    precision = 0
    if useAnglePrecision:
        precision = precisionAngle
    else {
        precision = precisionLength

    field-old-text = edit-.text()
    field-new-text.setNum(num, 'f', precision)

    # Prevent negative zero :D
    negative-zero = "-0."
    for i in range(precision)
        negative-zero += '0'
    if field-new-text == negative-zero:
        field-new-text = negative-zero.replace("-", "")

    if field-old-text.isEmpty()
        edit-.setText(field-new-text)
    elif field-old-text != field-new-text:
        edit-.setText(field-varies-text)*/
}

/*
void
updatefont-comboboxStrIfVaries(self, font-combobox, str)
    field-old-text = font-combobox.property("FontFamily").toString()
    field-new-text = str
    #(debug_message "old: %d %s, new: %d %s", oldIndex, qPrintable(font-combobox.currentText()), newIndex, qPrintable(str))
    if field-old-text.isEmpty()
        font-combobox.setCurrentFont(QFont(field-new-text))
        font-combobox.setProperty("FontFamily", field-new-text)
    elif field-old-text != field-new-text:
        if font-combobox.findText(field-varies-text) == -1:
            # Prevent multiple entries
            font-combobox.addItem(field-varies-text)
        font-combobox.setCurrentIndex(font-combobox.findText(field-varies-text))
}

void
updateComboBoxStrIfVaries(self, comboBox, str, strList)
    field-old-text = comboBox.currentText()
    field-new-text = str

    if field-old-text.isEmpty()
        for s in strList:
            comboBox.addItem(s, s)
        comboBox.setCurrentIndex(comboBox.findText(field-new-text))

    elif field-old-text != field-new-text:
        if comboBox.findText(field-varies-text) == -1:
            # Prevent multiple entries
            comboBox.addItem(field-varies-text)
        comboBox.setCurrentIndex(comboBox.findText(field-varies-text))
}

void
update-combobox-int-if-varies)
(self, comboBox, val, yesOrNoText)
    field-old-text = comboBox.currentText()
    if yesOrNoText:
        if val:
            field-new-text = fieldYesText
        else {
            field-new-text = fieldNoText

    else {
        if val:
            field-new-text = fieldOnText
        else {
            field-new-text = fieldOffText

    if field-old-text.isEmpty()
        if yesOrNoText:
            comboBox.addItem(fieldYesText, 1)
            comboBox.addItem(fieldNoText, 0)

        else {
            comboBox.addItem(fieldOnText, 1)
            comboBox.addItem(fieldOffText, 0)

        comboBox.setCurrentIndex(comboBox.findText(field-new-text))

    elif field-old-text != field-new-text:
        # Prevent multiple entries
        if comboBox.findText(field-varies-text) == -1:
            comboBox.addItem(field-varies-text)
        comboBox.setCurrentIndex(comboBox.findText(field-varies-text))
}

void
showGroups(self, obj-type)
    if (obj-type in obj-types) {
        groupbox-geometry[obj-type-BASE].show()
    }
    if obj-type == "Arc":
        groupboxMiscArc.show()
    elif obj-type == "Image":
        groupboxMiscImage.show()
    elif obj-type == "PATH":
        groupboxMiscPath.show()
    elif obj-type == "POLYLINE":
        groupboxMiscPolyline.show()
    elif obj-type == "Text Single":
        groupboxTextTextSingle.show()
        groupboxMiscTextSingle.show()
}

void
show-one-type self index)
    (hide-all-groups)
    showGroups(comboBoxSelected.itemData(index).toInt())
}

 NOTE: General group will never be hidden.

void
hide-all-groups)
    for i in obj-types:
        groupbox-geometry[i].hide()
    groupboxMiscArc.hide()
    groupboxMiscImage.hide()
    groupboxMiscPath.hide()
    groupboxMiscPolyline.hide()
    groupboxTextTextSingle.hide()
    groupboxMiscTextSingle.hide()
}

void
clear-all-fields)
    for i in range(COMBOBOX-PROPERTY-EDITORS) {
        comboBox[i].clear()
    }
    for i in range(edit--PROPERTY-EDITORS) {
        edit-[i].clear()
    }
      Text Single
    comboBoxTextSingleFont.removeItem(comboBoxTextSingleFont.findText(field-varies-text))
    # NOTE: Do not clear comboBoxTextSingleFont
    comboBoxTextSingleFont.setProperty("FontFamily", "")
}


void
create-groupbox-geometry self obj-type)
    gb = Qgroupbox(translate("Geometry"), this)

    # TODO: use proper icons
    form-layout = tk.form-layout(this)
    for i in obj-types:
        if property-editors[i].object == obj-type:
            index = property-editors[i].id
            button-[index] = createbutton-(property-editors[i].icon, translate(property-editors[i].label))
            edit-[index] = createedit-(property-editors[i].type, property-editors[i].read-only)
            form-layout.add-row(button-[index], edit-[index])
            mapSignal(edit-[index], property-editors[i].signal, obj-type)

    gb.setLayout(form-layout)

    return gb
}

void
createbutton-(self, char *iconName, char *txt)
    tb = Qbutton-(this)
    tb.setIcon(load-icon(blank-xpm))
    tb.setIconSize(QSize(iconSize, iconSize))
    tb.setText(txt)
    tb.setbutton-Style(propertyEditorButtonStyle)
    tb.setStyleSheet("border:none")
    return tb
}

void
create-edit-(int validatorType, int readOnly)
    le = Qedit-(this)
    if validatorType == "int":
        le.setValidator(QIntValidator(le))
    elif validatorType == "double":
        le.setValidator(QDoubleValidator(le))
    le.setReadOnly(readOnly)
    return le
}

void
create-combobox(self, disable)
    cb = QComboBox(this)
    cb.setDisabled(disable)
    return cb
}

void
createfont-combobox(self, disable)
    fcb = Qfont-combobox(this)
    fcb.setDisabled(disable)
    return fcb
}

void
map-signal(self, fieldObj, name, value)
{
    fieldObj.setObjectName(name)
    fieldObj.setProperty(qPrintable(name), value)

    if name.startsWith("edit-")
        connect(fieldObj, SIGNAL(editingFinished()), signalMapper, SLOT(map()))
    elif name.startsWith("comboBox")
        connect(fieldObj, SIGNAL(activated(str)), signalMapper, SLOT(map()))

    signalMapper.setMapping(fieldObj, fieldObj)
}

void
fieldEdited(self, fieldObj)
{
    int blockSignals = 0;
    if (blockSignals) {
        return;
    }

    debug_message("==========Field was Edited==========")
    objName = fieldObj.objectName()
    obj-type = fieldObj.property(qPrintable(objName)).toInt()

    for item in selectedItemList:
        if item.type() != obj-type:
            continue

        if item.type == "Arc":
            if objName == "edit-ArcCenterX":
                tempArcObj = item
                if tempArcObj:
                    p = tempArcObj.objectCenter()
                    p.setX(edit-[ARC-CENTER-X].text().toDouble())
                    tempArcObj.setPos(p)

            if objName == "edit-ArcCenterY":
                tempArcObj = item
                if tempArcObj:
                    p = tempArcObj.objectCenter()
                    p.setY(edit-[ARC-CENTER-Y].text().toDouble())
                    tempArcObj.setPos(p)

            if objName == "edit-ArcRadius":
                tempArcObj = item
                if tempArcObj:
                    tempArcObj.setObjectRadius(edit-[ARC-RADIUS].text().toDouble())

            if objName == "edit-ArcStartAngle":
                tempArcObj = item
                if tempArcObj:
                    tempArcObj.setObjectStartAngle(edit-[ARC-START-ANGLE].text().toDouble())

            if objName == "edit-ArcEndAngle":
                tempArcObj = item
                if tempArcObj:
                    tempArcObj.setObjectEndAngle(edit-[ARC-END-ANGLE].text().toDouble())

        elif item.type == "Block":
            # TODO: field editing
            break
        elif item.type == "Circle":
            if objName == "edit-CircleCenterX":
                p = item.objectCenter()
                p.setX(edit-[CIRCLE-CENTER-X].text().toDouble())
                item.setPos(p)

            if objName == "edit-CircleCenterY":
                tempCircleObj = item
                if tempCircleObj:
                    p = tempCircleObj.objectCenter()
                    p.setY(edit-[CIRCLE-CENTER-Y].text().toDouble())
                    tempCircleObj.setPos(p)

            if objName == "edit-CircleRadius":
                tempCircleObj = item
                if tempCircleObj:
                    tempCircleObj.setObjectRadius(edit-[CIRCLE-RADIUS].text().toDouble())

            if objName == "edit-CircleDiameter":
                tempCircleObj = item
                if tempCircleObj:
                    tempCircleObj.setObjectDiameter(edit-[CIRCLE-DIAMETER].text().toDouble())

            if objName == "edit-circle-area":
                tempCircleObj = item
                if tempCircleObj:
                    tempCircleObj.setObjectArea(edit-[CIRCLE-AREA].text().toDouble())
            if objName == "edit-CircleCircumference":
                tempCircleObj = item
                if tempCircleObj:
                    tempCircleObj.setObjectCircumference(edit-[CIRCLE-CIRCUMFERENCE].text().toDouble())

            break

        elif item.type == "DIMALIGNED":
            # TODO: field editing
            break

        elif item.type == "DIMANGULAR":
            # TODO: field editing
            break

        elif item.type == "DIMARCLENGTH":
            # TODO: field editing
            break

        elif item.type == "DIMDIAMETER":
            # TODO: field editing
            break

        elif item.type == "DIMLEADER":
            # TODO: field editing
            break

        elif item.type == "DIMLINEAR": #TODO: field editing
            break

        elif item.type == "DIMORDINATE": #TODO: field editing
            break

        elif item.type == "DIMRADIUS": #TODO: field editing
            break

        elif item.type == "ELLIPSE":
            if objName == "edit-ellipse-center-x":
                p = item.center()
                p.x = edit-[ELLIPSE-CENTER-X].text().toDouble()
                item.setPos(p)

            if objName == "edit-ellipseCenterY":
                p = item.center()
                p.y = edit-[ELLIPSE-CENTER-Y].text().toDouble()
                item.setPos(p)

            if objName == "edit-EllipseRadiusMajor":
                item.setObjectRadiusMajor(edit-[ELLIPSE-RADIUS-MAJOR].text().toDouble())

            if objName == "edit-EllipseRadiusMinor":
                item.set_radius-minor(edit--ELLIPSE-RADIUS-MINOR.text().toDouble())

            if objName == "edit-EllipseDiameterMajor":
                item.setObjectDiameterMajor(edit--ELLIPSE-DIAMETER-MAJOR.text().toDouble())

            if objName == "edit-EllipseDiameterMinor":
                item.setObjectDiameterMinor(edit-[ELLIPSE-DIAMETER-MINOR].text().toDouble())

            break
        elif IMAGE: #TODO: field editing
            break
        elif INFINITELINE: #TODO: field editing
            break
        elif LINE:
            if objName == "edit-LineStartX":
                item.setObjectX1(edit-[LINE-START-X].text().toDouble())

            elif objName == "edit-LineStartY":
                item.setObjectY1(-edit-[LINE-START-Y].text().toDouble())

            elif objName == "edit-LineEndX":
                item.setObjectX2(edit-[LINE-END-X].text().toDouble())

            elif objName == "edit-LineEndY":
                item.setObjectY2(-edit-[LINE-END-Y].text().toDouble())

        elif item.type == "PATH":
            #TODO: field editing

        elif item.type == "POINT":
            if objName == "edit-PointX":
                item.setObjectX(edit-[POINT-X].text().toDouble())

            elif objName == "edit-PointY":
                item.setObjectY(-edit-[POINT-Y].text().toDouble())

        elif item.type == "POLYGON":
            debug_message("TYPE Polygon has no field editing")

        elif item.type == "Polyline":
            debug_message("TYPE Polyline has no field editing")

        elif item.type == "RAY":
            # TODO: field editing
            debug_message("TYPE Polyline has no field editing")

        elif item.type == "RECTANGLE":
            # TODO: field editing
            debug_message("TYPE Polyline has no field editing")

        elif item.type == "TEXTMULTI":
            # TODO: field editing
            debug_message("TYPE Polyline has no field editing")

        elif item.type == "TEXTSINGLE":
            # TODO: field editing
            if objName == "edit-TextSingleContents":
                item.setObjectText(edit-TextSingleContents.text())

            if objName == "comboBoxTextSingleFont":
                if comboBoxTextSingleFont.currentText() == field-varies-text:
                    break
                item.setTextFont(comboBoxTextSingleFont.currentFont().family())
            if objName == "comboBoxTextSingleJustify":
                if comboBoxTextSingleJustify.currentText() != field-varies-text:
                    item.setTextJustify(comboBoxTextSingleJustify.itemData(comboBoxTextSingleJustify.currentIndex()).toString())

            if objName == "edit-TextSingleHeight":
                tempTextSingleObj = static-cast<TextSingle*>(item)
                if (tempTextSingleObj) {
                    tempTextSingleObj.setTextSize(edit-TextSingleHeight.text().toDouble())

            if objName == "edit-TextSingleRotation":
                tempTextSingleObj = static-cast<TextSingle*>(item)
                if tempTextSingleObj:
                    tempTextSingleObj.setRotation(-edit-TextSingleRotation.text().toDouble())

            if objName == "edit-TextSingleX":
                tempTextSingleObj = static-cast<TextSingle*>(item)
                if tempTextSingleObj:
                    tempTextSingleObj.setX(edit-TextSingleX.text().toDouble())

            if objName == "edit-TextSingleY":
                tempTextSingleObj = static-cast<TextSingle*>(item)
                if tempTextSingleObj:
                    tempTextSingleObj.setY(edit-TextSingleY.text().toDouble())

            if objName == "comboBoxTextSingleBackward":
                if comboBoxTextSingleBackward.currentText() != field-varies-text:
                    tempTextSingleObj = static-cast<TextSingle*>(item)
                    if tempTextSingleObj:
                        tempTextSingleObj.setTextBackward(comboBoxTextSingleBackward.itemData(comboBoxTextSingleBackward.currentIndex()))

            if objName == "comboBoxTextSingleUpsideDown":
                if comboBoxTextSingleUpsideDown.currentText() != field-varies-text:
                    item.setTextUpsideDown(comboBoxTextSingleUpsideDown.itemData(comboBoxTextSingleUpsideDown.currentIndex()))

    # Block this slot from running twice since calling setSelectedItems will trigger it
    blockSignals = 1

    widget = QApplication-focusWidget()
    # Update so all fields have fresh data
    # TODO: Improve this

    setSelectedItems(selectedItemList);
    hideAllGroups();
    showGroups(obj-type);

    if (widget) {
        widget.setFocus(Qt-OtherFocusReason);
    }

    blockSignals = 0
}
*/

/*
 *  To make the undo history easier to manage we use a dict for
 *  keeping all the action information together.
 *
 *  For more commentary on this file see the Settings Dialog sections
 *  of the README.
 */
void
load-icon(char *fname)
{
    debug_message("load-icon with fname:");
    debug_message(fname);
}

void
addColorsTocombobox(int combobox)
{
/*
    combobox.add-item(load-icon(colorred-xpm), translate("Red"), tk.Rgb(255, 0, 0));
    combobox.add-item(load-icon(coloryellow-xpm), translate("Yellow"), tk.Rgb(255,255, 0));
    combobox.add-item(load-icon(colorgreen-xpm), translate("Green"), tk.Rgb(  0,255, 0));
    combobox.add-item(load-icon(colorcyan-xpm), translate("Cyan"), tk.Rgb(  0,255,255));
    combobox.add-item(load-icon(colorblue-xpm), translate("Blue"), tk.Rgb(  0, 0,255));
    combobox.add-item(load-icon(colormagenta-xpm), translate("Magenta"), tk.Rgb(255, 0,255));
    combobox.add-item(load-icon(colorwhite-xpm), translate("White"), tk.Rgb(255,255,255));
    # TODO: Add Other... so the user can select custom colors */
}

void
comboboxLanguageCurrent-index-changed(char *lang)
{
    /*
    dialog-general-language = lang.toLower();
    */
}

void
combobox-icon-themeCurrent-index-changed(char *theme)
{
    /*
    strcpy(dialog-general-icon-theme, theme);
    */
}

void
combobox-icon-sizeCurrent-index-changed(int index)
{
    /*
    combobox = sender();
    if (combobox) {
        ok = 0;
        dialog-general-icon-size, ok = combobox.itemData(index).toUInt();
        if (!ok) {
            dialog-general-icon-size = 16;
        }
    }
    else {
        dialog-general-icon-size = 16;
    }
    */
}

void
checkbox_general_mdi_bg_use_logo_state_changed(int checked)
{
    /*
    preview.general-mdi-bg-use-logo = checked;
    mdi-area.useBackgroundLogo(int checked);
    */
}

void
chooseGeneralMdiBackgroundLogo(void)
{
/*
    button = sender()
    if button) {
        selectedImage = tk.FileDialog-get-open-fname(this, translate("Open File"),
                        tk.StandardPaths-writableLocation(QStandardPaths-PicturesLocation),
                        translate("Images (*.bmp *.png *.jpg)"))

        if selectedImage != "") {
            accept.general-mdi-bg-logo = selectedImage

        #Update immediately so it can be previewed
        mdi-area.setBackgroundLogo(accept-.general-mdi-bg-logo) */
}

void
checkbox_general_mdi_bg_use_texture_state_changed(int checked)
{
    /*
    preview.general-mdi-bg-use-texture = checked;
    mdi-area.useBackgroundTexture(int checked);
    */
}

void
chooseGeneralMdiBackgroundTexture(void)
{
/*
    button = sender()
    if (button) {
        selectedImage = tk.FileDialog-get-open-fname(
            this, translate("Open File"),
            tk.StandardPaths-writableLocation(QStandardPaths-PicturesLocation),
            translate("Images (*.bmp *.png *.jpg)"))

        if (selectedImage != "") {
            accept.general-mdi-bg-texture = selectedImage
        }*/

        /* Update immediately so it can be previewed. */ /*
        mdi-area.setBackgroundTexture(accept-.general-mdi-bg-texture)
    }*/
}

void
checkboxGeneralMdiBGUseColor-state-changed(int checked)
/*
    preview.general-mdi-bg-use-color = checked
    mdi-area.useBackgroundColor(int checked)
*/
}

void
chooseGeneralMdiBackgroundColor(void)
/*
    button = sender()
    if (button) {
        color-dialog = color-dialog(Color(accept-.general-mdi-bg-color), this)
        #connect(color-dialog, SIGNAL(currentColorChanged()), this, SLOT(currentGeneralMdiBackgroundColorChanged()))
        color-dialog-exec()

        if (color-dialog-result() == "Accepted") {
            accept.general-mdi-bg-color = color-dialog-selectedColor().rgb()
            pix = Image(16,16)
            pix.fill(Color(accept-.general-mdi-bg-color))
            button.setIcon(pix)
            mdi-area.setBackgroundColor(Color(accept-.general-mdi-bg-color))
        }
        else {
            mdi-area.setBackgroundColor(Color(dialog-general-mdi-bg-color))
        }
    } */
}

void
currentGeneralMdiBackgroundColorChanged(EmbColor color)
   /* preview.general-mdi-bg-color = color.rgb()
    mdi-area.setBackgroundColor(Color(preview.general-mdi-bg-color)) */
    /*
    lambda method constructor?
    */
}

void
checkboxTipOfTheDay-state-changed(void)
    /*
    check-func(checkboxTipOfTheDay-state-changed, general-tip-of-the-day)
    check-func(checkboxUseOpenGL-state-changed, INT-DISPLAY-USE-OPENGL)
    check-func(checkboxRenderHintAA-state-changed, display-renderhint-aa)
    check-func(checkboxRenderHintTextAA-state-changed, display-renderhint-text-aa)
    check-func(checkboxRenderHintSmoothPix-state-changed, display-renderhint-smooth-pix)
    check-func(checkboxRenderHintHighAA-state-changed, display-renderhint-high-aa)
    check-func(checkboxRenderHintNonCosmetic-state-changed, display-renderhint-noncosmetic)
    */
}

void
checkboxShowScrollBars-state-changed(int checked)
/*
    preview.display-show-scrollbars = checked
    updateAllViewScrollBars(preview.display-show-scrollbars) */
}

void
spinboxzoom-scale-actionIn-value-changed(double value)
    /* dialog-display-zoom-scale-action-in = value */
}

void
spinboxzoom-scale-actionOut-value-changed(double value)
    /*dialog-display-zoom-scale-action-out = value */
}

void
checkboxDisableBG-state-changed(int checked)
    /*dialog-printing-disable-bg = checked */
}

void
chooseDisplayCrossHairColor(void)
/*
    button = sender()
    if button) {
        color-dialog = color-dialog(Color(accept-.display-crosshair-color), this)
        #connect(color-dialog, SIGNAL(currentColorChanged()), this, SLOT(currentDisplayCrossHairColorChanged()))
        color-dialog-exec()

        if color-dialog-result() == "Accepted") {
            accept.display-crosshair-color = color-dialog-selectedColor().rgb()
            pix = Image(16,16)
            pix.fill(Color(accept-.display-crosshair-color))
            button.setIcon(pix)
            updateAllViewCrossHairColors(accept-.display-crosshair-color)
        }
        else {
            updateAllViewCrossHairColors(dialog-display-crosshair-color)
        }
    } */
}

void
currentDisplayCrossHairColorChanged(EmbColor color)
/*
    preview.display-crosshair-color = color.rgb()
    updateAllViewCrossHairColors(preview.display-crosshair-color) */
}
 /*
void
chooseDisplayBackgroundColor()
    button = sender()
    if button) {
        color-dialog = color-dialog(Color(accept-.display-bg-color), this)
        #connect(color-dialog, SIGNAL(currentColorChanged()), this, SLOT(currentDisplayBackgroundColorChanged()))
        color-dialog-exec()

        if color-dialog-result() == "Accepted") {
            accept.display-bg-color = color-dialog-selectedColor().rgb()
            pix = Image(16, 16)
            pix.fill(Color(accept-.display-bg-color))
            button.setIcon(pix)
            updateAllViewBackgroundColors(accept-.display-bg-color)
        else) {
            updateAllViewBackgroundColors(dialog-display-bg-color)

void
currentDisplayBackgroundColorChanged(EmbColor color)
    preview.display-bg-color = color.rgb()
    updateAllViewBackgroundColors(preview.display-bg-color)
}

void
chooseDisplaySelectBoxLeftColor()
    button = sender()
    if (button) {
        color-dialog = color-dialog(Color(accept--display-selectbox-left-color), this)
        #connect(color-dialog, SIGNAL(currentColorChanged()), this, SLOT(currentDisplaySelectBoxLeftColorChanged()))
        color-dialog-exec()

        if (color-dialog-result() == tk.Dialog-Accepted) {
            accept-display-selectbox-left-color = color-dialog-selectedColor().rgb()
            pix = Image(16, 16)
            pix.fill(Color(accept--display-selectbox-left-color))
            button.setIcon(pix)
            update-all-view-select-box-colors(accept--display-selectbox-left-color,
                accept-display-selectbox-left-fill,
                accept-display-selectbox-right-color,
                accept.display-selectbox-right-fill,
                preview.display-selectbox-alpha)
        }
        else {
            update-all-view-select-box-colors(
                dialog-display-selectbox-left-color,
                dialog-display-selectbox-left-fill,
                dialog-display-selectbox-right-color,
                dialog-display-selectbox-right-fill,
                preview.display-selectbox-alpha)
        }
    }
}

void
currentDisplaySelectBoxLeftColorChanged(color)
    preview-display-selectbox-left-color = color.rgb()
    update-all-view-select-box-colors(
        preview-display-selectbox-left-color,
        preview-display-selectbox-left-fill,
        preview-display-selectbox-right-color,
        preview-display-selectbox-right-fill,
        preview-display-selectbox-alpha)
}

void
chooseDisplaySelectBoxLeftFill(void)
    button = sender()
    if (button) {
        color-dialog = color-dialog(Color(accept-display-selectbox-left-fill), this)
        #connect(color-dialog, SIGNAL(currentColorChanged()), this, SLOT(currentDisplaySelectBoxLeftFillChanged()))
        color-dialog-exec()

        if (color-dialog-result() == "Accepted") {
            accept-display-selectbox-left-fill = color-dialog-selectedColor().rgb()
            pix = Image(16, 16)
            pix.fill(Color(accept-display-selectbox-left-fill))
            button.setIcon(pix)
            update-all-view-select-box-colors(
                accept-display-selectbox-left-color,
                accept-display-selectbox-left-fill,
                accept-display-selectbox-right-color,
                accept-display-selectbox-right-fill,
                preview-display-selectbox-alpha)
        }
        else {
            update-all-view-select-box-colors(
                dialog-display-selectbox-left-color,
                dialog-display-selectbox-left-fill,
                dialog-display-selectbox-right-color,
                dialog-display-selectbox-right-fill,
                preview-display-selectbox-alpha)
        }
    }
}

void
current-display-selectBoxLeftFillChanged(color)
    preview-display-selectbox-left-fill = color.rgb()
    update-all-view-select-box-colors(
        preview-display-selectbox-left-color,
        preview-display-selectbox-left-fill,
        preview-display-selectbox-right-color,
        preview-display-selectbox-right-fill,
        preview-display-selectbox-alpha)
}

void
choose-display-select-box-right-color(void)
    button = sender()
    if (button) {
        color-dialog = color-dialog(Color(accept--display-selectbox-right-color), this)
        #connect(color-dialog, SIGNAL(currentColorChanged()), this, SLOT(currentDisplaySelectBoxRightColorChanged()))
        color-dialog-exec()

        if (color-dialog-result() == "Accepted") {
            accept-display-selectbox-right-color = color-dialog-selectedColor().rgb()
            pix = Image(16, 16)
            pix.fill(Color(accept-display-selectbox-right-color))
            button.setIcon(pix)
            update-all-view-select-box-colors(
                accept-display-selectbox-left-color,
                accept-display-selectbox-left-fill,
                accept-display-selectbox-right-color,
                accept-display-selectbox-right-fill,
                preview-display-selectbox-alpha)
        }
        else {
            update-all-view-select-box-colors(
                dialog-display-selectbox-left-color,
                dialog-display-selectbox-left-fill,
                dialog-display-selectbox-right-color,
                dialog-display-selectbox-right-fill,
                preview-display-selectbox-alpha)
        }
    }
}

void
currentDisplaySelectBoxRightColorChanged(EmbColor color)
    preview-display-selectbox-right-color = color.rgb()
    update-all-view-select-box-colors(
        preview-display-selectbox-left-color,
        preview-display-selectbox-left-fill,
        preview-display-selectbox-right-color,
        preview-display-selectbox-right-fill,
        preview-display-selectbox-alpha)
}

void
chooseDisplaySelectBoxRightFill(void)
    button = sender()
    if (button) {
        color-dialog = color-dialog(Color(accept-.display-selectbox-right-fill), this)
        #connect(color-dialog, SIGNAL(currentColorChanged()), this, SLOT(currentDisplaySelectBoxRightFillChanged()))
        color-dialog-exec()

        if (color-dialog-result() == "Accepted") {
            accept.display-selectbox-right-fill = color-dialog-selectedColor().rgb()
            pix = Image(16, 16)
            pix.fill(Color(accept-.display-selectbox-right-fill))
            button.setIcon(pix)
            update-all-view-select-box-colors(
                accept-display-selectbox-left-color,
                accept-display-selectbox-left-fill,
                accept-display-selectbox-right-color,
                accept-display-selectbox-right-fill,
                preview-display-selectbox-alpha)
        }
        else {
            update-all-view-select-box-colors(
                dialog-display-selectbox-left-color,
                dialog-display-selectbox-left-fill,
                dialog-display-selectbox-right-color,
                dialog-display-selectbox-right-fill,
                preview-display-selectbox-alpha)
        }
    }
}

void
currentDisplaySelectBoxRightFillChanged(color)
    preview.display-selectbox-right-fill = color.rgb()
    update-all-view-select-box-colors(
        preview-display-selectbox-left-color,
        preview-display-selectbox-left-fill,
        preview-display-selectbox-right-color,
        preview-display-selectbox-right-fill,
        preview-display-selectbox-alpha)
}

void
spinboxDisplaySelectBoxAlpha-value-changed(value)
    preview.display-selectbox-alpha = value
    update-all-view-select-box-colors(
        accept-display-selectbox-left-color,
        accept-display-selectbox-left-fill,
        accept-display-selectbox-right-color,
        accept-display-selectbox-right-fill,
        preview-display-selectbox-alpha)
}
*/
/* .

void
checkboxCustomFilter-state-changed(int checked)
    checkbox = sender()
    if checkbox) {
        format = checkbox.text()
        debug_message("CustomFilter: %s %d", tk.Printable(format), checked)
        if (checked) {
            opensave-custom-filter.append(" *." + format.toLower())
        }
        else {
            opensave-custom-filter.remove("*." + format, tk.t-CaseInsensitive)
        }
        #dialog-opensave-custom-filter = checked #TODO
    }
}
*/
/* .

void
buttonCustomFilterSelectAllClicked(void)
    buttonCustomFilterSelectAll(1)
    opensave-custom-filter = "supported"
}
*/
/* .

void
buttonCustomFilterClearAllClicked(void)
    buttonCustomFilterClearAll(0)
    opensave-custom-filter.clear()
}
*/
/* .
*/ /*
void
checkboxGridColorMatchCrossHair-state-changed(int checked)
    dialogGridcolor-match-crosshair = checked
    if dialogGridcolor-match-crosshair) {
        updateAllViewGridColors(accept-.display-crosshair-color)

    else) {
        updateAllViewGridColors(accept-.grid-color)

    sender-obj = sender()
    if (!sender-obj) {
        return
    }

    parent = sender-obj.parent()
    if (!parent) {
        return

    labelGridColor = parent.find-child("labelGridColor")
    if labelGridColor) {
        labelGridColor.setEnabled(!dialogGridcolor-match-crosshair)

    buttonGridColor = parent.find-child("buttonGridColor")
    if buttonGridColor) {
        buttonGridColor.setEnabled(!dialogGridcolor-match-crosshair)
}
*/
/* .

void
chooseGridColor(void)
    button = sender()
    if (button) {
        color-dialog = color-dialog(Color(accept-.grid-color), this)
        #connect(color-dialog, SIGNAL(currentColorChanged()), this, SLOT(currentGridColorChanged()))
        color-dialog-exec()

        if (color-dialog-result() == "Accepted") {
            accept.grid-color = color-dialog-selectedColor().rgb()
            pix = Image(16, 16)
            pix.fill(Color(accept-.grid-color))
            button.setIcon(pix)
            updateAllViewGridColors(accept-.grid-color)
        }
        else {
            updateAllViewGridColors(dialogGridcolor)
        }
    }
}

void
currentGridcolor-changed color)
  (define previewGridcolor color)
  (update-all-viewGridcolors "preview"))

void
checkboxGridload-from-file-state-changed(int checked)
    dialogGridload-from-file = checked

    sender-obj = sender()
    if (!sender-obj) {
        return

    parent = sender-obj.parent()
    if (!parent) {
        return

    labelGridtype = parent.find-child("labelGridtype")
    if labelGridtype) {
        labelGridtype.setEnabled(!dialogGridload-from-file)

    comboboxGridtype = parent.find-child("comboboxGridtype")
    if comboboxGridtype) {
        comboboxGridtype.setEnabled(!dialogGridload-from-file)

    checkboxGridcenter-on-origin = parent.find-child("checkboxGridcenter-on-origin")
    if checkboxGridcenter-on-origin) {
        checkboxGridcenter-on-origin.setEnabled(!dialogGridload-from-file)

    labelGridcenter-x = parent.find-child("labelGridcenter-x")
    if labelGridcenter-x) {
        labelGridcenter-x.setEnabled(!dialogGridload-from-file and not dialogGridcenter-on-origin)

    spinboxGridCenterX = parent.find-child("spinboxGridCenterX")
    if spinboxGridCenterX) {
        spinboxGridCenterX.setEnabled(!dialogGridload-from-file and not dialogGridcenter-on-origin)

    labelGridcenter-y = parent.find-child("labelGridcenter-y")
    if labelGridcenter-y) {
        labelGridcenter-y.setEnabled(!dialogGridload-from-file and not dialogGridcenter-on-origin)

    spinboxGridcenter-y = parent.find-child("spinboxGridcenter-y")
    if spinboxGridcenter-y) {
        spinboxGridcenter-y.setEnabled(!dialogGridload-from-file and not dialogGridcenter-on-origin)

    labelGridsizeX = parent.find-child("labelGridsizeX")
    if labelGridsizeX) {
        labelGridsizeX.setEnabled(!dialogGridload-from-file)

    spinboxGridsizeX = parent.find-child("spinboxGridsizeX")
    if spinboxGridsizeX) {
        spinboxGridsizeX.setEnabled(!dialogGridload-from-file)

    labelGridsizeY = parent.find-child("labelGridsizeY")
    if labelGridsizeY) {
        labelGridsizeY.setEnabled(!dialogGridload-from-file)

    spinboxGridsizeY = parent.find-child("spinboxGridsizeY")
    if spinboxGridsizeY) {
        spinboxGridsizeY.setEnabled(!dialogGridload-from-file)

    labelGridSpacingX = parent.find-child("labelGridSpacingX")
    if labelGridSpacingX) {
        labelGridSpacingX.setEnabled(!dialogGridload-from-file)

    spinboxGridSpacingX = parent.find-child("spinboxGridSpacingX")
    if spinboxGridSpacingX) {
        spinboxGridSpacingX.setEnabled(!dialogGridload-from-file)

    labelGridSpacingY = parent.find-child("labelGridSpacingY")
    if labelGridSpacingY) {
        labelGridSpacingY.setEnabled(!dialogGridload-from-file)

    spinboxGridSpacingY = parent.find-child("spinboxGridSpacingY")
    if spinboxGridSpacingY) {
        spinboxGridSpacingY.setEnabled(!dialogGridload-from-file)

    labelGridsizeRadius = parent.find-child("labelGridsizeRadius")
    if labelGridsizeRadius) {
        labelGridsizeRadius.setEnabled(!dialogGridload-from-file)

    spinboxGridsizeRadius = parent.find-child("spinboxGridsizeRadius")
    if spinboxGridsizeRadius) {
        spinboxGridsizeRadius.setEnabled(!dialogGridload-from-file)

    labelGridSpacingRadius = parent.find-child("labelGridSpacingRadius")
    if labelGridSpacingRadius) {
        labelGridSpacingRadius.setEnabled(!dialogGridload-from-file)

    spinboxGridSpacingRadius = parent.find-child("spinboxGridSpacingRadius")
    if spinboxGridSpacingRadius) {
        spinboxGridSpacingRadius.setEnabled(!dialogGridload-from-file)

    labelGridSpacingAngle = parent.find-child("labelGridSpacingAngle")
    if labelGridSpacingAngle) {
        labelGridSpacingAngle.setEnabled(!dialogGridload-from-file)

    spinboxGridSpacingAngle = parent.find-child("spinboxGridSpacingAngle")
    if spinboxGridSpacingAngle) {
        spinboxGridSpacingAngle.setEnabled(!dialogGridload-from-file)
}
*/
/* .

void
comboboxGridtypeCurrent-index-changed(type-)
    dialogGridtype = type-

    sender-obj = sender()
    if (!sender-obj) {
        return

    parent = sender-obj.parent()
    if (!parent) {
        return

    visibility = False
    if type- == "Circular") {
        visibility = 1

    labelGridsize-x = parent.find-child("labelGridsizeX")
    if labelGridsize-x) {
        labelGridsize-x.visible = !visibility)

    spinboxGridsizeX = parent.find-child("spinboxGridsizeX")
    if spinboxGridsizeX) {
        spinboxGridsizeX.visible = !visibility)

    labelGridsizeY = parent.find-child("labelGridsizeY")
    if labelGridsizeY) {
        labelGridsizeY.visible = !visibility)

    spinboxGridsizeY = parent.find-child("spinboxGridsizeY")
    if spinboxGridsizeY) {
        spinboxGridsizeY.visible = !visibility)

    labelGridSpacingX = parent.find-child("labelGridSpacingX")
    if labelGridSpacingX) {
        labelGridSpacingX.visible = !visibility)

    spinboxGridSpacingX = parent.find-child("spinboxGridSpacingX")
    if spinboxGridSpacingX) {
        spinboxGridSpacingX.visible = !visibility)

    labelGridSpacingY = parent.find-child("labelGridSpacingY")
    if labelGridSpacingY) {
        labelGridSpacingY.visible = !visibility)

    spinboxGridSpacingY = parent.find-child("spinboxGridSpacingY")
    if spinboxGridSpacingY) {
        spinboxGridSpacingY.visible = !visibility)

    labelGridsizeRadius = parent.find-child("labelGridsizeRadius")
    if labelGridsizeRadius) {
        labelGridsizeRadius.visible = visibility)

    spinboxGridsizeRadius = parent.find-child("spinboxGridsizeRadius")
    if spinboxGridsizeRadius) {
        spinboxGridsizeRadius.visible = visibility)

    labelGridSpacingRadius = parent.find-child("labelGridSpacingRadius")
    if labelGridSpacingRadius) {
        labelGridSpacingRadius.visible = visibility)

    spinboxGridSpacingRadius = parent.find-child("spinboxGridSpacingRadius")
    if spinboxGridSpacingRadius) {
        spinboxGridSpacingRadius.visible = visibility)

    labelGridSpacingAngle = parent.find-child("labelGridSpacingAngle")
    if labelGridSpacingAngle) {
        labelGridSpacingAngle.visible = visibility)

    spinboxGridSpacingAngle = parent.find-child("spinboxGridSpacingAngle")
    if spinboxGridSpacingAngle) {
        spinboxGridSpacingAngle.visible = visibility)
}
*/
 /*
void
checkboxGridcenter-on-origin-state-changed(int checked)
    dialogGridcenter-on-origin = checked

    sender-obj = sender()
    if (!sender-obj) {
        return

    parent = sender-obj.parent()
    if (!parent) {
        return

    labelGridcenter-x = parent.find-child("labelGridcenter-x")
    if labelGridcenter-x) {
        labelGridcenter-x.setEnabled(!dialogGridcenter-on-origin)

    spinboxGridCenterX = parent.find-child("spinboxGridCenterX")
    if spinboxGridCenterX) {
        spinboxGridCenterX.setEnabled(!dialogGridcenter-on-origin)

    labelGridcenter-y = parent.find-child("labelGridcenter-y")
    if labelGridcenter-y) {
        labelGridcenter-y.setEnabled(!dialogGridcenter-on-origin)

    spinboxGridcenter-y = parent.find-child("spinboxGridcenter-y")
    if spinboxGridcenter-y) {
        spinboxGridcenter-y.setEnabled(!dialogGridcenter-on-origin)
}
*/

void
combobox-ruler-metric-current-index-changed(int index)
    /*
    combobox = sender()
    if (combobox) {
        ok = False
        dialog-ruler-metric = combobox.itemData(index)
    }
    else {
        dialog-ruler-metric = 1
    }*/
}

void
chooseRulerColor(void)
    /*
    button = sender()
    if (button) {
        color-dialog = color-dialog(
            Color(accept-.ruler-color), this)
        # connect(color-dialog, SIGNAL(currentColorChanged()),
        # this, SLOT(currentRulerColorChanged()))
        color-dialog-exec()

        if (color-dialog-result() == "QDialog-Accepted") {
            accept.ruler-color = color-dialog-selectedColor().rgb()
            pix = Image(16, 16)
            pix.fill(Color(accept-.ruler-color))
            button.setIcon(pix)
            updateAllViewRulerColors(accept-.ruler-color)
        }
        else {
            updateAllViewRulerColors(dialog-ruler-color)
        }
    }
    */
}

/*
 */
void
currentRulerColorChanged(EmbColor color)
    /*
    preview.ruler-color = color.rgb()
    updateAllViewRulerColors(preview.ruler-color)
    */
}

/*

void
buttonQSnapSelectAllClicked(void)
    buttonQSnapSelectAll(1)
}

/*
 * TODO:
 * Figure out how to abstract the slot in a way that it can be used for
 * comboboxes in general
 * Currently comboboxQSnapLocatorColorCurrent-index-changed(int index)
 *       comboboxSelectionCoolGripColorCurrent-index-changed(int index)
 *       comboboxSelectionHotGripColorCurrent-index-changed(int index)
 * are all similar except the dialog- variable being worked on and the
 * tk.Variant.

void
buttonQSnapClearAllClicked(void)
    buttonQSnapClearAll(0)
}

/* TODO: Alert user if color matched the display bg color

void
comboboxQSnapLocatorColorCurrent-index-changed(int index)
    combobox = sender()
    default-color = tk.Rgb(255,255,0)
    # Yellow
    if combobox) {
        dialog-qsnap-locator-color, ok = combobox.itemData(index).toUInt()
        if (!ok) {
            dialog-qsnap-locator-color = default-color
    else) {
        dialog-qsnap-locator-color = default-color
}
 /*
void
sliderQSnap-locator-size-value-changed(double value)
    dialog-qsnap-locator-size = value
}
 /*
void
sliderQSnap-aperture-size-value-changed(double value)
    dialog-qsnap-aperture-size = value
}

void
checkboxLwtShowLwt-state-changed(int checked)
    preview.lwt-show-lwt = checked
    if preview.lwt-show-lwt) {
        enable-lwt()
    else) {
        disableLwt()

    sender-obj = sender()
    if sender-obj) {
        parent = sender-obj.parent()
        if parent) {
            checkboxRealRender = parent.find-child("checkboxRealRender")
            if checkboxRealRender) {
                checkboxRealRender.setEnabled(preview.lwt-show-lwt)

void
checkboxLwtRealRender-state-changed(int checked)
    preview-lwt-real-render = checked
    if preview-lwt-real-render) {
        enableReal()
    else) {
        disableReal()
}

void
comboboxSelectionCoolGripColorCurrent-index-changed(index)
    # TODO: Alert user if color matched the display bg color
    combobox = sender()
    default-color = tk.Rgb(0,0,255)
    # Blue
    if combobox) {
        dialog-selection-coolgrip-color, ok = combobox.item-data(index).toUInt()
        if (!ok) {
            dialog-selection-coolgrip-color = default-color
    else) {
        dialog-selection-coolgrip-color = default-color
}

void
combobox-selectionHotGripColorCurrent-index-changed(index)
    # TODO: Alert user if color matched the display bg color
    combobox = sender()
    default-color = tk.Rgb(255,0,0)
    # Red
    if combobox) {
        dialog-selection-hotgrip-color, ok = combobox.item-data(index).toUInt()
        if (!ok) {
            dialog-selection-hotgrip-color = default-color
    else) {
        dialog-selection-hotgrip-color = default-color
}
*/
/* . */ /*
void
accept-changes(void)
    for k in preview.keys()
        dialog[k] = preview[k]
    for k in accept.keys()
        dialog[k] = preview[k]

    if dialogGridcolor-match-crosshair) {
        dialogGridcolor = accept-display-crosshair-color

    # Make sure the user sees the changes applied immediately.
    mdi-area.use-background-logo(dialog-general-mdi-bg-use-logo)
    mdi-area.use-background-texture(dialog-general-mdi-bg-use-texture)
    mdi-area.use-background-color(dialog-general-mdi-bg-use-color)
    mdi-area.set_background-logo(dialog-general-mdi-bg-logo)
    mdi-area.set_background-texture(dialog-general-mdi-bg-texture)
    mdi-area.set_background-color(dialog-general-mdi-bg-color)
    icon-resize(dialog-general-icon-size)
    update-all-view-scrollBars(dialog-display-show-scrollbars)
    update-all-view-cross-hair-colors(dialog-display-crosshair-color)
    update-all-view-background-colors(dialog-display-bg-color)
    update-all-view-select-box-colors(
        dialog-display-selectbox-left-color,
        dialog-display-selectbox-left-fill,
        dialog-display-selectbox-right-color,
        dialog-display-selectbox-right-fill,
        dialog-display-selectbox-alpha)
    update-all-viewGridcolors(dialogGridcolor)
    update-all-view-ruler-colors(dialog-ruler-color)

    if dialog-lwt-show-lwt) {
        enable-lwt()
    else) {
        disableLwt()

    if dialog-lwt-real-render) {
        enableReal()
    else) {
        disableReal()

    update-pick-add-mode(dialog-selection-mode-pickadd)

    writeSettings()
    accept()
}*/

/*
TODO: inform the user if they have changed settings

Update the view since the user must accept the preview
*/ /*
void
reject-changes(void)
    mdi-area.use-background-logo(dialog-general-mdi-bg-use-logo)
    mdi-area.use-background-texture(dialog-general-mdi-bg-use-texture)
    mdi-area.use-background-color(dialog-general-mdi-bg-use-color)
    mdi-area.set_background-logo(dialog-general-mdi-bg-logo)
    mdi-area.set_background-texture(dialog-general-mdi-bg-texture)
    mdi-area.set_background-color(dialog-general-mdi-bg-color)
    update-all-view-scroll-bars(dialog-display-show-scrollbars)
    update-all-view-cross-hair-colors(dialog-display-crosshair-color)
    update-all-view-background-colors(dialog-display-bg-color)
    update-all-view-select-box-colors(
        dialog-display-selectbox-left-color,
        dialog-display-selectbox-left-fill,
        dialog-display-selectbox-right-color,
        dialog-display-selectbox-right-fill,
        dialog-display-selectbox-alpha)
    update-all-viewGridcolors(dialogGridcolor)
    update-all-view-ruler-colors(dialog-ruler-color)

    if dialog-lwt-show-lwt) {
        enable-lwt()
    else) {
        disableLwt()

    if dialog-lwt-real-render) {
        enableReal()
    else) {
        disableReal()

    reject()
}

/*
 * For safe packaging, and to reduce the risk of program
 * crashing errors the resources are loaded into the
 * application folder each time the program boots.
 */
void
load-image(char *path)
    FILE *f
    /*
    image-fname = APPLICATION-FOLDER + os.sep + path
    with open(image-fname, "wb") as image-file: {
        icon = icons[path].replace("\n", "")
        icon = icon.replace(" ", "")
        decoded-image = binascii.unhexlify(icon)
        image-file.write(decoded-image)
    }
    return tk.PhotoImage(file=image-fname) */
}

/* Utility for creating the icons datafile.
 */
int
build-icons-json(char *folder)
    /*
    for (file in os.listdir(folder)) {
        if (".png" in file) {
            FILE *f = fopen(folder+os.sep+file, "rb")
            icons[file] = image-file.fread().hex()
            fclose(f)
        }
    } */
    return 0
}

/* 
Whenever the code happens across a todo call, write it in a log file.
void
def todo(char *msg, int action)
    if debug-mode) {
        with open("todo.txt", "a", encoding="utf-8") as logfile) {
            logfile.write(f"{msg}: {action}")
        }
    }
}

Whenever the code happens across a todo call, write it in a log file.
def error(char *msg, int action)
    if debug-mode) {
        with open("error.txt", "a", encoding="utf-8") as logfile) {
            logfile.write(f"{msg}: {action}")
        }
    }
}
*/


/*
def platform-string()
    r" Return the host system label for debugging purposes. "
    host-system = os.uname().sysname + " " + os.uname().release
    debug_message(f"Platform: {host-system}")
    return host-system */



/* The Color class definition.
 */

int color-mode
int stitches-total
int stitches-real
int stitches-jump
int stitches-trim
int color-total
int color-changes

/* 
 * To manage thread colors and such, this class can call the settings
 * JSON dict.
 */
void
color-init(void)
    /* color-mode = COLOR-BACKGROUND
    strcpy(prefix, "Enter RED,GREEN,BLUE values for background or [Crosshair/Grid]: ")
    set_prompt-prefix(translate(prefix)) */
}
 /*
void
color-prompt(char *cmd)
    int output[3], valid, new-mode
    valid = 0
    new-mode = -1
    if (cmd[0] == 'C' || !strcmp(cmd, "CROSSHAIR")) {
        new-mode = COLOR-CROSSHAIR
    }
    if (cmd[0] == 'G' || !strcmp(cmd, "GRID")) {
        new-mode = COLOR-GRID
    }
    if (cmd[0] == 'B' || !strcmp(cmd, "BACKGROUND")) {
        new-mode = COLOR-BACKGROUND
    }
    if (color-mode != new-mode) {
        switch (new-mode) {
        case COLOR-BACKGROUND) {
            color-mode = COLOR-BACKGROUND
            set_prompt-prefix(translate("Specify background color: "))
            break
        case COLOR-CROSSHAIR) {
            color-mode = COLOR-CROSSHAIR
            set_prompt-prefix(translate("Specify crosshair color: "))
            break
        case COLOR-GRID) {
            color-mode = COLOR-GRID
            set_prompt-prefix(translate("Specify grid color: "))
            break
        }
        return
    }
    valid = parse-three-ints(cmd, output)
    switch (color-mode) {
    case COLOR-BACKGROUND) {
        if (valid) {
            setBackgroundColor(output[0], output[1], output[2])
        }
        else {
            alert("Invalid color.", "R,G,B values must be in the range of 0-255.")
            set_prompt-prefix(translate("Specify background color: "))
        }
        break
    case COLOR-CROSSHAIR) {
        if (valid) {
            setCrossHairColor(output[0], output[1], output[2])
        }
        else {
            alert("Invalid color.", "R,G,B values must be in the range of 0-255.")
            set_prompt-prefix(translate("Specify crosshair color: "))
        }
        break
    case COLOR-GRID) {
        if (valid) {
            setGridColor(output[0], output[1], output[2])
        }
        else {
            alert("Invalid color.", "R,G,B values must be in the range of 0-255.")
            set_prompt-prefix(translate("Specify grid color: "))
        }
        break
    default) {
        break
    }
}
*/
/*  The layer manager.
 */
void
layer-manager-init(LayerManager *mgr, int tab)
    /*
    layer-model = tk.StandardItemModel(mw, 0, 8)

    layer-model-sorted = tk.SortFilterPoxyModel()
    layer-model-sorted.setDynamicSortFilter(1)
    layer-model-sorted.setSourceModel(layer-model)

    treeView = tk.TreeView(mw)
    treeView.setRootIsDecorated(0)
    treeView.setAlternatingRowColors(1)
    treeView.setModel(layer-model-sorted)
    treeView.setSortingEnabled(1)
    treeView.sortByColumn(0, "AscendingOrder")

    mainLayout = tk.VBoxLayout()
    mainLayout.addWidget(treeView)
    set_layout(mainLayout)

    set_window-title(translate("Layer Manager"))
    set_minimum-size(750, 550)

    layer-model.set_header-data(0, "horizontal", translate("Name"))
    layer-model.set_header-data(1, "horizontal", translate("Visible"))
    layer-model.set_header-data(2, "horizontal", translate("Frozen"))
    layer-model.set_header-data(3, "horizontal", translate("Z Value"))
    layer-model.set_header-data(4, "horizontal", translate("Color"))
    layer-model.set_header-data(5, "horizontal", translate("Linetype"))
    layer-model.set_header-data(6, "horizontal", translate("Lineweight"))
    layer-model.set_header-data(7, "horizontal", translate("Print"))

    add-layer("0", 1, 0, 0.0, (0, 0, 0), "Continuous", "Default", 1)
    add-layer("1", 1, 0, 1.0, (0, 0, 0), "Continuous", "Default", 1)
    add-layer("2", 1, 0, 2.0, (0, 0, 0), "Continuous", "Default", 1)
    add-layer("3", 1, 0, 3.0, (0, 0, 0), "Continuous", "Default", 1)
    add-layer("4", 1, 0, 4.0, (0, 0, 0), "Continuous", "Default", 1)
    add-layer("5", 1, 0, 5.0, (0, 0, 0), "Continuous", "Default", 1)
    add-layer("6", 1, 0, 6.0, (0, 0, 0), "Continuous", "Default", 1)
    add-layer("7", 1, 0, 7.0, (0, 0, 0), "Continuous", "Default", 1)
    add-layer("8", 1, 0, 8.0, (0, 0, 0), "Continuous", "Default", 1)
    add-layer("9", 1, 0, 9.0, (0, 0, 0), "Continuous", "Default", 1)

    for i in range(layer-model.count())
        treeView.resizeColumnToContents(i)

    Application-setOverrideCursor(Qt-ArrowCursor)

    return self
    */
}

void
layer-manager-add(
    LayerManager *mgr,
    char *name,
    int visible,
    int frozen,
    int zValue,
    char *color,
    char *lineType,
    double line-weight)
    strcpy(mgr->name, name)
    mgr->visible = visible
    mgr->frozen = frozen
    mgr->z-value = zValue
    strcpy(mgr->color, color)
    strcpy(mgr->line-type, lineType)
    mgr->line-weight = line-weight
    /*
    # const print)
    layer-model.insertRow(0)
    layer-model.set_data(layer-model.index(0, 0), name)
    layer-model.set_data(layer-model.index(0, 1), visible)
    layer-model.set_data(layer-model.index(0, 2), frozen)
    layer-model.set_data(layer-model.index(0, 3), zValue)

    colorPix = QPixmap(16, 16)
    colorPix.fill(Color(color))
    layer-model.itemFromIndex(layer-model.index(0, 4)).setIcon(QIcon(colorPix))
    layer-model.set_data(layer-model.index(0, 4), Color(color))

    layer-model.set_data(layer-model.index(0, 5), lineType)
    layer-model.set_data(layer-model.index(0, 6), lineWeight)
    #layer-model.set_data(layer-model.index(0, 7), print)
    */
}

void
layer-model(void)
    return
}

void
layer-model-sorted(void)
    return
}

void
tree-view(void)
    return
}

void
layer-manager-delete(int tab)
    /* restore-override-cursor() */
}

/*
 *  The dialog showing details of the pattern including histograms.
 */


 Creates a dialog showing key information about the pattern,
 ideally this will update as the pattern changes without
 any key presses or clicks.

void
details-dialog-init)
    /* kiss-window dialog = kiss-window()
    dialog-setMinimumSize(750, 550)
    dialog-title(translate("Embroidery Design Details"))
    */

    stitches-total = 0
    stitches-real = 0
    stitches-jump = 0
    stitches-trim = 0
    color-total = 0
    color-changes = 0

    /*
    bounding-rect = Rect(0.0, 0.0, 0.1, 0.1)

    get-info()
    main-widget = create-main-widget()

    button-box = tk.ButtonBox(dialog, text="QDialogButtonBox-Ok") */
    /* # connect(buttonBox, SIGNAL(accepted()), SLOT(accept())) */
    /*
    vbox-layout-main = tk.VBoxLayout()
    vbox-layout-main.addWidget(main-widget)
    vbox-layout-main.addWidget(button-box)
    dialog-set_layout(vbox-layout-main)

    mw.setOverrideCursor("ArrowCursor") */
}

void
details-dialog-free)
    /* mw.restoreOverrideCursor() */
}

 Probably needs to be in C.
void
create-histogram)
  debug_message("TODO: createHistogram"))

 Uses the pattern index.
void
get-info pattern)
    /* TODO: grab this information from the pattern */
    stitches-total = 5
    /* TODO: embStitchList-count(pattern->stitchList, TOTAL) */
    stitches-real = 4
    /* TODO: embStitchList-count(pattern.stitchList, NORMAL) */
    stitches-jump = 3
    /* TODO: embStitchList-count(pattern.stitchList, JUMP) */
    stitches-trim = 2
    /* TODO: embStitchList-count(pattern.stitchList, TRIM) */
    color-total = 1
    /* TODO: embThreadList-count(pattern.threadList, TOTAL) */
    color-changes = 0
    /* TODO: embThreadList-count(pattern.threadList, CHANGES) */
    /* bounding-rect.set_rect(0, 0, 50, 100) */
    /* # TODO: embPattern-calcBoundingBox(pattern) */
}

void
create-main-widget)
    /* widget = tk.Widget(dialog)

 Misc
    /* groupbox-misc = tk.groupbox(translate("General Information"), widget)

    Labels = []
    fields = []

    for i in range(12)
        details-labels = mw.setting-details-label-text
        Labels[i] = tk.Label(translate(details-labels[i]))

    fields[0] = tk.Label(str(stitches-total))
    fields[1] = tk.Label(str(stitches-real))
    fields[2] = tk.Label(str(stitches-jump))
    fields[3] = tk.Label(str(stitches-trim))
    fields[4] = tk.Label(str(color-total))
    fields[5] = tk.Label(str(colorChanges))
    fields[6] = tk.Label(str(bounding-rect.left()) + " mm")
    fields[7] = tk.Label(str(bounding-rect.top()) + " mm")
    fields[8] = tk.Label(str(bounding-rect.right()) + " mm")
    fields[9] = tk.Label(str(bounding-rect.bottom()) + " mm")
    fields[10] = tk.Label(str(bounding-rect.width()) + " mm")
    fields[11] = tk.Label(str(bounding-rect.height()) + " mm")

    grid-layout-misc = GridLayout(groupbox-misc)
    for i in range(12)
        grid-layout-misc.addWidget(tk.Labels[i], i, 0, "Qt-AlignLeft")
        grid-layout-misc.addWidget(fields[i], i, 1, "Qt-AlignLeft")

    grid-layout-misc.setColumnStretch(1, 1)
    groupbox-misc.set_layout(grid-layout-misc)

    /*
    # TODO: Color Histogram

    # Stitch Distribution
    # groupboxDist = Qgroupbox(translate("Stitch Distribution"), widget)

    # TODO: Stitch Distribution Histogram
    */

    /* Widget Layout */
    /* vbox-layout-main = tk.VBoxLayout(widget)
    vbox-layout-main.addWidget(groupbox-misc) */
    /* vbox-layout-main.addWidget(groupboxDist) */
    /* vbox-layout-main.addStretch(1)
    widget.set_layout(vbox-layout-main)

    scroll-area = tk.scroll-area()
    scroll-area.set_widget-resizable(1)
    scroll-area.set_widget(widget)
    return scroll-area */
}

/*
 *  To display an embedded image as a widget in SDL2.
 */
void
image-widget-init(char *filename)
    /* debug_message("ImageWidget Constructor")
    img = img-load(filename)

    min-width = img.width()
    min-height = img.height()
    max-width = img.width()
    max-height = img.height()

    img.show() */
}

void
image-widget-load(char *fileName)
    /* return img.load(fileName) */
}

void
image-widget-save(char *fileName)
    /* return img.save(fileName, "PNG") */
}

/*
 *
 */
void
paintEvent(SDL-Event event)
    /* 
    painter = Painter()
    painter.setViewport(0, 0, img.width(), img.height())
    painter.setWindow(0, 0, img.width(), img.height())
    painter.drawImage(0, 0, img) */
}


/*
    Tooltip manager. SDL doesn't support tooltips out of the box.
    ------------------------------------------------------------
    
    
    Example here) {
    https://stackoverflow.com/questions/3221956/how-do-i-display-tooltips-in-tkinter
    https://gamedev.stackexchange.com/questions/186482/sdl2-show-a-tooltip-at-the-cursor-that-displays-rgb-of-the-pixel-under-the-cur
 *
 *  ------------------------------------------------------------
 *  Preview Dialog
 *  ------------------------------------------------------------
 *
 *  Preview the output.
 * 
 */
/*
void
PreviewDialog-init--(self, parent, caption, dir, filter)
    debug_message("PreviewDialog Constructor")

    #TODO: get actual thumbnail image from file, lets also use a size of 128x128 for now...
    #TODO: make thumbnail size adjustable thru settings dialog
    img-widget = ImageWidget("icons/default/nopreview.png")

    lay = layout()
    if lay) {
        columns = lay.columnCount()
        rows = lay.rowCount()
        lay.addWidget(img-widget, 0, columns, rows, 1)

    modal = 1
    option = "FileDialog-DontUseNativeDialog"
    view-mode = "FileDialog-Detail"
    file-mode = "FileDialog-ExistingFiles"

    #TODO: connect the currentChanged signal to update the preview img-widget.

void
PreviewDialogtoPolyline(self, pattern, obj-pos, obj-path, layer, color, line-type,  line-weight)
    debug_message("Unused arguments:")
    debug_message(f"    {pattern}, {obj-pos}, {obj-path},")
    debug_message(f"    {layer}, {color}, {line-type}, {line-weight}.")
}

 ------------------------------------------------------------

 The color selection docker class definition file.

 The color selection docker.

 From a duplicated version) {

SelectBox(Shape s, QWidget* parent = 0)

def setDirection(int dir)
def setColors(colorL, fillL, colorR, fillR, newAlpha)
def paintEvent(QPaintEvent*)
def forceRepaint()
def force-repaint()
    #HACK: Take that QRubberBand!
    hack = size()
    resize(hack + QSize(1,1))
    resize(hack)
    # WARNING) {
    # DO NOT SET THE QMDISUBWINDOW (this) FOCUSPROXY TO THE PROMPT
    # AS IT WILL CAUSE THE WINDOW MENU TO NOT SWITCH WINDOWS PROPERLY!
    # ALTHOUGH IT SEEMS THAT SETTING INTERNAL WIDGETS FOCUSPROXY IS OK.

    #    gview.setFocusProxy(mw.prompt)

    resize(sizeHint())

    curLayer = "0"
    curColor = 0 #TODO: color ByLayer
    curline-type = "ByLayer"
    curline-weight = "ByLayer"

    # Due to strange Qt4.2.3 feature the child window icon is not drawn
    # in the main menu if showMaximized() is called for a non-visible child window
    # Therefore calling show() first...
    show()
    showMaximized()

    setFocusPolicy(WheelFocus)
    setFocus()

    onWindowActivated()
}

 Default values.

void
select-box-init tools)
    tools->left-brush.color = "#FFFFFF"
    tools->left-brush.style = BRUSH-STYLE-SOLID
    tools->right-brush.color = "#FFFFFF"
    tools->right-brush.style = BRUSH-STYLE-SOLID
    tools->left-pen.color = "#FFFFFF"
    tools->left-pen.style = LINE-STYLE-SOLID
    tools->right-pen.color = "#FFFFFF"
    tools->right-pen.style = LINE-STYLE-SOLID
    tools->alpha = 0xFF
    tools->box-dir = 0
    select-box-direction(tools, tools->box-dir)
      not sure what this line was for 
    select-box-colors("darkGreen", "green", "darkBlue", "blue", 32)
}

 Choose between the left an right tools.

void
select-box-direction tools dir)
    tools->box-dir = dir
    if (dir == DIRECTION-RIGHT) {
        tools->dir-pen = tools->right-pen
        tools->dir-brush = tools->right-brush
    }
    else {
        tools->dir-pen = tools->left-pen
        tools->dir-brush = tools->left-brush
    }
}

 TODO: allow customization.

void
select-box-colors
    tools color-left fill-left color-right fill-right new-alpha)
    debug_message("SelectBox colors()")
    tools->alpha = new-alpha

    tools->left-pen.color = color-left
    tools->left-pen.style = LINE-STYLE-DASHED
    tools->left-brush.color = fill-left + new-alpha
    tools->left-brush.style = BRUSH-STYLE-SOLID

    tools->right-pen.color = color-right
    tools->right-pen.style = LINE-STYLE-SOLID
    tools->right-brush.color = fill-right + new-alpha
    tools->right-brush.style = BRUSH-STYLE-SOLID

    direction(tools->box-dir)

    select-box-force-repaint()
}

 Carry out a paint given the current pen and brush.
 May need the canvas passing in.

void
select-box-paint-event tools event)
    painter = QPainter()
    painter.pen(dir-pen)
    painter.fill-rect(0, 0, width()-1, height()-1, dir-brush)
    painter.draw-rect(0, 0, width()-1, height()-1)
}

 HACK: Take that QRubberBand!

void
select-box-force-repaint)
    hack = size()
    resize(hack + (1, 1))
    resize(hack)
}

void
status-bar-update status-bar)

}

void status-bar-context-menu-event(WindowTab *status-bar, SDL-Event *event)
    setOverrideCursor(Qt-ArrowCursor)
    menu- = QMenu(this)
    switch (object-name) {
    case SNAP:
        settingsSnapAction = Action(load-icon(gridsnapsettings-xpm), "Settings...", menu-)
        connect(settingsSnapAction, SIGNAL(triggered()), this, SLOT(settingsSnap()))
        menu-.addAction(settingsSnapAction)
        break

    case GRID:
        settingsGridAction = Action(load-icon(gridsettings-xpm), "Settings...", menu-)
        connect(settingsGridAction, SIGNAL(triggered()), this, SLOT(settingsGrid()))
        menu-.addAction(settingsGridAction)
        break

    case RULER:
        settingsRulerAction = Action( Icon("icons/rulersettings.png"), "Settings...", menu-)
        connect(settingsRulerAction, SIGNAL(triggered()), this, SLOT(settingsRuler()))
        menu-.addAction(settingsRulerAction)
        break

    case ORTHO:
        settingsOrthoAction = Action( Icon("icons/orthosettings.png"), "Settings...", menu-)
        connect(settingsOrthoAction, SIGNAL(triggered()), this, SLOT(settingsOrtho()))
        menu-.addAction(settingsOrthoAction)
        break

    case POLAR:
        settingsPolarAction = Action( Icon("icons/polarsettings.png"), "Settings...", menu-)
        connect(settingsPolarAction, SIGNAL(triggered()), this, SLOT(settingsPolar()))
        menu-.addAction(settingsPolarAction)
        break

    case QSNAP:
        settingsQSnapAction = Action( Icon("icons/qsnapsettings.png"), "Settings...", menu-)
        connect(settingsQSnapAction, SIGNAL(triggered()), this, SLOT(settingsQSnap()))
        menu-.addAction(settingsQSnapAction)

    case QTRACK:
        settingsQTrackAction = Action( Icon("icons/qtracksettings.png"), "Settings...", menu-)
        connect(settingsQTrackAction, SIGNAL(triggered()), this, SLOT(settingsQTrack()))
        menu-.addAction(settingsQTrackAction)
        break

    case LWT:
        gview = main-win.active-view()
        if (gview) {
            enable-real-action = Action(Icon("icons/realrender.png"), "RealRender On", menu-)
            enable-real-action.setEnabled(!gview.isRealEnabled())
            connect(enable-real-action, SIGNAL(triggered()), this, SLOT(enableReal()))
            menu-.addAction(enable-real-action)

            disable-real-action = Action(Icon("icons/realrender.png"), "RealRender Off", menu-)
            disable-real-action.setEnabled(gview.isRealEnabled())
            connect(disable-real-action, SIGNAL(triggered()), this, SLOT(disableReal()))
            menu-.addAction(disable-real-action)
        }

        settingsLwtAction = Action(load-icon(lineweightsettings-xpm), "Settings...", menu-)
        connect(settingsLwtAction, SIGNAL(triggered()), this, SLOT(settingsLwt()))
        menu-.addAction(settingsLwtAction)
        break

    default:
        break
    }

    menu-.exec(event.globalPos())
    restoreOverrideCursor()
    statusbar.clearMessage()
    */
}

 TODO: set format from settings.

void status-bar-mouse-coord(WindowTab *tab, int x, int y)
    if (tab->number-mode == ARCHITECTURAL) {
        return
    }
    if (tab->number-mode == ENGINEERING) {
        return
    }
    if (tab->number-mode == FRACTIONAL) {
        return
    }
    if (tab->number-mode == SCIENTIFIC) {
        /* status-bar-mouse-coord.setText("".setNum(x, 'E', 4)
            + ", " + "".setNum(y, 'E', 4))
        # TODO: use precision from unit settings */
        return
    }
    /* Else decimal */
    /*
     * status-bar-mouse-coord.setText("".setNum(x, 'F', 4) + ", "
     * + "".setNum(y, 'F', 4))
     * #TODO: use precision from unit settings */
}

void status-bar-init(WindowTab *status-bar)
    /*
void
--init--(self, buttonText, mw, statbar, parent)
    statusbar = statbar

    this.set_object-name("StatusBarButton" + buttonText)

    this.setText(buttonText)
    this.setAutoRaise(1)
    this.setCheckable(1)

    if (object-name >= 0 && object-name < N-STATUS) {
        status[object-name] = !status[object-name]
    }
}

 class StatusBar()
/*
 * StatusBar( QWidget* parent = 0)
 * def setMouseCoord(x, y)
 */
/*  def --init--(self, mw, parent)
    this.set_object-name("StatusBar")

    for i in range(N-STATUS)
        status-bar[i] = StatusBarButton(status-bar-label[i], main-win, this, this)

    status-bar-mouse-coord = tk.Label(this)

    status-bar-mouse-coord.set_minimum-width(300)
    # Must fit this text always
    status-bar-mouse-coord.set_maximum-width(300)
    # "+1.2345E+99, +1.2345E+99, +1.2345E+99"

    this.addWidget(status-bar-mouse-coord)
    for i in range(N-STATUS) {
        this.addWidget(status-bar[i])
    }
    */
}

/* The View class definition file.
 *
 * How we manage the content of the main editing area in the window.
 *
 * There are 4 regions managed as views, .
 *
 * We don't have a seperate window for the pop-ups like the file
 * browser for opening or saving a file. Instead, a view will
 * be created.
 */
void
view_init(view, the-scene)
{
    gscene = the-scene;
    frame-shape = "No frame";
    hash-deleted-objects = {};
    sparerubber-list = [];
    pan-distance = 0.0;
    pan-start-x = 0.0;
    pan-start-y = 0.0;
    rubber-room-list = 0;
    gripBase-obj = 0;
    tempBase-obj = 0;
    select-box = 0;
    cut-copy-mouse-point = 0;
    paste-object-item-group = 0;
    preview-object-list = 0;
    preview-object-item-group = 0;
    preview-point = 0;
    preview-data = 0;
    preview-mode = 0;
    origin-path = Path();
    grid-color = Color();
    grid-path = Path();

    /* 
    NOTE: self has to be done before setting mouse tracking.
    TODO: Review OpenGL for Qt5 later
    if settings.INT-DISPLAY-USE-OPENGL:
        debug_message("Using OpenGL...")
        setViewport(new QGLWidget(QGLFormat(QGL-DoubleBuffer)))
     */

    /* TODO: Review RenderHints later
    # set_render-hint(Painter-Antialiasing,
    #               display-render-hintAA())
    # set_render-hint(Painter-TextAntialiasing,
    #               display-render-hintTextAA())
    # set_render-hint(Painter-SmoothPixmapTransform,
    #               display-render-hintSmoothPix())
    # set_render-hint(Painter-HighQualityAntialiasing,
    #               display-render-hintHighAA())
    # set_render-hint(Painter-NonCosmeticDefaultPen,
    #               display-render-hint-noncosmetic)

    # NOTE
    # ----
    # FullViewportUpdate MUST be used for both the GL and Qt renderers.
    # Qt renderer will not draw the foreground properly if it isnt set.
    */ /*

    setViewportUpdateMode(Graphicsdef-FullViewportUpdate)

    pan-distance = 10;
    #TODO: should there be a setting for self???

    set_cursor("BlankCursor")
    horizontal-scroll-bar().set_cursor("ArrowCursor")
    vertical-scroll-bar().set_cursor("ArrowCursor")
    qsnap-locator-color = qsnap-locator-color
    grip-color-cool = selection-coolgrip-color
    grip-color-hot = selection-hotgrip-color
    set_crosshair-color(display-crosshair-color))
    set_cross-hair-size(display-crosshair-percent)
    setgrid-color(grid-color)

    if INTGridSHOW-ON-LOAD:
        create-grid(grid-type)
    else {
        create-grid("")

    toggleRuler(ruler-show-on-load)
    toggleReal(1)
    #TODO: load self from file, else settings with default being 1

    gripping-active = 0
    rapid-move-active = 0
    preview-mode = "PREVIEW-MODE-NULL"
    preview-data = 0
    preview-object-item-group = 0
    paste-object-item-group = 0
    preview-active = 0
    pasting-active = 0
    moving-active = 0
    selecting-active = 0
    zoom-window-active = 0
    panning-real-time-active = 0
    panning-point-active = 0
    panning-active = 0
    qsnap-active = 0
    qsnap-toggle = 0
*/
    /* Randomize the hot grip location initially so it's not located at (0,0). */ /*
    srand(1234)

    sceneGripPoint = Vector((rand()%1000)*0.1, (rand()%1000)*0.1)

    gripBase-obj = 0
    tempBase-obj = 0

    select-box = select-box("RubberBand-Rectangle")
    select-box.set_colors(
        display-select-box-left-color,
        display-select-box-left-fill,
        display-select-box-right-color,
        display-select-box-right-fill,
        display-select-box-alpha)

    show-scroll-bars(display-show-scrollbars)
    set_corner-button()

    installEventFilter(void)

    setMouseTracking(1)
    set_background-color(display-bg-color)
    /*  TODO: wrap self with a setBackgroundPixmap() function:
          set_background-brush(Pixmap("images/canvas))

    /* connect(gscene, SIGNAL(selection-changed()), self,
        SLOT(selection-changed())) /*
}

 Prevent memory leaks by deleting any objects that were removed from the scene.

void
delete-objects)
  (delete-all hash-deleted-objects.begin() hash-deleted-objects.end())
  (clear hash-deleted-objects)

   Prevent memory leaks by deleting any unused instances
  (delete-all preview-object-list.begin() preview-object-list.end())
  (clear preview-object-list)
}

 Selected objects are stored as a global variable indexed by tab.
 delta is a vector.

void
move-selected delta)

}

 Selected objects are stored as a global variable indexed by tab.
 point1 and point2 are vectors.

void
mirror-selected point1 point2)

}

 Sets the crosshair color for the current view.

void
set_crosshair-color color)

}

void
set_crosshair-size(self, percent)
    return
}

void
load-ruler-settings(void)
    return
}

void
center(void)
    return map-to-scene(rect().center())
}

void
enter-event(self, event)
    mdi-win = mw
    if mdi-win:
        mdiArea.setActiveSubWindow(mdi-win)
}

void
add-object(self, obj)
    gscene.addItem(-obj)
    scene_update()
    hash-deleted-objects.remove(-obj.-objID)
}

void
delete-object(self, obj)
    # NOTE: We really just remove the objects from the scene.
    #       deletion actually occurs in the destructor.
    obj.set_selected(0)
    gscene.remove-item(-obj)
    scene_update()
    hash-deleted-objects.insert(-obj.-objID, obj)
}

void
preview-on(self, clone, mode, x, y, data)
    debug_message("View preview-on()")
    preview-off()
    # Free the old objects before creating ones

    preview-mode = mode

    # Create objects and add them to the scene in an item group.
    if clone == "PREVIEW-CLONE-SELECTED":
        preview-object-list = create-object-list(gscene.selected-items())
    elif clone == PREVIEW-CLONE-RUBBER:
        preview-object-list = create-object-list(rubber-room-list)
    else {
        return
    preview-object-item-group = gscene.create-item-group(preview-object-list)

    if (preview-mode == PREVIEW-MODE-MOVE ||
        preview-mode == "PREVIEW-MODE-ROTATE" ||
        preview-mode == "PREVIEW-MODE-SCALE") {
        preview-point = Vector(x, y)
        # NOTE: Move: basePt
        # Rotate: basePt
        # Scale: basePt
        preview-data = data
        # NOTE: Move: unused
        # Rotate: refAngle
        # Scale: refFactor
        preview-active = 1
    }
    else {
        preview-mode = PREVIEW-MODE-NULL
        preview-point = Vector()
        preview-data = 0
        preview-active = 0
    }

    scene_update()
}

void
preview-off(void)
    " , "
    # Prevent memory leaks by deleting any unused instances.
    DeleteAll(preview-object-list.begin(), preview-object-list.end())
    preview-object-list.clear()

    if preview-object-item-group:
        gscene.remove-item(preview-object-item-group)
        del preview-object-item-group
        preview-object-item-group = 0

    preview-active = 0

    scene_update()
*/

void
enable-move-rapid-fire(void)
    debug_message("Enabling rapid move.")
    rapid-move-active = 1
}

void
disable-move-rapid-fire(void)
    debug_message("Disabling rapid move.")
    rapid-move-active = 0
}


void
setGridcolor(EmbColor color)
{
    grid_color = color;
    gscene.set_property("VIEW-COLOR-GRID", color);
    if (gscene) {
        scene_update();
    }
}

void
set_ruler-color(EmbColor color)
{
    void ruler-color) color)
    update-scene();
}

void
set_grid(int active)
{
    debug_message("View toggleGrid()")
    /* set_override-cursor("WaitCursor")
    if on) {
        create-grid(grid-type)
    else {
        create-grid("")
    }
    restore_override_cursor();
}

void
set_ruler(int active)
{
    debug_message("View toggle-ruler()");
    /*
    set_override-cursor("WaitCursor")
    gscene.set_property("ENABLE-RULER", active)
    ruler-color = Color(ruler-color)
    scene_update();
    restore_override_cursor();
}

void
set_ortho(int active)
{
    debug_message("View toggleOrtho()");
    /* set_override-cursor("WaitCursor")
    /* TODO: finish this. */ /*
    gscene.set_property("ENABLE-ORTHO", active);
    scene_update();
    restore_override_cursor();
}

void
set_polar(int active)
{
    debug_message("View togglePolar()");
    /*
    set_override-cursor("WaitCursor");
    /* TODO: finish this. */ /*
    gscene.set_property("ENABLE-POLAR", active);
    scene_update();
    restore-override-cursor();
}

void
set_qsnap(int active)
{
    debug_message("View toggleQSnap()");
    /*
    set_override-cursor("WaitCursor");
    qsnap-toggle = on;
    gscene.set_property("ENABLE-QSNAP", active);
    scene_update();
    restore-override-cursor();
}

/* TODO: finish this.
 */
void
set_qtrack(int active)
{
    debug_message("View toggleQTrack()");
    /*
    set_override-cursor("WaitCursor");
    gscene.set_property("ENABLE-QTRACK", active);
    scene_update();
    restore-override-cursor();
    */
}

void
set_lwt(int active)
{
    debug_message("View toggleLwt()");
    /*
    set_override-cursor("WaitCursor");
    gscene.set_property("ENABLE-LWT", active);
    scene_update();
    restore-override-cursor();
}

void
set_real(int active)
{
    debug_message("View toggleReal()");
    /*
    set_override-cursor("WaitCursor");
    gscene.set_property("ENABLE-REAL", active);
    scene_update();
    restore-override-cursor();
}

/* lwt and real should be per-document
 */
void
is-lwt-enabled(void)
    /* return gscene.property("ENABLE-LWT") */
}

void
is-real-enabled(void)
    /* return gscene.property("ENABLE-REAL") */
}

void
update-mouse-coords(EmbVector position)
    /*
    view-mouse-point = Vector(x, y)
    scene-mouse-point = to-emb-vector(map-to-scene(view-mouse-point))
    gscene.set_property("SCENE-QSNAP-POINT", scene-mouse-point) */
    /* TODO: if qsnap functionality is enabled, use it rather than the mouse point. */ /*
    gscene.set_property("SCENE-MOUSE-POINT", scene-mouse-point)
    gscene.set_property("VIEW-MOUSE-POINT", view-mouse-point)
    statusbar.setMouseCoord(scene-mouse-point.x, -scene-mouse-point.y)
}

/* 
 * NOTE:
 * crosshair-size is in pixels and is a percentage of your screen width
 * Example: (1280*0.05)/2 = 32, thus 32 + 1 + 32 = 65 pixel wide crosshair.
 */
void
set_crossHairSize(double percent)
    /*
    screen-width = qApp.screens()[0].geometry().width()
    if (percent > 0 && percent < 100) {
        crosshair-size = (screen-width*(percent/100.0))/2
    }
    else {
        crosshair-size = screen-width
    }
    */
}

void
set_corner-button(void)
    /*
    num = display-scrollbar-widget-num
    if (num) {
        cornerButton = tk.PushButton(void)
        cornerButton.setFlat(1)
        act = action-hash.value(num)
        /* NOTE: Prevent crashing if the action is NULL. */ /*
        if (!act) {
            error-title = translate("Corner Widget Error")
            message = translate("There are unused enum values in COMMAND-ACTIONS. Please report self as a bug.")
            information(error-title, message)
            setCornerWidget(0)
        }
        else {
            cornerButton.setIcon(act.icon())
            # connect(cornerButton, SIGNAL(clicked()), self, SLOT(cornerButtonClicked()))
            setCornerWidget(cornerButton)
            cornerButton.set_cursor(Qt-ArrowCursor)
        }
    }
    else {
        setCornerWidget(0)
    }
    */
}

void
cornerButtonClicked(void)
{
    /*
    debug_message("Corner Button Clicked.");
    display = display-scrollbar-widget-num;
    action-hash.value(display).trigger();
    */
}

void
selection-changed(void)
{
    /*
    if dock-prop-edit.isVisible()
        dock-prop-edit.setselected-items(gscene.selected-items());
    */
}

void
mouse-double-click-event(SDL-Event event)
{
     /*
    if event.button() == "LeftButton":
        item = gscene.itemAt(map-to-scene(event.pos()), QTransform());
        if item:
            dock-prop-edit.show();
        }
    }
    */
}

void
mouse_press_event(SDL-Event event)
{
    update-mouse-coords(event.x, event.y);
    if (event.button() == "LeftButton") {
        path = Path();
        pickList = gscene.items(Rect(map-to-scene(
            view-mouse-point.x()-pickBoxSize,
            view-mouse-point.y()-pickBoxSize),
            map-to-scene(view-mouse-point.x()+pickBoxSize,
            view-mouse-point.y()+pickBoxSize)));

        itemsInPickBox = pickList.size();
        if (itemsInPickBox && !selecting-active && !gripping-active) {
            itemsAlreadelta-ySelected = pickList.at(0).is-selected();
            if (!itemsAlreadelta-ySelected) {
                pickList.at(0).set_selected(1);
            }
            else {
                foundGrip = 0;
                base = pickList[0];
                #TODO: Allow multiple objects to be gripped at once
                if (!base) {
                    return;
                }

                qsnap-offset = Vector(qsnap-locator-size, qsnap-locator-size);
                gripPoint = base.mouseSnapPoint(scene-mouse-point);
                p1 = map-from-scene(gripPoint) - qsnap-offset;
                q1 = map-from-scene(gripPoint) + qsnap-offset;
                gripRect = Rect(map-to-scene(p1), map-to-scene(q1));
                pickRect = Rect(map-to-scene(view-mouse-point.x()-pickBoxSize, view-mouse-point.y()-pickBoxSize),
                                map-to-scene(view-mouse-point.x()+pickBoxSize, view-mouse-point.y()+pickBoxSize))
                if gripRect.intersects(pickRect)
                    foundGrip = 1;

                /* If the pick point is within the item's grip box,
                    * start gripping.
                    */ /*
                if (foundGrip) {
                    start-gripping(base);
                }
                else { */
                    /* start moving */ /*
                    moving-active = 1;
                    pressPoint = event.pos();
                    scenePressPoint = map-to-scene(pressPoint);
                }

        elif (gripping-active) {
            stop-gripping(1);
        }
        elif (!selecting-active) {
            selecting-active = 1;
            pressPoint = event.pos();
            scenePressPoint = map-to-scene(pressPoint);

            if (!select-box:
                select-box = select-box(QRubberBand-Rectangle);
            select-box.set_geometry(Rect(pressPoint, pressPoint));
            select-box.show();
        }
        else {
            selecting-active = 0;
            select-box.hide();
            releasePoint = event.pos();
            scene-release-point = map-to-scene(releasePoint);

            #Start select-box Code
            path.add-polygon(map-to-scene(select-box.geometry()));
            if (scene-release-point.x > scenePressPoint.x) {
                if (selection-mode-pickadd) {
                    if (shift-key) {
                        item-list = gscene.items(path, "ContainsItemShape");
                        for item in item-list: {
                            item.set_selected(0);
                        }
                    }
                    else {
                        item-list = gscene.items(path, "ContainsItemShape");
                        for item in item-list: {
                            item.set_selected(1);
                        }
                    }
                }
                else {
                    if (shift-key) {
                        item-list = gscene.items(path, "ContainsItemShape");
                        if (!item-list.size() {
                            clear-selection();
                        }
                        else {
                            for item in item-list: {
                                item.set_selected(!item.is-selected()) #Toggle selected;
                            }
                        }
                    }
                    else {
                        clear-selection();
                        item-list = gscene.items(path, "ContainsItemShape");
                        for item in item-list: {
                            item.set_selected(1);
                        }
                    }
            }
            else {
                if (selection-mode-pickadd) {
                    if (shift-key) {
                        item-list = gscene.items(path, "intersects-item-shape");
                        for item in item-list) {
                            item.set_selected(0);
                        }
                    }
                    else {
                        item-list = gscene.items(path, "intersects-item-shape");
                        for (item in item-list) {
                            item.set_selected(1);
                        }
                    }
                }
                else {
                    if (shift-key) {
                        item-list = gscene.items(path, "intersects-item-shape");
                        if (!item-list.size(void);
                            clear-selection();

                        else {
                            for item in item-list:
                                item.set_selected(!item.is-selected());
                                #Toggle selected

                    }
                    else {
                        clear-selection();
                        item-list = gscene.items(path, "intersects-item-shape");
                        for item in item-list) {
                            item.set_selected(1);
                        }
                   }
               }
            #End select-box Code

        if (pasting-active) {
            item-list = paste-object-item-group.childItems();
            gscene.destroy-item-group(paste-object-item-group);
            for item in item-list:
                gscene.remove-item(item);
                # Prevent Qt Runtime Warning,
                # QGraphicsScene-addItem: item has alreadelta-y been
                # added to self scene

            for item in item-list:
                if item:
                    debug_message("TODO: Loop bodelta-y");

            pasting-active = 0
            selecting-active = 0

        if zoom-window-active:
            fit-in-view(path.bounding-rect(), Qt-KeepAspectRatio);
            clear-selection();

    if event.button() == "MiddleButton":
        pan-start(event.pos());
        #The Undo command will record the spot where the pan started.
        event.accept();

    scene_update();
*/
}

void
pan_start(EmbVector point)
{
    /*
    recalculate-limits();

    align-scene-point-withViewPoint(map-to-scene(point), point);

    panning-active = 1;
    pan-start = point;
    */
}

/*
 * NOTE:
 * Increase the scene-rect limits if the point we want to go to lies outside of scene-rect's limits
 * If the scene-rect limits aren't increased, you cannot pan past its limits
 */
void
recalculate-limits(void)
{
    SDL-Rect scene-rect, new-rect, rect;
    viewRect = Rect(map-to-scene(rect().top-left()), map-to-scene(rect().bottom-right()));
    scene-rect = gscene.scene-rect();
    new-rect = viewRect.adjusted(
        -viewRect.width(), -viewRect.height(),
        viewRect.width(), viewRect.height());
    if (!scene-rect.contains(new-rect.top-left()) || !scene-rect.contains(new-rect.bottom-right()) {
        rect = scene-rect.adjusted(
            -viewRect.width(), -viewRect.height(),
            viewRect.width(), viewRect.height());
        gscene.setscene-rect(rect);
    }
}

/*
 * center-on also updates the scrollbars,
 * which shifts things out of wack o-O
 */
void
center_at(EmbVector centerPoint)
{
     /*
    center-on(centerPoint);
    # Reshift to the center
    offset = centerPoint - center();
    new-center = centerPoint + offset;
    center-on(new-center);
    */
}

/* center-on also updates the scrollbars, which shifts things out of wack o-O
 */
void
align-scene-point-with-view-point(EmbVector scene-point, EmbVector view-point)
{
    view-center = center();
    point-before = scene-point;
    center-on(view-center);
    #Reshift to the center so the scene and view points align
    point-after = map-to-scene(view-point);
    offset = point-before - point-after;
    new-center = view-center + offset;
    center-on(new-center);
}

void
mouse-move-event(SDL-Event event)
{
    mouse = Cursor-pos();
    update-mouse-coords(mouse.x(), mouse.y());
    move-point = event.pos();
    scenemove-point = map-to-scene(move-point);

    if (preview-active) {
        /*
        if (preview-mode == "PREVIEW-MODE-MOVE") {
            preview-object-item-group.set_pos(scene-mouse-point - preview-point);
        }
        elif (preview-mode == "PREVIEW-MODE-ROTATE") {
            x = preview-point.x();
            y = preview-point.y();
            mouse-angle = Line(x, y, scene-mouse-point.x, scene-mouse-point.y).angle();

            rad = radians(preview-data-mouse-angle);
            p = Vector(-x, -y);
            rot = rotate-vector(p, rad);
            rot.x += x;
            rot.y += y;

            preview-object-item-group.set_pos(rot.x, rot.y);
            preview-object-item-group.set_rotation(preview-data-mouse-angle);
        }
        elif (preview-mode == "PREVIEW-MODE-SCALE") {
            x = preview-point.x;
            y = preview-point.y;
            scale-factor = preview-data;

            factor = Line(x, y, scene-mouse-point.x, scene-mouse-point.y).length() / scale-factor;

            preview-object-item-group.setScale(1);
            preview-object-item-group.set_pos(0,0);

            if (scale-factor <= 0.0) {
                message = "Hi there. If you are not a developer, report this as a bug.";
                message += " If you are a developer, your code needs examined,";
                message += " and possibly your head too.";
                critical(self, translate("scale-factor Error"), translate(message));
            }
            else {
                # Calculate the offset
                old = Vector(0, 0);
                scale-line = Line(x, y, old.x, old.y);
                scale-line.set_length(scale-line.length()*factor);

                delta-x = scale-line.point2-x() - old.x;
                delta-y = scale-line.point2-y() - old.y;
            }
        }
    }
    /*
                preview-object-item-group.setScale(preview-object-item-group.scale()*factor);
                preview-object-item-group.moveBy(delta-x, delta-y);
    */

    if (pasting-active) {
        /*
        v = scene-mouse-point.subtract(pasteDelta);
        paste-object-item-group.set_pos(v);
        */
    }

    if (moving-active) {
        /* Ensure that the preview is only shown if the mouse has moved. */
        if (!preview-active) {
            /* preview-on("PREVIEW-CLONE-SELECTED", "PREVIEW-MODE-MOVE",
                   scenePressPoint.x(), scenePressPoint.y(), 0)  */
        }
    }

    if (selecting-active) {
        /*
        if (scenemove-point.x() >= scenePressPoint.x(void)) {
            select-box.setDirection(1);
        }

        else { select-box.setDirection(0);
        select-box.set_geometry(
            Rect(map-from-scene(scenePressPoint), event.pos()).normalized());
        event.accept();
        */
    }

    if (panning-active) {
        /*
        horizontal-scroll-bar().set_value(
            horizontal-scroll-bar().value() - (event.x() - pan-start-x));
        vertical-scroll-bar().set_value(
            vertical-scroll-bar().value() - (event.y() - pan-start-y));
        pan-start.x = event.x;
        pan-start.y = event.y;
        event.accept();
        */
    }

    scene_update();
}

void
mouse-release-event(SDL-Event event)
{
    update-mouse-coords(event);
    if (event.button() == "left-button") {
        if (moving-active) {
            preview-off();
            delta = scene-mouse-point.subtract(scene-press-point);
            /* Ensure that moving only happens if the mouse has moved. */ /*
            if (delta.x or delta.y) {
                moveSelected(delta);
            }
            moving-active = 0;
        }

        event.accept();
    }

    if (event.button() == "MiddleButton") {
        panning-active = 0;
        /* The Undo command will record the spot where the pan completed. */ /*
        event.accept();
    }

    if (event.button() == "XButton1") {
        debug_message("XButton1");
        main-undo() */
        /*  TODO: Make this customizable */ /*
        event.accept();
    }

    if (event.button() == "XButton2") {
        debug_message("XButton2");
        main-redo() */
        /* TODO: Make this customizable */ /*
        event.accept();
    }
    scene_update();
    */
}

void
wheel_event(SDL-Event event)
{
    double zoomDir = 1.0;
    /* zoomDir = event.pixel-delta().y */
    /*  TODO: double check self */
    /*mousePoint = event.global-pos() */
    /* TODO: self is causing weird versioning errors, */
    /* this appears to be supported on Qt5.12. */
    /* update-mouse-coords(mousePoint.x(), mousePoint.y()) */
    if (zoomDir > 0) {
        debug_message(".");
    }
    else {
        debug_message(".");
    }
}

void
zoom-to-point(EmbVector mouse-point, float zoom-dir)
{
    double s;
    /*
    point-before-scale(map-to-scene(mouse-point))

    # Do The zoom
    s = 1.0;
    if (zoom-dir > 0) {
        if (!allow-zoom-in()) {
            return;
        }
        s = display-zoom-scale-action-in;
    }
    else {
        if (!allow-zoom-out()) {
            return;
        }
        s = display-zoom-scale-action-out;
    }

    scale(s, s);
    align-scene-point-with-view-point(point-before-scale, mouse-point);
    recalculate-limits();
    align-scene-point-with-view-point(point-before-scale, mouse-point);

    update-mouse-coords(mouse-point.x(), mouse-point.y());
    if pasting-active:
        v = scene-mouse-point.subtract(paste-delta);
        paste-object-item-group.set_pos(v);

    if (selecting-active) {
        rect = Rect(map-from-scene(scenePressPoint), mousePoint).normalized();
        select-box.set_geometry(rect);
    }

    scene_update();
}

void
context-menu-event(SDL-Event event)
{
    int i;
    iconTheme = general-icon-theme;

    menu = ""
    item-list = gscene.selected-items()
    selection-empty = item-list.is-empty()

    for (i=0 i<len(item-list) i++) {
        if (item-list[i].type != "Not Set") {
            selection-empty = 0
            break
        }
    } */

    if (pasting-active) {
        return;
    }

    /*
    if (zoom-window-active) {
        cancel-zoom-win-action = Action("&Cancel (zoom-window-action)");
        cancel-zoom-win-action.set_status-tip("Cancels the zoom-window-action Command.");
        # connect(cancel-zoom-win-action, SIGNAL(triggered()), self, SLOT(escapePressed()));
        menu.add-action(cancel-zoom-win-action);
    }

    menu.add-separator();
    menu.add-action(action-hash.value("cut"));
    menu.add-action(action-hash.value("copy"));
    menu.add-action(action-hash.value("paste"));
    menu.add-separator();

    if (!selection-empty) {
        delete-action = Action(load-icon(erase-xpm), "D&elete");
        status-tip = "Removes objects from a drawing.";
        delete-action.set_status-tip(status-tip);
        # connect(delete-action, SIGNAL(triggered()), self, SLOT(deleteSelected()));
        menu.add-action(delete-action);

        move-action = Action(load-icon(move-xpm), "&Move");
        status-tip = "Displaces objects a specified distance in a specified direction.";
        move-action.set_status-tip(status-tip);
        connect(move-action, SIGNAL(triggered()), self, SLOT(move-action())) /*;
        menu.add-action(move-action);

        scale-action = Action(load-icon(scale-xpm), "Sca&le");
        status-tip = "Enlarges or reduces objects proportionally in the X, Y, and Z directions.";
        scale-action.set_status-tip(status-tip);
        */ /* connect(scale-action, SIGNAL(triggered()), self, SLOT(scale-action())) /*
        menu.add-action(scale-action);

        rotate-action = Action(load-icon(rotate-xpm), "R&otate");
        status-tip = "Rotates objects about a base point.";
        rotate-action.set_status-tip(status-tip);
        connect(rotate-action, SIGNAL(triggered()), self, SLOT(rotate-action())); /*
        menu.add-action(rotate-action);

        menu.add-separator();

        clear-action = Action("Cle&ar Selection");
        clear-action.set_status-tip("Removes all objects from the selection set.");
        connect(clear-action, SIGNAL(triggered()), self, SLOT(clear-selection())); /*
        menu.add-action(clear-action);
    }

    menu.exec(event.globalPos());
}

void
delete_pressed(void)
{
    debug_message("View delete-pressed()")
    /*
    set_override-cursor = tk.Application("WaitCursor")
    mdi-win = mdi-area.active-sub-window()
    if (mdi-win) {
        mdi-win.deletePressed()
    }
    restore-override-cursor()
    */
    if (pasting-active) {
        /*
        gscene.remove-item(paste-object-item-group)
        del paste-object-item-group
        */
    }

    pasting-active = 0
    zoom-window-active = 0
    selecting-active = 0
    /*
    select-box.hide()
    stop-gripping(0)
    deleteSelected()
    */
}

void
escape-pressed(scheme *sc)
{
    debug_message("View escape-pressed()");
    if (pasting-active) {
        /*
        gscene.remove-item(paste-object-item-group)
        del paste-object-item-group
        */
    }

    pasting-active = 0;
    zoom-window-active = 0;
    selecting-active = 0;
    /*
    select-box.hide()
    if (gripping-active) {
        stop-gripping(0)
    }
    else {
        clear-selection()
    }
    set_override-cursor("WaitCursor")
    mdi-win = mdi-area.active-sub-window()
    if mdi-win:
        mdi-win.escapePressed()
    restoreOverrideCursor()

    gview = active-view()
    if (gview) {
        gview.clearRubberRoom()
        gview.previewOff()
        gview.disableMoveRapidFire()
    }
    */
}


PropertyBox arc-tab = {
    "Arc Properties",
    OBJ-TYPE-ARC,
    arc-properties
};
#endif

