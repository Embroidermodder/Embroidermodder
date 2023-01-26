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

#if 0
QPainterPath
add_to_path(QPainterPath path, char type, EmbVector offset, float *data, float scale)
{
    if (type == 'm') {
        path.moveTo(offset.x+scale*data[0], offset.y-scale*data[1]);
    }
    if (type == 'l') {
        path.lineTo(offset.x+scale*data[0], offset.y-scale*data[1]);
    }
    if (type == 'e') {
        path.addEllipse(EmbVector(offset.x+scale*data[0], offset.y-scale*data[1]),
            data[2]*scale, data[3]*scale);
    }
    if (type == 'a') {
        path.arcTo(offset.x+scale*data[0], offset.y-scale*data[1],
            data[2]*scale, data[3]*scale, data[4], data[5]);
    }
    return path;
}

QPainterPath
load_to_qpath(EmbVector offset, std::string str, float scale)
{
    QPainterPath path;

    char type = '0';
    int dot = 0;
    int index = 0;
    float position[6] = {0.0, 0.0, 0.0, 0.0, 0.0, 0.0};
    int first = 1;
    float sign = 1.0;
    for (size_t i=0; i<str.size(); i++) {
        char c = str[i];
        if ((c >= 'a') && (c <= 'z')) {
            dot = 0;
            index = 0;
            if (!first) {
                position[index] *= sign;
                path = add_to_path(path, type, offset, position, scale);
            }
            first = 0;
            type = c;
            sign = 1.0;
            for (int j=0; j<6; j++) {
                position[j] = 0.0;
            }
        }
        if ((c >= '0') && (c <= '9')) {
            int x = (int)(c - '0');
            if (dot) {
                position[index] = position[index] + pow(0.1, dot)*x;
                dot++;
            }
            else {
                position[index] = 10.0 * position[index] + 1.0*x;
            }
        }
        if (c == '-') {
            sign = -1.0;
        }
        if (c == '.') {
            dot = 1;
        }
        if (c == ',') {
            position[index] *= sign;
            index++;
            sign = 1.0;
            dot = 0;
        }
    }

    position[index] *= sign;
    path = add_to_path(path, type, offset, position, scale);

    return path;
}

void
createRulerTextPath(float x, float y, std::string str, float height)
{
    imdraw->AddText(ImVec2(x, y), 0xFFFFFFFF, str);
}
#endif
