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

#include "imgui.h"
#include "backends/imgui_impl_glfw.h"
#include "backends/imgui_impl_opengl2.h"
#define STB_IMAGE_IMPLEMENTATION
#include "stb/stb_image.h"

#include "embroidermodder.h"

inline ImVec2 to_ImVec2(EmbVector v)
{
    return ImVec2(v.x, v.y);
}

/* Description is a a simple string table of path instructions,
 * 
 */
int
render_pattern(EmbPattern *p)
{
    ImDrawList* draw_list = ImGui::GetWindowDrawList();
    if (p->circles) {
        for (int i=0; i<p->circles->count; i++) {
            EmbCircle c = p->circles->circle[i];
            EmbColor thisColor = c.color;
            //setCurrentColor(qRgb(thisColor.r, thisColor.g, thisColor.b));
            //NOTE: With natives, the Y+ is up and libembroidery Y+ is up, so inverting the Y is NOT needed.
            // , false, OBJ_RUBBER_OFF
            int color = IM_COL32(c.color.r, c.color.g, c.color.b, 255);
            draw_list->AddCircle(to_ImVec2(c.center), c.radius, color);
            // TODO: fill
        }
    }
    if (p->ellipses) {
        for (int i=0; i<p->ellipses->count; i++) {
            EmbEllipse e = p->ellipses->ellipse[i];
            EmbColor thisColor = e.color;
            //setCurrentColor(qRgb(thisColor.r, thisColor.g, thisColor.b));
            // NOTE: With natives, the Y+ is up and libembroidery Y+ is up, so inverting the Y is NOT needed.
            //draw_list->AddEllipse(e.center.x, e.center.y, e.radius.x, e.radius.y, 0, false, OBJ_RUBBER_OFF);
            // TODO: rotation and fill
        }
    }
    if (p->lines) {
        for (int i=0; i<p->lines->count; i++) {
            EmbLine li = p->lines->line[i];
            EmbColor thisColor = li.color;
            //setCurrentColor(qRgb(thisColor.r, thisColor.g, thisColor.b));
            // NOTE: With natives, the Y+ is up and libembroidery Y+ is up, so inverting the Y is NOT needed.
            //draw_list->AddLine(ImVec2(li.start.x, li.start.y), ImVec2(li.end.x, li.end.y), 0, OBJ_RUBBER_OFF);
            // TODO: rotation
        }
    }
    if (p->paths) {
        //TODO: This is unfinished. It needs more work
        for (int i=0; i<p->paths->count; i++) {
            //QPainterPath pathPath;
            EmbArray *curPointList = p->paths->path[i].pointList;
            EmbColor thisColor = p->paths->path[i].color;
            /*
            if (curPointList) {
                EmbPoint pp = curPointList->point[0];
                pathPath.moveTo(pp.position.x, -pp.position.y);
                //NOTE: Qt Y+ is down and libembroidery Y+ is up, so inverting the Y is needed.
            }
            for (int j=0; j<curPointList->count; j++) {
                EmbPoint pp = curPointList->point[j];
                pathPath.lineTo(pp.position.x, -pp.position.y);
                // NOTE: Qt Y+ is down and libembroidery Y+ is up, so inverting the Y is needed.
            }

            QPen loadPen(qRgb(thisColor.r, thisColor.g, thisColor.b));
            loadPen.setWidthF(0.35);
            loadPen.setCapStyle(Qt::RoundCap);
            loadPen.setJoinStyle(Qt::RoundJoin);

            PathObject* obj = new PathObject(0,0, pathPath, loadPen.color().rgb());
            obj->setObjectRubberMode(OBJ_RUBBER_OFF);
            gscene->addItem(obj);
            */
        }
    }
    if (p->points) {
        for (int i=0; i<p->points->count; i++) {
            EmbPoint po = p->points->point[i];
            EmbColor thisColor = po.color;
            //setCurrentColor(qRgb(thisColor.r, thisColor.g, thisColor.b));
            // NOTE: With natives, the Y+ is up and libembroidery Y+ is up, so inverting the Y is NOT needed.
            double point_size = 10.0;
            draw_list->AddCircle(ImVec2(po.position.x, po.position.y), point_size, IM_COL32(0, 0, 0, 255));
        }
    }
    if (p->polygons) {
        for (int i=0; i<p->polygons->count; i++) {
            //QPainterPath polygonPath;
            bool firstPoint = false;
            double startX = 0, startY = 0;
            double x = 0, y = 0;
            /*
            EmbArray *curPointList = p->polygons->polygon[i].pointList;
            EmbColor thisColor = p->polygons->polygon[i].color;
            setCurrentColor(qRgb(thisColor.r, thisColor.g, thisColor.b));
            for (int j=0; j<curPointList->count; j++) {
                EmbPoint pp = curPointList->point[j];
                x = pp.position.x;
                y = -pp.position.y;
                // NOTE: Qt Y+ is down and libembroidery Y+ is up, so inverting the Y is needed.

                if (firstPoint) { polygonPath.lineTo(x,y); }
                else           { polygonPath.moveTo(x,y); firstPoint = true; startX = x; startY = y; }
            }

            polygonPath.translate(-startX, -startY);
            nativeAddPolygon(startX, startY, polygonPath, OBJ_RUBBER_OFF);
            */
        }
    }
    /* NOTE: Polylines should only contain NORMAL stitches. */
    if (p->polylines) {
        for (int i=0; i<p->polylines->count; i++) {
            /*
            EmbPolyline pl = p->polylines->polyline[i];
            QPainterPath polylinePath;
            bool firstPoint = false;
            double startX = 0, startY = 0;
            double x = 0, y = 0;
            EmbArray *curPointList = pl.pointList;
            EmbColor thisColor = pl.color;
            setCurrentColor(qRgb(thisColor.r, thisColor.g, thisColor.b));
            for (int j=0; j<curPointList->count; j++) {
                EmbPoint pp = curPointList->point[j];
                x = pp.position.x;
                y = -pp.position.y;
                // NOTE: Qt Y+ is down and libembroidery Y+ is up, so inverting the Y is needed.

                if (firstPoint) {
                    polylinePath.lineTo(x,y);
                }
                else {
                    polylinePath.moveTo(x,y);
                    firstPoint = true;
                    startX = x;
                    startY = y;
                }
            }

            polylinePath.translate(-startX, -startY);
            draw_list->AddPolyline(startX, startY, polylinePath, OBJ_RUBBER_OFF);
            */
        }
    }
    if (p->rects) {
        for (int i=0; i<p->rects->count; i++) {
            /*
            EmbRect r = p->rects->rect[i];
            EmbColor thisColor = r.color;
            setCurrentColor(qRgb(thisColor.r, thisColor.g, thisColor.b));
            //NOTE: With natives, the Y+ is up and libembroidery Y+ is up, so inverting the Y is NOT needed.
            draw_list->AddRectangle(r.left, r.top, r.right - r.left, r.bottom - r.top, 0, false, OBJ_RUBBER_OFF); //TODO: rotation and fill
            */
        }
    }
    if (p->stitchList->count > 1) {
        for (int i = 1; i<p->stitchList->count; i++) {
            EmbStitch prev = p->stitchList->stitch[i-1];
            EmbStitch st = p->stitchList->stitch[i];
            ImVec2 start = {prev.x, prev.y};
            ImVec2 end = {st.x, st.y};
            EmbThread thread = p->thread_list[st.color];
            int color = IM_COL32(thread.color.r, thread.color.g, thread.color.b, 255);
            draw_list->AddLine(start, end, color);
        }
    }
    return 0;
}

