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
 *  Use Python's PEP7 style guide.
 *      https://peps.python.org/pep-0007/
 */

#define STB_TRUETYPE_IMPLEMENTATION 1
#define STB_IMAGE_WRITE_IMPLEMENTATION 1
#include "em2.h"

int
parse_command(int argc, char *argv[])
{
    int i;
    for (i=1; i<argc; i++) {
        if (!strcmp(argv[i], "--debug") || !strcmp(argv[i], "-d")) {
            debug_mode = 1;
            printf("DEBUG MODE\n");
        }
        if (!strcmp(argv[i], "--help") || !strcmp(argv[i], "-h")) {
            puts(help_message_em2);
			return 0;
        }
        if (!strcmp(argv[i], "--version") || !strcmp(argv[i], "-v")) {
            /* For scripts that need the version string */
            puts(VERSION);
            return 0;
        }
        if (!strcmp(argv[i], "--test")) {
            testing = 1;
        }
    }
	return 1;
}

void
open_tabs(void)
{
    /*
    if (len(filesToOpen) > 0) {
        main_win.open_files_selected(argv, argc);
    }

    if (argc > 10) {
        argc = 10;
    }
    for (file=1; file<argc; file++) {
        pattern[file-1] = embPattern_create();
        embPattern_readAuto(pattern[file-1], argv[file]);
    }*/
}

/* Function definitions */
#if _WIN32
int WINAPI WinMain(
    HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR pCmdLine, int nCmdShow)
{
    const wchar_t CLASS_NAME[] = L"embroidermodder window";

    puts(boot_message_em2);

	WNDCLASS wc = { };

	wc.lpfnWndProc   = WindowProc;
	wc.hInstance     = hInstance;
	wc.lpszClassName = CLASS_NAME;

    Rect rect = make_rectangle(100, 100, 640, 480);
    create_window(rect, main_window_title);
	if (!windows[MAIN_WINDOW]) {
		return 0;
    }

	RegisterClass(&wc);

    windows[MAIN_WINDOW]->hwnd = CreateWindowEx(
        0, CLASS_NAME, L"Embroidermodder " VERSION, WS_OVERLAPPEDWINDOW,
        CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT,
        NULL, NULL, hInstance, NULL);

    if (windows[MAIN_WINDOW]->hwnd == NULL) {
        return 0;
    }

    ShowWindow(windows[MAIN_WINDOW]->hwnd, nCmdShow);

    load_state();

    MSG msg = { };
    while (GetMessage(&msg, NULL, 0, 0) > 0) {
        TranslateMessage(&msg);
        DispatchMessage(&msg);
    }

    return 0;
}

LRESULT CALLBACK WindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
    switch (uMsg) {
    case WM_KEYDOWN:
        break;

    case WM_DESTROY:
        PostQuitMessage(0);
	    break;

    case WM_PAINT: {
		int i;
		PAINTSTRUCT ps;
		HDC hdc = BeginPaint(hwnd, &ps);
		FillRect(hdc, &ps.rcPaint, (HBRUSH) (COLOR_WINDOW+1));

		for (i=0; i<windows[MAIN_WINDOW]->n_widgets; i++) {
			if (windows[MAIN_WINDOW]->widgets[i].visibility == ALWAYS_VISIBLE
			   || windows[MAIN_WINDOW]->widgets[i].visibility == menu_state) {
				if (windows[MAIN_WINDOW]->widgets[i].mode == WIDGET_MODE_BLOCK
				 || windows[MAIN_WINDOW]->widgets[i].mode == WIDGET_MODE_TEXT) {
					HBRUSH color = CreateSolidBrush(RGB(0, 1, 200));
					/* MoveTo(hdc,
						windows[MAIN_WINDOW]->widgets[i].rect.x,
						windows[MAIN_WINDOW]->widgets[i].rect.y);
					LineTo(hdc,
						windows[MAIN_WINDOW]->widgets[i].rect.x + windows[MAIN_WINDOW]->widgets[i].rect.w,
						windows[MAIN_WINDOW]->widgets[i].rect.y + windows[MAIN_WINDOW]->widgets[i].rect.h);  */
					RECT r;
					r.top = windows[MAIN_WINDOW]->widgets[i].rect.x;
					r.bottom = windows[MAIN_WINDOW]->widgets[i].rect.x + windows[MAIN_WINDOW]->widgets[i].rect.y;
					r.left = windows[MAIN_WINDOW]->widgets[i].rect.w;
					r.right = windows[MAIN_WINDOW]->widgets[i].rect.y + windows[MAIN_WINDOW]->widgets[i].rect.h;
					FillRect(hdc, &r, color);
				}
				if (windows[MAIN_WINDOW]->widgets[i].mode == WIDGET_MODE_BACKGROUND) {
					
				}
			}
		}
		EndPaint(hwnd, &ps);
        return 0;
        }

    default:
        break;
    }
    return DefWindowProc(hwnd, uMsg, wParam, lParam);
}
#else
int
main(int argc, char *argv[])
{
    puts(boot_message_em2);

    if (!parse_command(argc, argv)) {
		return 0;
	}

#if x11_version
#elif ANDROID_VERSION
#elif IOS_VERSION
#else
    if (SDL_Init(SDL_INIT_VIDEO) < 0) {
        return 0;
    }
#endif

    Rect rect = make_rectangle(100, 100, 640, 480);

    load_state();

    create_window(rect, main_window_title);

#if x11_version
    XEvent event;
#endif
    while (running) {
        process_input(windows[MAIN_WINDOW]);
        render(windows[MAIN_WINDOW]);
        wait(50);
    }

    destroy_window(windows[MAIN_WINDOW]);
    
#if x11_version
    XCloseDisplay(windows[MAIN_WINDOW]->display);
#elif ANDROID_VERSION
#elif IOS_VERSION
#elif __APPLE__
#endif
    return 0;
}
#endif

int
click_detection(EmbWidget *w, int x, int y)
{
    return (w->rect.x < x)
        && (x < w->rect.x + w->rect.w)
        && (w->rect.y < y)
        && (y < w->rect.y + w->rect.h);
}

Rect
make_rectangle(int x, int y, int w, int h)
{
    Rect rect;
    rect.x = x;
    rect.y = y;
    rect.w = w;
    rect.h = h;
    return rect;
}

int
get_widget_by_label(int window_id, char *label)
{
    int i;
    for (i=0; i<windows[window_id]->n_widgets; i++) {
        if (!strcmp(windows[window_id]->widgets[i].label, label)) {
            return i;
        }
    }
    return -1;
}

void
set_visibility(int window_id, char *label, int visibility)
{
    int i;
    for (i=0; i<windows[window_id]->n_widgets; i++) {
        if (!strcmp(windows[window_id]->widgets[i].label, label)) {
            windows[window_id]->widgets[i].visibility = visibility;
        }
    }
}


/*
visibility groups
"always"
"file-toolbar"
"edit-toolbar"
"window-toolbar"

"never" (use only for debugging)

ui palette
interface-background
button-hover
*/

/* -----------------------------
 * Crash test all basic actions.
 */
void
crash_test(void)
{
    run_script(crash_test_script);
}

void
run_script(TABLE(script))
{
    int i;
    for (i=0; i<MAX_CSV_ROWS; i++) {
        if (strcmp(script[i][0], "END")) {
            break;
        }
        actuator(script[i][0]);
    }
}

/* Data sanitisation could be a seperate function?
 *
 * This file needs to be read from the users home
 * directory to ensure it is writable.
 *
    pos = Vector(window-x, window-y)
    size = (window-width, window-height)

    layoutState = settings-file.value("LayoutState").toByteArray();
    if (!restoreState(layoutState)) {
        debug_message("LayoutState NOT restored! Setting Default Layout...");
        some_toolBar.visible = 1;
    }

    settings = load-data("config.json");

    Sanitise data here

    window-x = clamp(0, window-x, 1000);
    window-y = clamp(0, window-y, 1000);

    move(pos);
    resize(size);
 */
void
read_settings(char *fname)
{
    debug_message("Reading settings...");

    load_csv(settings_state, fname);

    main_window_dimensions.w = get_int(settings_state, "main_window_width");
    main_window_dimensions.h = get_int(settings_state, "main_window_height");
}

/* Write the current settings to the standard file CSV.
 *
 * The idea is that these override the settings in config.scm
 * so only what has changed needs to be written.
 */
void
write_settings(char *fname)
{
    int i;
    debug_message("Writing settings...");
    FILE *f;
    f = fopen(fname, "w");
    if (!f) {
        puts("Failed to write settings: could not open file for writing.");
    }

    set_int(settings_state, "main_window_x", main_window_dimensions.x);
    set_int(settings_state, "main_window_y", main_window_dimensions.y);
    set_int(settings_state, "main_window_width", main_window_dimensions.w);
    set_int(settings_state, "main_window_height", main_window_dimensions.h);

    for (i=0; i<MAX_CSV_ROWS; i++) {
        fprintf(f, "%s=%s\r\n",
            settings_state[i][0], settings_state[i][1]);
    }

    fclose(f);
    /* 
    settings["window_x"] = pos().x();
    settings["window_y"] = pos().y();
    settings["window_width"] = size().width();
    settings["window_height"] = size().height();
    */
}

/* .
 */
int
find_mdi_window(char *file_name)
{
    debug_message("find_mdi_window(fileName)");
    printf("%s\n", file_name);
    /*
    char *canonical_path;
    canonical_path = canonical_file_path(file_name);

    for (subWindow in mdi_area.sub_window_list(void)) {
        if (subWindow.getCurrentFile() == canonicalFilePath) {
            return subWindow;
        }
    }
    */
    return 0;
}


/*
 *  About Dialog
 */
EmbWindow *
about_dialog_init(void)
{
    create_window(make_rectangle(0, 0, 100, 100), "About Embroidermodder");
}

/* The dialog showing details of the pattern including histograms.
 */
void
details_dialog_free(void)
{
    restore_override_cursor();
}

void
create_int_label(int w, int x, int y, int value)
{
    char s[MAX_STRING_LENGTH];
    sprintf(s, "%d", value);
    create_label(w, x, y, s, "do-nothing", ALWAYS_VISIBLE);
}

void
create_measurement_label(int w, int x, int y, int spacing, char *label, float value)
{
    char s[MAX_STRING_LENGTH];
    create_label(w, x, y, label, "do-nothing", ALWAYS_VISIBLE);
    sprintf(s, "%f mm", value);
    create_label(w, x, y+spacing, s, "do-nothing", ALWAYS_VISIBLE);
}

void
create_labelled_int(int w, int x, int y, int spacing, char *label, int value)
{
    create_label(w, x, y, label, "do-nothing", ALWAYS_VISIBLE);
    create_int_label(w, x, y+spacing, value);
}

/* Creates a dialog showing key information about the pattern,
 * ideally this will update as the pattern changes without
 * any key presses or clicks.
 */
int
details_dialog_init(void)
{
    /* Dialog set minimum size. */
    int stitches_real = 0;
    int stitches_jump = 0;
    int stitches_trim = 0;
    int color_total = 0;
    int color_changes = 0;
    EmbRect bounding_rect = embRect_create();
    char *title = translate("Embroidery Design Details");
    Rect rect = {0, 0, 600, 400};
    create_window(rect, title);
    int right_column_offset = 100;
    int spacing = 200;

    /*
    button-box = tk.ButtonBox(dialog, text="QDialogButtonBox-Ok");
    connect(buttonBox, SIGNAL(accepted()), SLOT(accept()));
    vbox_layout_main = tk.VBoxLayout();
    vbox_layout_main, add_widget(main-widget);
    vbox_layout_main, add_widget(button-box);
    dialog-set_layout(vbox_layout_main);

    mw.setOverrideCursor("ArrowCursor");

    stitches_real = embStitchList_count(pattern.stitchList, NORMAL);
    stitches_jump = embStitchList_count(pattern.stitchList, JUMP);
    stitches_trim = embStitchList_count(pattern.stitchList, TRIM);
    color_total = embThreadList_count(pattern.threadList, TOTAL);
    color_changes = 0 = embThreadList_count(pattern.threadList, CHANGES);
    bounding_rect.set_rect(0, 0, 50, 100);
    TODO: embPattern-calcBoundingBox(pattern); */

    create_labelled_int(DETAILS_WINDOW, right_column_offset, 0*20, spacing,
        "Total Stitches:", windows[MAIN_WINDOW]->tabs[tab_index].pattern->stitchList->count);
    create_labelled_int(DETAILS_WINDOW, right_column_offset, 1*20, spacing,
        "Real Stitches:", stitches_real);
    create_labelled_int(DETAILS_WINDOW, right_column_offset, 2*20, spacing,
        "Jump Stitches:", stitches_jump);
    create_labelled_int(DETAILS_WINDOW, right_column_offset, 3*20, spacing,
        "Trim Stitches:", stitches_trim);
    create_labelled_int(DETAILS_WINDOW, right_column_offset, 4*20, spacing,
        "Total Colors:", color_total);
    create_labelled_int(DETAILS_WINDOW, right_column_offset, 5*20, spacing,
        "Color Changes:", color_changes);

    create_measurement_label(DETAILS_WINDOW, right_column_offset, 6*20, spacing,
        "Left:", bounding_rect.left);
    create_measurement_label(DETAILS_WINDOW, right_column_offset, 7*20, spacing,
        "Top:", bounding_rect.top);
    create_measurement_label(DETAILS_WINDOW, right_column_offset, 8*20, spacing,
        "Right:", bounding_rect.right);
    create_measurement_label(DETAILS_WINDOW, right_column_offset, 9*20, spacing,
        "Bottom:", bounding_rect.bottom);
    create_measurement_label(DETAILS_WINDOW, right_column_offset, 10*20, spacing,
        "Width:", fabs(bounding_rect.left - bounding_rect.right));
    create_measurement_label(DETAILS_WINDOW, right_column_offset, 11*20, spacing,
        "Height:", fabs(bounding_rect.top - bounding_rect.bottom));

    
    /* TODO: Color Histogram
     * Stitch Distribution
     * groupboxDist = Qgroupbox(translate("Stitch Distribution"), widget)
     *
     * TODO: Stitch Distribution Histogram */

    /* Widget Layout
    vbox_layout_main = tk.VBoxLayout(widget);
    vbox_layout_main, add_widget(groupbox-misc);
    vbox_layout_main, add_widget(groupboxDist);
    vbox_layout_main.addStretch(1);
    widget.set_layout(vbox_layout_main);

    scroll_area = tk.scroll_area();
    scroll_area.set_widget-resizable(1);
    scroll_area.set_widget(widget);
    return scroll_area;
    */
}

void
create_histogram(void)
{
    debug_message("TODO: createHistogram");
}

/*
 * Settings Dialog.
 * 
 * class settings-dialog-action()
 *   tab-widget* tab-widget
 *   dialogButtonBox* buttonBox
 *
 * showTab is the tab index, use the TAB-GENERAL style defines.
 */
void
settings_dialog_init(int showTab)
{
    printf("showTab: %d\n", showTab);
    /* mw = mw;
    accept = copy();
    dialog = copy();
    preview = copy();

    window = tk.Tk();
    window.size(750,550);
    tab_widget = tk.tab-widget(window);
    */

    /* TODO: Add icons to tabs */
    /*
    create_window_tab(tab_widget, "assets/ui/general_tab.csv", translate("General"));
    create_window_tab(tab_widget, "assets/ui/files_paths_tab.csv", translate("Files/Paths"));
    create_window_tab(tab_widget, "assets/ui/display_tab.csv", translate("Display"));
    create_window_tab(tab_widget, "assets/ui/open_save_tab.csv", translate("Open/Save"));
    create_window_tab(tab_widget, "assets/ui/printing_tab.csv", translate("Printing"));
    create_window_tab(tab_widget, "assets/ui/snap_tab.csv", translate("Snap"));
    create_window_tab(tab_widget, "assets/ui/grid_ruler_tab.csv", translate("Grid/Ruler"));
    create_window_tab(tab_widget, "assets/ui/ortho_polar_tab.csv", translate("Ortho/Polar"));
    create_window_tab(tab_widget, "assets/ui/quicksnap_tab.csv", translate("QuickSnap"));
    create_window_tab(tab_widget, "assets/ui/quicktrack_tab.csv", translate("QuickTrack"));
    create_window_tab(tab_widget, "assets/ui/lineweight_tab.csv", translate("LineWeight"));
    create_window_tab(tab_widget, "assets/ui/selection_tab.csv", translate("Selection"));

    for (i=0; i<12; i++) {
        if (showTab == settings_tab_label[i]) {
            tab-widget.set_current_index(i);
        }
    }

    buttonBox = tk.DialogButtonBox(QDialogButtonBox-Ok | tk.DialogButtonBox-Cancel);

    connect(buttonBox, SIGNAL(accepted()), this, SLOT(acceptChanges()));
    connect(buttonBox, SIGNAL(rejected()), this, SLOT(rejectChanges()));

    vbox_layout_main = tk.VBoxLayout();
    vbox_layout_main, add_widget(tab-widget);
    vbox_layout_main, add_widget(buttonBox);
    setLayout(vbox_layout_main);

    window.title(translate("Settings"));
    window.setOverrideCursor("ArrowCursor");
    */
}

void
load_widget(EmbTab *settings_tab_display, char *fname)
{

}

/* Genericised tab creation: uses "assets/ui/tab_name.csv" as the starting point.
 * it then chain loads the data it needs using load_widget(tab, fname);
 */
int
create_window_tab(int window_id, char *fname)
{
    TABLE(tab_data);
    load_csv(tab_data, fname);
    
    return 0;
}

/* Originally a macro for constructing checkboxes.
 */
void
make_checkbox(char *label, int checked, char *icon, int f, int x, int y)
{
    printf("%s %d %s %d %d %d",
        label, checked, icon, f, x, y);
    /*
    c = tk.checkbox(translate(label), groupboxQSnapLoc);
    c.set_checked(checked);
    c.set_icon(load-icon(icon));
    connect(c, SIGNAL(-state_changed(int)), this, SLOT(f(int)));
    connect(this, SIGNAL(buttonQSnapSelectAll(int)), c, SLOT(set_checked(int)));
    connect(this, SIGNAL(buttonQSnapClearAll(int)), c, SLOT(set_checked(int)));
    gridLayoutQSnap, add_widget(c, x, y, ALIGN_LEFT);
    dialog-checked = checked;
    */
}

/* 
 *  Editor functions.
 */
