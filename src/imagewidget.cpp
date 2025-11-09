/*!
 * \file imagewidget.cpp
 * \brief Widget to embed images for displaying in dialogs.
 */

#include "embroidermodder.h"

ImageWidget::ImageWidget(const QString &filename, QWidget* parent) : QWidget(parent)
{
    debug("ImageWidget Constructor");

    img.load(filename);

    setMinimumWidth(img.width());
    setMinimumHeight(img.height());
    setMaximumWidth(img.width());
    setMaximumHeight(img.height());

    this->show();
}

bool ImageWidget::load(const QString &fileName)
{
    img.load(fileName);
    return true;
}

bool ImageWidget::save(const QString &fileName)
{
    img.save(fileName, "PNG");
    return true;
}

ImageWidget::~ImageWidget()
{
    debug("ImageWidget Destructor");
}

void ImageWidget::paintEvent(QPaintEvent*)
{
    QPainter painter(this);
    painter.setViewport(0, 0, width(), height());
    painter.setWindow(0, 0, width(), height());
    painter.drawImage(0, 0, img);
}

