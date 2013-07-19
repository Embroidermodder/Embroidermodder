#ifndef FORMAT_PEC_H
#define FORMAT_PEC_H

#ifdef __cplusplus
extern "C" {
#endif

#include <stdio.h>
#include "emb-pattern.h"

int readPec(EmbPattern* pattern, const char* fileName);
int writePec(EmbPattern* pattern, const char* fileName);
void readPecStitches(EmbPattern* pattern, FILE* file);
void writePecStitches(EmbPattern* pattern, FILE* file, const char* filename);

static const int pecThreadCount = 65;
static const EmbThread pecThreads[] = {{{0, 0, 0}, "Unknown", ""},
        {{14, 31, 124}, "Prussian Blue", ""},
        {{10, 85, 163}, "Blue", ""},
        {{0, 135, 119}, "Teal Green", ""},
        {{75, 107, 175}, "Cornflower Blue", ""},
        {{237, 23, 31}, "Red", ""},
        {{209, 92, 0}, "Reddish Brown", ""},
        {{145, 54, 151}, "Magenta", ""},
        {{228, 154, 203}, "Light Lilac", ""},
        {{145, 95, 172}, "Lilac", ""},
        {{158, 214, 125}, "Mint Green", ""},
        {{232, 169, 0}, "Deep Gold", ""},
        {{254, 186, 53}, "Orange", ""},
        {{255, 255, 0}, "Yellow", ""},
        {{112, 188, 31}, "Lime Green", ""},
        {{186, 152, 0}, "Brass", ""},
        {{168, 168, 168}, "Silver", ""},
        {{125, 111, 0}, "Russet Brown", ""},
        {{255, 255, 179}, "Cream Brown", ""},
        {{79, 85, 86}, "Pewter", ""},
        {{0, 0, 0}, "Black", ""},
        {{11, 61, 145}, "Ultramarine", ""},
        {{119, 1, 118}, "Royal Purple", ""},
        {{41, 49, 51}, "Dark Gray", ""},
        {{42, 19, 1}, "Dark Brown", ""},
        {{246, 74, 138}, "Deep Rose", ""},
        {{178, 118, 36}, "Light Brown", ""},
        {{252, 187, 197}, "Salmon Pink", ""},
        {{254, 55, 15}, "Vermillion", ""},
        {{240, 240, 240}, "White", ""},
        {{106, 28, 138}, "Violet", ""},
        {{168, 221, 196}, "Seacrest", ""},
        {{37, 132, 187}, "Sky Blue", ""},
        {{254, 179, 67}, "Pumpkin", ""},
        {{255, 243, 107}, "Cream Yellow", ""},
        {{208, 166, 96}, "Khaki", ""},
        {{209, 84, 0}, "Clay Brown", ""},
        {{102, 186, 73}, "Leaf Green", ""},
        {{19, 74, 70}, "Peacock Blue", ""},
        {{135, 135, 135}, "Gray", ""},
        {{216, 204, 198}, "Warm Gray", ""},
        {{67, 86, 7}, "Dark Olive", ""},
        {{253, 217, 222}, "Flesh Pink", ""},
        {{249, 147, 188}, "Pink", ""},
        {{0, 56, 34}, "Deep Green", ""},
        {{178, 175, 212}, "Lavender", ""},
        {{104, 106, 176}, "Wisteria Violet", ""},
        {{239, 227, 185}, "Beige", ""},
        {{247, 56, 102}, "Carmine", ""},
        {{181, 75, 100}, "Amber Red", ""},
        {{19, 43, 26}, "Olive Green", ""},
        {{199, 1, 86}, "Dark Fuschia", ""},
        {{254, 158, 50}, "Tangerine", ""},
        {{168, 222, 235}, "Light Blue", ""},
        {{0, 103, 62}, "Emerald Green", ""},
        {{78, 41, 144}, "Purple", ""},
        {{47, 126, 32}, "Moss Green", ""},
        {{255, 204, 204}, "Flesh Pink", ""},
        {{255, 217, 17}, "Harvest Gold", ""},
        {{9, 91, 166}, "Electric Blue", ""},
        {{240, 249, 112}, "Lemon Yellow", ""},
        {{227, 243, 91}, "Fresh Green", ""},
        {{255, 153, 0}, "Orange", ""},
        {{255, 240, 141}, "Cream Yellow", ""},
        {{255, 200, 200}, "Applique", ""}};

#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif /* FORMAT_PEC_H */

/* kate: bom off; indent-mode cstyle; indent-width 4; replace-trailing-space-save on; */