void
draw_rulers(void)
{
    ImDrawList* draw_list = ImGui::GetWindowDrawList();
    draw_list->AddRectFilled(
        ImVec2(0, 100),
        ImVec2(50, 640),
        IM_COL32(150, 170, 0, 255)
    );
    draw_list->AddRectFilled(
        ImVec2(0, 100),
        ImVec2(480, 150),
        IM_COL32(150, 170, 0, 255)
    );
    for (int i=0; i<20; i++) {
        draw_list->AddRectFilled(
            ImVec2(50+40*i, 100),
            ImVec2(50+40*i+1, 150),
            IM_COL32(0, 0, 0, 255)
        );
        for (int j=0; j<10; j++) {
            draw_list->AddRectFilled(
                ImVec2(50+40*i+4*j, 130),
                ImVec2(50+40*i+4*j+1, 150),
                IM_COL32(0, 0, 0, 255)
            );
        }
    }
    for (int i=0; i<10; i++) {
        draw_list->AddRectFilled(
            ImVec2(0, 150+40*i),
            ImVec2(50, 150+40*i+1),
            IM_COL32(0, 0, 0, 255)
        );
        for (int j=0; j<10; j++) {
            draw_list->AddRectFilled(
                ImVec2(30, 150+40*i+4*j),
                ImVec2(50, 150+40*i+4*j+1),
                IM_COL32(0, 0, 0, 255)
            );
        }
    }
}

