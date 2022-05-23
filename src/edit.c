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

pointer
scm_export(scheme *sc, pointer args)
{
    debug_message("Export.");
    pointer arg[2];
    if (args == sc->NIL) {
        return sc->NIL;
    }
    if (list_length(sc, args) < 1) {
        return sc->NIL;
    }
    get_args(args, arg, 1);
    printf("%s\n", string_value(pair_car(arg[0])));
    return sc->NIL;
}

pointer
scm_do_nothing(scheme *sc, pointer args)
{
    debug_message("Do nothing.");
    if (args == sc->NIL) {
        return sc->NIL;
    }
    if (list_length(sc, args) > 0) {
        puts("do-nothing ignores arguments passed to it.");
        return sc->NIL;
    }
    return sc->NIL;
}

pointer
scm_debug_message(scheme *sc, pointer args)
{
    printf("Debug message\n");
    pointer arg[2];
    if (args == sc->NIL) {
        return sc->NIL;
    }
    if (list_length(sc, args) < 1) {
        return sc->NIL;
    }
    get_args(args, arg, 1);
    printf("%s\n", string_value(pair_car(arg[0])));
    
    return sc->NIL;
}

/* Creates a new file using the standard dialog_
 * Adds an mdi_area.
 */
pointer
scm_new_file(scheme *sc, pointer args)
{
    debug_message("New File.");
    int n_patterns = load_int(sc, "n-patterns");
    mainwnd->pattern[n_patterns] = embPattern_create();
    set_int(sc, "n-patterns", n_patterns+1);
    set_int(sc, "tab-index", load_int(sc, "tab-index")+1);
    set_int(sc, "n-docs", load_int(sc, "n-docs")+1);
    if (list_length(sc, args) > 0) {
        puts("The argument for new-file is unnecessary and ignored.");
    }
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
    return sc->NIL;
}

/* Opens a file using the standard dialog.
 */
pointer
scm_open_file(scheme *sc, pointer args)
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
    return sc->NIL;
}


/*
 * Instead of closing using exit() this allows us to chain any checks,
 * like the confirmation of the close, to the action.
 */
pointer
scm_exit_program(scheme *sc, pointer args)
{
    debug_message("Closing Embroidermodder 2.0.");
    if (list_length(sc, args) > 0) {
        puts("The argument for exit-program is unnecessary and ignored.");
    }
    mainwnd->running = 0;
    return sc->NIL;
}

pointer
scm_icon_16(scheme *sc, pointer args)
{
    debug_message("icon_16()");
    pointer arg[2];
    if (args == sc->NIL) {
        return sc->NIL;
    }
    if (list_length(sc, args) < 1) {
        return sc->NIL;
    }
    get_args(args, arg, 1);
    printf("%s\n", string_value(pair_car(arg[0])));
    return sc->NIL;
}

pointer
scm_icon_24(scheme *sc, pointer args)
{
    debug_message("icon_24()");
    pointer arg[2];
    if (args == sc->NIL) {
        return sc->NIL;
    }
    if (list_length(sc, args) < 1) {
        return sc->NIL;
    }
    get_args(args, arg, 1);
    printf("%s\n", string_value(pair_car(arg[0])));
    return sc->NIL;
}

pointer
scm_icon_32(scheme *sc, pointer args)
{
    debug_message("icon_32()");
    pointer arg[2];
    if (args == sc->NIL) {
        return sc->NIL;
    }
    if (list_length(sc, args) < 1) {
        return sc->NIL;
    }
    get_args(args, arg, 1);
    printf("%s\n", string_value(pair_car(arg[0])));
    return sc->NIL;
}

pointer
scm_icon_48(scheme *sc, pointer args)
{
    debug_message("icon_48()");
    pointer arg[2];
    if (args == sc->NIL) {
        return sc->NIL;
    }
    if (list_length(sc, args) < 1) {
        return sc->NIL;
    }
    get_args(args, arg, 1);
    printf("%s\n", string_value(pair_car(arg[0])));
    return sc->NIL;
}

pointer
scm_icon_64(scheme *sc, pointer args)
{
    debug_message("icon_64()");
    pointer arg[2];
    if (args == sc->NIL) {
        return sc->NIL;
    }
    if (list_length(sc, args) < 1) {
        return sc->NIL;
    }
    get_args(args, arg, 1);
    printf("%s\n", string_value(pair_car(arg[0])));
    return sc->NIL;
}

