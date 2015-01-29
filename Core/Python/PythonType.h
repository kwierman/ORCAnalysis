#ifndef ORCA_PythonType_h_
#define ORCA_PythonType_h_

#include <Python.h>
//Can also get from prefix/include/pythonversion/ and exec_prefix/include/pythonversion/



//All user visible names in Python are prefixed with Py or _Py. Names beginning with _Py or for internal use ony.
//Users should never define names beginning with that prefix

namespace ORCA{

	namespace Python{

		//Python opaque objects are called PyObjects
		//Never declare PyObject on the stack. Always on heap

		static PyObject* ORCAError;

		static PyObject* base_ORCA_python_method(PyObject* self, PyObject* args){
			const char* command;
			int sts;

			if(!PyArg_ParseTuple(args, "s", &command) )
				return NULL;
			sts = system(command);
			if(sts<0)
				PyErr_SetString(ORCAError, "ORCA Command Failed");
			return Py_BuildValue("i", sts);
		}

		//Errors and Exceptions

		static PyMethodDef ORCAMethods[]={
			//METH_VARARGS can also be METH_VARARGS | METH_KEYWORDS
			{"method", base_ORCA_python_method, METH_VARARGS,"Exectute ORCA"},
			{NULL,NULL,0,NULL}//sentinel
		};


		PyMODINIT_FUNC init_ORCA(void){
		    PyObject *m;

		    m = Py_InitModule("ORCA", ORCAMethods);
		    if (m == NULL)
		        return;

		    ORCAError = PyErr_NewException("ORCA.error", NULL, NULL);
		    Py_INCREF(ORCAError);
		    PyModule_AddObject(m, "error", ORCAError);
		}


	}

}



#endif //File Guardian