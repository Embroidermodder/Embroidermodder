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

 #define _SCHEME_SOURCE
#include "scheme.h"
#include "scheme-private.h"

/*------------------ API Wrappers -------------------------------------*/

int scheme_retcode(scheme *sc) {
  return sc->retcode;
}

pointer scheme_global_env(scheme *sc) {
  return sc->global_env;
}

pointer scheme_nil(scheme *sc) {
  return sc->NIL;
}

pointer scheme_true(scheme *sc) {
  return sc->T;
}

pointer scheme_false(scheme *sc) {
  return sc->F;
}

pointer scheme_reverse(scheme *sc, pointer a) {
  return reverse(sc, a);
}

pointer scheme_reverse_in_place(scheme *sc, pointer term, pointer list) {
  return reverse_in_place(sc, term, list);
}

pointer scheme_cons(scheme *sc, pointer a, pointer b, int immutable) {
	return _cons(sc, a, b, immutable);
}

pointer scheme_integer(scheme *sc, long num) {
	return mk_integer(sc, num);
}

pointer scheme_real(scheme *sc, double num) {
	return mk_real(sc, num);
}

pointer scheme_symbol(scheme *sc, const char *name) {
	return mk_symbol(sc, name);
}

pointer scheme_gensym(scheme *sc) {
	return gensym(sc);
}

pointer scheme_string(scheme *sc, const char *str) {
	return mk_string(sc, str);
}

pointer scheme_counted_string(scheme *sc, const char *str, int len) {
	return mk_counted_string(sc, str, len);
}

pointer scheme_empty_string(scheme *sc, int len, char fill) {
	return mk_empty_string(sc, len, fill);
}

pointer scheme_character(scheme *sc, int c) {
	return mk_character(sc, c);
}

pointer scheme_foreign_func(scheme *sc, foreign_func f) {
	return mk_foreign_func(sc, f);
}

void scheme_putstr(scheme *sc, const char *s) {
	putstr(sc, s);
}

int scheme_list_length(scheme *sc, pointer a) {
	return list_length(sc, a);
}

int scheme_eqv(pointer a, pointer b) {
	return eqv(a, b);
}

int scheme_is_string(pointer p) {
	return is_string(p);
}

char* scheme_string_value(pointer p) {
	return string_value(p);
}

int scheme_is_number(pointer p) {
	return is_number(p);
}

num scheme_nvalue(pointer p) {
	return nvalue(p);
}

long scheme_ivalue(pointer p) {
	return ivalue(p);
}

double scheme_rvalue(pointer p) {
	return rvalue(p);
}

int scheme_is_integer(pointer p) {
	return is_integer(p);
}

int scheme_is_real(pointer p) {
	return is_real(p);
}

int scheme_is_character(pointer p) {
	return is_character(p);
}

long scheme_charvalue(pointer p) {
	return charvalue(p);
}

int scheme_is_vector(pointer p) {
	return is_vector(p);
}

int scheme_is_port(pointer p) {
	return is_port(p);
}

int scheme_is_pair(pointer p) {
	return is_pair(p);
}

pointer scheme_pair_car(pointer p) {
	return pair_car(p);
}

pointer scheme_pair_cdr(pointer p) {
	return pair_cdr(p);
}

pointer scheme_set_car(pointer p, pointer q) {
	return set_car(p, q);
}

pointer scheme_set_cdr(pointer p, pointer q) {
	return set_cdr(p, q);
}

int scheme_is_symbol(pointer p) {
	return is_symbol(p);
}

char* scheme_symname(pointer p) {
	return symname(p);
}

int scheme_is_syntax(pointer p) {
	return is_syntax(p);
}

int scheme_is_proc(pointer p) {
	return is_proc(p);
}

int scheme_is_foreign(pointer p) {
	return is_foreign(p);
}

char* scheme_syntaxname(pointer p) {
	return syntaxname(p);
}

int scheme_is_closure(pointer p) {
	return is_closure(p);
}

#ifdef USE_MACRO

SCHEME_EXPORT int scheme_is_macro(pointer p) {
	return is_macro(p);
}

#endif

int scheme_is_continuation(pointer p) {
	return is_continuation(p);
}

int scheme_is_promise(pointer p) {
	return is_promise(p);
}

int scheme_is_environment(pointer p) {
	return is_environment(p);
}

int scheme_is_immutable(pointer p) {
	return is_immutable(p);
}

void scheme_setimmutable(pointer p) {
	setimmutable(p);
}

void scheme_fill_vector(pointer vec, pointer obj) {
	fill_vector(vec, obj);
}

pointer scheme_vector_elem(pointer vec, int ielem) {
	return vector_elem(vec, ielem);
}

pointer scheme_set_vector_elem(pointer vec, int ielem, pointer a) {
	return set_vector_elem(vec, ielem, a);
}

void scheme_memory_error(scheme *sc) {
	sc->no_memory=1;
}