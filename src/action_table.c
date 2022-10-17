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
 *  The action table.
 */

#include "em2.h"

Action action_list[MAX_ACTIONS] = {
    {
        "do-nothing",
        do_nothing,
        PERMISSIONS_USER,
        "Do &Nothing",
        "An action that intensionally does nothing."
    },
    /*
    {
        "debug-message",
        debug_message,
        PERMISSIONS_USER,
        "Debug &Message",
        "Prints to the console (that the program is launched from) any debugging information."
    },
    */
    {
        "new-file",
        new_file,
        PERMISSIONS_USER,
        "&New",
        "Create a new file in a new tab."
    },
    {
        "open-file",
        open_file,
        PERMISSIONS_USER,
        "&Open",
        "Open an existing file."
    },
    {
        "save-file",
        save_file,
        PERMISSIONS_USER,
        "&Save",
        "Save the design to disk."
    },
    {
        "save-file-as",
        save_file_as,
        PERMISSIONS_USER,
        "&Save File As...",
        "Save the design under a new name and/or format."
    },
    {
        "check-for-updates",
        check_for_updates,
        PERMISSIONS_USER,
        "Check for &Updates",
        "NULL"
    },
    {
        "select-all",
        select_all,
        PERMISSIONS_USER,
        "&Select All",
        "Selects all objects: SELECTALL"
    },
    {
        "whats-this",
        whats_this,
        PERMISSIONS_USER,
        "&What's This?",
        "What's This? Context Help!"
    },
    {
        "design-details",
        design_details,
        PERMISSIONS_USER,
        "&Design Details",
        "Show the design details dialog for the current design."
    },
    {
        "print-pattern",
        print_pattern,
        PERMISSIONS_USER,
        "&Print",
        "Opens the print dialog to print pattern information and images."
    },
    {
        "exit-program",
        exit_program,
        PERMISSIONS_USER,
        "E&xit",
        "Exit the application."
    },
    {
        "cut-object",
        cut,
        PERMISSIONS_USER,
        "Cu&t",
        "Cut the current selection's contents to the clipboard."
    },
    {
        "copy-object",
        copy,
        PERMISSIONS_USER,
        "&Copy",
        "Copy the current selection's contents to the clipboard."
    },
    {
        "paste-object",
        paste,
        PERMISSIONS_USER,
        "&Paste",
        "Paste the clipboard contents into the selection within the scene."
    },
    {
        "undo",
        undo,
        PERMISSIONS_USER,
        "&Undo",
        "Reverses the most recent action."
    },
    {
        "redo",
        redo,
        PERMISSIONS_USER,
        "&Redo",
        "Reverses the effects of the previous undo action."
    },
    {
        "window-close",
        window_close,
        PERMISSIONS_USER,
        "Cl&ose",
        "Close the active window."
    },
    {
        "window-close-all",
        window_close_all,
        PERMISSIONS_USER,
        "Close &All",
        "Close all the windows."
    },
    {
        "window-cascade",
        window_cascade,
        PERMISSIONS_USER,
        "&Cascade",
        "Cascade the windows."
    },
    {
        "window-tile",
        window_tile,
        PERMISSIONS_USER,
        "&Tile",
        "Tile the windows."
    },
    {
        "window-next",
        window_next,
        PERMISSIONS_USER,
        "Ne&xt",
        "Move the focus to the next window."
    },
    {
        "window-previous",
        window_previous,
        PERMISSIONS_USER,
        "Pre&vious",
        "Move the focus to the previous window."
    },
    {
        "help",
        help,
        PERMISSIONS_USER,
        "&Help",
        "Displays help."
    },
    {
        "changelog-dialog",
        changelog,
        PERMISSIONS_USER,
        "&Changelog",
        "Opens a log of recent, new features in this product."
    },
    {
        "tip-of-the-day-dialog",
        tip_of_the_day,
        PERMISSIONS_USER,
        "&Tip Of The Day",
        "Displays a dialog with useful tips."
    },
    {
        "about-dialog",
        about,
        PERMISSIONS_USER,
        "&About Embroidermodder 2",
        "Displays information about this product."
    },
    {
        "icon-16",
        icon_16,
        PERMISSIONS_USER,
        "Icon&16",
        "Sets the toolbar icon size to 16x16."
    },
    {
        "icon-24",
        icon_24,
        PERMISSIONS_USER,
        "Icon&24",
        "Sets the toolbar icon size to 24x24."
    },
    {
        "icon-32",
        icon_32,
        PERMISSIONS_USER,
        "Icon&36",
        "Sets the toolbar icon size to 36x36."
    },
    {
        "icon-48",
        icon_48,
        PERMISSIONS_USER,
        "Icon&48",
        "Sets the toolbar icon size to 48x48."
    },
    {
        "icon-64",
        icon_64,
        PERMISSIONS_USER,
        "Icon&64",
        "Sets the toolbar icon size to 64x64."
    },
    {
        "icon-128",
        icon_128,
        PERMISSIONS_USER,
        "Icon 12&8",
        "Sets the toolbar icon size to 128x128."
    },
    {
        "settings-dialog",
        settings_dialog,
        PERMISSIONS_USER,
        "NULL",
        "NULL"
    },
    {
        "make-layer-current",
        make_layer_current,
        PERMISSIONS_USER,
        "&Make Layer Active",
        "Makes the layer of a selected object the active layer"
    },
    {
        "layers",
        layers,
        PERMISSIONS_USER,
        "&Layers",
        "Manages layers and layer properties: LAYER"
    },
    {
        "layer-selector",
        layer_selector,
        PERMISSIONS_USER,
        "&Layer Selector",
        "Dropdown selector for changing the current layer"
    },
    {
        "layer-previous",
        layer_previous,
        PERMISSIONS_USER,
        "&Layer Previous",
        "Restores the previous layer settings: LAYERP"
    },
    {
        "color-selector",
        color_selector,
        PERMISSIONS_USER,
        "&Color Selector",
        "Dropdown selector for changing the current thread color"
    },
    {
        "line-type-selector",
        line_type_selector,
        PERMISSIONS_USER,
        "&Stitchtype Selector",
        "Dropdown selector for changing the current stitch type"
    },
    {
        "line-weight-selector",
        line_weight_selector,
        PERMISSIONS_USER,
        "&Threadweight Selector",
        "Dropdown selector for changing the current thread weight"
    },
    {
        "hide-all-layers",
        hide_all_layers,
        PERMISSIONS_USER,
        "&Hide All Layers",
        "Turns the visibility off for all layers in the current drawing: HIDEALL"
    },
    {
        "show-all-layers",
        show_all_layers,
        PERMISSIONS_USER,
        "&Show All Layers",
        "Turns the visibility on for all layers in the current drawing: show-all-layers"
    },
    {
        "freeze-all-layers",
        freeze_all_layers,
        PERMISSIONS_USER,
        "&Freeze All Layers",
        "Freezes all layers in the current drawing:  FREEZEALL"
    },
    {
        "thaw-all-layers",
        thaw_all_layers,
        PERMISSIONS_USER,
        "&Thaw All Layers",
        "Thaws all layers in the current drawing: thaw_all_layers"
    },
    {
        "lock-all-layers",
        lock_all_layers,
        PERMISSIONS_USER,
        "&Lock All Layers",
        "Locks all layers in the current drawing: lock-all-layers"
    },
    {
        "unlock-all-layers",
        unlock_all_layers,
        PERMISSIONS_USER,
        "&Unlock All Layers",
        "Unlocks all layers in the current drawing: unlock-all-layers"
    },
    {
        "text-bold",
        text_bold,
        PERMISSIONS_USER,
        "&Bold Text",
        "Sets text to be bold."
    },
    {
        "text-italic",
        text_italic,
        PERMISSIONS_USER,
        "&Italic Text",
        "Sets text to be italic."
    },
    {
        "text-underline",
        text_underline,
        PERMISSIONS_USER,
        "&Underline Text",
        "Sets text to be underlined."
    },
    {
        "text-strikeout",
        text_strikeout,
        PERMISSIONS_USER,
        "&StrikeOut Text",
        "Sets text to be striked out."
    },
    {
        "text-overline",
        text_overline,
        PERMISSIONS_USER,
        "&Overline Text",
        "Sets text to be overlined."
    },
    {
        "zoom-real-time",
        zoom_real_time,
        PERMISSIONS_USER,
        "Zoom &Realtime",
        "Zooms to increase or decrease the apparent size of objects in the current viewport."
    },
    {
        "zoom-previous",
        zoom_previous,
        PERMISSIONS_USER,
        "Zoom &Previous",
        "Zooms to display the previous view."
    },
    {
        "zoom-window",
        zoom_window,
        PERMISSIONS_USER,
        "Zoom &Window",
        "Zooms to display an area specified by a rectangular window."
    },
    {
        "zoom-dynamic",
        zoom_dynamic,
        PERMISSIONS_USER,
        "Zoom &Dynamic",
        "Zooms to display the generated portion of the drawing."
    },
    {
        "zoom-scale",
        zoom_scale,
        PERMISSIONS_USER,
        "Zoom Sca&le",
        "Zooms the display using a specified scale factor."
    },
    {
        "zoom-center",
        zoom_center,
        PERMISSIONS_USER,
        "Zoom &Center",
        "Zooms to display a view specified by a center point and  magnification or height."
    },
    {
        "zoom-in",
        zoom_in,
        PERMISSIONS_USER,
        "Zoom &In",
        "Zooms to increase the apparent size of objects."
    },
    {
        "zoom-out",
        zoom_out,
        PERMISSIONS_USER,
        "Zoom &Out",
        "Zoom to decrease the apparent size of object."
    },
    {
        "zoom-selected",
        zoom_selected,
        PERMISSIONS_USER,
        "Zoom Selec&ted",
        "Zooms to display the selected objects."
    },
    {
        "zoom-all",
        zoom_all,
        PERMISSIONS_USER,
        "Zoom &All",
        "Zooms to display the drawing extents or the grid limits."
    },
    {
        "zoom-extents",
        zoom_extents,
        PERMISSIONS_USER,
        "Zoom &Extents",
        "Zooms to display the drawing extents."
    },
    {
        "pan-real-time",
        pan_real_time,
        PERMISSIONS_USER,
        "&Pan Realtime",
        "Moves the view in the current viewport."
    },
    {
        "pan-point",
        pan_point,
        PERMISSIONS_USER,
        "Pan &Point",
        "Moves the view by the specified distance."
    },
    {
        "pan-left",
        pan_left,
        PERMISSIONS_USER,
        "&Pan Left",
        "Moves the view to the left."
    },
    {
        "pan-right",
        pan_right,
        PERMISSIONS_USER,
        "&Pan Right",
        "Moves the view to the right."
    },
    {
        "pan-up",
        pan_up,
        PERMISSIONS_USER,
        "&Pan Up",
        "Moves the view up."
    },
    {
        "pan-down",
        pan_down,
        PERMISSIONS_USER,
        "&Pan Down",
        "Moves the view down."
    },
    {
        "day-vision",
        day_vision,
        PERMISSIONS_USER,
        "&Day",
        "Updates the current view using day vision settings."
    },
    {
        "night-vision",
        night_vision,
        PERMISSIONS_USER,
        "&Night",
         "Updates the current view using night vision settings."
    },
    {
        "treble-clef",
        treble_clef,
        PERMISSIONS_USER,
        "&TrebleClef",
        "Creates a treble clef: TREBLECLEF"
    },
    {
        "path",
        path,
        PERMISSIONS_USER,
        "&Path",
		"Add a path object to the current view."
    },
    {
        "circle",
        circle,
        PERMISSIONS_USER,
        "&Circle",
        "Adds a circle to the vector layer."
    },
    {
        "line",
        line,
        PERMISSIONS_USER,
        "&Line",
        "Creates straight line segments: LINE"
    },
    {
        "distance",
        distance,
        PERMISSIONS_USER,
        "&Distance",
        "Measures the distance and angle between two points: DIST"
    },
    {
        "dolphin",
        dolphin,
        PERMISSIONS_USER,
        "&Dolphin",
		"Create a dolphin design in the vector layer."
    },
    {
        "ellipse",
        ellipse,
        PERMISSIONS_USER,
        "&Ellipse",
        "Add an ellipse to the vector layer."
    },
    {
        "delete-object",
        delete_object,
        PERMISSIONS_USER,
        "D&elete",
        "Removes objects from a drawing: delete"
    },
    {
        "heart",
        heart,
        PERMISSIONS_USER,
        "&Heart",
        "Creates a heart: HEART"
    },
    {
        "locate-point",
        locate_point,
        PERMISSIONS_USER,
        "&Locate Point",
        "Find a point that is near the indicated region."
    },
    {
        "move",
        move,
        PERMISSIONS_USER,
        "&Move",
        "Displaces objects a specified distance in a specified direction: MOVE"
    },
    {
        "export",
        export_,
        PERMISSIONS_USER,
        "&Export",
        "Export the current tab as a non-stitch format that will lose more data."
    },
    {
        "create-ui-rect",
        NULL,
        PERMISSIONS_SYSTEM,
        "NULL",
        "NULL"
    },
    {
        "create-label",
        NULL,
        PERMISSIONS_SYSTEM,
        "NULL",
        "NULL"
    },
    {
        "create-widget",
        NULL,
        PERMISSIONS_SYSTEM,
        "NULL",
        "NULL"
    },
    {
        "create-icon",
        NULL,
        PERMISSIONS_SYSTEM,
        "NULL",
        "NULL"
    },
    {
        "set-visibility",
        NULL,
        PERMISSIONS_SYSTEM,
        "NULL",
        "NULL"
    },
    {
        "file-menu",
        display_file_menu,
        PERMISSIONS_SYSTEM,
        "NULL",
        "NULL"
    },
    {
        "edit-menu",
        display_edit_menu,
        PERMISSIONS_SYSTEM,
        "NULL",
        "NULL"
    },
    {
        "view-menu",
        display_view_menu,
        PERMISSIONS_SYSTEM,
        "NULL",
        "NULL"
    },
    {
        "settings-menu",
        display_settings_menu,
        PERMISSIONS_SYSTEM,
        "NULL",
        "NULL"
    },
    {
        "END",
        NULL,
        0,
        "NULL",
        "NULL"
    }
};

    /*
    {
		"assets/objects/dolphin/dolphin.scm",
		num_points=512
		scale_x=0.04
		scale_y=0.04
        "Creates a dolphin: dolphin"
    },
    {
        "heart4",
        heart4,
        PERMISSIONS_USER,
		"&Heart4",
		"Adds a heart to the vector layer: heart4."
		"assets/scripts/heart4.scm"
    },
    {
		"heart5",
        heart5,
        PERMISSIONS_USER,
		"&Heart5",
		"Adds a heart to the vector layer."
		"assets/scripts/heart5.scm"
    },
    {
        "single-line-text",
        PERMISSIONS_USER,
        single_line_text,
        "&Single Line Text",
        "Creates single-line text objects: TEXT"
    },
    {
        "spell-check",
        PERMISSIONS_USER | OBJ_TYPE_TEXT,
        spell_check,
        "S&pell Check",
        "Spell check the text object selected."
    },
    {
        "quick-select",
        PERMISSIONS_USER,
        quick_select,
    },
    {
        "rectangle",
        PERMISSIONS_USER,
        rectangle,
        "&Rectangle",
        "Creates a rectangular polyline: RECTANGLE"
    },
    {
        "rgb",
        PERMISSIONS_USER,
        rgb,
        "&RGB",
        "Updates the current view colors: RGB"
    },
    {
        "rotate",
        PERMISSIONS_USER,
        rotate,
        "&Rotate",
        "Rotates objects about a base point: ROTATE"
    },
    {
        "sandbox",
        PERMISSIONS_USER,
        sandbox,
        "&Sandbox",
        "A sandbox to play in: SANDBOX"
    },
    {
        "quickleader",
        quickleader,
        PERMISSIONS_USER,
        "&QuickLeader",
        "Creates a leader and annotation: QUICKLEADER"
    },
    {
        "snowflake",
        snowflake,
        PERMISSIONS_USER,
        "&Snowflake",
        "Creates a snowflake: SNOWFLAKE"
    },
    {
        "star",
        star,
        PERMISSIONS_USER,
        "&Star",
        "Creates a star: STAR"
    },
    {
        "platform",
        platform,
        PERMISSIONS_USER,
        "&Platform",
        "List which platform is in use: PLATFORM"
    },
    {
        "point",
        point,
        PERMISSIONS_USER,
        "&Point",
        "Creates multiple points: POINT"
    },
    {
        "polygon",
        polygon,
        PERMISSIONS_USER,
        "Pol&ygon",
        "Creates a regular polygon: POLYGON"
    },
    {
        "polyline",
        polyline,
        PERMISSIONS_USER,
        "&Polyline",
        "Creates a 2D polyline: PLINE"
    },
    {
        "settings-dialog",
        settings_dialog,
        PERMISSIONS_USER,
        "&Settings",
        "Configure settings specific to this product."
    },
    {
        "quickleader",
        quickleader,
        PERMISSIONS_USER,
        "&QuickLeader",
        "Creates a leader and annotation: QUICKLEADER"
    },
    {
        "locate-point",
        locate_point,
        PERMISSIONS_USER,
        "&Locate Point",
        "Displays the coordinate values of a location: ID"
    },
    {
        "point",
        point,
        PERMISSIONS_USER,
        "&Point",
        "Creates multiple points: POINT"
    }
    */
