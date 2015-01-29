#ifndef Deuterium_Python_Module_hh
#define Deuterium_Python_Module_hh

/*
//The following should be used for the third     METH_VARARGS | METH_KEYWORDS
static PyMethodDef SpamMethods[] = {
    ...

    {"system",  spam_system, METH_VARARGS,"Execute a shell command."},
    ...
    {NULL, NULL, 0, NULL}        /* Sentinel */
//};


PyMODINIT_FUNC
initspam(void)
{
    (void) Py_InitModule("spam", SpamMethods);
}



//Module initialization:
/*PyMODINIT_FUNC
initspam(void)
{
    PyObject *m;

    m = Py_InitModule("spam", SpamMethods);
    if (m == NULL)
        return;

    SpamError = PyErr_NewException("spam.error", NULL, NULL);
    Py_INCREF(SpamError);
    PyModule_AddObject(m, "error", SpamError);
}*/



#endif