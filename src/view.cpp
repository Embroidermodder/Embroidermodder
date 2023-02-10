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

std::vector<View> views;
std::string current_view_state = "neutral";
unsigned int ruler_color = IM_COL32(150, 170, 0, 255);
unsigned int ticks_color = IM_COL32(0, 0, 0, 255);
unsigned int grid_color = IM_COL32(0, 0, 0, 255);
double zoomInLimit = 1.0e-10;
double zoomOutLimit = 1.0e10;
EmbVector grid_spacing = {40.0, 40.0};
float ruler_width = 50.0f;
float tick_depth = 30.0f;
float major_tick_seperation = 40.0f;
float minor_tick_seperation = 4.0f;
float needle_speed = 100.0;
float stitch_time = 0.1;

inline ImVec2 to_ImVec2(EmbVector v)
{
    return ImVec2(v.x, v.y);
}

inline int embColor_to_int(EmbColor c)
{
    return IM_COL32(c.r, c.g, c.b, 255);
}

ImVec2 operator+(const ImVec2 a, const ImVec2 b)
{
    return ImVec2(a.x+b.x, a.y+b.y);
}

ImVec2 operator-(const ImVec2 a, const ImVec2 b)
{
    return ImVec2(a.x-b.x, a.y-b.y);
}

int
render_pattern(EmbPattern *p)
{
    ImDrawList* draw_list = ImGui::GetWindowDrawList();
    View view = views[settings.pattern_index];
    if (p->circles) {
        for (int i=0; i<p->circles->count; i++) {
            EmbCircle c = p->circles->circle[i];
            int color = embColor_to_int(c.color);
            EmbVector scene_center;
            embVector_add(c.center, view.origin, &scene_center);
            double scene_radius = c.radius * view.scale;
            draw_list->AddCircle(to_ImVec2(scene_center), scene_radius, color);
            // TODO: fill
        }
    }
    if (p->ellipses) {
        for (int i=0; i<p->ellipses->count; i++) {
            EmbEllipse e = p->ellipses->ellipse[i];
            int color = embColor_to_int(e.color);
            //draw_list->AddEllipse(e.center.x, e.center.y, e.radius.x, e.radius.y, color);
        }
    }
    if (p->lines) {
        for (int i=0; i<p->lines->count; i++) {
            EmbVector scene_start, scene_end;
            EmbLine li = p->lines->line[i];
            int color = embColor_to_int(li.color);
            embVector_add(li.start, view.origin, &scene_start);
            embVector_add(li.end, view.origin, &scene_end);
            draw_list->AddLine(to_ImVec2(scene_start), to_ImVec2(scene_end), color);
        }
    }
    if (p->paths) {
        for (int i=0; i<p->paths->count; i++) {
            EmbPath path;
            EmbArray *curPointList = path.pointList;
            int color = embColor_to_int(path.color);
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
            loadPen.setCapStyle(RoundCap);
            loadPen.setJoinStyle(RoundJoin);

            PathObject* obj = new PathObject(0,0, pathPath, loadPen.color().rgb());
            obj->setObjectRubberMode(OBJ_RUBBER_OFF);
            gscene->addItem(obj);
            */
        }
    }
    if (p->points) {
        for (int i=0; i<p->points->count; i++) {
            EmbPoint po = p->points->point[i];
            int color = embColor_to_int(po.color);
            double point_size = 10.0;
            draw_list->AddCircle(to_ImVec2(po.position), point_size, color);
        }
    }
    if (p->polygons) {
        for (int i=0; i<p->polygons->count; i++) {
            // QPainterPath polygonPath;
            EmbPolygon polygon = p->polygons->polygon[i];
            bool firstPoint = false;
            EmbVector start = {0.0, 0.0};
            double x = 0.0, y = 0.0;
            EmbArray *curPointList = polygon.pointList;
            int color = embColor_to_int(polygon.color);
            for (int j=0; j<curPointList->count; j++) {
                /*
                EmbPoint pp = curPointList->point[j];
                x = pp.position.x;
                y = -pp.position.y;

                if (firstPoint) {
                    polygonPath.lineTo(x,y);
                }
                else {
                    polygonPath.moveTo(x,y);
                    firstPoint = true;
                    startX = x;
                    startY = y;
                }
                */
            }

            /*polygonPath.translate(-startX, -startY);
            nativeAddPolygon(startX, startY, polygonPath, OBJ_RUBBER_OFF); */
        }
    }
    /* NOTE: Polylines should only contain NORMAL stitches. */
    if (p->polylines) {
        for (int i=0; i<p->polylines->count; i++) {
            EmbPolyline pl = p->polylines->polyline[i];
            bool firstPoint = false;
            double startX = 0, startY = 0;
            double x = 0, y = 0;
            EmbArray *curPointList = pl.pointList;
            int color = embColor_to_int(pl.color);
            for (int j=0; j<curPointList->count; j++) {
                EmbPoint pp = curPointList->point[j];
                x = pp.position.x;
                y = -pp.position.y;
                /*
                if (firstPoint) {
                    polylinePath.lineTo(x,y);
                }
                else {
                    polylinePath.moveTo(x,y);
                    firstPoint = true;
                    startX = x;
                    startY = y;
                }
                */
            }

            /*
            polylinePath.translate(-startX, -startY);
            draw_list->AddPolyline(startX, startY, polylinePath, OBJ_RUBBER_OFF);
            */
        }
    }
    if (p->rects) {
        for (int i=0; i<p->rects->count; i++) {
            EmbRect r = p->rects->rect[i];
            int color = embColor_to_int(r.color);
            /*
            draw_list->AddRectangle(r.left, r.top, r.right - r.left, r.bottom - r.top, color); //TODO: rotation and fill
            */
        }
    }
    if (p->stitchList->count > 1) {
        for (int i = 1; i<p->stitchList->count; i++) {
            ImVec2 offset = ImGui::GetWindowPos();
            EmbStitch prev = p->stitchList->stitch[i-1];
            EmbStitch st = p->stitchList->stitch[i];
            ImVec2 start = {
                view.scale * prev.x + view.origin.x,
                - view.scale * prev.y + view.origin.y
            };
            ImVec2 end = {
                view.scale * st.x + view.origin.x,
                - view.scale * st.y + view.origin.y
            };
            EmbThread thread = p->thread_list[st.color];
            int color = embColor_to_int(thread.color);
            draw_list->AddLine(offset + start, offset + end, color);
        }
    }
    return 0;
}

void
render_shine(ImVec2 start, ImVec2 end, ImVec2 normal, float start_frac, float end_frac)
{
    ImDrawList* draw_list = ImGui::GetWindowDrawList();
    ImVec2 delta = end-start;
    ImVec2 shine_start = start + ImVec2(start_frac*delta.x, start_frac*delta.y);
    ImVec2 shine_end = start + ImVec2(end_frac*delta.x, end_frac*delta.y);
    draw_list->AddQuadFilled(
        shine_start+normal, shine_start-normal,
        shine_end-normal, shine_end+normal,
        IM_COL32(255, 255, 255, 20));    
}

void
real_render_pattern(EmbPattern *p)
{
    ImDrawList* draw_list = ImGui::GetWindowDrawList();
    View view = views[settings.pattern_index];
    if (p->stitchList->count > 1) {
        for (int i = 1; i<p->stitchList->count; i++) {
            ImVec2 offset = ImGui::GetWindowPos();
            EmbStitch prev = p->stitchList->stitch[i-1];
            EmbStitch st = p->stitchList->stitch[i];
            ImVec2 start = offset + to_ImVec2(view.origin) + ImVec2(
                view.scale * prev.x,
                - view.scale * prev.y
            );
            ImVec2 end = offset + to_ImVec2(view.origin) + ImVec2(
                view.scale * st.x,
                - view.scale * st.y
            );

            float lwt = 2.0;
            EmbVector delta;
            delta.x = prev.y - st.y;
            delta.y = prev.x - st.x;
            double length = embVector_length(delta);
            EmbVector normal_;
            embVector_normalize(delta, &normal_);
            normal_.x *= lwt;
            normal_.y *= lwt;
            ImVec2 normal = to_ImVec2(normal_);

            EmbThread thread = p->thread_list[st.color];
            int color = embColor_to_int(thread.color);
            draw_list->AddQuadFilled(start+normal, start-normal,
                end-normal, end+normal, color);
            render_shine(start, end, normal, 0.333*0.3, 0.333*0.7);
            render_shine(start, end, normal, 0.333*0.1, 0.333*0.9);
            render_shine(start, end, normal, 0.333*1.3, 0.333*1.7);
            render_shine(start, end, normal, 0.333*1.1, 0.333*1.9);
            render_shine(start, end, normal, 0.333*2.3, 0.333*2.7);
            render_shine(start, end, normal, 0.333*2.1, 0.333*2.9);
        }
    }
}

void
simulate_pattern(EmbPattern *p)
{
    ImDrawList* draw_list = ImGui::GetWindowDrawList();
    View view = views[settings.pattern_index];
    std::chrono::duration<float> duration = std::chrono::system_clock::now() - view.simulation_start;
    float time_passed = duration.count();
    ImVec2 offset = ImGui::GetWindowPos();
    int stitches = 0;
    if (p->stitchList->count > 1) {
        float current_time = 0.0;
        for (int i = 1; i<p->stitchList->count; i++) {
            EmbStitch prev = p->stitchList->stitch[i-1];
            EmbStitch st = p->stitchList->stitch[i];
            ImVec2 start = {
                view.scale * prev.x + view.origin.x,
                - view.scale * prev.y + view.origin.y
            };
            ImVec2 end = {
                view.scale * st.x + view.origin.x,
                - view.scale * st.y + view.origin.y
            };
            EmbThread thread = p->thread_list[st.color];
            int color = embColor_to_int(thread.color);
            EmbVector delta = {prev.x-st.x, prev.y-st.y};
            float time_for_stitch = stitch_time + embVector_length(delta)/needle_speed;
            current_time += time_for_stitch;
            stitches = i;
            if (current_time > time_passed) {
                end = {
                    start.x - (current_time-time_passed-time_for_stitch)*(end.x-start.x),
                    start.y - (current_time-time_passed-time_for_stitch)*(end.y-start.y)
                };
                draw_list->AddLine(offset + start, offset + end, color);
                break;
            }
            else {
                draw_list->AddLine(offset + start, offset + end, color);
            }
        }
    }

    ImGui::Begin("Controls");
    ImGui::SetWindowFontScale(1.5);
    ImGui::Text(("Stitch: " + std::to_string(stitches) + "/"
        + std::to_string(p->stitchList->count)).c_str());
    ImGui::Text(("Needle Speed: " + std::to_string(needle_speed)).c_str());
    ImGui::Text(("Stitch Time: " + std::to_string(stitch_time)).c_str());
    ImGui::Text(("Time Passed: " + std::to_string(time_passed)).c_str());

    if (ImGui::Button("Slower")) {
        
    }
    ImGui::SameLine();
    if (ImGui::Button("Faster")) {

    }
 
    if (ImGui::Button("Start")) {

    }
    ImGui::SameLine();
    if (ImGui::Button("Previous Stitch")) {

    }
    ImGui::SameLine();
    if (ImGui::Button("Next Stitch")) {

    }
    ImGui::SameLine();
    if (ImGui::Button("End")) {

    }
    ImGui::End();
}

