#!/usr/bin/env python3

r"""
    Embroidermodder 2.

    ------------------------------------------------------------

    Copyright 2013-2022 The Embroidermodder Team
    Embroidermodder 2 is Open Source Software.
    See LICENSE for licensing terms.

    ------------------------------------------------------------

    The View class definition file.

    How we manage the content of the main editing area in the window.
"""

import tkinter as tk

from libembroidery import Line, Path, Rect, Vector
from embroidermodder.pattern import Pattern
from embroidermodder.color import Color
from embroidermodder.utility import settings, translate, debug_message, critical


def round_to_multiple(round_up, input_number, multiple):
    " . "
    if multiple == 0:
        return input_number
    remainder = input_number % multiple
    if remainder == 0:
        return input_number

    if input_number < 0 and round_up:
        return input_number - remainder
    if round_up:
        return input_number + multiple - remainder
    #else round down
    if input_number < 0 and not round_up:
        return input_number - multiple - remainder
    return input_number - remainder

class View():
    """
    There are 4 regions managed as views, .

    We don't have a seperate window for the pop-ups like the file
    browser for opening or saving a file. Instead, a view will
    be created
    """
    def __init__(self, mw, the_scene):
        " . "
        self.mw = mw
        self.gscene = the_scene
        self.frame_shape = "No frame"
        self.hash_deleted_objects = {}
        self.sparerubber_list = []
        self.pan_distance = 0.0
        self.pan_start_x = 0.0
        self.pan_start_y = 0.0
        self.rubber_room_list = 0
        self.gripBase_obj = 0
        self.tempBase_obj = 0
        self.select_box = 0
        self.cut_copy_mouse_point = 0
        self.paste_object_item_group = 0
        self.preview_object_list = 0
        self.preview_object_item_group = 0
        self.preview_point = 0
        self.preview_data = 0
        self.preview_mode = 0
        self.origin_path = Path()
        self.grid_color = Color()
        self.grid_path = Path()

        # NOTE: self has to be done before setting mouse tracking.
        # TODO: Review OpenGL for Qt5 later
        #if settings.display_use_opengl:
        #    debug_message("Using OpenGL...")
        #    setViewport(new QGLWidget(QGLFormat(QGL_DoubleBuffer)))
        #

        # TODO: Review RenderHints later
        # set_render_hint(Painter_Antialiasing,
        #               settings.display_render_hintAA())
        # set_render_hint(Painter_TextAntialiasing,
        #               settings.display_render_hintTextAA())
        # set_render_hint(Painter_SmoothPixmapTransform,
        #               settings.display_render_hintSmoothPix())
        # set_render_hint(Painter_HighQualityAntialiasing,
        #               settings.display_render_hintHighAA())
        # set_render_hint(Painter_NonCosmeticDefaultPen,
        #               settings.display_render_hint_noncosmetic)

        # NOTE
        # ----
        # FullViewportUpdate MUST be used for both the GL and Qt renderers.
        # Qt renderer will not draw the foreground properly if it isnt set.

        setViewportUpdateMode(Graphicsdef_FullViewportUpdate)

        pan_distance = 10
        #TODO: should there be a setting for self???

        set_cursor("BlankCursor")
        horizontal_scroll_bar().set_cursor("ArrowCursor")
        vertical_scroll_bar().set_cursor("ArrowCursor")
        qsnap_locator_color = settings["qsnap_locator_color"]
        grip_color_cool = settings["selection_coolgrip_color"]
        grip_color_hot = settings["selection_hotgrip_color"]
        set_crosshair_color(settings["display_crosshair_color)"])
        set_cross_hair_size(settings["display_crosshair_percent"])
        setgrid_color(settings["grid_color"])

        if settings["grid_show_on_load"]:
            create_grid(settings["grid_type"])
        else:
            create_grid("")

        toggleRuler(settings["ruler_show_on_load"])
        toggleReal(1)
        #TODO: load self from file, else settings with default being 1

        settings["gripping_active"] = 0
        settings["rapid_move_active"] = 0
        preview_mode = "PREVIEW_MODE_NULL"
        self.preview_data = 0
        self.preview_object_item_group = 0
        self.paste_object_item_group = 0
        settings["preview_active"] = 0
        settings["pasting_active"] = 0
        settings["moving_active"] = 0
        settings["selecting_active"] = 0
        settings["zoom_window_active"] = 0
        settings["panning_real_time_active"] = 0
        settings["panning_point_active"] = 0
        settings["panning_active"] = 0
        settings["qsnap_active"] = 0
        settings["qsnap_toggle"] = 0

        # Randomize the hot grip location initially so it's not located at (0,0).
        srand(1234)

        sceneGripPoint = Vector((rand()%1000)*0.1, (rand()%1000)*0.1)

        gripBase_obj = 0
        tempBase_obj = 0

        select_box = select_box("RubberBand_Rectangle")
        select_box.set_colors(
            settings["display_select_box_left_color"],
            settings["display_select_box_left_fill"],
            settings["display_select_box_right_color"],
            settings["display_select_box_right_fill"],
            settings["display_select_box_alpha"])

        self.show_scroll_bars(settings["display_show_scrollbars"])
        set_corner_button()

        installEventFilter(self)

        setMouseTracking(1)
        set_background_color(settings.display_bg_color)
        # TODO: wrap self with a setBackgroundPixmap() function:
        #      set_background_brush(Pixmap("images/canvas))

        # connect(gscene, SIGNAL(selection_changed()), self,
        #     SLOT(selection_changed()))

    def __del__(self):
        " Prevent memory leaks by deleting any objects that were removed from the scene "
        DeleteAll(hash_deleted_objects.begin(), hash_deleted_objects.end())
        hash_deleted_objects.clear()

        # Prevent memory leaks by deleting any unused instances
        DeleteAll(preview_object_list.begin(), preview_object_list.end())
        preview_object_list.clear()

    def move_selected(self, delta_x, delta_y):
        " . "
        return

    def mirror_selected(self, point1_x, point1_y, point2_x, point2_y):
        " . "
        return

    def set_crosshair_color(self, color):
        " . "
        return

    def set_crosshair_size(self, percent):
        " . "
        return

    def load_ruler_settings(self):
        " . "
        return

    def center(self):
        " . "
        return map_to_scene(rect().center())

    def enter_event(self, event):
        " . "
        mdi_win = self.mw
        if mdi_win:
            self.mw.mdiArea.setActiveSubWindow(mdi_win)

    def add_object(self, obj):
        " . "
        self.gscene.addItem(_obj)
        self.gscene.update()
        hash_deleted_objects.remove(_obj._objID)

    def delete_object(self, obj):
        " . "
        # NOTE: We really just remove the objects from the scene.
        #       deletion actually occurs in the destructor.
        obj.set_selected(0)
        self.gscene.remove_item(_obj)
        self.gscene.update()
        hash_deleted_objects.insert(_obj._objID, obj)

    def preview_on(self, clone, mode, x, y, data):
        " . "
        debug_message("View preview_on()")
        preview_off()
        # Free the old objects before creating ones

        preview_mode = mode

        # Create objects and add them to the scene in an item group.
        if clone == "PREVIEW_CLONE_SELECTED":
            preview_object_list = create_object_list(gscene.selected_items())
        elif clone == PREVIEW_CLONE_RUBBER:
            preview_object_list = create_object_list(rubber_room_list)
        else:
            return
        preview_object_item_group = self.gscene.create_item_group(preview_object_list)

        if (preview_mode == "PREVIEW_MODE_MOVE" or
            preview_mode == "PREVIEW_MODE_ROTATE" or
            preview_mode == "PREVIEW_MODE_SCALE"):
            preview_point = Vector(x, y)
            # NOTE: Move: basePt
            # Rotate: basePt
            # Scale: basePt
            preview_data = data
            # NOTE: Move: unused
            # Rotate: refAngle
            # Scale: refFactor
            settings.preview_active = 1

        else:
            preview_mode = PREVIEW_MODE_NULL
            preview_point = Vector()
            preview_data = 0
            settings.preview_active = 0

        self.gscene.update()

    def preview_off(self):
        " , "
        # Prevent memory leaks by deleting any unused instances.
        DeleteAll(preview_object_list.begin(), preview_object_list.end())
        preview_object_list.clear()

        if preview_object_item_group:
            self.gscene.remove_item(preview_object_item_group)
            del preview_object_item_group
            preview_object_item_group = 0

        settings.preview_active = 0

        self.gscene.update()

    def enable_move_rapid_fire(self):
        " . "
        settings["rapidMoveActive"] = True

    def disable_move_rapid_fire(self):
        " . "
        settings["rapidMoveActive"] = False

    def allow_rubber(self):
        " TODO: self check should be removed later. "
        return not rubber_room_list.size()

    def add_to_rubber_room(self, item):
        " . "
        rubber_room_list.append(item)
        item.show()
        self.gscene.update()

    def vulcanize_rubber_room(self):
        " . "
        for base in rubber_room_list:
            if base:
                vulcanize_object(base)

        rubber_room_list.clear()
        self.gscene.update()

    def vulcanize_object(self, obj):
        " . "
        if not obj:
            return
        self.gscene.remove_item(_obj)
        # Prevent Qt Runtime Warning, QGraphicsScene_addItem:
        # item has alreadelta_y been added to self scene.
        obj.vulcanize()

    def clear_rubber_room(self):
        " . "
        for item in rubber_room_list:
            if item:
                if ((item.type == "Path" and spare_rubber_list.contains("SPARE_RUBBER_PATH")) or
                (item.type == "Polygon" and spare_rubber_list.contains("SPARE_RUBBER_POLYGON")) or
                (item.type == "Polyline" and spare_rubber_list.contains("SPARE_RUBBER_POLYLINE")) or
                (sparerubber_list.contains(item._objID))):
                    if not item._objectPath().element_count(self):
                        error_title = translate("Empty Rubber object Error")
                        error_message = translate(
    "The rubber object added contains no points. "
    + "The command that created self object has flawed logic. "
    + "The object will be deleted.")
                        critical(error_title, error_message)
                        self.gscene.remove_item(item)
                        del item

                    else:
                        vulcanize_object(item)

                else:
                    self.gscene.remove_item(item)
                    del item

        rubber_room_list.clear()
        spare_rubber_list.clear()
        self.gscene.update()

    def spare_rubber(self, id):
        " . "
        spare_rubber_list.append(id)

    def set_rubber_mode(self, mode):
        " . "
        for item in rubber_room_list:
            if item:
                item.set_object_rubber_mode(mode)

        self.gscene.update()

    def set_rubber_point(self, key, point):
        " . "
        for item in rubber_room_list:
            if item:
                item.set_object_rubber_point(key, point)

        self.gscene.update()

    def set_rubber_text(self, key, txt):
        " . "
        for item in rubber_room_list:
            if item:
                item.set_objectRubberText(key, txt)

        self.gscene.update()

    def set_grid_color(self, color):
        " . "
        grid_color = Color(color)
        self.gscene.set_property("VIEW_COLOR_GRID", color)
        if self.gscene:
            self.gscene.update()

    def set_ruler_color(self, color):
        " . "
        ruler_color = Color(color)
        self.gscene.update()

    def create_grid(self, gridType):
        " . "
        if gridType == "Rectangular":
            create_grid_rect()
            self.gscene.set_property("ENABLE_GRID", 1)

        elif gridType == "Circular":
            create_grid_polar()
            self.gscene.set_property("ENABLE_GRID", 1)

        elif gridType == "Isometric":
            create_grid_iso()
            self.gscene.set_property("ENABLE_GRID", 1)

        else:
            grid_path = Path()
            self.gscene.set_property("ENABLE_GRID", 0)

        createOrigin()

        # EXPERIMENT
        # Tagging experiments with the path system to the origin.

        position = [10.0, 0.0]
        scale = [1.0, 1.0]
        self.origin_path.add_list_to_path(origin_string, position, scale)

        self.gscene.update()

    def create_origin(self):
        " TODO: Make Origin Customizable "
        self.origin_path = Path()

        if settings["grid_show_origin"]:
            # self.origin_path.addEllipse(Vector(0,0), 0.5, 0.5)
            # TODO: Make Origin Customizable
            position = [0.0, 0.0]
            scale = [1.0, 1.0]
            self.origin_path.add_list_to_path(origin_string, position, scale)

    def create_grid_rect(self):
        " . "
        x_spacing = settings["grid_spacing"]["x"]
        y_spacing = settings["grid_spacing"]["y"]

        gr = Rect(0, 0,
                  settings["grid_size"]["x"],
                  -settings["grid_size"]["y"])
        # Ensure the loop will work correctly with negative numbers
        point1_x = min(gr.left(), gr.right())
        point1_y = min(gr.top(), gr.bottom())
        point2_x = max(gr.left(), gr.right())
        point2_y = max(gr.top(), gr.bottom())

        grid_path = Path()
        grid_path.add_rect(gr)
        gx = point1_x
        while gx < point2_x:
            gy = point1_y
            while gy < point2_y:
                grid_path.move_to(point1_x, gy)
                grid_path.line_to(point2_x, gy)
                grid_path.move_to(gx, point1_y)
                grid_path.line_to(gx, point2_y)
                gy += y_spacing
            gx += x_spacing

        # Center the Grid
        grid_rect = grid_path.bounding_rect()
        bx = grid_rect.width() / 2.0
        by = -grid_rect.height() / 2.0
        center = Vector(settings["grid_center"]["x"], -settings["grid_center"]["y"])
        delta_x = center.x - bx
        delta_y = center.y - by

        if settings["grid_center_on_origin"]:
            grid_path.translate(-bx, -by)
        else:
            grid_path.translate(delta_x, delta_y)

    def create_grid_polar(self):
        " . "
        rad_spacing = settings["grid_spacing_radius"]
        ang_spacing = settings["grid_spacing_angle"]

        rad = settings["grid_size_radius"]

        grid_path = Path()
        grid_path.addEllipse(Vector(0,0), rad, rad)
        r = 0.0
        while r < rad:
            grid_path.addEllipse(Vector(0,0), r, r)
            r += rad_spacing

        ang = 0.0
        while ang < 360:
            grid_path.move_to(0,0)
            grid_path.line_to(Line_from_polar(rad, ang).p2())
            ang += ang_spacing

        if not settings["grid_center_on_origin"]:
            grid_path.translate(settings.grid_center.x, -settings.grid_center.y)

    def create_grid_iso(self):
        " . "
        # Ensure the loop will work correctly with negative numbers
        isoW = abs(settings["grid_size"]["x"])
        isoH = abs(settings["grid_size"]["y"])

        p1 = Vector(0,0)
        p2 = Line_from_polar(isoW, 30).p2()
        p3 = Line_from_polar(isoH, 150).p2()
        p4 = p2 + p3

        grid_path = Path()
        grid_path.move_to(p1)
        grid_path.line_to(p2)
        grid_path.line_to(p4)
        grid_path.line_to(p3)
        grid_path.line_to(p1)

        x = 0.0
        while x < isoW:
            y = 0.0
            while y < isoH:
                px = Line_from_polar(x, 30).p2()
                py = Line_from_polar(y, 150).p2()

                grid_path.move_to(px)
                grid_path.line_to(px+p3)
                grid_path.move_to(py)
                grid_path.line_to(py+p2)
                y += settings["grid_spacing"]["y"]
            x += settings["grid_spacing"]["x"]

        #Center the Grid
        grid_rect = grid_path.bounding_rect()
        # bx is unused
        by = -grid_rect.height()/2.0
        cx = settings["grid_center"]["x"]
        cy = -settings["grid_center"]["y"]

        if settings["grid_center_on_origin"]:
            grid_path.translate(0, -by)
        else:
            grid_path.translate(0, -by)
            grid_path.translate(cx, cy)

    def toggle_snap(self, active):
        " . "
        debug_message("View toggle_snap()")
        set_override_cursor("WaitCursor")
        #  TODO: finish this.
        self.gscene.set_property("ENABLE_SNAP", active)
        self.gscene.update()
        restore_override_cursor()

    def toggle_grid(self, active):
        " . "
        debug_message("View toggleGrid()")
        set_override_cursor("WaitCursor")
        if on:
            create_grid(settings.grid_type)
        else:
            create_grid("")
        restore_override_cursor()

    def toggle_ruler(self, active):
        " . "
        debug_message("View toggle_ruler()")
        set_override_cursor("WaitCursor")
        self.gscene.set_property("ENABLE_RULER", active)
        ruler_color = Color(settings["ruler_color"])
        self.gscene.update()
        restore_override_cursor()

    def toggle_ortho(self, active):
        " . "
        debug_message("View toggleOrtho()")
        set_override_cursor("WaitCursor")
        # TODO: finish this.
        self.gscene.set_property("ENABLE_ORTHO", active)
        self.gscene.update()
        restore_override_cursor()

    def toggle_polar(self, active):
        " . "
        debug_message("View togglePolar()")
        set_override_cursor("WaitCursor")
        #TODO: finish this.
        self.gscene.set_property("ENABLE_POLAR", active)
        self.gscene.update()
        restore_override_cursor()

    def toggleQSnap(self, active):
        " . "
        debug_message("View toggleQSnap()")
        set_override_cursor("WaitCursor")
        settings["qsnap_toggle"] = on
        self.gscene.set_property("ENABLE_QSNAP", active)
        self.gscene.update()
        restore_override_cursor()

    def toggleQTrack(self, active):
        " . "
        debug_message("View toggleQTrack()")
        set_override_cursor("WaitCursor")
        #TODO: finish this.
        self.gscene.set_property("ENABLE_QTRACK", active)
        self.gscene.update()
        restore_override_cursor()

    def toggle_lwt(self, active):
        " . "
        debug_message("View toggleLwt()")
        set_override_cursor("WaitCursor")
        self.gscene.set_property("ENABLE_LWT", active)
        self.gscene.update()
        restore_override_cursor()

    def toggle_real(self, active):
        " . "
        debug_message("View toggleReal()")
        set_override_cursor("WaitCursor")
        self.gscene.set_property("ENABLE_REAL", active)
        self.gscene.update()
        restore_override_cursor()

    def is_lwt_enabled(self):
        " . "
        return self.gscene.property("ENABLE_LWT")

    def is_real_enabled(self):
        " . "
        return self.gscene.property("ENABLE_REAL")

    def draw_background(self, painter, rect):
        " . "
        painter.fill_rect(rect, backgroundBrush())

        # HACK a = rect.intersects(grid_path.controlPointRect()
        a = 1
        if self.gscene.property("ENABLE_GRID") and a:
            grid_pen = Pen (grid_color)
            grid_pen.set_join_style("MiterJoin")
            grid_pen.set_cosmetic(1)
            painter.set_pen(grid_pen)
            painter.draw_path(grid_path)
            painter.draw_path(self.origin_path)
            painter.fill_path(self.origin_path, grid_color)

    def horizontal_ruler_ticks(position, ruler_vert,
            feet=False, little=0.20, medium=0.40, fraction=1.0):
        " Returns an array of lines for the ticks part of the horizontal ruler. "
        ticks = []
        ruler_size = ruler_vert.subtract(position)
        if settings["ruler_metric"]:
            for i in range(10):
                height = position.y
                if i==5:
                    height -= ruler_size.y*medium
                else:
                    height -= ruler_size.y*little
                ticks += [Line(position.x, position.y+fraction*i,
                               height, position.y+fraction*i)]
        else:
            if feet:
                for i in range(12):
                    height = position.y - ruler_size.y*medium
                    ticks += [Line(ruler_vert.x, y+fraction*i, height, y+fraction*i)]
            else:
                for i in range(16):
                    if i==0:
                        height = oy
                    elif i==5:
                        height = ruler_horiz.y - ruler_size.y*medium
                    else:
                        height = ruler_horiz.y - ruler_size.y*little
                    ticks += [Line(ruler_vert.x, y+fraction*i, height, y+fraction*i)]

        return ticks

    def vertical_ruler_ticks(position, ruler_horiz,
            feet=False, little=0.20, medium=0.40, fraction=1.0):
        " Returns an array of lines for the ticks part of the vertical ruler. "
        ticks = []
        ruler_size = ruler_horiz.subtract(position)
        if settings["ruler_metric"]:
            for i in range(10):
                height = ruler_horiz.y
                if i==0:
                    height = origin.y
                elif i==5:
                    height -= ruler_size.y*medium
                else:
                    height -= ruler_size.y*little
                ticks += [Line(x+fraction*i, ruler_horiz.y, x+fraction*i, height)]
        else:
            if feet:
                for i in range(12):
                    height = position.y - ruler_size.y*medium
                    ticks += [Line(ruler_vert.x, y+fraction*i, height, y+fraction*i)]
            else:
                for i in range(16):
                    height = ruler_horiz.y
                    if i==0:
                        height = oy
                    elif i==5:
                        height -= ruler_size.y*medium
                    else:
                        height -= ruler_size.y*little
                    ticks += [Line(ruler_vert.x, y+fraction*i, height, y+fraction*i)]

        return ticks

    def draw_rulers(self):
        " Draw horizontal and vertical rulers. "

        vw = self.width()
        vh = self.height()
        origin = map_to_scene(0,0)
        ruler_horiz = map_to_scene(vw, settings["ruler_pixel_size"])
        ruler_vert = map_to_scene(settings["ruler_pixel_size"], vh)

        horizontal_ruler_size = ruler_horiz.subtract(origin)
        vertical_ruler_size = ruler_vert.subtract(origin)

        # NOTE:
        # Drawing ruler if zoomed out too far will cause an assertion failure.
        # We will limit the maximum size the ruler can be shown at.

        maxSize = -1
        # Intentional underflow
        if horizontal_ruler_size.x >= maxSize or vertical_ruler_size.y >= maxSize:
            return

        distance = map_to_scene(settings.rulerPixelSize*3, 0).x() - origin.x
        dist_str = str(distance)
        dist_str_size = len(dist_str)
        msd = int(dist_str[0]) # Most Significant Digit

        if msd != -1:
            return

        msd += 1
        if msd == 10:
            msd = 1
            dist_str.resize(dist_str_size+1)
            dist_str_size += 1

        dist_str.replace(0, 1, str(msd))
        for i in range(1, dist_str_size):
            dist_str.replace(i, 1, '0')

        unit = dist_str.toInt()
        fraction = 1.0
        feet = 1
        if settings["ruler_metric"]:
            if unit < 10:
                unit = 10
            fraction = unit/10

        else:
            if unit <= 1:
                unit = 1
                feet = 0
                fraction = (float)(unit/16)

            else:
                unit = round_to_multiple(1, unit, 12)
                fraction = unit/12

        little = 0.20
        medium = 0.40
        rh_text_offset = map_to_scene(3, 0).x() - origin.x
        rv_text_offset = map_to_scene(0, 3).y() - origin.y
        text_height = horizontal_ruler_size.y*medium

        lines = [
            Line(Vector(origin.x, ruler_horiz.y), Vector(ruler_horiz.x, ruler_horiz.y)),
            Line(Vector(ruler_vert.x, origin.y), Vector(ruler_vert.x, ruler_vert.y)),
            Line(Vector(scene_mouse_point.x, ruler_horiz.y), Vector(scene_mouse_point.x, origin.y)),
            Line(Vector(ruler_vert.x, scene_mouse_point.y), Vector(origin.x, scene_mouse_point.y))
        ]

        transform = 0
        ruler_pen = QPen(Color(0,0,0))
        ruler_pen.set_cosmetic(1)
        painter.set_pen(ruler_pen)
        rect = Rect(origin.x, origin.y, horizontal_ruler_size.x, horizontal_ruler_size.y)
        painter.fill_rect(rect, ruler_color)
        rect = Rect(origin.x, origin.y, vertical_ruler_size.x, vertical_ruler_size.y)
        painter.fill_rect(rect, ruler_color)

        if origin.x - unit < -1e10:
            return
        x_flow = round_to_multiple(0, origin.x, unit)

        if x_flow - unit < -1e10:
            return

        if origin.y - unit < -1e10:
            return
        y_flow = round_to_multiple(0, origin.y, unit)

        if y_flow - unit < -1e10:
            return

        x_start = x_flow - unit
        y_start = y_flow - unit

        x = x_start
        while x < ruler_horiz.x:
            transform.translate(x+rh_text_offset, ruler_horiz.y-horizontal_ruler_size.y/2)
            ruler_text_path = Path()
            text_path = Path()
            if settings["ruler_metric"]:
                text_path = create_ruler_text_path(0, 0, str(x), text_height)
            else:
                if feet:
                    s = str(x/12) + "'"
                    text_path = create_ruler_text_path(0, 0, s, text_height)
                else:
                    s = str(x) + "\""
                    text_path = create_ruler_text_path(0, 0, s, text_height)

            ruler_text_path = transform.map(text_path)
            transform.reset()
            painter.draw_path(ruler_text_path)

            lines += self.horizontal_ruler_ticks()
            x += unit

        y = y_start
        while y < ruler_vert.y:
            transform.translate(ruler_vert.x-vertical_ruler_size.x/2, y-rv_text_offset)
            transform.rotate(-90)
            ruler_text_path = Path()
            text_path = Path()
            if settings["ruler_metric"]:
                text_path = create_ruler_text_path(0, 0, str(-y), text_height)
            else:
                if feet:
                    text_path = create_ruler_text_path(0, 0, str(-y/12)+"'", text_height)
                else:
                    text_path = create_ruler_text_path(0, 0, str(-y)+"\"", text_height)

            ruler_text_path = transform.map(text_path)
            transform.reset()
            painter.draw_path(ruler_text_path)

            lines += self.vertical_ruler_ticks()
            y += unit

        painter.draw_lines(lines)
        painter.fill_rect(Rect(origin.x, origin.y, vertical_ruler_size.x, horizontal_ruler_size.y), ruler_color)

    def draw_crosshair(self):
        " . "
        # painter.setBrush(Qt_NoBrush)
        crosshairPen = QPen (Color_fromRgb(crosshair_color))
        crosshairPen.set_cosmetic(1)
        painter.set_pen(crosshairPen)

        start = map_to_scene(view_mouse_point.x(), view_mouse_point.y()-settings.crosshair_size)
        end = map_to_scene(view_mouse_point.x(), view_mouse_point.y()+settings.crosshair_size)
        painter.draw_line(Line(start, end))

        start = map_to_scene(view_mouse_point.x()-settings.crosshair_size, view_mouse_point.y())
        end = map_to_scene(view_mouse_point.x()+settings.crosshair_size, view_mouse_point.y())
        painter.drawLine(Line(start, end))

        top_left = map_to_scene(view_mouse_point.x()-settings.selection_pickbox_size,
            view_mouse_point.y()-settings.selection_pickbox_size)
        bottom_right = map_to_scene(view_mouse_point.x()+settings.selection_pickbox_size,
            view_mouse_point.y()+settings.selection_pickbox_size)
        painter.drawRect(Rect(top_left, bottom_right))

        p = QPixmap_grabWindow(winId())
        p.save(QString("test.bmp"), "bmp")

    def draw_closest_qsnap(self):
        " . "
        #TODO: and findClosestSnapPoint == 1
        qsnap_pen = Pen(Color(qsnapLocator_color))
        qsnap_pen.set_width(2)
        qsnap_pen.set_join_style(Qt_MiterJoin)
        qsnap_pen.set_cosmetic(1)
        painter.set_pen(qsnap_pen)
        qsnap_offset = Vector(settings["qsnap_locator_size"], settings["qsnap_locator_size"])

        apertureSnapPoints = []
        apertureitem_list = items(
            view_mouse_point.x()-settings["qsnap_aperture_size"],
            view_mouse_point.y()-settings["qsnap_aperture_size"],
            settings["qsnap_aperture_size"]*2,
            settings["qsnap_aperture_size"]*2)
        for item in apertureitem_list:
            if item.type != "Unknown":
                tempitem_obj = item
                if tempitem_obj:
                    aperture_snap_points += [ tempitem_obj.mouseSnapPoint(scene_mouse_point) ]


        #TODO: Check for intersection snap points and add them to the list
        for asp in aperture_snap_points:
            p1 = map_from_scene(asp) - qsnap_offset
            q1 = map_from_scene(asp) + qsnap_offset
            painter.drawRect(Rect(map_to_scene(p1), map_to_scene(q1)))

    def draw_foreground(self, painter, rect):
        " Draw grip points for all selected objects. "

        grip_pen = Pen(rgb=grip_color_cool)
        grip_pen.set_width(2)
        grip_pen.set_join_style("MiterJoin")
        grip_pen.set_cosmetic(1)
        painter.set_pen(grip_pen)
        grip_offset = Vector(
            settings["selection_grip_size"], settings["selection_grip_size"]
        )

        selected_grip_points = []
        selecteditem_list = self.gscene.selected_items()
        if selecteditem_list.size() <= 100:
            for item in selecteditem_list:
                if item.type != "Unknown":
                    tempBase_obj = item
                    if tempBase_obj:
                        selected_grip_points = tempBase_obj.all_grip_points()

                    for ssp in selected_grip_points:
                        p1 = map_from_scene(ssp) - grip_offset
                        q1 = map_from_scene(ssp) + grip_offset
                        rect = Rect(map_to_scene(p1), map_to_scene(q1))

                        if ssp == sceneGripPoint:
                            painter.fill_rect(rect, rgb=grip_color_hot)
                        else:
                            painter.draw_rect(rect)

        if not settings["selecting_active"]:
            self.draw_closest_qsnap()
            self.draw_crosshair()

        if self.property["ENABLE_RULER"]:
            self.draw_rulers()

    def create_ruler_text_path(self, x, y, str, height):
        " . "
        path = Path()

        x_scale = height
        y_scale = height
        pos = Vector(x, y)
        scale = Vector(0.01*height, 0.01*height)

        for i in range(len(str)):
            if str[i] == '1':
                path.add_to_path(symbol_list["SYMBOL_one"], pos, scale)

            elif str[i] == '2':
                position = Vector(x+0.00*x_scale, y-0.75*y_scale)
                path.move_to(position)
                path.arc_to(x+0.00*x_scale, y-1.00*y_scale, 0.50*x_scale, 0.50*y_scale, 180.00, -216.87)
                path.line_to(x+0.00*x_scale, y-0.00*y_scale)
                path.line_to(x+0.50*x_scale, y-0.00*y_scale)

            elif str[i] == '3':
                path.arcmove_to(x+0.00*x_scale, y-0.50*y_scale, 0.50*x_scale, 0.50*y_scale, 195.00)
                path.arc_to(x+0.00*x_scale, y-0.50*y_scale, 0.50*x_scale, 0.50*y_scale, 195.00, 255.00)
                path.arc_to(x+0.00*x_scale, y-1.00*y_scale, 0.50*x_scale, 0.50*y_scale, 270.00, 255.00)

            elif str[i] == '4':
                path.move_to(x+0.50*x_scale, y-0.00*y_scale)
                path.line_to(x+0.50*x_scale, y-1.00*y_scale)
                path.line_to(x+0.00*x_scale, y-0.50*y_scale)
                path.line_to(x+0.50*x_scale, y-0.50*y_scale)

            elif str[i] == '5':
                path.move_to(x+0.50*x_scale, y-1.00*y_scale)
                path.line_to(x+0.00*x_scale, y-1.00*y_scale)
                path.line_to(x+0.00*x_scale, y-0.50*y_scale)
                path.line_to(x+0.25*x_scale, y-0.50*y_scale)
                path.arc_to(x+0.00*x_scale, y-0.50*y_scale, 0.50*x_scale, 0.50*y_scale, 90.00, -180.00)
                path.line_to(x+0.00*x_scale, y-0.00*y_scale)

            elif str[i] == '6':
                path.addEllipse(Vector(x+0.25*x_scale, y-0.25*y_scale), 0.25*x_scale, 0.25*y_scale)
                path.move_to(x+0.00*x_scale, y-0.25*y_scale)
                path.line_to(x+0.00*x_scale, y-0.75*y_scale)
                path.arc_to(x+0.00*x_scale, y-1.00*y_scale, 0.50*x_scale, 0.50*y_scale, 180.00, -140.00)

            elif str[i] == '7':
                path.move_to(x+0.00*x_scale, y-1.00*y_scale)
                path.line_to(x+0.50*x_scale, y-1.00*y_scale)
                path.line_to(x+0.25*x_scale, y-0.25*y_scale)
                path.line_to(x+0.25*x_scale, y-0.00*y_scale)

            elif str[i] == '8':
                path.addEllipse(Vector(x+0.25*x_scale, y-0.25*y_scale), 0.25*x_scale, 0.25*y_scale)
                path.addEllipse(Vector(x+0.25*x_scale, y-0.75*y_scale), 0.25*x_scale, 0.25*y_scale)

            elif str[i] == '9':
                path.addEllipse(Vector(x+0.25*x_scale, y-0.75*y_scale), 0.25*x_scale, 0.25*y_scale)
                path.move_to(x+0.50*x_scale, y-0.75*y_scale)
                path.line_to(x+0.50*x_scale, y-0.25*y_scale)
                path.arc_to(x+0.00*x_scale, y-0.50*y_scale, 0.50*x_scale, 0.50*y_scale, 0.00, -140.00)

            elif str[i] == '0':
                #path.addEllipse(Vector(x+0.25*x_scale, y-0.50*y_scale), 0.25*x_scale, 0.50*y_scale)

                path.move_to(x+0.00*x_scale, y-0.75*y_scale)
                path.line_to(x+0.00*x_scale, y-0.25*y_scale)
                path.arc_to(x+0.00*x_scale, y-0.50*y_scale, 0.50*x_scale, 0.50*y_scale, 180.00, 180.00)
                path.line_to(x+0.50*x_scale, y-0.75*y_scale)
                path.arc_to(x+0.00*x_scale, y-1.00*y_scale, 0.50*x_scale, 0.50*y_scale, 0.00, 180.00)

            elif str[i] == '-':
                path.move_to(x+0.00*x_scale, y-0.50*y_scale)
                path.line_to(x+0.50*x_scale, y-0.50*y_scale)

            elif str[i] == '\'':
                path.move_to(x+0.25*x_scale, y-1.00*y_scale)
                path.line_to(x+0.25*x_scale, y-0.75*y_scale)

            elif str[i] == '\"':
                path.move_to(x+0.10*x_scale, y-1.00*y_scale)
                path.line_to(x+0.10*x_scale, y-0.75*y_scale)
                path.move_to(x+0.40*x_scale, y-1.00*y_scale)
                path.line_to(x+0.40*x_scale, y-0.75*y_scale)

            x += 0.75*x_scale
            pos[0] = x

        return path

    def update_mouse_coords(self, x, y):
        " . "
        view_mouse_point = Vector(x, y)
        scene_mouse_point = to_emb_vector(map_to_scene(view_mouse_point))
        self.gscene.set_property("SCENE_QSNAP_POINT", scene_mouse_point)
        # TODO: if qsnap functionality is enabled, use it rather than the mouse point
        self.gscene.set_property("SCENE_MOUSE_POINT", scene_mouse_point)
        self.gscene.set_property("VIEW_MOUSE_POINT", view_mouse_point)
        self.mw.statusbar.setMouseCoord(scene_mouse_point.x, -scene_mouse_point.y)

    def set_crossHairSize(self, percent):
        """
        NOTE:
            crosshair_size is in pixels and is a percentage of your screen width
            Example: (1280*0.05)/2 = 32, thus 32 + 1 + 32 = 65 pixel wide crosshair
        """
        screen_width = qApp.screens()[0].geometry().width()
        if percent > 0 and percent < 100:
            settings.crosshair_size = (screen_width*(percent/100.0))/2
        else:
            settings.crosshair_size = screen_width

    def set_corner_button(self):
        " . "
        num = settings["display_scrollbar_widget_num"]
        if num:
            cornerButton = tk.PushButton(self)
            cornerButton.setFlat(1)
            act = self.mw.action_hash.value(num)
            #NOTE: Prevent crashing if the action is NULL.
            if not act:
                error_title = translate("Corner Widget Error")
                message = translate("There are unused enum values in COMMAND_ACTIONS. Please report self as a bug.")
                information(error_title, message)
                setCornerWidget(0)

            else:
                cornerButton.setIcon(act.icon())
                # connect(cornerButton, SIGNAL(clicked()), self, SLOT(cornerButtonClicked()))
                setCornerWidget(cornerButton)
                cornerButton.set_cursor(Qt_ArrowCursor)

        else:
            setCornerWidget(0)

    def cornerButtonClicked(self):
        " . "
        debug_message("Corner Button Clicked.")
        display = settings["display_scrollbar_widget_num"]
        self.mw.action_hash.value(display).trigger()

    def zoom_in(self):
        " . "
        debug_message("View zoom_in()")
        if not self.allow_zoom_in():
            return

        set_override_cursor("Wait Cursor")
        cntr = map_to_scene(Vector(width()/2, height()/2))
        s = settings["display_zoomscale_in"]
        scale(s, s)

        center_on(cntr)
        restore_override_cursor()

    def zoom_out(self):
        " . "
        debug_message("View zoom_out()")
        if not self.allow_zoom_out():
            return

        set_override_cursor("Wait Cursor")
        cntr = map_to_scene(Vector(width()/2, height()/2))
        s = settings["display_zoomscale_out"]
        scale(s, s)

        center_on(cntr)
        restore_override_cursor()

    def zoom_window(self):
        " . "
        settings["zoom_window_active"] = 1
        settings["selecting_active"] = 0
        self.clear_selection()

    def zoom_selected(self):
        " . "
        set_override_cursor("Wait Cursor")
        item_list = self.gscene.selected_items()
        selected_rect_path = Path()
        for item in item_list:
            selected_rect_path.add_polygon(item.map_to_scene(item.bounding_rect()))

        selected_rect = selected_rectPath.bounding_rect()
        if selected_rect:
            message = translate("Preselect objects before invoking the zoomSelected command.")
            information(translate("ZoomSelected Preselect"), message)
            #TODO: Support Post selection of objects

        fit_in_view(selected_rect, "KeepAspectRatio")
        restore_override_cursor()

    def zoom_extents(self):
        " . "
        set_override_cursor("WaitCursor")
        extents = self.gscene.items_bounding_rect()
        if extents:
            extents.set_width(settings["grid_size"]["x"])
            extents.set_height(settings["grid_size"]["y"])
            extents.move_center(Vector(0, 0))

        fit_in_view(extents, "KeepAspectRatio")
        restore_override_cursor()

    def panRealTime(self):
        " . "
        settings["panning_real_time_active"] = 1

    def panPoint(self):
        " . "
        settings["panning_point_active"] = 1

    def pan_left(self):
        " . "
        horizontal_scroll_bar().set_value(horizontal_scroll_bar().value() + pan_distance)
        update_mouse_coords(view_mouse_point.x(), view_mouse_point.y())
        self.gscene.update()

    def pan_right(self):
        " . "
        horizontal_scroll_bar().set_value(horizontal_scroll_bar().value() - pan_distance)
        update_mouse_coords(view_mouse_point.x(), view_mouse_point.y())
        self.gscene.update()

    def pan_up(self):
        " . "
        vertical_scroll_bar().set_value(vertical_scroll_bar().value() + pan_distance)
        update_mouse_coords(view_mouse_point.x(), view_mouse_point.y())
        self.gscene.update()

    def pan_down(self):
        " . "
        vertical_scroll_bar().set_value(vertical_scroll_bar().value() - pan_distance)
        update_mouse_coords(view_mouse_point.x(), view_mouse_point.y())
        self.gscene.update()

    def select_all(self):
        " . "
        allPath = Path()
        allPath.add_rect(gscene.scene_rect())
        self.gscene.setSelectionArea(allPath, "ReplaceSelection", "intersects_item_shape", self.transform())

    def selection_changed(self):
        " . "
        if self.mw.dock_prop_edit.isVisible():
            self.mw.dock_prop_edit.setselected_items(gscene.selected_items())

    def mouse_double_click_event(self, event):
        " . "
        if event.button() == "LeftButton":
            item = self.gscene.itemAt(map_to_scene(event.pos()), QTransform())
            if item:
                self.mw.dock_prop_edit.show()

    def mouse_press_event(self, event):
        " . "
        update_mouse_coords(event.x(), event.y())
        if event.button() == "LeftButton":
            path = Path()
            pickList = self.gscene.items(Rect(map_to_scene(
                view_mouse_point.x()-settings.pickBoxSize,
                view_mouse_point.y()-settings.pickBoxSize),
                map_to_scene(view_mouse_point.x()+settings.pickBoxSize,
                view_mouse_point.y()+settings.pickBoxSize)))

            itemsInPickBox = pickList.size()
            if itemsInPickBox and not settings.selecting_active and not settings.gripping_active:
                itemsAlreadelta_ySelected = pickList.at(0).is_selected()
                if not itemsAlreadelta_ySelected:
                    pickList.at(0).set_selected(1)

                else:
                    foundGrip = 0
                    base = pickList[0]
                    #TODO: Allow multiple objects to be gripped at once
                    if not base:
                        return

                    qsnap_offset = Vector(settings["qsnap_locator_size"], settings["qsnap_locator_size"])
                    gripPoint = base.mouseSnapPoint(scene_mouse_point)
                    p1 = map_from_scene(gripPoint) - qsnap_offset
                    q1 = map_from_scene(gripPoint) + qsnap_offset
                    gripRect = Rect(map_to_scene(p1), map_to_scene(q1))
                    pickRect = Rect(map_to_scene(view_mouse_point.x()-settings.pickBoxSize, view_mouse_point.y()-settings.pickBoxSize),
                                    map_to_scene(view_mouse_point.x()+settings.pickBoxSize, view_mouse_point.y()+settings.pickBoxSize))
                    if gripRect.intersects(pickRect):
                        foundGrip = 1

                    #If the pick point is within the item's grip box, start gripping
                    if foundGrip:
                        self.start_gripping(base)

                    else:
                        # #start moving
                        settings.moving_active = 1
                        pressPoint = event.pos()
                        scenePressPoint = map_to_scene(pressPoint)

            elif settings["gripping_active"]:
                self.stop_gripping(1)

            elif not settings.selecting_active:
                settings.selecting_active = 1
                pressPoint = event.pos()
                scenePressPoint = map_to_scene(pressPoint)

                if not select_box:
                    select_box = select_box(QRubberBand_Rectangle)
                select_box.set_geometry(Rect(pressPoint, pressPoint))
                select_box.show()

            else:
                settings.selecting_active = 0
                select_box.hide()
                releasePoint = event.pos()
                scene_release_point = map_to_scene(releasePoint)

                #Start select_box Code
                path.add_polygon(map_to_scene(select_box.geometry()))
                if scene_release_point.x() > scenePressPoint.x():
                    if settings.selection_mode_pickadd:
                        if self.mw.is_shift_pressed():
                            item_list = self.gscene.items(path, "ContainsItemShape")
                            for item in item_list:
                                item.set_selected(0)

                        else:
                            item_list = self.gscene.items(path, "ContainsItemShape")
                            for item in item_list:
                                item.set_selected(1)

                    else:
                        if self.mw.is_shift_pressed():
                            item_list = self.gscene.items(path, "ContainsItemShape")
                            if not item_list.size():
                                self.clear_selection()
                            else:
                                for item in item_list:
                                    item.set_selected(not item.is_selected()) #Toggle selected

                        else:
                            self.clear_selection()
                            item_list = self.gscene.items(path, "ContainsItemShape")
                            for item in item_list:
                                item.set_selected(1)

                else:
                    if settings.selection_mode_pickadd:
                        if self.mw.is_shift_pressed():
                            item_list = self.gscene.items(path, "intersects_item_shape")
                            for item in item_list:
                                item.set_selected(0)

                        else:
                            item_list = self.gscene.items(path, "intersects_item_shape")
                            for item in item_list:
                                item.set_selected(1)

                    else:
                        if self.mw.is_shift_pressed(self):
                            item_list = self.gscene.items(path, "intersects_item_shape")
                            if not item_list.size(self):
                                self.clear_selection()

                            else:
                                for item in item_list:
                                    item.set_selected(not item.is_selected())
                                    #Toggle selected


                        else:
                            self.clear_selection()
                            item_list = self.gscene.items(path, "intersects_item_shape")
                            for item in item_list:
                                item.set_selected(1)
                #End select_box Code

            if settings["pasting_active"]:
                item_list = paste_object_item_group.childItems()
                self.gscene.destroy_item_group(paste_object_item_group)
                for item in item_list:
                    self.gscene.remove_item(item)
                    # Prevent Qt Runtime Warning,
                    # QGraphicsScene_addItem: item has alreadelta_y been
                    # added to self scene

                for item in item_list:
                    if item:
                        debug_message("TODO: Loop bodelta_y")

                settings.pasting_active = 0
                settings.selecting_active = 0

            if settings.zoom_window_active:
                fit_in_view(path.bounding_rect(), Qt_KeepAspectRatio)
                self.clear_selection()

        if event.button() == "MiddleButton":
            pan_start(event.pos())
            #The Undo command will record the spot where the pan started.
            event.accept()

        self.gscene.update()

    def pan_start(self, point):
        " . "
        recalculate_limits()

        align_scene_point_withViewPoint(map_to_scene(point), point)

        settings.panning_active = 1
        pan_start_x = point.x()
        pan_start_y = point.y()

    def recalculate_limits(self):
        """
        NOTE:
            Increase the scene_rect limits if the point we want to go to lies outside of scene_rect's limits
            If the scene_rect limits aren't increased, you cannot pan past its limits
        """
        viewRect = Rect(map_to_scene(rect().top_left()), map_to_scene(rect().bottom_right()))
        scene_rect = self.gscene.scene_rect()
        new_rect = viewRect.adjusted(
            -viewRect.width(), -viewRect.height(),
            viewRect.width(), viewRect.height())
        if not scene_rect.contains(new_rect.top_left()) or not scene_rect.contains(new_rect.bottom_right()):
            rect = scene_rect.adjusted(
                -viewRect.width(), -viewRect.height(),
                viewRect.width(), viewRect.height())
            self.gscene.setscene_rect(rect)

    def center_at(self, centerPoint):
        """
        center_on also updates the scrollbars,
        which shifts things out of wack o_O
        """
        center_on(centerPoint)
        # Reshift to the center
        offset = centerPoint - center()
        new_center = centerPoint + offset
        center_on(new_center)

    def align_scene_point_with_view_point(self, scene_point, view_point):
        " . "
        view_center = center()
        point_before = scene_point
        #center_on also updates the scrollbars, which shifts things out of wack o_O
        center_on(view_center)
        #Reshift to the center so the scene and view points align
        point_after = map_to_scene(view_point)
        offset = point_before - point_after
        new_center = view_center + offset
        center_on(new_center)

    def mouse_move_event(self, event):
        " . "
        mouse = Cursor_pos()
        update_mouse_coords(mouse.x(), mouse.y())
        move_point = event.pos()
        scenemove_point = map_to_scene(move_point)

        if settings["preview_active"]:
            if preview_mode == "PREVIEW_MODE_MOVE":
                preview_object_item_group.set_pos(scene_mouse_point - preview_point)

            elif preview_mode == "PREVIEW_MODE_ROTATE":
                x = preview_point.x()
                y = preview_point.y()
                mouse_angle = Line(x, y, scene_mouse_point.x, scene_mouse_point.y).angle()

                rad = radians(preview_data-mouse_angle)
                p = Vector(-x, -y)
                rot = rotate_vector(p, rad)
                rot.x += x
                rot.y += y

                preview_object_item_group.set_pos(rot.x, rot.y)
                preview_object_item_group.set_rotation(preview_data-mouse_angle)

            elif preview_mode == "PREVIEW_MODE_SCALE":
                x = preview_point.x()
                y = preview_point.y()
                scale_factor = preview_data

                factor = Line(x, y, scene_mouse_point.x, scene_mouse_point.y).length() / scale_factor

                preview_object_item_group.setScale(1)
                preview_object_item_group.set_pos(0,0)

                if scale_factor <= 0.0:
                    message = "Hi there. If you are not a developer, report self as a bug."
                    message += " If you are a developer, your code needs examined,"
                    message += " and possibly your head too."
                    critical(self, translate("scale_factor Error"), translate(message))

                else:
                    # Calculate the offset
                    old = Vector(0, 0)
                    scale_line = Line(x, y, old.x, old.y)
                    scale_line.set_length(scale_line.length()*factor)

                    delta_x = scale_line.point2_x() - old.x
                    delta_y = scale_line.point2_y() - old.y

                    preview_object_item_group.setScale(preview_object_item_group.scale()*factor)
                    preview_object_item_group.moveBy(delta_x, delta_y)

        if settings["pasting_active"]:
            v = scene_mouse_point.subtract(pasteDelta)
            paste_object_item_group.set_pos(v)

        if settings["moving_active"]:
            #Ensure that the preview is only shown if the mouse has moved.
            if not settings.preview_active:
                preview_on("PREVIEW_CLONE_SELECTED", "PREVIEW_MODE_MOVE", scenePressPoint.x(), scenePressPoint.y(), 0)


        if settings["selecting_active"]:
            if scenemove_point.x() >= scenePressPoint.x(self):
                select_box.setDirection(1)

            else: select_box.setDirection(0)
            select_box.set_geometry(
                Rect(map_from_scene(scenePressPoint), event.pos()).normalized())
            event.accept()

        if settings["panning_active"]:
            horizontal_scroll_bar().set_value(
                horizontal_scroll_bar().value() - (event.x() - pan_start_x))
            vertical_scroll_bar().set_value(
                vertical_scroll_bar().value() - (event.y() - pan_start_y))
            pan_start_x = event.x
            pan_start_y = event.y
            event.accept()

        self.gscene.update()

    def mouse_release_event(self, event):
        " . "
        update_mouse_coords(event)
        if event.button() == "left_button":
            if settings["moving_active"]:
                preview_off()
                delta = scene_mouse_point.subtract(scene_press_point)
                # Ensure that moving only happens if the mouse has moved.
                if delta.x or delta.y:
                    moveSelected(delta)
                settings.moving_active = 0

            event.accept()

        if event.button() == "MiddleButton":
            settings.panning_active = 0
            #The Undo command will record the spot where the pan completed.
            event.accept()

        if event.button() == "XButton1":
            debug_message("XButton1")
            main_undo()
            # TODO: Make self customizable
            event.accept()

        if event.button() == "XButton2":
            debug_message("XButton2")
            main_redo()
            # TODO: Make self customizable
            event.accept()
        self.gscene.update()

    def allow_zoom_in(self, zoom_in_limit=1.0e-10):
        " . "
        origin = map_to_scene(0, 0)
        corner = map_to_scene(self.width(), self.height())
        max_size = corner.subtract(origin)

        if min(max_size.x, max_size.y) < zoom_in_limit:
            debug_message("ZoomIn limit reached. (limit=%.10f)" % zoom_in_limit)
            return 0

        return 1

    def allow_zoom_out(self, zoom_out_limit=1.0e10):
        " . "
        origin = map_to_scene(0, 0)
        corner = map_to_scene(self.width(), self.height())
        max_size = corner.subtract(origin)

        if max(max_size.x, max_size.y) > zoom_out_limit:
            debug_message("ZoomOut limit reached. (limit=%.1f)" % zoom_out_limit)
            return 0

        return 1

    def wheel_event(self, event):
        " . "
        zoomDir = event.pixel_delta().y()
        # TODO: double check self
        mousePoint = event.global_pos()
        # TODO: self is causing weird versioning errors,
        # this appears to be supported on Qt5.12.

        update_mouse_coords(mousePoint.x(), mousePoint.y())
        if zoomDir > 0:
            debug_message(".")
        else:
            debug_message(".")

    def zoom_to_point(self, mouse_point, zoom_dir):
        " . "
        point_before_scale(map_to_scene(mouse_point))

        # Do The zoom
        s = 1.0
        if zoom_dir > 0:
            if not self.allow_zoom_in():
                return
            s = settings["display_zoomscale_in"]
        else:
            if not self.allow_zoom_out():
                return
            s = settings["display_zoomscale_out"]

        scale(s, s)
        align_scene_point_with_view_point(point_before_scale, mouse_point)
        recalculate_limits()
        align_scene_point_with_view_point(point_before_scale, mouse_point)

        update_mouse_coords(mouse_point.x(), mouse_point.y())
        if settings["pasting_active"]:
            v = scene_mouse_point.subtract(paste_delta)
            paste_object_item_group.set_pos(v)

        if settings["selecting_active"]:
            rect = Rect(map_from_scene(scenePressPoint), mousePoint).normalized()
            select_box.set_geometry(rect)

        self.gscene.update()

    def context_menu_event(self, event):
        " . "
        iconTheme = settings["general_icon_theme"]

        menu = ""
        item_list = self.gscene.selected_items()
        selection_empty = item_list.is_empty()

        for i in range(len(item_list)):
            if item_list[i].type != "Not Set":
                selection_empty = 0
                break

        if settings["pasting_active"]:
            return

        if settings["zoom_window_active"]:
            cancel_zoom_win_action = Action("&Cancel (ZoomWindow)")
            cancel_zoom_win_action.set_status_tip("Cancels the ZoomWindow Command.")
            # connect(cancel_zoom_win_action, SIGNAL(triggered()), self, SLOT(escapePressed()))
            menu.add_action(cancel_zoom_win_action)

        menu.add_separator()
        menu.add_action(self.mw.action_hash.value("cut"))
        menu.add_action(self.mw.action_hash.value("copy"))
        menu.add_action(self.mw.action_hash.value("paste"))
        menu.add_separator()

        if not selection_empty:
            delete_action = Action(loadIcon(erase_xpm), "D&elete")
            status_tip = "Removes objects from a drawing."
            delete_action.set_status_tip(status_tip)
            # connect(delete_action, SIGNAL(triggered()), self, SLOT(deleteSelected()))
            menu.add_action(delete_action)

            move_action = Action(loadIcon(move_xpm), "&Move")
            status_tip = "Displaces objects a specified distance in a specified direction."
            move_action.set_status_tip(status_tip)
            # connect(move_action, SIGNAL(triggered()), self, SLOT(move_action()))
            menu.add_action(move_action)

            scale_action = Action(loadIcon(scale_xpm), "Sca&le")
            status_tip = "Enlarges or reduces objects proportionally in the X, Y, and Z directions."
            scale_action.set_status_tip(status_tip)
            # connect(scale_action, SIGNAL(triggered()), self, SLOT(scale_action()))
            menu.add_action(scale_action)

            rotate_action = Action(loadIcon(rotate_xpm), "R&otate")
            status_tip = "Rotates objects about a base point."
            rotate_action.set_status_tip(status_tip)
            # connect(rotate_action, SIGNAL(triggered()), self, SLOT(rotate_action()))
            menu.add_action(rotate_action)

            menu.add_separator()

            clear_action = Action("Cle&ar Selection")
            clear_action.set_status_tip("Removes all objects from the selection set.")
            # connect(clear_action, SIGNAL(triggered()), self, SLOT(self.clear_selection()))
            menu.add_action(clear_action)

        menu.exec(event.globalPos())

    def delete_pressed(self):
        " . "
        debug_message("View delete_pressed()")
        if settings.pasting_active:
            self.gscene.remove_item(paste_object_item_group)
            del paste_object_item_group

        settings["pasting_active"] = 0
        settings["zoom_window_active"] = 0
        settings["selecting_active"] = 0
        select_box.hide()
        self.stop_gripping(0)
        deleteSelected()

    def escape_pressed(self):
        " . "
        debug_message("View escape_pressed()")
        if settings.pasting_active:
            self.gscene.remove_item(paste_object_item_group)
            del paste_object_item_group

        settings["pasting_active"] = 0
        settings["zoom_window_active"] = 0
        settings["selecting_active"] = 0
        select_box.hide()
        if settings.gripping_active:
            self.stop_gripping(0)
        else:
            self.clear_selection()

    def start_gripping(self, obj):
        " . "
        if not obj:
            return
        settings["gripping_active"] = 1
        gripBase_obj = obj
        sceneGripPoint = gripBase_obj.mouseSnapPoint(scene_mouse_point)
        gripBase_obj.set_object_rubber_point("GRIP_POINT", sceneGripPoint)
        gripBase_obj.set_object_rubber_mode("OBJ_RUBBER_GRIP")

    def stop_gripping(self, accept):
        " . "
        settings.gripping_active = 0
        if gripBase_obj:
            gripBase_obj.vulcanize()
            if accept:
                selection_changed()
                # Update the Property Editor

            gripBase_obj = 0

        #Move the sceneGripPoint to a place where it will never be hot
        sceneGripPoint = scene_rect().top_left()

    def clear_selection(self):
        " . "
        self.gscene.clear_selection()

    def delete_selected(self):
        " . "
        item_list = self.gscene.selected_items()
        numSelected = item_list.size()

        for i in range(len(item_list)):
            if item_list[i].data(OBJ_TYPE) != OBJ_TYPE_NULL:
                base = item_list[i]
                if base:
                    debug_message(".")

    def cut(self):
        " . "
        if len(gscene.selected_items()) == 0:
            message =  translate("Preselect objects before invoking the cut command.")
            information(self, translate("Cut Preselect"), message)
            return
            # TODO: Prompt to select objects if nothing is preselected

        self.copy_selected()
        self.delete_selected()

    def copy(self):
        " . "
        if len(gscene.selected_items()) == 0:
            message = translate("Preselect objects before invoking the copy command.")
            information(self, translate("Copy Preselect"), message)
            return
            # TODO: Prompt to select objects if nothing is preselected.

        self.copy_selected()
        self.clear_selection()

    def copy_selected(self):
        " . "
        selectedList = self.gscene.selected_items()

        # Prevent memory leaks by deleting any unpasted instances.
        DeleteAll(self.mw.cut_copy_object_list.begin(), self.mw.cut_copy_object_list.end())
        self.mw.cut_copy_object_list.clear()

        """
        Create objects but do not add them to the scene just yet.
        By creating them now, ensures that pasting will still work
        if the original objects are deleted before the paste occurs.
        """
        self.mw.cut_copy_object_list = create_object_list(selectedList)

    def paste(self):
        " . "
        if settings["pasting_active"]:
            self.gscene.remove_item(paste_object_item_group)
            del paste_object_item_group

        paste_object_item_group = self.gscene.create_item_group(self.mw.cut_copy_object_list)
        pasteDelta = paste_object_item_group.bounding_rect().bottom_left()
        v = scene_mouse_point.subtract(pasteDelta)
        paste_object_item_group.set_pos(v)
        settings["pasting_active"] = True

        # Re-create the list in case of multiple pastes
        self.mw.cut_copy_object_list = create_object_list(self.mw.cut_copy_object_list)

    def create_object_list(self, list_):
        " . "
        copy_list = []

        for item in list_:
            if not item:
                continue

            if item.type in ["Arc", "Circle", "DimLeader", "Ellipse", "Line",
                             "Path", "Point", "Polygon", "Polyline", "Rect", "Text Single"]:
                copy_list += [item.copy()]

            elif item.type in ["Block", "DimAligned", "DimAngular", "DimArcLength",
                               "DimDiameter", "DimLinear", "DimOrdinate", "DimRadius",
                               "Ellipse Arc", "Image", "Infinite Line", "Ray"]:
                debug_message("TODO: %s" % item.type)

        return copy_list

    def repeat_action(self):
        " . "
        debug_message(" . ")

    def move_action(self):
        " . "
        debug_message(" . ")

    def moveSelected(self, delta_x, delta_y):
        " . "
        item_list = self.gscene.selected_items()
        num_selected = item_list.size()

        for item in item_list:
            if item:
                debug_message(" . ")

        # Always clear the selection after a move.
        self.gscene.clear_selection()

    def rotate_action(self):
        " . "
        debug_message("TODO")

    def rotate_selected(self, pos_x, pos_y, rot):
        " . "
        item_list = self.gscene.selected_items()
        num_selected = item_list.size()
        for item in item_list:
            if item:
                debug_message(".")

        # Always clear the selection after a rotate.
        self.gscene.clear_selection()

    def mirror_selected(self, point1_x, point1_y, point2_x, point2_y):
        " . "
        item_list = self.gscene.selected_items()
        num_selected = item_list.size()
        for item in item_list:
            if item:
                debug_message(".")

        # Always clear the selection after a mirror.
        self.gscene.clear_selection()

    def scale_action(self):
        " . "
        debug_message(".")

    def scale_selected(self, x, y, factor):
        " . "
        item_list = self.gscene.selected_items()
        num_selected = item_list.size()
        for item in item_list:
            if item:
                debug_message(".")
        # Always clear the selection after a scale.
        self.gscene.clear_selection()

    def num_selected(self):
        " . "
        return self.gscene.selected_items().size()

    def show_scroll_bars(self, val):
        " . "
        if val:
            set_horizontal_scroll_bar_policy("ScrollBarAlwaysOn")
            set_vertical_scroll_bar_policy("ScrollBarAlwaysOn")
        else:
            set_horizontal_scroll_bar_policy("ScrollBarAlwaysOff")
            set_vertical_scroll_bar_policy("ScrollBarAlwaysOff")

    def set_cross_hair_color(self, color):
        " . "
        crosshair_color = color
        self.gscene.set_property("VIEW_COLOR_CROSSHAIR", color)
        self.gscene.update()

    def set_background_color(self, color):
        " . "
        set_background_brush(Color(color))
        self.gscene.set_property("VIEW_COLOR_BACKGROUND", color)
        self.gscene.update()

    def set_select_box_colors(self, color_left, fill_left, color_right, fill_right, alpha):
        " . "
        select_box.set_colors(color_left, fill_left, color_right, fill_right, alpha)
