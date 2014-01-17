/*! @file helpers-misc.h */
#ifndef HELPERS_MISC_H
#define HELPERS_MISC_H

#ifdef __cplusplus
extern "C" {
#endif

#define PI 3.1415926535

#ifndef MINMAX
#define MINMAX
  #ifndef max
    #define max(a,b) (((a) > (b)) ? (a) : (b))
  #endif
  #ifndef min
    #define min(a,b) (((a) < (b)) ? (a) : (b))
  #endif
#endif

int roundDouble(double src);
char startsWith(const char* pre, const char* str);

char* rTrim(char* const str, char junk);
char* lTrim(char* const str, char junk);
char *copy_trim(char const *s);
void inplace_trim(char *s);
char* emb_optOut(double num, char* str);
char* emb_strdup(const char* src);

#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif /* HELPERS_MISC_H */

/* kate: bom off; indent-mode cstyle; indent-width 4; replace-trailing-space-save on; */
