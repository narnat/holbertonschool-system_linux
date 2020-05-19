#include <Python.h>
#include <stdio.h>
#define PyListCast(op) ((PyListObject *)(op))
#define PyBytesCast(op) ((PyBytesObject *)(op))
#define PyVarObjectCast(op) ((PyVarObject *)(op))

/**
 * print_python_bytes - prints info about python bytes object
 * @p: Python generic object
 */
void print_python_bytes(PyObject *p)
{
	Py_ssize_t size, i;
	char *str;

	printf("[.] bytes object info\n");
	if (!p || !PyBytes_Check(p))
	{
		printf("  [ERROR] Invalid Bytes Object\n");
		return;
	}
	size = PyBytes_Size(p);
	str = PyBytesCast(p)->ob_sval;
	printf("  size: %ld\n", size);
	printf("  trying string: %s\n", str);
	size = size > 10 ? 10 : size + 1;
	printf("  first %ld bytes:", size);
	for (i = 0; i < size; ++i)
	{
		printf(" %02hhx", str[i]);
	}
	putchar('\n');
	fflush(stdout);
}

/**
 * print_python_float - prints info about python float object
 * @p: Python generic object
 */
void print_python_float(PyObject *p)
{
	char *s1;

	printf("[.] float object info\n");
	if (!p || !PyFloat_Check(p))
	{
		printf("  [ERROR] Invalid Float Object\n");
		return;
	}
	s1 = PyOS_double_to_string(PyFloat_AsDouble(p), 'r',
				   0, Py_DTSF_ADD_DOT_0, NULL);
	printf("  value: %s\n", s1);
	fflush(stdout);
	PyMem_Free(s1);
}

/**
 * print_python_list - prints info about python list object
 * @p: Python generic object
 */
void print_python_list(PyObject *p)
{
	Py_ssize_t i, size;
	PyObject *item;

	if (!p || !PyList_Check(p))
	{
		printf("  [ERROR] Invalid List Object\n");
		return;
	}

	size = PyVarObjectCast(p)->ob_size;
	printf("[*] Python list info\n");
	printf("[*] Size of the Python List = %ld\n", size);
	printf("[*] Allocated = %ld\n", PyListCast(p)->allocated);
	for (i = 0; i < size; ++i)
	{
		item = PyList_GET_ITEM(p, i);
		printf("Element %ld: %s\n", i, item->ob_type->tp_name);
		if (PyBytes_Check(item))
			print_python_bytes(item);
		else if (PyFloat_Check(item))
			print_python_float(item);
	}
	fflush(stdout);
}