/* TODO: labels for measurements.
 */
void
draw_rulers(void)
{
    ImGuiIO &io = ImGui::GetIO();
    ImVec2 offset = ImGui::GetWindowPos();
    ImVec2 screen_size = io.DisplaySize;
    ImDrawList* draw_list = ImGui::GetWindowDrawList();
    draw_list->AddRectFilled(
        offset,
        offset + ImVec2(ruler_width, screen_size.y),
        ruler_color
    );
    draw_list->AddRectFilled(
        offset,
        offset + ImVec2(screen_size.x, ruler_width),
        ruler_color
    );
    if (views[settings.pattern_index].metric) {
        int repeats = 100;
        for (int i=0; i<repeats; i++) {
            float ruler_pos = ruler_width + major_tick_seperation*i;
            draw_list->AddLine(
                offset + ImVec2(ruler_pos, 0.0),
                offset + ImVec2(ruler_pos, ruler_width),
                ticks_color
            );
            draw_list->AddText(
                offset+ImVec2(ruler_pos + 5.0, 0.0),
                ticks_color,
                std::to_string(i).c_str());
            for (int j=0; j<10; j++) {
                ruler_pos += major_tick_seperation/10.0;
                draw_list->AddLine(
                    offset + ImVec2(ruler_pos, tick_depth),
                    offset + ImVec2(ruler_pos, ruler_width),
                    ticks_color
                );
            }
        }
        for (int i=0; i<repeats; i++) {
            float ruler_pos = ruler_width + major_tick_seperation*i;
            draw_list->AddLine(
                offset + ImVec2(0.0, ruler_pos),
                offset + ImVec2(ruler_width, ruler_pos),
                ticks_color
            );
            draw_list->AddText(
                offset+ImVec2(5.0, ruler_pos),
                ticks_color,
                std::to_string(i).c_str());
            for (int j=0; j<10; j++) {
                ruler_pos += major_tick_seperation/10.0;
                draw_list->AddLine(
                    offset + ImVec2(tick_depth, ruler_pos),
                    offset + ImVec2(ruler_width, ruler_pos),
                    ticks_color
                );
            }
        }
    }
    else {
        int repeats = 100;
        for (int i=0; i<repeats; i++) {
            float ruler_pos = ruler_width + major_tick_seperation*i;
            draw_list->AddLine(
                offset + ImVec2(ruler_pos, 0.0),
                offset + ImVec2(ruler_pos, ruler_width),
                ticks_color
            );
            draw_list->AddText(
                offset+ImVec2(ruler_pos + 5.0, 0.0),
                ticks_color,
                std::to_string(i).c_str());
            for (int j=0; j<16; j++) {
                ruler_pos += major_tick_seperation/16.0;
                draw_list->AddLine(
                    offset + ImVec2(ruler_pos, tick_depth),
                    offset + ImVec2(ruler_pos, ruler_width),
                    ticks_color
                );
            }
        }
        for (int i=0; i<repeats; i++) {
            float ruler_pos = ruler_width + major_tick_seperation*i;
            draw_list->AddLine(
                offset + ImVec2(0.0, ruler_pos),
                offset + ImVec2(ruler_width, ruler_pos),
                ticks_color
            );
            draw_list->AddText(
                offset+ImVec2(0.0, ruler_pos),
                ticks_color,
                std::to_string(i).c_str());
            for (int j=0; j<16; j++) {
                ruler_pos += major_tick_seperation/16.0;
                draw_list->AddLine(
                    offset + ImVec2(tick_depth, ruler_pos),
                    offset + ImVec2(ruler_width, ruler_pos),
                    ticks_color
                );
            }
        }
    }
}

void
draw_grid(void)
{
    ImDrawList* draw_list = ImGui::GetWindowDrawList();
    ImGuiIO &io = ImGui::GetIO();
    ImVec2 offset = ImGui::GetWindowPos();
    ImVec2 screen_size = io.DisplaySize;
    for (float x=offset.x; x<screen_size.x; x+=grid_spacing.x) {
        draw_list->AddLine(
            ImVec2(x, offset.y),
            ImVec2(x, screen_size.y),
            grid_color);
    }
    for (float y=offset.y; y<screen_size.y; y+=grid_spacing.y) {
        draw_list->AddLine(
            ImVec2(offset.x, y),
            ImVec2(screen_size.x, y),
            grid_color);
    }
}

void
pattern_view(void)
{
    EmbPattern *pattern = views[settings.pattern_index].pattern;
    ImGui::BeginChild(views[settings.pattern_index].filename.c_str());
    if (views[settings.pattern_index].grid_mode) {
        draw_grid();
    }
    if (views[settings.pattern_index].lwt_mode) {
        real_render_pattern(pattern);
    }
    else {
        if (views[settings.pattern_index].simulate) {
            simulate_pattern(pattern);
        }
        else {
            render_pattern(pattern);
        }
    }
    if (views[settings.pattern_index].ruler_mode) {
        draw_rulers();
    }
    ImGui::EndChild();
}

View init_view(void)
{
    View view;
    view.origin.x = 400.0;
    view.origin.y = 300.0;
    view.scale = 5.0;
    view.grid_type = "RECTANGULAR";
    view.snap_mode = true;
    view.grid_mode = true;
    view.ruler_mode = true;
    view.ortho_mode = false;
    view.polar_mode = false;
    view.qsnap_mode = false;
    view.qtrack_mode = false;
    view.lwt_mode = false;
    view.metric = true;
    view.simulate = false;
    view.simulation_start = std::chrono::system_clock::now();
    view.text_font = "default";
    view.text_size = 16;
    view.text_angle = 0.0;
    view.text_style_bold = false;
    view.text_style_italic = false;
    view.text_style_underline = false;
    view.text_style_overline = false;
    view.text_style_strikeout = false;
    view.filename = "Untitled.dst";
    if (views.size() > 0) {
        view.filename = "Untitled" + std::to_string(views.size()) + ".dst";
    }
    view.undo_history_position = 0;
    view.pattern = embPattern_create();
    
    return view;
}

#if 0
void ImageWidget(std::string filename)
{
    debug_message("ImageWidget Constructor");

    img.load(filename);

    setMinimumWidth(img.width());
    setMinimumHeight(img.height());
    setMaximumWidth(img.width());
    setMaximumHeight(img.height());

    this->show();
}

bool imageWidget_load(std::string fileName)
{
    img.load(fileName);
    return true;
}

bool ImageWidget_save(std::string fileName)
{
    img.save(fileName, "PNG");
    return true;
}

void ImageWidget_paintEvent(void)
{
    QPainter painter(this);
    painter.setViewport(0, 0, width(), height());
    painter.setWindow(0, 0, width(), height());
    painter.drawImage(0, 0, img);
}

void LayerManager(MainWindow* mw, QWidget* parent)
{
    layerModel = new QStandardItemModel(0, 8, this);

    layerModelSorted = new QSortFilterProxyModel;
    layerModelSorted->setDynamicSortFilter(true);
    layerModelSorted->setSourceModel(layerModel);

    treeView = new QTreeView;
    treeView->setRootIsDecorated(false);
    treeView->setAlternatingRowColors(true);
    treeView->setModel(layerModelSorted);
    treeView->setSortingEnabled(true);
    treeView->sortByColumn(0, AscendingOrder);

    QVBoxLayout *mainLayout = new QVBoxLayout;
    mainLayout->addWidget(treeView);
    setLayout(mainLayout);

    setWindowTitle(tr("Layer Manager"));
    setMinimumSize(750, 550);

    layerModel->setHeaderData(0, Horizontal, translate("Name"));
    layerModel->setHeaderData(1, Horizontal, translate("Visible"));
    layerModel->setHeaderData(2, Horizontal, translate("Frozen"));
    layerModel->setHeaderData(3, Horizontal, translate("Z Value"));
    layerModel->setHeaderData(4, Horizontal, translate("Color"));
    layerModel->setHeaderData(5, Horizontal, translate("Linetype"));
    layerModel->setHeaderData(6, Horizontal, translate("Lineweight"));
    layerModel->setHeaderData(7, Horizontal, translate("Print"));

    addLayer("0", true, false, 0.0, qRgb(0,0,0), "Continuous", "Default", true);
    addLayer("1", true, false, 1.0, qRgb(0,0,0), "Continuous", "Default", true);
    addLayer("2", true, false, 2.0, qRgb(0,0,0), "Continuous", "Default", true);
    addLayer("3", true, false, 3.0, qRgb(0,0,0), "Continuous", "Default", true);
    addLayer("4", true, false, 4.0, qRgb(0,0,0), "Continuous", "Default", true);
    addLayer("5", true, false, 5.0, qRgb(0,0,0), "Continuous", "Default", true);
    addLayer("6", true, false, 6.0, qRgb(0,0,0), "Continuous", "Default", true);
    addLayer("7", true, false, 7.0, qRgb(0,0,0), "Continuous", "Default", true);
    addLayer("8", true, false, 8.0, qRgb(0,0,0), "Continuous", "Default", true);
    addLayer("9", true, false, 9.0, qRgb(0,0,0), "Continuous", "Default", true);

    for (int i = 0; i < layerModel->columnCount(); ++i)
        treeView->resizeColumnToContents(i);
}

