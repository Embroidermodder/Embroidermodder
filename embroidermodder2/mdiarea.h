#ifndef MDIAREA_H
#define MDIAREA_H

#include <QMdiArea>
#include <QPixmap>

class MainWindow;

QT_BEGIN_NAMESPACE
QT_END_NAMESPACE

class MdiArea : public QMdiArea
{
    Q_OBJECT

public:
    MdiArea(MainWindow* mw, QWidget* parent = 0);
    ~MdiArea();

    void useBackgroundLogo(bool use);
    void useBackgroundTexture(bool use);
    void useBackgroundColor(bool use);

    void setBackgroundLogo(const QString& fileName);
    void setBackgroundTexture(const QString& fileName);
    void setBackgroundColor(const QColor& color);

public slots:
    void cascade();
    void tile();
protected:
    virtual void mouseDoubleClickEvent(QMouseEvent* e);
    virtual void paintEvent(QPaintEvent* e);

private:
    MainWindow* mainWin;

    bool useLogo;
    bool useTexture;
    bool useColor;

    QPixmap bgLogo;
    QPixmap bgTexture;
    QColor  bgColor;

    void zoomExtentsAllSubWindows();
    void forceRepaint();
};

#endif

/* kate: bom off; indent-mode cstyle; indent-width 4; replace-trailing-space-save on; */
