/*
 *  Embroidermodder 2.
 *
 *  ------------------------------------------------------------
 *
 *  Copyright 2013-2022 The Embroidermodder Team
 *  Embroidermodder 2 is Open Source Software.
 *  See LICENSE for licensing terms.
 *
 *  ------------------------------------------------------------
 *
 *  Use Python's PEP7 style guide.
 *      https://peps.python.org/pep-0007/
 */

#include "embroidermodder.h"

/* Create the image widget for filename. */
ImageWidget::ImageWidget(QString filename, QWidget* parent) : QWidget(parent)
{
    debug_message("ImageWidget Constructor");

    img.load(filename);

    setMinimumWidth(img.width());
    setMinimumHeight(img.height());
    setMaximumWidth(img.width());
    setMaximumHeight(img.height());

    this->show();
}

/* Load image to widget. */
bool
ImageWidget::load(QString fileName)
{
    img.load(fileName);
    return true;
}

/* Save image from widget. */
bool
ImageWidget::save(QString fileName)
{
    img.save(fileName, "PNG");
    return true;
}

/* Destroy image widget. */
ImageWidget::~ImageWidget()
{
    debug_message("ImageWidget Destructor");
}

/* Paint the widget. */
void
ImageWidget::paintEvent(QPaintEvent*)
{
    QPainter painter(this);
    painter.setViewport(0, 0, width(), height());
    painter.setWindow(0, 0, width(), height());
    painter.drawImage(0, 0, img);
}
