/*
 * Embroidermodder 2 -- C Core
 * Copyright 2011-2026 The Embroidermodder Team
 *
 * See the official docs for advice on how this works:
 * https://docs.python.org/3/extending/embedding.html
 *
 * Scripting in embroidermodder is mainly for adding new
 * commands and configuration: the state is managed in a C
 * structure and we get and set data from it in scripts.
 * This is to keep rendering optimized and the data structures
 * consistent. Managing copies of the state would create conflicts.
 *
 * Many commands are loaded by `boot.py` by wrapping `cmd`,
 * for example:
 *
 *     def icon16():
 *         cmd("icon16")
 *
 * More complex commands use get and set to interact with
 * state variables.
 */

#define PY_SSIZE_T_CLEAN
#include <Python.h>

#include "core.h"
#include "gui.h"

static PyObject *
em2_cmd(PyObject *self, PyObject *args)
{
    char *line;
    if (!PyArg_ParseTuple(args, "s:cmd", &line)) {
        return NULL;
    }
    call(&state, (const char *)line);
    repaint();
    Py_RETURN_NONE;
}

/* FIXME: unfinished: should return a state value. */
static PyObject *
em2_get(PyObject *self, PyObject *args)
{
    char *variable;
    if (!PyArg_ParseTuple(args, "s:get", &variable)) {
        return NULL;
    }

    Py_RETURN_NONE;
}

/* FIXME: unfinished */
static PyObject *
em2_set(PyObject *self, PyObject *args)
{
    char *variable;
    char *value;
    if (!PyArg_ParseTuple(args, "ss:set", &variable, &value)) {
        return NULL;
    }

    Py_RETURN_NONE;
}

static PyMethodDef em2_methods[] = {
    {"cmd", (PyCFunction)(void(*)(void))em2_cmd, METH_VARARGS,
     "Call the command line."},
    {"get", (PyCFunction)(void(*)(void))em2_get, METH_VARARGS,
     "Get a state variable."},
    {"set", (PyCFunction)(void(*)(void))em2_set, METH_VARARGS,
     "Set a state variable."},
    {NULL, NULL, 0, NULL}
};

static struct PyModuleDef em2_module = {
    .m_base = PyModuleDef_HEAD_INIT,
    .m_name = "em2",
    .m_size = 0,
    .m_methods = em2_methods
};

PyMODINIT_FUNC
PyInit_em2(void)
{
    return PyModuleDef_Init(&em2_module);
}

/* Based on the example of module loading from:
 * https://docs.python.org/3/extending/extending.html
 */
int script_env_init(void)
{
    PyConfig config;
    PyStatus status;
    PyConfig_InitPythonConfig(&config);

    if (PyImport_AppendInittab("em2", PyInit_em2) == -1) {
        printf("ERROR: could not extend in-built modules table.");
        return 0;
    }

    status = PyConfig_SetBytesString(&config, &config.program_name, "embroidermodder2");
    if (PyStatus_Exception(status)) {
        PyConfig_Clear(&config);
        Py_ExitStatusException(status);
        return 0;
    }
    PyConfig_Clear(&config);

    status = Py_InitializeFromConfig(&config);
    if (PyStatus_Exception(status)) {
        PyConfig_Clear(&config);
        Py_ExitStatusException(status);
        return 0;
    }

    PyObject *module = PyImport_ImportModule("em2");
    if (!module) {
        PyErr_Print();
        printf("ERROR: could not import \"em2\".");
        return 0;
    }

    return script_env_call("script/boot.py");
}

int script_env_call(const char *fname)
{
    size_t length;
    uint8_t *boot_script = load_file(fname, &length);
    if (boot_script == NULL) {
        printf("ERROR: could not load boot script.");
        return 0;
    }
    if (length > 1000000) {
        printf("WARNING: boot script is too large.");
        return 0;
    }
    PyRun_SimpleString(boot_script);
    free(boot_script);
    return 1;
}

int script_env_free(void)
{
    if (Py_FinalizeEx() < 0) {
        /* We use the same error number as the Python docs. */
        return 120;
    }
    return 0;
}