void LayerManager_addLayer(
    std::string name,
    bool visible,
    bool frozen,
    double zValue,
    unsigned int color,
    std::string lineType,
    std::string lineWeight,
    bool print)
{
    layerModel->insertRow(0);
    layerModel->setData(layerModel->index(0, 0), name);
    layerModel->setData(layerModel->index(0, 1), visible);
    layerModel->setData(layerModel->index(0, 2), frozen);
    layerModel->setData(layerModel->index(0, 3), zValue);

    QPixmap colorPix(QSize(16,16));
    colorPix.fill(QColor(color));
    layerModel->itemFromIndex(layerModel->index(0, 4))->setIcon(QIcon(colorPix));
    layerModel->setData(layerModel->index(0, 4), QColor(color));

    layerModel->setData(layerModel->index(0, 5), lineType);
    layerModel->setData(layerModel->index(0, 6), lineWeight);
    layerModel->setData(layerModel->index(0, 7), print);
}

void View(MainWindow* mw, QGraphicsScene* theScene, QWidget* parent)
{
    mainWin = mw;
    gscene = theScene;

    setFrameShape(QFrame::NoFrame);

    // NOTE: This has to be done before setting mouse tracking.
    // TODO: Review OpenGL for Qt5 later
    // if (settings_display_use_opengl) {
    //     debug_message("Using OpenGL...");
    //     setViewport(new QGLWidget(QGLFormat(QGL::DoubleBuffer)));
    // }

    // NOTE: FullViewportUpdate MUST be used for both the GL and Qt renderers.
    // NOTE: Qt renderer will not draw the foreground properly if it isnt set.
    setViewportUpdateMode(QGraphicsview_FullViewportUpdate);

    panDistance = 10; // TODO: should there be a setting for this???

    setCursor(BlankCursor);
    horizontalScrollBar()->setCursor(ArrowCursor);
    verticalScrollBar()->setCursor(ArrowCursor);
    qsnapLocatorColor = settings_qsnap_locator_color;
    qsnapLocatorSize = settings_qsnap_locator_size;
    qsnapApertureSize = settings_qsnap_aperture_size;
    gripColorCool = settings_selection_coolgrip_color;
    gripColorHot = settings_selection_hotgrip_color;
    gripSize = settings_selection_grip_size;
    pickBoxSize = settings_selection_pickbox_size;
    setCrossHairColor(settings_display_crosshair_color);
    setCrossHairSize(settings_display_crosshair_percent);
    setGridColor(settings_grid_color);

    if (settings_grid_show_on_load) {
        createGrid(settings_grid_type);
    }
    else {
        createGrid("");
    }

    toggleRuler(settings_ruler_show_on_load);
    toggleReal(true); //TODO: load this from file, else settings with default being true

    grippingActive = false;
    rapidMoveActive = false;
    previewMode = PREVIEW_MODE_NULL;
    previewData = 0;
    previewObjectItemGroup = 0;
    pasteObjectItemGroup = 0;
    previewActive = false;
    pastingActive = false;
    movingActive = false;
    selectingActive = false;
    zoomWindowActive = false;
    panningRealTimeActive = false;
    panningPointActive = false;
    panningActive = false;
    qSnapActive = false;
    qSnapToggle = false;

    // Randomize the hot grip location initially so it's not located at (0,0)
    srand(time(NULL));
    sceneGripPoint = EmbVector(random_uniform()*1000, random_uniform()*1000);

    gripBaseObj = 0;
    tempBaseObj = 0;

    selectBox = new SelectBox(QRubberBand::Rectangle, this);
    selectBox->setColors(QColor(settings_display_selectbox_left_color),
                         QColor(settings_display_selectbox_left_fill),
                         QColor(settings_display_selectbox_right_color),
                         QColor(settings_display_selectbox_right_fill),
                         settings_display_selectbox_alpha);

    showScrollBars(settings_display_show_scrollbars);
    setCornerButton();

    undoStack = new QUndoStack(this);
    mainWin->dockUndoEdit->addStack(undoStack);

    installEventFilter(this);

    setMouseTracking(true);
    setBackgroundColor(settings_display_bg_color);
    //TODO: wrap this with a setBackgroundPixmap() function: setBackgroundBrush(QPixmap("images/canvas.png"));

    connect(gscene, SIGNAL(selectionChanged()), this, SLOT(selectionChanged()));
}

void view_free(void)
{
    //Prevent memory leaks by deleting any objects that were removed from the scene
    qDeleteAll(hashDeletedObjects.begin(), hashDeletedObjects.end());
    hashDeletedObjects.clear();

    //Prevent memory leaks by deleting any unused instances
    qDeleteAll(previewObjectList.begin(), previewObjectList.end());
    previewObjectList.clear();
}

void view_enterEvent(QEvent* /*event*/)
{
    QMdiSubWindow* mdiWin = qobject_cast<QMdiSubWindow*>(parent());
    if (mdiWin) {
        mainWin->mdiArea->setActiveSubWindow(mdiWin);
    }
}

void view_addObject(BaseObject* obj)
{
    gscene->addItem(obj);
    gscene->update();
    hashDeletedObjects.remove(obj->objID);
}

void view_deleteObject(BaseObject* obj)
{
    //NOTE: We really just remove the objects from the scene. deletion actually occurs in the destructor.
    obj->setSelected(false);
    gscene->removeItem(obj);
    gscene->update();
    hashDeletedObjects.insert(obj->objID, obj);
}

void view_previewOn(int clone, int mode, double x, double y, double data)
{
    debug_message("View previewOn()");
    previewOff(); //Free the old objects before creating new ones

    previewMode = mode;

    //Create new objects and add them to the scene in an item group.
    if (clone == PREVIEW_CLONE_SELECTED)
        previewObjectList = createObjectList(gscene->selectedItems());
    else if (clone == PREVIEW_CLONE_RUBBER)
        previewObjectList = createObjectList(rubberRoomList);
    else {
        return;
    }
    previewObjectItemGroup = gscene->createItemGroup(previewObjectList);

    if (previewMode == PREVIEW_MODE_MOVE   ||
       previewMode == PREVIEW_MODE_ROTATE ||
       previewMode == PREVIEW_MODE_SCALE) {
        previewPoint = EmbVector(x, y); //NOTE: Move: basePt; Rotate: basePt;   Scale: basePt;
        previewData = data;           //NOTE: Move: unused; Rotate: refAngle; Scale: refFactor;
        previewActive = true;
    }
    else {
        previewMode = PREVIEW_MODE_NULL;
        previewPoint = EmbVector();
        previewData = 0;
        previewActive = false;
    }

    gscene->update();
}

void view_previewOff()
{
    //Prevent memory leaks by deleting any unused instances
    qDeleteAll(previewObjectList.begin(), previewObjectList.end());
    previewObjectList.clear();

    if (previewObjectItemGroup) {
        gscene->removeItem(previewObjectItemGroup);
        delete previewObjectItemGroup;
        previewObjectItemGroup = 0;
    }

    previewActive = false;

    gscene->update();
}

void view_enableMoveRapidFire()
{
    rapidMoveActive = true;
}

void view_disableMoveRapidFire()
{
    rapidMoveActive = false;
}

bool view_allowRubber()
{
    //if (!rubberRoomList.size()) //TODO: this check should be removed later
        return true;
    return false;
}

void view_addToRubberRoom(QGraphicsItem* item)
{
    rubberRoomList.append(item);
    item->show();
    gscene->update();
}

void view_vulcanizeRubberRoom()
{
    foreach(QGraphicsItem* item, rubberRoomList)
    {
        BaseObject* base = static_cast<BaseObject*>(item);
        if (base) vulcanizeObject(base);
    }
    rubberRoomList.clear();
    gscene->update();
}

void view_vulcanizeObject(BaseObject* obj)
{
    if (!obj) return;
    gscene->removeItem(obj); //Prevent Qt Runtime Warning, QGraphicsScene::addItem: item has already been added to this scene
    obj->vulcanize();

    UndoableAddCommand* cmd = new UndoableAddCommand(obj->data(OBJ_NAME).toString(), obj, this, 0);
    if (cmd) undoStack->push(cmd);
}

void view_clearRubberRoom()
{
    foreach (QGraphicsItem* item, rubberRoomList) {
        BaseObject* base = static_cast<BaseObject*>(item);
        if (base) {
            int type = base->type;
            if ((type == OBJ_TYPE_PATH     && spareRubberList.contains(SPARE_RUBBER_PATH))     ||
               (type == OBJ_TYPE_POLYGON  && spareRubberList.contains(SPARE_RUBBER_POLYGON))  ||
               (type == OBJ_TYPE_POLYLINE && spareRubberList.contains(SPARE_RUBBER_POLYLINE)) ||
               (spareRubberList.contains(base->objID))) {
                if (!base->objectPath().elementCount()) {
                    critical_messagebox(this, translate("Empty Rubber Object Error"),
                                          translate("The rubber object added contains no points. "
                                          "The command that created this object has flawed logic. "
                                          "The object will be deleted."));
                    gscene->removeItem(item);
                    delete item;
                }
                else {
                    vulcanizeObject(base);
                }
            }
            else {
                gscene->removeItem(item);
                delete item;
            }
        }
    }

    rubberRoomList.clear();
    spareRubberList.clear();
    gscene->update();
}

void view_spareRubber(qint64 id)
{
    spareRubberList.append(id);
}

void view_setRubberMode(int mode)
{
    foreach (QGraphicsItem* item, rubberRoomList) {
        BaseObject* base = static_cast<BaseObject*>(item);
        if (base) { base->setObjectRubberMode(mode); }
    }
    gscene->update();
}

void view_setRubberPoint(const std::string& key, const EmbVector& point)
{
    foreach (QGraphicsItem* item, rubberRoomList) {
        BaseObject* base = static_cast<BaseObject*>(item);
        if (base) {
            base->setObjectRubberPoint(key, point);
        }
    }
    gscene->update();
}

void view_setRubberText(const std::string& key, const std::string& txt)
{
    foreach(QGraphicsItem* item, rubberRoomList)
    {
        BaseObject* base = static_cast<BaseObject*>(item);
        if (base) { base->setObjectRubberText(key, txt); }
    }
    gscene->update();
}

void view_setGridColor(unsigned int color)
{
    gridColor = QColor(color);
    gscene->setProperty(VIEW_COLOR_GRID, color);
    if (gscene) {
        gscene->update();
    }
}

void view_setRulerColor(unsigned int color)
{
    rulerColor = QColor(color);
    gscene->update();
}

