/*
 * Libembroidery 1.0.0-alpha
 * https://www.libembroidery.org
 *
 * A library for reading, writing, altering and otherwise
 * processing machine embroidery files and designs.
 *
 * Also, the core library supporting the Embroidermodder Project's
 * family of machine embroidery interfaces.
 *
 * -----------------------------------------------------------------------------
 *
 * Copyright 2018-2024 The Embroidermodder Team
 * Licensed under the terms of the zlib license.
 *
 * -----------------------------------------------------------------------------
 *
 * Only uses source from this directory or standard C libraries,
 * not including POSIX headers like unistd since this library
 * needs to support non-POSIX systems like Windows.
 *
 * -----------------------------------------------------------------------------
 *
 * Testing
 */

#include <math.h>

#include "embroidery.h"
#include "internal.h"

/*
 * Format: the first token is the function call, the next n tokens are the
 * number required by that function then the remaining tokens are the expected
 * output up to some stated tolerence.
 */
const char *tests[] = {
    "ctangent 0.001 0.0 0.0 3.0 4.0 0.0 2.25 1.9843 2.25 -1.9843",
    "extension example.zsk .zsk",
    "description example.zsk \"ZSK USA Embroidery Format\"",
    "reader example.zsk U"
};

int test_vadd(EmbVector v1, EmbVector v2, EmbVector result, EmbReal tolerence);
int test_vsubtract(EmbVector v1, EmbVector v2, EmbVector result,
    EmbReal tolerence);
int test_ctangents(EmbGeometry c, EmbVector p, EmbVector s0, EmbVector s1,
    EmbReal tolerence);

/* Currently just crash testing. */
int
test_convert(int test_case, int from, int to)
{
    EmbString fname_from;
    EmbString fname_to;
    sprintf(fname_from, "test%02d%s", test_case, formatTable[from].extension);
    sprintf(fname_to, "test%02d_out%s", test_case, formatTable[to].extension);
    printf("Converting test case %d: %s to %s.\n",
        test_case, fname_from, fname_to);
    if (create_test_file(test_case, from)) {
        return 1;
    }
    convert(fname_from, fname_to);
    return 0;
}

