#include "libembroidery-thumbnailer-kde4.h"

#include "emb-reader-writer.h"

extern "C"
{
    KDE_EXPORT ThumbCreator* new_creator()
    {
        return new EmbroideryThumbnailer;
    }
}

EmbroideryThumbnailer::EmbroideryThumbnailer()
{
}

EmbroideryThumbnailer::~EmbroideryThumbnailer()
{
}

const QImage loadThumb(const QString& fileName)
{
    QFile file(fileName);
    if(!file.open(QFile::ReadOnly | QFile::Text))
    {
        //Error opening file
        return QImage();
    }

    //Read
    EmbPattern* p = embPattern_create();
    if(!p) { return QImage(); }
    int readSuccessful = 0;
    QString readError;
    EmbReaderWriter* reader = embReaderWriter_getByFileName(qPrintable(fileName));
    if(!reader)
    {
        readSuccessful = 0;
    }
    else
    {
        readSuccessful = reader->reader(p, qPrintable(fileName));
    }
    free(reader);

    if(readSuccessful)
    {
        QGraphicsScene gscene;
        QPainterPath path;

        if(p->stitchList)
        {
            int previousColor = p->stitchList->stitch.color;

            EmbStitchList* curStitchItem = p->stitchList;
            while(curStitchItem)
            {
                EmbStitch tempStitch = curStitchItem->stitch;
                curStitchItem = curStitchItem->next;
                if((tempStitch.flags & STOP) || (tempStitch.flags & END))
                {
                    if(!path.isEmpty())
                    {
                        EmbColor thisColor = embThreadList_getAt(p->threadList, previousColor).color;
                        QPen loadPen(qRgb(thisColor.r, thisColor.g, thisColor.b));
                        loadPen.setWidthF(0.35);
                        loadPen.setCapStyle(Qt::RoundCap);
                        loadPen.setJoinStyle(Qt::RoundJoin);
                        gscene.addPath(path, loadPen);
                        previousColor = tempStitch.color;
                        path = QPainterPath();
                    }
                    path.moveTo(tempStitch.xx, -tempStitch.yy);
                }
                else if((tempStitch.flags & JUMP) || (tempStitch.flags & TRIM))
                {
                    path.moveTo(tempStitch.xx, -tempStitch.yy);
                }
                else
                {
                    path.lineTo(tempStitch.xx, -tempStitch.yy);
                }
            }
        }

        QImage sceneImg(gscene.width(), gscene.height(), QImage::Format_ARGB32);
        sceneImg.fill(QColor(0,0,0,0));

        QPainter painter(&sceneImg);
        gscene.render(&painter);
        return sceneImg;
    }
    embPattern_free(p);

    return QImage();
}

bool EmbroideryThumbnailer::create(const QString& path, int /*w*/, int /*h*/, QImage& img)
{
    img = loadThumb(path);

    if(img.isNull())
        return false;

    return true;
}

/* kate: bom off; indent-mode cstyle; indent-width 4; replace-trailing-space-save on; */
