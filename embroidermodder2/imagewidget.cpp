/**
 * \file imagewidget.cpp
 */

#include <QDebug>
#include "imagewidget.h"

/**
 * @brief ImageWidget::ImageWidget
 * @param filename
 * @param parent
 */
ImageWidget::ImageWidget(const QString &filename, QWidget* parent) : QWidget(parent)
{
    qDebug("ImageWidget Constructor");

    img.load(filename);

    setMinimumWidth(img.width());
    setMinimumHeight(img.height());
    setMaximumWidth(img.width());
    setMaximumHeight(img.height());

    this->show();
}

/**
 * @brief ImageWidget::load
 * @param fileName
 * @return
 */
bool
ImageWidget::load(const QString &fileName)
{
    img.load(fileName);
    return true;
}

/**
 * @brief ImageWidget::save
 * @param fileName
 * @return
 */
bool
ImageWidget::save(const QString &fileName)
{
    img.save(fileName, "PNG");
    return true;
}

/**
 * @brief ImageWidget::~ImageWidget
 */
ImageWidget::~ImageWidget()
{
    qDebug("ImageWidget Destructor");
}

/**
 * @brief ImageWidget::paintEvent
 */
void
ImageWidget::paintEvent(QPaintEvent*)
{
    QPainter painter(this);
    painter.setViewport(0, 0, width(), height());
    painter.setWindow(0, 0, width(), height());
    painter.drawImage(0, 0, img);
}