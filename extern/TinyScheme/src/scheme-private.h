/* T I N Y S C H E M E    1 . 50
 *   Continued by armornick (March 2016)
 *   Original work by Dimitrios Souflis (dsouflis@acm.org)
 *   
 *   Based on MiniScheme (original credits follow)
 * (MINISCM)               coded by Atsushi Moriwaki (11/5/1989)
 * (MINISCM)           E-MAIL :  moriwaki@kurims.kurims.kyoto-u.ac.jp
 * (MINISCM) This version has been modified by R.C. Secrist.
 * (MINISCM)
 * (MINISCM) Mini-Scheme is now maintained by Akira KIDA.
 * (MINISCM)
 * (MINISCM) This is a revised and modified version by Akira KIDA.
 * (MINISCM)    current version is 0.85k4 (15 May 1994)
 * (MINISCM)
 *
 */

/* scheme-private.h */

#ifndef _SCHEME_PRIVATE_H
#define _SCHEME_PRIVATE_H

#include "scheme.h"
/*------------------ Ugly internals -----------------------------------*/
/*------------------ Of interest only to FFI users --------------------*/

#ifdef __cplusplus
extern "C" {
#endif

enum scheme_port_kind {
  port_free=0,
  port_file=1,
  port_string=2,
  port_srfi6=4,
  port_input=16,
  port_output=32,
  port_saw_EOF=64
};

typedef struct port {
  unsigned char kind;
  union {
    struct {
      FILE *file;
      int closeit;
#if SHOW_ERROR_LINE
      int curr_line;
      char *filename;
#endif
    } stdio;
    struct {
      char *start;
      char *past_the_end;
      char *curr;
    } string;
  } rep;
} port;

/* cell structure */
struct cell {
  unsigned int _flag;
  union {
    struct {
      char   *_svalue;
      int   _length;
    } _string;
    num _number;
    port *_port;
    foreign_func _ff;
    struct {
      struct cell *_car;
      struct cell *_cdr;
    } _cons;
  } _object;
};

struct scheme {
  /* arrays for segments */
  func_alloc malloc;
  func_dealloc free;

  /* return code */
  int retcode;
  int tracing;


  #define CELL_SEGSIZE    5000  /* # of cells in one segment */
  #define CELL_NSEGMENT   10    /* # of segments for cells */
  char *alloc_seg[CELL_NSEGMENT];
  pointer cell_seg[CELL_NSEGMENT];
  int     last_cell_seg;

  /* We use 4 registers. */
  pointer args;            /* register for arguments of function */
  pointer envir;           /* stack register for current environment */
  pointer code;            /* register for current code */
  pointer dump;            /* stack register for next evaluation */

  int interactive_repl;    /* are we in an interactive REPL? */

  struct cell _sink;
  pointer sink;            /* when mem. alloc. fails */
  struct cell _NIL;
  pointer NIL;             /* special cell representing empty cell */
  struct cell _HASHT;
  pointer T;               /* special cell representing #t */
  struct cell _HASHF;
  pointer F;               /* special cell representing #f */
  struct cell _EOF_OBJ;
  pointer EOF_OBJ;         /* special cell representing end-of-file object */
  pointer oblist;          /* pointer to symbol table */
  pointer global_env;      /* pointer to global environment */
  pointer c_nest;          /* stack for nested calls from C */

  /* global pointers to special symbols */
  pointer LAMBDA;               /* pointer to syntax lambda */
  pointer QUOTE;           /* pointer to syntax quote */

  pointer QQUOTE;               /* pointer to symbol quasiquote */
  pointer UNQUOTE;         /* pointer to symbol unquote */
  pointer UNQUOTESP;       /* pointer to symbol unquote-splicing */
  pointer FEED_TO;         /* => */
  pointer COLON_HOOK;      /* *colon-hook* */
  pointer ERROR_HOOK;      /* *error-hook* */
  pointer SHARP_HOOK;  /* *sharp-hook* */
  pointer COMPILE_HOOK;  /* *compile-hook* */

  pointer free_cell;       /* pointer to top of free cells */
  long    fcells;          /* # of free cells */

  pointer inport;
  pointer outport;
  pointer save_inport;
  pointer loadport;

  #define MAXFIL 64
  port load_stack[MAXFIL];     /* Stack of open files for port -1 (LOADing) */
  int nesting_stack[MAXFIL];
  int file_i;
  int nesting;

  char    gc_verbose;      /* if gc_verbose is not zero, print gc status */
  char    no_memory;       /* Whether mem. alloc. has failed */

  #define LINESIZE 1024
  char    linebuff[LINESIZE];
  #define STRBUFFSIZE 256
  char    strbuff[STRBUFFSIZE];

  FILE *tmpfp;
  int tok;
  int print_flag;
  pointer value;
  int op;

  void *ext_data;     /* For the benefit of foreign functions */
  long gensym_cnt;

  struct scheme_interface *vptr;
  void *dump_base;    /* pointer to base of allocated dump stack */
  int dump_size;      /* number of frames allocated for dump stack */
};

/* operator code */
enum scheme_opcodes {
#define _OP_DEF(A,B,C,D,E,OP) OP,
#include "opdefines.h"
  OP_MAXDEFINED
};


pointer _cons(scheme *sc, pointer a, pointer b, int immutable);
pointer mk_integer(scheme *sc, long num);
pointer mk_real(scheme *sc, double num);
pointer mk_symbol(scheme *sc, const char *name);
pointer gensym(scheme *sc);
pointer mk_string(scheme *sc, const char *str);
pointer mk_counted_string(scheme *sc, const char *str, int len);
pointer mk_empty_string(scheme *sc, int len, char fill);
pointer mk_character(scheme *sc, int c);
pointer mk_foreign_func(scheme *sc, foreign_func f);
void putstr(scheme *sc, const char *s);
int list_length(scheme *sc, pointer a);
int eqv(pointer a, pointer b);


int is_string(pointer p);
char *string_value(pointer p);
int is_number(pointer p);
num nvalue(pointer p);
long ivalue(pointer p);
double rvalue(pointer p);
int is_integer(pointer p);
int is_real(pointer p);
int is_character(pointer p);
long charvalue(pointer p);
int is_vector(pointer p);

int is_port(pointer p);

int is_pair(pointer p);
pointer pair_car(pointer p);
pointer pair_cdr(pointer p);
pointer set_car(pointer p, pointer q);
pointer set_cdr(pointer p, pointer q);

int is_symbol(pointer p);
char *symname(pointer p);
int hasprop(pointer p);

int is_syntax(pointer p);
int is_proc(pointer p);
int is_foreign(pointer p);
char *syntaxname(pointer p);
int is_closure(pointer p);
#ifdef USE_MACRO
int is_macro(pointer p);
#endif
pointer closure_code(pointer p);
pointer closure_env(pointer p);

int is_continuation(pointer p);
int is_promise(pointer p);
int is_environment(pointer p);
int is_immutable(pointer p);
void setimmutable(pointer p);

pointer reverse(scheme *sc, pointer a);
pointer reverse_in_place(scheme *sc, pointer term, pointer list);
void fill_vector(pointer vec, pointer obj);
pointer vector_elem(pointer vec, int ielem);
pointer set_vector_elem(pointer vec, int ielem, pointer a);

#ifdef __cplusplus
}
#endif

#endif

