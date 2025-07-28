#include "mainwindow.h"
#include <QAction>
#include <QApplication>
#include <QMdiArea>

const Command command_map[] = {
    {
        .icon = "about",
        .command = "about-f",
        .tooltip = "&About Embroidermodder 2",
        .statustip = "Displays information about this product.",
        .shortcut = "",
        .macos = "",
        .checkable = 0,
        .function = about_f
    },
    {
        .icon = "alert",
        .command = "alert-f",
        .tooltip = "",
        .statustip = "",
        .shortcut = "",
        .macos = "",
        .checkable = 0,
        .function = alert_f
    },
    {
        .icon = "blink",
        .command = "blink-f",
        .tooltip = "",
        .statustip = "",
        .shortcut = "",
        .macos = "",
        .checkable = 0,
        .function = blink_f
    },
    {
        .icon = "changelog",
        .command = "changelog-f",
        .tooltip = "&Changelog",
        .statustip = "Describes new features in this product.",
        .shortcut = "",
        .macos = "",
        .checkable = 0,
        .function = changelog_f
    },
    {
        .icon = "circle",
        .command = "circle-f",
        .tooltip = "Circle",
        .statustip = "Creates a circle.",
        .shortcut = "",
        .macos = "",
        .checkable = 0,
        .function = circle_f
    },
    {
        .icon = "colorselector",
        .command = "color-selector-f",
        .tooltip = "&Color Selector",
        .statustip = "Dropdown selector for changing the current thread color",
        .shortcut = "",
        .macos = "",
        .checkable = 0,
        .function = color_selector_f
    },
    {
        .icon = "copy",
        .command = "copy-f",
        .tooltip = "&Copy",
        .statustip = "Copy the current selection's contents to the clipboard.",
        .shortcut = "Ctrl+C",
        .macos = "",
        .checkable = 0,
        .function = copy_f
    },
    {
        .icon = "cut",
        .command = "cut-f",
        .tooltip = "Cu&t",
        .statustip = "Cut the current selection's contents to the clipboard.",
        .shortcut = "Ctrl+X",
        .macos = "",
        .checkable = 0,
        .function = cut_f
    },
    {
        .icon = "debug",
        .command = "debug-f",
        .tooltip = "",
        .statustip = "",
        .shortcut = "",
        .macos = "",
        .checkable = 0,
        .function = debug_f
    },
    {
        .icon = "day",
        .tooltip = "&Day",
        .statustip = "Updates the current view using day vision settings.",
        .shortcut = "",
        .macos = "",
        .checkable = 0,
        .function = day_f
    },
    {
        .icon = "designdetails",
        .tooltip = "&Details",
        .statustip = "Details of the current design.",
        .shortcut = "Ctrl+D",
        .macos = "",
        .checkable = 0,
        .function = design_details_f
    },
    {
        .icon = "distance",
        .tooltip = "&Distance",
        .statustip = "Measures the distance and angle between two points.",
        .shortcut = "",
        .macos = "",
        .checkable = 0,
        .function = distance_f
    },
    {
        .icon = "dolphin",
        .tooltip = "&Dolphin",
        .statustip = "Creates a dolphin:  DOLPHIN",
        .shortcut = "",
        .macos = "",
        .checkable = 0,
        .function = dolphin_f
    },
    {
        .icon = "donothing",
        .tooltip = "&Do Nothing",
        .statustip = "Does Nothing",
        .shortcut = "",
        .macos = "",
        .checkable = 0,
        .function = do_nothing_f
    },
    {
        .icon = "ellipse",
        .tooltip = "Ellipse",
        .statustip = "Creates an ellipse.",
        .shortcut = "",
        .macos = "",
        .checkable = 0,
        .function = ellipse_f
    },
    {
        .icon = "erase",
        .tooltip = "D&elete",
        .statustip = "Removes objects from a drawing.",
        .shortcut = "",
        .macos = "",
        .checkable = 0,
        .function = erase_f
    },
    {
        .icon = "error",
        .tooltip = "",
        .statustip = "",
        .shortcut = "",
        .macos = "",
        .checkable = 0,
        .function = error_f
    },
    {
        .icon = "exit",
        .command = "exit-program-f",
        .tooltip = "E&xit",
        .statustip = "Exit the application.",
        .shortcut = "Ctrl+Q",
        .macos = "",
        .checkable = 0,
        .function = exit_program_f
    },
    {
        .icon = "freezealllayers",
        .tooltip = "&Freeze All Layers",
        .statustip = "Freezes all layers in the current drawing.",
        .shortcut = "",
        .macos = "",
        .checkable = 0,
        .function = freeze_all_layers_f
    },
    {
        .icon = "heart",
        .tooltip = "&Heart",
        .statustip = "Creates a heart.",
        .shortcut = "",
        .macos = "",
        .checkable = 0,
        .function = heart_f
    },
    {
        .icon = "help",
        .tooltip = "&Help",
        .statustip = "Displays the help file.",
        .shortcut = "",
        .macos = "",
        .checkable = 0,
        .function = help_f
    },
    {
        .icon = "help",
        .tooltip = "&Help",
        .statustip = "Displays help.",
        .shortcut = "",
        .macos = "",
        .checkable = 0,
        .function = help_f
    },
    {
        .icon = "hidealllayers",
        .tooltip = "&Hide All Layers",
        .statustip = "Turns the visibility off for all layers in the current drawing.",
        .shortcut = "",
        .macos = "",
        .checkable = 0,
        .function = hide_all_layers_f
    },
    {
        .icon = "icon128",
        .command = "icon128-f",
        .tooltip = "Icon12&8",
        .statustip = "Sets the toolbar icon size to 128x128.",
        .shortcut = "",
        .macos = "",
        .checkable = 0,
        .function = icon128_f
    },
    {
        .icon = "icon16",
        .command = "icon16-f",
        .tooltip = "Icon&16",
        .statustip = "Sets the toolbar icon size to 16x16.",
        .shortcut = "",
        .macos = "",
        .checkable = 0,
        .function = icon16_f
    },
    {
        .icon = "icon24",
        .command = "icon24-f",
        .tooltip = "Icon&24",
        .statustip = "Sets the toolbar icon size to 24x24.",
        .shortcut = "",
        .macos = "",
        .checkable = 0,
        .function = icon24_f
    },
    {
        .icon = "icon32",
        .command = "icon32-f",
        .tooltip = "Icon&32",
        .statustip = "Sets the toolbar icon size to 32x32.",
        .shortcut = "",
        .macos = "",
        .checkable = 0,
        .function = icon32_f
    },
    {
        .icon = "icon48",
        .command = "icon48-f",
        .tooltip = "Icon&48",
        .statustip = "Sets the toolbar icon size to 48x48.",
        .shortcut = "",
        .macos = "",
        .checkable = 0,
        .function = icon48_f
    },
    {
        .icon = "icon64",
        .command = "icon64-f",
        .tooltip = "Icon&64",
        .statustip = "Sets the toolbar icon size to 64x64.",
        .shortcut = "",
        .macos = "",
        .checkable = 0,
        .function = icon64_f
    },
    {
        .icon = "layers",
        .tooltip = "&Layers",
        .statustip = "Manages layers and layer properties:  LAYER",
        .shortcut = "",
        .macos = "",
        .checkable = 0,
        .function = layers_f
    },
    {
        .icon = "layerprevious",
        .tooltip = "&Layer Previous",
        .statustip = "Restores the previous layer settings:  LAYERP",
        .shortcut = "",
        .macos = "",
        .checkable = 0,
        .function = layer_previous_f
    },
    {
        .icon = "layerselector",
        .tooltip = "&Layer Selector",
        .statustip = "Dropdown selector for changing the current layer",
        .shortcut = "",
        .macos = "",
        .checkable = 0,
        .function = layer_selector_f
    },
    {
        .icon = "line",
        .tooltip = "&Line",
        .statustip = "Creates straight line segments.",
        .shortcut = "",
        .macos = "",
        .checkable = 0,
        .function = line_f
    },
    {
        .icon = "linetypeselector",
        .tooltip = "&Stitchtype Selector",
        .statustip = "Dropdown selector for changing the current stitch type",
        .shortcut = "",
        .macos = "",
        .checkable = 0,
        .function = line_type_selector_f
    },
    {
        .icon = "lineweightselector",
        .tooltip = "&Threadweight Selector",
        .statustip = "Dropdown selector for changing the current thread weight",
        .shortcut = "",
        .macos = "",
        .checkable = 0,
        .function = line_weight_selector_f
    },
    {
        .icon = "locatepoint",
        .command = "locate-point-f",
        .tooltip = "&Locate Point",
        .statustip = "Displays the coordinate values of a location.",
        .shortcut = "",
        .macos = "",
        .checkable = 0,
        .function = locate_point_f
    },
    {
        .icon = "lockalllayers",
        .tooltip = "&Lock All Layers",
        .statustip = "Locks all layers in the current drawing.",
        .shortcut = "",
        .macos = "",
        .checkable = 0,
        .function = lock_all_layers_f
    },
    {
        .icon = "makelayercurrent",
        .tooltip = "&Make Layer Active",
        .statustip = "Makes the layer of a selected object the active layer.",
        .shortcut = "",
        .macos = "",
        .checkable = 0,
        .function = make_layer_current_f
    },
    {
        .icon = "move",
        .tooltip = "&Move",
        .statustip = "Displaces objects a specified distance in a specified direction:  MOVE",
        .shortcut = "",
        .macos = "",
        .checkable = 0,
        .function = move_f
    },
    {
        .icon = "new",
        .command = "new-file-f",
        .tooltip = "&New",
        .statustip = "Create a new file.",
        .shortcut = "Ctrl+N",
        .macos = "",
        .checkable = 0,
        .function = new_file_f
    },
    {
        .icon = "night",
        .command = "night-vision-f",
        .tooltip = "&Night",
        .statustip = "Updates the current view using night vision settings.",
        .shortcut = "",
        .macos = "",
        .checkable = 0,
        .function = night_vision_f
    },
    {
        .icon = "open",
        .command = "open-file-f",
        .tooltip = "&Open",
        .statustip = "Open an existing file.",
        .shortcut = "Ctrl+O",
        .macos = "",
        .checkable = 0,
        .function = open_file_f
    },
    {
        .icon = "path",
        .command = "path-f",
        .tooltip = "&Path",
        .statustip = "Creates a 2D path.",
        .shortcut = "",
        .macos = "",
        .checkable = 0,
        .function = path_f
    },
    {
        .icon = "platform",
        .tooltip = "&Platform",
        .statustip = "List which platform is in use.",
        .shortcut = "",
        .macos = "",
        .checkable = 0,
        .function = platform_f
    },
    {
        .icon = "point",
        .tooltip = "&Point",
        .statustip = "Creates multiple points.",
        .shortcut = "",
        .macos = "",
        .checkable = 0,
        .function = point_f
    },
    {
        .icon = "polygon",
        .tooltip = "Pol&ygon",
        .statustip = "Creates a regular polygon.",
        .shortcut = "",
        .macos = "",
        .checkable = 0,
        .function = polygon_f
    },
    {
        .icon = "polyline",
        .tooltip = "&Polyline",
        .statustip = "Creates a 2D polyline.",
        .shortcut = "",
        .macos = "",
        .checkable = 0,
        .function = polyline_f
    },
    {
        .icon = "quickleader",
        .tooltip = "&QuickLeader",
        .statustip = "Creates a leader and annotation.",
        .shortcut = "",
        .macos = "",
        .checkable = 0,
        .function = quickleader_f
    },
    {
        .icon = "rectangle",
        .tooltip = "&Rectangle",
        .statustip = "Creates a rectangular polyline.",
        .shortcut = "",
        .macos = "",
        .checkable = 0,
        .function = rectangle_f
    },
    {
        .icon = "redo",
        .tooltip = "&Redo",
        .statustip = "Reverses the effects of the previous undo action.",
        .shortcut = "",
        .macos = "",
        .checkable = 0,
        .function = redo_f
    },
    {
        .icon = "rgb",
        .tooltip = "&RGB",
        .statustip = "Updates the current view colors.",
        .shortcut = "",
        .macos = "",
        .checkable = 0,
        .function = rgb_f
    },
    {
        .icon = "rotate",
        .tooltip = "&Rotate",
        .statustip = "Rotates objects about a base point.",
        .shortcut = "",
        .macos = "",
        .checkable = 0,
        .function = rotate_f
    },
    {
        .icon = "selectall",
        .tooltip = "&Select All",
        .statustip = "Selects all objects:  SELECTALL",
        .shortcut = "Ctrl+A",
        .macos = "",
        .checkable = 0,
        .function = selectall_f
    },
    {
        .icon = "singlelinetext",
        .tooltip = "&Single Line Text",
        .statustip = "Creates single-line text objects:  TEXT",
        .shortcut = "",
        .macos = "",
        .checkable = 0,
        .function = singlelinetext_f
    },
    {
        .icon = "pandown",
        .command = "pan-down-f",
        .tooltip = "&Pan Down",
        .statustip = "Moves the view down.",
        .shortcut = "",
        .macos = "",
        .checkable = 0,
        .function = pan_down_f
    },
    {
        .icon = "panleft",
        .command = "pan-left-f",
        .tooltip = "&Pan Left",
        .statustip = "Moves the view to the left.",
        .shortcut = "",
        .macos = "",
        .checkable = 0,
        .function = pan_left_f
    },
    {
        .icon = "panpoint",
        .command = "pan-point-f",
        .tooltip = "&Pan Point",
        .statustip = "Moves the view by the specified distance.",
        .shortcut = "",
        .macos = "",
        .checkable = 0,
        .function = pan_point_f
    },
    {
        .icon = "panrealtime",
        .command = "pan-real-time-f",
        .tooltip = "&Pan Realtime",
        .statustip = "Moves the view in the current viewport.",
        .shortcut = "",
        .macos = "",
        .checkable = 0,
        .function = pan_real_time_f
    },
    {
        .icon = "panright",
        .command = "pan-right-f",
        .tooltip = "&Pan Right",
        .statustip = "Moves the view to the right.",
        .shortcut = "",
        .macos = "",
        .checkable = 0,
        .function = pan_right_f
    },
    {
        .icon = "panup",
        .command = "pan-up-f",
        .tooltip = "&Pan Up",
        .statustip = "Moves the view up.",
        .shortcut = "",
        .macos = "",
        .checkable = 0,
        .function = pan_up_f
    },
    {
        .icon = "paste",
        .tooltip = "&Paste",
        .statustip = "Paste the clipboard's contents into the current selection.",
        .shortcut = "Ctrl+V",
        .macos = "",
        .checkable = 0,
        .function = paste_f
    },
    {
        .icon = "print",
        .tooltip = "&Print",
        .statustip = "Print the design.",
        .shortcut = "Ctrl+P",
        .macos = "",
        .checkable = 0,
        .function = print_f
    },
    {
        .icon = "redo",
        .tooltip = "&Redo",
        .statustip = "Reverses the effects of the previous undo action.",
        .shortcut = "Ctrl+Shift+Z",
        .macos = "",
        .checkable = 0,
        .function = redo_f
    },
    {
        .icon = "save",
        .tooltip = "&Save",
        .statustip = "Save the design to disk.",
        .shortcut = "Ctrl+S",
        .macos = "",
        .checkable = 0,
        .function = save_f
    },
    {
        .icon = "saveas",
        .tooltip = "Save &As",
        .statustip = "Save the design under a new name.",
        .shortcut = "Ctrl+Shift+S",
        .macos = "",
        .checkable = 0,
        .function = saveas_f
    },
    {
        .icon = "scale",
        .tooltip = "Sca&le",
        .statustip = "Enlarges or reduces objects proportionally in the X, Y, and Z directions.",
        .shortcut = "",
        .macos = "",
        .checkable = 0,
        .function = scale_f
    },
    {
        .icon = "settingsdialog",
        .tooltip = "&Settings",
        .statustip = "Configure settings specific to this product.",
        .shortcut = "",
        .macos = "",
        .checkable = 0,
        .function = settings_dialog_f
    },
    {
        .icon = "showalllayers",
        .tooltip = "&Show All Layers",
        .statustip = "Turns the visibility on for all layers in the current drawing:  SHOWALL",
        .shortcut = "",
        .macos = "",
        .checkable = 0,
        .function = show_all_layers_f
    },
    {
        .icon = "snowflake",
        .tooltip = "&Snowflake",
        .statustip = "Creates a snowflake.",
        .shortcut = "",
        .macos = "",
        .checkable = 0,
        .function = snowflake_f
    },
    {
        .icon = "star",
        .tooltip = "&Star",
        .statustip = "Creates a star.",
        .shortcut = "",
        .macos = "",
        .checkable = 0,
        .function = star_f
    },
    {
        .icon = "syswindows",
        .tooltip = "&SysWindows",
        .statustip = "Arrange the windows.",
        .shortcut = "",
        .macos = "",
        .checkable = 0,
        .function = syswindows_f
    },
    {
        .icon = "textbold",
        .tooltip = "&Bold Text",
        .statustip = "Sets text to be bold.",
        .shortcut = "",
        .macos = "",
        .checkable = 1,
        .function = text_bold_f
    },
    {
        .icon = "textitalic",
        .tooltip = "&Italic Text",
        .statustip = "Sets text to be italic.",
        .shortcut = "",
        .macos = "",
        .checkable = 1,
        .function = text_italic_f
    },
    {
        .icon = "textoverline",
        .tooltip = "&Overline Text",
        .statustip = "Sets text to be overlined.",
        .shortcut = "",
        .macos = "",
        .checkable = 1,
        .function = text_overline_f
    },
    {
        .icon = "textstrikeout",
        .tooltip = "&StrikeOut Text",
        .statustip = "Sets text to be striked out.",
        .shortcut = "",
        .macos = "",
        .checkable = 1,
        .function = text_strikeout_f
    },
    {
        .icon = "textunderline",
        .tooltip = "&Underline Text",
        .statustip = "Sets text to be underlined.",
        .shortcut = "",
        .macos = "",
        .checkable = 1,
        .function = text_underline_f
    },
    {
        .icon = "thawalllayers",
        .tooltip = "&Thaw All Layers",
        .statustip = "Thaws all layers in the current drawing:  THAWALL",
        .shortcut = "",
        .macos = "",
        .checkable = 0,
        .function = thaw_all_layers_f
    },
    {
        .icon = "tipoftheday",
        .tooltip = "&Tip Of The Day",
        .statustip = "Displays a dialog with useful tips",
        .shortcut = "",
        .macos = "",
        .checkable = 0,
        .function = tip_of_the_day_f
    },
    {
        .icon = "todo",
        .tooltip = "",
        .statustip = "",
        .shortcut = "",
        .macos = "",
        .checkable = 0,
        .function = todo_f
    },
    {
        .icon = "undo",
        .tooltip = "&Undo",
        .statustip = "Reverses the most recent action.",
        .shortcut = "Ctrl+Z",
        .macos = "",
        .checkable = 0,
        .function = undo_f
    },
    {
        .icon = "unlockalllayers",
        .tooltip = "&Unlock All Layers",
        .statustip = "Unlocks all layers in the current drawing:  UNLOCKALL",
        .shortcut = "",
        .macos = "",
        .checkable = 0,
        .function = unlock_all_layers_f
    },
    {
        .icon = "whatsthis",
        .tooltip = "&What's This?",
        .statustip = "What's This? Context Help!",
        .shortcut = "",
        .macos = "",
        .checkable = 0,
        .function = whats_this_f
    },
    {
        .icon = "windowcascade",
        .tooltip = "&Cascade",
        .statustip = "Cascade the windows.",
        .shortcut = "",
        .macos = "",
        .checkable = 0,
        .function = window_cascade_f
    },
    {
        .icon = "windowclose",
        .tooltip = "Cl&ose",
        .statustip = "Close the active window.",
        .shortcut = "",
        .macos = "",
        .checkable = 0,
        .function = window_close_f
    },
    {
        .icon = "windowcloseall",
        .tooltip = "Close &All",
        .statustip = "Close all the windows.",
        .shortcut = "",
        .macos = "",
        .checkable = 0,
        .function = window_close_all_f
    },
    {
        .icon = "windownext",
        .tooltip = "Ne&xt",
        .statustip = "Move the focus to the next window.",
        .shortcut = "",
        .macos = "",
        .checkable = 0,
        .function = window_next_f
    },
    {
        .icon = "windowprevious",
        .tooltip = "Pre&vious",
        .statustip = "Move the focus to the previous window.",
        .shortcut = "",
        .macos = "",
        .checkable = 0,
        .function = window_previous_f
    },
    {
        .icon = "windowtile",
        .tooltip = "&Tile",
        .statustip = "Tile the windows.",
        .shortcut = "",
        .macos = "",
        .checkable = 0,
        .function = window_tile_f
    },
    {
        .icon = "zoomall",
        .tooltip = "Zoom &All",
        .statustip = "Zooms to display the drawing extents or the grid limits.",
        .shortcut = "",
        .macos = "",
        .checkable = 0,
        .function = zoom_all_f
    },
    {
        .icon = "zoomcenter",
        .tooltip = "Zoom &Center",
        .statustip = "Zooms to display a view specified by a center point and magnification or height.",
        .shortcut = "",
        .macos = "",
        .checkable = 0,
        .function = zoom_center_f
    },
    {
        .icon = "zoomdynamic",
        .tooltip = "Zoom &Dynamic",
        .statustip = "Zooms to display the generated portion of the drawing.",
        .shortcut = "",
        .macos = "",
        .checkable = 0,
        .function = zoom_dynamic_f
    },
    {
        .icon = "zoomextents",
        .tooltip = "Zoom &Extents",
        .statustip = "Zooms to display the drawing extents.",
        .shortcut = "",
        .macos = "",
        .checkable = 0,
        .function = zoom_extents_f
    },
    {
        .icon = "zoomin",
        .tooltip = "Zoom &In",
        .statustip = "Zooms to increase the apparent size of objects.",
        .shortcut = "",
        .macos = "",
        .checkable = 0,
        .function = zoom_in_f
    },
    {
        .icon = "zoomout",
        .tooltip = "Zoom &Out",
        .statustip = "Zooms to decrease the apparent size of objects.",
        .shortcut = "",
        .macos = "",
        .checkable = 0,
        .function = zoom_out_f
    },
    {
        .icon = "zoomscale",
        .tooltip = "Zoom &Scale",
        .statustip = "Zooms the display using a specified scale factor.",
        .shortcut = "",
        .macos = "",
        .checkable = 0,
        .function = zoom_scale_f
    },
    {
        .icon = "zoomprevious",
        .tooltip = "Zoom &Previous",
        .statustip = "Zooms to display the previous view.",
        .shortcut = "",
        .macos = "",
        .checkable = 0,
        .function = zoom_previous_f
    },
    {
        .icon = "zoomrealtime",
        .tooltip = "Zoom &Realtime",
        .statustip = "Zooms to increase or decrease the apparent size of objects in the current viewport.",
        .shortcut = "",
        .macos = "",
        .checkable = 0,
        .function = zoom_real_time_f
    },
    {
        .icon = "zoomselected",
        .tooltip = "Zoom Selec&ted",
        .statustip = "Zooms to display the selected objects.",
        .shortcut = "",
        .macos = "",
        .checkable = 0,
        .function = zoom_selected_f
    },
    {
        .icon = "zoomwindow",
        .command = "zoom-window-f",
        .tooltip = "Zoom &Window",
        .statustip = "Zooms to display an area specified by a rectangular window.",
        .shortcut = "",
        .macos = "",
        .checkable = 0,
        .function = zoom_window_f
    },
    {
        .icon = "^END",
        .command = "do-nothing-f",
        .tooltip = "^END",
        .statustip = "^END",
        .shortcut = "",
        .macos = "",
        .checkable = 0,
        .function = do_nothing_f
    }
};

