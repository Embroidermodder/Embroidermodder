#include "embroidermodder.h"
#include <QLabel>

StatusBar::StatusBar(MainWindow* mw, QWidget *parent) : QStatusBar(parent)
{
    this->setObjectName("StatusBar");

    statusBarSnapButton   = new StatusBarButton("SNAP", mw, this, this);
    statusBarGridButton   = new StatusBarButton("GRID", mw, this, this);
    statusBarRulerButton  = new StatusBarButton("RULER", mw, this, this);
    statusBarOrthoButton  = new StatusBarButton("ORTHO", mw, this, this);
    statusBarPolarButton  = new StatusBarButton("POLAR", mw, this, this);
    statusBarQSnapButton  = new StatusBarButton("QSNAP", mw, this, this);
    statusBarQTrackButton = new StatusBarButton("QTRACK", mw, this, this);
    statusBarLwtButton    = new StatusBarButton("LWT", mw, this, this);
    statusBarMouseCoord   = new QLabel(this);

    statusBarMouseCoord->setMinimumWidth(300); // Must fit this text always
    statusBarMouseCoord->setMaximumWidth(300); // "+1.2345E+99, +1.2345E+99, +1.2345E+99"

    this->addWidget(statusBarMouseCoord);
    this->addWidget(statusBarSnapButton);
    this->addWidget(statusBarGridButton);
    this->addWidget(statusBarRulerButton);
    this->addWidget(statusBarOrthoButton);
    this->addWidget(statusBarPolarButton);
    this->addWidget(statusBarQSnapButton);
    this->addWidget(statusBarQTrackButton);
    this->addWidget(statusBarLwtButton);
}

void StatusBar::setMouseCoord(qreal x, qreal y)
{
    //TODO: set format from settings (Architectural, Decimal, Engineering, Fractional, Scientific)

    //Decimal
    statusBarMouseCoord->setText(QString().setNum(x, 'F', 4) + ", " + QString().setNum(y, 'F', 4)); //TODO: use precision from unit settings

    //Scientific
    //statusBarMouseCoord->setText(QString().setNum(x, 'E', 4) + ", " + QString().setNum(y, 'E', 4)); //TODO: use precision from unit settings
}

/* kate: bom off; indent-mode cstyle; indent-width 4; replace-trailing-space-save on; */
