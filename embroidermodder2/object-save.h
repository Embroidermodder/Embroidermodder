#ifndef OBJECT_SAVE_H
#define OBJECT_SAVE_H

#include "../extern/libembroidery/embroidery.h"

#include <QObject>
#include <QPainterPath>

QT_BEGIN_NAMESPACE
class QGraphicsItem;
class QGraphicsScene;
QT_END_NAMESPACE

class SaveObject : public QObject
{
    Q_OBJECT

public:
    SaveObject(QGraphicsScene* theScene, QObject* parent = 0);
    ~SaveObject();

    bool save(const QString &fileName);

    void addArc          (EmbPattern* pattern, QGraphicsItem* item);
    void addBlock        (EmbPattern* pattern, QGraphicsItem* item);
    void addCircle       (EmbPattern* pattern, QGraphicsItem* item);
    void addDimAligned   (EmbPattern* pattern, QGraphicsItem* item);
    void addDimAngular   (EmbPattern* pattern, QGraphicsItem* item);
    void addDimArcLength (EmbPattern* pattern, QGraphicsItem* item);
    void addDimDiameter  (EmbPattern* pattern, QGraphicsItem* item);
    void addDimLeader    (EmbPattern* pattern, QGraphicsItem* item);
    void addDimLinear    (EmbPattern* pattern, QGraphicsItem* item);
    void addDimOrdinate  (EmbPattern* pattern, QGraphicsItem* item);
    void addDimRadius    (EmbPattern* pattern, QGraphicsItem* item);
    void addEllipse      (EmbPattern* pattern, QGraphicsItem* item);
    void addEllipseArc   (EmbPattern* pattern, QGraphicsItem* item);
    void addGrid         (EmbPattern* pattern, QGraphicsItem* item);
    void addHatch        (EmbPattern* pattern, QGraphicsItem* item);
    void addImage        (EmbPattern* pattern, QGraphicsItem* item);
    void addInfiniteLine (EmbPattern* pattern, QGraphicsItem* item);
    void addLine         (EmbPattern* pattern, QGraphicsItem* item);
    void addPath         (EmbPattern* pattern, QGraphicsItem* item);
    void addPoint        (EmbPattern* pattern, QGraphicsItem* item);
    void addPolygon      (EmbPattern* pattern, QGraphicsItem* item);
    void addPolyline     (EmbPattern* pattern, QGraphicsItem* item);
    void addRay          (EmbPattern* pattern, QGraphicsItem* item);
    void addRectangle    (EmbPattern* pattern, QGraphicsItem* item);
    void addSlot         (EmbPattern* pattern, QGraphicsItem* item);
    void addSpline       (EmbPattern* pattern, QGraphicsItem* item);
    void addTextMulti    (EmbPattern* pattern, QGraphicsItem* item);
    void addTextSingle   (EmbPattern* pattern, QGraphicsItem* item);

private:
    QGraphicsScene* gscene;
    int formatType;

    void toPolyline(EmbPattern* pattern, const QPointF& objPos, const QPainterPath& objPath, const QString& layer, const QColor& color, const QString& lineType, const QString& lineWeight);
};

#endif

/* kate: bom off; indent-mode cstyle; indent-width 4; replace-trailing-space-save on; */
