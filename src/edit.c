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
 *
 *  Editor functions.
 */

#include "em2.h"

/* .
 */
void
text_italic(void)
{
    debug_message("text_italic()");
    /* text_style_italic = !text_style_italic; */
}

/* .
 */
void
text_bold(void)
{
    debug_message("text_bold()");
    /* text_style_bold = !text_style_bold; */
}

/* .
 */
void
text_strikeout(void)
{
    debug_message("text_strikeout()");
    /* text_style.strikeout = !text_style.strikeout; */
}

/* .
 */
void
text_underline(void)
{
    debug_message("text_underline()");
    /* text_style.underline = !text_style.underline; */
}

/* .
 */
void
text_overline(void)
{
    debug_message("text_overline()");
    /* text_style.overline = !text_style.overline; */
}

void
load_file_action(char *file_name)
{
    FILE *file;
    EmbPattern *p;
    debug_message("MdiWindow load-file()");
    /*
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
        debug_message("Reading file was unsuccessful: %s\n", file_name)
        mw.restore_override_cursor()
        message = translate("Reading file was unsuccessful: ") + file_name
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

        set_current-file(file_name)
        mw.statusbar.showMessage("File loaded.")
        stitches = ""
        stitches.setNum(stitchCount)

        if grid-load-from-file:
            #TODO: Josh, provide me a hoop size and/or grid spacing from the pattern.
            debug_message(".")

        mw.restore_override_cursor()

    p.free()

    # Clear the undo stack so it is not possible to undo past this point.
    undo-history-length = 0;

    set_current-color(tmpColor);
    return 1;
    */
}

void
display_file_menu(void)
{
    set_int(mainwnd->state, "menu-state", FILE_MENU);
}

void
display_edit_menu(void)
{
    set_int(mainwnd->state, "menu-state", EDIT_MENU);
}

void
display_view_menu(void)
{
    set_int(mainwnd->state, "menu-state", VIEW_MENU);
}

void
display_settings_menu(void)
{
    set_int(mainwnd->state, "menu-state", SETTINGS_MENU);
}

void
display_window_menu(void)
{
    set_int(mainwnd->state, "menu-state", WINDOW_MENU);
}

void
display_help_menu(void)
{
    set_int(mainwnd->state, "menu-state", HELP_MENU);
}

void
display_recent_menu(void)
{
    set_int(mainwnd->state, "menu-state", RECENT_MENU);
}

void
display_zoom_menu(void)
{
    set_int(mainwnd->state, "menu-state", ZOOM_MENU);
}

