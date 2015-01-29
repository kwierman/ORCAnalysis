#ifndef Deuterium_Python_Function_hh_
#define Deuterium_Python_Function_hh_
namespace Deuterium{
	namespace Python{
		//The C Function always has two arguments, the self argument and the arguments argument
		// It always returns a PyObject*
		static PyObject* spam_system(PyObject *self, PyObject *args){
			
		    const char *command;
		    int sts;

			//Extract the argument
		    if (!PyArg_ParseTuple(args, "s", &command))
		        return NULL;
		    //run the interior argument
		    sts = system(command);
			//Set error conditions
			if (sts < 0) {
		        PyErr_SetString(SpamError, "System command failed");
		        return NULL;
			}

		    //Build the return value
		    return Py_BuildValue("i", sts);
		}

	}
}


#endif //File Guardian


