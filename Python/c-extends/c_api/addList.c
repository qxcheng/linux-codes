#include <Python.h>

static PyObject* addList_add(PyObject* self, PyObject* args)
{
    PyObject * listObj;

    if (! PyArg_ParseTuple( args, "O", &listObj ))
        return NULL;

    long length = PyList_Size(listObj);

    int i, sum =0;
    for (i = 0; i < length; i++) 
    {
        PyObject* temp = PyList_GetItem(listObj, i);
        long elem = PyInt_AsLong(temp);
        sum += elem;
    }
    return Py_BuildValue("i", sum);
}

static char addList_docs[] =
"add(  ): add all elements of the list\n";

static PyMethodDef addList_funcs[] = 
{
    {"add", (PyCFunction)addList_add, METH_VARARGS, addList_docs},
    {NULL, NULL, 0, NULL}
};

PyMODINIT_FUNC initaddList(void)
{
    Py_InitModule3("addList", addList_funcs,
            "Add all ze lists");
}
