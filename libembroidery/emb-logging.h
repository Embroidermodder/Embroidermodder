/*! @file emb-logging.h */
#ifndef EMB_LOGGING_H
#define EMB_LOGGING_H

#include "api-start.h"
#ifdef __cplusplus
extern "C" {
#endif

#ifdef ARDUINO
#include "utility/ino-logging.h"
#endif

extern EMB_PUBLIC void EMB_CALL embLog_print(const char* format, ...);
extern EMB_PUBLIC void EMB_CALL embLog_error(const char* format, ...);

#ifdef __cplusplus
}
#endif /* __cplusplus */
#include "api-stop.h"

#endif /* EMB_LOGGING_H */

/* kate: bom off; indent-mode cstyle; indent-width 4; replace-trailing-space-save on; */
