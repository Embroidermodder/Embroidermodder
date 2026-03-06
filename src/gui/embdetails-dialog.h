#ifndef EMBDETAILS_DIALOG_H
#define EMBDETAILS_DIALOG_H

#include <QDialog>

QT_BEGIN_NAMESPACE
class QDialogButtonBox;
class QGraphicsScene;
QT_END_NAMESPACE

class EmbDetailsDialog : public QDialog
{
    Q_OBJECT

public:
    EmbDetailsDialog(QGraphicsScene* theScene, QWidget *parent = 0);
    ~EmbDetailsDialog();

private:
    QWidget*          mainWidget;

    void              getInfo();
    QWidget*          createMainWidget();
    QWidget*          createHistogram();

    QDialogButtonBox* buttonBox;

    quint32 stitchesTotal;
    quint32 stitchesReal;
    quint32 stitchesJump;
    quint32 stitchesTrim;
    quint32 colorTotal;
    quint32 colorChanges;

    QRectF boundingRect;
};

#endif

/* kate: bom off; indent-mode cstyle; indent-width 4; replace-trailing-space-save on; */