/* . */
int
testMain(int test_index)
{
    /* const char *test_str = tests[test_index]; */
    switch (test_index) {
    case 0: {
        EmbReal epsilon = 0.001f;
        EmbGeometry c1 = emb_circle(0.0f, 0.0f, 3.0f);
        EmbVector p = emb_vector(4.0f, 0.0f);
        /* Solution */
        EmbVector s0 = emb_vector(2.2500f, 1.9843f);
        EmbVector s1 = emb_vector(2.2500f, -1.9843f);
        return test_ctangents(c1, p, s0, s1, epsilon);
    }
    case 1: {
        EmbReal epsilon = 0.001f;
        EmbGeometry c2 = emb_circle(20.1762f, 10.7170f, 6.8221f);
        EmbVector p = emb_vector(24.3411f, 18.2980f);
        /* Solution */
        EmbVector s0 = emb_vector(19.0911f, 17.4522f);
        EmbVector s1 = emb_vector(26.4428f, 13.4133f);
        return test_ctangents(c2, p, s0, s1, epsilon);
    }
    case 2: {
        unsigned int tColor = 0xFF0d6b2f;
        EmbColor c;
        c.r = 0x0D;
        c.g = 0x6B;
        c.b = 0x2F;
        int tBrand = EMB_BRAND_SVG;
        int tNum = threadColorNum(tColor, tBrand);
        char tName[50];
        string_copy(tName, threadColorName(tColor, tBrand));

        printf("Color : 0x%X\n"
           "Brand : %d\n"
           "Num   : %d\n"
           "Name  : %s\n\n",
            tColor,
            tBrand,
            tNum, /* Solution: 29 */
            tName); /* Solution: Dark Olive Green */
        return 0;
    }
    case 3: {
        const char*  tName = "example.zsk";
        int format = emb_identify_format(tName);

        printf("Filename   : %s\n"
           "Extension  : %s\n"
           "Description: %s\n"
           "Reader     : %c\n"
           "Writer     : %c\n"
           "Type       : %d\n\n",
            tName,
            formatTable[format].extension,
            formatTable[format].description,
            formatTable[format].reader_state,
            formatTable[format].writer_state,
            formatTable[format].type);

        if (!string_equals(formatTable[format].extension, ".zsk")) {
            puts("In test 3 the extension lookup failed.");
   			return 20;
		}
        if (!string_equals(formatTable[format].description, "ZSK USA Embroidery Format")) {
            puts("In test 3 the description lookup failed.");
            return 21;
        }
        if (formatTable[format].reader_state != 'U') {
            puts("In test 3 the reader_state lookup failed.");
			return 22;
		}
        if (formatTable[format].writer_state != ' ') {
            puts("In test 3 the write_state lookup failed.");
			return 23;
		}
        if (formatTable[format].type != 1) {
            puts("In test 3 the type lookup failed.");
			return 24;
		}
        return 0;
    }
    case 4: {
        EmbGeometry g = emb_arc(1.0, 0.0, 0.0, 0.0, 2.0, 1.0);
        EmbVector center, chordMid;
        EmbReal bulge, radius, diameter, chord, sagitta, apothem, incAngle;
        char clockwise;

        bulge = -0.414213562373095f;
        center = emb_arc_center(g);
        chord = emb_chord(&g);
        radius = emb_arc_radius(g);
        diameter = emb_arc_diameter(g);
        chordMid = emb_arc_chordMid(g);
        sagitta = emb_arc_sagitta(g);
        apothem = emb_arc_apothem(g);
        incAngle = emb_arc_incAngle(g);
        clockwise = emb_arc_clockwise(g);
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
        center = emb_arc_center(g);
        chord = emb_chord(&g);
        radius = emb_arc_radius(g);
        diameter = emb_arc_diameter(g);
        chordMid = emb_arc_chordMid(g);
        sagitta = emb_arc_sagitta(g);
        apothem = emb_arc_apothem(g);
        incAngle = emb_arc_incAngle(g);
        clockwise = emb_arc_clockwise(g);
        /* bulge = emb_arc_bulge(g); */
        printf("Counter-Clockwise Test:\n");
        printArcResults(bulge, g.object.arc, center,
                    radius, diameter, chord,
                    chordMid, sagitta,   apothem,
                    incAngle, clockwise);

        return 0;
    }
    case 5: {
        return create_test_file(0, EMB_FORMAT_CSV);
    }
    case 6: {
        return create_test_file(1, EMB_FORMAT_CSV);
    }
    case 7: {
        return create_test_file(2, EMB_FORMAT_CSV);
    }
    case 8: {
        return test_convert(0, EMB_FORMAT_CSV, EMB_FORMAT_SVG);
    }
    case 9: {
        return test_convert(0, EMB_FORMAT_CSV, EMB_FORMAT_DST);
    }
    case 10: {
        return test_convert(1, EMB_FORMAT_CSV, EMB_FORMAT_SVG);
    }
    case 11: {
        return test_convert(1, EMB_FORMAT_CSV, EMB_FORMAT_DST);
    }
    case 12: {
        return test_convert(2, EMB_FORMAT_CSV, EMB_FORMAT_SVG);
    }
    case 13: {
        return test_convert(2, EMB_FORMAT_CSV, EMB_FORMAT_DST);
    }
    case 14: {
        EmbPattern *pattern = emb_pattern_create();
        int hilbertCurveResult = hilbert_curve(pattern, 3);
        int renderResult = emb_pattern_render(pattern, "hilbert_level_3.png");
        int simulateResult = emb_pattern_simulate(pattern, "hilbert_level_3.avi");
        emb_pattern_free(pattern);
        return hilbertCurveResult;
    }
    default: break;
    }
    return 10;
}

