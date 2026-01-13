#ifndef THREAD_COLOR_H
#define THREAD_COLOR_H

#ifdef __cplusplus
extern "C" {
#endif

typedef enum
{
    Arc_Polyester,
    Arc_Rayon,
    CoatsAndClark_Rayon,
    Exquisite_Polyester,
    Fufu_Polyester,
    Fufu_Rayon,
    Hemingworth_Polyester,
    Isacord_Polyester,
    Isafil_Rayon,
    Marathon_Polyester,
    Marathon_Rayon,
    Madeira_Polyester,
    Madeira_Rayon,
    Metro_Polyester,
    Pantone,
    RobisonAnton_Polyester,
    RobisonAnton_Rayon,
    Sigma_Polyester,
    Sulky_Rayon,
    ThreadArt_Rayon,
    ThreadArt_Polyester,
    ThreaDelight_Polyester,
    Z102_Isacord_Polyester
} ThreadBrand;

/* NOTE:
 * colors must be passed in #AARRGGBB format with
 * the alpha value always being FF, i.e. opaque
*/

int threadColorNum(unsigned int color, ThreadBrand brand);
const char* threadColorName(unsigned int color, ThreadBrand brand);

#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif /* THREAD_COLOR_H */

/* kate: bom off; indent-mode cstyle; indent-width 4; replace-trailing-space-save on; */
