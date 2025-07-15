#ifndef EMBROIDERMODDER_H
#define EMBROIDERMODDER_H

#include "embroidery.h"
#undef bufferSize

#include "scheme.h"
#include "scheme-private.h"

#include "cmdprompt.h"
#include "dialog.h"
#include "dynload.h"
#include "imagewidget.h"
#include "layer-manager.h"
#include "mainwindow.h"
#include "mdiarea.h"
#include "mdiwindow.h"
#include "object.h"
#include "preview-dialog.h"
#include "property-editor.h"
#include "script.h"
#include "selectbox.h"
#include "settings-dialog.h"
#include "statusbar-button.h"
#include "statusbar.h"
#include "undo-commands.h"
#include "undo-editor.h"
#include "view.h"

#include <algorithm>
#include <iostream>
#include <map>
#include <unordered_map>
#include <vector>
#include <string>

#include <math.h>
#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <QAction>
#include <QApplication>
#include <QBrush>
#include <QClipboard>
#include <QCloseEvent>
#include <QColor>
#include <QComboBox>
#include <QContextMenuEvent>
#include <QDate>
#include <QDateTime>
#include <QDebug>
#include <QDesktopServices>
#include <QDialog>
#include <QDialogButtonBox>
#include <QDir>
#include <QDockWidget>
#include <QFile>
#include <QFileDialog>
#include <QFileInfo>
#include <QFontComboBox>
#include <QFormLayout>
#include <QFrame>
#include <QGraphicsItem>
#include <QGraphicsPathItem>
#include <QGraphicsScene>
#include <QGraphicsView>
#include <QGridLayout>
#include <QGroupBox>
#include <QHash>
#include <QHeaderView>
#include <QImage>
#include <QKeyEvent>
#include <QLabel>
#include <QLineEdit>
#include <QList>
#include <QLocale>
#include <QMainWindow>
#include <QMdiArea>
#include <QMdiSubWindow>
#include <QMenu>
#include <QMenuBar>
#include <QMessageBox>
#include <QMetaObject>
#include <QObject>
//#include <QOpenGLWidget>
#include <QPainter>
#include <QPainterPath>
#include <QPen>
#include <QPixmap>
#include <QPointF>
#include <QProcess>
#include <QPushButton>
#include <QRubberBand>
#include <QScreen>
#include <QScrollArea>
#include <QScrollBar>
#include <QSettings>
#include <QSignalMapper>
#include <QSplitter>
#include <QStandardPaths>
#include <QStatusBar>
#include <QString>
#include <QStyle>
#include <QStyleOption>
#include <QtCore/qmath.h>
#include <QTextBrowser>
#include <QTextLayout>
#include <QTextStream>
#include <QtGlobal>
#include <QtGui>
#include <QTimer>
#include <QToolBar>
#include <QToolButton>
#include <QtPrintSupport>
#include <QTransform>
#include <QUndoCommand>
#include <QUndoGroup>
#include <QUndoStack>
#include <QUndoView>
#include <QUrl>
#include <QVBoxLayout>
#include <QWhatsThis>
#include <QWidget>
#include <QXmlStreamReader>

class MainWindow;

typedef pointer SchemeFunction(scheme *, pointer);

/*!
 * If the function is an in-built then the flag is set to 1 and main, click,
 * move and context arrays don't contain the function definition: instead
 * the function pointer to command contains the definition.
 *
 * TODO: replace the alias system with simple wrappers:
 * (define (alias) (function))
 */
typedef struct SchemeData_ {
    const char name[100];
    const char tooltip[1000];
    const char statustip[1000];
    SchemeFunction *function;
} SchemeData;

// On Mac, if the user drops a file on the app's Dock icon, or uses Open As, then this is how the app actually opens the file.
class Application : public QApplication
{
    Q_OBJECT
public:
    Application(int argc, char **argv);
    void setMainWin(MainWindow* mainWin) { _mainWin = mainWin; }
protected:
    virtual bool event(QEvent *e);
private:
    MainWindow* _mainWin;
};

#endif /* EMBROIDERMODDER_H */