pointer
scm_icon_128(scheme *sc, pointer args)
{
    debug_message("icon_128()");
    pointer arg[2];
    if (args == sc->NIL) {
        return sc->NIL;
    }
    if (list_length(sc, args) < 1) {
        return sc->NIL;
    }
    get_args(args, arg, 1);
    printf("%s\n", string_value(pair_car(arg[0])));
    return sc->NIL;
}

/* .
 */
pointer
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
            int n_docs = load_int(mainwnd->sc, "n-docs");
            
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
pointer
scm_open_recent_file(scheme *sc, pointer args)
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
pointer
scm_save_file_as(scheme *sc, pointer args)
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
    return sc->NIL;
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
pointer
scm_save_file(scheme *sc, pointer args)
{
    EmbPattern *pattern;
    int write_successful, format_type;
    debug_message("Save file()");
    /* debug_message(current_file_name); */

    write_successful = 0;
    format_type = emb_identify_format(load_str(sc, "current-file-name"));
    if (format_type == EMBFORMAT_UNSUPPORTED) {
        debug_message("The format of the file is not supported.");
        return sc->NIL;
    }

    pattern = embPattern_create();
    if (!pattern) {
        debug_message("Could not allocate memory for embroidery pattern");
        return sc->NIL;
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
    return sc->NIL;
}

/* . */
pointer
scm_whats_this(scheme *sc, pointer args)
{
    debug_message("whats_this()");
    /*
    what_this = 1;
    QWhatsThis_enterWhatsThisMode();
    */
    return sc->NIL;
}

/* Call print dialog to print out a render of the embroidery.
 */
pointer
scm_print_pattern(scheme *sc, pointer args)
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
pointer
scm_cut(scheme *sc, pointer args)
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
    return sc->NIL;
}

/*
 * Copy acts on objects selected before the action is called.
 *
 * If no objects are selected an error is returned.
 */
pointer
scm_copy(scheme *sc, pointer args)
{
    debug_message("copy()");
    if (list_length(sc, args) > 0) {
        puts("The argument for copy is unnecessary and ignored.");
    }

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
    return sc->NIL;
}

/* .
 */
pointer
scm_copy_selected(scheme *sc, pointer args)
{
    if (list_length(sc, args) > 0) {
        puts("The argument for copy-selected is unnecessary and ignored.");
    }
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
    return sc->NIL;
}

/* Paste whatever objects are on the clipboard.
 */
pointer
scm_paste(scheme *sc, pointer args)
{
    debug_message("paste()");

    if (list_length(sc, args) > 0) {
        puts("The argument for paste is unnecessary and ignored.");
    }

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
    return sc->NIL;
}

/*
 * Reapply the action that is next on the undo_history array,
 * if we are at the top of the array then do nothing.
 */
pointer
scm_redo(scheme *sc, pointer args)
{
    debug_message("redo()");
    if (list_length(sc, args) > 0) {
        puts("The argument for redo is unnecessary and ignored.");
    }

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
    return sc->NIL;
}

/*
 * Apply the reverse of the action on the current position
 * of the undo_history array.
 */
pointer
scm_undo(scheme *sc, pointer args)
{
    debug_message("undo()");
    if (list_length(sc, args) > 0) {
        puts("The argument for redo is unnecessary and ignored.");
    }
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
    return sc->NIL;
}

/* Uses the undo stack.
 */
pointer
scm_repeat(scheme *sc, pointer args)
{
    debug_message(" . ");
    if (list_length(sc, args) > 0) {
        puts("The argument for redo is unnecessary and ignored.");
    }
    return sc->NIL;
}

/* Apply 1 to all layer's visibility flags.
 */
pointer
scm_show_all_layers(scheme *sc, pointer args)
{
    debug_message("showAllLayers()");
    if (list_length(sc, args) > 0) {
        puts("The argument for show-all-layers is unnecessary and ignored.");
    }
    return sc->NIL;
}
/* .
 */
pointer
scm_design_details(scheme *sc, pointer args)
{
    if (list_length(sc, args) > 0) {
        puts("The argument for design-details is unnecessary and ignored.");
    }

    return sc->NIL;
}


/* Make layer active.
 */
pointer
scm_make_layer_active(scheme *sc, pointer args)
{
    debug_message("make_layer_active()");
    debug_message("Implement makeLayerActive.");
    if (list_length(sc, args) > 0) {
        puts("The argument for make-layer-active is unnecessary and ignored.");
    }
    return sc->NIL;
}

