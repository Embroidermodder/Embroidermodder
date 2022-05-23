/* re.c */
/* Henry Spencer's implementation of Regular Expressions,
   used for TinyScheme */
/* Refurbished by Stephen Gildea */
/* Updated by armornick */

/* this defintion is to export the init function */
#define SCHEME_MODULE
#include "regex.h"
#include <scheme.h>

pointer foreign_re_match(scheme *sc, pointer args) {
  pointer retval=scheme_false(sc);
  int retcode;
  regex_t rt;
  pointer first_arg, second_arg;
  pointer third_arg=scheme_nil(sc);
  char *string;
  char *pattern;
  int num=0;

  if(!((args != scheme_nil(sc)) && scheme_is_string((first_arg = scheme_pair_car(args)))
       && (args=scheme_pair_cdr(args))
       && scheme_is_pair(args) && scheme_is_string((second_arg = scheme_pair_car(args))))) {
    return scheme_false(sc);
  }
  pattern = scheme_string_value(first_arg);
  string = scheme_string_value(second_arg);

  args=scheme_pair_cdr(args);
  if(args!=scheme_nil(sc)) {
    if(!(scheme_is_pair(args) && scheme_is_vector((third_arg = scheme_pair_car(args))))) {
      return scheme_false(sc);
    } else {
      num = scheme_ivalue(third_arg);
    }
  }


  if(regcomp(&rt,pattern,REG_EXTENDED)!=0) {
    return scheme_false(sc);
  }

  if(num==0) {
    retcode=regexec(&rt,string,0,0,0);
  } else {
    regmatch_t *pmatch=malloc((num+1)*sizeof(regmatch_t));
    if(pmatch!=0) {
      retcode=regexec(&rt,string,num+1,pmatch,0);
      if(retcode==0) {
    int i;
    for(i=0; i<num; i++) {
      scheme_set_vector_elem(third_arg, i,
              cons(sc, scheme_integer(sc, pmatch[i].rm_so),
                          scheme_integer(sc, pmatch[i].rm_eo)));

    }
      }
      free(pmatch);
    } else {
      scheme_memory_error(sc);
      retcode=-1;
    }
  }

  if(retcode==0) {
    retval=scheme_true(sc);
  }

  regfree(&rt);

  return(retval);
}

static char* utilities=";; return the substring of STRING matched in MATCH-VECTOR, \n"
";; the Nth subexpression match (default 0).\n"
"(define (re-match-nth string match-vector . n)\n"
"  (let ((n (if (pair? n) (car n) 0)))\n"
"    (substring string (car (vector-ref match-vector n))\n"
"                    (cdr (vector-ref match-vector n)))))\n"
"(define (re-before-nth string match-vector . n)\n"
"  (let ((n (if (pair? n) (car n) 0)))\n"
"    (substring string 0 (car (vector-ref match-vector n)))))\n"
"(define (re-after-nth string match-vector . n)\n"
"  (let ((n (if (pair? n) (car n) 0)))\n"
"    (substring string (cdr (vector-ref match-vector n))\n"
"             (string-length string))))\n";

SCHEME_EXPORT void init_re(scheme *sc) {
  scheme_define(sc, scheme_global_env(sc), scheme_symbol(sc, "re-match"), scheme_foreign_func(sc, foreign_re_match));
  // printf("RE: adding utility functions\n");
  // scheme_load_string(sc, utilities);
}
