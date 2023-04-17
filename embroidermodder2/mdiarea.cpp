/**
 * \file mdiarea.cpp
 */

#include "mdiarea.h"
#include "mainwindow.h"
#include "view.h"

/**
 * @brief MdiArea::MdiArea
 * @param mw
 * @param parent
 */
MdiArea::MdiArea(MainWindow* mw, QWidget *parent) : QMdiArea(parent), mainWin(mw)
{
    #if QT_VERSION >= 0x040800
        setTabsClosable(true);
    #endif

    useLogo = false;
    useTexture = false;
    useColor = false;
}

/**
 * @brief MdiArea::~MdiArea
 */
MdiArea::~MdiArea()
{
}

/**
 * @brief MdiArea::useBackgroundLogo
 * @param use
 */
void
MdiArea::useBackgroundLogo(bool use)
{
    useLogo = use;
    forceRepaint();
}

/**
 * @brief MdiArea::useBackgroundTexture
 * @param use
 */
void
MdiArea::useBackgroundTexture(bool use)
{
    useTexture = use;
    forceRepaint();
}

void MdiArea::useBackgroundColor(bool use)
{
    useColor = use;
    forceRepaint();
}

/**
 * @brief MdiArea::setBackgroundLogo
 * @param fileName
 */
void MdiArea::setBackgroundLogo(const QString& fileName)
{
    bgLogo.load(fileName);

    forceRepaint();
}

/**
 * @brief MdiArea::setBackgroundTexture
 * @param fileName
 */
void MdiArea::setBackgroundTexture(const QString& fileName)
{
    bgTexture.load(fileName);

    forceRepaint();
}

/**
 * @brief MdiArea::setBackgroundColor
 * @param color
 */
void MdiArea::setBackgroundColor(const QColor& color)
{
    if(!color.isValid())
        bgColor = background().color();
    else
        bgColor = color;

    forceRepaint();
}

/**
 * @brief MdiArea::mouseDoubleClickEvent
 */
void
MdiArea::mouseDoubleClickEvent(QMouseEvent* /*e*/)
{
    mainWin->openFile();
}

/**
 * @brief MdiArea::paintEvent
 */
void
MdiArea::paintEvent(QPaintEvent* /*e*/)
{
    QWidget* vport = viewport();
    QRect rect = vport->rect();

    QPainter painter(vport);
    painter.setRenderHint(QPainter::SmoothPixmapTransform);

    //Always fill with a solid color first
    if(useColor) painter.fillRect(rect, bgColor);
    else         painter.fillRect(rect, background());

    //Then overlay the texture
    if(useTexture)
    {
        QBrush bgBrush(bgTexture);
        painter.fillRect(rect, bgBrush);
    }

    //Overlay the logo last
    if(useLogo)
    {
        //Center the pixmap
        int dx = (rect.width()-bgLogo.width())/2;
        int dy = (rect.height()-bgLogo.height())/2;
        painter.drawPixmap(dx, dy, bgLogo.width(), bgLogo.height(), bgLogo);
    }
}

/**
 * @brief MdiArea::cascade
 */
void MdiArea::cascade()
{
    cascadeSubWindows();
    zoomExtentsAllSubWindows();
}

/**
 * @brief MdiArea::tile
 */
void MdiArea::tile()
{
    tileSubWindows();
    zoomExtentsAllSubWindows();
}

/**
 * @brief MdiArea::zoomExtentsAllSubWindows
 */
void MdiArea::zoomExtentsAllSubWindows()
{
    foreach(QMdiSubWindow* window, subWindowList())
    {
        MdiWindow* mdiWin = qobject_cast<MdiWindow*>(window);
        if(mdiWin)
        {
            View* v = mdiWin->getView();
            if(v)
            {
                v->recalculateLimits();
                v->zoomExtents();
            }
        }
    }
}

/**
 * @brief MdiArea::forceRepaint
 */
void MdiArea::forceRepaint()
{
    //HACK: Take that QMdiArea!
    QSize hack = size();
    resize(hack + QSize(1,1));
    resize(hack);
}
