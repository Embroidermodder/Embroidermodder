#ifndef TEST_DIALOG_H
#define TEST_DIALOG_H

#include <QDialog>

QT_BEGIN_NAMESPACE
class QDialogButtonBox;
class QGraphicsScene;
QT_END_NAMESPACE

class TestDialog : public QDialog
{
    Q_OBJECT

public:
    TestDialog(QWidget *parent = 0);
    ~TestDialog();

private:
    QWidget *mainWidget;

    QWidget *createMainWidget();
    QWidget *createHistogram();

    QDialogButtonBox* buttonBox;

    QRectF boundingRect;
};

#endif