void
load_file_action(char *file_name)
{
    debug_message("MdiWindow load-file()");
    printf("%s\n", file_name);
    /*
    FILE *file;
    EmbPattern *p;
    tmpColor = get-current-color()

    file = open(file_name, "r")
    if (!file) {
        warning(translate("Error reading file"),
                translate("Cannot read file %1:\n%2.")
                .arg(file_name).arg(file.errorString()))
        return 0
    }

    mw.set_override_cursor("WaitCursor");

    ext = fileExtension(file_name);
    debug_message("ext: %s", qPrintable(ext));

    p = embPattern-create();
    if (!p) {
        printf("Could not allocate memory for embroidery pattern\n");
        exit(1);
    }

    if (!p.readAuto(file_name)) {
        printf("Reading file was unsuccessful: %s\n", file_name);
        restore_override_cursor();
        message = translate("Reading file was unsuccessful: ") + file_name
        warning(this, translate("Error reading pattern"), message)
    }
    else {
        p.move-stitch-list-to-polylines()
        */ /* TODO: Test more
        stitchCount = p.stitch-list.count
        path = Path()

        if (p.circles) {
            for i in range(len(p.circles))
                c = p.circles.circle[i].circle
                this-color = p.circles.circle[i].color
                set_current-color(this_color)
                */ /* NOTE: With natives, the Y+ is up and libembroidery Y+ is up, so inverting the Y is NOT needed.
                mw.nativeAddCircle(c.center.x, c.center.y, c.radius, 0, "RUBBER-OFF")
                */ /* TODO: fill

        if p.ellipses:
            for i in range(len(p.ellipses))
                e = p.ellipses.ellipse[i].ellipse
                this-color = p.ellipses.ellipse[i].color
                set_current-color(this_color)
                */ /* NOTE: With natives, the Y+ is up and libembroidery Y+ is up, so inverting the Y is NOT needed.
                mw.nativeAddEllipse(e.centerX, e.centerY, e.radiusX, e.radiusY, 0, 0, OBJ-RUBBER-OFF)
                */ /* TODO: rotation and fill

        if p.lines:
            for i in range(len(p.lines))
                li = p.lines.line[i].line
                this-color = p.lines.line[i].color
                set_current-color(this_color)
                */ /* NOTE: With natives, the Y+ is up and libembroidery Y+ is up, so inverting the Y is NOT needed.
                mw.nativeAddLine(li.start.x, li.start.y, li.end.x, li.end.y, 0, OBJ-RUBBER-OFF)
                */ /* TODO: rotation

        if p.paths:
            */ /* TODO: This is unfinished. It needs more work
            for i in range(p.paths.count)
                curpoint-list = p.paths.path[i].point-list
                pathPath = Path()
                this-color = p.paths.path[i].color
                if curpoint-list.count > 0:
                    pp = curpoint-list[0].point.point
                    pathPath.move-to(pp.x, -pp.y)
                    */ /* NOTE: Qt Y+ is down and libembroidery Y+ is up, so inverting the Y is needed.

                for j in range(curpoint-list.count)
                    pp = curpoint-list[j].point.point
                    pathPath.line-to(pp.x, -pp.y)
                    */ /* NOTE: Qt Y+ is down and libembroidery Y+ is up, so inverting the Y is needed.

                loadPen = Pen(this_color)
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
                set_current-color(this_color)
                */ /* NOTE: With natives, the Y+ is up and libembroidery Y+ is up, so inverting the Y is NOT needed.
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
                set_current-color(this_color)
                for j in range(curpoint-list.count)
                    pp = curpoint-list.point[j].point
                    x = pp.x
                    y = -pp.y
                    */ /* NOTE: Qt Y+ is down and libembroidery Y+ is up, so inverting the Y is needed.

                    if first-point:
                        polygonPath.lineTo(x,y)
                    else {
                        polygonPath.move-to(x,y)
                        firstPo= 1
                        startX = x
                        startY = y

                polygonPath.translate(-startX, -startY)
                mw.nativeAddPolygon(startX, startY, polygonPath, OBJ-RUBBER-OFF)

        */ /* NOTE: Polylines should only contain NORMAL stitches.
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
                set_current-color(this_color)
                for j in range(curpoint-list.count)
                    pp = curpoint-list.point[j].point
                    x = pp.x
                    y = -pp.y
                    */ /* NOTE: Qt Y+ is down and libembroidery Y+ is up, so inverting the Y is needed.
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
                set_current-color(this_color)
                */ /* NOTE: With natives, the Y+ is up and libembroidery Y+ is up, so inverting the Y is NOT needed.
                mw.nativeAddRectangle(Rect-x(r), Rect-y(r), Rect-width(r), Rect-height(r), 0, 0, OBJ-RUBBER-OFF)
                */ /* TODO: rotation and fill

        set_current-file(file_name)
        mw.statusbar.showMessage("File loaded.")
        stitches = ""
        stitches.setNum(stitchCount)

        if (grid-load-from-file) {
            */ /* TODO: Josh, provide me a hoop size and/or grid spacing from the pattern.
            debug_message(".");
        }

        restore_override_cursor();
    }

    p.free();

    */ /* Clear the undo stack so it is not possible to undo past this point.
    undo-history-length = 0;

    set_current-color(tmpColor);
    return 1;
    */
}

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


void
set_selected_items(int *itemList)
{
    printf("%d\n", itemList[0]);
    /*
    selectedItemList = itemList
    */ /* Hide all the groups initially, then decide which ones to show
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
     */
    /*
    if (numTypes > 1) {
        comboBoxSelected.addItem(translate("Varies") + " (" + "".setNum(numAll) + ")");
        connect(comboBoxSelected, SIGNAL(currentIndexChanged(int)), this, SLOT(showOneType(int)));
    }

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
     */
    /*
    clear-all-fields();

    for item in itemList:
        if (!item) {
            continue;
        }
        */

        /* TODO: load data into the General field */
        /*
        if (item.type == "Arc") {
            p = obj.objectCenter();
            update_edit_num_if_varies("arc-center-x", p.x, 0);
            update_edit_num_if_varies("arc-center-y", -p.y, 0);
            update_edit_num_if_varies("arc-radius", obj.objectRadius(), 0);
            update_edit_num_if_varies("arc-start-angle", obj.objectStartAngle(), 1);
            update_edit_num_if_varies("arc-end-angle", obj.objectEndAngle(), 1);
            update_edit_num_if_varies("arc-start-x", obj.objectStartPoint().x, 0);
            update_edit_num_if_varies("arc-start-y", -obj.objectStartPoint().y, 0);
            update_edit_num_if_varies("arc-end-x", obj.objectEndPoint().x, 0);
            update_edit_num_if_varies("arc-end-y", -obj.objectEndPoint().y, 0);
            update_edit_num_if_varies("arc-area", obj.objectArea(), 0);
            update_edit_num_if_varies("arc-length", obj.objectArcLength(), 0);
            update_edit_num_if_varies("arc-chord", obj.objectChord(), 0);
            update_edit_num_if_varies("arc-inc-angle", obj.objectIncludedAngle(), 1);
            updateComboBoxintIfVaries("arc-clockwise", obj.objectClockwise(), 1); */ /* dropdown */
            /*
        }

        if (item.type == "Block") {
            debug_message("TODO: load block data");
        }

        if (item.type == "Circle") {
            p = obj.objectCenter()
            update_edit_num_if_varies(CIRCLE-CENTER-X], p.x, 0);
            update_edit_num_if_varies(CIRCLE-CENTER-Y], -p.y, 0);
            update_edit_num_if_varies(CIRCLE-RADIUS], obj.objectRadius(), 0);
            update_edit_num_if_varies(CIRCLE-DIAMETER], obj.objectDiameter(), 0);
            update_edit_num_if_varies(CIRCLE-AREA], obj.objectArea(), 0);
            update_edit_num_if_varies(CIRCLE-CIRCUMFERENCE], obj.objectCircumference(), 0);
        }

        if (item.type == "DimAligned") {
            debug_message("TODO: load aligned dimension data");
        }

        if (item.type == "DimAngular") {
            debug_message("TODO: load angular dimension data");
        }

        if (item.type == "DimArcLength") {
            debug_message("TODO: load arclength dimension data");
        }

        if (item.type == DIMDIAMETER) {
            debug_message("TODO: load diameter dimension data");
        }

        if (item.type == DIMLEADER) {
            debug_message("TODO: load leader dimension data");
        }

        if (item.type == DIMLINEAR) {
            debug_message("TODO: load linear dimension data");
        }

        if (item.type == DIMORDINATE) {
            debug_message("TODO: load ordinate dimension data");
        }

        if (item.type == "DimRadius") {
            debug_message("TODO: load radius dimension data");
        }

        if (item.type == "Ellipse") {
            p = obj.objectCenter()
            update_edit_num_if_varies("ellipse-center-x", p.x, 0);
            update_edit_num_if_varies("ellipse-center-y", -p.y, 0);
            update_edit_num_if_varies("ellipse-radius-major", obj.objectRadiusMajor(), 0);
            update_edit_num_if_varies("ellipse-radius-minor", obj.objectRadiusMinor(), 0);
            update_edit_num_if_varies("ellipse-diameter-major", obj.objectDiameterMajor(), 0);
            update_edit_num_if_varies("ellipse-diameter-minor", obj.objectDiameterMinor(), 0);
        }

        if (item.type == "Image") {
            debug_message("TODO: load image data");
        }

        if (item.type == "Infinite Line") {
            debug_message("TODO: load infinite line data");
        }

        if (item.type == "Line") {
            update_edit_num_if_varies(LINE-START-X], obj.objectEndPoint1().x, 0);
            update_edit_num_if_varies(LINE-START-Y], -obj.objectEndPoint1().y, 0);
            update_edit_num_if_varies(LINE-END-X], obj.objectEndPoint2().x, 0);
            update_edit_num_if_varies(LINE-END-Y], -obj.objectEndPoint2().y, 0);
            update_edit_num_if_varies(LINE-DELTA-X], obj.objectDeltaX(), 0);
            update_edit_num_if_varies(LINE-DELTA-Y], -obj.objectDeltaY(), 0);
            update_edit_num_if_varies(LINE-ANGLE], obj.objectAngle(), 1);
            update_edit_num_if_varies(LINE-LENGTH], obj.objectLength(), 0);
        }

        if (item.type == "Path") {
            debug_message(".");
        }

        if (item.type == "Point") {
            update_edit_num_if_varies(POINT-X], obj.objectX(), 0);
            update_edit_num_if_varies(POINT-Y], -obj.objectY(), 0);
        }

        if (item.type == "Polygon") {
            debug_message(".");
        }

        if (item.type == "Polyline") {
            debug_message(".");
        }

        if (item.type == "Ray") {
            debug_message(".");
        }

        if (item.type == RECTANGLE) {
            corn1 = obj.objectTopLeft();
            corn2 = obj.objectTopRight();
            corn3 = obj.objectBottomLeft();
            corn4 = obj.objectBottomRight();

            update_edit_num_if_varies("rect-corner-x1", corn1.x, 0);
            update_edit_num_if_varies("rect-corner-y1", -corn1.y, 0);
            update_edit_num_if_varies("rect-corner-x2", corn2.x, 0);
            update_edit_num_if_varies("rect-corner-y2", -corn2.y, 0);
            update_edit_num_if_varies("rect-corner-x3", corn3.x, 0);
            update_edit_num_if_varies("rect-corner-y3", -corn3.y, 0);
            update_edit_num_if_varies("rect-corner-x4", corn4.x, 0);
            update_edit_num_if_varies("rect-corner-y4", -corn4.y, 0);
            update_edit_num_if_varies("rect-width", obj.objectWidth(), 0);
            update_edit_num_if_varies("rect-height", -obj.objectHeight(), 0);
            update_edit_num_if_varies("rect-area", obj.objectArea(), 0);
        }

        if (item.type == TEXTMULTI) {
            debug_message(".");
        }

        if (item.type == TEXTSINGLE) {
            update-edit-StrIfVaries(edit-TextSingleContents, obj.objText);
            updatefont-comboboxStrIfVaries(comboBoxTextSingleFont, obj.objTextFont);
            updateComboBoxStrIfVaries(comboBoxTextSingleJustify, obj.objTextJustify, obj.objectTextJustifyList());
            update_edit_num_if_varies(edit-TextSingleHeight, obj.obj-text.size, 0);
            update_edit_num_if_varies(edit-TextSingleRotation, -obj.rotation(), 1);
            update_edit_num_if_varies(edit-TextSingleX, obj.objectX(), 0);
            update_edit_num_if_varies(edit-TextSingleY, -obj.objectY(), 0);
            updateComboBoxintIfVaries(comboBoxTextSingleBackward, obj.obj-text.backward, 1);
            updateComboBoxintIfVaries(comboBoxTextSingleUpsideDown, obj.obj-text.upsidedown, 1);
        }
    }
    */

    /* Only show fields if all objects are the same type
     * =================================================
     */
    /*
    if (numTypes == 1) {
        for (obj-type in typeSet) {
            showGroups(obj-type);
        }
    }
    */
}

void
export_(void)
{
    debug_message("Export.");
}

/* Creates a new file using the standard dialog_
 * Adds an mdi_area.
 */
void
new_file(void)
{
    debug_message("New File.");
    EmbWindow *w = windows[MAIN_WINDOW];
    w->tabs[n_docs].pattern = embPattern_create();
    w->tab_index = w->n_docs-1;
    w->n_docs++;
    /*
    mdi_win = mdi_window(doc_index, mdi_area, "SubWindow"); */
    /* connect(mdi_win, SIGNAL(sendClosemdi_win()), self, SLOT(on_close_mdi_win()));
    connect(mdi_area, SIGNAL(subWindowActivated()), self, SLOT(on_window_activated())); */ /*

    update_menu_toolbar_statusbar();
    window_menu_about_to_show();

    v = mdi_win.get_view();
    if (v) {
        v.recalculateLimits();
        v.zoom_extents();
    }
    */
}

/* Opens a file using the standard dialog.
 */
void
open_file(void)
/*, char *recent[], int recentFiles*/
{
    debug_message("Open File");
    set_override_cursor("ArrowCursor");
    /*
    nfdchar_t *outpath = NULL;
    nfdresult_t result = NFD_OpenDialog("c;md", NULL, &outpath);
    if (result == NFD_CANCEL) {
        return;
    }
    puts((char*)outpath);
    embPattern_readAuto(pattern[n_patterns], outpath);
    n_patterns++;
    free(outpath);
    files = []
    preview = opensave_open_thumbnail
    open_filesPath = opensave_recent_directory

    */ /* Check to see if self from the recent files list
    if (recent) {
        files.append(recentFile);
        open_files_selected(files);
    }
    elif (!preview) { */
        /* TODO: set get_open_fname's selectedFilter parameter from opensave_open_format */ /*
        files = FileDialog_get_open_fnames(translate("Open"), open_filesPath, format_filter_open);
        open_files_selected(files);
    elif preview:
        openDialog = PreviewDialog(translate("Open w/Preview"), open_filesPath, format_filter_open);
        */ /* TODO: set opendialog_selectNameFilter(const String& filter) from opensave_open_format
        */ /* connect(openDialog, SIGNAL(filesSelected()), self, SLOT(open_files_selected()));
        opendialog_exec();
    } */

    restore_override_cursor();
}

/* .
 */
void
open_files_selected(char **files, int n_files)
{
    int doOnce = 1;

    if (n_files) {
        int i;
        for (i=0; i<n_files; i++) {
            int existing = 0;
            if (!valid_file_format(files[i])) {
                continue;
            }

            /* existing = find_mdi_window(files[i]); */
            if (existing) {
                /* mdi_area.setactive_sub_window(existing); */
                continue;
            }

            /* The docIndex doesn't need increased as it
             * is only used for unnamed files. */            
            n_docs++;

            /* mdi_win = mdi_window(doc_index, self, mdi_area, "SubWindow"); */
            /* connect(mdi_win, SIGNAL(sendClosemdi_win()), self, SLOT(onClosemdi_win())); */
            /* connect(mdi_area, SIGNAL(subWindowActivated()), self, SLOT(onWindowActivated())); */

            /* Make sure the toolbars/etc... are shown before doing their zoom_extents */
            if (doOnce) {
                /* update_menu_toolbar_statusbar(); */
                doOnce = 0;
            }

            /*
            if (mdi_win.load_file(files[i])) {
                statusbar.show_message(translate("File(s) loaded"), 2000);
                mdi_win.show();
                mdi_win.showMaximized(); */
                /* Prevent duplicate entries in the recent files list. */ /*
                if (!opensave_recent_list_of_files.contains(filesToOpen[i], CaseInsensitive) {
                    opensave_recent_list_of_files.prepend(filesToOpen[i]);*/

                /* Move the recent file to the top of the list. */ /*
                else {
                    opensave_recent_list_of_files.removeAll(filesToOpen[i]);
                    opensave_recent_list_of_files.prepend(filesToOpen[i]);
                }

                opensave_recent_directory = FileInfo(absolute_path(files[i]));

                v = mdi_win.getView();
                if (v) {
                    v.recalculateLimits();
                    v.zoom_extents();
                }
            }
            else {
                mdi_win.close();
            }
            */
        }
    }
    /* window_menu_about_to_show(); */
}

/* .
 */
void
open_recent_file(void)
{
    debug_message("open_recent_file()");
    /* Check to see if this from the recent files list.
    recent_sender = sender();
    if (recent_sender) {
        open_file(1, str(recent_sender.data()));
    }
    */
}


/* Saves an existing file as a new format using the standard dialog_ */
void
save_file_as(void)
{
    debug_message("Save file as...");
    /* Need to find the active_sub_window before it loses
     * focus to the FileDialog */
    /* mdi_win = active_sub_window;
    if (!mdi_win) {
        return;
    }

    open_filesPath = opensave_recent_directory;
    file = QFileDialog_getsave_fileName(translate("Save As"), open_filesPath, format_filter_save);

    mdi_win.save_file(file); */
}

/* Saves a file using the standard dialog.
 *
 * TODO:
 *  Before saving to a stitch only format, Embroidermodder needs
 *  to calculate the optimal path to minimize jump stitches. Also
 *  based upon which layer needs to be stitched first,
 *  the path to the next object needs to be hidden beneath fills
 *  that will come later. When finding the optimal path, we need
 *  to take into account the color of the thread, as we do not want
 *  to try to hide dark colored stitches beneath light colored fills.
 */