void
display_pan_menu(void)
{
    set_int(mainwnd->state, "menu-state", PAN_MENU);
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
            updateComboBoxintIfVaries("arc-clockwise", obj.objectClockwise(), 1); /* dropdown */ /*
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

void
do_nothing(void)
{
    debug_message("Do nothing.");
}

/* Creates a new file using the standard dialog_
 * Adds an mdi_area.
 */
void
new_file(void)
{
    debug_message("New File.");
    int n_patterns = get_int(mainwnd->state, "n-patterns");
    mainwnd->pattern[n_patterns] = embPattern_create();
    set_int(mainwnd->state, "n-patterns", n_patterns+1);
    set_int(mainwnd->state, "tab-index", get_int(mainwnd->state, "tab-index")+1);
    set_int(mainwnd->state, "n-docs", get_int(mainwnd->state, "n-docs")+1);
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
    /*
    Application_setOverrideCursor("ArrowCursor");

    files = []
    preview = opensave_open_thumbnail
    open_filesPath = opensave_recent_directory

    #Check to see if self from the recent files list
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
        #TODO: set opendialog_selectNameFilter(const String& filter) from opensave_open_format
        #connect(openDialog, SIGNAL(filesSelected()), self, SLOT(open_files_selected()));
        opendialog_exec();
    }

    Application_restoreOverrideCursor(); */
}


/*
 * Instead of closing using exit() this allows us to chain any checks,
 * like the confirmation of the close, to the action.
 */
void
exit_program(void)
{
    debug_message("Closing Embroidermodder 2.0.");
    running = 0;
}

void
icon_16(void)
{
    debug_message("icon_16()");
}

void
icon_24(void)
{
    debug_message("icon_24()");
}

void
icon_32(void)
{
    debug_message("icon_32()");
}

void
icon_48(void)
{
    debug_message("icon_48()");
}

void
icon_64(void)
{
    debug_message("icon_64()");
}

void
icon_128(void)
{
    debug_message("icon_128()");
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
            int n_docs = get_int(mainwnd->state, "n-docs");
            
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
    EmbPattern *pattern;
    int format_type;
    debug_message("Save file()");
    /* debug_message(current_file_name); */

    format_type = emb_identify_format(get_str(mainwnd->state, "current-file-name"));
    if (format_type == EMBFORMAT_UNSUPPORTED) {
        debug_message("The format of the file is not supported.");
    }

    pattern = embPattern_create();
    if (!pattern) {
        debug_message("Could not allocate memory for embroidery pattern");
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
                # TODO: proper layer/line_type/line_weight
                # TODO: Improve precision, replace simplified
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
            # addDimOrdinate(pattern, item);
            debug_message(".");
            break;

        case OBJ_TYPE_DIMRADIUS":
            # addDimRadius(pattern, item)
            debug_message(".");
            break;

        case OBJ_TYPE_ELLIPSE":
            if formatType == EMBFORMAT_STITCHONLY:
                path = item.objectSavePath()
                to_polyline(pattern, item.objectCenter(), path.simplified(), "0", item.objectColor(), "CONTINUOUS", "BYLAYER")
                # TODO: proper layer/line_type/line_weight
                # TODO: Improve precision, replace simplified
            else {
                # TODO: ellipse rotation
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
            # addInfiniteLine(pattern, item)
            debug_message(".")
            break;

        case OBJ_TYPE_Line":
            if formatType == EMBFORMAT_STITCHONLY:
                to_polyline(pattern, item.objectEndPoint1(), item.objectSavePath(), "0", item.objectColor(), "CONTINUOUS", "BYLAYER")
                # TODO: proper layer/line_type/line_weight
            else {
                embPattern_addLineObjectAbs(pattern, item.objectX1(), item.objectY1(), item.objectX2(), item.objectY2())

        elif item.type == "Path":
            #TODO: Reimplement addPolyline() using the libembroidery C API
            #
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
                        P1 = path.element_at(i-1) # start point
                        P2 = path.element_at(i)   # control point
                        P3 = path.element_at(i+1) # control point
                        P4 = path.element_at(i+2) # end point

                        pattern.add_stitch_abs(P4.x, -P4.y, NORMAL)
                        # TODO: This is temporary
                        # TODO: Curved Polyline segments are always arcs

                pattern.AddStitchRel(0, 0, STOP)
                c = polyline_item.pen().color()
                pattern.add_color(c.red, c.green, c.blue, "", "")
            break;

        case OBJ_TYPE_Point":
            if formatType == "EMBFORMAT_STITCHONLY":
                to_polyline(pattern, item.objectPos(), item.objectSavePath(), "0", item.objectColor(), "CONTINUOUS", "BYLAYER")
                # TODO: proper layer/line_type/line_weight
            else {
                pattern.addPointObjectAbs(item.objectX(), item.objectY())
            break;

        case OBJ_TYPE_Polygon":
            to_polyline(pattern, item.objectPos(), item.objectSavePath(), "0", item.objectColor(), "CONTINUOUS", "BYLAYER")
            # TODO: proper layer/line_type/line_weight
            break;

        case OBJ_TYPE_Polyline":
            to_polyline(pattern, item.objectPos(), item.objectSavePath(), "0", item.objectColor(), "CONTINUOUS", "BYLAYER")
            # TODO: proper layer/line_type/line_weight
            break;

        case OBJ_TYPE_RAY:
            # addRay(pattern, item)
            debug_message(".")
            break;

        case OBJ_TYPE_RECTANGLE:
            if formatType == "EMBFORMAT_STITCHONLY":
                to_polyline(pattern, item.objectPos(), item.objectSavePath(), "0", item.objectColor(), "CONTINUOUS", "BYLAYER")
                # TODO: proper layer/line_type/line_weight
            else {
                # TODO: Review this at some point
                top_left = item.object_top_left()
                pattern.add_rect_object_abs(top_left.x, top_left.y, item.object_width(), item.object_height())
            break;

        case OBJ_TYPE_SLOT:
            debug_message(".");
            break;

        case OBJ_TYPE_SPLINE:
            # TODO: abstract bezier into geom-bezier... cubicBezierMagic(P1, P2, P3, P4, 0.0, 1.0, tPoints)
            debug_message(".");
            break;

        case OBJ_TYPE_TextMulti:
            # TODO: saving polygons, polylines and paths must be stable before we go here.
            debug_message(".")
            break;

        case OBJ_TYPE_TextSingle:
            # TODO: saving polygons, polylines and paths must be stable before we go here.

            # TODO: This needs to work like a path, not a polyline. Improve this
            
            if (formatType == EMBFORMAT_STITCHONLY) {
                path_list = item.objectSavePathList()
                for (path in path_list) {
                    to_polyline(pattern, item.objectPos(), path.simplified(),
                        "0", item.objectColor(), "CONTINUOUS", "BYLAYER")
                    # TODO: proper layer/line_type/line_weight
                    # TODO: Improve precision, replace simplified
                }
            }
            break;
        default:
            break;
        }
    }

    # TODO: handle EMBFORMAT_STCHANDOBJ also
    if formatType == "EMBFORMAT_STITCHONLY":
        pattern.move_polylines_to_stitch_list()
        # TODO: handle all objects like this

    write_successful = pattern.write_auto(file_name);
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
            # Save current bg
            brush = gview.backgroundBrush();
            # Save ink by not printing the bg at all
            gview.setBackgroundBrush(Qt_NoBrush);
            # Print, fitting the viewport contents into a full page
            gview.render(painter);
            # Restore the bg
            gview.setBackgroundBrush(brush);
        else {
            # Print, fitting the viewport contents into a full page
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
        undo_history_position += 1
        print("undo_history_position = %d\n", undo_history_position);
        print("undo_history_length = %d\n", undo_history_length);
        undo_call = undo_history[undo_history_position]
        #set reverse flag
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

        #Create the reverse action from the last action and apply with
        #the main actuator.
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
}


/* .
 */
void
help(void)
{
    debug_message("help()");
    /* 
    #display in a custom widget instead
    #Open the HTML Help in the default browser
    helpURL = QUrl ("file:///" + qApp.applicationDirPath() + "/help/doc-index.html");
    QDesktopServices_openUrl(helpURL);

    #TODO: self is how to start an external program. Use this elsewhere...
    #program = "firefox"
    #arguments = _help/commands.html
    #myProcess = Process();
    #myProcess.start(program, arguments); */
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
    # tk.dialog(_mainWin);
    # img = tk.ImageWidget(application_folder + "/images/logo-small.png");
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
    # text.setWordWrap(1);

    button = tk.Button(
        dialog,
        text="Oh, Yeah!",
        command=dialog_destroy);
    button.grid(row=2, column=1);

    dialog_mainloop();
    */
}


/* TODO: Make day vision color settings.
 */
void
day_vision(void)
{
    /*
    gview = self->active_view();
    if gview:
        gview->setBackgroundColor("#FFFFFF");
        gview->setCrossHairColor("#000000");
        gview->setGridColor("#000000");
        */
    debug_message("day_vision()");
}

/* TODO: Make night vision color settings.
 */
void
night_vision(void)
{
	/*
    gview = self->active_view();
    if gview:
        gview->setBackgroundColor("#000000");
        gview->setCrossHairColor("#FFFFFF");
        gview->setGridColor("#FFFFFF");
    */
    debug_message("night_vision()");
}

/*
Display the changelog stored in the config.json data file.

Not currently maintained.
*/
void
changelog(void)
{
    debug_message("changelog()");
    /*
    #display in a custom widget instead
    #
    #QUrl changelogURL("help/changelog.html");
    #QDesktopServices_openUrl(changelogURL);
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
    */ /* connect(button1, SIGNAL(triggered()), wizard_tip_of_the_day,
        SLOT(wizard_tip_of_the_day.close()));
    connect(button2, SIGNAL(triggered()), wizard_tip_of_the_day,
        SLOT(wizard_tip_of_the_day.close()));
    connect(button3, SIGNAL(triggered()), wizard_tip_of_the_day,
        SLOT(wizard_tip_of_the_day.close())); */ /*

    layout = tk.VBoxLayout(wizard_tip_of_the_day);
    layout.add_widget(img_banner);
    layout.add_strut(1);
    layout.add_widget(label_tip_of_the_day);
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
    debug_message("scm_check_for_updates()");
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
treble_clef(void)
{
    debug_message("scm_treble_clef()");
}

/* .
 */
void
path(void)
{
    debug_message("scm_path()");
}

/* .
 */
void
circle(void)
{
    debug_message("scm_circle()");
}

/* .
 */
void
line(void)
{
    debug_message("scm_line()");
}

/* .
 */
void
distance(void)
{
    debug_message("scm_distance()");
}

/* .
 */
void
dolphin(void)
{
    debug_message("scm_dolphin()");
}

/* .
 */
void
ellipse(void)
{
    debug_message("scm_ellipse()");
}

/* .
 */
void
delete_object(void)
{
    debug_message("scm_delete_object()");
}

/* .
 */
void
heart(void)
{
    debug_message("scm_heart()");
}

/* .
 */
void
locate_point(void)
{
    debug_message("scm_locate_point()");
}

/* .
 */
void
move(void)
{
    debug_message("scm_locate_point()");
}

/* .
 */
void
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
    return 1;*/
}

/* .
 */
void
allow_zoom_out(void)
{
	/*
    origin = map_to_scene(0, 0);
    corner = map_to_scene(width(), height())
    max_size = corner.subtract(origin);

    if max(max_size.x, max_size.y) > zoom_out_limit:
        debug_message("zoom_out limit reached. (limit=%.1f)" % zoom_out_limit);
        return 0;

    return 1; */
}

/* .
 */
void
zoom_in(void)
{
    debug_message("zoom_in()");
    debug_message("View zoom_in()");
	/*
    if (!allow_zoom_in()) {
        return;
    }

    set_override_cursor("Wait Cursor");
    cntr = map_to_scene(Vector(width()/2, height()/2));
    s = display_zoom_scale_in;
    scale(s, s);

    center_on(cntr);
    restore_override_cursor();
    */
}

/* .
 */
void
zoom_out(void)
{
    debug_message("zoom_out()");
    debug_message("View zoom_out()");
	/*
    if (!allow_zoom_out()
        return;

    set_override_cursor("Wait Cursor");
    cntr = map_to_scene(Vector(width()/2, height()/2));
    s = display_zoom_scale_out;
    scale(s, s);

    center_on(cntr);
    restore_override_cursor();
    */
}

/* . */
void
zoom_selected(void)
{
    debug_message("zoom_selected()");
	/*
    set_override_cursor("Wait Cursor");
    item_list = gscene.selected_items();
    selected_rect_path = Path();
    for (item in item_list) {
        selected_rect_path.add_polygon(item.map_to_scene(item.bounding_rect()));
    }

    selected_rect = selected_rectPath.bounding_rect()
    if (selected_rect) {
        message = translate("Preselect objects before invoking the zoom_selected command.")
        information(translate("zoom_selected Preselect"), message) */
        /* TODO: Support Post selection of objects */ /*
    }

    fit_in_view(selected_rect, "KeepAspectRatio")
    restore_override_cursor() */
}

/* .
 */
void
zoom_scale(void)
{
    debug_message("zoom_scale()");
    debug_message("Implement zoom_scale.");
}

/* .
 */
void
zoom_center(void)
{
    debug_message("zoom_center()");
    debug_message("Implement zoom_center.");
}

/* .
 */
void
zoom_all(void)
{
    debug_message("zoom_all()");
    debug_message("Implement zoom_all.");
}

/* .
 */
void
zoom_extents(void)
{
    debug_message("zoom_extents()");
	/*
    set_override_cursor("WaitCursor")
    extents = gscene.items_bounding_rect()
    if extents:
        extents.set_width(grid_size_x)
        extents.set_height(grid_size_y)
        extents.move_center(Vector(0, 0))

    fit_in_view(extents, "KeepAspectRatio")
    restore_override_cursor()
    */
}

void
zoom_real_time(void)
{
    debug_message("zoomRealtime()");
    debug_message("Implement zoomRealtime.");
}

void
zoom_previous(void)
{
    debug_message("zoomPrevious()");
    debug_message("Implement zoomPrevious.");
}

void
zoom_window(void)
{
    debug_message("zoom_window()");
    /*
    gview = active_view();
    if gview:
        gview->zoom_window(); */

	/*
    zoom_window_active = 1;
    selecting_active = 0;
    clear_selection();
    */
}

void
zoom_dynamic(void)
{
    debug_message("zoom_dynamic()");
    debug_message("Implement zoom_dynamic.");
}


void
zoom_to_point(EmbVector mouse_point, float zoom_dir)
{
    double s;
    /*
    point-before-scale(map_to_scene(mouse-point)) */

    /* Do The zoom */
    s = 1.0;
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

    update-mouse-coords(mouse-point.x, mouse-point.y);
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

/* .
 */
void
pan_real_time(void)
{
    debug_message("pan-real-time-action()");
    /* mainwnd->panning_real_time_active = 1; */
}

/* .
 */
void
pan_point(void)
{
    debug_message("pan-point-action()");
    /* mainwnd->panning_point_active = 1; */
}

/* .
 */
void
pan_left(void)
{
    debug_message("pan-left-action()");
    /*
    horizontal-scroll-bar().set_value(horizontal-scroll-bar().value() + pan-distance);
    update-mouse-coords(view-mouse-point.x(), view-mouse-point.y());
    scene_update();
    */
}

/* .
 */
void
pan_right(void)
{
    debug_message("pan-right-action()");
    /*
    horizontal-scroll-bar().set_value(horizontal-scroll-bar().value() - pan-distance);
    update-mouse-coords(view-mouse-point.x(), view-mouse-point.y());
    scene_update();
    */
}

/* .
 */
void
pan_up(void)
{
    debug_message("pan-up-action()");
    /*
    vertical-scroll-bar().set_value(vertical-scroll-bar().value() + pan-distance);
    update-mouse-coords(view-mouse-point.x(), view-mouse-point.y());
    scene_update();
    */
}

/* .
 */
void
pan_down(void)
{
    debug_message("pan-down-action()");
    /*
    vertical-scroll-bar().set_value(vertical-scroll-bar().value() - pan-distance);
    update-mouse-coords(view-mouse-point.x(), view-mouse-point.y());
    scene_update();
    */
}


void
window_next(void)
{
    debug_message("window_next()");
}

void
window_previous(void)
{
    debug_message("window_previous()");
}

/* Close editor window.
 */
void
window_close(void)
{
    debug_message("window_close()");
}

/* Tile editor windows.
 */
void
window_tile(void)
{
    debug_message("window_tile()");
}

/* Close all editor windows.
 */
void
window_close_all(void)
{
    debug_message("window_close_all()");
}

/* Cascade editor windows.
 */
void
window_cascade(void)
{
    debug_message("window_cascade()");
}


void
settings_snap(void)
{
    debug_message("snap settings tab");
}

void
settings_grid(void)
{
    debug_message("grid settings tab");
}

void
settings_ruler(void)
{
    debug_message("ruler settings tab");
}

void
settings_ortho(void)
{
    debug_message("settings ortho");
}

void
settings_polar(void)
{
    debug_message("settings polar");
}

void
settings_qsnap(void)
{
    debug_message("settings qsnap");
}

void
settings_qtrack(void)
{
    debug_message("settings qtrack");
}

void
settings_lwt(void)
{
    debug_message("settings lwt");
}

void
toggle(char state[2*MAX_VARIABLES][MAX_STRING_LENGTH], char *key)
{
    int value = get_int(mainwnd->state, key);
    set_int(mainwnd->state, key, !value);
}

void
toggle_grid(void)
{
    debug_message("StatusBarButton toggleGrid()");
    toggle(mainwnd->state, "show-grid");
}

void
toggle_ruler(void)
{
    debug_message("StatusBarButton toggleRuler()");
    toggle(mainwnd->state, "show-ruler");
}

void
toggle_ortho(void)
{
    debug_message("StatusBarButton toggleOrtho()");
    toggle(mainwnd->state, "show-ortho");
}

void
set_snap(int active)
{
    debug_message("View toggle-snap()");
    printf("%d\n", active);
    /*
    set_override_cursor("WaitCursor");
    #  TODO: finish this.
    gscene.set-property("ENABLE-SNAP", active);
    gscene.update();
    restore-override-cursor();
    */
}

void
toggle_track(void)
{
    debug_message("StatusBarButton toggleQTrack()");
    toggle(mainwnd->state, "track-mode");
}

void
toggle_lwt(void)
{
    debug_message("StatusBarButton toggleLwt()");
    toggle(mainwnd->state, "show-lwt");
}

/*
 * Switch to rendering all line weights more accurately,
 * so the effect of different thread weights
 * can be understood.
 */
void
enable_lwt(void)
{
    debug_message("StatusBarButton enableLwt()");
    set_int(mainwnd->state, "show-lwt", 1);
}

/*
 * Switch to rendering all line weights the same,
 * so the effect of different thread weights can be ignored.
 */
void
disable_lwt(void)
{
    debug_message("StatusBarButton disableLwt()");
    set_int(mainwnd->state, "show-lwt", 0);
}

/* Turn real rendering on and see the pattern as
 * an approximation of what the stitched embroidery
 * will look like.
 */
void
enable_real(void)
{
    debug_message("StatusBarButton enableReal()");
    set_int(mainwnd->state, "real-render", 1);
}

/*
 * Turn real rendering off and see the pattern
 * as collection of geometric primatives.
 */
void
disable_real(void)
{
    debug_message("StatusBarButton disableReal()");
    set_int(mainwnd->state, "real-render", 0);
}