/* .
 */
pointer
scm_help(scheme *sc, pointer args)
{
    debug_message("help()");
    if (list_length(sc, args) > 0) {
        puts("The argument for help is unnecessary and ignored.");
    }
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
    return sc->NIL;
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
pointer
scm_about(scheme *sc, pointer args)
{
    debug_message("about()");
    if (list_length(sc, args) > 0) {
        puts("The argument for about is unnecessary and ignored.");
    }
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
    return sc->NIL;
}

/* Make all layers not editable.
 */
pointer
scm_freeze_all_layers(scheme *sc, pointer args)
{
    debug_message("freezeAllLayers()");
    return sc->NIL;
}

/* Make layers editable again.
 */
pointer
scm_thaw_all_layers(scheme *sc, pointer args)
{
    debug_message("thawAllLayers()");
    return sc->NIL;
}

/*
 * Make all layers not editable.
 * (What's the difference between freeze and lock?);
 */
pointer
lock_all_layers(scheme *sc, pointer args)
{
    debug_message("lockAllLayers()");
    return sc->NIL;
}

/* .
 */
pointer
unlock_all_layers(scheme *sc, pointer args)
{
    debug_message("unlock_all_layers()");
    if (list_length(sc, args) > 0) {
        puts("The argument for redo is unnecessary and ignored.");
    }
    return sc->NIL;
}

/* .
 */
pointer
hide_all_layers(scheme *sc, pointer args)
{
    debug_message("hide_all_Layers()");
    if (list_length(sc, args) > 0) {
        puts("The argument for redo is unnecessary and ignored.");
    }
    return sc->NIL;
}

/* .
 */
pointer
make_layer_current(scheme *sc, pointer args)
{
    debug_message("makeLayerCurrent()");
    if (list_length(sc, args) > 0) {
        puts("The argument for redo is unnecessary and ignored.");
    }
    return sc->NIL;
}

/* TODO: Make day vision color settings.
 */
pointer
day_vision(scheme *sc, pointer args)
{
    /*
    gview = self->active_view();
    if gview:
        gview->setBackgroundColor("#FFFFFF");
        gview->setCrossHairColor("#000000");
        gview->setGridColor("#000000");
        */
    debug_message("day_vision()");
    if (list_length(sc, args) > 0) {
        puts("The argument for redo is unnecessary and ignored.");
    }
    return sc->NIL;
}

/* TODO: Make night vision color settings.
 */
pointer
night_vision(scheme *sc, pointer args)
{
	/*
    gview = self->active_view();
    if gview:
        gview->setBackgroundColor("#000000");
        gview->setCrossHairColor("#FFFFFF");
        gview->setGridColor("#FFFFFF");
    */
    debug_message("night_vision()");
    if (list_length(sc, args) > 0) {
        puts("The argument for redo is unnecessary and ignored.");
    }
    return sc->NIL;
}

/*
Display the changelog stored in the config.json data file.

Not currently maintained.
*/
pointer
scm_changelog(scheme *sc, pointer args)
{
    debug_message("changelog()");
    if (list_length(sc, args) > 0) {
        puts("The argument for redo is unnecessary and ignored.");
    }
    /*
    #display in a custom widget instead
    #
    #QUrl changelogURL("help/changelog.html");
    #QDesktopServices_openUrl(changelogURL);
    */
    return sc->NIL;
}

/* .
 */
pointer
scm_settings_dialog(scheme *sc, pointer args)
{
    debug_message("settings_dialog()");
    if (list_length(sc, args) > 0) {
        puts("The argument for redo is unnecessary and ignored.");
    }
    return sc->NIL;
}

/* Run the "tip of the day" dialog box.
 */
pointer
scm_tip_of_the_day(scheme *sc, pointer args)
{
    debug_message("tip_of_the_day()");
    if (list_length(sc, args) > 0) {
        puts("The argument for redo is unnecessary and ignored.");
    }
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
    return sc->NIL;
}

/* .
 */
pointer
scm_check_for_updates(scheme *sc, pointer args)
{
    debug_message("scm_check_for_updates()");
    if (list_length(sc, args) > 0) {
        puts("The argument for redo is unnecessary and ignored.");
    }
    return sc->NIL;
}

/* .
 */
pointer
scm_select_all(scheme *sc, pointer args)
{
    debug_message("scm_select_all()");
    if (list_length(sc, args) > 0) {
        puts("The argument for redo is unnecessary and ignored.");
    }
    return sc->NIL;
}

/* .
 */
pointer
scm_color_selector(scheme *sc, pointer args)
{
    debug_message("scm_color_selector()");
    if (list_length(sc, args) > 0) {
        puts("The argument for redo is unnecessary and ignored.");
    }
    return sc->NIL;
}

/* .
 */
pointer
scm_line_type_selector(scheme *sc, pointer args)
{
    debug_message("scm_line_type_selector()");
    if (list_length(sc, args) > 0) {
        puts("The argument for redo is unnecessary and ignored.");
    }
    return sc->NIL;
}

/* .
 */
pointer
scm_line_weight_selector(scheme *sc, pointer args)
{
    debug_message("scm_line_weight_selector()");
    if (list_length(sc, args) > 0) {
        puts("The argument for redo is unnecessary and ignored.");
    }
    return sc->NIL;
}

/* .
 */
pointer
scm_day_vision(scheme *sc, pointer args)
{
    debug_message("scm_day_vision()");
    if (list_length(sc, args) > 0) {
        puts("The argument for redo is unnecessary and ignored.");
    }
    return sc->NIL;
}

/* .
 */
pointer
scm_night_vision(scheme *sc, pointer args)
{
    debug_message("scm_night_vision()");
    if (list_length(sc, args) > 0) {
        puts("The argument for redo is unnecessary and ignored.");
    }
    return sc->NIL;
}

/* .
 */
pointer
scm_treble_clef(scheme *sc, pointer args)
{
    debug_message("scm_treble_clef()");
    if (list_length(sc, args) > 0) {
        puts("The argument for redo is unnecessary and ignored.");
    }
    return sc->NIL;
}

/* .
 */
pointer
scm_path(scheme *sc, pointer args)
{
    debug_message("scm_path()");
    if (list_length(sc, args) > 0) {
        puts("The argument for redo is unnecessary and ignored.");
    }
    return sc->NIL;
}

/* .
 */
pointer
scm_circle(scheme *sc, pointer args)
{
    debug_message("scm_circle()");
    if (list_length(sc, args) > 0) {
        puts("The argument for redo is unnecessary and ignored.");
    }
    return sc->NIL;
}

/* .
 */
pointer
scm_line(scheme *sc, pointer args)
{
    debug_message("scm_line()");
    if (list_length(sc, args) > 0) {
        puts("The argument for redo is unnecessary and ignored.");
    }
    return sc->NIL;
}

/* .
 */
pointer
scm_distance(scheme *sc, pointer args)
{
    debug_message("scm_distance()");
    if (list_length(sc, args) > 0) {
        puts("The argument for redo is unnecessary and ignored.");
    }
    return sc->NIL;
}

/* .
 */
pointer
scm_dolphin(scheme *sc, pointer args)
{
    debug_message("scm_dolphin()");
    if (list_length(sc, args) > 0) {
        puts("The argument for redo is unnecessary and ignored.");
    }
    return sc->NIL;
}

/* .
 */
pointer
scm_ellipse(scheme *sc, pointer args)
{
    debug_message("scm_ellipse()");
    if (list_length(sc, args) > 0) {
        puts("The argument for redo is unnecessary and ignored.");
    }
    return sc->NIL;
}

/* .
 */
pointer
scm_delete_object(scheme *sc, pointer args)
{
    debug_message("scm_delete_object()");
    if (list_length(sc, args) > 0) {
        puts("The argument for redo is unnecessary and ignored.");
    }
    return sc->NIL;
}

/* .
 */
pointer
scm_heart(scheme *sc, pointer args)
{
    debug_message("scm_heart()");
    if (list_length(sc, args) > 0) {
        puts("The argument for redo is unnecessary and ignored.");
    }
    return sc->NIL;
}

/* .
 */
pointer
scm_locate_point(scheme *sc, pointer args)
{
    debug_message("scm_locate_point()");
    if (list_length(sc, args) > 0) {
        puts("The argument for redo is unnecessary and ignored.");
    }
    return sc->NIL;
}

/* .
 */
pointer
scm_move(scheme *sc, pointer args)
{
    debug_message("scm_locate_point()");
    if (list_length(sc, args) > 0) {
        puts("The argument for redo is unnecessary and ignored.");
    }
    return sc->NIL;
}

