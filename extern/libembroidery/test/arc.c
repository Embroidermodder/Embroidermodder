/*
 * Testing
 */

#include <string.h>
#include <math.h>

#include "../src/embroidery.h"

void
printArcResults(
    EmbReal bulge,
    EmbArc arc,
    EmbVector center,
    EmbReal radius,
    EmbReal diameter,
    EmbReal chord,
    EmbVector chordMid,
    EmbReal sagitta,
    EmbReal apothem,
    EmbReal incAngle,
    char clockwise);

void emb_vector_print(EmbVector v, char *label);
void emb_arc_print(EmbArc a);

/* TODO: */
int
main(void)
{
    return 0;
    EmbGeometry g = emb_arc(1.0, 0.0, 0.0, 0.0, 2.0, 1.0);
    EmbVector center, chordMid;
    EmbReal bulge, radius, diameter, chord, sagitta, apothem, incAngle;
    unsigned char clockwise;

    bulge = -0.414213562373095f;
    center = emb_gget(&g, EMB_CENTER).v;
    chord = emb_gget(&g, EMB_CHORD).r;
    radius = emb_gget(&g, EMB_RADIUS).r;
    diameter = emb_gget(&g, EMB_DIAMETER).r;
    chordMid = emb_gget(&g, EMB_CHORDMID).v;
    sagitta = emb_gget(&g, EMB_SAGITTA).r;
    apothem = emb_gget(&g, EMB_APOTHEM).r;
    incAngle = emb_gget(&g, EMB_INCANGLE).r;
    clockwise = emb_gget(&g, EMB_CLOCKWISE).b;
    /* bulge = emb_arc_bulge(g); */
    printf("Clockwise Test:\n");
    printArcResults(bulge, g.object.arc, center,
                radius, diameter,
                chord, chordMid,
                sagitta,   apothem,
                incAngle,  clockwise);

    bulge  = 2.414213562373095f;
    /* FIXME: midpoints */
    g = emb_arc(4.0, 0.0, 0.0, 0.0, 5.0, 1.0);
    center = emb_gget(&g, EMB_CENTER).v;
    chord = emb_gget(&g, EMB_CHORD).r;
    radius = emb_gget(&g, EMB_RADIUS).r;
    diameter = emb_gget(&g, EMB_DIAMETER).r;
    chordMid = emb_gget(&g, EMB_CHORDMID).v;
    sagitta = emb_gget(&g, EMB_SAGITTA).r;
    apothem = emb_gget(&g, EMB_APOTHEM).r;
    incAngle = emb_gget(&g, EMB_INCANGLE).r;
    clockwise = emb_gget(&g, EMB_CLOCKWISE).b;
    /* bulge = emb_arc_bulge(g); */
    printf("Counter-Clockwise Test:\n");
    printArcResults(bulge, g.object.arc, center,
                radius, diameter, chord,
                chordMid, sagitta,   apothem,
                incAngle, clockwise);

    return 0;
}

/* . */
void
printArcResults(
    EmbReal bulge,
    EmbArc arc,
    EmbVector center,
    EmbReal radius,
    EmbReal diameter,
    EmbReal chord,
    EmbVector chordMid,
    EmbReal sagitta,
    EmbReal apothem,
    EmbReal incAngle,
    char clockwise)
{
    printf("test_arc:");
    emb_arc_print(arc);
    emb_vector_print(center, "center");
    emb_vector_print(chordMid, "chordMid");
    printf(
        "bulge     = %f\n"
        "radius    = %f\n"
        "diameter  = %f\n"
        "chord     = %f\n"
        "sagitta   = %f\n"
        "apothem   = %f\n"
        "incAngle  = %f\n"
        "clockwise = %d\n"
        "\n",
        bulge,
        radius,
        diameter,
        chord,
        sagitta,
        apothem,
        incAngle,
        clockwise);
}

