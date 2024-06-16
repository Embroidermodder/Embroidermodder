/*
 * Embroidermodder 2.
 *
 * Copyright 2011-2024 The Embroidermodder Team
 * Embroidermodder 2 is Open Source Software, see LICENSE.md for licensing terms.
 * Visit https://www.libembroidery.org/refman for advice on altering this file,
 * or read the markdown version in embroidermodder2/docs/refman.
 *
 * Save
 */

#include "embroidermodder.h"

#include <QGraphicsScene>
#include <QGraphicsItem>

/* TODO: Before saving to a stitch only format, Embroidermodder needs
 *       to calculate the optimal path to minimize jump stitches. Also
 *       based upon which layer needs to be stitched first,
 *       the path to the next object needs to be hidden beneath fills
 *       that will come later. When finding the optimal path, we need
 *       to take into account the color of the thread, as we do not want
 *       to try to hide dark colored stitches beneath light colored fills.
 */
bool pattern_save(EmbPattern *pattern, const char *fileName)
{
    qDebug("SaveObject save(%s)", fileName);

    bool writeSuccessful = false;
    int i;

    int formatType = emb_identify_format(fileName);
    if (formatType <= 0) { /* EMBFORMAT_UNSUPPORTED */
        return false;
    }

    if (!pattern) {
        qDebug("The EmbPattern has not been allocated.");
        return false;
    }

    // TODO: handle EMBFORMAT_STCHANDOBJ also
    if (formatType == EMBFORMAT_STITCHONLY) {
        // emb_pattern_movePolylinesToStitchList(pattern);
        // TODO: handle all objects like this
    }

    writeSuccessful = emb_pattern_write(pattern, fileName, formatType);
    if (!writeSuccessful) {
        qDebug("Writing file %s was unsuccessful", fileName);
    }

    //TODO: check the embLog for errors and if any exist, report them.

    emb_pattern_free(pattern);

    return writeSuccessful;
}

/*
void SaveObject::addTextSingle(EmbPattern* pattern, QGraphicsItem* item)
{
    //TODO: saving polygons, polylines and paths must be stable before we go here.

    //TODO: This needs to work like a path, not a polyline. Improve this
    TextSingleObject* obj = static_cast<TextSingleObject*>(item);
    if (obj) {
        if (formatType == EMBFORMAT_STITCHONLY) {
            QList<QPainterPath> pathList = obj->objectSavePathList();
            foreach(QPainterPath path, pathList) {
                toPolyline(pattern, obj->objectPos(), path.simplified(), "0", obj->objectColor(), "CONTINUOUS", "BYLAYER"); //TODO: proper layer/lineType/lineWeight
                // TODO: Improve precision, replace simplified
            }
        }
    }
}
*/