void view_createGrid(const std::string& gridType)
{
    if (gridType == "Rectangular") {
        createGridRect();
        gscene->setProperty("ENABLE_GRID", true);
    }
    else if (gridType == "Circular") {
        createGridPolar();
        gscene->setProperty("ENABLE_POLAR", true);
    }
    else if (gridType == "Isometric") {
        createGridIso();
        gscene->setProperty("ENABLE_ISOMETRIC", true);
    }
    else {
        gridPath = QPainterPath();
        gscene->setProperty("ENABLE_GRID", false);
    }

    createOrigin();

    gscene->update();
}

void view_createOrigin() //TODO: Make Origin Customizable
{
    originPath = QPainterPath();

    if (settings_grid_show_origin) {
        //originPath.addEllipse(EmbVector(0,0), 0.5, 0.5); //TODO: Make Origin Customizable
        double rad = 0.5;
        originPath.moveTo(0.0, rad);
        originPath.arcTo(-rad, -rad, rad*2.0, rad*2.0, 90.0, 360.0);
        originPath.arcTo(-rad, -rad, rad*2.0, rad*2.0, 90.0, -360.0);
        originPath.lineTo(0.0, -rad);
        originPath.arcTo(-rad, -rad, rad*2.0, rad*2.0, 270.0, 90.0);
        originPath.lineTo(-rad, 0.0);
        originPath.arcTo(-rad, -rad, rad*2.0, rad*2.0, 180.0, -90.0);
        originPath.closeSubpath();
    }
}

void view_createGridRect()
{
    EmbRect gr(0, 0, settings_grid_size_x, -settings_grid_size_y);
    //Ensure the loop will work correctly with negative numbers
    double x1 = std::min(gr.left(), gr.right());
    double y1 = std::min(gr.top(), gr.bottom());
    double x2 = std::max(gr.left(), gr.right());
    double y2 = std::max(gr.top(), gr.bottom());

    gridPath = QPainterPath();
    gridPath.addRect(gr);
    for (double gx = x1; gx < x2; gx += settings_grid_spacing_x) {
        for (double gy = y1; gy < y2; gy += settings_grid_spacing_y) {
            gridPath.moveTo(x1,gy);
            gridPath.lineTo(x2,gy);
            gridPath.moveTo(gx,y1);
            gridPath.lineTo(gx,y2);
        }
    }

    //Center the Grid
    EmbRect gridRect = gridPath.boundingRect();
    double bx = gridRect.width()/2.0;
    double by = -gridRect.height()/2.0;
    double dx = settings_grid_center_x - bx;
    double dy = settings_grid_center_y - by;

    if (settings_grid_center_on_origin) {
        gridPath.translate(-bx, -by);
    }
    else {
        gridPath.translate(dx, dy);
    }
}

void view_createGridPolar()
{
    double rad = settings_grid_size_radius;

    gridPath = QPainterPath();
    gridPath.addEllipse(EmbVector(0, 0), rad, rad);
    for (double r = 0; r < rad; r += settings_grid_spacing_radius) {
        gridPath.addEllipse(EmbVector(0,0), r, r);
    }
    for (double ang = 0; ang < 360; ang += settings_grid_spacing_angle) {
        gridPath.moveTo(0, 0);
        gridPath.lineTo(EmbLine::fromPolar(rad, ang).p2());
    }

    if (!settings_grid_center_on_origin) {
        gridPath.translate(settings_grid_center_x, -settings_grid_center_y);
    }
}

void view_createGridIso()
{
    //Ensure the loop will work correctly with negative numbers
    double isoW = std::fabs(settings_grid_size_x);
    double isoH = std::fabs(settings_grid_size_y);

    EmbVector p1 = EmbVector(0,0);
    EmbVector p2 = EmbLine::fromPolar(isoW,  30).p2();
    EmbVector p3 = EmbLine::fromPolar(isoH, 150).p2();
    EmbVector p4 = p2 + p3;

    gridPath = QPainterPath();
    gridPath.moveTo(p1);
    gridPath.lineTo(p2);
    gridPath.lineTo(p4);
    gridPath.lineTo(p3);
    gridPath.lineTo(p1);

    for (float x = 0.0; x < isoW; x += settings_grid_spacing_x) {
        for (float y = 0.0; y < isoH; y += settings_grid_spacing_y) {
            EmbVector px = EmbLine::fromPolar(x,  30).p2();
            EmbVector py = EmbLine::fromPolar(y, 150).p2();

            gridPath.moveTo(px);
            gridPath.lineTo(px+p3);
            gridPath.moveTo(py);
            gridPath.lineTo(py+p2);
        }
    }

    //Center the Grid

    EmbRect gridRect = gridPath.boundingRect();
    // bx is unused
    double by = -gridRect.height()/2.0;

    if (settings_grid_center_on_origin) {
        gridPath.translate(0, -by);
    }
    else {
        gridPath.translate(0, -by);
        gridPath.translate(settings_grid_center_x, -settings_grid_center_y);
    }
}

void view_toggleSnap(bool on)
{
    debug_message("View toggleSnap()");
    //TODO: finish this
    gscene->setProperty("ENABLE_SNAP", on);
    gscene->update();
}

void view_toggleGrid(bool on)
{
    debug_message("View toggleGrid()");
    if (on) {
        createGrid(settings_grid_type);
    }
    else {
        createGrid("");
    }
}

void view_toggleRuler(bool on)
{
    debug_message("View toggleRuler()");
    gscene->setProperty("ENABLE_RULER", on);
    rulerMetric = settings_ruler_metric;
    rulerColor = QColor(settings_ruler_color);
    rulerPixelSize = settings_ruler_pixel_size;
    gscene->update();
}

void view_toggleOrtho(bool on)
{
    debug_message("View toggleOrtho()");
    //TODO: finish this
    gscene->setProperty("ENABLE_ORTHO", on);
    gscene->update();
}

void view_togglePolar(bool on)
{
    debug_message("View togglePolar()");
    //TODO: finish this
    gscene->setProperty("ENABLE_POLAR", on);
    gscene->update();
}

void view_toggleQSnap(bool on)
{
    debug_message("View toggleQSnap()");
    qSnapToggle = on;
    gscene->setProperty("ENABLE_QSNAP", on);
    gscene->update();
}

void view_toggleQTrack(bool on)
{
    debug_message("View toggleQTrack()");
    //TODO: finish this
    gscene->setProperty("ENABLE_QTRACK", on);
    gscene->update();
}

void view_toggleLwt(bool on)
{
    debug_message("View toggleLwt()");
    gscene->setProperty("ENABLE_LWT", on);
    gscene->update();
}

void view_toggleReal(bool on)
{
    debug_message("View toggleReal()");
    gscene->setProperty("ENABLE_REAL", on);
    gscene->update();
}

bool view_isLwtEnabled()
{
    return gscene->property("ENABLE_LWT").toBool();
}

bool view_isRealEnabled()
{
    return gscene->property("ENABLE_REAL").toBool();
}

void view_drawBackground(QPainter* painter, const EmbRect& rect)
{
    painter->fillRect(rect, backgroundBrush());

    if (gscene->property("ENABLE_GRID").toBool() && rect.intersects(gridPath.controlPointRect())) {
        QPen gridPen(gridColor);
        gridPen.setJoinStyle(MiterJoin);
        gridPen.setCosmetic(true);
        painter->setPen(gridPen);
        painter->drawPath(gridPath);
        painter->drawPath(originPath);
        painter->fillPath(originPath, gridColor);
    }
}

void view_drawForeground(QPainter* painter, const EmbRect& rect)
{
    //==================================================
    //Draw grip points for all selected objects
    //==================================================

    QPen gripPen(QColor::fromRgb(gripColorCool));
    gripPen.setWidth(2);
    gripPen.setJoinStyle(MiterJoin);
    gripPen.setCosmetic(true);
    painter->setPen(gripPen);
    EmbVector gripOffset(gripSize, gripSize);

    std::vector<EmbVector> selectedGripPoints;
    std::vector<QGraphicsItem*> selectedItemList = gscene->selectedItems();
    if (selectedItemList.size() <= 100) {
        foreach (QGraphicsItem* item, selectedItemList) {
            if (item->type() >= OBJ_TYPE_BASE) {
                tempBaseObj = static_cast<BaseObject*>(item);
                if (tempBaseObj) { selectedGripPoints = tempBaseObj->allGripPoints(); }

                foreach (EmbVector ssp, selectedGripPoints) {
                    EmbVector p1 = mapFromScene(ssp) - gripOffset;
                    EmbVector q1 = mapFromScene(ssp) + gripOffset;

                    if (ssp == sceneGripPoint) {
                        painter->fillRect(EmbRect(mapToScene(p1), mapToScene(q1)), QColor::fromRgb(gripColorHot));
                    }
                    else {
                        painter->drawRect(EmbRect(mapToScene(p1), mapToScene(q1)));
                    }
                }
            }
        }
    }

    // ==================================================
    // Draw the closest qsnap point
    // ==================================================

    // TODO: && findClosestSnapPoint == true
    if (!selectingActive) {
        QPen qsnapPen(QColor::fromRgb(qsnapLocatorColor));
        qsnapPen.setWidth(2);
        qsnapPen.setJoinStyle(MiterJoin);
        qsnapPen.setCosmetic(true);
        painter->setPen(qsnapPen);
        EmbVector qsnapOffset(qsnapLocatorSize, qsnapLocatorSize);

        std::vector<EmbVector> apertureSnapPoints;
        std::vector<QGraphicsItem *> apertureItemList = items(viewMousePoint.x()-qsnapApertureSize,
                                                        viewMousePoint.y()-qsnapApertureSize,
                                                        qsnapApertureSize*2,
                                                        qsnapApertureSize*2);
        foreach (QGraphicsItem* item, apertureItemList) {
            if (item->type() >= OBJ_TYPE_BASE) {
                tempBaseObj = static_cast<BaseObject*>(item);
                if (tempBaseObj) { apertureSnapPoints << tempBaseObj->mouseSnapPoint(sceneMousePoint); }
            }
        }
        //TODO: Check for intersection snap points and add them to the list
        foreach (EmbVector asp, apertureSnapPoints) {
            EmbVector p1 = mapFromScene(asp) - qsnapOffset;
            EmbVector q1 = mapFromScene(asp) + qsnapOffset;
            painter->drawRect(EmbRect(mapToScene(p1), mapToScene(q1)));
        }
    }

    //==================================================
    //Draw the crosshair
    //==================================================

    if (!selectingActive) {
        //painter->setBrush(NoBrush);
        QPen crosshairPen(QColor::fromRgb(crosshairColor));
        crosshairPen.setCosmetic(true);
        painter->setPen(crosshairPen);
        painter->drawLine(EmbLine(mapToScene(viewMousePoint.x(), viewMousePoint.y()-crosshairSize),
                                 mapToScene(viewMousePoint.x(), viewMousePoint.y()+crosshairSize)));
        painter->drawLine(EmbLine(mapToScene(viewMousePoint.x()-crosshairSize, viewMousePoint.y()),
                                 mapToScene(viewMousePoint.x()+crosshairSize, viewMousePoint.y())));
        painter->drawRect(EmbRect(mapToScene(viewMousePoint.x()-pickBoxSize, viewMousePoint.y()-pickBoxSize),
                                 mapToScene(viewMousePoint.x()+pickBoxSize, viewMousePoint.y()+pickBoxSize)));
    }
}


