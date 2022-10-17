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
 *  actions
 */

#include "em2.h"

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
    set_int("menu-state", FILE_MENU);
}

void
display_edit_menu(void)
{
    set_int("menu-state", EDIT_MENU);
}

void
display_view_menu(void)
{
    set_int("menu-state", VIEW_MENU);
}

void
display_settings_menu(void)
{
    set_int("menu-state", SETTINGS_MENU);
}

void
display_window_menu(void)
{
    set_int("menu-state", WINDOW_MENU);
}

void
display_help_menu(void)
{
    set_int("menu-state", HELP_MENU);
}

void
display_recent_menu(void)
{
    set_int("menu-state", RECENT_MENU);
}

void
display_zoom_menu(void)
{
    set_int("menu-state", ZOOM_MENU);
}

void
display_pan_menu(void)
{
    set_int("menu-state", PAN_MENU);
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

