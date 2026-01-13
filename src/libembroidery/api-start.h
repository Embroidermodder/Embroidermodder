/* Only include this in libembroidery header files around functions to be exported as shared library */
#if defined(API_START_H)
    #error Nested inclusion of api-start.h is not allowed
#endif
#define API_START_H

/* Sanity check */
#if defined(LIBEMBROIDERY_SHARED) && defined(LIBEMBROIDERY_STATIC)
    #error Library cannot be shared and static at the same time
#endif

/* When building a shared library, use the proper export keyword depending on the compiler */
#if defined(LIBEMBROIDERY_SHARED)
    #if defined(__WIN32__)
        #define EMB_PUBLIC __declspec(dllexport)
        #define EMB_PRIVATE
    #else
        #if defined(__GNUC__) && __GNUC__ >= 4
            #define EMB_PUBLIC __attribute__ ((visibility("default")))
            #define EMB_PRIVATE __attribute__ ((visibility("hidden")))
        #elif defined(__GNUC__) && __GNUC__ >= 2
            #define EMB_PUBLIC __declspec(dllexport)
            #define EMB_PRIVATE
        #else
            #define EMB_PUBLIC
            #define EMB_PRIVATE
        #endif
    #endif
#else
    #define EMB_PUBLIC
    #define EMB_PRIVATE
#endif

/* Use the C calling convention */
#ifndef EMB_CALL
    #if defined(__WIN32__) && !defined(__GNUC__)
        #define EMB_CALL __cdecl
    #else
        #define EMB_CALL
    #endif
#endif /* EMBCALL */

/* Disable warnings about unsafe use of fopen, fseek etc */
#if !defined(__GNUC__)
    #pragma warning(disable: 4996)
#endif

/* kate: bom off; indent-mode cstyle; indent-width 4; replace-trailing-space-save on; */
