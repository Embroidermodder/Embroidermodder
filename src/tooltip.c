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
 *  Tooltip manager. SDL doesn't support tooltips out of the box.
 *  ------------------------------------------------------------
 *
 *  Example here
 *  https://stackoverflow.com/questions/3221956/how-do-i-display-tooltips-in-tkinter
 *  https://gamedev.stackexchange.com/questions/186482/sdl2-show-a-tooltip-at-the-cursor-that-displays-rgb-of-the-pixel-under-the-cur
 *
 */

#include "em2.h"


void
scene_update(void)
{

}

void
set_override_cursor(char *cursor)
{
    printf("cursor: %s\n", cursor);
}

void
restore_override_cursor(void)
{

}

/* This may need to be a method of Main Window.
 */
void
clear_selection(void)
{
    debug_message("clear_selection");
    n_selected = 0;
    /* gscene.clear_selection();
    prompt = ""; */
}

void
enableReal(void)
{

}


void
disableReal(void)
{

}

void
enableLwt(void)
{

}


void
disableLwt(void)
{

}


EmbVector map_to_scene(double x, double y)
{
    char a[100];
    char *current_scene = get_str(mainwnd->state, "scene");
    sprintf(a, "%s-offset", current_scene);
    EmbVector result = get_vector(mainwnd->state, a);
    result.x = x;
    result.y = y;
    return result;
}

/*
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
    status_bar = toolButton = [
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

    Label status_bar-mouse-coord;

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
draw_background(int scene, Painter *painter, SDL_Rect rect)
{
    printf("called with: %d %d", scene, rect.x);
    /*
    fill_rect(painter, rect, backgroundBrush());

    a = rect.intersects(grid_path.controlPointRect());
    */
    int a = 1;
    if (get_int(mainwnd->state, "enable-grid") && a) {
        EmbColor grid_color;
        grid_color.r = 0;
        grid_color.g = 0;
        grid_color.b = 0;
        painter->pen->color = grid_color;
        painter->pen->join_style = MITER_JOIN;
        painter->pen->cosmetic = 1;
        /*
        draw_path(painter, grid-path);
        draw_path(painter, origin_path);
        fill_path(painter, origin_path, grid_color);
        */
    }
}

int
char_to_int(char a)
{
    if (a >= '0' && a <= '9') {
        return a-'0';
    }
    if (a >= 'A' && a <= 'F') {
        return a-'A'+10;
    }
    if (a >= 'a' && a <= 'f') {
        return a-'a'+10;
    }
    return 0;
}

EmbColor
get_color(char state[2*MAX_VARIABLES][MAX_STRING_LENGTH], char *key)
{
    EmbColor color;
    char *s = get_str(state, key);
    color.r = 16*char_to_int(s[0]) + char_to_int(s[1]);
    color.g = 16*char_to_int(s[2]) + char_to_int(s[3]);
    color.b = 16*char_to_int(s[4]) + char_to_int(s[5]);
    return color;
}

EmbVector view_mouse_point;

void
draw_crosshair(Painter *painter)
{
    /* painter.setBrush("NoBrush"); */
    Pen *crosshair_pen = create_pen();
    crosshair_pen->color = get_color(mainwnd->state, "crosshair-color");
    crosshair_pen->cosmetic = 1;
    painter->pen = crosshair_pen;

    horizontal_rule(
        mainwnd,
        view_mouse_point.x,
        view_mouse_point.y-get_int(mainwnd->state, "crosshair-size"),
        view_mouse_point.y+get_int(mainwnd->state, "crosshair-size"),
        ALWAYS_VISIBLE);

    vertical_rule(
        mainwnd,
        view_mouse_point.y,
        view_mouse_point.x-get_int(mainwnd->state, "crosshair-size"),
        view_mouse_point.x+get_int(mainwnd->state, "crosshair-size"),
        ALWAYS_VISIBLE);

    /*
    top-left = map_to_scene(
        view_mouse_point.x-settings.selection-pickbox-size,
        view_mouse_point.y-settings.selection-pickbox-size);
    bottom-right = map_to_scene(view_mouse_point.x+settings.selection-pickbox-size,
        view_mouse_point.y+settings.selection-pickbox-size);
    painter.drawRect(Rect(top-left, bottom-right));

    p = QPixmap-grabWindow(winId());
    p.save(QString("test.bmp"), "bmp");
    */
}

/* TODO: and findClosestSnapPoint == 1 */
void
draw_closest_qsnap(Painter *painter)
{
    /* printf("%d\n", view); */
    int qsnap_aperture_size = get_int(mainwnd->state, "qsnap-aperture-size");
    printf("%d\n", qsnap_aperture_size);
    /*
    qsnap-pen = Pen(Color(qsnapLocator-color));
    qsnap-pen.set_width(2);
    qsnap-pen.set_join-style(Qt-MiterJoin);
    qsnap-pen.set_cosmetic(1);
    painter.set_pen(qsnap-pen);
    qsnap-offset = Vector(qsnap-locator-size, qsnap-locator-size"]);

    apertureSnapPoints = [];
    apertureitem_list = items(
        view_mouse_point.x - qsnap_aperture_size,
        view_mouse_point.y - qsnap_aperture_size,
        2 * qsnap_aperture_size,
        2 * qsnap_aperture_size);
    for (item in apertureitem_list) {
        if (item.type != "Unknown") {
            tempitem-obj = item
            if (tempitem-obj) {
                aperture-snap-points += [ tempitem-obj.mouseSnapPoint(scene-mouse-point) ]
            }
        }
    }
    */

    /* TODO: Check for intersection snap points and add them to the list. */
    /*
    for (asp in aperture-snap-points) {
        p1 = map_from_scene(asp) - qsnap_offset;
        q1 = map_from_scene(asp) + qsnap_offset;
        painter.drawRect(Rect(map_to_scene(p1), map_to_scene(q1)));
    }
    */
}