void view_updateMouseCoords(int x, int y)
{
    viewMousePoint = EmbVector(x, y);
    sceneMousePoint = mapToScene(viewMousePoint);
    gscene->setProperty("SCENE_QSNAP_POINT", sceneMousePoint); //TODO: if qsnap functionality is enabled, use it rather than the mouse point
    gscene->setProperty("SCENE_MOUSE_POINT", sceneMousePoint);
    gscene->setProperty("VIEW_MOUSE_POINT", viewMousePoint);
    mainWin->statusbar->setMouseCoord(sceneMousePoint.x(), -sceneMousePoint.y());
}

void view_setCrossHairSize(quint8 percent)
{
    //NOTE: crosshairSize is in pixels and is a percentage of your screen width
    //NOTE: Example: (1280*0.05)/2 = 32, thus 32 + 1 + 32 = 65 pixel wide crosshair
    QScreen *screen = qApp->primaryScreen();
    quint32 screenWidth = screen->grabWindow(0).width();
    if (percent > 0 && percent < 100) {
        crosshairSize = (screenWidth*(percent/100.0))/2;
    }
    else {
        crosshairSize = screenWidth;
    }
}

void view_setCornerButton()
{
    int num = settings_display_scrollbar_widget_num;
    if (num) {
        QPushButton* cornerButton = new QPushButton(this);
        cornerButton->setFlat(true);
        QAction* act = mainWin->actionHash.value(num);
        //NOTE: Prevent crashing if the action is NULL.
        if (!act) {
            information_messagebox(this, translate("Corner Widget Error"), translate("There are unused enum values in COMMAND_ACTIONS. Please report this as a bug."));
            setCornerWidget(0);
        }
        else {
            cornerButton->setIcon(act->icon());
            connect(cornerButton, SIGNAL(clicked()), this, SLOT(cornerButtonClicked()));
            setCornerWidget(cornerButton);
            cornerButton->setCursor(ArrowCursor);
        }
    }
    else {
        setCornerWidget(0);
    }
}

void view_cornerButtonClicked()
{
    debug_message("Corner Button Clicked.");
    mainWin->actionHash.value(settings_display_scrollbar_widget_num)->trigger();
}

void view_zoomIn()
{
    debug_message("View zoomIn()");
    if (!allowZoomIn()) {
        return;
    }
    EmbVector cntr = mapToScene(EmbVector(width()/2,height()/2));
    scale(settings_display_zoomscale_in, settings_display_zoomscale_in);

    centerOn(cntr);
}

void view_zoomOut()
{
    debug_message("View zoomOut()");
    if (!allowZoomOut()) {
        return;
    }
    EmbVector cntr = mapToScene(EmbVector(width()/2,height()/2));
    scale(settings_display_zoomscale_out, settings_display_zoomscale_out);

    centerOn(cntr);
}

void view_zoomWindow()
{
    zoomWindowActive = true;
    selectingActive = false;
    clearSelection();
}

void view_zoomSelected()
{
    std::vector<QGraphicsItem*> itemList = gscene->selectedItems();
    QPainterPath selectedRectPath;
    foreach (QGraphicsItem* item, itemList) {
        selectedRectPath.addPolygon(item->mapToScene(item->boundingRect()));
    }
    EmbRect selectedRect = selectedRectPath.boundingRect();
    if (selectedRect.isNull()) {
        information_messagebox(this, translate("ZoomSelected Preselect"), translate("Preselect objects before invoking the zoomSelected command."));
        //TODO: Support Post selection of objects
    }
    fitInView(selectedRect, KeepAspectRatio);
}

void view_zoomExtents()
{
    EmbRect extents = gscene->itemsBoundingRect();
    if (extents.isNull()) {
        extents.setWidth(settings_grid_size_x);
        extents.setHeight(settings_grid_size_y);
        extents.moveCenter(EmbVector(0,0));
    }
    fitInView(extents, KeepAspectRatio);
}

void view_panLeft()
{
    horizontalScrollBar()->setValue(horizontalScrollBar()->value() + panDistance);
    updateMouseCoords(viewMousePoint);
    gscene->update();
}

void view_panRight()
{
    horizontalScrollBar()->setValue(horizontalScrollBar()->value() - panDistance);
    updateMouseCoords(viewMousePoint);
    gscene->update();
}

void view_panUp()
{
    verticalScrollBar()->setValue(verticalScrollBar()->value() + panDistance);
    updateMouseCoords(viewMousePoint);
    gscene->update();
}

void view_panDown()
{
    verticalScrollBar()->setValue(verticalScrollBar()->value() - panDistance);
    updateMouseCoords(viewMousePoint);
    gscene->update();
}

void view_selectAll()
{
    QPainterPath allPath;
    allPath.addRect(gscene->sceneRect());
    /* gscene->setSelectionArea(allPath, IntersectsItemShape, this->transform()); */
}

void view_selectionChanged()
{
    if (mainWin->dockPropEdit->isVisible()) {
        mainWin->dockPropEdit->setSelectedItems(gscene->selectedItems());
    }
}

void view_mouseDoubleClickEvent(QMouseEvent* event)
{
    if (event->button() == LeftButton) {
        QGraphicsItem* item = gscene->itemAt(mapToScene(event->pos()), QTransform());
        if (item) {
            mainWin->dockPropEdit->show();
        }
    }
}

void view_mousePressEvent(QMouseEvent* event)
{
    updateMouseCoords(event->x(), event->y());
    if (event->button() == LeftButton) {
        if (mainWin->isCommandActive()) {
            EmbVector cmdPoint = mapToScene(event->pos());
            mainWin->runCommandClick(mainWin->activeCommand(), cmdPoint.x(), cmdPoint.y());
            return;
        }
        QPainterPath path;
        std::vector<QGraphicsItem*> pickList = gscene->items(EmbRect(mapToScene(viewMousePoint.x()-pickBoxSize, viewMousePoint.y()-pickBoxSize),
                                                              mapToScene(viewMousePoint.x()+pickBoxSize, viewMousePoint.y()+pickBoxSize)));

        bool itemsInPickBox = pickList.size();
        if (itemsInPickBox && !selectingActive && !grippingActive) {
            bool itemsAlreadySelected = pickList.at(0)->isSelected();
            if (!itemsAlreadySelected) {
                pickList.at(0)->setSelected(true);
            }
            else {
                bool foundGrip = false;
                BaseObject* base = static_cast<BaseObject*>(pickList.at(0)); //TODO: Allow multiple objects to be gripped at once
                if (!base) return;

                EmbVector qsnapOffset(qsnapLocatorSize, qsnapLocatorSize);
                EmbVector gripPoint = base->mouseSnapPoint(sceneMousePoint);
                EmbVector p1 = mapFromScene(gripPoint) - qsnapOffset;
                EmbVector q1 = mapFromScene(gripPoint) + qsnapOffset;
                EmbRect gripRect = EmbRect(mapToScene(p1), mapToScene(q1));
                EmbRect pickRect = EmbRect(mapToScene(viewMousePoint.x()-pickBoxSize, viewMousePoint.y()-pickBoxSize),
                                        mapToScene(viewMousePoint.x()+pickBoxSize, viewMousePoint.y()+pickBoxSize));
                if (gripRect.intersects(pickRect))
                    foundGrip = true;

                //If the pick point is within the item's grip box, start gripping
                if (foundGrip)
                {
                    startGripping(base);
                }
                else //start moving
                {
                    movingActive = true;
                    pressPoint = event->pos();
                    scenePressPoint = mapToScene(pressPoint);
                }
            }
        }
        else if (grippingActive) {
            stopGripping(true);
        }
        else if (!selectingActive) {
            selectingActive = true;
            pressPoint = event->pos();
            scenePressPoint = mapToScene(pressPoint);

            if (!selectBox)
                selectBox = new SelectBox(QRubberBand::Rectangle, this);
            selectBox->setGeometry(QRect(pressPoint, pressPoint));
            selectBox->show();
        }
        else {
            selectingActive = false;
            selectBox->hide();
            releasePoint = event->pos();
            sceneReleasePoint = mapToScene(releasePoint);

            //Start SelectBox Code
            path.addPolygon(mapToScene(selectBox->geometry()));
            if (sceneReleasePoint.x() > scenePressPoint.x()) {
                if (settings_selection_mode_pickadd) {
                    if (mainWin->isShiftPressed()) {
                        std::vector<QGraphicsItem*> itemList = gscene->items(path, ContainsItemShape);
                        foreach(QGraphicsItem* item, itemList)
                            item->setSelected(false);
                    }
                    else {
                        std::vector<QGraphicsItem*> itemList = gscene->items(path, ContainsItemShape);
                        foreach(QGraphicsItem* item, itemList)
                            item->setSelected(true);
                    }
                }
                else {
                    if (mainWin->isShiftPressed()) {
                        std::vector<QGraphicsItem*> itemList = gscene->items(path, ContainsItemShape);
                        if (!itemList.size())
                            clearSelection();
                        else {
                            foreach (QGraphicsItem* item, itemList)
                                item->setSelected(!item->isSelected()); //Toggle selected
                        }
                    }
                    else {
                        clearSelection();
                        std::vector<QGraphicsItem*> itemList = gscene->items(path, ContainsItemShape);
                        foreach(QGraphicsItem* item, itemList)
                            item->setSelected(true);
                    }
                }
            }
            else {
                if (settings_selection_mode_pickadd) {
                    if (mainWin->isShiftPressed()) {
                        std::vector<QGraphicsItem*> itemList = gscene->items(path, IntersectsItemShape);
                        foreach(QGraphicsItem* item, itemList)
                            item->setSelected(false);
                    }
                    else {
                        std::vector<QGraphicsItem*> itemList = gscene->items(path, IntersectsItemShape);
                        foreach(QGraphicsItem* item, itemList)
                            item->setSelected(true);
                    }
                }
                else {
                    if (mainWin->isShiftPressed()) {
                        std::vector<QGraphicsItem*> itemList = gscene->items(path, IntersectsItemShape);
                        if (!itemList.size())
                            clearSelection();
                        else {
                            foreach(QGraphicsItem* item, itemList)
                                item->setSelected(!item->isSelected()); //Toggle selected
                        }
                    }
                    else {
                        clearSelection();
                        std::vector<QGraphicsItem*> itemList = gscene->items(path, IntersectsItemShape);
                        foreach(QGraphicsItem* item, itemList)
                            item->setSelected(true);
                    }
                }
            }
            //End SelectBox Code
        }

        if (pastingActive) {
            std::vector<QGraphicsItem*> itemList = pasteObjectItemGroup->childItems();
            gscene->destroyItemGroup(pasteObjectItemGroup);
            foreach (QGraphicsItem* item, itemList) {
                gscene->removeItem(item); //Prevent Qt Runtime Warning, QGraphicsScene::addItem: item has already been added to this scene
            }

            undoStack->beginMacro("Paste");
            foreach (QGraphicsItem* item, itemList) {
                BaseObject* base = static_cast<BaseObject*>(item);
                if (base) {
                    UndoableAddCommand* cmd = new UndoableAddCommand(base->data(OBJ_NAME).toString(), base, this, 0);
                    if (cmd) undoStack->push(cmd);
                }
            }
            undoStack->endMacro();

            pastingActive = false;
            selectingActive = false;
        }
        if (zoomWindowActive) {
            fitInView(path.boundingRect(), KeepAspectRatio);
            clearSelection();
        }
    }
    if (event->button() == MiddleButton) {
        panStart(event->pos());
        //The Undo command will record the spot where the pan started.
        UndoableNavCommand* cmd = new UndoableNavCommand("PanStart", this, 0);
        undoStack->push(cmd);
        event->accept();
    }
    gscene->update();
}