void
save_file(void)
{
    int format_type;
    debug_message("Save file()");
    /* debug_message(current_file_name); */

    format_type = emb_identify_format(current_file_name);
    if (format_type == EMBFORMAT_UNSUPPORTED) {
        debug_message("The format of the file is not supported.");
    }

    /*
    for (item in mw.activeScene().items("Qt_AscendingOrder")) {
        switch (item.type) {
        case OBJ_TYPE_ARC:
            addArc(pattern, item);
            debug_message(".");
            break;

        case OBJ_TYPE_BLOCK:
            addBlock(pattern, item)
            debug_message(".");
            break;

        case OBJ_TYPE_CIRCLE:
            if (formatType == EMBFORMAT_STITCHONLY) {
                path = item.objectSavePath();
                to_polyline(pattern, item.objectCenter(), path.simplified(), "0", item.objectColor(), "CONTINUOUS", "BYLAYER");
                */ /* TODO: proper layer/line_type/line_weight
                */ /* TODO: Improve precision, replace simplified
            }
            else {
                p = item.objectCenter();
                r = item.objectRadius();
                embPattern_add_circle_object_abs(pattern, p.x(), p.y(), r);
            }
            break;

        case OBJ_TYPE_DIMALIGNED:
            addDimAligned(pattern, item);
            debug_message(".");
            break;

        case OBJ_TYPE_DIMANGULAR:
            addDimAngular(pattern, item);
            debug_message(".");
            break;

        case OBJ_TYPE_DIMARCLENGTH:
            addDimArcLength(pattern, item);
            debug_message(".");
            break;

        case OBJ_TYPE_DIMDIAMETER:
            addDimDiameter(pattern, item);
            debug_message(".");
            break;

        case OBJ_TYPE_DIMLEADER:
            addDimLeader(pattern, item);
            debug_message(".");
            break;

        case OBJ_TYPE_DIMLINEAR":
            addDimLinear(pattern, item);
            debug_message(".");
            break;

        case OBJ_TYPE_DIMORDINATE:
            */ /* addDimOrdinate(pattern, item);
            debug_message(".");
            break;

        case OBJ_TYPE_DIMRADIUS":
            */ /* addDimRadius(pattern, item)
            debug_message(".");
            break;

        case OBJ_TYPE_ELLIPSE":
            if formatType == EMBFORMAT_STITCHONLY:
                path = item.objectSavePath()
                to_polyline(pattern, item.objectCenter(), path.simplified(), "0", item.objectColor(), "CONTINUOUS", "BYLAYER")
                */ /* TODO: proper layer/line_type/line_weight
                */ /* TODO: Improve precision, replace simplified
            else {
                */ /* TODO: ellipse rotation
                pattern.add_ellipse_abs(item.objectCenter().x(), item.objectCenter().y(), item.objectWidth()/2.0, item.objectHeight()/2.0)
            break;

        case OBJ_TYPE_ELLIPSEARC":
            add_ellipse_arc(pattern, item);
            debug_message(".");
            break;

        case OBJ_TYPE_GRID":
            add_grid(pattern, item);
            debug_message(".");
            break;

        case OBJ_TYPE_Hatch":
            add_hatch(pattern, item);
            debug_message(".");
            break;

        case OBJ_TYPE_Image":
            addImage(pattern, item);
            debug_message(".");
            break;

        case OBJ_TYPE_INFINITELINE":
            */ /* addInfiniteLine(pattern, item)
            debug_message(".")
            break;

        case OBJ_TYPE_Line":
            if formatType == EMBFORMAT_STITCHONLY:
                to_polyline(pattern, item.objectEndPoint1(), item.objectSavePath(), "0", item.objectColor(), "CONTINUOUS", "BYLAYER")
                */ /* TODO: proper layer/line_type/line_weight
            else {
                embPattern_addLineObjectAbs(pattern, item.objectX1(), item.objectY1(), item.objectX2(), item.objectY2())

        elif item.type == "Path":
            */ /* TODO: Reimplement addPolyline() using the libembroidery C API
            */ /* 
            debug_message("addPolyline()")
            polyline_item = item
            if polyline_item:
                path = polyline_item.path()
                pos = polyline_item.pos()
                startX = pos.x()
                startY = pos.y()

                element = []
                P1 = []
                P2 = []
                P3 = []
                P4 = []

                for i in range(path.element_count()-1)
                    element = path.element_at(i)
                    if element.is_move_to()
                        pattern.add_stitch_abs((element.x + startX), -(element.y + startY), TRIM)

                    elif element.is_line_to()
                        pattern.add_stitch_abs((element.x + startX), -(element.y + startY), NORMAL)

                    elif element.is_curve_to()
                        P1 = path.element_at(i-1) */ /* start point
                        P2 = path.element_at(i)   */ /* control point
                        P3 = path.element_at(i+1) */ /* control point
                        P4 = path.element_at(i+2) */ /* end point

                        pattern.add_stitch_abs(P4.x, -P4.y, NORMAL)
                        */ /* TODO: This is temporary
                        */ /* TODO: Curved Polyline segments are always arcs

                pattern.AddStitchRel(0, 0, STOP)
                c = polyline_item.pen().color()
                pattern.add_color(c.red, c.green, c.blue, "", "")
            break;

        case OBJ_TYPE_Point":
            if formatType == "EMBFORMAT_STITCHONLY":
                to_polyline(pattern, item.objectPos(), item.objectSavePath(), "0", item.objectColor(), "CONTINUOUS", "BYLAYER")
                */ /* TODO: proper layer/line_type/line_weight
            else {
                pattern.addPointObjectAbs(item.objectX(), item.objectY())
            break;

        case OBJ_TYPE_Polygon":
            to_polyline(pattern, item.objectPos(), item.objectSavePath(), "0", item.objectColor(), "CONTINUOUS", "BYLAYER")
            */ /* TODO: proper layer/line_type/line_weight
            break;

        case OBJ_TYPE_Polyline":
            to_polyline(pattern, item.objectPos(), item.objectSavePath(), "0", item.objectColor(), "CONTINUOUS", "BYLAYER")
            */ /* TODO: proper layer/line_type/line_weight
            break;

        case OBJ_TYPE_RAY:
            */ /* addRay(pattern, item)
            debug_message(".")
            break;

        case OBJ_TYPE_RECTANGLE:
            if formatType == "EMBFORMAT_STITCHONLY":
                to_polyline(pattern, item.objectPos(), item.objectSavePath(), "0", item.objectColor(), "CONTINUOUS", "BYLAYER")
                */ /* TODO: proper layer/line_type/line_weight
            else {
                */ /* TODO: Review this at some point
                top_left = item.object_top_left()
                pattern.add_rect_object_abs(top_left.x, top_left.y, item.object_width(), item.object_height())
            break;

        case OBJ_TYPE_SLOT:
            debug_message(".");
            break;

        case OBJ_TYPE_SPLINE:
            */ /* TODO: abstract bezier into geom-bezier... cubicBezierMagic(P1, P2, P3, P4, 0.0, 1.0, tPoints)
            debug_message(".");
            break;

        case OBJ_TYPE_TextMulti:
            */ /* TODO: saving polygons, polylines and paths must be stable before we go here.
            debug_message(".")
            break;

        case OBJ_TYPE_TextSingle:
            */ /* TODO: saving polygons, polylines and paths must be stable before we go here.

            */ /* TODO: This needs to work like a path, not a polyline. Improve this
            
            if (formatType == EMBFORMAT_STITCHONLY) {
                path_list = item.objectSavePathList()
                for (path in path_list) {
                    to_polyline(pattern, item.objectPos(), path.simplified(),
                        "0", item.objectColor(), "CONTINUOUS", "BYLAYER")
                    */ /* TODO: proper layer/line_type/line_weight
                    */ /* TODO: Improve precision, replace simplified
                }
            }
            break;
        default:
            break;
        }
    }

    */ /* TODO: handle EMBFORMAT_STCHANDOBJ also
    if (formatType == "EMBFORMAT_STITCHONLY") {
        pattern.move_polylines_to_stitch_list()
        */ /* TODO: handle all objects like this
    }

    write_successful = embPattern_writeAuto(file_name);
    if (!write_successful) {
        debug_message("Writing file %s was unsuccessful" % file_name);
    }
    */
}

/* . */
void
whats_this(void)
{
    debug_message("whats_this()");
    /*
    what_this = 1;
    QWhatsThis_enterWhatsThisMode();
    */
}

/* Call print dialog to print out a render of the embroidery.
 */
void
print_pattern(void)
{
    debug_message("print()");
    /*
    mdi_win = mdi_area.active_sub_window();
    if (mdi_win) {
        mdi_win.print();
    }
    dialog = tk.PrintDialog(printer, this);
    if (dialog_exec() == "QDialog_Accepted") {
        painter = QPainter(printer);
        if (printing_disable_bg) {
            */ /* Save current bg
            brush = gview.backgroundBrush();
            */ /* Save ink by not printing the bg at all
            gview.setBackgroundBrush(Qt_NoBrush);
            */ /* Print, fitting the viewport contents into a full page
            gview.render(painter);
            */ /* Restore the bg
            gview.setBackgroundBrush(brush);
        else {
            */ /* Print, fitting the viewport contents into a full page
            gview.render(painter);
        }
    } */
}


/*
 * Cut acts on objects selected before the action is called.
 *
 * If no objects are selected an error is returned.
 */
void
cut(void)
{
    debug_message("cut()");

    /* gview = active_view
    if gview) {
        gview.cut();
    } */

    /* if (len(gscene.selected_items()) == 0) {
        message = translate("Preselect objects before invoking the cut command.");
        information(self, translate("Cut Preselect"), message);
        return; */
        /*  TODO: Prompt to select objects if nothing is preselected. */
    /* }

    copy_selected();
    delete_selected(); */
}

/*
 * Copy acts on objects selected before the action is called.
 *
 * If no objects are selected an error is returned.
 */
void
copy(void)
{
    debug_message("copy()");
    /* gview = active_view
    if gview:
        gview.copy(); */

    /* if (len(gscene.selected_items()) == 0) {
        message = translate("Preselect objects before invoking the copy command.");
        information(self, translate("Copy Preselect"), message);
        return; */
        /*  TODO: Prompt to select objects if nothing is preselected. */
    /* }

    copy_selected(window);
    clear_selection(window); */
}

/* .
 */
void
copy_selected(void)
{
    /* selectedList = gscene.selected_items(); */

    /*  Prevent memory leaks by deleting any unpasted instances. */
    /* DeleteAll(cut_copy_object_list.begin(), cut_copy_object_list.end());
    cut_copy_object_list.clear(); */

    /*
    Create objects but do not add them to the scene just yet.
    By creating them now, ensures that pasting will still work
    if the original objects are deleted before the paste occurs.
    */
    /* cut_copy_object_list = create_object_list(selectedList); */
}

/* Paste whatever objects are on the clipboard.
 */
void
paste(void)
{
    debug_message("paste()");
    /* 
    gview = active_view;
    if (gview) {
        gview.paste();
    } */

    /*if (pasting_active) {
        gscene.remove_item(paste_object_item_group);
        del paste_object_item_group;
    }

    paste_object_item_group = gscene.create_item_group(cut_copy_object_list)
    pasteDelta = paste_object_item_group.bounding_rect().bottom_left()
    v = scene_mouse_point.subtract(pasteDelta)
    paste_object_item_group.set_pos(v)
    pasting_active = 1 */

    /*  Re-create the list in case of multiple pastes. */
    /* cut_copy_object_list = create_object_list(cut_copy_object_list); */
}

/*
 * Reapply the action that is next on the undo_history array,
 * if we are at the top of the array then do nothing.
 */
void
redo(void)
{
    debug_message("redo()");
    /*
    gview = active_view;
    if (gview) {
        gview->copy();
    }
    undo_call = "";
    if (undo_history_position < undo_history_length) {
        undo_history_position += 1;
        print("undo_history_position = %d\n", undo_history_position);
        print("undo_history_length = %d\n", undo_history_length);
        undo_call = undo_history[undo_history_position]
        */ /* set reverse flag
        undo_call += " -r"
        actuator(undo_call);
    }
    */
}

/*
 * Apply the reverse of the action on the current position
 * of the undo_history array.
 */
void
undo(void)
{
    debug_message("undo()");
    /*
    gview = active_view;
    if (gview) {
        gview->paste();
    }
    if (undo_history_position > 0) {
        last = undo_history[undo_history_position]
        undo_history_position -= 1
        print("undo_history_position = %d\n" % undo_history_position);
        print("undo_history_length = %d\n" % undo_history_length);

        */ /* Create the reverse action from the last action and apply with
        */ /* the main actuator.
        if last[0] == "donothing":
            debug_message("The last action has no undo candidate.");

        actuator(last);
    }
    */
}

/* Uses the undo stack.
 */
void
repeat(void)
{
    debug_message(" . ");
}

/* .
 */
void
design_details(void)
{
    /* "assets/ui/design_details_window.csv" */
}


/* .
 */
void
help(void)
{
    /* "assets/ui/help_window.csv" */
}

/*
 * Create and show the about dialog with a close button.
 *
 * Some layout from the previous version) {
 *
 * TODO: QTabWidget for about dialog
 * QApplication_setOverrideCursor(Qt_ArrowCursor)
 *
 * layout = v_box();
    layout.setAlignment(Qt_AlignCenter);
    layout.addWidget(img);
    layout.addWidget(text);
    layout.addWidget(buttonbox);

    dialog_setWindowTitle(title)
    dialog_setMinimumWidth(img.minimumWidth()+30)
    dialog_setMinimumHeight(img.minimumHeight()+50)
    dialog_setLayout(layout)
    restoreOverrideCursor()
 */
void
about(void)
{
    debug_message("about()");
    /* 
    dialog = tk.Tk();
    dialog_title("About Embroidermodder 2");
    dialog_minsize(400, 400);
    */ /* tk.dialog(_mainWin);
    */ /* img = tk.ImageWidget(application_folder + "/images/logo-small.png");
    text_block = (
        "Embroidermodder " + version + "\n\n" +
        translate("http://embroidermodder.org") +
        "\n\n" +
        translate(
            "Available Platforms: GNU/Linux, Windows, Mac OSX, Raspberry Pi"
        ) + "\n\n" +
        translate("Embroidery formats by Josh Varga.") +
        "\n" +
        translate("User Interface by Jonathan Greig and Robin Swift.") +
        "\n\n" +
        translate("Free under the zlib/libpng license.")
        + "\n\n" +
        translate("Build Hash: ") + BUILD_GIT_HASH
    );
    text = tk.Label(dialog, text=text_block);
    text.grid(row=1, column=1);
    */ /* text.setWordWrap(1);

    button = tk.Button(
        dialog,
        text="Oh, Yeah!",
        command=dialog_destroy);
    button.grid(row=2, column=1);

    dialog_mainloop();
    */
}

/*
 * Display the changelog stored in the config.json data file.
 *
 * Not currently maintained.
 */
void
changelog(void)
{
    debug_message("changelog()");
    create_window(make_rectangle(0, 0, 600, 400), translate("Changelog"));

    create_label(CHANGELOG_WINDOW, 0, 0, "changelog", "do-nothing", ALWAYS_VISIBLE);
    /* display in a custom widget instead
    
    QUrl changelogURL("help/changelog.html");
    QDesktopServices_openUrl(changelogURL);
    */
    
}

/* .
 */
void
settings_dialog(void)
{
    debug_message("settings_dialog()");
}

/* Run the "tip of the day" dialog box.
 */
void
tip_of_the_day(void)
{
    debug_message("tip_of_the_day()");
    /* 
    appDir = qApp.applicationDirPath();
    wizard_tip_of_the_day = tk.Dialog();
    button1 = tk.Button(wizard_tip_of_the_day);
    button2 = tk.Button(wizard_tip_of_the_day);
    button3 = tk.Button(wizard_tip_of_the_day);

    img_banner = ImageWidget(
        appDir + "/images/did-you-know.png", wizard_tip_of_the_day
    );

    checkBoxTipOfTheDay = tk.CheckBox(translate("&Show tips on startup"), wizard_tip_of_the_day);
    checkBoxTipOfTheDay.set_checked(general_tip_of_the_day); */
    /*  connect(checkBoxTipOfTheDay, SIGNAL(stateChanged(int)), self, SLOT(checkBoxTipOfTheDayStateChanged(int))); */ /*

    tips = tips
    if tips[general_current_tip] == "":
        general_current_tip = 0
    label_tip_of_the_day = tk.Label(tips[general_current_tip], wizard_tip_of_the_day);
    label_tip_of_the_day.setWordWrap(1);

    button1.setText("&Previous");
    button2.setText("&Next");
    button3.setText("&Close");
    connect(button1,
        SIGNAL(triggered()), wizard_tip_of_the_day,
        SLOT(wizard_tip_of_the_day.close()));
    connect(button2,
        SIGNAL(triggered()), wizard_tip_of_the_day,
        SLOT(wizard_tip_of_the_day.close()));
    connect(button3,
        SIGNAL(triggered()), wizard_tip_of_the_day,
        SLOT(wizard_tip_of_the_day.close()));

    layout = tk.VBoxLayout(wizard_tip_of_the_day);
    add_widget(layout, img_banner);
    add_strut(layout, 1);
    add_widget(layout, label_tip_of_the_day);
    layout.add_stretch(1);
    layout.add_widget(checkBoxTipOfTheDay);
    layout.add_strut(1);
    layout.add_widget(button1);
    layout.add_strut(1);
    layout.add_widget(button2);
    layout.add_strut(1);
    layout.add_widget(button3);

    wizard_tip_of_the_day.set_layout(layout);
    wizard_tip_of_the_day.set_window_title("Tip of the Day");
    wizard_tip_of_the_day.set_minimum_size(550, 400);
    wizard_tip_of_the_day.exec(); */
}

/* .
 */
void
check_for_updates(void)
{
    debug_message("check_for_updates()");
}

/* .
 */
void
select_all(void)
{
    debug_message("select_all()");
}

/* Open color selector dialog.
 */
void
color_selector(void)
{
    debug_message("color_selector()");
}

/* Change layer action.
 */
void
layer_selector_action(void)
{
    debug_message("color-selector-action()");
}

/* Open line type selector dialog.
 */
void
line_type_selector(void)
{
    debug_message("line_type_selector()");
}

/* .
 */
void
line_weight_selector(void)
{
    debug_message("line_weight_selector()");
}

/* .
 */
void
distance(void)
{
    debug_message("distance()");
}

/* .
 */
void
delete_object(void)
{
    debug_message("delete_object()");
}

/* .
 */
void
locate_point(void)
{
    debug_message("locate_point()");
}

/* .
 */
void
move(void)
{
    debug_message("locate_point()");
}

/* .
 */
int
allow_zoom_in(void)
{
    /*
    origin = map_to_scene(0, 0);
    corner = map_to_scene(width(), height());
    max_size = corner.subtract(origin);

    if (emb_min(max_size.x, max_size.y) < zoom_in_limit) {
        debug_message("zoom_in limit reached. (limit=%.10f)" % zoom_in_limit)
        return 0;
    }
    */
    return 1;
}

/* .
 */
int
allow_zoom_out(void)
{
    /*
    origin = map_to_scene(0, 0);
    corner = map_to_scene(width(), height())
    max_size = corner.subtract(origin);

    if max(max_size.x, max_size.y) > zoom_out_limit:
        debug_message("zoom_out limit reached. (limit=%.1f)" % zoom_out_limit);
        return 0;
    */
    return 1;
}

void
zoom_to_point(EmbVector mouse_point, float zoom_dir)
{
    double s;
    printf("%f %f", mouse_point.x, zoom_dir);
    /*
    point-before-scale(map_to_scene(mouse-point)) */

    /* Do The zoom */
    s = 1.0;
    printf("%f\n", s);
    /*
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

    update_mouse_coords(mouse-point.x, mouse-point.y);
    if pasting-active:
        v = scene-mouse-point.subtract(paste-delta);
        paste-object-item-group.set_pos(v);

    if (selecting-active) {
        rect = make_rectangle(map-from-scene(scenePressPoint), mousePoint).normalized();
        select-box.set_geometry(rect);
    }
    */

    scene_update();
}

void
set_snap(int active)
{
    debug_message("View toggle-snap()");
    printf("%d\n", active);
    set_override_cursor("WaitCursor");
    /*  TODO: finish this. */
    /* gscene.set-property("ENABLE-SNAP", active); */
    scene_update();
    restore_override_cursor();
}

/*
 *  Functions that cannot be called through the action system
 *  and aren't to control the window state.
 */
