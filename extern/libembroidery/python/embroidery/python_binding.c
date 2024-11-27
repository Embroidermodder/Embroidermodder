/*
 * This file is part of libembroidery.
 *
 * Copyright 2018-2022 The Embroidermodder Team
 * Licensed under the terms of the zlib license.
 *
 * ---------------------------------------------------------
 *
 * The bindings for libembroidery on Python.
 */

#include <stdio.h>

#include "embroidery.h"

#define PY_SSIZE_T_CLEAN
#include <Python.h>

EmbPattern *patterns[100];

static PyObject *
embroidery_create_pattern(PyObject *self, PyObject *args)
{
     return PyLong_fromLong(0);
}

static PyMethodDef EmbroideryMethods[] = {
    {"create_pattern", embroidery_create_pattern, METH_VARARGS, "Create a new pattern object."},
    {NULL, NULL, 0, NULL}
};

static struct PyModuleDef embroiderymodule = {

};

PyMODINIT_FUNC
PyInit_embroidery(void)
{
    return PyModule_Create(&embroiderymodule);
}