void view_panStart(const EmbVector& point)
{
    recalculateLimits();

    alignScenePointWithViewPoint(mapToScene(point), point);

    panningActive = true;
    panStartX = point.x();
    panStartY = point.y();
}

void view_recalculateLimits()
{
    //NOTE: Increase the sceneRect limits if the point we want to go to lies outside of sceneRect's limits
    //      If the sceneRect limits aren't increased, you cannot pan past its limits
    EmbRect  viewRect(mapToScene(rect().topLeft()), mapToScene(rect().bottomRight()));
    EmbRect  sceneRect(gscene->sceneRect());
    EmbRect  newRect = viewRect.adjusted(-viewRect.width(), -viewRect.height(), viewRect.width(), viewRect.height());
    if (!sceneRect.contains(newRect.topLeft()) || !sceneRect.contains(newRect.bottomRight())) {
        gscene->setSceneRect(sceneRect.adjusted(-viewRect.width(),
                                                -viewRect.height(),
                                                viewRect.width(),
                                                viewRect.height()));
    }
}

void view_centerAt(const EmbVector& centerPoint)
{
    //centerOn also updates the scrollbars, which shifts things out of wack o_O
    centerOn(centerPoint);
    //Reshift to the new center
    EmbVector offset = centerPoint - center();
    EmbVector newCenter = centerPoint + offset;
    centerOn(newCenter);
}

void view_alignScenePointWithViewPoint(const EmbVector& scenePoint, const EmbVector& viewPoint)
{
    EmbVector viewCenter = center();
    EmbVector pointBefore = scenePoint;
    //centerOn also updates the scrollbars, which shifts things out of wack o_O
    centerOn(viewCenter);
    //Reshift to the new center so the scene and view points align
    EmbVector pointAfter = mapToScene(viewPoint);
    EmbVector offset = pointBefore - pointAfter;
    EmbVector newCenter = viewCenter + offset;
    centerOn(newCenter);
}

void view_mouseMoveEvent(QMouseEvent* event)
{
    updateMouseCoords(event->position().x(), event->position().y());
    movePoint = event->pos();
    sceneMovePoint = mapToScene(movePoint);

    if (mainWin->isCommandActive()) {
        if (rapidMoveActive) {
            mainWin->runCommandMove(mainWin->activeCommand(), sceneMovePoint.x(), sceneMovePoint.y());
        }
    }
    if (previewActive) {
        if (previewMode == PREVIEW_MODE_MOVE) {
            previewObjectItemGroup->setPos(sceneMousePoint - previewPoint);
        }
        else if (previewMode == PREVIEW_MODE_ROTATE) {
            double mouseAngle = EmbLine(previewPoint, sceneMousePoint).angle();

            EmbVector p;
            double alpha = radians(previewData - mouseAngle);
            p.x = -previewPoint.x;
            p.y = -previewPoint.y;
            rot = embVector_rotate(p, alpha);
            rot = embVector_add(rot, previewPoint);

            previewObjectItemGroup->setPos(rot);
            previewObjectItemGroup->setRotation(previewData - mouseAngle);
        }
        else if (previewMode == PREVIEW_MODE_SCALE) {
            double scaleFactor = previewData;

            double factor = EmbLine(previewPoint, sceneMousePoint).length()/scaleFactor;

            previewObjectItemGroup->setScale(1);
            previewObjectItemGroup->setPos(0,0);

            if (scaleFactor <= 0.0) {
                critical_messagebox(this, translate("ScaleFactor Error"),
                                    translate("Hi there. If you are not a developer, report this as a bug. "
                                    "If you are a developer, your code needs examined, and possibly your head too."));
            }
            else {
                //Calculate the offset
                EmbVector old(0, 0);
                EmbLine scaleLine(preview_point, old);
                scaleLine.setLength(scaleLine.length()*factor);

                EmbVector delta = embVector_subtract(scaleLine.end, old);

                previewObjectItemGroup->setScale(previewObjectItemGroup->scale()*factor);
                previewObjectItemGroup->moveBy(delta);
            }
        }
    }
    if (pastingActive) {
        pasteObjectItemGroup->setPos(sceneMousePoint - pasteDelta);
    }
    if (movingActive) {
        //Ensure that the preview is only shown if the mouse has moved.
        if (!previewActive)
            previewOn(PREVIEW_CLONE_SELECTED, PREVIEW_MODE_MOVE, scenePressPoint.x(), scenePressPoint.y(), 0);
    }
    if (selectingActive) {
        if (sceneMovePoint.x() >= scenePressPoint.x()) {
            selectBox->setDirection(1);
        }
        else {
            selectBox->setDirection(0);
        }
        selectBox->setGeometry(QRect(mapFromScene(scenePressPoint), event->pos()).normalized());
        event->accept();
    }
    if (panningActive) {
        horizontalScrollBar()->setValue(horizontalScrollBar()->value() - (event->x() - panStartX));
        verticalScrollBar()->setValue(verticalScrollBar()->value() - (event->y() - panStartY));
        panStartX = event->x();
        panStartY = event->y();
        event->accept();
    }
    gscene->update();
}

void view_mouseReleaseEvent(QMouseEvent* event)
{
    updateMouseCoords(event->x(), event->y());
    if (event->button() == LeftButton) {
        if (movingActive) {
            previewOff();
            double dx = sceneMousePoint.x()-scenePressPoint.x();
            double dy = sceneMousePoint.y()-scenePressPoint.y();
            //Ensure that moving only happens if the mouse has moved.
            if (dx || dy) moveSelected(dx, dy);
            movingActive = false;
        }
        event->accept();
    }
    if (event->button() == MiddleButton) {
        panningActive = false;
        //The Undo command will record the spot where the pan completed.
        UndoableNavCommand* cmd = new UndoableNavCommand("PanStop", this, 0);
        undoStack->push(cmd);
        event->accept();
    }
    if (event->button() == XButton1) {
        debug_message("XButton1");
        mainWin->undo(); //TODO: Make this customizable
        event->accept();
    }
    if (event->button() == XButton2) {
        debug_message("XButton2");
        mainWin->redo(); //TODO: Make this customizable
        event->accept();
    }
    gscene->update();
}

bool view_allowZoomIn()
{
    EmbVector origin  = mapToScene(0,0);
    EmbVector corner  = mapToScene(width(), height());
    double maxWidth  = corner.x() - origin.x();
    double maxHeight = corner.y() - origin.y();

    if (std::min(maxWidth, maxHeight) < zoomInLimit) {
        debug_message("ZoomIn limit reached. (limit=%.10f)", zoomInLimit);
        return false;
    }

    return true;
}

bool view_allowZoomOut()
{
    EmbVector origin  = mapToScene(0,0);
    EmbVector corner  = mapToScene(width(), height());
    double maxWidth  = corner.x() - origin.x();
    double maxHeight = corner.y() - origin.y();

    if (std::max(maxWidth, maxHeight) > zoomOutLimit) {
        debug_message("ZoomOut limit reached. (limit=%.1f)", zoomOutLimit);
        return false;
    }

    return true;
}

void view_wheelEvent(QWheelEvent* event)
{
    int zoomDir = 1; /* TODO: event->delta(); */
    EmbVector mousePoint = event->position();

    updateMouseCoords(mousePoint.x(), mousePoint.y());
    if (zoomDir > 0) {
        UndoableNavCommand* cmd = new UndoableNavCommand("ZoomInToPoint", this, 0);
        undoStack->push(cmd);
    }
    else {
        UndoableNavCommand* cmd = new UndoableNavCommand("ZoomOutToPoint", this, 0);
        undoStack->push(cmd);
    }
}

