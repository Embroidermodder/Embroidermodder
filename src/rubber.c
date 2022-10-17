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

/* TODO: This check should be removed later.
 */
void
allow_rubber(void)
{
    /* return not rubber-room-list.size(); */
}

void
add_to_rubber_room(int item)
{
    printf("%d\n", item);
    /*
    rubber-room-list.append(item);
    item.show();
    gscene.update();
    */
}

void
vulcanize_rubber_room(void)
{
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
vulcanize_object(int obj)
{
    if (!obj) {
        return;
    }
    /*
    gscene.remove-item(-obj)
    # Prevent Qt Runtime Warning, QGraphicsScene-addItem:
    # item has alreadelta-y been added to self scene.
    obj.vulcanize()
    */
}

void
clear_rubber_room(void)
{
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
    printf("%d", id);
    /* spare_rubber_list.append(id) */
}

/* .
 */
void
set_rubber_mode(int mode)
{
    printf("%d", mode);
    /*
    for (item in rubber-room-list) {
        if (item) {
            item.set-object-rubber-mode(mode);
        }
    }
    */

    scene_update();
}

/* key (string)
 * point (vector)
 */
void
set_rubber_point(char *key, int point)
{
    printf("%s %d", key, point);
    /*
    for (item in rubber-room-list) {
        if (item) {
            item.set-object-rubber-point(key, point);
        }
    }
    */

    scene_update();
}

/* key (string)
 * txt (string)
 */
void
set_rubber_text(char *key, char *txt)
{
    printf("%s %s\n", key, txt);
    /*
    for (item in rubber-room-list) {
        if (item) {
            item.set-objectRubberText(key, txt);
        }
    }
    */

    scene_update();
}

void
start_gripping(int obj)
{
    if (obj) {
        /*
        int gripping-active = 1;
        int gripBase-obj = obj;
        sceneGripPoint = gripBase-obj.mouseSnapPoint(scene-mouse-point);
        gripBase-obj.set-object-rubber-point("GRIP-POINT", sceneGripPoint);
        gripBase-obj.set-object-rubber-mode("OBJ-RUBBER-GRIP");
        */
    }
}

void
stop_gripping(int accept)
{
    printf("%d", accept);
    /*
    int gripping-active = 0;
    if (gripBase_obj) {
        gripBase_obj.vulcanize();
        if (accept) {
            selection-changed();
            */
            /*  Update the Property Editor */
            /*
        }

        gripBase_obj = 0;
    }
    */
    /* Move the sceneGripPoint to a place where it will never be hot. */
    /* sceneGripPoint = scene-rect().top-left() */
}

