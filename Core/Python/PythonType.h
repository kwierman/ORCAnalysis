#ifndef Deuterium_PythonType_h_
#define Deuterium_PythonType_h_

#include <Python.h>
//Can also get from prefix/include/pythonversion/ and exec_prefix/include/pythonversion/



//All user visible names in Python are prefixed with Py or _Py. Names beginning with _Py or for internal use ony.
//Users should never define names beginning with that prefix

namespace Deuterium{

	namespace Python{

		//Python opaque objects are called PyObjects
		//Never declare PyObject on the stack. Always on heap

		static PyObject* DeuteriumError;

		static PyObject* base_deuterium_python_method(PyObject* self, PyObject* args){
			const char* command;
			int sts;

			if(!PyArg_ParseTuple(args, "s", &command) )
				return NULL;
			sts = system(command);
			if(sts<0)
				PyErr_SetString(DeuteriumError, "Deuterium Command Failed");
			return Py_BuildValue("i", sts);
		}

		//Errors and Exceptions

		static PyMethodDef DeuteriumMethods[]={
			//METH_VARARGS can also be METH_VARARGS | METH_KEYWORDS
			{"method", base_deuterium_python_method, METH_VARARGS,"Exectute Deuterium"},
			{NULL,NULL,0,NULL}//sentinel
		};


		PyMODINIT_FUNC init_deuterium(void){
		    PyObject *m;

		    m = Py_InitModule("deuterium", DeuteriumMethods);
		    if (m == NULL)
		        return;

		    DeuteriumError = PyErr_NewException("Deuterium.error", NULL, NULL);
		    Py_INCREF(DeuteriumError);
		    PyModule_AddObject(m, "error", DeuteriumError);
		}


	}

}



#endif //File Guardian