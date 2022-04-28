#!/usr/bin/env python3

r"""
    Embroidermodder 2.

    -----

    Copyright 2013-2022 The Embroidermodder Team
    Embroidermodder 2 is Open Source Software.
    See LICENSE for licensing terms.

    -----

"""

import tkinter as tk


class LayerManager():
    r" . "
    def __init__(self, tab, mw, parent):
        r"."
        layer_model = tk.StandardItemModel(mw, 0, 8)

        layer_model_sorted = tk.SortFilterProxyModel()
        layer_model_sorted.setDynamicSortFilter(1)
        layer_model_sorted.setSourceModel(layer_model)

        treeView = tk.TreeView(mw)
        treeView.setRootIsDecorated(0)
        treeView.setAlternatingRowColors(1)
        treeView.setModel(layer_model_sorted)
        treeView.setSortingEnabled(1)
        treeView.sortByColumn(0, "AscendingOrder")

        mainLayout = tk.VBoxLayout()
        mainLayout.addWidget(treeView)
        set_layout(mainLayout)

        set_window_title(translate("Layer Manager"))
        set_minimum_size(750, 550)

        layer_model.set_header_data(0, "horizontal", translate("Name"))
        layer_model.set_header_data(1, "horizontal", translate("Visible"))
        layer_model.set_header_data(2, "horizontal", translate("Frozen"))
        layer_model.set_header_data(3, "horizontal", translate("Z Value"))
        layer_model.set_header_data(4, "horizontal", translate("Color"))
        layer_model.set_header_data(5, "horizontal", translate("Linetype"))
        layer_model.set_header_data(6, "horizontal", translate("Lineweight"))
        layer_model.set_header_data(7, "horizontal", translate("Print"))

        add_layer("0", 1, 0, 0.0, (0, 0, 0), "Continuous", "Default", 1)
        add_layer("1", 1, 0, 1.0, (0, 0, 0), "Continuous", "Default", 1)
        add_layer("2", 1, 0, 2.0, (0, 0, 0), "Continuous", "Default", 1)
        add_layer("3", 1, 0, 3.0, (0, 0, 0), "Continuous", "Default", 1)
        add_layer("4", 1, 0, 4.0, (0, 0, 0), "Continuous", "Default", 1)
        add_layer("5", 1, 0, 5.0, (0, 0, 0), "Continuous", "Default", 1)
        add_layer("6", 1, 0, 6.0, (0, 0, 0), "Continuous", "Default", 1)
        add_layer("7", 1, 0, 7.0, (0, 0, 0), "Continuous", "Default", 1)
        add_layer("8", 1, 0, 8.0, (0, 0, 0), "Continuous", "Default", 1)
        add_layer("9", 1, 0, 9.0, (0, 0, 0), "Continuous", "Default", 1)

        for i in range(layer_model.count()):
            treeView.resizeColumnToContents(i)

        Application_setOverrideCursor(Qt_ArrowCursor)

        return self

    def add(self, name, visible=True, frozen=False, zValue=0, color="black",
            lineType="solid", line_weight=0.35):
        r"."
        # const print):
        layer_model.insertRow(0)
        layer_model.set_data(layer_model.index(0, 0), name)
        layer_model.set_data(layer_model.index(0, 1), visible)
        layer_model.set_data(layer_model.index(0, 2), frozen)
        layer_model.set_data(layer_model.index(0, 3), zValue)

        colorPix = QPixmap(16, 16)
        colorPix.fill(Color(color))
        layer_model.itemFromIndex(layer_model.index(0, 4)).setIcon(QIcon(colorPix))
        layer_model.set_data(layer_model.index(0, 4), Color(color))

        layer_model.set_data(layer_model.index(0, 5), lineType)
        layer_model.set_data(layer_model.index(0, 6), lineWeight)
        #layer_model.set_data(layer_model.index(0, 7), print)

    def layer_model(self):
        r"."
        return

    def layer_model_sorted(self):
        r"."
        return

    def tree_view(self):
        r"."
        return

    def __del__(self):
        r"."
        self.mw.restore_override_cursor()
