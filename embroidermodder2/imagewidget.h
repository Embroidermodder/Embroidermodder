#ifndef IMAGEWIDGET_H
#define IMAGEWIDGET_H

#include <QPainter>
#include <QImage>
#include <QWidget>

QT_BEGIN_NAMESPACE
class QPainter;
class QImage;
QT_END_NAMESPACE

class ImageWidget : public QWidget
{
    Q_OBJECT

public:
    ImageWidget(const QString &filename, QWidget* parent = 0);
    ~ImageWidget();

    bool load(const QString &fileName);
    bool save(const QString &fileName);

protected:
    void paintEvent(QPaintEvent* event);

private:
    QImage img;
};

#endif
