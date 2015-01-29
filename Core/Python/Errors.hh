#ifndef Deuterium_Python_Errors_hh_
#define Deuterium_Python_Errors_hh_

#include "Python.h"

//Rules: when a function fails, it should set an exception condition and return an error value (usually a NULL pointer)
//Exception conditions to set:
//  PyErr_SetString()

//How to declare an error
#define PYTHON_ERR_DECL(ERR_NAME) static PyObject *ERR_NAME;
//How to define an error
#define PYTHON_ERR_DEF(ERR_NAME,ERR_VALUE, MODULE_NAME)  ERR_NAME = PyErr_NewException(#ERR_VALUE, NULL, NULL); Py_INCREF(ERR_NAME); PyModule_AddObject(MODULE_NAME, "error", ERR_NAME);


#endif //File Guardian