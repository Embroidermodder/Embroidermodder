#ifndef LAYER_MANAGER_H
#define LAYER_MANAGER_H

#include <QDialog>

class MainWindow;

QT_BEGIN_NAMESPACE
class QAbstractItemModel;
class QCheckBox;
class QComboBox;
class QGroupBox;
class QLabel;
class QLineEdit;
class QSortFilterProxyModel;
class QTreeView;
class QStandardItemModel;
QT_END_NAMESPACE

class LayerManager : public QDialog
{
    Q_OBJECT

public:
    LayerManager(MainWindow* mw, QWidget *parent = 0);
    ~LayerManager();

void addLayer(const QString& name,
              const bool visible,
              const bool frozen,
              const qreal zValue,
              const QRgb color,
              const QString& lineType,
              const QString& lineWeight,
              const bool print);

private slots:

private:
    QStandardItemModel*    layerModel;
    QSortFilterProxyModel* layerModelSorted;
    QTreeView*             treeView;
};

#endif