void MainWindow::createAllActions()
{
    qDebug("Creating All Actions...");
    QString appDir = qApp->applicationDirPath();
    QString theme = appDir + "/icons/" + getSettingsGeneralIconTheme();

    for (int i=0; command_map[i].icon[0] != '^'; i++) {
        QString icon = command_map[i].icon;
        QString command = command_map[i].command;
        QString tooltip = tr(command_map[i].tooltip);
        QString statustip = tr(command_map[i].statustip);
        QString shortcut = command_map[i].shortcut;
        QString macos = command_map[i].macos;

        QAction *ACTION = new QAction(QIcon(theme + "/" + icon + ".png"), tooltip, this);
        ACTION->setStatusTip(statustip);
        ACTION->setObjectName(icon);
        // TODO: Set What's This Context Help to statustip for now so there is some infos there.
        // Make custom whats this context help popup with more descriptive help than just
        // the status bar/tip one liner(short but not real long) with a hyperlink in the custom popup
        // at the bottom to open full help file description. Ex: like wxPython AGW's SuperToolTip.
        ACTION->setWhatsThis(statustip);
        // TODO: Finish All Commands ... <.<

        if (shortcut != "") {
            ACTION->setShortcut(shortcut);
        }
        if (command_map[i].checkable != 0) {
            ACTION->setCheckable(true);
            // FIXME: connect(ACTION, SIGNAL(toggled(bool)), this, SLOT(setTextBold(bool)));
        }
        connect(ACTION, &QAction::triggered, this,
            [=]() { runCommandMain(command); } );

        /*
        else if(icon == "windowcascade") connect(ACTION, SIGNAL(triggered()), mdiArea, SLOT(cascade()));
        else if(icon == "windowtile") connect(ACTION, SIGNAL(triggered()), mdiArea, SLOT(tile()));
        else if(icon == "windowclose") { ACTION->setShortcut(QKeySequence::Close);    connect(ACTION, SIGNAL(triggered()), this, SLOT(onCloseWindow()));   }
        else if(icon == "windowcloseall") connect(ACTION, SIGNAL(triggered()), mdiArea, SLOT(closeAllSubWindows()));
        else if(icon == "windownext") { ACTION->setShortcut(QKeySequence::NextChild);     connect(ACTION, SIGNAL(triggered()), mdiArea, SLOT(activateNextSubWindow()));     }
        else if(icon == "windowprevious") { ACTION->setShortcut(QKeySequence::PreviousChild); connect(ACTION, SIGNAL(triggered()), mdiArea, SLOT(activatePreviousSubWindow())); }
        */

        actionHash.insert(command_map[i].icon, ACTION);
    }

    actionHash.value("windowclose")->setEnabled(numOfDocs > 0);
    actionHash.value("designdetails")->setEnabled(numOfDocs > 0);
}

