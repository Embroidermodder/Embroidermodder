/*! @file emb-logging.h */
#ifndef EMB_LOGGING_H
#define EMB_LOGGING_H

#ifdef __cplusplus
extern "C" {
#endif

void embLog_print(const char* format, ...);
void embLog_error(const char* format, ...);

#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif /* EMB_LOGGING_H */

/* kate: bom off; indent-mode cstyle; indent-width 4; replace-trailing-space-save on; */