void
draw_grid(void)
{
    ImDrawList* draw_list = ImGui::GetWindowDrawList();
    for (int i=0; i<10; i++) {
        draw_list->AddRectFilled(
            ImVec2(150+40*i, 150),
            ImVec2(150+40*i+1, 640),
            IM_COL32(0, 0, 0, 255)
        );
        draw_list->AddRectFilled(
            ImVec2(50, 150+40*i),
            ImVec2(640, 150+40*i+1),
            IM_COL32(0, 0, 0, 255)
        );
    }
}

void
pattern_view(void)
{
    EmbPattern *pattern = pattern_list[pattern_index];
    if (show_rulers) {
        draw_rulers();
    }
    if (show_grid) {
        draw_grid();
    }
    render_pattern(pattern);
}


#if 0
QPainterPath
add_to_path(QPainterPath path, char type, EmbVector offset, float *data, float scale)
{
    if (type == 'm') {
        path.moveTo(offset.x+scale*data[0], offset.y-scale*data[1]);
    }
    if (type == 'l') {
        path.lineTo(offset.x+scale*data[0], offset.y-scale*data[1]);
    }
    if (type == 'e') {
        path.addEllipse(EmbVector(offset.x+scale*data[0], offset.y-scale*data[1]),
            data[2]*scale, data[3]*scale);
    }
    if (type == 'a') {
        path.arcTo(offset.x+scale*data[0], offset.y-scale*data[1],
            data[2]*scale, data[3]*scale, data[4], data[5]);
    }
    return path;
}

QPainterPath
load_to_qpath(EmbVector offset, std::string str, float scale)
{
    QPainterPath path;

    char type = '0';
    int dot = 0;
    int index = 0;
    float position[6] = {0.0, 0.0, 0.0, 0.0, 0.0, 0.0};
    int first = 1;
    float sign = 1.0;
    for (size_t i=0; i<str.size(); i++) {
        char c = str[i];
        if ((c >= 'a') && (c <= 'z')) {
            dot = 0;
            index = 0;
            if (!first) {
                position[index] *= sign;
                path = add_to_path(path, type, offset, position, scale);
            }
            first = 0;
            type = c;
            sign = 1.0;
            for (int j=0; j<6; j++) {
                position[j] = 0.0;
            }
        }
        if ((c >= '0') && (c <= '9')) {
            int x = (int)(c - '0');
            if (dot) {
                position[index] = position[index] + pow(0.1, dot)*x;
                dot++;
            }
            else {
                position[index] = 10.0 * position[index] + 1.0*x;
            }
        }
        if (c == '-') {
            sign = -1.0;
        }
        if (c == '.') {
            dot = 1;
        }
        if (c == ',') {
            position[index] *= sign;
            index++;
            sign = 1.0;
            dot = 0;
        }
    }

    position[index] *= sign;
    path = add_to_path(path, type, offset, position, scale);

    return path;
}

void
createRulerTextPath(float x, float y, std::string str, float height)
{
    imdraw->AddText(ImVec2(x, y), 0xFFFFFFFF, str);
}
#endif
