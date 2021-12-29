#!/usr/bin/env python3

r"""
    Embroidermodder 2.

    -----

    Copyright 2013-2022 The Embroidermodder Team
    Embroidermodder 2 is Open Source Software.
    See LICENSE for licensing terms.

    -----

"""

class Layer_Manager():
    def __init__(self, tab):
        return self

    def add(self, name, visible=True, frozen=False, zValue=0, color="black",
            lineType="solid", line_weight=0.35):
        return

    def layer_model(self):
        return

    def layer_model_sorted(self):
        return

    def tree_view(self):
        return

    LayerManager_LayerManager(MainWindow* mw, QWidget* parent):
        layerModel = new QStandardItemModel(0, 8, this)

        layerModelSorted = new QSortFilterProxyModel
        layerModelSorted.setDynamicSortFilter(1)
        layerModelSorted.setSourceModel(layerModel)

        treeView = new QTreeView
        treeView.setRootIsDecorated(0)
        treeView.setAlternatingRowColors(1)
        treeView.setModel(layerModelSorted)
        treeView.setSortingEnabled(1)
        treeView.sortByColumn(0, Qt_AscendingOrder)

        QVBoxLayout *mainLayout = new QVBoxLayout
        mainLayout.addWidget(treeView)
        setLayout(mainLayout)

        setWindowTitle(translate("Layer Manager"))
        setMinimumSize(750, 550)

        layerModel.setHeaderData(0, Qt_Horizontal, translate("Name"))
        layerModel.setHeaderData(1, Qt_Horizontal, translate("Visible"))
        layerModel.setHeaderData(2, Qt_Horizontal, translate("Frozen"))
        layerModel.setHeaderData(3, Qt_Horizontal, translate("Z Value"))
        layerModel.setHeaderData(4, Qt_Horizontal, translate("Color"))
        layerModel.setHeaderData(5, Qt_Horizontal, translate("Linetype"))
        layerModel.setHeaderData(6, Qt_Horizontal, translate("Lineweight"))
        layerModel.setHeaderData(7, Qt_Horizontal, translate("Print"))

        addLayer("0", 1, 0, 0.0, qRgb(0,0,0), "Continuous", "Default", 1)
        addLayer("1", 1, 0, 1.0, qRgb(0,0,0), "Continuous", "Default", 1)
        addLayer("2", 1, 0, 2.0, qRgb(0,0,0), "Continuous", "Default", 1)
        addLayer("3", 1, 0, 3.0, qRgb(0,0,0), "Continuous", "Default", 1)
        addLayer("4", 1, 0, 4.0, qRgb(0,0,0), "Continuous", "Default", 1)
        addLayer("5", 1, 0, 5.0, qRgb(0,0,0), "Continuous", "Default", 1)
        addLayer("6", 1, 0, 6.0, qRgb(0,0,0), "Continuous", "Default", 1)
        addLayer("7", 1, 0, 7.0, qRgb(0,0,0), "Continuous", "Default", 1)
        addLayer("8", 1, 0, 8.0, qRgb(0,0,0), "Continuous", "Default", 1)
        addLayer("9", 1, 0, 9.0, qRgb(0,0,0), "Continuous", "Default", 1)

        for(i = 0; i < layerModel.columnCount(); ++i)
            treeView.resizeColumnToContents(i)

        QApplication_setOverrideCursor(Qt_ArrowCursor)

    LayerManager_~LayerManager():
        QApplication_restoreOverrideCursor()

    def LayerManager_addLayer(const QString& name,
                                const visible,
                                const frozen,
                                const zValue,
                                const unsigned color,
                                const QString& lineType,
                                const QString& lineWeight,
                                const print):
        layerModel.insertRow(0)
        layerModel.setData(layerModel.index(0, 0), name)
        layerModel.setData(layerModel.index(0, 1), visible)
        layerModel.setData(layerModel.index(0, 2), frozen)
        layerModel.setData(layerModel.index(0, 3), zValue)

        QPixmap colorPix(QSize(16,16))
        colorPix.fill(Color(color))
        layerModel.itemFromIndex(layerModel.index(0, 4)).setIcon(QIcon(colorPix))
        layerModel.setData(layerModel.index(0, 4), Color(color))

        layerModel.setData(layerModel.index(0, 5), lineType)
        layerModel.setData(layerModel.index(0, 6), lineWeight)
        layerModel.setData(layerModel.index(0, 7), print)
