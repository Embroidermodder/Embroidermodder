#ifndef MDIWINDOW_H
#define MDIWINDOW_H
//Qt
#include <QObject>
#include <QMainWindow>
#include <QMdiSubWindow>
#include <QScrollBar>
#include <QGridLayout>

#if QT_VERSION >= 0x050000
//Qt5
#include <QtPrintSupport>
#else
//Qt4
#include <QPrinter>
#endif


#include "mainwindow.h"

class MainWindow;
class View;

QT_BEGIN_NAMESPACE
class QMdiArea;
class QGraphicsScene;
class QGraphicsView;
QT_END_NAMESPACE

class MDIWindow: public QMdiSubWindow
{
    Q_OBJECT

public:
    MDIWindow(const int theIndex, MainWindow* mw, QMdiArea* parent, Qt::WindowFlags wflags);
    ~MDIWindow();

    virtual QSize              sizeHint() const;
    QString                    getCurrentFile()   { return curFile; }
    QString                    getShortCurrentFile();
    View*                      getView() { return gview; }
    QGraphicsScene*            getScene() { return gscene; }
    QString                    getCurrentLayer() { return curLayer; }
    QRgb                       getCurrentColor() { return curColor; }
    QString                    getCurrentLineType() { return curLineType; }
    QString                    getCurrentLineWeight() { return curLineWeight; }
    void                       designDetails();
    bool                       loadFile(const QString &fileName);
    bool                       saveFile(const QString &fileName);
signals:
    void                       message(const QString&, int );
    void                       sendCloseMdiWin(MDIWindow*);

public slots:
    void                       closeEvent(QCloseEvent* e);
    void                       onWindowActivated();

    void                       currentLayerChanged(const QString& layer);
    void                       currentColorChanged(const QRgb& color);
    void                       currentLinetypeChanged(const QString& type);
    void                       currentLineweightChanged(const QString& weight);

    void                       updateColorLinetypeLineweight();
    void                       deletePressed();
    void                       escapePressed();

    void                       showViewScrollBars(bool val);
    void                       setViewCrossHairColor(QRgb color);
    void                       setViewBackgroundColor(QRgb color);
    void                       setViewGridColor(QRgb color);
    void                       setViewRulerColor(QRgb color);

    void                       print();
    void                       saveBMC();

protected:

private:
    MainWindow*                mainWin;
    QMdiArea*                  mdiArea;
    QGraphicsScene*            gscene;
    View*                      gview;

    QPrinter                   printer;

    QString                    curFile;
    void                       setCurrentFile(const QString& fileName);
    QString                    fileExtension(const QString& fileName);

    int                        myIndex;

    QString                    curLayer;
    QRgb                       curColor;
    QString                    curLineType;
    QString                    curLineWeight;
};

#endif

/* kate: bom off; indent-mode cstyle; indent-width 4; replace-trailing-space-save on; */
