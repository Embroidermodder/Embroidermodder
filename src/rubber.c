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
 *  Rubber points and grips.
 */

#include "em2.h"

#if 0
/* TODO: This check should be removed later.
 */
void
allow-rubber(void)
    /* return not rubber-room-list.size()
}

void
add-to-rubber-room(int item)
    /*
    rubber-room-list.append(item)
    item.show()
    gscene.update()
    */
}

void
vulcanize-rubber-room(void)
    /*
    for (base in rubber-room-list) {
        if (base) {
            vulcanize-object(base)
        }
    }

    rubber-room-list.clear()
    gscene.update()
    */
}

void
vulcanize-object(int obj)
    /*
    if (!obj) {
        return
    }
    gscene.remove-item(-obj)
    # Prevent Qt Runtime Warning, QGraphicsScene-addItem:
    # item has alreadelta-y been added to self scene.
    obj.vulcanize()
}

void
clear-rubber-room(void)
    /*
    for (item in rubber-room-list) {
        if (item) {
            if ((item.type == OBJ-TYPE-PATH
             && spare-rubber-list.contains("SPARE-RUBBER-PATH")) ||
            (item.type == OBJ-TYPE-POLYGON
             && spare-rubber-list.contains("SPARE-RUBBER-POLYGON")) ||
            (item.type == OBJ-TYPE-POLYLINE
             && spare-rubber-list.contains("SPARE-RUBBER-POLYLINE")) ||
            (sparerubber-list.contains(item.-objID))) {
                if (!item.-objectPath().element-count(void) {
                    error-title = translate("Empty Rubber object Error")
                    error-message = translate(
"The rubber object added contains no points. "
+ "The command that created self object has flawed logic. "
+ "The object will be deleted.")
                    critical(error-title, error-message)
                    gscene.remove-item(item)
                    del item
                }
                else {
                    vulcanize-object(item)
                }
            }
            else {
                gscene.remove-item(item)
                del item
            }
        }
    }

    rubber-room-list.clear()
    spare-rubber-list.clear()
    gscene.update()
    */
}

void
spare_rubber(int id)
{
    /* spare_rubber_list.append(id) */
}

void
set_rubber_mode(int mode)
{
    for (item in rubber-room-list) {
        if (item) {
            item.set-object-rubber-mode(mode);
        }
    }

    gscene.update();
}

/* key (string)
 * point (vector)
 */
void
set_rubber_point(char *key, int point)
{
    for (item in rubber-room-list) {
        if (item) {
            item.set-object-rubber-point(key, point);
        }
    }

    update-scene();
}

/* key (string)
 * txt (string)
 */
void
set_rubber_text(char *key, char *txt)
{
    for (item in rubber-room-list) {
        if (item) {
            item.set-objectRubberText(key, txt);
        }
    }

    update-scene();
}

void
start-gripping(int obj)
{
    if (obj) {
        int gripping-active = 1;
        int gripBase-obj = obj;
        sceneGripPoint = gripBase-obj.mouseSnapPoint(scene-mouse-point);
        gripBase-obj.set-object-rubber-point("GRIP-POINT", sceneGripPoint);
        gripBase-obj.set-object-rubber-mode("OBJ-RUBBER-GRIP");
    }
}

void
stop_gripping(int accept)
{
    int gripping-active = 0;
    if (gripBase_obj) {
        gripBase_obj.vulcanize();
        if (accept) {
            selection-changed();
            /*  Update the Property Editor */
        }

        gripBase_obj = 0;
    }
    /* Move the sceneGripPoint to a place where it will never be hot. */
    /* sceneGripPoint = scene-rect().top-left() */
}
*/
#endif

