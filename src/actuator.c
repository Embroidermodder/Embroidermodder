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

#include "em2.h"

/*
 * ACTUATOR
 *
 * In order to have a complex version of saving work, with backups,
 * undo history and forks we need a good recording of what has happened.
 *
 * An action has been taken, we are at the current head of the stack.
 *
 * The action string is command similar to how a command
 * line works: seperated by spaces we have a function followed
 * by arguments.
 */
int
actuator(EmbWindow *window, char *action)
{
    int undo_history_max = MAX_CSV_ROWS;
    int undo_history_chunk_size = 10;
    window->undo_history_position++;
    if (undo_history_max <= window->undo_history_position) {
        int i;
        int before_end_chunk = undo_history_max - undo_history_chunk_size;
        for (i=0; i<before_end_chunk; i++) {
            strcpy(window->undo_history[i][0], window->undo_history[i+undo_history_chunk_size][0]);
        }
        window->undo_history_position -= undo_history_chunk_size;
    }
    
    strcpy(window->undo_history[window->undo_history_position][0], action);

    printf("action: %s\n", action);
    /* Command/argument seperation is done in order to reduce the number of
     * checks on every click before an action can run.
     *
     * File actions can be grouped under starts_with "file" etc.
     */
    if (starts_with(action, "do-nothing")) {
        debug_message("This action intentionally does nothing.");
        return 0;
    }
    if (starts_with(action, "menu-state")) {
        if (!strcmp(action+11, "file")) {
            window->menu_state = FILE_MENU;
            return 0;
        }
        if (!strcmp(action+11, "edit")) {
            window->menu_state = EDIT_MENU;
            return 0;
        }
        if (!strcmp(action+11, "view")) {
            window->menu_state = VIEW_MENU;
            return 0;
        }
        if (!strcmp(action+11, "settings")) {
            window->menu_state = SETTINGS_MENU;
            return 0;
        }
        if (!strcmp(action+11, "window")) {
            window->menu_state = WINDOW_MENU;
            return 0;
        }
        if (!strcmp(action+11, "help")) {
            window->menu_state = HELP_MENU;
            return 0;
        }
        if (!strcmp(action+11, "recent")) {
            window->menu_state = RECENT_MENU;
            return 0;
        }
        if (!strcmp(action+11, "zoom")) {
            window->menu_state = ZOOM_MENU;
            return 0;
        }
        if (!strcmp(action+11, "pan")) {
            window->menu_state = PAN_MENU;
            return 0;
        }
        return 0;
    }

    if (starts_with(action, "debug-message")) {
        debug_message("This action intentionally does nothing.");
        return 0;
    }

    if (starts_with(action, "add")) {
        char *arguments = action+strlen("add")+1;
        if (starts_with(arguments, "circle")) {
            debug_message("TODO: add circle action.");
            return 0;
        }
        if (starts_with(arguments, "ellipse")) {
            debug_message("TODO: add ellipse action.");
            return 0;
        }
        if (starts_with(arguments, "path")) {
            debug_message("TODO: add path action.");
            return 0;
        }
        if (starts_with(arguments, "heart")) {
            debug_message("TODO: add heart action.");
            return 0;
        }
        if (starts_with(arguments, "treble clef")) {
            debug_message("TODO: add treble clef action.");
            return 0;
        }
        if (starts_with(arguments, "line")) {
            debug_message("TODO: add line action.");
            return 0;
        }
        if (starts_with(arguments, "dolphin")) {
            debug_message("TODO: add dolphin action.");
            return 0;
        }
        return 0;
    }

    if (starts_with(action, "tab")) {
        char *arguments = action + strlen("tab") + 1;
        if (starts_with(arguments, "snap")) {
            debug_message("TODO: open the snap tab action.");
            window->tab_index = 0;
            return 0;
        }
        if (starts_with(arguments, "grid")) {
            debug_message("TODO: open the grid tab action.");
            window->tab_index = 1;
            return 0;
        }
        if (starts_with(arguments, "ruler")) {
            debug_message("TODO: open the ruler tab action.");
            window->tab_index = 1;
            return 0;
        }
        if (starts_with(arguments, "ortho")) {
            debug_message("TODO: open the ortho tab action.");
            window->tab_index = 1;
            return 0;
        }
        if (starts_with(arguments, "polar")) {
            debug_message("TODO: open the polar tab action.");
            window->tab_index = 1;
            return 0;
        }
        if (starts_with(arguments, "snap")) {
            debug_message("TODO: open the qsnap tab action.");
            window->tab_index = 1;
            return 0;
        }
        if (starts_with(arguments, "track")) {
            debug_message("TODO: open the track tab action.");
            window->tab_index = 1;
            return 0;
        }
        if (starts_with(arguments, "lwt")) {
            debug_message("TODO: open the lwt tab action.");
            window->tab_index = 1;
            return 0;
        }
        return 0;
    }

    if (starts_with(action, "new-file")) {
        new_file(window);
        return 0;
    }
    if (starts_with(action, "open")) {
        open_file(window);
        return 0;
    }
    if (starts_with(action, "save-file")) {
        save_file(window);
        return 0;
    }
    if (starts_with(action, "save-file-as")) {
        save_file_as(window);
        return 0;
    }
    if (starts_with(action, "check-for-updates")) {
        puts("Visit https://libembroidery.org for information about new releases.");
        puts("Your version is: " VERSION);
        return 0;
    }
    if (starts_with(action, "select-all")) {
        select_all(window);
        return 0;
    }
    if (starts_with(action, "whats-this")) {
        whats_this(window);
        return 0;
    }
    if (starts_with(action, "design-details")) {
        design_details(window);
        return 0;
    }
    if (starts_with(action, "print-pattern")) {
        print_pattern(window);
        return 0;
    }
    if (starts_with(action, "exit-program")) {
        /*
         * Instead of closing using exit() this allows us to chain any checks,
         * like the confirmation of the close, to the action.
         */
        debug_message("Closing Embroidermodder 2.0.");
        window->running = 0;
        return 0;
    }

    if (starts_with(action, "cut-object")) {
        cut(window);
        return 0;
    }
    if (starts_with(action, "copy-object")) {
        copy(window);
        return 0;
    }
    if (starts_with(action, "paste-object")) {
        paste(window);
        return 0;
    }

    if (starts_with(action, "undo")) {
        undo(window);
        return 0;
    }
    if (starts_with(action, "redo")) {
        redo(window);
        return 0;
    }

    if (starts_with(action, "window")) {
        char *arguments = action + strlen("window") + 1;
        if (starts_with(arguments, "close")) {
            debug_message("window_close()");
            return 0;
        }
        if (starts_with(arguments, "close-all")) {
            debug_message("window_close_all()");
            return 0;
        }
        if (starts_with(arguments, "cascade")) {
            debug_message("window_cascade()");
            return 0;
        }
        if (starts_with(arguments, "tile")) {
            debug_message("window_tile()");
            return 0;
        }
        if (starts_with(arguments, "next")) {
            debug_message("window_next()");
            return 0;
        }
        if (starts_with(arguments, "previous")) {
            debug_message("window_previous()");
            return 0;
        }
        return 0;
    }

    if (starts_with(action, "help")) {
        help(window);
        return 0;
    }
    if (starts_with(action, "changelog-dialog")) {
        changelog(window);
        return 0;
    }
    if (starts_with(action, "tip-of-the-day-dialog")) {
        tip_of_the_day(window);
        return 0;
    }
    if (starts_with(action, "about-dialog")) {
        about(window);
        return 0;
    }

    if (starts_with(action, "icon")) {
        debug_message("icon_resize()");
        icon_resize(atoi(action+5));
        return 0;
    }

    if (starts_with(action, "settings-dialog")) {
        EmbWindow *settings = settings_dialog(window);
        return 0;
    }
    if (starts_with(action, "make-layer-current")) {
        debug_message("make_layer_current()");
        return 0;
    }
    if (starts_with(action, "layer-manager")) {
        debug_message("layers()");
        return 0;
    }
    if (starts_with(action, "layer-selector")) {
        debug_message("layer_selector()");
        return 0;
    }
    if (starts_with(action, "color-selector")) {
        color_selector(window);
        return 0;
    }
    if (starts_with(action, "line-type-selector")) {
        line_type_selector(window);
        return 0;
    }

    if (starts_with(action, "line-weight-selector")) {
        line_weight_selector(window);
        return 0;
    }

    if (starts_with(action, "layer-previous")) {
        debug_message("layer_previous()");
        return 0;
    }
    if (starts_with(action, "hide-all-layers")) {
        debug_message("hide_all_layers()");
        return 0;
    }
    if (starts_with(action, "show-all-layers")) {
        debug_message("showAllLayers()");
        return 0;
    }
    if (starts_with(action, "freeze-all-layers")) {
        debug_message("freezeAllLayers()");
        return 0;
    }
    if (starts_with(action, "thaw-all-layers")) {
        debug_message("thawAllLayers()");
        return 0;
    }
    if (starts_with(action, "lock-all-layers")) {
        debug_message("lock_all_layers()");
        return 0;
    }
    if (starts_with(action, "unlock-all-layers")) {
        debug_message("unlock_all_layers()");
        return 0;
    }
    if (starts_with(action, "enable")) {
        char *arguments = action + strlen("enable") + 1;
        EmbPanel *panel = window->panels[window->tab_index];
        if (starts_with(arguments, "grid")) {
            debug_message("Show grid");
            panel->grid = 1;
            return 0;
        }
        if (starts_with(arguments, "real")) {
            debug_message("Enable real render in this tab.");
            panel->real = 1;
            return 0;
        }
        if (starts_with(arguments, "ruler")) {
            debug_message("Show rulers in this tab.");
            panel->ruler = 1;
            return 0;
        }
        if (starts_with(arguments, "ortho")) {
            debug_message("Show orthogonal grid in this tab.");
            panel->ortho = 1;
            return 0;
        }
        if (starts_with(arguments, "qsnap")) {
            debug_message("Activate QSnap mode in this tab.");
            panel->snap = 1;
            return 0;
        }
        if (starts_with(arguments, "polar")) {
            debug_message("Activate polar grid in this tab.");
            panel->polar = 1;
            return 0;
        }
        if (starts_with(arguments, "track")) {
            debug_message("Activate QTrack mode in this tab.");
            panel->qtrack = 1;
            return 0;
        }
        if (starts_with(arguments, "lwt")) {
            debug_message("Show Lineweight in this tab.");
            panel->lwt = 1;
            return 0;
        }
        return 0;
    }

    if (starts_with(action, "disable")) {
        char *arguments = action + strlen("disable") + 1;
        EmbPanel *panel = window->panels[window->tab_index];
        if (starts_with(arguments, "grid")) {
            debug_message("Hide grid in this tab.");
            panel->grid = 0;
            return 0;
        }
        if (starts_with(arguments, "real")) {
            debug_message("Disable real render in this tab.");
            panel->real = 0;
            return 0;
        }
        if (starts_with(arguments, "ruler")) {
            debug_message("Hide rulers in this tab.");
            panel->ruler = 0;
            return 0;
        }
        if (starts_with(arguments, "ortho")) {
            debug_message("Hide orthogonal grid in this tab.");
            panel->ortho = 0;
            return 0;
        }
        if (starts_with(arguments, "qsnap")) {
            debug_message("Disable qsnap mode in this tab.");
            panel->snap = 0;
            return 0;
        }
        if (starts_with(arguments, "polar")) {
            debug_message("Disable orthogonal grid in this tab.");
            panel->polar = 0;
            return 0;
        }
        if (starts_with(arguments, "track")) {
            debug_message("Disable QTrack in this tab.");
            panel->qtrack = 0;
            return 0;
        }
        if (starts_with(arguments, "lwt")) {
            debug_message("Hide lineweight in this tab.");
            panel->lwt = 0;
            return 0;
        }
        return 0;
    }

    if (starts_with(action, "toggle")) {
        char *arguments = action + strlen("toggle") + 1;
        EmbPanel *panel = window->panels[window->tab_index];
        if (starts_with(arguments, "grid")) {
            debug_message("Toggle show grid in this tab.");
            panel->grid = !panel->grid;
            return 0;
        }
        if (starts_with(arguments, "real")) {
            debug_message("Toggle real render.");
            panel->real = !panel->real;
            return 0;
        }
        if (starts_with(arguments, "ruler")) {
            debug_message("toggle show ruler");
            panel->ruler = !panel->ruler;
            return 0;
        }
        if (starts_with(arguments, "ortho")) {
            debug_message("StatusBarButton toggleOrtho()");
            panel->ortho = !panel->ortho;
            return 0;
        }
        if (starts_with(arguments, "qsnap")) {
            debug_message("StatusBarButton toggleOrtho()");
            panel->snap = !panel->snap;
            return 0;
        }
        if (starts_with(arguments, "polar")) {
            debug_message("StatusBarButton toggleOrtho()");
            panel->polar = !panel->polar;
            return 0;
        }
        if (starts_with(arguments, "track")) {
            debug_message("StatusBarButton toggleQTrack()");
            panel->qtrack = !panel->qtrack;
            return 0;
        }
        if (starts_with(arguments, "lwt")) {
            debug_message("StatusBarButton toggleLwt()");
            panel->lwt = !panel->lwt;
            return 0;
        }
        return 0;
    }

    if (starts_with(action, "text")) {
        char *arguments = action + strlen("text") + 1;
        if (starts_with(arguments, "bold")) {
            debug_message("text_bold()");
            window->text_style.bold = !window->text_style.bold;
            return 0;
        }
        if (starts_with(arguments, "italic")) {
            debug_message("text_italic()");
            window->text_style.italic = !window->text_style.italic;
            return 0;
        }
        if (starts_with(arguments, "underline")) {
            debug_message("text_underline()");
            window->text_style.underline = !window->text_style.underline;
            return 0;
        }
        if (starts_with(arguments, "strikeout")) {
            debug_message("text_strikeout()");
            window->text_style.strikeout = !window->text_style.strikeout;
            return 0;
        }
        if (starts_with(arguments, "overline")) {
            debug_message("text_overline()");
            window->text_style.overline = !window->text_style.overline;
            return 0;
        }
        return 0;
    }

    if (starts_with(action, "zoom")) {
        EmbPanel *panel = window->panels[window->active_panel];
        char *arguments = action + strlen("zoom") + 1;
        if (starts_with(arguments, "real-time")) {
            debug_message("action: zoom-real-time");
            debug_message("Implement zoomRealtime.");
            return 0;
        }
        if (starts_with(arguments, "previous")) {
            debug_message("action: zoom-previous");
            debug_message("Implement zoomPrevious.");
            return 0;
        }
        if (starts_with(arguments, "window")) {
            debug_message("zoom_window()");
            /*
            gview = active_view(window);
            if (gview) {
                gview->zoom_window(window);
            } */

            window->zoom_window_active = 1;
            window->selecting_active = 0;
            window->n_selected = 0;
            return 0;
        }
        if (starts_with(arguments, "dynamic")) {
            debug_message("zoom_dynamic()");
            debug_message("Implement zoom_dynamic.");
            return 0;
        }
        if (starts_with(arguments, "scale")) {
            debug_message("zoom_scale()");
            debug_message("Implement zoom_scale.");
            return 0;
        }
        if (starts_with(arguments, "center")) {
            debug_message("zoom_center()");
            debug_message("Implement zoom_center.");
            return 0;
        }
        if (starts_with(arguments, "in")) {
            debug_message("zoom_in()");
            debug_message("View zoom_in()");
            if (!allow_zoom_in(panel)) {
                return 0;
            }

            set_override_cursor("Wait Cursor");
            /*cntr = map_to_scene(Vector(width()/2, height()/2));
            s = display_zoom_scale_in;
            scale(s, s);

            center_on(cntr);
            */
            restore_override_cursor(window);
            return 0;
        }
        if (starts_with(arguments, "out")) {
            debug_message("zoom_out()");
            debug_message("View zoom_out()");
            if (!allow_zoom_out(panel)) {
                return 0;
            }

            set_override_cursor("Wait Cursor");
            /*
            cntr = map_to_scene(Vector(width()/2, height()/2));
            s = display_zoom_scale_out;
            scale(s, s);

            center_on(cntr);
            */
            restore_override_cursor(window);
            return 0;
        }
        if (starts_with(arguments, "selected")) {
            debug_message("zoom_selected()");
            set_override_cursor("Wait Cursor");
            /*
            item_list = gscene.selected_items(window);
            selected_rect_path = Path(window);
            for (item in item_list) {
                selected_rect_path.add_polygon(item.map_to_scene(item.bounding_rect()));
            }

            selected_rect = selected_rectPath.bounding_rect()
            if (selected_rect) {
                message = translate("Preselect objects before invoking the zoom_selected command.")
                information(translate("zoom_selected Preselect"), message) */
                /* TODO: Support Post selection of objects */
                /*
            }

            fit_in_view(selected_rect, "KeepAspectRatio");
            */
            restore_override_cursor(window);
            return 0;
        }
        if (starts_with(arguments, "all")) {
            debug_message("zoom_all()");
            debug_message("Implement zoom_all.");
            return 0;
        }
        if (starts_with(arguments, "extents")) {
            debug_message("zoom_extents()");
            /*
            set_override_cursor("WaitCursor")
            extents = gscene.items_bounding_rect()
            if extents:
                extents.set_width(grid_size_x)
                extents.set_height(grid_size_y)
                extents.move_center(Vector(0, 0))

            fit_in_view(extents, "KeepAspectRatio")
            restore_override_cursor(window);
            */
            return 0;
        }
    }

    if (starts_with(action, "pan")) {
        char *arguments = action + strlen("pan") + 1;
        EmbPanel *panel = window->panels[window->active_panel];
        if (starts_with(arguments, "real-time")) {
            debug_message("pan-real-time-action()");
            /* mainwnd->panning_real_time_active = 1; */
            return 0;
        }
        if (starts_with(arguments, "point")) {
            debug_message("pan-point-action()");
            /* mainwnd->panning_point_active = 1; */
            return 0;
        }
        if (starts_with(arguments, "left")) {
            debug_message("pan-left-action()");
            /*
            horizontal-scroll-bar().set_value(horizontal-scroll-bar().value() + pan-distance);
            update_mouse_coords(view-mouse-point.x(), view-mouse-point.y());
            */
            scene_update(panel);
            return 0;
        }
        if (starts_with(arguments, "right")) {
            debug_message("pan-right-action()");
            /*
            horizontal-scroll-bar().set_value(horizontal-scroll-bar().value() - pan-distance);
            update_mouse_coords(view-mouse-point.x(), view-mouse-point.y());
            */
            scene_update(panel);
            return 0;
        }
        if (starts_with(arguments, "up")) {
            debug_message("pan-up-action()");
            /*
            vertical-scroll-bar().set_value(vertical-scroll-bar().value() + pan-distance);
            update_mouse_coords(view-mouse-point.x(), view-mouse-point.y());
            */
            scene_update(panel);
            return 0;
        }
        if (starts_with(arguments, "down")) {
            debug_message("pan-down-action()");
            /*
            vertical-scroll-bar().set_value(vertical-scroll-bar().value() - pan-distance);
            update_mouse_coords(view-mouse-point.x(), view-mouse-point.y());
            */
            scene_update(panel);
            return 0;
        }
    }

    if (starts_with(action, "day-vision")) {
        debug_message("action: day-vision");
        /*
        gview = self->active_view(window);
        if (gview) {
            gview->setBackgroundColor("#FFFFFF");
            gview->setCrossHairColor("#000000");
            gview->setGridColor("#000000");
        }
        */
        return 0;
    }
    if (starts_with(action, "night-vision")) {
        debug_message("action: night-vision");
        /*
        gview = self->active_view(window);
        if gview:
            gview->setBackgroundColor("#000000");
            gview->setCrossHairColor("#FFFFFF");
            gview->setGridColor("#FFFFFF");
        */
        return 0;
    }
    if (starts_with(action, "distance")) {
        distance(window);
        return 0;
    }
    if (starts_with(action, "delete-object")) {
        delete_object(window);
        return 0;
    }
    if (starts_with(action, "locate_point")) {
        locate_point(window);
        return 0;
    }
    if (starts_with(action, "move")) {
        move(window);
        return 0;
    }
    if (starts_with(action, "export")) {
        export_(window);
        return 0;
    }
    if (starts_with(action, "create-ui-rect")) {
        puts("create-ui-rect as an action hasn't been implimented.");
        return 0;
    }
    if (starts_with(action, "create-label")) {
        puts("create-label as an action hasn't been implimented.");
        return 0;
    }
    if (starts_with(action, "create-widget")) {
        puts("create-widget as an action hasn't been implimented.");
        return 0;
    }
    if (starts_with(action, "create-icon")) {
        puts("create-icon as an action hasn't been implimented.");
        return 0;
    }
    if (starts_with(action, "set-visibility")) {
        puts("set-visibility as an action hasn't been implimented.");
        return 0;
    }
    puts("Failed to identify action.");

    return 0;
}
