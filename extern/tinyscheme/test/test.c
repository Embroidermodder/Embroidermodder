// Simple test

#include "scheme.h"
#include "scheme-private.h"
//#include "scheme.c"

pointer test_send_tuple(scheme *sc, pointer args) {
	//                                                                                      (a b c NIL)
	int a = sc->vptr->ivalue(sc->vptr->pair_car(args)); args = sc->vptr->pair_cdr(args); // (b c NIL)
	int b = sc->vptr->ivalue(sc->vptr->pair_car(args)); args = sc->vptr->pair_cdr(args); // (c NIL)
	int c = sc->vptr->ivalue(sc->vptr->pair_car(args)); args = sc->vptr->pair_cdr(args); // NIL
	printf("(%d %d %d)\n", a, b, c);
	return sc->NIL;
}

pointer test_print_asterisk(scheme *sc, pointer args) {
	if(!sc->vptr->is_string(sc->vptr->pair_car(args))) {
		printf("print-*: not a string");
		return sc->NIL;
	}
	printf("** %s **", sc->vptr->string_value(sc->vptr->pair_car(args)));
	return sc->NIL;
}

scheme_registerable ff_list[] = {
	{ test_send_tuple, "send-tuple" },
	{ test_print_asterisk, "print-*" }
};

int main(void) {
	scheme *sc;

	sc = scheme_init_new();

	scheme_set_input_port_file(sc, stdin);
  	scheme_set_output_port_file(sc, stdout);

	// Load init.scm (make sure it's in workdir)
	char *cinit = "init.scm";
	FILE *finit = fopen(cinit, "r");
	if (!finit) { cinit = "../init.scm"; finit = fopen(cinit, "r"); }
	scheme_load_file(sc, finit);
	fclose(finit);

	scheme_register_foreign_func_list(sc, ff_list, 2);

	char *source = "(display \"Hello World!\")    (send-tuple 1 2 3)    (print-* \"This works!\")";
	scheme_load_string(sc, source);

	scheme_deinit(sc);
	return 0;
}