/* Draw grip points for all selected objects. */
void
draw_foreground(int view, Painter *painter, SDL_Rect rect)
{
    printf("called with: %d %d", view, rect.x);
    Pen *grip_pen = create_pen();
    grip_pen->width = 2.0;
    grip_pen->join_style = MITER_JOIN;
    grip_pen->cosmetic = 1;
    /* grip_pen = Pen(rgb=grip-color-cool); */
    painter->pen = grip_pen;
    /*
    grip-offset = Vector(
        selection-grip-size, selection-grip-size
    );

    selected-grip-points = [];
    selecteditem_list = self.gscene.selected-items()
    if (selecteditem_list.size() <= 100) {
        for (item in selecteditem_list) {
            if (item.type != "Unknown") {
                tempBase-obj = item
                if (tempBase-obj) {
                    selected-grip-points = tempBase-obj.all-grip-points();
                }

                for (ssp in selected-grip-points) {
                    p1 = map_from_scene(ssp) - grip-offset;
                    q1 = map_from_scene(ssp) + grip-offset;
                    rect = make_rectangle(map_to_scene(p1), map_to_scene(q1));

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
    */

    if (!get_int(mainwnd->state, "selecting_active")) {
        draw_closest_qsnap(painter);
        draw_crosshair(painter);
    }

    if (get_int(mainwnd->state, "show-ruler")) {
        draw_rulers();
    }
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
    targetRect = make_rectangle(0,0,150,150)
    if (get_int(mainwnd->state, "printing-disable-bg")) { */
        /* TODO: Make BMC background into it's own setting? */ /*
        brush = gscene.backgroundBrush()
        gscene.setBackgroundBrush(Qt-NoBrush)
        scene_update();
        gscene.render(painter, targetRect, extents, "Qt-KeepAspectRatio")
        gscene.setBackgroundBrush(brush)
    }
    else {
        scene_update();
        gscene.render(painter, targetRect, extents, "Qt-KeepAspectRatio")
    }
    img.convertToFormat(QImage-Format-Indexed8, Qt-ThresholdDither|Qt-AvoidDither).save("test.bmc", "BMP")
    */
}

void
create_settings_widget(char *type)
{
    /* translate(setting.description),
        int-dialog[setting.index] = int-setting[setting.index] */
    if (type[0] == 'i') { /* Int */
        return;
    }
    if (type[1] == 'o') { /* dOuble */
        return;
    }
    if (type[1] == 'r') { /* dRopdown */
        return;
    }
    if (type[0] == 's') { /* String */
        return;
    }
    /* Error. */
}

void
create_settings_box(int box)
{
    int i;
    printf("box: %d\n", box);
    /*
    debug_message(box.title);
    for (i=0; box.settings[i].type >= 0; i++) {
        create_settings_widget(box.settings[i]);
    }
    */
}

void
create_settings_tab(int tab)
{
    int i;
    printf("tab: %d\n", tab);
    /*
    debug_message(tab.title);
    for (i=0; i<tab.n-boxes; i++) {
        create_settings_box(tab.box[i]);
    }
    */
}


/* Action functions.
 */
void
create_line_edit(int type, int user_editable)
{
    printf("create-line-edit(%d, %d)\n", type, user_editable);
}

void
create_tool_button(int type, char *label)
{
    printf("create-tool-button(%d, %s)\n", type, label);
}

/* .
 */
void
delete_selected(void)
{
    int i;

    for (i=0; i<n_selected; i++) {
        /*
        if (selected_items.data(OBJ-TYPE) != OBJ-TYPE-NULL) {
            base = item_list[i];
            if (base) {
                debug_message(".");
            }
        }
        */
    }
}

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

/* Selected objects are stored as a global variable
 * indexed by tab. Delta is a vector.
 */
void
move_selected(EmbVector delta)
{
    int i;

    for (i=0; i<n_selected; i++) {
        int item = selected_items[i];
        if (item) {
            debug_message(".");
        }
    }

    /* Always clear the selection after a move. */
    clear_selection();
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

void
settings_dialog(void) */ /*int showTab */ /*
{
    dialog = settings-dialog-action(showTab);
    dialog-mainloop();
}
*/

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
check_for_updates_(void)
{
    debug_message("check-for-updates()");
    /* TODO: Check website for versions, commands, etc... */
}

void
select_all_(void)
{
    debug_message("select_all()");
    int gview = get_int(mainwnd->state, "active-view");
    /*
    if (gview) {
        gview.select_all();
    }

    allPath = Path();
    allPath.add-rect(gscene.scene-rect());
    gscene.setSelectionArea(allPath, "ReplaceSelection", "intersects-item-shape", transform());
    */
}

void
design_details_(void)
{
    int scene = get_int(mainwnd->state, "active-scene");
    if (scene) {
        /*
        dialog = details-dialog-init(scene, self);
        dialog_exec();
        */        
    }
}

int n_tips = 15;

void
button_tip_of_the_day_clicked(int button)
{
    debug_message("button-tip-of-the-day-clicked()");
    if (button == 0) {
        if (get_int(mainwnd->state, "general-current-tip") > 0) {
            decrement(mainwnd->state, "general-current-tip");
        }
        else {
            set_int(mainwnd->state, "general-current-tip", n_tips-1);
        }
        /* setText(tips[general-current-tip]); */
        return;
    }
    if (button == 1) {
        increment(mainwnd->state, "general-current-tip");
        if (get_int(mainwnd->state, "general-current-tip") >= n_tips) {
            set_int(mainwnd->state, "general-current-tip", 0);
        }
        /* setText(tips[general-current-tip]); */
        return;
    }
    /* close dialog */
}

void
icon_resize(int icon_size)
{
    /*
    seticon-size(icon-size, icon-size);
    layer_selector.seticon-size(icon-size*4, icon-size);
    color-selector.seticon-size(icon-size, icon-size);
    linetype-selector.seticon-size(icon-size*4, icon-size);
    lineweightSelector.seticon-size(icon-size*4, icon-size);
    #set the minimum combobox width so the text is always readable
    layer_selector.set_minimum-width(icon-size*4);
    color-selector.set_minimum-width(icon-size*2);
    linetype-selector.set_minimum-width(icon-size*4);
    lineweightSelector.set_minimum-width(icon-size*4);

    #TODO: low-priority:
    #open app with icon-size set to 128. resize the icons to a smaller size.

    general_icon_size = icon_size;
    */
}

void
active_mdi_window(void)
{
    debug_message("activemdi-window()");
    /*
    mdi-win = mdi_area.active-sub-window();
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


/* This function intentionally does nothing.
void
do_nothing(void)
{
    debug_message("do-nothing()");
} */

void
close_toolbar(int action)
{
    /*
    if (action.object-name() == "toolbarclose") {
        tb = sender();
        if (tb) {
            debug_message("%s closed.", str(tb.object-name()));
            tb.hide();
        }
    }
    */
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
    if (debug_mode) {
        FILE *f;
        f = fopen("alert.txt", "a");
        fprintf(f, "%s\n%s\n", title, message);
        fclose(f);
    }
}

/* Should we need to add this to an error report.
 */
void
report_platform(void)
{
    /* print(os.uname()) */
}

void
scale_action(void)
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
active_view(void)
{
    debug_message("active_view()");
    /*
    mdi-win = mdi_area.active-sub-window();
    if (mdi-win) {
        v = mdi-win.getView();
        return v;
    }
    */
}

/* replace with a variable in Window? */
int
active_scene(void)
{
    debug_message("active-scene()");
    /*
    mdi-win = mdi_area.active-sub-window();
    if (mdi-win) {
        return mdi-win.getScene();
    }
    */
}

void
update_all_view_scroll_bars(int val)
{
    printf("%d\n", val);
    /*
    windowList = mdi_area.sub-window-list();
    for (mdi-win in windowList) {
        mdi-win.showViewScrollBars(val);
    }
    */
}

void
update_all_view_cross_hair_colors(EmbColor color)
{
    printf("%d\n", color.r);
    /*
    windowList = mdi_area.sub-window-list();
    for (mdi-win in windowList) {
        mdi-win.setViewCrossHairColor(color);
    }
    */
}

void
updateAllViewBackgroundColors(EmbColor color)
{
    printf("%d\n", color.r);
    /*
    windowList = mdi_area.sub-window-list()
    for (mdi-win in windowList) {
        mdi-win.setViewBackgroundColor(color)
    }
    */
}


void
checkbox(int setting, int checked)
{
    printf("%d %d\n", setting, checked);
    /*
    (dialog-setting-int[setting] = checked));
    */
}

void
spinbox(int setting, int value)
{
    printf("%d %d\n", setting, value);
    /* dialog-setting-double[setting] = value; */
}

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
    /*
    translatorQt = ""
    translatorQt.load("" + QLocale-system().name(), QLibraryInfo-location(QLibraryInfo-TranslationsPath)) */
    /* TODO: ensure self always loads, ship a copy of self with the app. */
    /* qApp.installTranslator(translatorQt) */
    /*
 Selectors
    layer_selector = ComboBox();
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

 create the mdi_area
    vbox = Frame(void);
    layout = VBoxLayout(vbox);
    layout.setContentsMargins(Margins());
    vbox.setFrameStyle(Frame-StyledPanel | Frame-Sunken);
    mdi_area = mdi_area(vbox);
    mdi_area.useBackgroundLogo(general-mdi-bg-use-logo);
    mdi_area.useBackgroundTexture(general-mdi-bg-use-texture);
    mdi_area.useBackgroundColor(general-mdi-bg-use-color);
    mdi_area.setBackgroundLogo(general-mdi-bg-logo);
    mdi_area.setBackgroundTexture(general-mdi-bg-texture);
    mdi_area.setBackgroundColor(Color(general-mdi-bg-color));
    mdi_area.setViewMode("TabbedView");
    mdi_area.setHorizontalScrollBarPolicy("ScrollBarAsNeeded");
    mdi_area.setVerticalScrollBarPolicy("ScrollBarAsNeeded");
    mdi_area.setActivationOrder("ActivationHistoryOrder");
    layout, add_widget(mdi_area);
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
        ACTION.set_object_name(action-list[i].abbreviation);
        ACTION.setWhatsself(action-list[i].description);

        connect(ACTION, SIGNAL(triggered()), self, SLOT(actions()));
        action-hash.insert(i, ACTION);
    }

    action-hash.value("window-close").setEnabled(n-docs > 0);
    action-hash.value("design-details").setEnabled(n-docs > 0);

    menu-FILE.add-menu(menu[RECENT-MENU]);
    */
    /* Do not allow the Recent Menu to be torn off.
     * It's a pain in the ass to maintain.
     */
    /*
    menu[RECENT-MENU].set_tear-off-enabled(0);

    debug_message("createWindowMenu()");
    menu-bar().add-menu(menu-WINDOW);
    connect(menu-WINDOW, SIGNAL(aboutToShow()), self, SLOT(window-menu-about-to-show()));
    */
    /* Do not allow the Window Menu to be torn off.
     * It's a pain in the ass to maintain.
     */
    /*
    menu-WINDOW.set_tear-off-enabled(0);

    for i in range(N-TOOLBARS) {
        message = "creating %s\n" % toolbar-label[i];
        debug_message(message);

        toolbar[i].set_object_name(toolbar-label[i]);

        for (j in toolbars[i]) {
            if toolbars[i][j] >= 0:
                toolbar[i].add-action(action-hash.value(toolbars[i][j]));
            else {
                toolbar[i].add-separator();
            }
        }
        */
        /*  connect(toolbar[i], SIGNAL(topLevelChanged(int)), self, SLOT(floating-changed-toolbar(int))); */
        /*
    }
    */

    debug_message("createLayerToolbar()");
    /*
    toolbar-LAYER.set_object_name("toolbarLayer");
    toolbar-LAYER.add-action(action-hash.value("make-layer-current"));
    toolbar-LAYER.add-action(action-hash.value("layers"));
    */

    /* TODO: Create layer pixmaps by concatenating several icons. */
    /*
    add_item(layer_selector, "linetypebylayer.png", "0");
    add_item(layer_selector, "linetypebylayer.png", "1");
    add_item(layer_selector, "linetypebylayer.png", "2");
    add_item(layer_selector, "linetypebylayer.png", "3");
    add_item(layer_selector, "linetypebylayer.png", "4");
    add_item(layer_selector, "linetypebylayer.png", "5");
    add_item(layer_selector, "linetypebylayer.png", "6");
    add_item(layer_selector, "linetypebylayer.png", "7");
    add_item(layer_selector, "linetypebylayer.png", "8");
    add_item(layer_selector, "linetypebylayer.png", "9");
    toolbar-LAYER, add_widget(layer_selector);
    connect(layer_selector, SIGNAL(currentIndexChanged(int)), self, SLOT(layer_selectorIndexChanged(int)));

    toolbar-LAYER.add-action(action-hash.value("layer-previous"));

    connect(toolbar-LAYER, SIGNAL(topLevelChanged(int)), self, SLOT(floating-changed-toolbar(int)));

    debug_message("createPropertiesToolbar()");

    toolbar-PROPERTIES.set_object_name("toolbar-properties");

    color-selector.set_focusProxy(menu-FILE);
 NOTE: Qt4.7 wont load icons without an extension...
    color-selector.add_item("colorbylayer.png", "ByLayer");
    color-selector.add_item("colorbyblock.png", "ByBlock");
    color-selector.add_item("colorred.png", translate("Red"), (255, 0, 0));
    color-selector.add_item("coloryellow.png", translate("Yellow"), (255,255, 0));
    color-selector.add_item("colorgreen.png", translate("Green"), (0, 255, 0));
    color-selector.add_item("colorcyan.png", translate("Cyan"), (0,255,255));
    color-selector.add_item("colorblue.png", translate("Blue"), (0, 0,255));
    color-selector.add_item("colormagenta.png", translate("Magenta"), (255, 0,255));
    color-selector.add_item("colorwhite.png", translate("White"), (255,255,255));
    color-selector.add_item("colorother.png", translate("Other..."))
    toolbar-PROPERTIES, add_widget(color-selector)
    connect(color-selector, SIGNAL(currentIndexChanged(int)), self, SLOT(color-selectorIndexChanged(int)))

    toolbar-PROPERTIES.add-separator()
    linetype-selector.set_focusProxy(menu-FILE)
    linetype-selector.add_item("linetypebylayer.png", "ByLayer")
    linetype-selector.add_item("linetypebyblock.png", "ByBlock")
    linetype-selector.add_item("linetypecontinuous.png", "Continuous")
    linetype-selector.add_item("linetypehidden.png", "Hidden")
    linetype-selector.add_item("linetypecenter.png", "Center")
    linetype-selector.add_item("linetypeother.png", "Other...")
    toolbar-PROPERTIES, add_widget(linetype-selector)
    connect(linetype-selector, SIGNAL(currentIndexChanged(int)), self, SLOT(linetype-selectorIndexChanged(int)))

    toolbar-PROPERTIES.add-separator()
    lineweightSelector.set_focusProxy(menu-FILE)
    #NOTE: Qt4.7 wont load icons without an extension...
    #TODO: Thread weight is weird. See http://en.wikipedia.org/wiki/Thread-(yarn)#Weight
    for line in thread-weights:
        lineweightSelector.add_item(line[0], line[1], line[2])
    lineweightSelector.setMinimumContentsLength(8)
    #Prevent dropdown text readability being squish...d.
    toolbar-PROPERTIES, add_widget(lineweightSelector)
    connect(lineweightSelector, SIGNAL(currentIndexChanged(int)), self, SLOT(lineweightSelectorIndexChanged(int)))

    connect(toolbar-PROPERTIES, SIGNAL(topLevelChanged()), self, SLOT(floating-changed-toolbar()))

    debug_message("createTextToolbar()")
    toolbar-TEXT.set_object_name("toolbarText")
    toolbar-TEXT, add_widget(textFontSelector)
    textFontSelector.setCurrentFont(Font(text-font))
    connect(textFontSelector, SIGNAL(currentFontChanged()), self, SLOT(textFontSelectorCurrentFontChanged()))

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
        text-size-selector.add_item(str(size)+" pt", size)
    setTextSize(text-size)
    toolbar-TEXT, add_widget(text-size-selector)
    connect(text-size-selector, SIGNAL(currentIndexChanged(int)), self, SLOT(text-size-selectorIndexChanged(int)))

    connect(toolbar-TEXT, SIGNAL(topLevelChanged(int)), self, SLOT(floating-changed-toolbar(int)))

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

    showNormal();

    if (get_int("general-tip-of-the-day")) {
        tip_of_the_day_action();
    }
    */
}

void
read_settings(void)
{
    debug_message("Reading settings...");

    /* This file needs to be read from the users home
     * directory to ensure it is writable.
     */ /*
    pos = Vector(window-x, window-y)
    size = (window-width, window-height)

    layoutState = settings-file.value("LayoutState").toByteArray()
    if ! restoreState(layoutState) {
        debug_message("LayoutState NOT restored! Setting Default Layout...")
        #someToolBar.setVisible(1)

    settings = load-data("config.json"); */

    /* Sanitise data here */
    /*
    window-x = clamp(0, window-x, 1000);
    window-y = clamp(0, window-y, 1000);

    move(pos);
    resize(size); */
}

void
rotate_action(void)
{
    debug_message("TODO");
}

void
rotate_selected(EmbVector pos, double rot)
{
    /*
    item_list = gscene.selected-items();
    num-selected = item_list.size()
    for (item in item_list) {
        if (item) {
            debug_message(".");
        }
    } */

    /* Always clear the selection after a rotate. */
    clear_selection();
}


/* Selected objects are stored as a global variable
 * indexed by tab. Point1 and point2 are vectors.
 */
void
mirror_selected(EmbVector point1, EmbVector point2)
{
    /*
    item_list = gscene.selected-items();
    num-selected = item_list.size();
    for (item in item_list) {
        if (item) {
            debug_message(".");
        }
    } */

    /* Always clear the selection after a mirror. */
    clear_selection();
}

void
scale_selected(EmbVector v, double factor)
{
    /* item_list = gscene.selected-items()
    num-selected = item_list.size()
    for (item in item_list) {
        if (item) {
            debug_message(".")
        }
    } */
    /*  Always clear the selection after a scale. */
    /* gscene.clear_selection() */
}

void
show_scroll_bars(int val)
{
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
set_cross_hair_color(MdiArea *mdi_area, EmbColor color)
{
    /* crosshair-color = color
    gscene.set_property("VIEW-COLOR-CROSSHAIR", color)
    scene_update();
    */
}

void
set_background_color(MdiArea *mdi_area, EmbColor color)
{
    /* set_background-brush(Color(color))
    gscene.set_property("VIEW-COLOR-BACKGROUND", color)
    scene_update();
    */
}

void
set_select_box_colors(
    MdiArea *mdi_area,
    EmbColor colorL, EmbColor fillL,
    EmbColor colorR, EmbColor fillR,
    double alpha)
{
    printf("%d\n", mdi_area->tabs_closeable);
    printf("%d\n", colorL.r);
    printf("%d\n", fillL.r);
    printf("%d\n", colorR.r);
    printf("%d\n", fillR.r);
    printf("alpha %f\n", alpha);
    /* select-box.set_colors(color-left, fill-left, color-right, fill-right, alpha) */
}

void
update_all_view_select_box_colors(
    EmbColor colorL, EmbColor fillL,
    EmbColor colorR, EmbColor fillR,
    double alpha)
{
    printf("%d\n", colorL.r);
    printf("%d\n", fillL.r);
    printf("%d\n", colorR.r);
    printf("%d\n", fillR.r);
    printf("alpha %f\n", alpha);
    /* windowList = mdi_area.sub-window-list()
    for (mdi-win in windowList) {
        mdi-win.setViewSelectBoxColors(colorL, fillL, colorR, fillR, alpha)
    } */
}

void
update_all_view_grid_colors(EmbColor color)
{
    /*
    windowList = mdi_area.sub-window-list();
    for (mdi-win in windowList) {
        mdi-win.setViewGridColor(color);
    }
    */
}

void
update_all_view_ruler_colors(EmbColor color)
{
    /*
    windowList = mdi_area.sub-window-list();
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
    updatepickadd_mode(val)
    */
}

/* Color selector index changed.
 */
void
color_selector_index_changed(int index)
{
    printf("color-selectorIndexChanged(%d)", index);
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

    mdi-win = mdi_area.active-sub-window()
    if (mdi-win) {
        mdi-win.currentColorChanged(newColor)
    }
    */
}

/*
void
linetype-selector-index_changed(int index)
{
    printf("linetype-selectorIndexChanged(%d)", index)
    actuator(ACTION-DO-NOTHING)
}

void
lineweight-selector-index_changed(int index)
{
    printf("lineweightSelectorIndexChanged(%d)", index)
    actuator(ACTION-DO-NOTHING)
} */

/* Text font selector current font changed.
 */
void
text_fontSelectorCurrentFontChanged(char *font)
{
   /*  debug_message("textFontSelectorCurrentFontChanged()")
    textFontSelector.setCurrentFont(Font(font.family()))
    text-font = font.family().toLocal8Bit().constData() */
}

/* TODO: check that the toReal() conversion is ok.
 */
void
text_size_selectorIndexChanged(int index)
{
    /* debug_message("text-size-selectorIndexChanged(%d)", index)
    text-style.size = abs(text-size-selector.itemData(index).toReal()) */
}

void
text_font(void)
{
    /*return text-font */
}

void
setTextSize(int num)
{
    int index;
    /*
    text-style.size = abs(num)
    index = text-size-selector.find-text("Custom", "MatchContains")
    if (index != -1) {
        text-size-selector.remove-item(index)
    }
    text-size-selector.add_item("Custom " + "".set_num(num, 'f', 2) + " pt", num)
    index = text-size-selector.find-text("Custom", "MatchContains")
    if (index != -1) {
        text-size-selector.set_current_index(index)
    }*/
}

int
getCurrentLayer(void)
{
    debug_message("getCurrentLayer");
    /*
    if (window == NULL) {
        debug_message("called without window initialised.")
        return -2
    }
    */
    /*
    debug_message((char*)title)
    mdi-win = mdi_area.active-sub-window()
    if (mdi-win) {
        return mdi-win.getCurrentLayer()
    }
    */
    return 0;
}

/* TODO: return color ByLayer.
 */
int
get_current_color(void)
{
    debug_message("get-current-color");
    /*
    if (window == NULL) {
        debug_message("called without window initialised.")
        return -2
    }
    */
    /*
    debug_message((char*)title)
    mdi-win = mdi_area.active-sub-window()
    if mdi-win:
        return mdi-win.get-current-color()
    */
    return 0;
}


int
get_current_line_type(void)
{
    debug_message("get-current-line_type");
    /*
    if (window == NULL) {
        debug_message("called without window initialised.");
        return -2;
    }
    */
    /*
    debug_message((char*)title);
    mdi-win = mdi_area.active-sub-window();
    if (mdi-win) {
        return mdi-win.get-current-line_type();
    }
    */
    /* return OBJ_LWT_BYLAYER;
     */
    return 0;
}

int
get_current_line_weight(void)
{
    debug_message("get-current-line_weight");
    /*
    if (window == NULL) {
        debug_message("called without window initialised.");
        return -2;
    }

    debug_message(title);
    mdi-win = mdi_area.active-sub-window();
    if (mdi-win) {
        return mdi-win.get-current-line_weight();
    }

    return OBJ-LWT-BYLAYER;
    */
    return 0;
}

float
calculate_angle(EmbVector point1, EmbVector point2)
{
    /* return Line(x1, -y1, x2, -y2).angle(); */
    return 0.0;
}

float
calculate_distance(EmbVector point1, EmbVector point2)
{
    /* return Line(x1, y1, x2, y2).length(); */
    return 0.0;
}

void
fill_menu(int menu_id)
{
    /*
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
    */
}

/*
 * This is currently causing a bug and is going to be replaced
 * with a libembroidery function.
 */
double
native_perpendicular_distance(void)
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
    return 0.0;
}

void
recent_menu_about_to_show(void)
{
    debug_message("recentMenuAboutToShow()");
    /*
    menu[RECENT-MENU].clear();

    recent-file-info = "";
    recent-value = "";
    for (i=0; i<len(opensave-recent-list-of-files); i++) {
        /* If less than the max amount of entries add to menu */
        /*
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
                connect(rAction, SIGNAL(triggered()), self, SLOT(openrecentfile()));
            }
        }
    } */

    /* Ensure the list only has max amount of entries */
    /*
    while (opensave-recent-list-of-files.size() > opensave-recent-max-files) {
        opensave-recent-list-of-files.removeLast();
    }
    */
}

void
window_menu_about_to_show(void)
{
    debug_message("window-menu-about-to-show()");
    /*
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
    windows = mdi_area.sub-window-list();
    for (i=0; i<len(windows); i++) {
        an-action = Action(windows[i].window-title(), self);
        an-action.set_checkable(1);
        an-action.set_data(i);
        menu-WINDOW.add-action(an-action);
        connect(an-action, SIGNAL(toggled(int)), self, SLOT(windowMenuActivated(int)));
        an-action.set_checked(mdi_area.active-sub-window() == windows[i]);
    }
    */
}

void
window_menu_activated(int *checked)
{
    /*
    int a-sender;
    debug_message("windowMenuActivated()");
    a-sender = sender();
    if (!a-sender) {
        return;
    }
    w = mdi_area.sub-window-list().at[a-sender.data().toInt()];
    if (w and checked) {
        w.set_focus();
    }
    */
}

void
close_event(SDL_Event event)
{
    debug_message("MdiWindow closeEvent()");
    /*
    mdi_area.close-all-sub-windows();
    write-settings();
    event.accept();

    sendCloseMdiWin();
    */
}

void
on_close_window(void)
{
    debug_message("onCloseWindow()");
    /*
    mdi-win = mdi_area.active-sub-window();
    if (mdi-win) {
        onClosemdi-win(mdi-win);
    }
    */
}

void
on_close_mdi_win(void)
{
    int keep_maximized;
    debug_message("onClosemdi-win()");
    decrement(mainwnd->state, "n-docs");
    keep_maximized = 0;
    /*
    if (the_mdi_win) {
        keep_maximized = the-mdi-win.is-maximized();
    }

    mdi_area.remove-sub-window(the-mdi-win);
    the-mdi-win.delete-later();

    update-menu-toolbar-statusbar();
    window-menu-about-to-show();

    if (keep_maximized) {
        mdi-win = mdi_area.active-sub-window();
        if (mdi-win) {
            mdi-win.show-maximized();
        }
    }
    */
}

void
resize_event(SDL_Event e)
{
    debug_message("resizeEvent()");
    /* resizeEvent(e);
    statusBar().setSizeGripEnabled(!isMaximized()); */
}

void
update_menu_toolbar_statusbar(void)
{
    debug_message("updateMenuToolbarStatusbar()");
    /*
    action-enabled[ACTION-PRINT] = n-docs;
    action-enabled[ACTION-WINDOW-CLOSE] = n-docs;
    action-enabled[ACTION-DESIGN-DETAILS] = n-docs;
    */

    if (get_int(mainwnd->state, "n-docs")) {
        /*
        #Toolbars
        for key in toolbar.keys() {
            toolbar[key].show()

        #DockWidgets
        dock_prop_edit.show()
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
        status_bar-mouse-coord.show()
        status_bar-SNAP.show()
        status_bar-GRID.show()
        status_bar-RULER.show()
        status_bar-ORTHO.show()
        status_bar-POLAR.show()
        status_bar-QSNAP.show()
        status_bar-QTRACK.show()
        status_bar-LWT.show()
        */
    }
    else {
        int i;
        /*
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
        } */

        /* DockWidgets */
        /* dockPropEdit.hide();
        dockUndoEdit.hide(); */

        /* Menus */
        /*
        menu-bar().clear();
        menu-bar().add-menu(menu-FILE);
        menu-bar().add-menu(menu-EDIT);
        menu-bar().add-menu(menu-MENU);
        menu-bar().add-menu(menu-WINDOW);
        menu-bar().add-menu(menu-HELP);

        menu-WINDOW.setEnabled(0);
        */

        /* Statusbar */
        /*
        statusbar.clear-message()
        status_bar-mouse-coord.hide()
        for (k=0 k<status_bar-n-keys k++) {
            status_bar[k].hide();
        }
        */
    }
}

void
load_formats(void)
{
    int i, curFormat;
    char stable, unstable;
    char supported_readers[MAX_STRING_LENGTH];
    char individual_readers[MAX_STRING_LENGTH];
    char supported_writers[MAX_STRING_LENGTH];
    char individual_writers[MAX_STRING_LENGTH];

    strcpy(supported_readers, "All Supported Files (");
    strcpy(individual_readers, "All Files (*)");
    strcpy(supported_writers, "All Supported Files (");
    strcpy(individual_writers, "All Files (*)");

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
    curFormat = 0;
    for (i=0; i<numberOfFormats; i++) {
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

    strcat(supported_readers, ")");
    strcat(supported_writers, ")");

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
floating_changed_toolbar(int isFloating)
{
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
            ACTION.set_object_name("toolbarclose")
            tb.add-action(ACTION)
            connect(tb, SIGNAL(actionTriggered()), self, SLOT(close-toolbar()))
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
build_menu_bar(void)
{
    debug_message("build-menu-bar");
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
build_button_grid(void)
{
    debug_message("build-buttongrid");
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
application_event(SDL_Event event)
{
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
repaint(void)
{

}


/* NOTE: This function should be used to interpret various
 * object types and save them as polylines for stitchOnly formats.
 */
void
to_polyline(EmbPattern *pattern, EmbVector obj_pos,
    /* obj-path, layer, */
    EmbColor color, int line_type, double line_weight)
{
    /*
    startX = obj-pos.x;
    startY = obj-pos.y;
    point-list = [];
    for i in range(obj-path.element-count()) {
        element = obj-path.element-at(i);
        a = Vector(0.0, 0.0);
        a.point.x = element.x + startX;
        a.point.y = -(element.y + startY);
        point-list += [a];
    }

    poly_object = Polyline();
    poly_object.point-list = point-list;
    poly_object.color = color;
    poly_object.line_type = "solid";
    pattern.add-polyline(poly_object);
    */
}

/*
class MdiWindow()*/
    /*
    MdiWindow(index, mw, parent, wflags)
    ~MdiWindow()

    virtual QSize  sizeHint() const

    static void getCurrentFile()
    static void getShortCurrentFile()
    static void getView()
    static void getScene()
    static void getCurrentLayer()
    static void getCurrentColor()
    static void getCurrentline_type()
    static void getCurrentline_weight()
    static void setCurrentLayer(layer)
    static void set_current-color(color)
    static void setCurrentline_type(line_type)
    static void setCurrentline_weight(line_weight)
    static void design-details-action()
    static void sendCloseMdiWin(MdiWindow*)

    mwdow*    mw
    QGraphicsScene*    gscene
    QMdiArea*  mdiArea
    View*  gview
    int fileWasLoaded

    # QPrinter printer

    QString curFile
void setCurrentFile(file_name)
    QString fileExtension(file_name)

    int myIndex

    QString curLayer
    unsigned int curColor
    QString curline_type
    QString curline_weight

void closeEvent(e)
void mdi_on_window_activated()
void mdi_current_layer_changed(layer)
void mdi_current_color_changed(color)
void mdi_current_line_type_changed(type)
void mdi-currentline_weightChanged(weight)
void mdi-updateColorline_typeline_weight()
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

void
set_current_file(char *file_name)
{
    /*
    curFile = QFileInfo(file_name).canonicalFilePath();
    setWindowModified(0);
    setWindowTitle(getShortCurrentFile());
    */
}

void
get_short_current_file(void)
{
    /* return QFileInfo(curFile).file_name() */
}

void
file_extension(char *file_name)
{
    /*
    return QFileInfo(file_name).suffix().toLower();
    */
}

void
on_window_activated(void/* mdi-window *subwindow */)
{
    debug_message("MdiWindow onWindowActivated()");
    /*
    mdi-win = w.mdi-window()
    if (mdi-win) {
        mdi-win.on-window-activated()
    }
    status_bar-SNAP.set_checked(gscene.property("ENABLE-SNAP"))
    status_bar-GRID.set_checked(gscene.property("ENABLE-GRID"))
    status_bar-RULER.set_checked(gscene.property("ENABLE-RULER"))
    status_bar[STATUS-ORTHO].set_checked(gscene.property("ENABLE-ORTHO"))
    status_bar[STATUS-POLAR].set_checked(gscene.property("ENABLE-POLAR"))
    status_bar[STATUS-QSNAP].set_checked(gscene.property("ENABLE-QSNAP"))
    status_bar[STATUS-QTRACK].set_checked(gscene.property("ENABLE-QTRACK"))
    status_bar[STATUS-LWT].set_checked(gscene.property("ENABLE-LWT"))
    #mw.prompt.setHistory(promptHistory)
    */
}

void
sizeHint()
{
    debug_message("MdiWindow sizeHint()");
    /*
    return QSize(450, 300);
    */
}

void
current_layer_changed(void /*layer */)
{
    /* curLayer = layer; */
}

void
current_color_changed(EmbColor color)
{
    /* curColor = color; */
}

void
current_line_type_changed(int type)
{
    /* curline_type = type; */
}

void
current_line_weight_changed(double weight)
{
    /* curline_weight = weight; */
}

void
update_color_line_type_line_weight(void)
{
    debug_message("update color line type weight");
}

void
show_view_scroll_bars(double val)
{
    /* gview.showScrollBars(val) */
}

void
set_view_cross_hair_color(EmbColor color)
{
    /* gview.setCrossHairColor(color) */
}

void
set_view_background_color(EmbColor color)
{
    /* gview.setBackgroundColor(color) */
}

void
set_viewGridcolor(EmbColor color)
{
    /* gview.setGridColor(color) */
}

void
set_view_ruler_color(EmbColor color)
{
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
    PropertyEditor(iconDirectory = "", int pickadd_mode = 1, QWidget* widgetToFocus = 0, QWidget* parent = 0, Qt-WindowFlags flags = Qt-Widget)
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
pickadd_modeToggled()

void
setSelectedItems(QList<QGraphicsItem*> itemList)
{

}

void
update_pick_add_mode_button(int pickadd_mode)
{

}

void
fieldEdited(QObject* fieldObj)
{

}

void
show_groups(int obj_type)
{

}

void
show_one_type(int index)
{

}

void
hideAllGroups(void)
{

}

void
clear_all_fields(void)
{

}

void
togglepickadd_mode(

*/
/*
void
property-editor-init(self, iconDirectory, pickadd_mode, widgetToFocus, parent, flags)
{ */
    /* . */ /*
    iconDir = iconDirectory
    iconSize = 16
    propertyEditorButtonStyle = Qt-button-TextBesideIcon */
    /* TODO: Make customizable. */ /*
    setMinimumSize(100,100)

    pickAdd = pickadd_mode

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
    hboxLayoutSelection, add_widget(create-comboboxSelected())
    hboxLayoutSelection, add_widget(createbutton-QSelect())
    hboxLayoutSelection, add_widget(createbutton_pickadd())
    widgetSelection.setLayout(hboxLayoutSelection)

    for (i=1; i<UNKNOWN_BASE; i++) {
        groupbox-geometry[i] = creategroupbox-geometry(i+BASE);
    }

    scrollProperties = QScrollArea(this);
    widgetProperties = QWidget(this);
    vboxLayoutProperties = QVBoxLayout(this);
    vboxLayoutProperties, add_widget(creategroupboxGeneral());
    for (i=1; i<UNKNOWN_BASE; i++) {
        add_widget(vboxLayoutProperties, groupbox-geometry[i+BASE]);
    }

    vboxLayoutProperties, add_widget(creategroupboxMiscArc());
    vboxLayoutProperties, add_widget(creategroupboxMiscImage());
    vboxLayoutProperties, add_widget(creategroupboxMiscPath());
    vboxLayoutProperties, add_widget(creategroupboxMiscPolyline());
    vboxLayoutProperties, add_widget(creategroupboxTextTextSingle());
    vboxLayoutProperties, add_widget(creategroupboxMiscTextSingle());
    vboxLayoutProperties.addStretch(1);
    widgetProperties.setLayout(vboxLayoutProperties);
    scrollProperties.setWidget(widgetProperties);
    scrollProperties.setWidgetResizable(1);

    vboxLayoutMain = QVBoxLayout(this);
    vboxLayoutMain, add_widget(widgetSelection);
    vboxLayoutMain, add_widget(scrollProperties);
    widgetMain.setLayout(vboxLayoutMain);

    setWidget(widgetMain);
    setWindowTitle(translate("Properties"));
    setAllowedAreas(Qt-LeftDockWidgetArea | Qt-RightDockWidgetArea);

    hideAllGroups();

    connect(signalMapper, SIGNAL(mapped()), this, SLOT(fieldEdited()));

    focusWidget = widgetToFocus;
    this.installEventFilter(this);
}
*/

void
create_groupbox(int data)
{
    /*
    for (i=0; i<; i++) {
        button = create_tool_button("blank", translate(label[i]));
        edit = create_line_edit("double", 0);
        add_to_form(button, edit);
    }
    */
}

void
event_filter(int obj, SDL_Event event)
{
    /*
    if (event.type() == "KeyPress") {
        key = event.key()
        if (Qt-Key-Escape) {
            if focusWidget:
                focusWidget.setFocus(Qt-OtherFocusReason);
            return 1;
        }
        else {
            event.ignore();
        }
    }
    return QObject-eventFilter(obj, event);
    */
}

void
create_combobox_selected(void)
{
    /*
    comboBoxSelected = tk.ComboBox(this);
    comboBoxSelected.addItem(translate("No Selection"));
    return comboBoxSelected;
    */
}

void
create_button_QSelect(void)
{
    /*
    button-QSelect = tk.Button-(this);
    button-QSelect.setIcon(load-icon(quickselect-xpm));
    button-QSelect.setIconSize(QSize(iconSize, iconSize));
    button-QSelect.setText("QSelect");
    button-QSelect.setToolTip("QSelect"); #TODO: Better Description
    button-QSelect.setbutton-Style(Qt-button-IconOnly);
    return button-QSelect;
    */
}

void
create_button_pick_add(void)
{
/*
    #TODO: Set as PickAdd or PickNew based on settings
    button_pickadd = Qbutton-(this);
    update_pick_add_mode_button(pickAdd);
    connect(button_pickadd, SIGNAL(clicked(int)), this, SLOT(togglepickadd_mode()));
    return button_pickadd;
    */
}

void
update_pick_add_mode_button(int pickadd_mode)
{
    /*
    pickAdd = pickadd_mode;
    if (pickAdd) {
        button_pickadd.setIcon(load-icon(pickadd-xpm));
        button_pickadd.setIconSize(QSize(iconSize, iconSize));
        button_pickadd.setText("PickAdd");
        button_pickadd.setToolTip("PickAdd Mode - Add to current selection.\nClick to switch to PickNew Mode.");
        button_pickadd.setbutton-Style(Qt-button-IconOnly);
    }
    else {
        button_pickadd.setIcon(load-icon(picknew-xpm));
        button_pickadd.setIconSize(QSize(iconSize, iconSize));
        button_pickadd.setText("PickNew");
        button_pickadd.setToolTip("PickNew Mode - Replace current selection.\nClick to switch to PickAdd Mode.");
        button_pickadd.setbutton-Style(Qt-button-IconOnly);
    }
    */
}

void
toggle_pick_add_mode(void)
{
    /* emit pickadd_modeToggled() */
    debug_message("not sure how to deal with emit yet");
}

void
update_edit_StrIfVaries(int edit, char *str)
{
    /*
    field-old-text = edit-.text();
    field-new-text = str;

    if (field-old-text.isEmpty() {
        edit-.setText(field-new-text);
    }
    elif (field-old-text != field-new-text) {
        edit-.setText(field-varies-text)
    }
    */
}

void
update_edit_num_if_varies(char *property, int num, int useAnglePrecision)
{
    /*
    int precision = 0;
    if (useAnglePrecision) {
        precision = precisionAngle;
    }
    else {
        precision = precisionLength;
    }

    field-old-text = edit-.text();
    field-new-text.setNum(num, 'f', precision);
    */

    /* Prevent negative zero :D */
    /*
    negative-zero = "-0."
    for i in range(precision)) {
        negative-zero += '0';
    }
    if field-new-text == negative-zero:
        field-new-text = negative-zero.replace("-", "")

    if field-old-text.isEmpty()) {
        edit-.setText(field-new-text);
    }
    elif field-old-text != field-new-text) {
        edit-.setText(field-varies-text);
    }
    */
}

/*
void
updatefont-comboboxStrIfVaries(self, font-combobox, str)
{
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
        font-combobox.set_current_index(font-combobox.findText(field-varies-text))
}

void
updateComboBoxStrIfVaries(self, comboBox, str, strList)
    field-old-text = comboBox.currentText()
    field-new-text = str

    if field-old-text.isEmpty()
        for s in strList:
            comboBox.addItem(s, s)
        comboBox.set_current_index(comboBox.findText(field-new-text))

    elif field-old-text != field-new-text:
        if comboBox.findText(field-varies-text) == -1:
            # Prevent multiple entries
            comboBox.addItem(field-varies-text)
        comboBox.set_current_index(comboBox.findText(field-varies-text))
}
*/

void
update_combobox_int_if_varies(int self, int comboBox, int val, int yesOrNoText)
{
    /*
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

        comboBox.set_current_index(comboBox.findText(field-new-text))

    elif field-old-text != field-new-text:
        # Prevent multiple entries
        if comboBox.findText(field-varies-text) == -1:
            comboBox.addItem(field-varies-text)
        comboBox.set_current_index(comboBox.findText(field-varies-text))
    */
}

void
show_groups(int obj_type)
{
    /*
    if (obj-type in obj-types) {
        groupbox-geometry[obj-type-BASE].show();
    }
    if (obj-type == "Arc") {
        groupboxMiscArc.show();
    }
    if (obj-type == "Image":
        groupboxMiscImage.show();
    }
    if (obj-type == "PATH":
        groupboxMiscPath.show();
    }
    if (obj-type == "POLYLINE") {
        groupboxMiscPolyline.show();
    }
    if (obj-type == "Text Single") {
        groupboxTextTextSingle.show();
        groupboxMiscTextSingle.show();
    }
    */
}

void
show_one_type(int self, int index)
{
    /*
    (hide-all-groups)
    showGroups(comboBoxSelected.itemData(index).toInt())
    */
}

/* NOTE: General group will never be hidden.
 */
/*
void
hide_all_groups(void)
{
    for i in obj-types:
        groupbox-geometry[i].hide();
    groupboxMiscArc.hide();
    groupboxMiscImage.hide();
    groupboxMiscPath.hide();
    groupboxMiscPolyline.hide();
    groupboxTextTextSingle.hide();
    groupboxMiscTextSingle.hide();
}

void
clear_all_fields(void)
{
    for i in range(COMBOBOX-PROPERTY-EDITORS) {
        comboBox[i].clear()
    }
    for i in range(edit--PROPERTY-EDITORS) {
        i].clear()
    }
      Text Single
    comboBoxTextSingleFont.removeItem(comboBoxTextSingleFont.findText(field-varies-text))
    # NOTE: Do not clear comboBoxTextSingleFont
    comboBoxTextSingleFont.setProperty("FontFamily", "")
}


void
create-groupbox-geometry self obj-type)
{
    gb = Qgroupbox(translate("Geometry"), this)

    # TODO: use proper icons
    form-layout = tk.form-layout(this)
    for i in obj-types:
        if property-editors[i].object == obj-type:
            index = property-editors[i].id
            button-[index] = createbutton-(property-editors[i].icon, translate(property-editors[i].label))
            index] = createedit-(property-editors[i].type, property-editors[i].read-only)
            form-layout.add-row(button-[index], index])
            mapSignal(index], property-editors[i].signal, obj-type)

    gb.setLayout(form-layout)

    return gb
}

void
createbutton-(self, char *iconName, char *txt)
{
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
{
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
{
    cb = QComboBox(this)
    cb.setDisabled(disable)
    return cb
}

void
createfont-combobox(self, disable)
{
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
                    p.setX(ARC-CENTER-X].text().toDouble())
                    tempArcObj.setPos(p)

            if objName == "edit-ArcCenterY":
                tempArcObj = item
                if tempArcObj:
                    p = tempArcObj.objectCenter()
                    p.setY(ARC-CENTER-Y].text().toDouble())
                    tempArcObj.setPos(p)

            if objName == "edit-ArcRadius":
                tempArcObj = item
                if tempArcObj:
                    tempArcObj.setObjectRadius(ARC-RADIUS].text().toDouble())

            if objName == "edit-ArcStartAngle":
                tempArcObj = item
                if tempArcObj:
                    tempArcObj.setObjectStartAngle(ARC-START-ANGLE].text().toDouble())

            if objName == "edit-ArcEndAngle":
                tempArcObj = item
                if tempArcObj:
                    tempArcObj.setObjectEndAngle(ARC-END-ANGLE].text().toDouble())

        if (item.type == "Block":
            # TODO: field editing
            break
        if (item.type == "Circle":
            if objName == "edit-CircleCenterX":
                p = item.objectCenter()
                p.setX(CIRCLE-CENTER-X].text().toDouble())
                item.setPos(p)

            if objName == "edit-CircleCenterY":
                tempCircleObj = item
                if tempCircleObj:
                    p = tempCircleObj.objectCenter()
                    p.setY(CIRCLE-CENTER-Y].text().toDouble())
                    tempCircleObj.setPos(p)

            if objName == "edit-CircleRadius":
                tempCircleObj = item
                if tempCircleObj:
                    tempCircleObj.setObjectRadius(CIRCLE-RADIUS].text().toDouble())

            if objName == "edit-CircleDiameter":
                tempCircleObj = item
                if tempCircleObj:
                    tempCircleObj.setObjectDiameter(CIRCLE-DIAMETER].text().toDouble())

            if objName == "edit-circle-area":
                tempCircleObj = item
                if tempCircleObj:
                    tempCircleObj.setObjectArea(CIRCLE-AREA].text().toDouble())
            if objName == "edit-CircleCircumference":
                tempCircleObj = item
                if tempCircleObj:
                    tempCircleObj.setObjectCircumference(CIRCLE-CIRCUMFERENCE].text().toDouble())

            break

        if (item.type == "DIMALIGNED":
            # TODO: field editing
            break

        if (item.type == "DIMANGULAR":
            # TODO: field editing
            break

        if (item.type == "DIMARCLENGTH":
            # TODO: field editing
            break

        if (item.type == "DIMDIAMETER":
            # TODO: field editing
            break

        if (item.type == "DIMLEADER":
            # TODO: field editing
            break

        if (item.type == "DIMLINEAR": #TODO: field editing
            break

        if (item.type == "DIMORDINATE": #TODO: field editing
            break

        if (item.type == "DIMRADIUS": #TODO: field editing
            break

        if (item.type == "ELLIPSE":
            if objName == "edit-ellipse-center-x":
                p = item.center()
                p.x = ELLIPSE-CENTER-X].text().toDouble()
                item.setPos(p)

            if objName == "edit-ellipseCenterY":
                p = item.center()
                p.y = "ELLIPSE-CENTER-Y].text().toDouble()
                item.setPos(p)

            if objName == "edit-EllipseRadiusMajor":
                item.setObjectRadiusMajor("ELLIPSE-RADIUS-MAJOR].text().toDouble())

            if objName == "edit-EllipseRadiusMinor":
                item.set_radius-minor(edit--ELLIPSE-RADIUS-MINOR.text().toDouble())

            if objName == "edit-EllipseDiameterMajor":
                item.setObjectDiameterMajor(edit--ELLIPSE-DIAMETER-MAJOR.text().toDouble())

            if objName == "edit-EllipseDiameterMinor":
                item.setObjectDiameterMinor("ELLIPSE-DIAMETER-MINOR].text().toDouble())

            break
        elif IMAGE: #TODO: field editing
            break
        elif INFINITELINE: #TODO: field editing
            break
        elif LINE:
            if objName == "edit-LineStartX":
                item.setObjectX1("LINE-START-X].text().toDouble())

            elif objName == "edit-LineStartY":
                item.setObjectY1(-"LINE-START-Y].text().toDouble())

            elif objName == "edit-LineEndX":
                item.setObjectX2("LINE-END-X].text().toDouble())

            elif objName == "edit-LineEndY":
                item.setObjectY2(-"LINE-END-Y].text().toDouble())

        if (item.type == "PATH":
            #TODO: field editing

        if (item.type == "POINT":
            if objName == "edit-PointX":
                item.setObjectX("POINT-X].text().toDouble())

            elif objName == "edit-PointY":
                item.setObjectY(-"POINT-Y].text().toDouble())

        if (item.type == "POLYGON":
            debug_message("TYPE Polygon has no field editing")

        if (item.type == "Polyline":
            debug_message("TYPE Polyline has no field editing")

        if (item.type == "RAY":
            # TODO: field editing
            debug_message("TYPE Polyline has no field editing")

        if (item.type == "RECTANGLE":
            # TODO: field editing
            debug_message("TYPE Polyline has no field editing")

        if (item.type == "TEXTMULTI":
            # TODO: field editing
            debug_message("TYPE Polyline has no field editing")

        if (item.type == "TEXTSINGLE":
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
load_icon(char *fname)
{
    debug_message("load-icon with fname:");
    debug_message(fname);
}

/* . */
void
addColorsTocombobox(int combobox)
{
/*
    combobox.add_item(load-icon(colorred-xpm), translate("Red"), tk.Rgb(255, 0, 0));
    combobox.add_item(load-icon(coloryellow-xpm), translate("Yellow"), tk.Rgb(255,255, 0));
    combobox.add_item(load-icon(colorgreen-xpm), translate("Green"), tk.Rgb(  0,255, 0));
    combobox.add_item(load-icon(colorcyan-xpm), translate("Cyan"), tk.Rgb(  0,255,255));
    combobox.add_item(load-icon(colorblue-xpm), translate("Blue"), tk.Rgb(  0, 0,255));
    combobox.add_item(load-icon(colormagenta-xpm), translate("Magenta"), tk.Rgb(255, 0,255));
    combobox.add_item(load-icon(colorwhite-xpm), translate("White"), tk.Rgb(255,255,255));
    # TODO: Add Other... so the user can select custom colors */
}

/* . */
void
combobox_language_current_index_changed(char *lang)
{
    /*
    dialog-general-language = lang.toLower();
    */
}

/* . */
void
combobox_icon_theme_current_index_changed(char *theme)
{
    /*
    strcpy(dialog-general-icon-theme, theme);
    */
}

/* . */
void
combobox_icon_size_current_index_changed(int index)
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

/* . */
void
checkbox_general_mdi_bg_use_logo_state_changed(int checked)
{
    /*
    preview.general-mdi-bg-use-logo = checked;
    mdi_area.useBackgroundLogo(int checked);
    */
}

/* . */
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
        mdi_area.setBackgroundLogo(accept-.general-mdi-bg-logo) */
}

/* . */
void
checkbox_general_mdi_bg_use_texture_state_changed(int checked)
{
    /*
    preview.general-mdi-bg-use-texture = checked;
    mdi_area.useBackgroundTexture(int checked);
    */
}

/* . */
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
        mdi_area.setBackgroundTexture(accept-.general-mdi-bg-texture)
    }
    */
}

/* . */
void
checkbox_general_mdi_bg_use_color_state_changed(int checked)
{
    /*
    preview.general-mdi-bg-use-color = checked;
    mdi_area.useBackgroundColor(int checked);
    */
}

/* . */
void
choose_general_mdi_background_color(void)
{
    /*
    button = sender();
    if (button) {
        color-dialog = color-dialog(Color(accept-.general-mdi-bg-color), this);
        connect(color-dialog, SIGNAL(currentColorChanged()), this, SLOT(currentGeneralMdiBackgroundColorChanged()));
        color-dialog-exec();

        if (color-dialog-result() == "Accepted") {
            accept.general-mdi-bg-color = color-dialog-selectedColor().rgb();
            pix = Image(16,16);
            pix.fill(Color(accept-.general-mdi-bg-color));
            button.setIcon(pix);
            mdi_area.setBackgroundColor(Color(accept-.general-mdi-bg-color))
        }
        else {
            mdi_area.setBackgroundColor(Color(dialog-general-mdi-bg-color))
        }
    } */
}

/* . */
void
currentGeneralMdiBackgroundColorChanged(EmbColor color)
{
   /* preview.general-mdi-bg-color = color.rgb()
    mdi_area.setBackgroundColor(Color(preview.general-mdi-bg-color)) */
    /*
    lambda method constructor?
    */
}

/* . */
void
checkboxTipOfTheDay_state_changed(void)
{
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

/* . */
void
checkboxShowScrollBars_state_changed(int checked)
{
/*
    preview.display-show-scrollbars = checked
    updateAllViewScrollBars(preview.display-show-scrollbars) */
}

/* . */
void
spinboxzoom_scale_action_in_value_changed(double value)
{
    /* dialog_display_zoom-scale-action-in = value */
}

/* . */
void
spinboxzoom_scale_action_out_value_changed(double value)
{
    /*dialog_display_zoom-scale-action-out = value */
}

/* . */
void
checkboxDisableBG_state_changed(int checked)
{
    /*dialog-printing-disable-bg = checked */
}

/* . */
void
chooseDisplayCrossHairColor(void)
{
    /*
    button = sender()
    if button) {
        color-dialog = color-dialog(Color(accept-.display-crosshair-color), this)
        connect(color-dialog, SIGNAL(currentColorChanged()), this, SLOT(currentDisplayCrossHairColorChanged()))
        color-dialog-exec()

        if color-dialog-result() == "Accepted") {
            accept.display-crosshair-color = color-dialog-selectedColor().rgb()
            pix = Image(16,16)
            pix.fill(Color(accept-.display-crosshair-color))
            button.setIcon(pix)
            updateAllViewCrossHairColors(accept-.display-crosshair-color)
        }
        else {
            updateAllViewCrossHairColors(dialog_display_crosshair-color)
        }
    } */
}

/* . */
void
currentDisplayCrossHairColorChanged(EmbColor color)
{
/*
    preview.display-crosshair-color = color.rgb()
    updateAllViewCrossHairColors(preview.display-crosshair-color) */
}

/* . */
void
chooseDisplayBackgroundColor(void)
{
    /*
    button = sender()
    if button) {
        color-dialog = color-dialog(Color(accept-.display-bg-color), this)
        connect(color-dialog, SIGNAL(currentColorChanged()), this, SLOT(currentDisplayBackgroundColorChanged()))
        color-dialog-exec()

        if color-dialog-result() == "Accepted") {
            accept.display-bg-color = color-dialog-selectedColor().rgb()
            pix = Image(16, 16)
            pix.fill(Color(accept-.display-bg-color))
            button.setIcon(pix)
            updateAllViewBackgroundColors(accept-.display-bg-color)
        else {
            update_all_view_background_colors(dialog_display_bg-color)
        }
    }
    */
}

/* . */
void
currentDisplayBackgroundColorChanged(EmbColor color)
{
    /*
    preview.display-bg-color = color.rgb();
    updateAllViewBackgroundColors(preview.display-bg-color);
    */
}

/* . */
void
chooseDisplaySelectBoxLeftColor(void)
{
    /*
    button = sender();
    if (button) {
        color-dialog = color-dialog(Color(accept--display-selectbox-left-color), this);
        connect(color-dialog, SIGNAL(currentColorChanged()), this, SLOT(currentDisplaySelectBoxLeftColorChanged()));
        color-dialog-exec();

        if (color-dialog-result() == tk.Dialog-Accepted) {
            accept-display-selectbox-left-color = color-dialog-selectedColor().rgb();
            pix = Image(16, 16);
            pix.fill(Color(accept--display-selectbox-left-color));
            button.setIcon(pix);
            update_all_view_selectbox_colors(
                "accept-display-selectbox");
        }
        else {
            update_all_view_selectbox_colors(
                "dialog_display_selectbox");
        }
    }
    */
}

/* . */
void
currentDisplaySelectBoxLeftColorChanged(color)
{
    /*
    preview-display-selectbox-left-color = color.rgb()
    update_all_view_selectbox_colors(
        "preview-display-selectbox");
    */
}

/* . */
void
chooseDisplaySelectBoxLeftFill(void)
{
    /*
    button = sender();
    if (button) {
        color-dialog = color-dialog(Color(accept-display-selectbox-left-fill), this);
        connect(color-dialog, SIGNAL(currentColorChanged()), this, SLOT(currentDisplaySelectBoxLeftFillChanged()));
        color-dialog-exec();

        if (color-dialog-result() == "Accepted") {
            accept-display-selectbox-left-fill = color-dialog-selectedColor().rgb();
            pix = Image(16, 16);
            pix.fill(Color(accept-display-selectbox-left-fill));
            button.setIcon(pix);
            update_all_view_selectbox_colors(
                "accept-display-selectbox");
        }
        else {
            update_all_view_selectbox_colors(
                "dialog-display-selectbox");
        }
    }
    */
}

void
current_display_selectbox_left_fill_changed(EmbColor color)
{
    /*
    preview-display-selectbox-left-fill = rgb(color);
    update_all_view_selectbox_colors(
        "preview-display-selectbox");
    */
}

void
choose_display_select_box_right_color(void)
{
    /*
    button = sender();
    if (button) {
        color-dialog = color-dialog(Color(accept--display-selectbox-right-color), this);
        connect(color-dialog, SIGNAL(currentColorChanged()), this, SLOT(currentDisplaySelectBoxRightColorChanged()));
        color-dialog-exec();

        if (color-dialog-result() == "Accepted") {
            accept-display-selectbox-right-color = color-dialog-selectedColor().rgb();
            pix = Image(16, 16);
            pix.fill(Color(accept-display-selectbox-right-color));
            button.setIcon(pix);
            update_all_view_selectbox_colors(
                "accept-display-selectbox");
        }
        else {
            update_all_view_selectbox_colors(
                "dialog-display-selectbox");
        }
    }
    */
}

void
currentDisplaySelectBoxRightColorChanged(EmbColor color)
{
    /*
    preview-display-selectbox-right-color = rgb(color);
    update_all_view_selectbox_colors(
        "preview-display-selectbox");
    */
}

/* . */
void
choose_display_selectbox_right_fill(void)
{
    /*
    int button = sender();
    if (button) {
        color-dialog = color-dialog(Color(accept-.display-selectbox-right-fill), this);
        connect(color-dialog, SIGNAL(currentColorChanged()), this, SLOT(currentDisplaySelectBoxRightFillChanged()));
        color-dialog-exec();

        if (color-dialog-result() == "Accepted") {
            accept.display-selectbox-right-fill = color-dialog-selectedColor().rgb();
            pix = Image(16, 16);
            pix.fill(Color(accept-.display-selectbox-right-fill));
            button.setIcon(pix);
            update_all_view_selectbox_colors(
                "accept-display-selectbox");
        }
        else {
            update_all_view_selectbox_colors(
                "dialog-display-selectbox");
        }
    }
    */
}

/* . */
void
currentDisplaySelectBoxRightFillChanged(color)
{
    /*
    preview.display-selectbox-right-fill = color.rgb();
    update_all_view_selectbox_colors(
        "preview-display-selectbox");
    */
}

/* . */
void
spinboxDisplaySelectBoxAlpha_value_changed(double value)
{
    /*
    preview.display-selectbox-alpha = value;
    update_all_view_selectbox_colors(
        "accept-display-selectbox");
    */
}

/* . */
void
checkboxCustomFilter_state_changed(int checked)
{
    /*
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
    */
}

/* . */
void
buttonCustomFilterSelectAllClicked(void)
{
    /*
    buttonCustomFilterSelectAll(1);
    opensave-custom-filter = "supported";
    */
}

/* . */
void
buttonCustomFilterClearAllClicked(void)
{
    /*
    buttonCustomFilterClearAll(0);
    opensave-custom-filter.clear();
    */
}

/* .
*/
void
checkboxGridColorMatchCrossHair_state_changed(int checked)
{
    /*
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
}

/* . */
void
chooseGridColor(void)
{
    /*
    button = sender()
    if (button) {
        color-dialog = color-dialog(Color(accept-.grid-color), this)
        connect(color-dialog, SIGNAL(currentColorChanged()), this, SLOT(currentGridColorChanged()))
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
    */
}

/* . */
void
current_grid_color_changed(EmbColor color)
{
    /*
    (define previewGridcolor color);
    (update-all-viewGridcolors "preview");
    */
}

/* . */
void
checkbox_grid_load_from_file_state_changed(int checked)
{
    /*
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

    combobox_grid_type = parent.find-child("combobox_grid_type")
    if combobox_grid_type) {
        combobox_grid_type.setEnabled(!dialogGridload-from-file);
    }

    checkbox_grid_center_on_origin = parent.find-child("checkbox_grid_center_on_origin")
    if checkbox_grid_center_on_origin) {
        checkbox_grid_center_on_origin.setEnabled(!dialogGridload-from-file);
    }

    label_grid_center_x = parent.find-child("label_grid_center_x")
    if (label_grid_center_x) {
        label_grid_center_x.setEnabled(!dialogGridload-from-file && ! dialog_grid_center_on_origin);
    }

    spinboxGridCenterX = parent.find-child("spinboxGridCenterX")
    if (spinboxGridCenterX) {
        spinboxGridCenterX.setEnabled(!dialogGridload-from-file && ! dialog_grid_center_on_origin);
    }

    labelGridcenter-y = parent.find-child("labelGridcenter-y")
    if (labelGridcenter-y) {
        labelGridcenter-y.setEnabled(!dialogGridload-from-file && ! dialog_grid_center_on_origin);
    }

    spinboxGridcenter-y = parent.find-child("spinboxGridcenter-y")
    if (spinboxGridcenter-y) {
        spinboxGridcenter-y.setEnabled(!dialogGridload-from-file && ! dialog_grid_center_on_origin);
    }

    labelGridsizeX = parent.find-child("labelGridsizeX")
    if labelGridsizeX) {
        labelGridsizeX.setEnabled(!dialogGridload-from-file);
    }

    spinboxGridsizeX = parent.find-child("spinboxGridsizeX")
    if spinboxGridsizeX) {
        spinboxGridsizeX.setEnabled(!dialogGridload-from-file);
    }

    labelGridsizeY = parent.find-child("labelGridsizeY")
    if labelGridsizeY) {
        labelGridsizeY.setEnabled(!dialogGridload-from-file);
    }

    spinboxGridsizeY = parent.find-child("spinboxGridsizeY")
    if (spinboxGridsizeY) {
        spinboxGridsizeY.setEnabled(!dialogGridload-from-file);
    }

    labelGridSpacingX = parent.find-child("labelGridSpacingX")
    if (labelGridSpacingX) {
        labelGridSpacingX.setEnabled(!dialogGridload-from-file)

    spinboxGridSpacingX = parent.find-child("spinboxGridSpacingX")
    if (spinboxGridSpacingX) {
        spinboxGridSpacingX.setEnabled(!dialogGridload-from-file);
    }

    labelGridSpacingY = parent.find-child("labelGridSpacingY")
    if (labelGridSpacingY) {
        labelGridSpacingY.setEnabled(!dialogGridload-from-file);
    }

    spinboxGridSpacingY = parent.find-child("spinboxGridSpacingY")
    if spinboxGridSpacingY) {
        spinboxGridSpacingY.setEnabled(!dialogGridload-from-file);
    }

    labelGridsizeRadius = parent.find-child("labelGridsizeRadius")
    if labelGridsizeRadius) {
        labelGridsizeRadius.setEnabled(!dialogGridload-from-file);
    }

    spinboxGridsizeRadius = parent.find-child("spinboxGridsizeRadius")
    if spinboxGridsizeRadius) {
        spinboxGridsizeRadius.setEnabled(!dialogGridload-from-file);
    }

    labelGridSpacingRadius = parent.find-child("labelGridSpacingRadius")
    if labelGridSpacingRadius) {
        labelGridSpacingRadius.setEnabled(!dialogGridload-from-file);
    }

    spinboxGridSpacingRadius = parent.find-child("spinboxGridSpacingRadius")
    if spinboxGridSpacingRadius) {
        spinboxGridSpacingRadius.setEnabled(!dialogGridload-from-file);
    }

    labelGridSpacingAngle = parent.find-child("labelGridSpacingAngle")
    if labelGridSpacingAngle) {
        labelGridSpacingAngle.setEnabled(!dialogGridload-from-file);
    }

    spinboxGridSpacingAngle = parent.find-child("spinboxGridSpacingAngle")
    if spinboxGridSpacingAngle) {
        spinboxGridSpacingAngle.setEnabled(!dialogGridload-from-file);
    }
    */
}

/* .
 */
void
combobox_grid_type_current_index_changed(int type)
{
    /*
    dialogGridtype = type-

    sender-obj = sender()
    if (!sender-obj) {
        return;
    }

    parent = sender-obj.parent()
    if (!parent) {
        return;
    }

    visibility = False
    if type- == "Circular") {
        visibility = 1;
    }

    label_grid_size_x = parent.find-child("labelGridsizeX");
    if (label_grid_size_x) {
        label_grid_size_x.visible = !visibility;
    }

    spinboxGridsizeX = parent.find-child("spinboxGridsizeX");
    if (spinboxGridsizeX) {
        spinboxGridsizeX.visible = !visibility;
    }

    labelGridsizeY = parent.find-child("labelGridsizeY");
    if (labelGridsizeY) {
        labelGridsizeY.visible = !visibility;
    }

    spinboxGridsizeY = parent.find-child("spinboxGridsizeY");
    if (spinboxGridsizeY) {
        spinboxGridsizeY.visible = !visibility;
    }

    labelGridSpacingX = parent.find-child("labelGridSpacingX");
    if (labelGridSpacingX) {
        labelGridSpacingX.visible = !visibility;
    }

    spinboxGridSpacingX = parent.find-child("spinboxGridSpacingX")
    if spinboxGridSpacingX) {
        spinboxGridSpacingX.visible = !visibility;
    }

    labelGridSpacingY = parent.find-child("labelGridSpacingY")
    if labelGridSpacingY) {
        labelGridSpacingY.visible = !visibility;
    }

    spinboxGridSpacingY = parent.find-child("spinboxGridSpacingY")
    if spinboxGridSpacingY) {
        spinboxGridSpacingY.visible = !visibility;
    }

    labelGridsizeRadius = parent.find-child("labelGridsizeRadius")
    if labelGridsizeRadius) {
        labelGridsizeRadius.visible = visibility;
    }

    spinboxGridsizeRadius = parent.find-child("spinboxGridsizeRadius")
    if spinboxGridsizeRadius) {
        spinboxGridsizeRadius.visible = visibility;
    }

    labelGridSpacingRadius = parent.find-child("labelGridSpacingRadius")
    if (labelGridSpacingRadius) {
        labelGridSpacingRadius.visible = visibility;
    }

    spinboxGridSpacingRadius = parent.find-child("spinboxGridSpacingRadius")
    if (spinboxGridSpacingRadius) {
        spinboxGridSpacingRadius.visible = visibility;
    }

    labelGridSpacingAngle = parent.find-child("labelGridSpacingAngle")
    if (labelGridSpacingAngle) {
        labelGridSpacingAngle.visible = visibility;
    }

    spinboxGridSpacingAngle = parent.find-child("spinboxGridSpacingAngle")
    if (spinboxGridSpacingAngle) {
        spinboxGridSpacingAngle.visible = visibility;
    }
    */
}

/*
 */
void
checkbox_grid_center_on_origin_state_changed(int checked)
{
    /*
    dialog_grid_center_on_origin = checked

    sender-obj = sender()
    if (!sender-obj) {
        return

    parent = sender-obj.parent();
    if (!parent) {
        return
    }

    label_grid_center_x = parent.find-child("label_grid_center_x");
    if label_grid_center_x) {
        label_grid_center_x.setEnabled(!dialog_grid_center_on_origin);
    }

    spinboxGridCenterX = parent.find-child("spinboxGridCenterX");
    if spinboxGridCenterX) {
        spinboxGridCenterX.setEnabled(!dialog_grid_center_on_origin);
    }

    labelGridcenter-y = parent.find-child("labelGridcenter-y");
    if labelGridcenter-y) {
        labelGridcenter-y.setEnabled(!dialog_grid_center_on_origin);
    }

    spinboxGridcenter-y = parent.find-child("spinboxGridcenter-y");
    if spinboxGridcenter-y) {
        spinboxGridcenter-y.setEnabled(!dialog_grid_center_on_origin);
    }
*/
}

void
combobox_ruler_metric_current_index_changed(int index)
{
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

/*
 * Needs to be aware of the current view.
 */
void
choose_ruler_color(void)
{
    /*
    color_dialog = color_dialog(
        Color(accept-.ruler-color), this);
    connect(color-dialog, SIGNAL(currentColorChanged()),
        this, SLOT(currentRulerColorChanged()));
    color-dialog-exec();

    if (color-dialog-result() == "QDialog-Accepted") {
        accept.ruler-color = color-dialog-selectedColor().rgb();
        pix = Image(16, 16);
        pix.fill(Color(accept-.ruler-color));
        button.setIcon(pix);
        updateAllViewRulerColors(accept-.ruler-color);
    }
    else {
        updateAllViewRulerColors(dialog-ruler-color);
    }
    */
}

/*
 */
void
current_ruler_color_changed(EmbColor color)
{
    /*
    preview.ruler-color = rgb(color);
    update_all_view_ruler_colors("preview-ruler-color");
    */
}

void
button_qsnap_select_all_clicked(void)
{
    /* buttonQSnapSelectAll(1); */
}

/*
 * TODO:
 * Figure out how to abstract the slot in a way that it can be used for
 * comboboxes in general
 * Currently comboboxQSnapLocatorColorCurrent-index_changed(int index)
 *       comboboxSelectionCoolGripColorCurrent-index_changed(int index)
 *       comboboxSelectionHotGripColorCurrent-index_changed(int index)
 * are all similar except the dialog- variable being worked on and the
 * tk.Variant.

void
buttonQSnapClearAllClicked(void)
    buttonQSnapClearAll(0)
} */

/* TODO: Alert user if color matched the display bg color */
void
comboboxQSnapLocatorColorCurrent_index_changed(int index)
{
    /*
    combobox = sender()
    default-color = tk.Rgb(255,255,0)
    # Yellow
    if combobox) {
        dialog-qsnap-locator-color, ok = combobox.itemData(index).toUInt()
        if (!ok) {
            dialog-qsnap-locator-color = default-color
    else) {
        dialog-qsnap-locator-color = default-color
    */
}

/* . */
void
slider_qsnap_locator_size_value_changed(double value)
{
    set_float(mainwnd->state, "dialog-qsnap-locator-size", value);
}

/* . */
void
slider_qsnap_aperture_size_value_changed(double value)
{
    set_float(mainwnd->state, "dialog-qsnap-aperture-size", value);
}

void
checkbox_lwt_show_lwt_state_changed(int checked)
{
    /*
    preview.lwt-show-lwt = checked;
    if (get_int(mainwnd->state, "preview-lwt-show-lwt")) {
        enable_lwt();
    }
    else {
        disable_lwt();
    }

    sender-obj = sender();
    if (sender-obj) {
        parent = sender-obj.parent();
        if (parent) {
            checkboxRealRender = parent.find-child("checkboxRealRender");
            if checkboxRealRender) {
                checkboxRealRender.setEnabled(preview.lwt-show-lwt);
            }
        }
    }
    */
}

void
checkbox_lwt_real_render_state_changed(int checked)
{
    set_int(mainwnd->state, "preview-lwt-real-render", checked);
    if (get_int(mainwnd->state, "preview-lwt-real-render")) {
        enableReal();
    }
    else {
        disableReal();
    }
}

/*
void
comboboxSelectionCoolGripColorCurrent-index_changed(index)
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
combobox-selectionHotGripColorCurrent-index_changed(index)
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

/* . */
void
accept_changes(void)
{
    /*
    for k in preview.keys()
        dialog[k] = preview[k]
    for k in accept.keys()
        dialog[k] = preview[k]

    if dialogGridcolor-match-crosshair) {
        dialogGridcolor = accept-display-crosshair-color
    */

    /* Make sure the user sees the changes applied immediately. */
    /*
    mdi_area.use_background_logo(dialog-general-mdi-bg-use-logo);
    mdi_area.use_background_texture(dialog-general-mdi-bg-use-texture);
    mdi_area.use_background_color(dialog-general-mdi-bg-use-color);
    mdi_area.set_background-logo(dialog-general-mdi-bg-logo);
    mdi_area.set_background-texture(dialog-general-mdi-bg-texture);
    mdi_area.set_background-color(dialog-general-mdi-bg-color);
    icon-resize(dialog-general-icon-size);
    update-all-view-scrollBars(dialog_display_show-scrollbars);
    update-all-view-cross-hair-colors(dialog_display_crosshair-color);
    update-all-view-background-colors(dialog_display_bg-color);
    update_all_view_selectbox_colors(
        "dialog_display_selectbox"); */
        /*_left-color,
        dialog_display_selectbox_left-fill,
        dialog_display_selectbox_right-color,
        dialog_display_selectbox_right-fill,
        dialog_display_selectbox_alpha
        */
        /*
    update-all-viewGridcolors(dialogGridcolor);
    update-all-view-ruler-colors(dialog-ruler-color);

    if (get_int(mainwnd->state, "dialog-lwt-show-lwt")) {
        enableLwt();
    }
    else {
        disableLwt();
    }

    if (get_int(mainwnd->state, "dialog-lwt-real-render")) {
        enableReal();
    }
    else {
        disableReal();
    }

    update_pick_add_mode(dialog_selection_mode_pickadd);

    writeSettings();
    accept();
    */
}

/* TODO: inform the user if they have changed settings
 *
 * Update the view since the user must accept the preview
 */
void
reject_changes(void)
{
    /*
    mdi_area.use_background_logo(dialog-general-mdi-bg-use-logo);
    mdi_area.use_background_texture(dialog-general-mdi-bg-use-texture);
    mdi_area.use_background_color(dialog-general-mdi-bg-use-color);
    mdi_area.set_background-logo(dialog-general-mdi-bg-logo);
    mdi_area.set_background-texture(dialog-general-mdi-bg-texture);
    mdi_area.set_background-color(dialog-general-mdi-bg-color);
    update-all-view-scroll-bars(dialog_display_show-scrollbars);
    update-all-view-cross-hair-colors(dialog_display_crosshair-color);
    update-all-view-background-colors(dialog_display_bg-color);
    update_all_view_selectbox_colors(
        dialog_display_selectbox_left-color,
        dialog_display_selectbox_left-fill,
        dialog_display_selectbox_right-color,
        dialog_display_selectbox_right-fill,
        dialog_display_selectbox_alpha);
    update-all-viewGridcolors(dialogGridcolor);
    update-all-view-ruler-colors(dialog-ruler-color);

    if (get_int(mainwnd->state, "dialog-lwt-show-lwt")) {
        enable_lwt();
    }
    else {
        disable_lwt();
    }

    if (get_int(mainwnd->state, "dialog-lwt-real-render")) {
        enable_real();
    }
    else {
        disable_real();
    }

    reject();
    */
}

/*
 * For safe packaging, and to reduce the risk of program
 * crashing errors the resources are loaded into the
 * application folder each time the program boots.
 */
void
load_image(char *path)
{
    char image_fname[200];
    FILE *f;
    sprintf(image_fname, "%s%s%s",
        get_str(mainwnd->state, "application-folder"),
        get_str(mainwnd->state, "os-seperator"),
        path);
    f = fopen(image_fname, "wb");
    /*
    icon = icons[path].replace("\n", "")
    icon = icon.replace(" ", "")
    decoded-image = binascii.unhexlify(icon)
    image-file.write(decoded-image)
    return tk.PhotoImage(file=image-fname)
    */
    fclose(f);
}

/* Return the host system label for debugging purposes.
 */
void platform_string(void)
{
    /*
    host-system = os.uname().sysname + " " + os.uname().release
    debug_message(f"Platform: {host-system}")
    return host-system;
    */
}

/* The Color class definition.

int color-mode;
int stitches-total;
int stitches-real;
int stitches-jump;
int stitches-trim;
int color-total;
int color-changes;
*/

/* 
 * To manage thread colors and such, this class can call the settings
 * JSON dict.
 */
void
color_init(void)
{
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

void
tree_view(void)
{

}

/*
 *  To display an embedded image as a widget in SDL2.
 */
void
image_widget_init(char *filename)
{
    /* debug_message("ImageWidget Constructor")
    img = img-load(filename)

    min-width = img.width()
    min-height = img.height()
    max-width = img.width()
    max-height = img.height()

    img.show() */
}

void
image_widget_load(char *fileName)
{
    /* return img.load(fileName) */
}

void
image_widget_save(char *fileName)
{
    /* return img.save(fileName, "PNG") */
}

/*
 *
 */
void
paint_event(SDL_Event event)
{
    /* 
    painter = Painter();
    painter.setViewport(0, 0, img.width(), img.height());
    painter.setWindow(0, 0, img.width(), img.height());
    painter.drawImage(0, 0, img);
    */
}

/*
 *  Preview Dialog
 *
 *  Preview the output. 
 */
/*
void
preview_dialog_init(self, parent, caption, dir, filter)
{
    debug_message("PreviewDialog Constructor");

    */ /* TODO: get actual thumbnail image from file, lets also use a size of 128x128 for now...*/ /* 
    */ /* TODO: make thumbnail size adjustable thru settings dialog */ /* 
    img-widget = ImageWidget("icons/default/nopreview.png")

    lay = layout()
    if lay) {
        columns = lay.columnCount()
        rows = lay.rowCount()
        lay, add_widget(img-widget, 0, columns, rows, 1)

    modal = 1
    option = "FileDialog-DontUseNativeDialog"
    view-mode = "FileDialog-Detail"
    file-mode = "FileDialog-ExistingFiles"

    */ /* TODO: connect the currentChanged signal to update the preview img-widget.*/ /* 
}

void
PreviewDialogtoPolyline(self, pattern, obj-pos, obj-path, layer, color, line_type,  line_weight)
{
    debug_message("Unused arguments:")
    debug_message(f"    {pattern}, {obj-pos}, {obj-path},")
    debug_message(f"    {layer}, {color}, {line_type}, {line_weight}.")
}

/* -----------------------------------------------
 *
 * The color selection docker class definition file.
 *
 * The color selection docker.
 *
 * From a duplicated version.
 */
/*
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
    curColor = 0 */ /* TODO: color ByLayer */ /* 
    curline_type = "ByLayer"
    curline_weight = "ByLayer"

    # Due to strange Qt4.2.3 feature the child window icon is not drawn
    # in the main menu if showMaximized() is called for a non-visible child window
    # Therefore calling show() first...
    show()
    showMaximized()

    setFocusPolicy(WheelFocus)
    setFocus()

    onWindowActivated()
}

/* Prevent memory leaks by deleting any objects that
 * were removed from the scene.
 */
void
delete_objects(void)
{
    /*
    (delete-all hash-deleted-objects.begin() hash-deleted-objects.end())
    (clear hash-deleted-objects)

    Prevent memory leaks by deleting any unused instances
    (delete-all preview-object-list.begin() preview-object-list.end())
    (clear preview-object-list)
    */
}

/* . */
void
load_ruler_settings(void)
{
    return;
}

/* . */
void
center(void)
{
    /* return map_to_scene(rect().center()); */
}

/* . */
void
enter_event(SDL_Event event)
{
    /*
    mdi-win = mw;
    if (mdi-win) {
        mdiArea.setActiveSubWindow(mdi-win);
    }
    */
}

/* . */
void
add_object(/*self, obj*/)
{
    /*
    gscene.addItem(-obj);
    scene_update();
    hash-deleted-objects.remove(-obj.-objID);
    */
}

/* .
void
delete_object(int obj)
{
    printf("%d\n", obj); */
    /*
    # NOTE: We really just remove the objects from the scene.
    #       deletion actually occurs in the destructor.
    obj.set_selected(0);
    gscene.remove-item(-obj);
    scene_update();
    hash-deleted-objects.insert(-obj.-objID, obj);
}
*/

void
preview_on(int clone, int mode, int x, int y, int data)
{
    debug_message("View preview-on()");
    printf("%d, %d, %d, %d\n", mode, x, y, data);
    /*
    preview-off();
    # Free the old objects before creating ones

    preview-mode = mode;
    */

    /* Create objects and add them to the scene in an item group. */
    /*
    if clone == "PREVIEW-CLONE-SELECTED":
        preview-object-list = create-object-list(gscene.selected-items());
    elif clone == PREVIEW-CLONE-RUBBER:
        preview-object-list = create-object-list(rubber-room-list);
    else {
        return;
    preview-object-item-group = gscene.create-item-group(preview-object-list);

    if (preview-mode == PREVIEW-MODE-MOVE ||
        preview-mode == "PREVIEW-MODE-ROTATE" ||
        preview-mode == "PREVIEW-MODE-SCALE") {
        preview_point = Vector(x, y);
        # NOTE: Move: basePt
        # Rotate: basePt
        # Scale: basePt
        preview-data = data;
        # NOTE: Move: unused
        # Rotate: refAngle
        # Scale: refFactor
        preview-active = 1;
    }
    else {
        preview-mode = PREVIEW-MODE-NULL;
        preview_point = Vector();
        preview-data = 0;
        preview-active = 0;
    }
    */

    scene_update();
}

void
preview_off(void)
{
    /* Prevent memory leaks by deleting any unused instances. */
    /*
    DeleteAll(preview-object-list.begin(), preview-object-list.end());
    preview-object-list.clear();

    if (preview-object-item-group) {
        gscene.remove-item(preview-object-item-group);
        del preview-object-item-group;
        preview-object-item-group = 0;
    }
    */

    set_int(mainwnd->state, "preview-active", 0);

    scene_update();
}

void
enable_move_rapid_fire(void)
{
    debug_message("Enabling rapid move.");
    set_int(mainwnd->state, "rapid-move-active", 1);
}

void
disable_move_rapid_fire(void)
{
    debug_message("Disabling rapid move.");
    set_int(mainwnd->state, "rapid-move-active", 0);
}

void
set_qsnap(int active)
{
    debug_message("View toggleQSnap()");
    printf("%d", active);
    set_override_cursor("WaitCursor");
    /*
    qsnap-toggle = on;
    gscene.set_property("ENABLE-QSNAP", active); */
    scene_update();
    restore_override_cursor();
}

/* TODO: finish this.
 */
void
set_qtrack(int active)
{
    debug_message("View toggleQTrack()");
    printf("%d", active);
    set_override_cursor("WaitCursor");
    /* gscene.set_property("ENABLE-QTRACK", active); */
    scene_update();
    restore_override_cursor();
}

void
set_lwt(int active)
{
    debug_message("View toggleLwt()");
    printf("%d", active);
    set_override_cursor("WaitCursor");
    /* gscene.set_property("ENABLE-LWT", active); */
    scene_update();
    restore_override_cursor();
}

void
set_real(int active)
{
    debug_message("View toggleReal()");
    printf("%d", active);
    set_override_cursor("WaitCursor");
    /* gscene.set_property("ENABLE-REAL", active); */
    scene_update();
    restore_override_cursor();
}

/* lwt and real should be per-document
 */
int
is_lwt_enabled(void)
{
    return get_int(mainwnd->state, "show-lwt");
}

int
is_real_enabled(void)
{
    return get_int(mainwnd->state, "real-render");
}

void
update_mouse_coords(EmbVector position)
{
    printf("called with %f", position.x);
    /*
    EmbVector view_mouse_point = Vector(x, y);
    scene-mouse-point = to-emb-vector(map_to_scene(view_mouse_point));
    gscene.set_property("SCENE-QSNAP-POINT", scene-mouse-point);
    */
    /* TODO: if qsnap functionality is enabled, use it rather than the mouse point. */ /*
    gscene.set_property("SCENE-MOUSE-POINT", scene-mouse-point);
    gscene.set_property("view_mouse_point", view_mouse_point);
    statusbar.setMouseCoord(scene-mouse-point.x, -scene-mouse-point.y);
    */
}

/* 
 * NOTE:
 * crosshair-size is in pixels and is a percentage of your screen width
 * Example: (1280*0.05)/2 = 32, thus 32 + 1 + 32 = 65 pixel wide crosshair.
 */
void
set_crosshair_size(double percent)
{
    printf("called with %f", percent);
    /*
    screen-width = qApp.screens()[0].geometry().width()
    if (percent > 0 && percent < 100) {
        crosshair-size = (screen-width*(percent/100.0))/2;
    }
    else {
        crosshair-size = screen-width;
    }
    */
}

/* Sets the crosshair color for the current view. */
void
set_crosshair_color(EmbColor color)
{
    printf("%d\n", color.r);
    printf("%d\n", color.g);
    printf("%d\n", color.b);
}

void
set_corner_button(void)
{
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
corner_button_clicked(void)
{
    debug_message("Corner Button Clicked.");
    /*
    display = display-scrollbar-widget-num;
    action-hash.value(display).trigger();
    */
}

void
selection_changed(void)
{
    /*
    if (dock_prop_edit.isVisible()) {
        dock_prop_edit.setselected-items(gscene.selected-items());
    }
    */
}

void
mouse_double_click_event(SDL_Event event)
{
    /*
    if (event.button() == LEFT_BUTTON) {
        item = gscene.itemAt(map_to_scene(event.pos()), QTransform());
        if (item) {
            dock_prop_edit.show();
        }
    }
    */
}

void
mouse_press_event(SDL_Event event)
{
    /*
    update-mouse-coords(event.x, event.y);
    if (event.button() == "LeftButton") {
        path = Path();
        pickList = gscene.items(Rect(map_to_scene(
            view_mouse_point.x-pickBoxSize,
            view_mouse_point.y-pickBoxSize),
            map_to_scene(view_mouse_point.x+pickBoxSize,
            view_mouse_point.y+pickBoxSize)));

        itemsInPickBox = pickList.size();
        if (itemsInPickBox && !selecting-active && !gripping-active) {
            itemsAlreadelta-ySelected = pickList.at(0).is-selected();
            if (!itemsAlreadelta-ySelected) {
                pickList.at(0).set_selected(1);
            }
            else {
                foundGrip = 0;
                base = pickList[0];
                */ /* TODO: Allow multiple objects to be gripped at once */ /* 
                if (!base) {
                    return;
                }

                qsnap-offset = Vector(qsnap-locator-size, qsnap-locator-size);
                gripPoint = base.mouseSnapPoint(scene-mouse-point);
                p1 = map_from_scene(gripPoint) - qsnap-offset;
                q1 = map_from_scene(gripPoint) + qsnap-offset;
                gripRect = make_rectangle(map_to_scene(p1), map_to_scene(q1));
                pickRect = make_rectangle(map_to_scene(view_mouse_point.x-pickBoxSize, view_mouse_point.y-pickBoxSize),
                                map_to_scene(view_mouse_point.x+pickBoxSize, view_mouse_point.y+pickBoxSize))
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
                    scenePressPoint = map_to_scene(pressPoint);
                }

        elif (gripping-active) {
            stop-gripping(1);
        }
        elif (!selecting-active) {
            selecting-active = 1;
            pressPoint = event.pos();
            scenePressPoint = map_to_scene(pressPoint);

            if (!select-box:
                select-box = select-box(QRubberBand-Rectangle);
            select-box.set_geometry(Rect(pressPoint, pressPoint));
            select-box.show();
        }
        else {
            selecting-active = 0;
            select-box.hide();
            releasePoint = event.pos();
            scene-release-point = map_to_scene(releasePoint);

            #Start select-box Code
            path.add-polygon(map_to_scene(select-box.geometry()));
            if (scene-release-point.x > scenePressPoint.x) {
                if (selection-mode-pickadd) {
                    if (shift-key) {
                        item_list = gscene.items(path, "ContainsItemShape");
                        for item in item_list: {
                            item.set_selected(0);
                        }
                    }
                    else {
                        item_list = gscene.items(path, "ContainsItemShape");
                        for item in item_list: {
                            item.set_selected(1);
                        }
                    }
                }
                else {
                    if (shift-key) {
                        item_list = gscene.items(path, "ContainsItemShape");
                        if (!item_list.size() {
                            clear_selection();
                        }
                        else {
                            for item in item_list: {
                                item.set_selected(!item.is-selected()) #Toggle selected;
                            }
                        }
                    }
                    else {
                        clear_selection();
                        item_list = gscene.items(path, "ContainsItemShape");
                        for item in item_list: {
                            item.set_selected(1);
                        }
                    }
            }
            else {
                if (selection-mode-pickadd) {
                    if (shift-key) {
                        item_list = gscene.items(path, "intersects-item-shape");
                        for item in item_list) {
                            item.set_selected(0);
                        }
                    }
                    else {
                        item_list = gscene.items(path, "intersects-item-shape");
                        for (item in item_list) {
                            item.set_selected(1);
                        }
                    }
                }
                else {
                    if (shift-key) {
                        item_list = gscene.items(path, "intersects-item-shape");
                        if (!item_list.size(void);
                            clear_selection();

                        else {
                            for item in item_list:
                                item.set_selected(!item.is-selected());
                                #Toggle selected

                    }
                    else {
                        clear_selection();
                        item_list = gscene.items(path, "intersects-item-shape");
                        for item in item_list) {
                            item.set_selected(1);
                        }
                   }
               }
            #End select-box Code
        }

        if (get_int(mainwnd->state, "pasting-active")) {
            item_list = paste-object-item-group.childItems();
            gscene.destroy-item-group(paste-object-item-group);
            for (item in item_list) {
                gscene.remove-item(item);
                # Prevent Qt Runtime Warning,
                # QGraphicsScene-addItem: item has alreadelta-y been
                # added to self scene
            }

            for (item in item_list) {
                if (item) {
                    debug_message("TODO: Loop bodelta-y");
                }
            }

            pasting-active = 0;
            selecting-active = 0;
        }

        if (get_int(mainwnd->state, "zoom-window-active")) {
            fit_in_view(path.bounding-rect(), Qt-KeepAspectRatio);
            clear_selection();
        }
    }

    if (event.button() == "MiddleButton") {
        pan-start(event.pos());
        */
        /* The Undo command will record the spot where
         * the pan started. */
        /*
        event.accept();
    }
    */
    scene_update();
}

void
pan_start(EmbVector point)
{
    printf("%f", point.x);
    /*
    recalculate_limits();

    align-scene-point-withViewPoint(map_to_scene(point), point);

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
recalculate_limits(void)
{
    /*
    SDL-Rect scene-rect, new-rect, rect;
    viewRect = make_rectangle(map_to_scene(rect().top-left()), map_to_scene(rect().bottom-right()));
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
    */
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
align_scene_point_with_view_point(EmbVector scene_point, EmbVector view_point)
{
    /*
    view-center = center();
    point-before = scene-point;
    center-on(view-center); */
    /* Reshift to the center so the scene and view points align. */
    /*
    point-after = map_to_scene(view-point);
    offset = point-before - point-after;
    new-center = view-center + offset;
    center-on(new-center); */
}

void
mouse_move_event(SDL_Event event)
{
    /*
    mouse = Cursor-pos();
    update-mouse-coords(mouse.x, mouse.y);
    move-point = event.pos();
    scenemove-point = map_to_scene(move-point);

    if (preview-active) { */
        /*
        if (preview-mode == "PREVIEW-MODE-MOVE") {
            preview-object-item-group.set_pos(scene-mouse-point - preview_point);
        }
        elif (preview-mode == "PREVIEW-MODE-ROTATE") {
            x = preview_point.x;
            y = preview_point.y;
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
            x = preview_point.x;
            y = preview_point.y;
            scale_factor = preview-data;

            factor = Line(x, y, scene-mouse-point.x, scene-mouse-point.y).length() / scale_factor;

            preview-object-item-group.setScale(1);
            preview-object-item-group.set_pos(0,0);

            if (scale_factor <= 0.0) {
                message = "Hi there. If you are not a developer, report this as a bug.";
                message += " If you are a developer, your code needs examined,";
                message += " and possibly your head too.";
                critical(self, translate("scale_factor Error"), translate(message));
            }
            else { */
                /* Calculate the offset */
                /*
                old = Vector(0, 0);
                scale_line = Line(x, y, old.x, old.y);
                scale_line.set_length(scale-line.length()*factor);

                delta_x = scale_line.point2.x - old.x;
                delta_y = scale_line.point2.y - old.y;
            }
        }
    } */
    /*
                preview-object-item-group.setScale(preview-object-item-group.scale()*factor);
                preview-object-item-group.moveBy(delta-x, delta-y);
    */

    if (get_int(mainwnd->state, "pasting-active")) {
        /*
        v = scene-mouse-point.subtract(pasteDelta);
        paste-object-item-group.set_pos(v);
        */
    }

    if (get_int(mainwnd->state, "moving-active")) {
        /* Ensure that the preview is only shown if the mouse has moved. */
        if (!get_int(mainwnd->state, "preview-active")) {
            /* preview-on("PREVIEW-CLONE-SELECTED", "PREVIEW-MODE-MOVE",
                   scenePressPoint.x, scenePressPoint.y, 0)  */
        }
    }

    if (get_int(mainwnd->state, "selecting-active")) {
        /*
        if (scenemove-point.x >= scenePressPoint.x(void)) {
            select-box.setDirection(1);
        }

        else { select-box.setDirection(0);
        select-box.set_geometry(
            make_rectangle(map-from-scene(scenePressPoint), event.pos()).normalized());
        event.accept();
        */
    }

    if (get_int(mainwnd->state, "panning-active")) {
        /*
        horizontal-scroll-bar().set_value(
            horizontal-scroll-bar().value()
             - (event.x - pan-start-x));
        vertical-scroll-bar().set_value(
            vertical-scroll-bar().value()
             - (event.y - pan-start-y));
        pan_start = event.position;
        event.accept();
        */
    }

    scene_update();
}

void
mouse_release_event(SDL_Event event)
{
    /*
    update_mouse_coords(event);
    if (event.button() == "left-button") {
        if (moving-active) {
            preview-off();
            delta = scene-mouse-point.subtract(scene-press-point); */
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
        /* TODO: Make this customizable */ /*
        event.accept();
    }

    if (event.button() == "XButton2") {
        debug_message("XButton2");
        main-redo() */
        /* TODO: Make this customizable */
        /* event.accept();
    }
    */
    scene_update();
}

void
wheel_event(SDL_Event event)
{
    double zoom_dir = 1.0;
    /* zoom_dir = event.pixel-delta().y; */
    /* TODO: double check self */
    /*mousePoint = event.global-pos() */
    /* TODO: this is causing weird versioning errors, */
    /* this appears to be supported on Qt5.12. */
    /* update-mouse-coords(mousePoint.x, mousePoint.y) */
    if (zoom_dir > 0) {
        debug_message(".");
    }
    else {
        debug_message(".");
    }
}

void
context_menu_event(SDL_Event event)
{
    int i;
    /*
    iconTheme = general-icon-theme;

    menu = "";
    selection-empty = item_list.is-empty();
    */

    for (i=0; i<n_selected; i++) {
        /*
        if (selected_items[i].type != "Not Set") {
            selection-empty = 0;
            break;
        }
        */
    }

    if (get_int(mainwnd->state, "pasting-active")) {
        return;
    }

    /*
    if (zoom-window-active) {
        cancel-zoom-win-action = Action("&Cancel (zoom-window-action)");
        cancel-zoom-win-action.set_status-tip("Cancels the zoom-window-action Command.");
        connect(cancel-zoom-win-action, SIGNAL(triggered()), self, SLOT(escapePressed()));
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
        connect(delete-action, SIGNAL(triggered()), self, SLOT(deleteSelected()));
        menu.add-action(delete-action);

        move-action = Action(load-icon(move-xpm), "&Move");
        status-tip = "Displaces objects a specified distance in a specified direction.";
        move-action.set_status-tip(status-tip);
        connect(move-action, SIGNAL(triggered()), self, SLOT(move-action()));
        menu.add-action(move-action);

        scale-action = Action(load-icon(scale-xpm), "Sca&le");
        status-tip = "Enlarges or reduces objects proportionally in the X, Y, and Z directions.";
        scale-action.set_status-tip(status-tip);
        connect(scale-action, SIGNAL(triggered()), self, SLOT(scale-action()))
        menu.add-action(scale-action);

        rotate-action = Action(load-icon(rotate-xpm), "R&otate");
        status-tip = "Rotates objects about a base point.";
        rotate-action.set_status-tip(status-tip);
        connect(rotate-action, SIGNAL(triggered()), self, SLOT(rotate-action()));
        menu.add-action(rotate-action);

        menu.add-separator();

        clear-action = Action("Cle&ar Selection");
        clear-action.set_status-tip("Removes all objects from the selection set.");
        connect(clear-action, SIGNAL(triggered()), self, SLOT(clear_selection()));
        menu.add-action(clear-action);
    }

    menu.exec(event.globalPos());
    */
}

void
delete_pressed(void)
{
    debug_message("View delete-pressed()");
    set_override_cursor("WaitCursor");
    /*
    mdi-win = mdi_area.active_sub_window();
    if (mdi-win) {
        mdi-win.delete_pressed();
    }
    */
    restore_override_cursor();
    if (get_int(mainwnd->state, "pasting-active")) {
        /*
        gscene.remove-item(paste-object-item-group);
        del paste-object-item-group;
        */
    }

    set_int(mainwnd->state, "pasting-active", 0);
    set_int(mainwnd->state, "zoom-window-active", 0);
    set_int(mainwnd->state, "selecting-active", 0);
    /*
    select-box.hide();
    stop-gripping(0);
    deleteSelected();
    */
}

void
escape_pressed(void)
{
    debug_message("View escape-pressed()");
    if (get_int(mainwnd->state, "pasting-active")) {
        /*
        gscene.remove-item(paste-object-item-group)
        del paste-object-item-group
        */
    }

    set_int(mainwnd->state, "pasting-active", 0);
    set_int(mainwnd->state, "zoom-window-active", 0);
    set_int(mainwnd->state, "selecting-active", 0);
    /*
    select-box.hide()
    if (get_int(mainwnd->state, "gripping-active")) {
        stop_gripping(0);
    }
    else {
        clear_selection();
    }
    */
    set_override_cursor("WaitCursor");
    /*
    mdi_win = mdi_area.active-sub-window();
    if (mdi_win) {
        mdi_win.escape_pressed();
    }
    */
    restore_override_cursor();

    int gview = get_int(mainwnd->state, "active-view");
    if (gview) {
        /*
        gview.clearRubberRoom();
        gview.previewOff();
        gview.disableMoveRapidFire();
        */
    }
}

/*
PropertyBox arc-tab = {
    "Arc Properties",
    OBJ-TYPE-ARC,
    arc-properties
};
*/

