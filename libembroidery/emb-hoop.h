/*! @file emb-hoop.h */
#ifndef EMB_HOOP_H
#define EMB_HOOP_H

#ifdef __cplusplus
extern "C" {
#endif

typedef struct EmbHoop_
{
    double width;
    double height;
} EmbHoop;

double embHoop_width(EmbHoop hoop);
double embHoop_height(EmbHoop hoop);

#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif /* EMB_HOOP_H */

/* kate: bom off; indent-mode cstyle; indent-width 4; replace-trailing-space-save on; */
