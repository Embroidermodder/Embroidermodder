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

char* rTrim(char* const string, char junk);
char* lTrim(char* const string, char junk);

char* emb_strdup(const char* src);

#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif /* HELPERS_MISC_H */

/* kate: bom off; indent-mode cstyle; indent-width 4; replace-trailing-space-save on; */
