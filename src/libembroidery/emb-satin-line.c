#include "emb-satin-line.h"
#include "emb-line.h"
#include "emb-logging.h"
#include "emb-vector.h"
#include <math.h>
#include <stdlib.h>

void embSatinOutline_generateSatinOutline(EmbVector lines[], int numberOfPoints, double thickness, EmbSatinOutline* result)
{
    int i;
    EmbSatinOutline outline;
    double halfThickness = thickness / 2.0;
    int intermediateOutlineCount = 2 * numberOfPoints - 2;
    outline.side1 = (EmbVector*)malloc(sizeof(EmbVector) * intermediateOutlineCount);
    if(!outline.side1) { embLog_error("emb-satin-line.c embSatinOutline_generateSatinOutline(), cannot allocate memory for outline->side1\n"); return; }
    outline.side2 = (EmbVector*)malloc(sizeof(EmbVector) * intermediateOutlineCount);
    if(!outline.side2) { embLog_error("emb-satin-line.c embSatinOutline_generateSatinOutline(), cannot allocate memory for outline->side2\n"); return; }

    for(i = 1; i < numberOfPoints; i++)
    {
        int j = (i - 1) * 2;
        EmbVector v1;
        EmbVector temp;

        embLine_normalVector(lines[i - 1], lines[i], &v1, 1);

        embVector_multiply(v1, halfThickness, &temp);
        embVector_add(temp, lines[i - 1], &outline.side1[j]);
        embVector_add(temp, lines[i], &outline.side1[j + 1]);

        embVector_multiply(v1, -halfThickness, &temp);
        embVector_add(temp, lines[i - 1], &outline.side2[j]);
        embVector_add(temp, lines[i], &outline.side2[j + 1]);
    }

    if(!result) { embLog_error("emb-satin-line.c embSatinOutline_generateSatinOutline(), result argument is null\n"); return; }
    result->side1 = (EmbVector*)malloc(sizeof(EmbVector) * numberOfPoints);
    if(!result->side1) { embLog_error("emb-satin-line.c embSatinOutline_generateSatinOutline(), cannot allocate memory for result->side1\n"); return; }
    result->side2 = (EmbVector*)malloc(sizeof(EmbVector) * numberOfPoints);
    if(!result->side2) { embLog_error("emb-satin-line.c embSatinOutline_generateSatinOutline(), cannot allocate memory for result->side2\n"); return; }

    result->side1[0] = outline.side1[0];
    result->side2[0] = outline.side2[0];

    for(i = 3; i < intermediateOutlineCount; i += 2)
    {
        embLine_intersectionPoint(outline.side1[i - 3], outline.side1[i - 2], outline.side1[i - 1], outline.side1[i], &result->side1[(i - 1) / 2]);
    }

    for(i = 3; i < intermediateOutlineCount; i += 2)
    {
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

    if(!result) { embLog_error("emb-satin-line.c embSatinOutline_renderStitches(), result argument is null\n"); return 0; }

    if(result->length > 0)
    {
        double currTopX = 0;
        double currTopY = 0;
        double currBottomX = 0;
        double currBottomY = 0;

        for(j = 0; j < result->length - 1; j++)
        {
            EmbVector p1 = result->side1[j];
            EmbVector p2 = result->side1[j + 1];
            EmbVector p3 = result->side2[j];
            EmbVector p4 = result->side2[j + 1];

            double topXDiff = p2.X - p1.X;
            double topYDiff = p2.Y - p1.Y;
            double bottomXDiff = p4.X - p3.X;
            double bottomYDiff = p4.Y - p3.Y;

            double midLeftX = (p1.X + p3.X) / 2;
            double midLeftY = (p1.Y + p3.Y) / 2;
            double midRightX = (p2.X + p4.X) / 2;
            double midRightY = (p2.Y + p4.Y) / 2;

            double midXDiff = midLeftX - midRightX;
            double midYDiff = midLeftY - midRightY;
            double midLength = sqrt(midXDiff * midXDiff + midYDiff * midYDiff);

            int numberOfSteps = (int)(midLength * density / 200);
            double topStepX = topXDiff / numberOfSteps;
            double topStepY = topYDiff / numberOfSteps;
            double bottomStepX = bottomXDiff / numberOfSteps;
            double bottomStepY = bottomYDiff / numberOfSteps;
            currTopX = p1.X;
            currTopY = p1.Y;
            currBottomX = p3.X;
            currBottomY = p3.Y;

            for(i = 0; i < numberOfSteps; i++)
            {
                EmbVector temp2;
                temp.X= currTopX;
                temp.Y = currTopY;
                if(stitches)
                {
                    currentStitch = embVectorList_add(currentStitch, temp);
                }
                else
                {
                    stitches = currentStitch = embVectorList_create(temp);
                }
                temp2.X = currBottomX;
                temp2.Y = currBottomY;
                currentStitch = embVectorList_add(currentStitch, temp2);
                currTopX += topStepX;
                currTopY += topStepY;
                currBottomX += bottomStepX;
                currBottomY += bottomStepY;
            }
        }
        temp.X = currTopX;
        temp.Y = currTopY;
        currentStitch = embVectorList_add(currentStitch, temp);
        temp.X = currBottomX;
        temp.Y = currBottomY;
        currentStitch = embVectorList_add(currentStitch, temp);
    }
    return stitches;
}

/* kate: bom off; indent-mode cstyle; indent-width 4; replace-trailing-space-save on; */
