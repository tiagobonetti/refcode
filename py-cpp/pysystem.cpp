#include <iostream>
#include <Python.h>

static PyObject *PySystem(PyObject *self, PyObject *args)
{
    const char *command;
    int ret;

    if (!PyArg_ParseTuple(args, "s", &command)) {
        return nullptr;
    }
    std::cout << "CMD: \"" << command << "\"" << std::endl;
    ret = system(command);
    return Py_BuildValue("i", ret);
}

static PyMethodDef PyMethods[] = {
    { "system", PySystem, METH_VARARGS, "Execute a shell command." },
    { NULL, NULL, 0, NULL } /* Sentinel */
};

static struct PyModuleDef PyModule = {  //
    PyModuleDef_HEAD_INIT,              //
    "cppdemo",                          // name of module
    "Test module for cpp code",         // module documentation, may be NULL
    -1,                                 // size of per-interpreter state of the module, or
                                        // -1 if the module keeps state in global variables.
    PyMethods
};

PyMODINIT_FUNC PyInit_cppdemo(void)
{
    return PyModule_Create(&PyModule);
}