/* . */
int
test_vadd(EmbVector v1, EmbVector v2, EmbVector result, EmbReal tolerence)
{
    EmbVector testResult = emb_vector_add(v1, v2);
    double xerror = fabs(testResult.x - result.x);
    double yerror = fabs(testResult.y - result.y);
    printf("errors: %f %f\n", xerror, yerror);
    if (tolerence < xerror) {
        printf("Error calculating vector sum.\n");
        return 1;
    }
    if (tolerence < yerror) {
        printf("Error calculating vector sum.\n");
        return 1;
    }
    return 0;
}

/* . */
int
test_vsubtract(EmbVector v1, EmbVector v2, EmbVector result, EmbReal tolerence)
{
    EmbVector testResult = emb_vector_subtract(v1, v2);
    double xerror = fabs(testResult.x - result.x);
    double yerror = fabs(testResult.y - result.y);
    printf("errors: %f %f\n", xerror, yerror);
    if (tolerence < xerror) {
        printf("Error calculating vector sum.\n");
        return 1;
    }
    if (tolerence < yerror) {
        printf("Error calculating vector sum.\n");
        return 1;
    }
    return 0;
}

/* . */
int
test_ctangents(EmbGeometry c, EmbVector p, EmbVector s0, EmbVector s1,
    EmbReal tolerence)
{
    EmbVector p0, p1;
    if (!getCircleTangentPoints(c.object.circle, p, &p0, &p1)) {
        printf("Error calculating tangent points.\n");
        return 1;
    }

    printf("Circle : cr=%f, cx=%f, cy=%f\n"
       "Point  : px=%f, py=%f\n"
       "Tangent: tx0=%f, ty0=%f\n"
       "Tangent: tx1=%f, ty1=%f\n\n",
       c.object.circle.radius, c.object.circle.center.x, c.object.circle.center.y,
       p.x, p.y,
       p0.x, p0.y,
       p1.x, p1.y);

    double error = emb_vector_distance(p0, s0) + emb_vector_distance(p1, s1);
    if (error > tolerence) {
        printf("Error larger than tolerance, circle test 2: %f.\n\n", error);
        return 1;
    }
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

/* Create a test file 1 object. */
int
create_test_file(int test_file, int format)
{
    EmbPattern* p;
    char outf[200];

    p = emb_pattern_create();
    if (!p) {
        puts("ERROR: convert(), cannot allocate memory for p\n");
        return 1;
    }

    emb_pattern_addThread(p, black_thread);

    switch (test_file) {
	case 0: {
        int i;
        EmbStitch st;
		/* 10mm circle */
		for (i = 0; i < 20; i++) {
			emb_pattern_addStitchRel(p, 0.0, 1.0, JUMP, 0);
		}
		for (i = 0; i < 200; i++) {
			st.x = 10 + 10 * sin(i * (0.03141592));
			st.y = 10 + 10 * cos(i * (0.03141592));
			st.flags = NORMAL;
			st.color = 0;
			emb_pattern_addStitchAbs(p, st.x, st.y, st.flags, st.color);
		}
		break;
	}
	case 1: {
        int i;
        EmbStitch st;
        /* sin wave */
        for (i = 0; i < 100; i++) {
            st.x = 10 + 10 * sin(i * (0.5 / 3.141592));
            st.y = 10 + i * 0.1;
            st.flags = NORMAL;
            st.color = 0;
            emb_pattern_addStitchAbs(p, st.x, st.y, st.flags, st.color);
        }
		break;
	}
	case 2: {
        EmbGeometry circle = emb_circle(10.0f, 1.0f, 5.0f);
        emb_array_add_geometry(p->geometry, circle);
        emb_pattern_convertGeometry(p);
		break;
	}
    default: {
		puts("This case is not covered.");
		break;
	}
    }

    sprintf(outf, "test%02d%s", test_file, formatTable[format].extension);
    emb_pattern_end(p);
    if (!emb_pattern_write(p, outf, format)) {
        emb_pattern_free(p);
        return 16;
    }

    emb_pattern_free(p);
    return 0;
}

