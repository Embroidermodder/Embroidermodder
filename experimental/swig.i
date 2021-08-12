/* libembroidery SWIG language bindings interface file */
%module libembroidery

%inline %{
#include "embroidery.h"
%}

/* TODO: review EMB_PRIVATE functions and move them into private files if needed. */
%include "embroidery.h"