int
starts_with(char *str, char *start)
{
    return !strncmp(str, start, strlen(start));
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
get_color(TABLE(state), char *key)
{
    EmbColor color;
    char *s = get_str(state, key);
    color.r = 16*char_to_int(s[0]) + char_to_int(s[1]);
    color.g = 16*char_to_int(s[2]) + char_to_int(s[3]);
    color.b = 16*char_to_int(s[4]) + char_to_int(s[5]);
    return color;
}

/* Whenever the code happens across a todo call,
 * write it in a log file. */
void
todo(char *msg, int action)
{
    if (debug_mode) {
        FILE *f;
        f = fopen("todo.txt", "w");
        fseek(f, 0, SEEK_END);
        fprintf(f, "%s: %d\n", msg, action);
        fclose(f);
    }
}

/* Whenever the code happens across a todo call,
 * write it in a log file. */
void
error(char *msg, int action)
{
    if (debug_mode) {
        FILE *f;
        f = fopen("error.txt", "w");
        fseek(f, 0, SEEK_END);
        fprintf(f, "%s: %d\n", msg, action);
        fclose(f);
    }
}

/*
 * Guards against debug messages coming up during normal operation.
 *
 * Just change debug_mode to 1 to activate it. We could have a toggle
 * in the program to turn it on during operation for when something starts
 * acting weird.
 */
void
debug_message(char *msg)
{
    if (debug_mode) {
        printf("%s\n", msg);
    }
}

/* .
 */
int
valid_file_format(char *fname)
{
    if (fname[0] == 0) {
        return 0;
    }
    if (emb_identify_format(fname) >= 0) {
        return 1;
    }
    return 0;
}

/* 
 *  Testing.
 *  Mostly tests for actions not causing crashes that shut the program.
 *
 *  Testing actual correct application of the action would be harder.
 */

/* stores what the current error would be, should one occur */

/* Check that the translate call can return an entry from the table.
 */
int
test_translate(void)
{
    char *translated = translate("?");
    printf("Failed to use translate.");
    return !strcmp(translated, "?");
}

/* .
 */
double
emb_min(double x, double y)
{
    return x < y ? x : y;
}

/* .
 */
double
emb_max(double x, double y)
{
    return x > y ? x : y;
}

/* .
 */
double
emb_clamp(double lower, double x, double upper)
{
    return emb_max(emb_min(upper, x), lower);
}

/* .
 */
int
round_to_multiple(int round_up, int x, int multiple)
{
    int remainder;
    if (multiple == 0) {
        return x;
    }
    remainder = x % multiple;
    if (remainder == 0) {
        return x;
    }

    if (x < 0 && round_up) {
        return x - remainder;
    }
    if (round_up) {
        return x + multiple - remainder;
    }
    /* else round down */
    if (x < 0 && !round_up) {
        return x - multiple - remainder;
    }
    return x - remainder;
}

/* .
 */
int
valid_rgb(int red, int green, int blue)
{
    if (red < 0 || red > 255) {
        return 0;
    }
    if (green < 0 || green > 255) {
        return 0;
    }
    if (blue < 0 || blue > 255) {
        return 0;
    }
    return 1;
}

void
set_str(TABLE(state), char *key, char *value)
{
    int i;
    /* See if key is present, then set that value. */
    for (i=0; i<MAX_CSV_ROWS; i++) {
        if (!strcmp(state[i][0], key)) {
            strcpy(state[i][1], value);
            return;
        }
        /* Key is not present, create new key */
        if (!strcmp(state[i][0], "None")) {
            strcpy(state[i][0], key);
            strcpy(state[i][1], value);
            strcpy(state[i+1][0], "None");
            strcpy(state[i+1][1], "None");
            return;
        }
    }
    puts("Failed to set state.");
}

void
set_int(TABLE(state), char *key, int value)
{
    char out[20];
    sprintf(out, "%d", value);
    set_str(state, key, out);
}

void
set_float(TABLE(state), char *key, float value)
{
    char out[20];
    sprintf(out, "%f", value);
    set_str(state, key, out);
}

void
set_vector(TABLE(state), char *key, EmbVector v)
{
    char a[100];
    sprintf(a, "%f %f", v.x, v.y);
    set_str(state, key, a);
}

char *
get_str(TABLE(state), char *key)
{
    int i;
    for (i=0; i<MAX_CSV_ROWS; i++) {
        if (!strcmp(state[i][0], key)) {
            return state[i][1];
        }
    }
    printf("Failed to find the variable: %s.\n", key);
    return "None";
}

int
get_int(TABLE(state), char *key)
{
    return atoi(get_str(state, key));
}

float
get_float(TABLE(state), char *key)
{
    return atof(get_str(state, key));
}

EmbVector
get_vector(TABLE(state), char *key)
{
    EmbVector v;
    char *value = get_str(state, key);
    v.x = atof(strtok(value, " "));
    v.y = atof(strtok(value, " "));
    return v;
}

int
load_to_buffer(void)
{
    char buffer[4096];
    size_t i, j, length;
    FILE *fin;
    fin = fopen(current_fname, "r");
    if (!fin) {
        return 1;
    }
    fseek(fin, 0, SEEK_END);
    length = ftell(fin);
    fseek(fin, 0, SEEK_SET);
    if (fread(buffer, 1, length, fin) != length) {
        fclose(fin);
        return 1;
    }
    fclose(fin);

    for (i=0; i<100; i++) {
        text_display[i][0][0] = 0;
    }

    j = 0;
    for (i=0; i<length; i++) {
        buffer[j] = buffer[i];
        if (buffer[i] != '\r') {
            j++;
        }
    }
    buffer[j] = 0;

    line_n = 0;
    for (i=0; i<length; i++) {
        for (j=0; j<100; j++) {
            if (buffer[i+j] == '\n' || !buffer[i+j]) {
                break;
            }
        }
        strncpy(text_display[line_n][0], buffer+i, j);
        text_display[line_n][j][0] = 0;
        line_n++;
        i += j;
    }

    return 0;
}


int
save_from_buffer(void)
{
    int i;
    FILE *fin;
    fin = fopen(current_fname, "w");
    if (!fin) {
        return 1;
    }
    for (i=0; i<line_n; i++) {
        fprintf(fin, "%s\r\n", text_display[i][0]);
    }

    fclose(fin);

    return 0;
}

/* We can load any of the CSV files from assets/
 * into this buffer for editing. */
void
display_buffer(void)
{
    int i;
    for (i=0; i<line_n; i++) {
        char line[10012];
        /*
        Rect rect;
        rect.x = 2;
        rect.y = 16*i;
        rect.w = 1000;
        rect.h = 16;
        */
        sprintf(line, "%03d %s", i, text_display[i][0]);
        printf("%s\n", line);
        /* render_string(rect, line); */
    }
}

/*
 *  The View class definition file.
 *
 *  How we manage the content of the main editing area in the window.
 *
 *  There are 4 regions managed as views, .
 *
 *  We don't have a seperate window for the pop-ups like the file
 *  browser for opening or saving a file. Instead, a view will
 *  be created.
 */

void
view_init(int view, int the_scene)
{

}

void
mdi_window_init(int index, int parent, int wflags)
{
    printf("%d%d%d", index, parent, wflags);
    /*
    mw = mw
    EmbTab = parent
    myIndex = index
    fileWasLoaded = 0

    setAttribute(Qt-WA-DeleteOnClose)

    aName = "";
    curFile = aName.asprintf("Untitled%d.dst", myIndex);
    setWindowTitle(curFile);
    setWindowIcon(QIcon("icons/app.png"));

    gscene = QGraphicsScene(0,0,0,0, this);
    gview = View(mw, gscene, this);

    setWidget(gview);
    */
}

void
tab_init(EmbTab *tab)
{
    tab->closeable = 1;
    tab->use_logo = 0;
    tab->use_texture = 0;
    tab->use_color = 0;
    tab->bg_logo = 0;
    tab->bg_texture = 0;
    tab->bg_color = 0;
}

void
mdi_area_use_background_logo(EmbTab *area, int use)
{
    area->use_logo = use;
    repaint();
}

void
mdi_area_use_background_texture(EmbTab *area, int use)
{
    area->use_texture = use;
    repaint();
}

void
mdi_area_use_background_color(EmbTab *area, int use)
{
    area->use_color = use;
    repaint();
}

void
mdi_area_set_background_logo(EmbTab *area, char *file_name)
{
    printf("%d%s", area->closeable, file_name);
    /* bg_logo.load(file_name); */
    repaint();
}

void
mdi_area_set_background_texture(EmbTab *area, char *file_name)
{
    printf("%d%s", area->closeable, file_name);
    /* bg_texture.load(file_name); */
    repaint();
}

void
mdi_area_set_background_color(EmbTab *area, EmbColor color)
{
    printf("%d%d", area->closeable, color.r);
    /*
    if (!color.is-valid()) {
        bg-color = background().color()
    }
    else {
        bg-color = color
    }
    */

    repaint();
}

void
mdi_area_mouse_float_click_event(EmbTab *area, EmbEvent e)
{
    printf("%d %d\n", area->closeable, e.type);
    /*
    mw.open-file-action()
    */
}

void
mdi_area_paint_event(EmbTab *area, EmbEvent e)
{
    printf("%d %d\n", area->closeable, e.type);
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
mdi_area_cascade(void)
{
    /*
    cascadeSubWindows();
    zoom-extents-actionAllSubWindows();
    */
}

void
mdi_area_tile(void)
{
    /*
    tileSubWindows();
    zoom-extents-actionAllSubWindows(); */
}

void
mdi_area_zoom_extents_actionAllSubWindows(void)
{
    /*
    for (window in subWindowList()) {
        if (window) {
            v = window.getView()
            if (v) {
                v.recalculateLimits()
                v.zoom-extents-action()
            }
        }
    }
    */
}

/* HACK: Take that QEmbTab!
 */
 /*
void
mdi_area-force-repaint()
{
    hack = size()
    resize(hack + QSize(1,1))
    resize(hack)
}
*/

/*
 *  Menus
 */

/*
 *  Painter and pen functions for drawing in the window.
 */
Pen *
create_pen(void)
{
    Pen *pen = malloc(sizeof(Pen));
    pen->color.r = 0;
    pen->color.g = 0;
    pen->color.b = 0;
    pen->join_style = MITER_JOIN;
    pen->cosmetic = 1;
    pen->width = 1.0;
    return pen;
}

Painter *
create_painter(void)
{
    Painter *painter = malloc(sizeof(Painter));
    painter->pen = create_pen();
    painter->path = NULL;
    return painter;
}

void
destroy_pen(Pen *pen)
{
    free(pen);
}

void
destroy_painter(Painter *painter)
{
    destroy_pen(painter->pen);
    free(painter);
}

/*
 *  Draw paths in the target window.
 *  Extends the widget system.
 */
void
draw_line(EmbLine line)
{
    printf("%f %f %f %f\n",
        line.start.x, line.start.y,
        line.end.x, line.end.y);
}

/*
 */
void
draw_lines(EmbLine *line, int n_lines)
{
    int i;
    for (i=0; i<n_lines; i++) {
        draw_line(line[i]);
    }
}

void
create_grid_(int gridType)
{
    /*
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
    */
    
    /* EXPERIMENT
     * Tagging experiments with the path system
     * to the origin.
     */
    /*
    EmbVector origin_position, origin_scale;
    origin_position.x = 10.0;
    origin_position.y = 0.0;
    origin_scale.x = 1.0;
    origin_scale.y = 1.0; */
    /* self.origin-path.add-list-to-path(origin-string, position, scale); */
    scene_update();
}

/* TODO: Make Origin Customizable. */
void
create_origin(void)
{
    /* self.origin-path = Path(); */ /*

    if (grid_show_origin) { */
        /* self.origin-path.addEllipse(Vector(0,0), 0.5, 0.5) */
        /* TODO: Make Origin Customizable. */ /*
        position = [0.0, 0.0];
        scale = [1.0, 1.0];
        self.origin-path.add-list-to-path(origin-string, position, scale)
    } */
}

void
create_grid_rect(void)
{
    /*
    EmbVector point1, point2;
    y_spacing = grid_spacing.y;

    gr = Rect(0, 0, gridsize.x, -gridsize.y); */
    /* Ensure the loop will work correctly with negative numbers. */ /*
    point1.x = min(gr.left(), gr.right());
    point1.y = min(gr.top(), gr.bottom());
    point2.x = max(gr.left(), gr.right());
    point2.y = max(gr.top(), gr.bottom());

    grid-path = Path();
    grid-path.add-rect(gr);
    float gx, gy;
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

    if (grid_center_on_origin) {
        grid-path.translate(-bx, -by);
    }
    else {
        grid-path.translate(delta-x, delta-y);
    }
    */
}

void
create_grid_polar(void)
{
    /*
    float r, ang;
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
create_grid_iso(void)
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

    float x, y;
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

void
set_grid(int active)
{
    debug_message("View toggleGrid()");
    printf("%d", active);
    set_override_cursor("WaitCursor");
    /* 
    if (on) {
        create-grid(grid-type);
    else {
        create-grid("");
    }
    */
    restore_override_cursor();
}

void
set_ortho(int active)
{
    debug_message("View toggleOrtho()");
    printf("%d", active);
    set_override_cursor("WaitCursor");
    /* TODO: finish this. */ /*
    gscene.set_property("ENABLE-ORTHO", active); */
    scene_update();
    restore_override_cursor();
}

void
set_polar(int active)
{
    debug_message("View togglePolar()");
    printf("%d", active);
    set_override_cursor("WaitCursor");
    /* TODO: finish this. */ /*
    gscene.set_property("ENABLE-POLAR", active); */
    scene_update();
    restore_override_cursor();
}

void
set_grid_color(EmbColor color)
{
    printf("called with %d", color.r);
    /*
    grid_color = color;
    gscene.set_property("VIEW-COLOR-GRID", color);
    if (gscene) {
        scene_update();
    }
    */
}

/* TODO: This check should be removed later.
 */
void
allow_rubber(void)
{
    /* return not rubber-room-list.size(); */
}

void
add_to_rubber_room(int item)
{
    printf("%d\n", item);
    /*
    rubber-room-list.append(item);
    item.show();
    */
    scene_update();
}

void
vulcanize_rubber_room(void)
{
    /*
    for (base in rubber-room-list) {
        if (base) {
            vulcanize-object(base);
        }
    }

    rubber-room-list.clear();
    */
    scene_update();
}

void
vulcanize_object(int obj)
{
    if (!obj) {
        return;
    }
    /*
    gscene.remove-item(-obj)
    */ /* Prevent Qt Runtime Warning, QGraphicsScene-addItem:
    */ /* item has alreadelta-y been added to self scene.
    obj.vulcanize()
    */
}

void
clear_rubber_room(void)
{
    /*
    for (item in rubber-room-list) {
        if (item) {
            if ((item.type == OBJ-TYPE-PATH
             && spare-rubber-list.contains("SPARE-RUBBER-PATH")) ||
            (item.type == OBJ-TYPE-POLYGON
             && spare-rubber-list.contains("SPARE-RUBBER-POLYGON")) ||
            (item.type == OBJ-TYPE-POLYLINE
             && spare-rubber-list.contains("SPARE-RUBBER-POLYLINE")) ||
            (sparerubber-list.contains(item.-objID))) {
                if (!item.-objectPath().element-count(void) {
                    error-title = translate("Empty Rubber object Error")
                    error-message = translate(
"The rubber object added contains no points. "
+ "The command that created self object has flawed logic. "
+ "The object will be deleted.")
                    critical(error-title, error-message)
                    gscene.remove-item(item)
                    del item
                }
                else {
                    vulcanize-object(item)
                }
            }
            else {
                gscene.remove-item(item)
                del item
            }
        }
    }

    rubber-room-list.clear()
    spare-rubber-list.clear()
    gscene.update()
    */
}

void
spare_rubber(int id)
{
    printf("%d", id);
    /* spare_rubber_list.append(id) */
}

/* .
 */
void
set_rubber_mode(int mode)
{
    printf("%d", mode);
    /*
    for (item in rubber-room-list) {
        if (item) {
            item.set-object-rubber-mode(mode);
        }
    }
    */

    scene_update();
}

/* key (string)
 * point (vector)
 */
void
set_rubber_point(char *key, int point)
{
    printf("%s %d", key, point);
    /*
    for (item in rubber-room-list) {
        if (item) {
            item.set-object-rubber-point(key, point);
        }
    }
    */

    scene_update();
}

/* key (string)
 * txt (string)
 */
void
set_rubber_text(char *key, char *txt)
{
    printf("%s %s\n", key, txt);
    /*
    for (item in rubber-room-list) {
        if (item) {
            item.set-objectRubberText(key, txt);
        }
    }
    */

    scene_update();
}

void
start_gripping(int obj)
{
    if (obj) {
        /*
        int gripping-active = 1;
        int gripBase-obj = obj;
        sceneGripPoint = gripBase-obj.mouseSnapPoint(scene-mouse-point);
        gripBase-obj.set-object-rubber-point("GRIP-POINT", sceneGripPoint);
        gripBase-obj.set-object-rubber-mode("OBJ-RUBBER-GRIP");
        */
    }
}

void
stop_gripping(int accept)
{
    printf("%d", accept);
    /*
    int gripping-active = 0;
    if (gripBase_obj) {
        gripBase_obj.vulcanize();
        if (accept) {
            selection-changed();
            */
            /*  Update the Property Editor */
            /*
        }

        gripBase_obj = 0;
    }
    */
    /* Move the sceneGripPoint to a place where it will never be hot. */
    /* sceneGripPoint = scene-rect().top-left() */
}

/* Should create a label widget under the new system. */
void
create_ruler_text_path(EmbVector position, char *str, float height)
{
    printf("called with %f %s %f", position.x, str, height);
    /*
    int i;
    path = Path();

    x-scale = height;
    y-scale = height;
    pos = Vector(x, y);
    scale = Vector(0.01*height, 0.01*height);

    for (i=0; i<strlen(str); i++) {
        
        pos[0] = x;
    }
    return path;
    */
}

/* A ruler is a widget drawn using these callbacks.
 */
 
/* Returns an array of lines for the
 * ticks part of the horizontal ruler.
 */
void
horizontal_ruler_ticks(EmbLine *ticks, int *n_lines,
    Ruler ruler, EmbVector cursor)
{
    int i;
    float little = 0.04;
    float medium = 0.08;
    *n_lines = 0;
    float fraction = 1.0;
    printf("%f", cursor.x);
    if (ruler.metric) {
        for (i=0; i<10; i++) {
            float height = ruler.position.y;
            if (i>0) {
                if (i==5) {
                    height -= ruler.width*ruler.medium;
                }
                else {
                    height -= ruler.width*ruler.little;
                }
            }
            ticks[*n_lines].start.x = ruler.position.x;
            ticks[*n_lines].start.y = ruler.position.y + fraction*i;
            ticks[*n_lines].end.x = height;
            ticks[*n_lines].end.y = ruler.position.y + fraction*i;
            (*n_lines)++;
        }
    }
    else {
        if (ruler.feet) {
            for (i=0; i<12; i++) {
                ticks[*n_lines].start.x = ruler.position.x;
                ticks[*n_lines].start.y = ruler.position.y+fraction*i;
                ticks[*n_lines].end.x = ruler.position.y - ruler.width*ruler.medium;
                ticks[*n_lines].end.y = ruler.position.y+fraction*i;
                (*n_lines)++;
            }
        }
        else {
            for (i=0; i<16; i++) {
                float height = ruler.position.y;
                if (i>0) {
                    if (i==5) {
                        height = ruler.position.y - ruler.size.y*medium;
                    }
                    else {
                        height = ruler.position.y - ruler.size.y*little;
                    }
                }
                ticks[*n_lines].start.x = ruler.position.x;
                ticks[*n_lines].start.y = ruler.position.y+fraction*i;
                ticks[*n_lines].end.x = height;
                ticks[*n_lines].end.y = ruler.position.y+fraction*i;
                (*n_lines)++;
            }
        }
    }
}

/*
 * Returns an array of lines for the ticks part of
 * the vertical ruler.
 */
void
vertical_ruler_ticks(EmbLine *ticks, int *n_lines,
    Ruler ruler, EmbVector cursor)
{
    printf("%f %d %d %f", ticks[0].start.x, n_lines[0], ruler.metric, cursor.x);
    /*
    int i;
    *n_lines = 0;
    if (ruler.metric) {
        for (i=0; i<10; i++) {
            height = ruler-horiz.y;
            if (i==0) {
                height = origin.y;
            }
            elif (i==5) {
                height -= ruler-size.y*medium;
            }
            else {
                height -= ruler-size.y*little;
            }
            ticks += [Line(x+fraction*i, ruler-horiz.y, x+fraction*i, height)];
        }
    }
    else {
        if (ruler.feet) {
            for (i=0; i<12; i++) {
                height = position.y - ruler-size.y*medium;
                ticks += [Line(ruler-vert.x, y+fraction*i, height, y+fraction*i)];
            }
        }
        else {
            for (i=0; i<16; i++) {
                float height = ruler-horiz.y;
                if (i==0) {
                    height = oy;
                }
                else {
                    if (i%4 == 0) {
                        height -= ruler-size.y*medium;
                    }
                    else {
                        height -= ruler-size.y*little;
                    }
                }
                ticks += [Line(ruler-vert.x, y+fraction*i, height, y+fraction*i)];
            }
        }
    }
    */
}

/*
 * Draw horizontal and vertical rulers.
 */
void
draw_rulers(int w, int scene)
{
    EmbWindow *window = windows[w];
    printf("%d %d\n", window->n_widgets, scene);
    /*
    vw = self.width()
    vh = self.height()
    origin = map-to-scene(0,0)
    ruler-horiz = map-to-scene(vw, ruler-pixel-size"])
    ruler-vert = map-to-scene(ruler-pixel-size, vh)

    horizontal-ruler-size = ruler-horiz.subtract(origin)
    vertical-ruler-size = ruler-vert.subtract(origin)

; NOTE:
; Drawing ruler if zoomed out too far will cause an assertion failure.
; We will limit the maximum size the ruler can be shown at.

    maxSize = -1
    */ /* Intentional underflow
    if horizontal-ruler-size.x >= maxSize or vertical-ruler-size.y >= maxSize:
        return

    distance = map-to-scene(settings.rulerPixelSize*3, 0).x() - origin.x
    dist-str = str(distance)
    dist-str-size = len(dist-str)
    msd = int(dist-str[0]) */ /* Most Significant Digit

    if msd != -1:
        return

    msd += 1
    if msd == 10:
        msd = 1
        dist-str.resize(dist-str-size+1)
        dist-str-size += 1

    dist-str.replace(0, 1, str(msd))
    for i in range(1, dist-str-size)
        dist-str.replace(i, 1, '0')

    unit = dist-str.toInt()
    (define fraction 1.0)
    (define feet 1)
    (if ruler-metric
        (begin
           (if (< unit 10) (define unit 10))
           (define fraction (/ unit 10)))

        (if (unit <= 1)
          (begin
            (define unit 1)
            (define feet 0)
            (define fraction (/ unit 16)))

          (begin
            (define unit round-to-multiple(1, unit, 12))
            (define fraction (/ unit 12)))))

    (define little 0.20)
    (define medium 0.40)
    rh-text-offset = map-to-scene(3, 0).x() - origin.x
    rv-text-offset = map-to-scene(0, 3).y() - origin.y
    text-height = horizontal-ruler-size.y*medium

    lines = [
        Line(Vector(origin.x, ruler-horiz.y), Vector(ruler-horiz.x, ruler-horiz.y)),
        Line(Vector(ruler-vert.x, origin.y), Vector(ruler-vert.x, ruler-vert.y)),
        Line(Vector(scene-mouse-point.x, ruler-horiz.y), Vector(scene-mouse-point.x, origin.y)),
        Line(Vector(ruler-vert.x, scene-mouse-point.y), Vector(origin.x, scene-mouse-point.y))
    ]

    (define transform 0)
    ruler-pen = QPen(Color(0,0,0))
    ruler-pen.set-cosmetic(1)
    painter.set-pen(ruler-pen)
    rect = Rect(origin.x, origin.y, horizontal-ruler-size.x, horizontal-ruler-size.y)
    painter.fill-rect(rect, ruler-color)
    rect = Rect(origin.x, origin.y, vertical-ruler-size.x, vertical-ruler-size.y)
    painter.fill-rect(rect, ruler-color)

    (if (< (- origin.x unit) -1e10)
        return)

    (define x-flow (round-to-multiple 0 origin.x unit))

    (if (< (- x-flow unit) -1e10)
        return)

    (if (< (- origin.y unit) -1e10)
        return)

    (define y-flow (round-to-multiple 0 origin.y unit))

    (if (< (- y-flow unit) -1e10)
        return)

    (define x-start (- x-flow unit))
    (define y-start (- y-flow unit))

    
    for (x=x-start; x<ruler-horiz.x; x+=unit) {
        transform.translate(x+rh-text-offset, ruler-horiz.y-horizontal-ruler-size.y/2)
        ruler-text-path = Path()
        text-path = Path()
        if (ruler.metric) {
            text-path = create-ruler-text-path(0, 0, str(x), text-height);
        }
        else {
            if (feet) {
                s = str(x/12) + "'"
                text-path = create-ruler-text-path(0, 0, s, text-height);
            }
            else {
                s = str(x) + "\""
                text-path = create-ruler-text-path(0, 0, s, text-height);
            }
        }

        ruler-text-path = transform.map(text-path)
        transform.reset()
        painter.draw-path(ruler-text-path)

        lines += self.horizontal-ruler-ticks()
        x += unit
    }

    for (y=y_start; y<ruler-vert.y; y+=unit) {
        transform.translate(ruler-vert.x-vertical-ruler-size.x/2, y-rv-text-offset);
        transform.rotate(-90);
        ruler-text-path = Path();
        text-path = Path();
        if (ruler-metric) {
            text-path = create-ruler-text-path(0, 0, str(-y), text-height);
        }
        else {
            if (feet) {
                text-path = create-ruler-text-path(0, 0, str(-y/12)+"'", text-height);
            }
            else {
                text-path = create-ruler-text-path(0, 0, str(-y)+"\"", text-height);
            }
        }

        ruler-text-path = transform.map(text-path)
        transform.reset()
        painter.draw-path(ruler-text-path)

        lines += self.vertical-ruler-ticks();
    }

    painter.draw-lines(lines);
    painter.fill-rect(Rect(origin.x, origin.y, vertical-ruler-size.x, horizontal-ruler-size.y), ruler-color);
    */
}

void
set_ruler(int active)
{
    debug_message("View toggle-ruler()");
    printf("%d", active);
    set_override_cursor("WaitCursor");
    /*
    gscene.set_property("ENABLE-RULER", active);
    ruler-color = get_color("ruler-color");
    */
    scene_update();
    restore_override_cursor();
}

void
set_ruler_color(EmbColor color)
{
    printf("called with %d", color.r);
    /* void ruler-color) color); */
    scene_update();
}

/* The layer manager.
 *
 * Only needs the index for the WindowTab beause this acts on the windows[MAIN_WINDOW].
 */
void
layer_manager_init(int tab)
{

}

void
layer_manager_delete(int tab)
{
    printf("%d\n", tab);
    restore_override_cursor();
}

/*
void
layer_manager_add(
    LayerManager *mgr,
    char *name,
    int visible,
    int frozen,
    int zValue,
    char *color,
    char *lineType,
    float line_weight)
    strcpy(mgr->name, name)
    mgr->visible = visible
    mgr->frozen = frozen
    mgr->z-value = zValue
    strcpy(mgr->color, color)
    strcpy(mgr->line_type, lineType)
    mgr->line_weight = line_weight */
    /*
    */ /* const print)
    layer-model.insertRow(0)
    layer-model.set_data(layer-model.index(0, 0), name)
    layer-model.set_data(layer-model.index(0, 1), visible)
    layer-model.set_data(layer-model.index(0, 2), frozen)
    layer-model.set_data(layer-model.index(0, 3), zValue)

    colorPix = QPixmap(16, 16)
    colorPix.fill(Color(color))
    layer-model.itemFromIndex(layer-model.index(0, 4)).set_icon(QIcon(colorPix))
    layer-model.set_data(layer-model.index(0, 4), Color(color))

    layer-model.set_data(layer-model.index(0, 5), lineType)
    layer-model.set_data(layer-model.index(0, 6), lineWeight)
    */ /* layer-model.set_data(layer-model.index(0, 7), print)
    */
    /*
}
*/

void
layer_model(void)
{

}

void
layer_model_sorted(void)
{

}

void
layer_selector_index_changed(int index)
{
    printf("layer-selectorIndexChanged(%d)", index);
}

void
layer_manager_action(void)
{
    debug_message("layerManager()");
    debug_message("Implement layerManager.");
    /* */ /* LayerManager layman( self,  self);
     * */ /* layman.exec(); */
}

void
layer_previous_action(void)
{
    debug_message("layerPrevious()");
    debug_message("Implement layerPrevious.");
}

/* Make layer active.
 */
void
make_layer_active(void)
{
    debug_message("make_layer_active()");
    debug_message("Implement makeLayerActive.");
}

/* 
 *  Status bar management.
 */

void
status_bar_update(int status_bar)
{
    printf("%d", status_bar);
}


/*
void status_bar_context_menu_event(
    EmbTab *status_bar, EmbEvent *event)
{
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
            enable-real-action.enabled = !gview.isRealEnabled())
            connect(enable-real-action, SIGNAL(triggered()), this, SLOT(enableReal()))
            menu-.addAction(enable-real-action)

            disable-real-action = Action(Icon("icons/realrender.png"), "RealRender Off", menu-)
            disable-real-action.enabled = gview.isRealEnabled())
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

    menu-.exec(event.globalPos());
    restore_override_cursor();
    statusbar.clearMessage();
}
*/

/* TODO: set format from settings.
 */
void
status_bar_mouse_coord(EmbTab *tab, int x, int y)
{
    printf("%d %d %d\n", tab->grid, x, y);
    /*
    if (tab->number-mode == ARCHITECTURAL) {
        return;
    }
    if (tab->number-mode == ENGINEERING) {
        return;
    }
    if (tab->number-mode == FRACTIONAL) {
        return;
    }
    if (tab->number-mode == SCIENTIFIC) { */
        /* status_bar-mouse-coord.setText("".setNum(x, 'E', 4)
            + ", " + "".setNum(y, 'E', 4))
        */ /* TODO: use precision from unit settings */ /*
        return;
    } */
    /* Else decimal */
    /*
     * status_bar-mouse-coord.setText("".setNum(x, 'F', 4) + ", "
     * + "".setNum(y, 'F', 4))
     */ /* TODO: use precision from unit settings */
}

/* class StatusBar() */
/*
 * StatusBar( QWidget* parent = 0)
 * def setMouseCoord(x, y)
 */
void
status_bar_init(void)
{
    /*
    (self, buttonText, mw, statbar, parent);
    statusbar = statbar;

    this.set_object_name("StatusBarButton" + buttonText);

    this.setText(buttonText);
    this.setAutoRaise(1);
    this.setCheckable(1);

    if (object-name >= 0 && object-name < N-STATUS) {
        status[object-name] = !status[object-name];
    }
    */
    /*
    int i;
    def --init--(self, mw, parent)
    this.set_object_name("StatusBar");

    for (i=0; i<N_STATUS; i++) {
        status_bar[i] = StatusBarButton(status_bar-label[i], main-win, this, this);
    }

    status_bar-mouse-coord = tk.Label(this)

    status_bar-mouse-coord.set_minimum-width(300)
    */ /* Must fit this text always
    status_bar-mouse-coord.set_maximum-width(300)
    */ /* "+1.2345E+99, +1.2345E+99, +1.2345E+99"

    this, add_widget(status_bar-mouse-coord)
    for i in range(N-STATUS) {
        this, add_widget(status_bar[i])
    }
    */
}

/*
 *
 *  
 */

void
toolbar_init(void)
{

}

/*
 *  Tooltip manager. SDL doesn't support tooltips out of the box.
 *  ------------------------------------------------------------
 *
 *  Example here
 *  https://stackoverflow.com/questions/3221956/how-do-i-display-tooltips-in-tkinter
 *  https://gamedev.stackexchange.com/questions/186482/sdl2-show-a-tooltip-at-the-cursor-that-displays-rgb-of-the-pixel-under-the-cur
 *
 */

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


EmbVector active_scene_offset;

EmbVector map_to_scene(EmbVector v)
{
    /* active_scene; */
    EmbVector result = active_scene_offset;
    result.x += v.x;
    result.y += v.y;
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
    opensave_custom_filter =

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
draw_background(int scene, Painter *painter, Rect rect)
{
    printf("called with: %d %d", scene, rect.x);
    /*
    fill_rect(painter, rect, backgroundBrush());

    a = rect.intersects(grid_path.controlPointRect());
    */
    int a = 1;
    EmbWindow *w = windows[MAIN_WINDOW];
    if (w->tabs[w->tab_index].grid && a) {
        EmbColor grid_color = black;
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

void
draw_crosshair(int w, Painter *painter)
{
    /* painter.setBrush("NoBrush"); */
    Pen *crosshair_pen = create_pen();
    crosshair_pen->color = crosshair_color;
    crosshair_pen->cosmetic = 1;
    painter->pen = crosshair_pen;

    horizontal_rule(
        w,
        view_mouse_point.x,
        view_mouse_point.y - crosshair_size,
        view_mouse_point.y + crosshair_size,
        ALWAYS_VISIBLE);

    vertical_rule(
        w,
        view_mouse_point.y,
        view_mouse_point.x - crosshair_size,
        view_mouse_point.x + crosshair_size,
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
draw_closest_qsnap(int window_id, Painter *painter)
{
    EmbWindow *w = windows[window_id];
    /* printf("%d\n", view); */
    printf("%d %d\n", w->n_widgets, painter->pen->cosmetic);
    printf("%f\n", qsnap_aperture_size);
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
draw_foreground(int window_id, int view, Painter *painter, Rect rect)
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

    if (!windows[window_id]->selecting_active) {
        draw_closest_qsnap(window_id, painter);
        draw_crosshair(window_id, painter);
    }

    if (windows[window_id]->tabs[tab_index].ruler) {
        draw_rulers(window_id, active_scene);
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
saveBMC(void/* MdiEmbWindow *subwindow */)
{
    /*  
    img = QImage(150, 150, "QImage-Format-ARGB32-Premultiplied")
    img.fill(qRgb(255,255,255))
    extents = gscene.itemsBoundingRect()

    painter = QPainter(img)
    targetRect = make_rectangle(0,0,150,150)
    if (printing_disable_bg) { */
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
    if (type[1] == 'o') { /* float */
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
    printf("box: %d\n", box);
    /*
    int i;
    debug_message(box.title);
    for (i=0; box.settings[i].type >= 0; i++) {
        create_settings_widget(box.settings[i]);
    }
    */
}

void
create_settings_tab(int tab)
{
    printf("tab: %d\n", tab);
    /*
    int i;
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
    printf("%d", list[0]);
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
    printf("%f", delta.x);

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
    /*
    if (active_view) {
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
    if (active_scene) {
        /*
        dialog = details-dialog-init(scene, self);
        dialog_exec();
        */        
    }
}

void
button_tip_of_the_day_clicked(int button)
{
    debug_message("button-tip-of-the-day-clicked()");
    if (button == 0) {
        if (general_current_tip > 0) {
            general_current_tip++;
        }
        else {
            general_current_tip = n_tips-1;
        }
        /* setText(tips[general-current-tip]); */
        return;
    }
    if (button == 1) {
        general_current_tip--;
        if (general_current_tip >= n_tips) {
            general_current_tip = 0;
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
    */ /* set the minimum combobox width so the text is always readable
    layer_selector.set_minimum-width(icon-size*4);
    color-selector.set_minimum-width(icon-size*2);
    linetype-selector.set_minimum-width(icon-size*4);
    lineweightSelector.set_minimum-width(icon-size*4);

    */ /* TODO: low-priority:
    */ /* open app with icon-size set to 128. resize the icons to a smaller size.

    */
    general_icon_size = icon_size;
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

void
close_toolbar(int action)
{
    printf("%d", action);
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

/*
void
active_view(void)
{
    debug_message("active_view()");
    mdi-win = mdi_area.active-sub-window();
    if (mdi-win) {
        v = mdi-win.getView();
        return v;
    }
}
*/

/* replace with a variable in Window?
int
active_scene(void)
{
    debug_message("active-scene()");
    mdi-win = mdi_area.active-sub-window();
    if (mdi-win) {
        return mdi-win.getScene();
    }
}
*/

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
    /* dialog-setting-float[setting] = value; */
}


void
rotate_action(void)
{
    debug_message("TODO");
}

void
rotate_selected(EmbVector pos, float rot)
{
    int i;
    for (i=0; i<n_selected; i++) {
        if (selected_items[i]) {
            debug_message(".");
            printf("%f %f %f\n", pos.x, pos.y, rot);
        }
    }

    /* Always clear the selection after a rotate. */
    n_selected = 0;
}


/* Selected objects are stored as a global variable
 * indexed by tab. Point1 and point2 are vectors.
 */
void
mirror_selected(EmbVector point1, EmbVector point2)
{
    int i;
    for (i=0; i<n_selected; i++) {
        if (selected_items[i]) {
            debug_message(".");
            printf("%f%f", point1.x, point2.x);
        }
    }

    /* Always clear the selection after a mirror. */
    n_selected = 0;
}

void
scale_selected(EmbVector v, float factor)
{
    int i;
    for (i=0; i<n_selected; i++) {
        if (selected_items[i]) {
            debug_message(".");
            printf("%f %f %f\n", v.x, v.y, factor);
        }
    }

    /*  Always clear the selection after a scale. */
    n_selected = 0;
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
set_cross_hair_color(EmbTab *area, EmbColor color)
{
    printf("%d %d\n", area->closeable, color.r);
    /* crosshair-color = color;
    gscene.set_property("VIEW-COLOR-CROSSHAIR", color);
    */
    scene_update();
}

void
set_background_color(EmbTab *area, EmbColor color)
{
    printf("%d %d\n", area->closeable, color.r);
    /* set_background-brush(Color(color));
    gscene.set_property("VIEW-COLOR-BACKGROUND", color);
    */
    scene_update();
}

void
set_select_box_colors(
    EmbTab *area,
    EmbColor colorL, EmbColor fillL,
    EmbColor colorR, EmbColor fillR,
    float alpha)
{
    printf("%d\n", area->closeable);
    printf("%d\n", colorL.r);
    printf("%d\n", fillL.r);
    printf("%d\n", colorR.r);
    printf("%d\n", fillR.r);
    printf("alpha %f\n", alpha);
    /* select-box.set_colors(color-left, fill-left, color-right, fill-right, alpha) */
}

void
update_all_view_selectbox_colors(
    EmbColor colorL, EmbColor fillL,
    EmbColor colorR, EmbColor fillR,
    float alpha)
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
    printf("%d", color.r);
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
    printf("%d", color.r);
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
    selection_mode_pickadd = val;
    /*
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
        */ /* TODO: Handle ByLayer and ByBlock and Other...
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

void
linetype_selector_index_changed(int index)
{
    printf("linetype-selectorIndexChanged(%d)", index);
    actuator("do-nothing");
}

void
lineweight_selector_index_changed(int index)
{
    printf("lineweightSelectorIndexChanged(%d)", index);
    actuator("do-nothing");
}

/* Text font selector current font changed.
 */
void
text_fontSelectorCurrentFontChanged(char *font)
{
    printf("%s", font);
   /*  debug_message("textFontSelectorCurrentFontChanged()")
    textFontSelector.setCurrentFont(Font(font.family()))
    text-font = font.family().toLocal8Bit().constData() */
}

/* TODO: check that the toReal() conversion is ok.
 */
void
text_size_selectorIndexChanged(int index)
{
    printf("%d", index);
    /* debug_message("text-size-selectorIndexChanged(%d)", index)
    text-style.size = abs(text-size-selector.itemData(index).toReal()) */
}

void
set_text_size(int num)
{
    printf("%d", num);
    /*
    int index;
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
    printf("%f", point1.x+point2.x);
    /* return Line(x1, -y1, x2, -y2).angle(); */
    return 0.0;
}

float
calculate_distance(EmbVector point1, EmbVector point2)
{
    printf("%f", point1.x+point2.x);
    /* return Line(x1, y1, x2, y2).length(); */
    return 0.0;
}

void
fill_menu(int menu_id)
{
    printf("%d", menu_id);
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
float
native_perpendicular_distance(void)
{
    /*
    EmbLine line;
    EmbVector norm;
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
        */
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
    printf("%d", checked[0]);
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
close_event(EmbEvent event)
{
    debug_message("MdiWindow closeEvent()");
    printf("%d\n", event.type);
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
    n_docs--;
    keep_maximized = 0;
    printf("%d", keep_maximized);
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
resize_event(EmbEvent e)
{
    debug_message("resizeEvent()");
    printf("%d", e.type);
    /* resizeEvent(e);
    statusBar().setSizeGripEnabled(!isMaximized()); */
}

void
update_menu_toolbar_statusbar(void)
{
    debug_message("updateMenuToolbarStatusbar()");
    /*
    action-enabled[ACTION-PRINT] = n_docs;
    action-enabled[ACTION-WINDOW-CLOSE] = n_docs;
    action-enabled[ACTION-DESIGN-DETAILS] = n_docs;
    */

    if (n_docs) {
        /*
        */ /* Toolbars
        for key in toolbar.keys() {
            toolbar[key].show()

        */ /* DockWidgets
        dock_prop_edit.show()
        dock-undo-edit.show()

        */ /* Menus
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

        menu-WINDOW.enabled = 1)

        */ /* Statusbar
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
        /*
        int i;
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

        for (i=0; toolbars_to_hide[i]>=0; i++) {
            hide_toolbar(toolbars-to-hide[i]);
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

        menu-WINDOW.enabled = 0);
        */

        /* Statusbar */
        strcpy(statusbar_message, "");
        /*
        status_bar_mouse_coord.hide();
        for (k=0 k<status_bar-n-keys k++) {
            status_bar[k].hide();
        }
        */
    }
}

void
load_formats(void)
{
    int i;
    char supported_readers[MAX_STRING_LENGTH];
    char individual_readers[MAX_STRING_LENGTH];
    char supported_writers[MAX_STRING_LENGTH];
    char individual_writers[MAX_STRING_LENGTH];
    char *description, *extension;
    int  readerState, writerState;

    strcpy(supported_readers, "All Supported Files (");
    strcpy(individual_readers, "All Files (*)");
    strcpy(supported_writers, "All Supported Files (");
    strcpy(individual_writers, "All Files (*)");

    /*
    supported-str = "";
    individual-str = "";

    */ /* TODO: Stable Only (Settings Option)
    */ /* stable = 'S';
    */ /* unstable = 'S';

    */ /* Stable + Unstable
    stable = 'S';
    unstable = 'U';

    curFormat = 0;
    */
    for (i=0; i<numberOfFormats; i++) {
        extension = formatTable[i].extension;
        description = formatTable[i].description;
        readerState = formatTable[i].reader_state;
        writerState = formatTable[i].writer_state;
        printf("%s%s%d%d", extension, description,
            readerState, writerState);

        /*
        upper-ext = extension.upper()
        supported-str = "*" + upper-ext + " "
        individual-str = upper-ext.replace(".", "") + " - " + description + " (*" + extension + ")"
        if(readerState == stable or readerState == unstable) {
            */ /* Exclude color file formats from open dialogs
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
    printf("%d", isFloating);
    /* tb = sender()
    if (tb) {
        if (isFloating) { */
            /* */ /* TODO: Determine best suited close button on various platforms.
            */ /* Style-SP-DockWidgetCloseButton
            */ /* Style-SP-TitleBarCloseButton
            */ /* Style-SP-DialogCloseButton */
/*
            ACTION = Action(tb.style().standard-icon("Style-SP-DialogCloseButton"), "Close", self)
        }
        else {
            for (action in tb.actions()) {
                if (action.object-name() == "toolbarclose") {
                    tb.remove-action(action)
                    */ /* disconnect(tb, SIGNAL(actionTriggered()), self, SLOT(close-toolbar()))
                    del action
                }
            }
        }
    } */
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
application_event(EmbEvent event)
{
    printf("%d", event.type);
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
    EmbColor color, int line_type, float line_weight)
{
    printf("%f\n",
        1.0 + pattern->stitchList->count
        + obj_pos.x + color.r + line_type + line_weight);
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
    QEmbTab*  EmbTab
    View*  gview
    int fileWasLoaded

    */ /* QPrinter printer

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
    printf("%s", file_name);
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
    printf("%s", file_name);
    /*
    return QFileInfo(file_name).suffix().toLower();
    */
}

void
on_window_activated(void/* mdi-window *subwindow */)
{
    debug_message("MdiWindow onWindowActivated()");
    /*
    mdi-win = w.mdi-window();
    if (mdi-win) {
        mdi-win.on-window-activated();
    }
    status_bar-SNAP.set_checked(gscene.property("ENABLE-SNAP"));
    status_bar-GRID.set_checked(gscene.property("ENABLE-GRID"));
    status_bar-RULER.set_checked(gscene.property("ENABLE-RULER"));
    status_bar[STATUS-ORTHO].set_checked(gscene.property("ENABLE-ORTHO"));
    status_bar[STATUS-POLAR].set_checked(gscene.property("ENABLE-POLAR"));
    status_bar[STATUS-QSNAP].set_checked(gscene.property("ENABLE-QSNAP"));
    status_bar[STATUS-QTRACK].set_checked(gscene.property("ENABLE-QTRACK"));
    status_bar[STATUS-LWT].set_checked(gscene.property("ENABLE-LWT"));
    */ /* mw.prompt.setHistory(promptHistory);
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
    printf("%d", color.r);
    /* curColor = color; */
}

void
current_line_type_changed(int type)
{
    printf("%d", type);
    /* curline_type = type; */
}

void
current_line_weight_changed(float weight)
{
    printf("%f", weight);
    /* curline_weight = weight; */
}

void
update_color_line_type_line_weight(void)
{
    debug_message("update color line type weight");
}

void
show_view_scroll_bars(float val)
{
    printf("%f\n", val);
    /* gview.showScrollBars(val) */
}

void
set_view_cross_hair_color(EmbColor color)
{
    crosshair_color = color;
    /* gview.setCrossHairColor(color) */
}

void
set_view_background_color(EmbColor color)
{
    background_color = color;
    /* gview.setBackgroundColor(color) */
}

void
set_viewGridcolor(EmbColor color)
{
    grid_color = color;
    /* gview.setGridColor(color) */
}

void
set_view_ruler_color(EmbColor color)
{
    ruler_color = color;
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

    */ /* Helper functions
    QToolButton*   createToolButton(const QString& iconName, const QString& txt)
    QLineEdit* createLineEdit(const QString& validatorType = "", int readOnly = false)
    QComboBox* create-combobox(int disable = false)
    Qfont-combobox* createfont-combobox(int disable = false)

void
update_lineedit_str_if_varies(QLineEdit* lineEdit, const QString& str)
{

}

void
update_lineedit_num_if_varies(QLineEdit* lineEdit, num, int useAnglePrecision)
{

}

void
update_font_combobox_str_if_varies(Qfont-combobox* font-combobox, const QString& str)
{

}

void
updateComboBoxStrIfVaries(QComboBox* comboBox, const QString& str, const QStringList& strList)
{

}

void
updateComboBoxintIfVaries(QComboBox* comboBox, int val, int yesOrNoText)
{
    QSignalMapper* signalMapper
}

void
mapSignal(QObject* fieldObj, const QString& name, QVariant value)
{
    QComboBox*   create-comboboxSelected()
    QToolButton* createToolButtonQSelect()
    QToolButton* createToolButtonPickAdd()

    */ /* TODO: Alphabetic/Categorized TabWidget
    bool eventFilter(QObject *obj, QEvent *event)
}

void
pickadd_modeToggled(void)
{

}

void
set_selected_items(int *itemList)
{
    int i;
    for (i=0; itemList[i]>=0; i++) {
        selected_items[i] = itemList[i];
    }
    n_selected = i;
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

void
create_groupbox(int data)
{
    printf("%d", data);
    /*
    for (i=0; i<; i++) {
        button = create_tool_button("blank", translate(label[i]));
        edit = create_line_edit("float", 0);
        add_to_form(button, edit);
    }
    */
}

void
event_filter(int obj, EmbEvent event)
{
    printf("%d", obj+event.type);
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
    button-QSelect.set_icon(load-icon(quickselect-xpm));
    button-QSelect.setIconSize(QSize(iconSize, iconSize));
    button-QSelect.setText("QSelect");
    button-QSelect.setToolTip("QSelect"); */ /* TODO: Better Description
    button-QSelect.setbutton-Style(Qt-button-IconOnly);
    return button-QSelect;
    */
}

void
create_button_pick_add(void)
{
/*
    */ /* TODO: Set as PickAdd or PickNew based on settings
    button_pickadd = Qbutton-(this);
    update_pick_add_mode_button(pickAdd);
    connect(button_pickadd, SIGNAL(clicked(int)), this, SLOT(togglepickadd_mode()));
    return button_pickadd;
    */
}

void
update_pick_add_mode_button(int pickadd_mode)
{
    printf("%d", pickadd_mode);
    /*
    pickAdd = pickadd_mode;
    if (pickAdd) {
        button_pickadd.set_icon(load-icon(pickadd-xpm));
        button_pickadd.setIconSize(QSize(iconSize, iconSize));
        button_pickadd.setText("PickAdd");
        button_pickadd.setToolTip("PickAdd Mode - Add to current selection.\nClick to switch to PickNew Mode.");
        button_pickadd.setbutton-Style(Qt-button-IconOnly);
    }
    else {
        button_pickadd.set_icon(load-icon(picknew-xpm));
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
update_edit_str_if_varies(int edit, char *str)
{
    printf("%d %s", edit, str);
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
    printf("%s %d %d", property, num, useAnglePrecision);
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
    */ /* (debug_message "old: %d %s, new: %d %s", oldIndex, qPrintable(font-combobox.currentText()), newIndex, qPrintable(str))
    if field-old-text.isEmpty()) {
        font-combobox.setCurrentFont(QFont(field-new-text))
        font-combobox.setProperty("FontFamily", field-new-text)
    }
    elif field-old-text != field-new-text) {
        if font-combobox.findText(field-varies-text) == -1:
            */ /* Prevent multiple entries
            font-combobox.addItem(field-varies-text)
        font-combobox.set_current_index(font-combobox.findText(field-varies-text))
    }
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
            */ /* Prevent multiple entries
            comboBox.addItem(field-varies-text)
        comboBox.set_current_index(comboBox.findText(field-varies-text))
}
*/

void
update_combobox_int_if_varies(int self, int comboBox, int val, int yesOrNoText)
{
    printf("%d %d %d %d", self, comboBox, val, yesOrNoText);
    /*
    field_old_text = comboBox.currentText();
    if (yesOrNoText) {
        if (val) {
            field_new_text = fieldYesText;
        }
        else {
            field_new_text = fieldNoText;
        }
    }
    else {
        if (val) {
            field_new_text = fieldOnText;
        }
        else {
            field_new_text = fieldOffText;
        }
    }

    if (strlen(field_old_text)==0) {
        if (yesOrNoText) {
            comboBox.addItem(fieldYesText, 1);
            comboBox.addItem(fieldNoText, 0);
        }
        else {
            comboBox.addItem(fieldOnText, 1);
            comboBox.addItem(fieldOffText, 0);
        }

        comboBox.set_current_index(comboBox.findText(field-new-text));
    }

    elif field-old-text != field-new-text:
        */ /* Prevent multiple entries
        if comboBox.findText(field-varies-text) == -1:
            comboBox.addItem(field-varies-text)
        comboBox.set_current_index(comboBox.findText(field-varies-text))
    */
}

void
show_groups(int obj_type)
{
    printf("%d", obj_type);
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
    printf("%d %d", self, index);
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
    */ /* NOTE: Do not clear comboBoxTextSingleFont
    comboBoxTextSingleFont.setProperty("FontFamily", "")
}


void
create-groupbox-geometry self obj-type)
{
    gb = Qgroupbox(translate("Geometry"), this)

    */ /* TODO: use proper icons
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
create_button(self, char *iconName, char *txt)
{
    
    tb = Qbutton-(this);
    tb.set_icon(load-icon(blank-xpm));
    tb.setIconSize(QSize(iconSize, iconSize));
    tb.setText(txt);
    tb.setbutton-Style(propertyEditorButtonStyle);
    tb.setStyleSheet("border:none");
    return tb;
}

void
create_editor(int validatorType, int readOnly)
{
    le = Qedit-(this);
    if validatorType == "int":
        le.setValidator(QIntValidator(le));
    elif validatorType == "float":
        le.setValidator(QfloatValidator(le));
    le.setReadOnly(readOnly);
    return le;
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
                    p.setX(ARC-CENTER-X].text().tofloat())
                    tempArcObj.setPos(p)

            if objName == "edit-ArcCenterY":
                tempArcObj = item
                if tempArcObj:
                    p = tempArcObj.objectCenter()
                    p.setY(ARC-CENTER-Y].text().tofloat())
                    tempArcObj.setPos(p)

            if objName == "edit-ArcRadius":
                tempArcObj = item
                if tempArcObj:
                    tempArcObj.setObjectRadius(ARC-RADIUS].text().tofloat())

            if objName == "edit-ArcStartAngle":
                tempArcObj = item
                if tempArcObj:
                    tempArcObj.setObjectStartAngle(ARC-START-ANGLE].text().tofloat())

            if objName == "edit-ArcEndAngle":
                tempArcObj = item
                if tempArcObj:
                    tempArcObj.setObjectEndAngle(ARC-END-ANGLE].text().tofloat())

        if (item.type == "Block":
            */ /* TODO: field editing
            break
        if (item.type == "Circle":
            if objName == "edit-CircleCenterX":
                p = item.objectCenter()
                p.setX(CIRCLE-CENTER-X].text().tofloat())
                item.setPos(p)

            if objName == "edit-CircleCenterY":
                tempCircleObj = item
                if tempCircleObj:
                    p = tempCircleObj.objectCenter()
                    p.setY(CIRCLE-CENTER-Y].text().tofloat())
                    tempCircleObj.setPos(p)

            if objName == "edit-CircleRadius":
                tempCircleObj = item
                if tempCircleObj:
                    tempCircleObj.setObjectRadius(CIRCLE-RADIUS].text().tofloat())

            if objName == "edit-CircleDiameter":
                tempCircleObj = item
                if tempCircleObj:
                    tempCircleObj.setObjectDiameter(CIRCLE-DIAMETER].text().tofloat())

            if objName == "edit-circle-area":
                tempCircleObj = item
                if tempCircleObj:
                    tempCircleObj.setObjectArea(CIRCLE-AREA].text().tofloat())
            if objName == "edit-CircleCircumference":
                tempCircleObj = item
                if tempCircleObj:
                    tempCircleObj.setObjectCircumference(CIRCLE-CIRCUMFERENCE].text().tofloat())

            break

        if (item.type == "DIMALIGNED":
            */ /* TODO: field editing
            break

        if (item.type == "DIMANGULAR":
            */ /* TODO: field editing
            break

        if (item.type == "DIMARCLENGTH":
            */ /* TODO: field editing
            break

        if (item.type == "DIMDIAMETER":
            */ /* TODO: field editing
            break

        if (item.type == "DIMLEADER":
            */ /* TODO: field editing
            break

        if (item.type == "DIMLINEAR": */ /* TODO: field editing
            break

        if (item.type == "DIMORDINATE": */ /* TODO: field editing
            break

        if (item.type == "DIMRADIUS": */ /* TODO: field editing
            break

        if (item.type == "ELLIPSE":
            if objName == "edit-ellipse-center-x":
                p = item.center()
                p.x = ELLIPSE-CENTER-X].text().tofloat()
                item.setPos(p)

            if objName == "edit-ellipseCenterY":
                p = item.center()
                p.y = "ELLIPSE-CENTER-Y].text().tofloat()
                item.setPos(p)

            if objName == "edit-EllipseRadiusMajor":
                item.setObjectRadiusMajor("ELLIPSE-RADIUS-MAJOR].text().tofloat())

            if objName == "edit-EllipseRadiusMinor":
                item.set_radius-minor(edit--ELLIPSE-RADIUS-MINOR.text().tofloat())

            if objName == "edit-EllipseDiameterMajor":
                item.setObjectDiameterMajor(edit--ELLIPSE-DIAMETER-MAJOR.text().tofloat())

            if objName == "edit-EllipseDiameterMinor":
                item.setObjectDiameterMinor("ELLIPSE-DIAMETER-MINOR].text().tofloat())

            break
        elif IMAGE: */ /* TODO: field editing
            break
        elif INFINITELINE: */ /* TODO: field editing
            break
        elif LINE:
            if objName == "edit-LineStartX":
                item.setObjectX1("LINE-START-X].text().tofloat())

            elif objName == "edit-LineStartY":
                item.setObjectY1(-"LINE-START-Y].text().tofloat())

            elif objName == "edit-LineEndX":
                item.setObjectX2("LINE-END-X].text().tofloat())

            elif objName == "edit-LineEndY":
                item.setObjectY2(-"LINE-END-Y].text().tofloat())

        if (item.type == "PATH":
            */ /* TODO: field editing

        if (item.type == "POINT":
            if objName == "edit-PointX":
                item.setObjectX("POINT-X].text().tofloat())

            elif objName == "edit-PointY":
                item.setObjectY(-"POINT-Y].text().tofloat())

        if (item.type == "POLYGON":
            debug_message("TYPE Polygon has no field editing")

        if (item.type == "Polyline":
            debug_message("TYPE Polyline has no field editing")

        if (item.type == "RAY":
            */ /* TODO: field editing
            debug_message("TYPE Polyline has no field editing")

        if (item.type == "RECTANGLE":
            */ /* TODO: field editing
            debug_message("TYPE Polyline has no field editing")

        if (item.type == "TEXTMULTI":
            */ /* TODO: field editing
            debug_message("TYPE Polyline has no field editing")

        if (item.type == "TEXTSINGLE":
            */ /* TODO: field editing
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
                    tempTextSingleObj.setTextSize(edit-TextSingleHeight.text().tofloat())

            if objName == "edit-TextSingleRotation":
                tempTextSingleObj = static-cast<TextSingle*>(item)
                if tempTextSingleObj:
                    tempTextSingleObj.setRotation(-edit-TextSingleRotation.text().tofloat())

            if objName == "edit-TextSingleX":
                tempTextSingleObj = static-cast<TextSingle*>(item)
                if tempTextSingleObj:
                    tempTextSingleObj.setX(edit-TextSingleX.text().tofloat())

            if objName == "edit-TextSingleY":
                tempTextSingleObj = static-cast<TextSingle*>(item)
                if tempTextSingleObj:
                    tempTextSingleObj.setY(edit-TextSingleY.text().tofloat())

            if objName == "comboBoxTextSingleBackward":
                if comboBoxTextSingleBackward.currentText() != field-varies-text:
                    tempTextSingleObj = static-cast<TextSingle*>(item)
                    if tempTextSingleObj:
                        tempTextSingleObj.setTextBackward(comboBoxTextSingleBackward.itemData(comboBoxTextSingleBackward.currentIndex()))

            if objName == "comboBoxTextSingleUpsideDown":
                if comboBoxTextSingleUpsideDown.currentText() != field-varies-text:
                    item.setTextUpsideDown(comboBoxTextSingleUpsideDown.itemData(comboBoxTextSingleUpsideDown.currentIndex()))

    */ /* Block this slot from running twice since calling setSelectedItems will trigger it
    blockSignals = 1

    widget = QApplication-focusWidget()
    */ /* Update so all fields have fresh data
    */ /* TODO: Improve this

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
add_colors_to_combobox(int combobox)
{
    printf("%d\n", combobox);
    /*
    add_item(combobox, load-icon(colorred-xpm), translate("Red"), tk.Rgb(255, 0, 0));
    add_item(combobox, load-icon(coloryellow-xpm), translate("Yellow"), tk.Rgb(255,255, 0));
    combobox.add_item(load-icon(colorgreen-xpm), translate("Green"), tk.Rgb(  0,255, 0));
    combobox.add_item(load-icon(colorcyan-xpm), translate("Cyan"), tk.Rgb(  0,255,255));
    combobox.add_item(load-icon(colorblue-xpm), translate("Blue"), tk.Rgb(  0, 0,255));
    combobox.add_item(load-icon(colormagenta-xpm), translate("Magenta"), tk.Rgb(255, 0,255));
    combobox.add_item(load-icon(colorwhite-xpm), translate("White"), tk.Rgb(255,255,255));
    */ /* TODO: Add Other... so the user can select custom colors */
}

/* . */
void
combobox_language_current_index_changed(char *lang)
{
    printf("%s\n", lang);
    /*
    dialog-general-language = lang.toLower();
    */
}

/* . */
void
combobox_icon_theme_current_index_changed(char *theme)
{
    printf("%s\n", theme);
    /*
    strcpy(dialog-general-icon-theme, theme);
    */
}

/* . */
void
combobox_icon_size_current_index_changed(int index)
{
    printf("%d\n", index);
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
    printf("%d\n", checked);
    /*
    preview.general-mdi-bg-use-logo = checked;
    mdi_area.useBackgroundLogo(checked);
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

        */ /* Update immediately so it can be previewed
        mdi_area.setBackgroundLogo(accept-.general-mdi-bg-logo) */
}

/* . */
void
checkbox_general_mdi_bg_use_texture_state_changed(int checked)
{
    preview_mdi_bg_use_texture = checked;
    /*
    mdi_area.useBackgroundTexture(checked);
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
    preview_mdi_bg_use_color = checked;
    /*
    mdi_area.useBackgroundColor(checked);
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
            button.set_icon(pix);
            mdi_area.setBackgroundColor(Color(accept-.general-mdi-bg-color))
        }
        else {
            mdi_area.setBackgroundColor(Color(dialog_general_mdi_bg_color))
        }
    } */
}

/* . */
void
currentGeneralMdiBackgroundColorChanged(EmbColor color)
{
    printf("%d", color.r);
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
    preview_show_scrollbars = checked;
/*
    updateAllViewScrollBars(preview.display-show-scrollbars) */
}

/* . */
void
spinboxzoom_scale_action_in_value_changed(float value)
{
    dialog_display_zoom_scale_action_in = value;
}

/* . */
void
spinboxzoom_scale_action_out_value_changed(float value)
{
    dialog_display_zoom_scale_action_out = value;
}

/* . */
void
checkboxDisableBG_state_changed(int checked)
{
    dialog_printing_disable_bg = checked;
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
            button.set_icon(pix)
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
    preview_display_crosshair_color = color;
/*
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
            button.set_icon(pix)
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
    preview_display_bg_color = color;
    /* updateAllViewBackgroundColors(preview.display-bg-color); */
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
            button.set_icon(pix);
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
currentDisplaySelectBoxLeftColorChanged(EmbColor color)
{
    preview_display_selectbox_left_color = color;
    update_all_view_selectbox_colors(
        preview_display_selectbox_color_left,
        preview_display_selectbox_fill_left,
        preview_display_selectbox_color_right,
        preview_display_selectbox_fill_right,
        preview_display_selectbox_alpha
        );
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
            button.set_icon(pix);
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
    preview_display_selectbox_left_fill = color;
    update_all_view_selectbox_colors(
        preview_display_selectbox_color_left,
        preview_display_selectbox_fill_left,
        preview_display_selectbox_color_right,
        preview_display_selectbox_fill_right,
        preview_display_selectbox_alpha);
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
            button.set_icon(pix);
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
    preview_display_selectbox_right_color = color;
    /* update_all_view_selectbox_colors(
        "preview-display-selectbox"); */
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
            button.set_icon(pix);
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
current_display_selectbox_right_fill_changed(EmbColor color)
{
    preview_display_selectbox_right_fill = color;
    /* update_all_view_selectbox_colors("preview-display-selectbox"); */
}


/* . */
void
spinbox_display_selectbox_alpha_value_changed(float value)
{
    preview_display_selectbox_alpha = value;
    update_all_view_selectbox_colors(
        accept_display_selectbox_color_left,
        accept_display_selectbox_fill_left,
        accept_display_selectbox_color_right,
        accept_display_selectbox_fill_right,
        accept_display_selectbox_alpha);
}

/* . */
void
checkbox_custom_filter_state_changed(int checked)
{
    printf("%d", checked);
    /*
    int checkbox = sender();
    if (checkbox) {
        format = checkbox.text();
        debug_message("CustomFilter: %s %d", tk.Printable(format), checked);
        if (checked) {
            opensave_custom_filter.append(" *." + format.toLower());        }
        else {
            opensave_custom_filter.remove("*." + format, tk.t-CaseInsensitive);
        }
        */ /* dialog-opensave_custom_filter = checked */ /* TODO
    }
    */
}

/* . */
void
button_custom_filter_select_all_clicked(void)
{
    /*
    buttonCustomFilterSelectAll(1);
    opensave_custom_filter = "supported";
    */
}

/* . */
void
button_custom_filter_clear_all_clicked(void)
{
    /*
    buttonCustomFilterClearAll(0);
    opensave_custom_filter.clear();
    */
}

/* .
*/
void
checkboxGridColorMatchCrossHair_state_changed(int checked)
{
    int dialog_grid_color_match_crosshair = checked;
    /*
    if (dialog_grid_color_match_crosshair) {
        update_all_view_grid_colors(accept-.display-crosshair-color);
    }
    else {
        update_all_view_grid_colors(accept-.grid-color);
    }

    sender_obj = sender();
    if (!sender_obj) {
        return
    }

    parent = sender_obj.parent();
    if (!parent) {
        return;
    }

    labelGridColor = get_widget("labelGridColor");
    if labelGridColor) {
        labelGridColor.enabled = !dialog_grid_color_match_crosshair);
    }

    buttonGridColor = get_widget("buttonGridColor");
    if buttonGridColor) {
        buttonGridColor.enabled = !dialog_grid_color_match_crosshair);
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
            accept.grid-color = color-dialog-selectedColor().rgb();
            pix = Image(16, 16);
            pix.fill(Color(accept-.grid-color));
            button.set_icon(pix);
            update_all_view_grid_colors(accept-.grid-color);
        }
        else {
            update_all_view_grid_colors(dialogGridcolor);
        }
    }
    */
}

/* . */
void
current_grid_color_changed(EmbColor color)
{
    preview_grid_color = color;
    /*
    (update_all_viewGridcolors "preview");
    */
}

/* . */
void
checkbox_grid_load_from_file_state_changed(int checked)
{
    printf("%d", checked);
    /*
    int dialog_grid_load_from_file = checked;

    sender_obj = sender();
    if (!sender_obj) {
        return

    parent = sender_obj.parent();
    if (!parent) {
        return;
    }

    labelGridtype = get_widget("labelGridtype");
    if (labelGridtype) {
        labelGridtype.enabled = !dialog_grid_load_from_file);
    }

    combobox_grid_type = get_widget("combobox_grid_type");
    if combobox_grid_type) {
        combobox_grid_type.enabled = !dialog_grid_load_from_file);
    }

    checkbox_grid_center_on_origin = get_widget("checkbox_grid_center_on_origin");
    if (checkbox_grid_center_on_origin) {
        checkbox_grid_center_on_origin.enabled = !dialog_grid_load_from_file);
    }

    label_grid_center_x = get_widget("label_grid_center_x");
    if (label_grid_center_x) {
        label_grid_center_x.enabled = !dialog_grid_load_from_file && ! dialog_grid_center_on_origin);
    }

    spinboxGridCenterX = get_widget("spinboxGridCenterX");
    if (spinboxGridCenterX) {
        spinboxGridCenterX.enabled = !dialog_grid_load_from_file && ! dialog_grid_center_on_origin);
    }

    labelGridcenter-y = get_widget("labelGridcenter-y");
    if (labelGridcenter-y) {
        labelGridcenter-y.enabled = !dialog_grid_load_from_file && ! dialog_grid_center_on_origin);
    }

    spinboxGridcenter-y = get_widget("spinboxGridcenter-y");
    if (spinboxGridcenter-y) {
        spinboxGridcenter-y.enabled = !dialog_grid_load_from_file && ! dialog_grid_center_on_origin);
    }

    labelGridsizeX = get_widget("labelGridsizeX");
    if labelGridsizeX) {
        labelGridsizeX.enabled = !dialog_grid_load_from_file);
    }

    spinboxGridsizeX = get_widget("spinboxGridsizeX");
    if spinboxGridsizeX) {
        spinboxGridsizeX.enabled = !dialog_grid_load_from_file);
    }

    labelGridsizeY = get_widget("labelGridsizeY")
    if labelGridsizeY) {
        labelGridsizeY.enabled = !dialog_grid_load_from_file);
    }

    spinbox_grid_size_y  = get_widget("spinbox_grid_size_y ")
    if (spinbox_grid_size_y ) {
        spinbox_grid_size_y .enabled = !dialog_grid_load_from_file);
    }

    label_grid_spacing_x = get_widget("label_grid_spacing_x")
    if (label_grid_spacing_x) {
        label_grid_spacing_x.enabled = !dialog_grid_load_from_file)

    spinbox_grid_spacing_x = get_widget("spinbox_grid_spacing_x")
    if (spinbox_grid_spacing_x) {
        spinbox_grid_spacing_x.enabled = !dialog_grid_load_from_file);
    }

    labelGridSpacingY = get_widget("labelGridSpacingY")
    if (labelGridSpacingY) {
        labelGridSpacingY.enabled = !dialog_grid_load_from_file);
    }

    spinboxGridSpacingY = get_widget("spinboxGridSpacingY")
    if spinboxGridSpacingY) {
        spinboxGridSpacingY.enabled = !dialog_grid_load_from_file);
    }

    labelGridsizeRadius = get_widget("labelGridsizeRadius")
    if labelGridsizeRadius) {
        labelGridsizeRadius.enabled = !dialog_grid_load_from_file);
    }

    spinboxGridsizeRadius = get_widget("spinboxGridsizeRadius")
    if spinboxGridsizeRadius) {
        spinboxGridsizeRadius.enabled = !dialog_grid_load_from_file);
    }

    labelGridSpacingRadius = get_widget("labelGridSpacingRadius")
    if labelGridSpacingRadius) {
        labelGridSpacingRadius.enabled = !dialog_grid_load_from_file);
    }

    spinboxGridSpacingRadius = get_widget("spinboxGridSpacingRadius")
    if spinboxGridSpacingRadius) {
        spinboxGridSpacingRadius.enabled = !dialog_grid_load_from_file);
    }

    labelGridSpacingAngle = get_widget("labelGridSpacingAngle")
    if labelGridSpacingAngle) {
        labelGridSpacingAngle.enabled = !dialog_grid_load_from_file);
    }

    spinboxGridSpacingAngle = get_widget("spinboxGridSpacingAngle")
    if spinboxGridSpacingAngle) {
        spinboxGridSpacingAngle.enabled = !dialog_grid_load_from_file);
    }
    */
}

int get_widget(int w, char *tag);
void set_group_visibility(int w, TABLE(group), int visibility);

/* TODO: add loop to find widget */
int
get_widget(int w, char *tag)
{
    return -1;
}

void
set_group_visibility(int w, TABLE(group), int visibility)
{
    int i;
    for (i=0; strcmp(group[i][0], "END"); i++) {
        int widget_index = get_widget(w, group[i][0]);
        if (widget_index > 0) {
            windows[w]->widgets[widget_index].visibility = visibility;
        }
    }
}

/* .
 */
void
combobox_grid_type_current_index_changed(int type)
{
    int dialog_grid_type = type;

    if (type == CIRCULAR_GRID) {
        set_group_visibility(MAIN_WINDOW, grid_group_rectangular, 0);
        set_group_visibility(MAIN_WINDOW, grid_group_circular, 1);
    }
    else {
        set_group_visibility(MAIN_WINDOW, grid_group_rectangular, 0);
        set_group_visibility(MAIN_WINDOW, grid_group_circular, 1);
    }

}

/*
 */
void
checkbox_grid_center_on_origin_state_changed(int checked)
{
    printf("%d", checked);
    /*
    dialog_grid_center_on_origin = checked

    sender_obj = sender()
    if (!sender_obj) {
        return

    parent = sender_obj.parent();
    if (!parent) {
        return
    }

    label_grid_center_x = get_widget("label_grid_center_x");
    if label_grid_center_x) {
        label_grid_center_x.enabled = !dialog_grid_center_on_origin);
    }

    spinboxGridCenterX = get_widget("spinboxGridCenterX");
    if spinboxGridCenterX) {
        spinboxGridCenterX.enabled = !dialog_grid_center_on_origin);
    }

    labelGridcenter-y = get_widget("labelGridcenter-y");
    if labelGridcenter-y) {
        labelGridcenter-y.enabled = !dialog_grid_center_on_origin);
    }

    spinboxGridcenter-y = get_widget("spinboxGridcenter-y");
    if spinboxGridcenter-y) {
        spinboxGridcenter-y.enabled = !dialog_grid_center_on_origin);
    }
*/
}

void
combobox_ruler_metric_current_index_changed(int index)
{
    printf("%d", index);
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
        button.set_icon(pix);
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
    preview_ruler_color = color;
    /*
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
    printf("%d", index);
    /*
    combobox = sender()
    default_color = tk.Rgb(255,255,0)
    */ /* Yellow
    if combobox) {
        dialog-qsnap-locator-color, ok = combobox.itemData(index).toUInt()
        if (!ok) {
            dialog-qsnap-locator-color = default_color
    else) {
        dialog-qsnap-locator-color = default_color
    */
}

/* . */
void
slider_qsnap_locator_size_value_changed(float value)
{
    dialog_qsnap_locator_size = value;
}

/* . */
void
slider_qsnap_aperture_size_value_changed(float value)
{
    dialog_qsnap_aperture_size = value;
}

void
checkbox_lwt_show_lwt_state_changed(int checked)
{
    preview_lwt_show_lwt = checked;
    /*
    if (preview_lwt_show_lwt) {
        enable_lwt();
    }
    else {
        disable_lwt();
    }

    sender_obj = sender();
    if (sender_obj) {
        parent = sender_obj.parent();
        if (parent) {
            checkboxRealRender = get_widget("checkboxRealRender");
            if checkboxRealRender) {
                checkboxRealRender.enabled = preview.lwt-show-lwt);
            }
        }
    }
    */
}

void
checkbox_lwt_real_render_state_changed(int checked)
{
    preview_lwt_real_render = checked;
    if (preview_lwt_real_render) {
        enableReal();
    }
    else {
        disableReal();
    }
}

void
combobox_selection_cool_grip_color_current_index_changed(index)
{
    /* TODO: Alert user if color matched the display bg color
    EmbColor default_color = blue;
    if (sender) {
        dialog-selection-coolgrip-color, ok = combobox.item-data(index).toUInt()
        if (!ok) {
            dialog-selection-coolgrip-color = default_color;
		}
	}
    else {
        dialog-selection-coolgrip-color = default_color;
	}
    */
}

void
combobox_selection_hot_grip_color_current_index_changed(int index)
{
    /* TODO: Alert user if color matched the display bg color
    EmbColor default_color = red;
    if (sender) {
        dialog-selection-hotgrip-color, ok = combobox.item-data(index).toUInt()
        if (!ok) {
            dialog-selection-hotgrip-color = default_color;
		}
	}
    else {
        dialog-selection-hotgrip-color = default_color;
	}
    */
}

/* . */
void
accept_changes(void)
{
    /*
    for k in preview.keys()
        dialog[k] = preview[k]
    for k in accept.keys()
        dialog[k] = preview[k]

    if dialog_grid_color_match_crosshair) {
        dialogGridcolor = accept-display-crosshair-color
    */

    /* Make sure the user sees the changes applied immediately. */
    /*
    mdi_area.use_background_logo(dialog-general-mdi-bg-use-logo);
    mdi_area.use_background_texture(dialog-general-mdi-bg-use-texture);
    mdi_area.use_background_color(dialog-general-mdi-bg-use-color);
    mdi_area.set_background-logo(dialog-general-mdi-bg-logo);
    mdi_area.set_background-texture(dialog-general-mdi-bg-texture);
    mdi_area.set_background-color(dialog_general_mdi_bg_color);
    icon-resize(dialog-general-icon-size);
    update_all_view-scrollBars(dialog_display_show-scrollbars);
    update_all_view-cross-hair-colors(dialog_display_crosshair-color);
    update_all_view-background-colors(dialog_display_bg-color);
    update_all_view_selectbox_colors(
        dialog_display_selectbox_color_left,
        dialog_display_selectbox_fill_left,
        dialog_display_selectbox_color_right,
        dialog_display_selectbox_fill_right,
        accept_display_selectbox_alpha);
    update_all_viewGridcolors(dialogGridcolor);
    update_all_view-ruler-colors(dialog-ruler-color);

    if (dialog_lwt_show_lwt) {
        enableLwt();
    }
    else {
        disableLwt();
    }

    if (dialog_lwt_real_render) {
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
    mdi_area.set_background-color(dialog_general_mdi_bg_color);
    update_all_view-scroll-bars(dialog_display_show-scrollbars);
    update_all_view-cross-hair-colors(dialog_display_crosshair-color);
    update_all_view-background-colors(dialog_display_bg-color);
    update_all_view_selectbox_colors(
        dialog_display_selectbox_left-color,
        dialog_display_selectbox_left-fill,
        dialog_display_selectbox_right-color,
        dialog_display_selectbox_right-fill,
        dialog_display_selectbox_alpha);
    update_all_viewGridcolors(dialogGridcolor);
    update_all_view-ruler-colors(dialog-ruler-color);

    if (dialog_lwt_show_lwt) {
        enable_lwt();
    }
    else {
        disable_lwt();
    }

    if (dialog_lwt_real_render) {
        enable_real();
    }
    else {
        disable_real();
    }

    reject();
    */
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
color_prompt(char *cmd)
{
    int output[3], valid, new-mode;
    valid = 0;
    new-mode = -1;
    if (cmd[0] == 'C' || !strcmp(cmd, "CROSSHAIR")) {
        new-mode = COLOR-CROSSHAIR;
    }
    if (cmd[0] == 'G' || !strcmp(cmd, "GRID")) {
        new-mode = COLOR-GRID;
    }
    if (cmd[0] == 'B' || !strcmp(cmd, "BACKGROUND")) {
        new-mode = COLOR-BACKGROUND;
    }
    if (color-mode != new-mode) {
        switch (new-mode) {
        case COLOR-BACKGROUND:
            color-mode = COLOR-BACKGROUND;
            set_prompt-prefix(translate("Specify background color: "));
            break;
        case COLOR-CROSSHAIR:
            color-mode = COLOR-CROSSHAIR;
            set_prompt-prefix(translate("Specify crosshair color: "));
            break;
        case COLOR-GRID:
            color-mode = COLOR-GRID;
            set_prompt-prefix(translate("Specify grid color: "));
            break;
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
    printf("%s", filename);
    /* debug_message("ImageWidget Constructor")
    img = img-load(filename)

    min-width = img.width()
    min-height = img.height()
    max-width = img.width()
    max-height = img.height()

    img.show() */
}

void
image_widget_load(char *filename)
{
    printf("%s", filename);
    /* return img.load(fileName) */
}

void
image_widget_save(char *filename)
{
    printf("%s", filename);
    /* return img.save(fileName, "PNG") */
}

/*
 *
 */
void
paint_event(EmbEvent event)
{
    printf("%d\n", event.type);
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
    */

    /* TODO: get actual thumbnail image from file, lets also use a size of 128x128 for now...
     * TODO: make thumbnail size adjustable thru settings dialog.
     */

    /* 
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
*/

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
    */ /* HACK: Take that QRubberBand!
    hack = size()
    resize(hack + QSize(1,1))
    resize(hack)
    */
    /* WARNING
     * DO NOT SET THE QMDISUBWINDOW (this) FOCUSPROXY TO THE PROMPT
     * AS IT WILL CAUSE THE WINDOW MENU TO NOT SWITCH WINDOWS PROPERLY!
     * ALTHOUGH IT SEEMS THAT SETTING INTERNAL WIDGETS FOCUSPROXY IS OK.

    */
    /*
    gview.setFocusProxy(mw.prompt);

    resize(sizeHint());

    curLayer = "0";
    curColor = 0; */
    /* TODO: color ByLayer */
    /* 
    curline_type = BYLAYER;
    curline_weight = BYLAYER;

    */
    /* Due to strange Qt4.2.3 feature the child window icon is not drawn
     * in the main menu if showMaximized() is called for a non-visible child window
     * Therefore calling show() first...
     */
    /*
    show()
    showMaximized()

    setFocusPolicy(WheelFocus)
    setFocus()

    onWindowActivated()
}
*/

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
EmbVector
center(void)
{
    EmbVector v;
    v.x = 0;
    v.y = 0;
    /* return map_to_scene(rect().center()); */
    return v;
}

/* . */
void
enter_event(EmbEvent event)
{
    printf("%d\n", event.type);
    /*
    mdi-win = mw;
    if (mdi-win) {
        EmbTab.setActiveSubWindow(mdi-win);
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
    */ /* NOTE: We really just remove the objects from the scene.
    */ /*       deletion actually occurs in the destructor.
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
    printf("%d %d, %d, %d, %d\n", clone, mode, x, y, data);
    /*
    preview-off();
    */ /* Free the old objects before creating ones

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
    preview_object_item_group = gscene.create-item-group(preview-object-list);

    if (preview-mode == PREVIEW-MODE-MOVE ||
        preview-mode == "PREVIEW-MODE-ROTATE" ||
        preview-mode == "PREVIEW-MODE-SCALE") {
        preview_point = Vector(x, y);
        */
        /* NOTE: Move: basePt
         * Rotate: basePt
         * Scale: basePt
         */
        /*
        preview-data = data;
        */
        /* NOTE: Move: unused
         * Rotate: refAngle
         * Scale: refFactor
         */
        /*
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

    if (preview_object_item_group) {
        gscene.remove-item(preview_object_item_group);
        del preview_object_item_group;
        preview_object_item_group = 0;
    }
    */

    preview_active = 0;

    scene_update();
}

void
enable_move_rapid_fire(void)
{
    debug_message("Enabling rapid move.");
    rapid_move_active = 1;
}

void
disable_move_rapid_fire(void)
{
    debug_message("Disabling rapid move.");
    rapid_move_active = 0;
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

void
update_mouse_coords(EmbVector position)
{
    printf("called with %f", position.x);
    /*
    EmbVector view_mouse_point = Vector(x, y);
    scene-mouse-point = to-emb-vector(map_to_scene(view_mouse_point));
    gscene.set_property("SCENE-QSNAP-POINT", scene-mouse-point);
    */

    /* TODO: if qsnap functionality is enabled,
     * use it rather than the mouse point.
     */

    /*
    gscene.set_property("SCENE-MOUSE-POINT", scene-mouse-point);
    gscene.set_property("view_mouse_point", view_mouse_point);
    statusbar.setMouseCoord(scene-mouse-point.x, -scene-mouse-point.y);
    */
}

/* 
 * NOTE:
 * "crosshair-size" is in pixels and is a percentage of your screen width
 * Example: (1280*0.05)/2 = 32, thus 32 + 1 + 32 = 65 pixel wide crosshair.
 */
void
set_crosshair_size(float percent)
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
    crosshair_color = color;
}

void
set_corner_button(void)
{
    /*
    num = display-scrollbar-widget-num;
    if (num) {
        cornerButton = tk.PushButton(void);
        cornerButton.setFlat(1);
        act = action-hash.value(num);
        */
        /* NOTE: Prevent crashing if the action is NULL. */ /*
        if (!act) {
            error-title = translate("Corner Widget Error")
            message = translate("There are unused enum values in COMMAND-ACTIONS. Please report self as a bug.")
            information(error-title, message)
            setCornerWidget(0)
        }
        else {
            cornerButton.set_icon(act.icon())
            */ /* connect(cornerButton, SIGNAL(clicked()), self, SLOT(cornerButtonClicked()))
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
mouse_float_click_event(EmbEvent event)
{
    printf("%d\n", event.type);
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
mouse_press_event(EmbEvent event)
{
    printf("%d\n", event.type);
    /*
    update_mouse_coords(event.x, event.y);
    if (event.button() == "LeftButton") {
        path = Path();
        pickList = gscene.items(Rect(map_to_scene(
            view_mouse_point.x-pickBoxSize,
            view_mouse_point.y-pickBoxSize),
            map_to_scene(view_mouse_point.x+pickBoxSize,
            view_mouse_point.y+pickBoxSize)));

        itemsInPickBox = pickList.size();
        if (itemsInPickBox && !selecting_active && !gripping-active) {
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
                if (gripRect.intersects(pickRect)) {
                    foundGrip = 1;
                }
                */
                /* If the pick point is within the item's grip box,
                    * start gripping.
                    */
                /*
                if (foundGrip) {
                    start_gripping(base);
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
        elif (!selecting_active) {
            selecting_active = 1;
            pressPoint = event.pos();
            scenePressPoint = map_to_scene(pressPoint);

            if (!select-box:
                select-box = select-box(QRubberBand-Rectangle);
            select-box.set_geometry(Rect(pressPoint, pressPoint));
            select-box.show();
        }
        else {
            selecting_active = 0;
            select-box.hide();
            releasePoint = event.pos();
            scene-release-point = map_to_scene(releasePoint);

            */ /* Start select-box Code
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
                            n_selected = 0;
                        }
                        else {
                            for item in item_list: {
                                item.set_selected(!item.is-selected()) */ /* Toggle selected;
                            }
                        }
                    }
                    else {
                       n_selected = 0;
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
                                n_selected = 0;


                        else {
                            for item in item_list:
                                item.set_selected(!item.is-selected());
                                */ /* Toggle selected

                    }
                    else {
                            n_selected = 0;

                        item_list = gscene.items(path, "intersects-item-shape");
                        for item in item_list) {
                            item.set_selected(1);
                        }
                   }
               }
            */ /* End select-box Code
        }

        if (pasting_active) {
            item_list = paste-object-item-group.childItems();
            gscene.destroy-item-group(paste-object-item-group);
            for (item in item_list) {
                gscene.remove-item(item);
                */ /* Prevent Qt Runtime Warning,
                */ /* QGraphicsScene-addItem: item has alreadelta-y been
                */ /* added to self scene
            }

            for (item in item_list) {
                if (item) {
                    debug_message("TODO: Loop bodelta-y");
                }
            }

            pasting_active = 0;
            selecting_active = 0;
        }

        if (zoom-window-active) {
            fit_in_view(path.bounding-rect(), Qt-KeepAspectRatio);
               n_selected = 0;

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
    Rect scene_rect, new_rect, rect;
    viewRect = make_rectangle(map_to_scene(rect().top-left()), map_to_scene(rect().bottom-right()));
    scene_rect = gscene.scene-rect();
    new_rect = viewRect.adjusted(
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

void
center_on(EmbVector center_point)
{
    printf("%f\n", center_point.x);
}

/*
 * center_on also updates the scrollbars,
 * which shifts things out of wack o-O
 */
void
center_at(EmbVector center_point)
{
    EmbVector offset, new_center, view_center;
    center_on(center_point);
    /* Reshift to the center */
    view_center = center();
    embVector_subtract(center_point, view_center, &offset);
    embVector_add(center_point, offset, &new_center);
    center_on(new_center);
}

/* center-on also updates the scrollbars, which shifts things out of wack o-O
 */
void
align_scene_point_with_view_point(EmbVector scene_point, EmbVector view_point)
{
    EmbVector point_before = scene_point;
    EmbVector view_center = center();
    center_on(view_center);
    /* Reshift to the center so the scene and view points align. */
    EmbVector point_after = map_to_scene(view_point);
    EmbVector offset, new_center;
    embVector_subtract(point_before, point_after, &offset);
    embVector_add(view_center, offset, &new_center);
    center_on(new_center);
}

void
event_accept(void)
{

}

void
mouse_move_event(EmbEvent event)
{
    /*
    EmbVector mouse = cursor_pos();
    update_mouse_coords(mouse.x, mouse.y);
    EmbVector move_point = event.mouse.position;
    EmbVector scene_move_point = map_to_scene(move_point);

    if (preview_active) {
        switch (preview_mode) {
        case PREVIEW_MODE_MOVE:
            preview_object_item_group.set_pos(scene-mouse-point - preview_point);
            break;
        case PREVIEW_MODE_ROTATE:
            mouse-angle = Line(preview_point.x, preview_point.y, scene-mouse-point.x, scene-mouse-point.y).angle();

            rad = radians(preview-data-mouse-angle);
            p = Vector(-preview_point.x, -preview_point.y);
            rot = rotate_vector(p, rad);
            rot.x += preview_point.x;
            rot.y += preview_point.y;

            preview_object_item_group.set_pos(rot.x, rot.y);
            preview_object_item_group.set_rotation(preview-data-mouse-angle);
            break;
        case PREVIEW_MODE_SCALE:
            scale_factor = preview-data;

            factor = Line(preview_point.x, preview_point.y, scene-mouse-point.x, scene-mouse-point.y).length() / scale_factor;

            preview_object_item_group.setScale(1);
            preview_object_item_group.set_pos(0,0);

            if (scale_factor <= 0.0) {
                critical(self, translate("scale_factor Error"), translate(the_greig_message));
            }
            else {
                EmbVector old, delta;
                old.x = 0.0;
                old.y = 0.0;
                EmbLine scale_line;
                scale_line.start = preview_point;
                scale_line.end = old; scale_line.set_length(embVector_lenth(scale-line)*factor);

                embVector_subtract(scale_line.point2, old, &delta);
            }
            break;
        default:
            break;
        }
    }
    preview_object_item_group.scale = preview_object_item_group.scale()*factor;
    moveBy(preview_object_item_group, delta);
    */

    if (pasting_active) {
        /*
        EmbVector v = scene-mouse-point.subtract(pasteDelta);
        paste-object-item-group.set_pos(v);
        */
    }

    if (moving_active) {
        /* Ensure that the preview is only shown if the mouse has moved. */
        if (!preview_active) {
            /* preview-on("PREVIEW-CLONE-SELECTED", "PREVIEW-MODE-MOVE",
                   scenePressPoint.x, scenePressPoint.y, 0)  */
        }
    }

    if (selecting_active) {
        /*
        if (scenemove-point.x >= scenePressPoint.x(void)) {
            select-box.setDirection(1);
        }

        else { select-box.setDirection(0);
        select-box.set_geometry(
            make_rectangle(map-from-scene(scenePressPoint), event.pos()).normalized());
        */
        event_accept();
    }

    if (panning_active) {
        /*
        horizontal-scroll-bar().value = 
            horizontal-scroll-bar().value()
             - (event.x - pan_start.x);
        vertical-scroll-bar().value = 
            vertical-scroll-bar().value()
             - (event.y - pan_start.y);
        pan_start = event.position;
        */
        event_accept();
    }

    scene_update();
}

void
mouse_release_event(EmbEvent event)
{
    printf("%d\n", event.type);
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

        event_accept();
    }

    if (event.button() == "MiddleButton") {
        panning_active = 0;
        */
        /* The Undo command will record the spot where the pan completed. */ /*
        event_accept();
    }

    if (event.button() == "XButton1") {
        debug_message("XButton1");
        main-undo() */
        /* TODO: Make this customizable */ /*
        event_accept();
    }

    if (event.button() == "XButton2") {
        debug_message("XButton2");
        main-redo() */
        /* TODO: Make this customizable */
        /*
        event_accept();
    }
    */
    scene_update();
}

void
wheel_event(EmbEvent event)
{
    float zoom_dir = 1.0;
    printf("%d\n", event.type);
    /* zoom_dir = event.pixel-delta().y; */
    /* TODO: float check self */
    /*mousePoint = event.global-pos() */
    /* TODO: this is causing weird versioning errors, */
    /* this appears to be supported on Qt5.12. */
    /* update_mouse_coords(mousePoint.x, mousePoint.y) */
    if (zoom_dir > 0) {
        debug_message(".");
    }
    else {
        debug_message(".");
    }
}

void
context_menu_event(EmbEvent event)
{
    int i;
    TABLE(context_menu);
    TABLE(all_entries);
    int entry = 0;
    load_csv(all_entries, "assets/context_menu.csv");
    printf("%d\n", event.type);
    /*
    iconTheme = general-icon-theme;

    menu = "";
    */

    for (i=0; i<n_selected; i++) {
        /*
        if (selected_items[i].type != "Not Set") {
            selection_empty = 0;
            break;
        }
        */
    }

    if (pasting_active) {
        return;
    }


    for (i=0; strcmp(all_entries[i][0], "END"); i++) {
        int show = !strcmp(all_entries[i][2], "ALWAYS_VISIBLE");
        show = show || (!strcmp(all_entries[i][2], "SELECTED") && (n_selected > 0));
        show = show || (!strcmp(all_entries[i][2], "ZOOM") && zoom_window_active);
        if (show) {
			strcpy(context_menu[entry][0], all_entries[i][0]);
			strcpy(context_menu[entry][1], all_entries[i][1]);
			entry++;
            continue;
        }
    }

    /* menu.exec(event.globalPos()); */
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
    if (pasting_active) {
        /*
        gscene.remove-item(paste-object-item-group);
        del paste-object-item-group;
        */
    }

    pasting_active = 0;
    zoom_window_active = 0;
    selecting_active = 0;
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
    if (pasting_active) {
        /*
        gscene.remove-item(paste-object-item-group)
        del paste-object-item-group
        */
    }

    pasting_active = 0;
    zoom_window_active = 0;
    selecting_active = 0;
    /*
    select-box.hide()
    if (gripping_active) {
        stop_gripping(0);
    }
    else {
          n_selected = 0;

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

    if (active_view) {
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

/* 
 *  Select box.
 */

/* Default values.
 */
void
select_box_init(int tools)
{
    printf("%d", tools);
    /*
    tools->left-brush.color = "#FFFFFF";
    tools->left-brush.style = BRUSH-STYLE-SOLID;
    tools->right-brush.color = "#FFFFFF";
    tools->right-brush.style = BRUSH-STYLE-SOLID;
    tools->left-pen.color = "#FFFFFF";
    tools->left-pen.style = LINE-STYLE-SOLID;
    tools->right-pen.color = "#FFFFFF";
    tools->right-pen.style = LINE-STYLE-SOLID;
    tools->alpha = 0xFF;
    tools->box-dir = 0;
    select-box-direction(tools, tools->box-dir);
    */
    /* not sure what this line was for */
    /*
    select_box_colors("darkGreen", "green", "darkBlue", "blue", 32);
    */
}

/* Choose between the left an right tools.
 */
void
select_box_direction(int tools, int dir)
{
    printf("%d %d\n", tools, dir);
    /*
    tools->box-dir = dir
    if (dir == DIRECTION-RIGHT) {
        tools->dir-pen = tools->right-pen
        tools->dir-brush = tools->right-brush
    }
    else {
        tools->dir-pen = tools->left-pen
        tools->dir-brush = tools->left-brush
    }
    */
}

void
set_view_select_box_colors(
    EmbColor colorL,
    EmbColor fillL,
    EmbColor colorR,
    EmbColor fillR,
    float alpha)
{
    printf("%d\n", colorL.r);
    printf("%d\n", fillL.r);
    printf("%d\n", colorR.r);
    printf("%d\n", fillR.r);
    printf("alpha %f\n", alpha);
     /* gview.setSelectBoxColors(colorL, fillL, colorR, fillR, alpha) */
}

/* TODO: allow customization.
 * 
 */
void
select_box_colors(Toolset *tools, TABLE(state), char *setting)
{
    char setting_str[100];
    debug_message("SelectBox colors()");
    sprintf(setting_str, "%s-alpha", setting);
    tools->alpha = get_float(state, setting_str);

    sprintf(setting_str, "%s-color-left", setting);
    tools->left_pen.color = get_color(state, setting_str);
    tools->left_pen.style = LINE_STYLE_DASHED;

    sprintf(setting_str, "%s-fill-left", setting);
    tools->left_brush.color = get_color(state, setting_str);
    tools->left_brush.style = BRUSH_STYLE_SOLID;

    sprintf(setting_str, "%s-color-right", setting);
    tools->right_pen.color = get_color(state, setting_str);
    tools->right_pen.style = LINE_STYLE_SOLID;

    sprintf(setting_str, "%s-fill-right", setting);
    tools->right_brush.color = get_color(state, setting_str);
    tools->right_brush.style = BRUSH_STYLE_SOLID;

    /*
    direction(tools->box_dir);

    select_box_force_repaint();
    */
}

/* Carry out a paint given the current pen and brush.
 * May need the canvas passing in.
 */
void
select_box_paint_event(int tools, EmbEvent event)
{
    Painter *painter = create_painter();
    printf("%d %d %d\n", tools, event.type, painter->pen->cosmetic);
    /*
    painter->pen->dir_pen = 0;
    painter.fill-rect(0, 0, width()-1, height()-1, dir-brush);
    painter.draw-rect(0, 0, width()-1, height()-1);
    */
}

/* HACK: Take that QRubberBand! */
void
select_box_force_repaint(void)
{
    /*
    hack = size();
    resize(hack + (1, 1));
    resize(hack);
    */
}

char *
translate(char *string)
{
    int i;
    for (i=0; strcmp(translation_tables[language][i][0], "END"); i++) {
        if (!strcmp(translation_tables[language][i][0], string)) {
            return translation_tables[language][i][1];
        }
    }
    return string;
}