void view_zoomToPoint(const EmbVector& mousePoint, int zoomDir)
{
    EmbVector pointBeforeScale(mapToScene(mousePoint));

    // Do The zoom
    double s = 1.0;
    if (zoomDir > 0) {
        if (!allowZoomIn()) {
            return;
        }
        s = settings_display_zoomscale_in;
    }
    else {
        if (!allowZoomOut()) {
            return;
        }
        s = settings_display_zoomscale_out;
    }

    scale(s, s);
    alignScenePointWithViewPoint(pointBeforeScale, mousePoint);
    recalculateLimits();
    alignScenePointWithViewPoint(pointBeforeScale, mousePoint);

    updateMouseCoords(mousePoint.x(), mousePoint.y());
    if (pastingActive) {
        pasteObjectItemGroup->setPos(sceneMousePoint - pasteDelta);
    }
    if (selectingActive) {
        selectBox->setGeometry(QRect(mapFromScene(scenePressPoint), mousePoint).normalized());
    }
    gscene->update();
}

void view_contextMenuEvent(QContextMenuEvent* event)
{
    QMenu menu;
    std::vector<QGraphicsItem*> itemList = gscene->selectedItems();
    bool selectionEmpty = itemList.isEmpty();

    for(int i = 0; i < itemList.size(); i++)
    {
        if (itemList.at(i)->data(OBJ_TYPE) != OBJ_TYPE_NULL)
        {
            selectionEmpty = false;
            break;
        }
    }

    if (pastingActive)
    {
        return;
    }
    if (!mainWin->prompt->isCommandActive())
    {
        std::string lastCmd = mainWin->prompt->lastCommand();
        QAction* repeatAction = new QAction(mainWin->load_icon(lastCmd), "Repeat " + lastCmd, this);
        repeatAction->setStatusTip("Repeats the previously issued command.");
        connect(repeatAction, SIGNAL(triggered()), this, SLOT(repeatAction()));
        menu.addAction(repeatAction);
    }
    if (zoomWindowActive)
    {
        QAction* cancelZoomWinAction = new QAction("&Cancel (ZoomWindow)", this);
        cancelZoomWinAction->setStatusTip("Cancels the ZoomWindow Command.");
        connect(cancelZoomWinAction, SIGNAL(triggered()), this, SLOT(escapePressed()));
        menu.addAction(cancelZoomWinAction);
    }

    menu.addSeparator();
    menu.addAction(mainWin->actionHash.value(ACTION_cut));
    menu.addAction(mainWin->actionHash.value(ACTION_copy));
    menu.addAction(mainWin->actionHash.value(ACTION_paste));
    menu.addSeparator();

    if (!selectionEmpty)
    {
        QAction* deleteAction = new QAction(mainWin->load_icon("erase"), "D&elete", this);
        deleteAction->setStatusTip("Removes objects from a drawing.");
        connect(deleteAction, SIGNAL(triggered()), this, SLOT(deleteSelected()));
        menu.addAction(deleteAction);

        QAction* moveAction = new QAction(mainWin->load_icon("move"), "&Move", this);
        moveAction->setStatusTip("Displaces objects a specified distance in a specified direction.");
        connect(moveAction, SIGNAL(triggered()), this, SLOT(moveAction()));
        menu.addAction(moveAction);

        QAction* scaleAction = new QAction(mainWin->load_icon("scale"), "Sca&le", this);
        scaleAction->setStatusTip("Enlarges or reduces objects proportionally in the X, Y, and Z directions.");
        connect(scaleAction, SIGNAL(triggered()), this, SLOT(scaleAction()));
        menu.addAction(scaleAction);

        QAction* rotateAction = new QAction(mainWin->load_icon("rotate"), "R&otate", this);
        rotateAction->setStatusTip("Rotates objects about a base point.");
        connect(rotateAction, SIGNAL(triggered()), this, SLOT(rotateAction()));
        menu.addAction(rotateAction);

        menu.addSeparator();

        QAction* clearAction = new QAction("Cle&ar Selection", this);
        clearAction->setStatusTip("Removes all objects from the selection set.");
        connect(clearAction, SIGNAL(triggered()), this, SLOT(clearSelection()));
        menu.addAction(clearAction);
    }

    menu.exec(event->globalPos());
}

void view_deletePressed()
{
    debug_message("View deletePressed()");
    if (pastingActive)
    {
        gscene->removeItem(pasteObjectItemGroup);
        delete pasteObjectItemGroup;
    }
    pastingActive = false;
    zoomWindowActive = false;
    selectingActive = false;
    selectBox->hide();
    stopGripping(false);
    deleteSelected();
}

void view_escapePressed()
{
    debug_message("View escapePressed()");
    if (pastingActive)
    {
        gscene->removeItem(pasteObjectItemGroup);
        delete pasteObjectItemGroup;
    }
    pastingActive = false;
    zoomWindowActive = false;
    selectingActive = false;
    selectBox->hide();
    if (grippingActive) stopGripping(false);
    else clearSelection();
}

void view_startGripping(BaseObject* obj)
{
    if (!obj) return;
    grippingActive = true;
    gripBaseObj = obj;
    sceneGripPoint = gripBaseObj->mouseSnapPoint(sceneMousePoint);
    gripBaseObj->setObjectRubberPoint("GRIP_POINT", sceneGripPoint);
    gripBaseObj->setObjectRubberMode(OBJ_RUBBER_GRIP);
}

void view_stopGripping(bool accept)
{
    grippingActive = false;
    if (gripBaseObj)
    {
        gripBaseObj->vulcanize();
        if (accept)
        {
            UndoableGripEditCommand* cmd = new UndoableGripEditCommand(sceneGripPoint, sceneMousePoint, translate("Grip Edit ") + gripBaseObj->data(OBJ_NAME).toString(), gripBaseObj, this, 0);
            if (cmd) undoStack->push(cmd);
            selectionChanged(); //Update the Property Editor
        }
        gripBaseObj = 0;
    }
    //Move the sceneGripPoint to a place where it will never be hot
    sceneGripPoint = sceneRect().topLeft();
}

void view_clearSelection()
{
    gscene->clearSelection();
}

void view_deleteSelected()
{
    std::vector<QGraphicsItem*> itemList = gscene->selectedItems();
    int numSelected = itemList.size();
    if (numSelected > 1)
        undoStack->beginMacro("Delete " + std::string().setNum(itemList.size()));
    for(int i = 0; i < itemList.size(); i++)
    {
        if (itemList.at(i)->data(OBJ_TYPE) != OBJ_TYPE_NULL)
        {
            BaseObject* base = static_cast<BaseObject*>(itemList.at(i));
            if (base)
            {
                UndoableDeleteCommand* cmd = new UndoableDeleteCommand(tr("Delete 1 ") + base->data(OBJ_NAME).toString(), base, this, 0);
                if (cmd)
                undoStack->push(cmd);
            }
        }
    }
    if (numSelected > 1)
        undoStack->endMacro();
}

void view_cut()
{
    if (gscene->selectedItems().isEmpty())
    {
        information_messagebox(this, translate("Cut Preselect"), translate("Preselect objects before invoking the cut command."));
        return; //TODO: Prompt to select objects if nothing is preselected
    }

    undoStack->beginMacro("Cut");
    copySelected();
    deleteSelected();
    undoStack->endMacro();
}

void view_copy()
{
    if (gscene->selectedItems().isEmpty())
    {
        information_messagebox(this, translate("Copy Preselect"), translate("Preselect objects before invoking the copy command."));
        return; //TODO: Prompt to select objects if nothing is preselected
    }

    copySelected();
    clearSelection();
}

void view_copySelected()
{
    std::vector<QGraphicsItem*> selectedList = gscene->selectedItems();

    //Prevent memory leaks by deleting any unpasted instances
    qDeleteAll(mainWin->cutCopyObjectList.begin(), mainWin->cutCopyObjectList.end());
    mainWin->cutCopyObjectList.clear();

    //Create new objects but do not add them to the scene just yet.
    //By creating them now, ensures that pasting will still work
    //if the original objects are deleted before the paste occurs.
    mainWin->cutCopyObjectList = createObjectList(selectedList);
}

void view_paste()
{
    if (pastingActive)
    {
        gscene->removeItem(pasteObjectItemGroup);
        delete pasteObjectItemGroup;
    }

    pasteObjectItemGroup = gscene->createItemGroup(mainWin->cutCopyObjectList);
    pasteDelta = pasteObjectItemGroup->boundingRect().bottomLeft();
    pasteObjectItemGroup->setPos(sceneMousePoint - pasteDelta);
    pastingActive = true;

    //Re-create the list in case of multiple pastes
    mainWin->cutCopyObjectList = createObjectList(mainWin->cutCopyObjectList);
}

