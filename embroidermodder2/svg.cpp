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

std::unordered_map<std::string, std::string> path_table = {
    {"0", "m0.00,0.75l0.00,0.25a0.00,0.50,0.50,0.50,180.00,180.00l0.50,0.75a0.00,1.00,0.50,0.50,0.00,180.00"},
    {"1", "m0.05,0.00l0.45,0.00m0.00,0.75l0.25,1.00l0.25,0.00"},
    {"2", "m0.00,0.75a0.00,1.00,0.50,0.50,180.00,-216.87l0.00,0.00l0.50,0.00"},
    {"3", "a0.00,0.50,0.50,0.50,195.00a0.00,0.50,0.50,0.50,195.00,255.00a0.00,1.00,0.50,0.50,270.00,255.00"},
    {"4", "m0.50,0.00l0.50,1.00l0.00,0.50l0.50,0.50"},
    {"5", "m0.50,1.00l0.00,1.00l0.00,0.50l0.25,0.50a0.00,0.50,0.50,0.50,90.00,-180.00l0.00,0.00"},
    {"6", "e0.25,0.25,0.25,0.25m0.00,0.25l0.00,0.75a0.00,1.00,0.50,0.50,180.00,-140.00"},
    {"7", "m0.00,1.00l0.50,1.00l0.25,0.25l0.25,0.00"},
    {"8", "e0.25,0.25,0.25,0.25e0.25,0.75,0.25,0.25"},
    {"9", "e0.25,0.75,0.25,0.25m0.50,0.75l0.50,0.25a0.00,0.50,0.50,0.50,0.00,-140.00"},
    {"'", "m0.25,1.00l0.25,0.75"},
    {"\"", "m0.10,1.00l0.10,0.75m0.40,1.00l0.40,0.75"},
    {"-", "m0.00,0.50l0.50,0.50"}
};

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
        path.addEllipse(QPointF(offset.x+scale*data[0], offset.y-scale*data[1]),
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

QPainterPath createRulerTextPath(float x, float y, QString str, float height)
{
    QPainterPath path;

    qreal xScale = height;
    qreal yScale = height;

    EmbVector offset;
    offset.x = x;
    offset.y = y;

    for (int i = 0; i < str.length(); ++i) {
        std::string s(1, str[i].toLatin1());
        if (s == "1") {
            std::unordered_map<std::string,std::string>::const_iterator it = path_table.find(s);
            path = load_to_qpath(offset, it->second, height);
        }
        else if (s == "2") {
            path.moveTo(x+0.00*xScale, y-0.75*yScale);
            path.arcTo(x+0.00*xScale, y-1.00*yScale, 0.50*xScale, 0.50*yScale, 180.00, -216.87);
            path.lineTo(x+0.00*xScale, y-0.00*yScale);
            path.lineTo(x+0.50*xScale, y-0.00*yScale);
        }
        else if (s == "3") {
            path.arcMoveTo(x+0.00*xScale, y-0.50*yScale, 0.50*xScale, 0.50*yScale, 195.00);
            path.arcTo(x+0.00*xScale, y-0.50*yScale, 0.50*xScale, 0.50*yScale, 195.00, 255.00);
            path.arcTo(x+0.00*xScale, y-1.00*yScale, 0.50*xScale, 0.50*yScale, 270.00, 255.00);
        }
        else if (s == "4") {
            path.moveTo(x+0.50*xScale, y-0.00*yScale);
            path.lineTo(x+0.50*xScale, y-1.00*yScale);
            path.lineTo(x+0.00*xScale, y-0.50*yScale);
            path.lineTo(x+0.50*xScale, y-0.50*yScale);
        }
        else if (s == "5") {
            path.moveTo(x+0.50*xScale, y-1.00*yScale);
            path.lineTo(x+0.00*xScale, y-1.00*yScale);
            path.lineTo(x+0.00*xScale, y-0.50*yScale);
            path.lineTo(x+0.25*xScale, y-0.50*yScale);
            path.arcTo(x+0.00*xScale, y-0.50*yScale, 0.50*xScale, 0.50*yScale, 90.00, -180.00);
            path.lineTo(x+0.00*xScale, y-0.00*yScale);
        }
        else if (s == "6") {
            path.addEllipse(QPointF(x+0.25*xScale, y-0.25*yScale), 0.25*xScale, 0.25*yScale);
            path.moveTo(x+0.00*xScale, y-0.25*yScale);
            path.lineTo(x+0.00*xScale, y-0.75*yScale);
            path.arcTo(x+0.00*xScale, y-1.00*yScale, 0.50*xScale, 0.50*yScale, 180.00, -140.00);
        }
        else if (s == "7") {
            path.moveTo(x+0.00*xScale, y-1.00*yScale);
            path.lineTo(x+0.50*xScale, y-1.00*yScale);
            path.lineTo(x+0.25*xScale, y-0.25*yScale);
            path.lineTo(x+0.25*xScale, y-0.00*yScale);
        }
        else if (s == "8") {
            path.addEllipse(QPointF(x+0.25*xScale, y-0.25*yScale), 0.25*xScale, 0.25*yScale);
            path.addEllipse(QPointF(x+0.25*xScale, y-0.75*yScale), 0.25*xScale, 0.25*yScale);
        }
        else if (s == "9") {
            path.addEllipse(QPointF(x+0.25*xScale, y-0.75*yScale), 0.25*xScale, 0.25*yScale);
            path.moveTo(x+0.50*xScale, y-0.75*yScale);
            path.lineTo(x+0.50*xScale, y-0.25*yScale);
            path.arcTo(x+0.00*xScale, y-0.50*yScale, 0.50*xScale, 0.50*yScale, 0.00, -140.00);
        }
        else if (s == "0") {
            //path.addEllipse(QPointF(x+0.25*xScale, y-0.50*yScale), 0.25*xScale, 0.50*yScale);

            path.moveTo(x+0.00*xScale, y-0.75*yScale);
            path.lineTo(x+0.00*xScale, y-0.25*yScale);
            path.arcTo(x+0.00*xScale, y-0.50*yScale, 0.50*xScale, 0.50*yScale, 180.00, 180.00);
            path.lineTo(x+0.50*xScale, y-0.75*yScale);
            path.arcTo(x+0.00*xScale, y-1.00*yScale, 0.50*xScale, 0.50*yScale,   0.00, 180.00);
        }
        else if (s == "-") {
            path.moveTo(x+0.00*xScale, y-0.50*yScale);
            path.lineTo(x+0.50*xScale, y-0.50*yScale);
        }
        else if (s == "'") {
            path.moveTo(x+0.25*xScale, y-1.00*yScale);
            path.lineTo(x+0.25*xScale, y-0.75*yScale);
        }
        else if (s == "\"") {
            path.moveTo(x+0.10*xScale, y-1.00*yScale);
            path.lineTo(x+0.10*xScale, y-0.75*yScale);
            path.moveTo(x+0.40*xScale, y-1.00*yScale);
            path.lineTo(x+0.40*xScale, y-0.75*yScale);
        }

        x += 0.75*xScale;
        offset.x = x;
    }

    return path;
}
