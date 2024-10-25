#ifndef LIBEMBROIDERY_THUMBNAILER_KDE4_H
#define LIBEMBROIDERY_THUMBNAILER_KDE4_H

#include <kio/thumbcreator.h>

class EmbroideryThumbnailer : public ThumbCreator
{
public:
    EmbroideryThumbnailer();
    virtual ~EmbroideryThumbnailer();
    virtual bool create(const QString& path, int w, int h, QImage& img);
};

#endif

/* kate: bom off; indent-mode cstyle; indent-width 4; replace-trailing-space-save on; */
