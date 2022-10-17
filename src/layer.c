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
 *  Layer functions.
 */

#include "em2.h"

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

