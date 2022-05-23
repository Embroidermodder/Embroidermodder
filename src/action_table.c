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
        scm_do_nothing,
        PERMISSIONS_USER,
        "Do &Nothing",
        "An action that intensionally does nothing."
    },
    {
        "debug-message",
        scm_debug_message,
        PERMISSIONS_USER,
        "Debug &Message",
        "Prints to the console (that the program is launched from) any debugging information."
    },
    {
        "new-file",
        scm_new_file,
        PERMISSIONS_USER,
        "&New",
        "Create a new file in a new tab."
    },
    {
        "open-file",
        scm_open_file,
        PERMISSIONS_USER,
        "&Open",
        "Open an existing file."
    },
    {
        "save-file",
        scm_save_file,
        PERMISSIONS_USER,
        "&Save",
        "Save the design to disk."
    },
    {
        "save-file-as",
        scm_save_file_as,
        PERMISSIONS_USER,
        "&Save File As...",
        "Save the design under a new name and/or format."
    },
    {
        "check-for-updates",
        scm_check_for_updates,
        PERMISSIONS_USER,
        "Check for &Updates",
        "NULL"
    },
    {
        "select-all",
        scm_select_all,
        PERMISSIONS_USER,
        "&Select All",
        "Selects all objects: SELECTALL"
    },
    {
        "whats-this",
        scm_whats_this,
        PERMISSIONS_USER,
        "&What's This?",
        "What's This? Context Help!"
    },
    {
        "design-details",
        scm_design_details,
        PERMISSIONS_USER,
        "&Design Details",
        "Show the design details dialog for the current design."
    },
    {
        "print-pattern",
        scm_print_pattern,
        PERMISSIONS_USER,
        "&Print",
        "Opens the print dialog to print pattern information and images."
    },
    {
        "exit-program",
        scm_exit_program,
        PERMISSIONS_USER,
        "E&xit",
        "Exit the application."
    },
    {
        "cut-object",
        scm_cut,
        PERMISSIONS_USER,
        "Cu&t",
        "Cut the current selection's contents to the clipboard."
    },
    {
        "copy-object",
        scm_copy,
        PERMISSIONS_USER,
        "&Copy",
        "Copy the current selection's contents to the clipboard."
    },
    {
        "paste-object",
        scm_paste,
        PERMISSIONS_USER,
        "&Paste",
        "Paste the clipboard contents into the selection within the scene."
    },
    {
        "undo",
        scm_undo,
        PERMISSIONS_USER,
        "&Undo",
        "Reverses the most recent action."
    },
    {
        "redo",
        scm_redo,
        PERMISSIONS_USER,
        "&Redo",
        "Reverses the effects of the previous undo action."
    },
    {
        "window-close",
        scm_window_close,
        PERMISSIONS_USER,
        "Cl&ose",
        "Close the active window."
    },
    {
        "window-close-all",
        scm_window_close_all,
        PERMISSIONS_USER,
        "Close &All",
        "Close all the windows."
    },
    {
        "window-cascade",
        scm_window_cascade,
        PERMISSIONS_USER,
        "&Cascade",
        "Cascade the windows."
    },
    {
        "window-tile",
        scm_window_tile,
        PERMISSIONS_USER,
        "&Tile",
        "Tile the windows."
    },
    {
        "window-next",
        scm_window_next,
        PERMISSIONS_USER,
        "Ne&xt",
        "Move the focus to the next window."
    },
    {
        "window-previous",
        scm_window_previous,
        PERMISSIONS_USER,
        "Pre&vious",
        "Move the focus to the previous window."
    },
    {
        "help",
        scm_help,
        PERMISSIONS_USER,
        "&Help",
        "Displays help."
    },
    {
        "changelog-dialog",
        scm_changelog,
        PERMISSIONS_USER,
        "&Changelog",
        "Opens a log of recent, new features in this product."
    },
    {
        "tip-of-the-day-dialog",
        scm_tip_of_the_day,
        PERMISSIONS_USER,
        "&Tip Of The Day",
        "Displays a dialog with useful tips."
    },
    {
        "about-dialog",
        scm_about,
        PERMISSIONS_USER,
        "&About Embroidermodder 2",
        "Displays information about this product."
    },
    {
        "icon-16",
        scm_icon_16,
        PERMISSIONS_USER,
        "Icon&16",
        "Sets the toolbar icon size to 16x16."
    },
    {
        "icon-24",
        scm_icon_24,
        PERMISSIONS_USER,
        "Icon&24",
        "Sets the toolbar icon size to 24x24."
    },
    {
        "icon-32",
        scm_icon_32,
        PERMISSIONS_USER,
        "Icon&36",
        "Sets the toolbar icon size to 36x36."
    },
    {
        "icon-48",
        scm_icon_48,
        PERMISSIONS_USER,
        "Icon&48",
        "Sets the toolbar icon size to 48x48."
    },
    {
        "icon-64",
        scm_icon_64,
        PERMISSIONS_USER,
        "Icon&64",
        "Sets the toolbar icon size to 64x64."
    },
    {
        "icon-128",
        scm_icon_128,
        PERMISSIONS_USER,
        "Icon 12&8",
        "Sets the toolbar icon size to 128x128."
    },
    {
        "settings-dialog",
        scm_settings_dialog,
        PERMISSIONS_USER,
        "NULL",
        "NULL"
    },
    {
        "make-layer-current",
        scm_make_layer_current,
        PERMISSIONS_USER,
        "&Make Layer Active",
        "Makes the layer of a selected object the active layer"
    },
    {
        "layers",
        scm_layers,
        PERMISSIONS_USER,
        "&Layers",
        "Manages layers and layer properties: LAYER"
    },
    {
        "layer-selector",
        scm_layer_selector,
        PERMISSIONS_USER,
        "&Layer Selector",
        "Dropdown selector for changing the current layer"
    },
    {
        "layer-previous",
        scm_layer_previous,
        PERMISSIONS_USER,
        "&Layer Previous",
        "Restores the previous layer settings: LAYERP"
    },
    {
        "color-selector",
        scm_color_selector,
        PERMISSIONS_USER,
        "&Color Selector",
        "Dropdown selector for changing the current thread color"
    },
    {
        "line-type-selector",
        scm_line_type_selector,
        PERMISSIONS_USER,
        "&Stitchtype Selector",
        "Dropdown selector for changing the current stitch type"
    },
    {
        "line-weight-selector",
        scm_line_weight_selector,
        PERMISSIONS_USER,
        "&Threadweight Selector",
        "Dropdown selector for changing the current thread weight"
    },
    {
        "hide-all-layers",
        scm_hide_all_layers,
        PERMISSIONS_USER,
        "&Hide All Layers",
        "Turns the visibility off for all layers in the current drawing: HIDEALL"
    },
    {
        "show-all-layers",
        scm_show_all_layers,
        PERMISSIONS_USER,
        "&Show All Layers",
        "Turns the visibility on for all layers in the current drawing: show-all-layers"
    },
    {
        "freeze-all-layers",
        scm_freeze_all_layers,
        PERMISSIONS_USER,
        "&Freeze All Layers",
        "Freezes all layers in the current drawing:  FREEZEALL"
    },
    {
        "thaw-all-layers",
        scm_thaw_all_layers,
        PERMISSIONS_USER,
        "&Thaw All Layers",
        "Thaws all layers in the current drawing: thaw_all_layers"
    },
    {
        "lock-all-layers",
        scm_lock_all_layers,
        PERMISSIONS_USER,
        "&Lock All Layers",
        "Locks all layers in the current drawing: lock-all-layers"
    },
    {
        "unlock-all-layers",
        scm_unlock_all_layers,
        PERMISSIONS_USER,
        "&Unlock All Layers",
        "Unlocks all layers in the current drawing: unlock-all-layers"
    },
    {
        "text-bold",
        scm_text_bold,
        PERMISSIONS_USER,
        "&Bold Text",
        "Sets text to be bold."
    },
    {
        "text-italic",
        scm_text_italic,
        PERMISSIONS_USER,
        "&Italic Text",
        "Sets text to be italic."
    },
    {
        "text-underline",
        scm_text_underline,
        PERMISSIONS_USER,
        "&Underline Text",
        "Sets text to be underlined."
    },
    {
        "text-strikeout",
        scm_text_strikeout,
        PERMISSIONS_USER,
        "&StrikeOut Text",
        "Sets text to be striked out."
    },
    {
        "text-overline",
        scm_text_overline,
        PERMISSIONS_USER,
        "&Overline Text",
        "Sets text to be overlined."
    },
    {
        "zoom-real-time",
        scm_zoom_real_time,
        PERMISSIONS_USER,
        "Zoom &Realtime",
        "Zooms to increase or decrease the apparent size of objects in the current viewport."
    },
    {
        "zoom-previous",
        scm_zoom_previous,
        PERMISSIONS_USER,
        "Zoom &Previous",
        "Zooms to display the previous view."
    },
    {
        "zoom-window",
        scm_zoom_window,
        PERMISSIONS_USER,
        "Zoom &Window",
        "Zooms to display an area specified by a rectangular window."
    },
    {
        "zoom-dynamic",
        scm_zoom_dynamic,
        PERMISSIONS_USER,
        "Zoom &Dynamic",
        "Zooms to display the generated portion of the drawing."
    },
    {
        "zoom-scale",
        scm_zoom_scale,
        PERMISSIONS_USER,
        "Zoom Sca&le",
        "Zooms the display using a specified scale factor."
    },
    {
        "zoom-center",
        scm_zoom_center,
        PERMISSIONS_USER,
        "Zoom &Center",
        "Zooms to display a view specified by a center point and  magnification or height."
    },
    {
        "zoom-in",
        scm_zoom_in,
        PERMISSIONS_USER,
        "Zoom &In",
        "Zooms to increase the apparent size of objects."
    },
    {
        "zoom-out",
        scm_zoom_out,
        PERMISSIONS_USER,
        "Zoom &Out",
        "Zoom to decrease the apparent size of object."
    },
    {
        "zoom-selected",
        scm_zoom_selected,
        PERMISSIONS_USER,
        "Zoom Selec&ted",
        "Zooms to display the selected objects."
    },
    {
        "zoom-all",
        scm_zoom_all,
        PERMISSIONS_USER,
        "Zoom &All",
        "Zooms to display the drawing extents or the grid limits."
    },
    {
        "zoom-extents",
        scm_zoom_extents,
        PERMISSIONS_USER,
        "Zoom &Extents",
        "Zooms to display the drawing extents."
    },
    {
        "pan-real-time",
        scm_pan_real_time,
        PERMISSIONS_USER,
        "&Pan Realtime",
        "Moves the view in the current viewport."
    },
    {
        "pan-point",
        scm_pan_point,
        PERMISSIONS_USER,
        "Pan &Point",
        "Moves the view by the specified distance."
    },
    {
        "pan-left",
        scm_pan_left,
        PERMISSIONS_USER,
        "&Pan Left",
        "Moves the view to the left."
    },
    {
        "pan-right",
        scm_pan_right,
        PERMISSIONS_USER,
        "&Pan Right",
        "Moves the view to the right."
    },
    {
        "pan-up",
        scm_pan_up,
        PERMISSIONS_USER,
        "&Pan Up",
        "Moves the view up."
    },
    {
        "pan-down",
        scm_pan_down,
        PERMISSIONS_USER,
        "&Pan Down",
        "Moves the view down."
    },
    {
        "day-vision",
        scm_day_vision,
        PERMISSIONS_USER,
        "&Day",
        "Updates the current view using day vision settings."
    },
    {
        "night-vision",
        scm_night_vision,
        PERMISSIONS_USER,
        "&Night",
         "Updates the current view using night vision settings."
    },
    {
        "treble-clef",
        scm_treble_clef,
        PERMISSIONS_USER,
        "&TrebleClef",
        "Creates a treble clef: TREBLECLEF"
    },
    {
        "path",
        scm_path,
        PERMISSIONS_USER,
        "&Path",
		"Add a path object to the current view."
    },
    {
        "circle",
        scm_circle,
        PERMISSIONS_USER,
        "&Circle",
        "Adds a circle to the vector layer."
    },
    {
        "line",
        scm_line,
        PERMISSIONS_USER,
        "&Line",
        "Creates straight line segments: LINE"
    },
    {
        "distance",
        scm_distance,
        PERMISSIONS_USER,
        "&Distance",
        "Measures the distance and angle between two points: DIST"
    },
    {
        "dolphin",
        scm_dolphin,
        PERMISSIONS_USER,
        "&Dolphin",
		"Create a dolphin design in the vector layer."
    },
    {
        "ellipse",
        scm_ellipse,
        PERMISSIONS_USER,
        "&Ellipse",
        "Add an ellipse to the vector layer."
    },
    {
        "delete-object",
        scm_delete_object,
        PERMISSIONS_USER,
        "D&elete",
        "Removes objects from a drawing: delete"
    },
    {
        "heart",
        scm_heart,
        PERMISSIONS_USER,
        "&Heart",
        "Creates a heart: HEART"
    },
    {
        "locate-point",
        scm_locate_point,
        PERMISSIONS_USER,
        "&Locate Point",
        "Find a point that is near the indicated region."
    },
    {
        "move",
        scm_move,
        PERMISSIONS_USER,
        "&Move",
        "Displaces objects a specified distance in a specified direction: MOVE"
    },
    {
        "export",
        scm_export,
        PERMISSIONS_USER,
        "&Export",
        "Export the current tab as a non-stitch format that will lose more data."
    },
    {
        "create-ui-rect",
        scm_create_ui_rect,
        PERMISSIONS_SYSTEM,
        "NULL",
        "NULL"
    },
    {
        "create-label",
        scm_create_label,
        PERMISSIONS_SYSTEM,
        "NULL",
        "NULL"
    },
    {
        "create-widget",
        scm_create_widget,
        PERMISSIONS_SYSTEM,
        "NULL",
        "NULL"
    },
    {
        "create-icon",
        scm_create_icon,
        PERMISSIONS_SYSTEM,
        "NULL",
        "NULL"
    },
    {
        "set-visibility",
        scm_set_visibility,
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
        scm_heart4,
        PERMISSIONS_USER,
		"&Heart4",
		"Adds a heart to the vector layer: heart4."
		"assets/scripts/heart4.scm"
    },
    {
		"heart5",
        scm_heart5,
        PERMISSIONS_USER,
		"&Heart5",
		"Adds a heart to the vector layer."
		"assets/scripts/heart5.scm"
    },
    {
        "single-line-text",
        PERMISSIONS_USER,
        scm_single_line_text,
        "&Single Line Text",
        "Creates single-line text objects: TEXT"
    },
    {
        "spell-check",
        PERMISSIONS_USER | OBJ_TYPE_TEXT,
        scm_spell_check,
        "S&pell Check",
        "Spell check the text object selected."
    },
    {
        "quick-select",
        PERMISSIONS_USER,
        scm_quick_select,
    },
    {
        "rectangle",
        PERMISSIONS_USER,
        scm_rectangle,
        "&Rectangle",
        "Creates a rectangular polyline: RECTANGLE"
    },
    {
        "rgb",
        PERMISSIONS_USER,
        scm_rgb,
        "&RGB",
        "Updates the current view colors: RGB"
    },
    {
        "rotate",
        PERMISSIONS_USER,
        scm_rotate,
        "&Rotate",
        "Rotates objects about a base point: ROTATE"
    },
    {
        "sandbox",
        PERMISSIONS_USER,
        scm_sandbox,
        "&Sandbox",
        "A sandbox to play in: SANDBOX"
    },
    {
        "quickleader",
        scm_quickleader,
        PERMISSIONS_USER,
        "&QuickLeader",
        "Creates a leader and annotation: QUICKLEADER"
    },
    {
        "snowflake",
        scm_snowflake,
        PERMISSIONS_USER,
        "&Snowflake",
        "Creates a snowflake: SNOWFLAKE"
    },
    {
        "star",
        scm_star,
        PERMISSIONS_USER,
        "&Star",
        "Creates a star: STAR"
    },
    {
        "platform",
        scm_platform,
        PERMISSIONS_USER,
        "&Platform",
        "List which platform is in use: PLATFORM"
    },
    {
        "point",
        scm_point,
        PERMISSIONS_USER,
        "&Point",
        "Creates multiple points: POINT"
    },
    {
        "polygon",
        scm_polygon,
        PERMISSIONS_USER,
        "Pol&ygon",
        "Creates a regular polygon: POLYGON"
    },
    {
        "polyline",
        scm_polyline,
        PERMISSIONS_USER,
        "&Polyline",
        "Creates a 2D polyline: PLINE"
    },
    {
        "settings-dialog",
        scm_settings_dialog,
        PERMISSIONS_USER,
        "&Settings",
        "Configure settings specific to this product."
    },
    {
        "quickleader",
        scm_quickleader,
        PERMISSIONS_USER,
        "&QuickLeader",
        "Creates a leader and annotation: QUICKLEADER"
    },
    {
        "locate-point",
        scm_locate_point,
        PERMISSIONS_USER,
        "&Locate Point",
        "Displays the coordinate values of a location: ID"
    },
    {
        "point",
        scm_point,
        PERMISSIONS_USER,
        "&Point",
        "Creates multiple points: POINT"
    }
    */
