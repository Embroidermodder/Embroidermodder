/* Fractal Generation Testing */

#include <string.h>
#include <math.h>

#include "../src/embroidery.h"

int
main(void)
{
    EmbPattern *pattern = emb_pattern_create();
    int hilbertCurveResult = hilbert_curve(pattern, 3);
    int renderResult = emb_pattern_render(pattern, "hilbert_level_3.png");
    int simulateResult = emb_pattern_simulate(pattern, "hilbert_level_3.avi");
    printf("hilbert curve result: %d\n", hilbertCurveResult);
    printf("render result: %d\n", renderResult);
    printf("simulate result: %d\n", simulateResult);
    emb_pattern_free(pattern);
    return hilbertCurveResult;
}

