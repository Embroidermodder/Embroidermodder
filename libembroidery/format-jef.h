/*! @file format-jef.h */
#ifndef FORMAT_JEF_H
#define FORMAT_JEF_H

#include "emb-pattern.h"

#include "api-start.h"
#ifdef __cplusplus
extern "C" {
#endif

extern EMB_PRIVATE int EMB_CALL readJef(EmbPattern* pattern, const char* fileName);
extern EMB_PRIVATE int EMB_CALL writeJef(EmbPattern* pattern, const char* fileName);

static const EmbThread jefThreads[] = {
    {{0, 0 ,0}, "Black", ""},
    {{0, 0, 0}, "Black", ""},
    {{255, 255, 255}, "White", ""},
    {{255, 255, 23}, "Yellow", ""},
    {{250, 160, 96}, "Orange", ""},
    {{92, 118, 73}, "Olive Green", ""},
    {{64, 192, 48}, "Green", ""},
    {{101, 194, 200}, "Sky", ""},
    {{172, 128, 190}, "Purple", ""},
    {{245, 188, 203}, "Pink", ""},
    {{255, 0, 0}, "Red", ""},
    {{192, 128, 0}, "Brown", ""},
    {{0, 0, 240}, "Blue", ""},
    {{228, 195, 93}, "Gold", ""},
    {{165, 42, 42}, "Dark Brown", ""},
    {{213, 176, 212}, "Pale Violet", ""},
    {{252, 242, 148}, "Pale Yellow", ""},
    {{240, 208, 192}, "Pale Pink", ""},
    {{255, 192, 0}, "Peach", ""},
    {{201, 164, 128}, "Beige", ""},
    {{155, 61, 75}, "Wine Red", ""},
    {{160, 184, 204}, "Pale Sky", ""},
    {{127, 194, 28}, "Yellow Green", ""},
    {{185, 185, 185}, "Silver Grey", ""},
    {{160, 160, 160}, "Grey", ""},
    {{152, 214, 189}, "Pale Aqua", ""},
    {{184, 240, 240}, "Baby Blue", ""},
    {{54, 139, 160}, "Powder Blue", ""},
    {{79, 131, 171}, "Bright Blue", ""},
    {{56, 106, 145}, "Slate Blue", ""},
    {{0, 32, 107}, "Nave Blue", ""},
    {{229, 197, 202}, "Salmon Pink", ""},
    {{249, 103, 107}, "Coral", ""},
    {{227, 49, 31}, "Burnt Orange", ""},
    {{226, 161, 136}, "Cinnamon", ""},
    {{181, 148, 116}, "Umber", ""},
    {{228, 207, 153}, "Blonde", ""},
    {{225, 203, 0}, "Sunflower", ""},
    {{225, 173, 212}, "Orchid Pink", ""},
    {{195, 0, 126}, "Peony Purple", ""},
    {{128, 0, 75}, "Burgundy", ""},
    {{160, 96, 176}, "Royal Purple", ""},
    {{192, 64, 32}, "Cardinal Red", ""},
    {{202, 224, 192}, "Opal Green", ""},
    {{137, 152, 86}, "Moss Green", ""},
    {{0, 170, 0}, "Meadow Green", ""},
    {{33, 138, 33}, "Dark Green", ""},
    {{93, 174, 148}, "Aquamarine", ""},
    {{76, 191, 143}, "Emerald Green", ""},
    {{0, 119, 114}, "Peacock Green", ""},
    {{112, 112, 112}, "Dark Grey", ""},
    {{242, 255, 255}, "Ivory White", ""},
    {{177, 88, 24}, "Hazel", ""},
    {{203, 138, 7}, "Toast", ""},
    {{247, 146, 123}, "Salmon", ""},
    {{152, 105, 45}, "Cocoa Brown", ""},
    {{162, 113, 72}, "Sienna", ""},
    {{123, 85, 74}, "Sepia", ""},
    {{79, 57, 70}, "Dark Sepia", ""},
    {{82, 58, 151}, "Violet Blue", ""},
    {{0, 0, 160}, "Blue Ink", ""},
    {{0, 150, 222}, "Solar Blue", ""},
    {{178, 221, 83}, "Green Dust", ""},
    {{250, 143, 187}, "Crimson", ""},
    {{222, 100, 158}, "Floral Pink", ""},
    {{181, 80, 102}, "Wine", ""},
    {{94, 87, 71}, "Olive Drab", ""},
    {{76, 136, 31}, "Meadow", ""},
    {{228, 220, 121}, "Mustard", ""},
    {{203, 138, 26}, "Yellow Ochre", ""},
    {{198, 170, 66}, "Old Gold", ""},
    {{236, 176, 44}, "Honeydew", ""},
    {{248, 128, 64}, "Tangerine", ""},
    {{255, 229, 5}, "Canary Yellow", ""},
    {{250, 122, 122}, "Vermillion", ""},
    {{107, 224, 0}, "Bright Green", ""},
    {{56, 108, 174}, "Ocean Blue", ""},
    {{227, 196, 180}, "Beige Grey", ""},
    {{227, 172, 129}, "Bamboo", ""}};

#ifdef __cplusplus
}
#endif /* __cplusplus */
#include "api-stop.h"

#endif /* FORMAT_JEF_H */

/* kate: bom off; indent-mode cstyle; indent-width 4; replace-trailing-space-save on; */
