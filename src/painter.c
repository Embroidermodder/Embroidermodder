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
 *  Painter and pen functions for drawing in the window.
 */

#include "em2.h"

void draw_line(EmbLine line);
void draw_lines(EmbLine *line, int n_lines);

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


#if 0
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
create_grid_rect(scheme *sc)
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
create_grid_polar(void)
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
#endif

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
    gscene.update();
    */
}

void
vulcanize_rubber_room(void)
{
    /*
    for (base in rubber-room-list) {
        if (base) {
            vulcanize-object(base)
        }
    }

    rubber-room-list.clear()
    gscene.update()
    */
}

void
vulcanize_object(int obj)
{
    if (!obj) {
        return;
    }
    /*
    gscene.remove-item(-obj)
    # Prevent Qt Runtime Warning, QGraphicsScene-addItem:
    # item has alreadelta-y been added to self scene.
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


typedef struct Ruler_ {
    EmbVector position;
    EmbVector size;
    double width;
    int metric;
    int feet;
    double little;
    double medium;
    double fraction;
} Ruler;

typedef struct ViewPort_ {
    Ruler vertical;
    Ruler horizontal;
} ViewPort;

/* Should create a label widget under the new system. */
void
create_ruler_text_path(EmbVector position, char *str, double height)
{
    printf("called with %f %s %f", position.x, str, height);
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

#if 0
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
    *n_lines = 0;
    if (ruler.metric) {
        for (i=0; i<10; i++) {
            double height = position.y;
            if (i>0) {
                if (i==5) {
                    height -= ruler.width*ruler.medium;
                }
                else {
                    height -= ruler.width*ruler.little;
                }
            }
            ticks[n_lines].x1 = position.x;
            ticks[n_lines].y1 = position.y + fraction*i;
            ticks[n_lines].x2 = height;
            ticks[n_lines].y2 = position.y + fraction*i;
            (*n_lines)++;
        }
    }
    else {
        if (ruler.feet) {
            for (i=0; i<12; i++) {
                ticks[n_lines].x1 = ruler-vert.x;
                ticks[n_lines].y1 = y+fraction*i;
                ticks[n_lines].x2 = position.y - ruler.width*ruler.medium;
                ticks[n_lines].y2 = y+fraction*i;
                (*n_lines)++;
            }
        }
        else {
            for (i=0; i<16; i++) {
                double height = oy;
                if (i>0) {
                    if (i==5 || i==) {
                        height = ruler-horiz.y - ruler-size.y*medium;
                    }
                    if (i!=0 && i!=5) {
                        height = ruler-horiz.y - ruler-size.y*little;
                    }
                }
                ticks[n_lines].x1 = ruler-vert.x;
                ticks[n_lines].y1 = y+fraction*i;
                ticks[n_lines].x2 = height;
                ticks[n_lines].y2 = y+fraction*i;
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
    int i;
    *n_lines = 0;
    if (ruler.metric) {
        for (i=0; i<10; i++) {
            height = ruler-horiz.y
            if i==0:
                height = origin.y
            elif i==5:
                height -= ruler-size.y*medium
            else:
                height -= ruler-size.y*little
            ticks += [Line(x+fraction*i, ruler-horiz.y, x+fraction*i, height)]
    else {
        if (ruler.feet) {
            for i in range(12)
                height = position.y - ruler-size.y*medium;
                ticks += [Line(ruler-vert.x, y+fraction*i, height, y+fraction*i)];
        }
        else {
            for (i=0; i<16; i++) {
                double height = ruler-horiz.y;
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
)

/*
 * Draw horizontal and vertical rulers.
 */
void
draw_rulers(ViewPort *scene)
{
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
    # Intentional underflow
    if horizontal-ruler-size.x >= maxSize or vertical-ruler-size.y >= maxSize:
        return

    distance = map-to-scene(settings.rulerPixelSize*3, 0).x() - origin.x
    dist-str = str(distance)
    dist-str-size = len(dist-str)
    msd = int(dist-str[0]) # Most Significant Digit

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

#endif

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

void
draw_rulers()
{

}


/*  The layer manager.
 */
void
layer_manager_init(LayerManager *mgr, int tab)
{
    printf("%d %d\n", mgr->visible, tab);
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
    mainLayout, add_widget(treeView)
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
    double line_weight)
    strcpy(mgr->name, name)
    mgr->visible = visible
    mgr->frozen = frozen
    mgr->z-value = zValue
    strcpy(mgr->color, color)
    strcpy(mgr->line_type, lineType)
    mgr->line_weight = line_weight */
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
    /* #LayerManager layman( self,  self);
     * #layman.exec(); */
}

void
layer_previous_action(void)
{
    debug_message("layerPrevious()");
    debug_message("Implement layerPrevious.");
}

/* Make all layers not editable.
 */
void
freeze_all_layers(void)
{
    debug_message("freezeAllLayers()");
}

/* Make layers editable again.
 */
void
thaw_all_layers(void)
{
    debug_message("thawAllLayers()");
}

/*
 * Make all layers not editable.
 * (What's the difference between freeze and lock?);
 */
void
lock_all_layers(void)
{
    debug_message("lock_all_layers()");
}

/* .
 */
void
unlock_all_layers(void)
{
    debug_message("unlock_all_layers()");
}

/* .
 */
void
hide_all_layers(void)
{
    debug_message("hide_all_layers()");
}

/* .
 */
void
make_layer_current(void)
{
    debug_message("make_layer_current()");
}

/* .
 */
void
layers(void)
{
    debug_message("layers()");
}

/* .
 */
void
layer_selector(void)
{
    debug_message("layer_selector()");
}

/* .
 */
void
layer_previous(void)
{
    debug_message("scm_layer_previous()");
}

/* Apply 1 to all layer's visibility flags.
 */
void
show_all_layers(void)
{
    debug_message("showAllLayers()");
}

/* Make layer active.
 */
void
make_layer_active(void)
{
    debug_message("make_layer_active()");
    debug_message("Implement makeLayerActive.");
}