std::vector<QGraphicsItem*> view_createObjectList(std::vector<QGraphicsItem*> list)
{
    std::vector<QGraphicsItem*> copyList;

    for(int i = 0; i < list.size(); i++)
    {
        QGraphicsItem* item = list.at(i);
        if (!item)
            continue;

        int objType = item->data(OBJ_TYPE).toInt();

        if (objType == OBJ_TYPE_ARC)
        {
            ArcObject* arcObj = static_cast<ArcObject*>(item);
            if (arcObj)
            {
                ArcObject* copyArcObj = new ArcObject(arcObj);
                copyList.append(copyArcObj);
            }
        }
        else if (objType == OBJ_TYPE_BLOCK)
        {
            //TODO: cut/copy blocks
        }
        else if (objType == OBJ_TYPE_CIRCLE)
        {
            CircleObject* circObj = static_cast<CircleObject*>(item);
            if (circObj)
            {
                CircleObject* copyCircObj = new CircleObject(circObj);
                copyList.append(copyCircObj);
            }
        }
        else if (objType == OBJ_TYPE_DIMALIGNED)
        {
            //TODO: cut/copy aligned dimensions
        }
        else if (objType == OBJ_TYPE_DIMANGULAR)
        {
            //TODO: cut/copy angular dimensions
        }
        else if (objType == OBJ_TYPE_DIMARCLENGTH)
        {
            //TODO: cut/copy arclength dimensions
        }
        else if (objType == OBJ_TYPE_DIMDIAMETER)
        {
            //TODO: cut/copy diameter dimensions
        }
        else if (objType == OBJ_TYPE_DIMLEADER)
        {
            DimLeaderObject* dimLeaderObj = static_cast<DimLeaderObject*>(item);
            if (dimLeaderObj)
            {
                DimLeaderObject* copyDimLeaderObj = new DimLeaderObject(dimLeaderObj);
                copyList.append(copyDimLeaderObj);
            }
        }
        else if (objType == OBJ_TYPE_DIMLINEAR)
        {
            //TODO: cut/copy linear dimensions
        }
        else if (objType == OBJ_TYPE_DIMORDINATE)
        {
            //TODO: cut/copy ordinate dimensions
        }
        else if (objType == OBJ_TYPE_DIMRADIUS)
        {
            //TODO: cut/copy radius dimensions
        }
        else if (objType == OBJ_TYPE_ELLIPSE)
        {
            EllipseObject* elipObj = static_cast<EllipseObject*>(item);
            if (elipObj)
            {
                EllipseObject* copyElipObj = new EllipseObject(elipObj);
                copyList.append(copyElipObj);
            }
        }
        else if (objType == OBJ_TYPE_ELLIPSEARC)
        {
            //TODO: cut/copy elliptical arcs
        }
        else if (objType == OBJ_TYPE_IMAGE)
        {
            //TODO: cut/copy images
        }
        else if (objType == OBJ_TYPE_INFINITELINE)
        {
            //TODO: cut/copy infinite lines
        }
        else if (objType == OBJ_TYPE_LINE)
        {
            LineObject* lineObj = static_cast<LineObject*>(item);
            if (lineObj)
            {
                LineObject* copyLineObj = new LineObject(lineObj);
                copyList.append(copyLineObj);
            }
        }
        else if (objType == OBJ_TYPE_PATH)
        {
            PathObject* pathObj = static_cast<PathObject*>(item);
            if (pathObj)
            {
                PathObject* copyPathObj = new PathObject(pathObj);
                copyList.append(copyPathObj);
            }
        }
        else if (objType == OBJ_TYPE_POINT)
        {
            PointObject* pointObj = static_cast<PointObject*>(item);
            if (pointObj)
            {
                PointObject* copyPointObj = new PointObject(pointObj);
                copyList.append(copyPointObj);
            }
        }
        else if (objType == OBJ_TYPE_POLYGON)
        {
            PolygonObject* pgonObj = static_cast<PolygonObject*>(item);
            if (pgonObj)
            {
                PolygonObject* copyPgonObj = new PolygonObject(pgonObj);
                copyList.append(copyPgonObj);
            }
        }
        else if (objType == OBJ_TYPE_POLYLINE)
        {
            PolylineObject* plineObj = static_cast<PolylineObject*>(item);
            if (plineObj)
            {
                PolylineObject* copyPlineObj = new PolylineObject(plineObj);
                copyList.append(copyPlineObj);
            }
        }
        else if (objType == OBJ_TYPE_RAY)
        {
            //TODO: cut/copy rays
        }
        else if (objType == OBJ_TYPE_RECTANGLE)
        {
            RectObject* rectObj = static_cast<RectObject*>(item);
            if (rectObj)
            {
                RectObject* copyRectObj = new RectObject(rectObj);
                copyList.append(copyRectObj);
            }
        }
        else if (objType == OBJ_TYPE_TEXTSINGLE)
        {
            TextSingleObject* textObj = static_cast<TextSingleObject*>(item);
            if (textObj)
            {
                TextSingleObject* copyTextObj = new TextSingleObject(textObj);
                copyList.append(copyTextObj);
            }
        }
    }

    return copyList;
}

void view_repeatAction()
{
    mainWin->prompt->endCommand();
    mainWin->prompt->setCurrentText(mainWin->prompt->lastCommand());
    mainWin->prompt->processInput(Key_Return);
}

void view_moveAction()
{
    mainWin->prompt->endCommand();
    mainWin->prompt->setCurrentText("move");
    mainWin->prompt->processInput(Key_Return);
}

void view_moveSelected(double dx, double dy)
{
    std::vector<QGraphicsItem*> itemList = gscene->selectedItems();
    int numSelected = itemList.size();
    if (numSelected > 1)
        undoStack->beginMacro("Move " + std::string().setNum(itemList.size()));
    foreach(QGraphicsItem* item, itemList)
    {
        BaseObject* base = static_cast<BaseObject*>(item);
        if (base)
        {
            UndoableMoveCommand* cmd = new UndoableMoveCommand(dx, dy, translate("Move 1 ") + base->data(OBJ_NAME).toString(), base, this, 0);
            if (cmd) undoStack->push(cmd);
        }
    }
    if (numSelected > 1)
        undoStack->endMacro();

    //Always clear the selection after a move
    gscene->clearSelection();
}

void view_rotateAction()
{
    mainWin->prompt->endCommand();
    mainWin->prompt->setCurrentText("rotate");
    mainWin->prompt->processInput(Key_Return);
}

void view_rotateSelected(double x, double y, double rot)
{
    std::vector<QGraphicsItem*> itemList = gscene->selectedItems();
    int numSelected = itemList.size();
    if (numSelected > 1)
        undoStack->beginMacro("Rotate " + std::string().setNum(itemList.size()));
    foreach(QGraphicsItem* item, itemList)
    {
        BaseObject* base = static_cast<BaseObject*>(item);
        if (base)
        {
            UndoableRotateCommand* cmd = new UndoableRotateCommand(x, y, rot, translate("Rotate 1 ") + base->data(OBJ_NAME).toString(), base, this, 0);
            if (cmd) undoStack->push(cmd);
        }
    }
    if (numSelected > 1)
        undoStack->endMacro();

    //Always clear the selection after a rotate
    gscene->clearSelection();
}

void view_mirrorSelected(double x1, double y1, double x2, double y2)
{
    std::vector<QGraphicsItem*> itemList = gscene->selectedItems();
    int numSelected = itemList.size();
    if (numSelected > 1)
        undoStack->beginMacro("Mirror " + std::string().setNum(itemList.size()));
    foreach(QGraphicsItem* item, itemList)
    {
        BaseObject* base = static_cast<BaseObject*>(item);
        if (base)
        {
            UndoableMirrorCommand* cmd = new UndoableMirrorCommand(x1, y1, x2, y2, translate("Mirror 1 ") + base->data(OBJ_NAME).toString(), base, this, 0);
            if (cmd) undoStack->push(cmd);
        }
    }
    if (numSelected > 1)
        undoStack->endMacro();

    //Always clear the selection after a mirror
    gscene->clearSelection();
}

void view_scaleAction()
{
    mainWin->prompt->endCommand();
    mainWin->prompt->setCurrentText("scale");
    mainWin->prompt->processInput(Key_Return);
}

void view_scaleSelected(double x, double y, double factor)
{
    std::vector<QGraphicsItem*> itemList = gscene->selectedItems();
    int numSelected = itemList.size();
    if (numSelected > 1)
        undoStack->beginMacro("Scale " + std::string().setNum(itemList.size()));
    foreach(QGraphicsItem* item, itemList)
    {
        BaseObject* base = static_cast<BaseObject*>(item);
        if (base)
        {
            UndoableScaleCommand* cmd = new UndoableScaleCommand(x, y, factor, translate("Scale 1 ") + base->data(OBJ_NAME).toString(), base, this, 0);
            if (cmd) undoStack->push(cmd);
        }
    }
    if (numSelected > 1)
        undoStack->endMacro();

    //Always clear the selection after a scale
    gscene->clearSelection();
}

int view_numSelected()
{
    return gscene->selectedItems().size();
}

void view_showScrollBars(bool val)
{
    if (val) {
        setHorizontalScrollBarPolicy(ScrollBarAlwaysOn);
        setVerticalScrollBarPolicy(ScrollBarAlwaysOn);
    }
    else {
        setHorizontalScrollBarPolicy(ScrollBarAlwaysOff);
        setVerticalScrollBarPolicy(ScrollBarAlwaysOff);
    }
}

void view_setCrossHairColor(unsigned int color)
{
    crosshairColor = color;
    gscene->setProperty(VIEW_COLOR_CROSSHAIR, color);
    if (gscene) gscene->update();
}

void view_setBackgroundColor(unsigned int color)
{
    setBackgroundBrush(QColor(color));
    gscene->setProperty("VIEW_COLOR_BACKGROUND", color);
    if (gscene) gscene->update();
}

void view_setSelectBoxColors(unsigned int colorL, unsigned int fillL, unsigned int colorR, unsigned int fillR, int alpha)
{
    selectBox->setColors(QColor(colorL), QColor(fillL), QColor(colorR), QColor(fillR), alpha);
}

SelectBox::SelectBox(Shape s, QWidget* parent) : QRubberBand(s, parent)
{
    //Default values
    setColors(QColor(Qt::darkGreen), QColor(Qt::green), QColor(Qt::darkBlue), QColor(Qt::blue), 32);
}

void SelectBox::setDirection(int dir)
{
    if (!dir) { dirPen = leftPen;  dirBrush = leftBrush;  }
    else     { dirPen = rightPen; dirBrush = rightBrush; }
    boxDir = dir;
}

void SelectBox::setColors(const QColor& colorL, const QColor& fillL, const QColor& colorR, const QColor& fillR, int newAlpha)
{
    debug_message("SelectBox setColors()");
    alpha = newAlpha;

    leftPenColor = colorL; //TODO: allow customization
    leftBrushColor = QColor(fillL.red(), fillL.green(), fillL.blue(), alpha);
    rightPenColor = colorR; //TODO: allow customization
    rightBrushColor = QColor(fillR.red(), fillR.green(), fillR.blue(), alpha);

    leftPen.setColor(leftPenColor);
    leftPen.setStyle(Qt::DashLine);
    leftBrush.setStyle(Qt::SolidPattern);
    leftBrush.setColor(leftBrushColor);

    rightPen.setColor(rightPenColor);
    rightPen.setStyle(Qt::SolidLine);
    rightBrush.setStyle(Qt::SolidPattern);
    rightBrush.setColor(rightBrushColor);

    if (!boxDir) { dirPen = leftPen;  dirBrush = leftBrush;  }
    else        { dirPen = rightPen; dirBrush = rightBrush; }

    forceRepaint();
}

void SelectBox::paintEvent(QPaintEvent*)
{
    QPainter painter(this);
    painter.setPen(dirPen);
    painter.fillRect(0,0,width()-1, height()-1, dirBrush);
    painter.drawRect(0,0,width()-1, height()-1);
}

void SelectBox::forceRepaint()
{
    //HACK: Take that QRubberBand!
    QSize hack = size();
    resize(hack + QSize(1,1));
    resize(hack);
}

#endif
