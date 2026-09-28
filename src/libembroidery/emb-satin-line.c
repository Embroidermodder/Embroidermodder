#include <math.h>
#include <stdlib.h>

#include "embroidery.h"

void embSatinOutline_generate(EmbVector lines[], int numberOfPoints, double thickness, EmbSatinOutline* result)
{
    int i;
    EmbSatinOutline outline;
    double halfThickness = thickness / 2.0;
    int intermediateOutlineCount = 2 * numberOfPoints - 2;
    outline.side1 = (EmbVector*)malloc(sizeof(EmbVector) * intermediateOutlineCount);
    if (!outline.side1) { embLog_error("emb-satin-line.c embSatinOutline_generateSatinOutline(), cannot allocate memory for outline->side1\n"); return; }
    outline.side2 = (EmbVector*)malloc(sizeof(EmbVector) * intermediateOutlineCount);
    if (!outline.side2) { embLog_error("emb-satin-line.c embSatinOutline_generateSatinOutline(), cannot allocate memory for outline->side2\n"); return; }

    for (i = 1; i < numberOfPoints; i++) {
        int j = (i - 1) * 2;
        EmbVector v1;
        EmbVector temp;

        v1 = embLine_normal(lines[i - 1], lines[i], 1);

        temp = embVector_scale(v1, halfThickness);
        outline.side1[j] = embVector_add(temp, lines[i - 1]);
        outline.side1[j + 1] = embVector_add(temp, lines[i]);

        temp = embVector_scale(v1, -halfThickness);
        outline.side2[j] = embVector_add(temp, lines[i - 1]);
        outline.side2[j + 1] = embVector_add(temp, lines[i]);
    }

    if (!result) {
        embLog_error("emb-satin-line.c embSatinOutline_generateSatinOutline(), result argument is null\n");
        return;
    }
    result->side1 = (EmbVector*)malloc(sizeof(EmbVector) * numberOfPoints);
    if (!result->side1) {
        embLog_error("emb-satin-line.c embSatinOutline_generateSatinOutline(), cannot allocate memory for result->side1\n");
        return;
    }
    result->side2 = (EmbVector*)malloc(sizeof(EmbVector) * numberOfPoints);
    if (!result->side2) {
        embLog_error("emb-satin-line.c embSatinOutline_generateSatinOutline(), cannot allocate memory for result->side2\n");
        return;
    }

    result->side1[0] = outline.side1[0];
    result->side2[0] = outline.side2[0];

    for (i = 3; i < intermediateOutlineCount; i += 2) {
        embLine_intersectionPoint(outline.side1[i - 3], outline.side1[i - 2], outline.side1[i - 1], outline.side1[i], &result->side1[(i - 1) / 2]);
    }

    for (i = 3; i < intermediateOutlineCount; i += 2) {
        embLine_intersectionPoint(outline.side2[i - 3], outline.side2[i - 2], outline.side2[i - 1], outline.side2[i], &result->side2[(i - 1) / 2]);
    }

    result->side1[numberOfPoints - 1] = outline.side1[2 * numberOfPoints - 3];
    result->side2[numberOfPoints - 1] = outline.side2[2 * numberOfPoints - 3];
    result->length = numberOfPoints;
}

EmbVectorList* embSatinOutline_renderStitches(EmbSatinOutline* result, double density)
{
    int i, j;
    EmbVectorList* stitches = 0;
    EmbVectorList* currentStitch = 0;
    EmbVector temp;

    if (!result) { embLog_error("emb-satin-line.c embSatinOutline_renderStitches(), result argument is null\n"); return 0; }

    if (result->length <= 0) {
        return stitches;
    }

    EmbVector currTop = embVector(0.0, 0.0);
    EmbVector currBottom = embVector(0.0, 0.0);

    for (j = 0; j < result->length - 1; j++) {
        EmbVector p1 = result->side1[j];
        EmbVector p2 = result->side1[j + 1];
        EmbVector p3 = result->side2[j];
        EmbVector p4 = result->side2[j + 1];

        EmbVector topDiff = embVector_subtract(p2, p1);
        EmbVector bottomDiff = embVector_subtract(p4, p3);

        EmbVector midLeft = embVector_average(p1, p3);
        EmbVector midRight = embVector_average(p2, p4);

        EmbVector midDiff = embVector_subtract(midLeft, midRight);
        double midLength = embVector_getLength(midDiff);

        int numberOfSteps = (int)(midLength * density / 200);
        EmbVector topStep = embVector_scale(topDiff, 1.0 / numberOfSteps);
        EmbVector bottomStep = embVector_scale(bottomDiff, 1.0 / numberOfSteps);
        EmbVector currTop = p1;
        EmbVector currBottom = p3;

        for (i = 0; i < numberOfSteps; i++) {
            if (stitches) {
                currentStitch = embVectorList_add(currentStitch, temp);
            }
            else {
                stitches = currentStitch = embVectorList_create(currTop);
            }
            currentStitch = embVectorList_add(currentStitch, currBottom);
            currTop = embVector_add(currTop, topStep);
            currBottom = embVector_add(currBottom, bottomStep);
        }
    }
    currentStitch = embVectorList_add(currentStitch, currTop);
    currentStitch = embVectorList_add(currentStitch, currBottom);

    return stitches;
}

