#include <Python.h>
#define abs(x)  (((x) < 0) ? -(x) : (x))

#define WRITE_DIGITS(p)							\
	do {								\
		for (i = 0; i < size - 1; i++)				\
		{							\
			rem = pout[i];					\
			for (j = 0; j < _PyLong_DECIMAL_SHIFT; j++)	\
			{						\
				*--p = '0' + rem % 10;			\
				rem /= 10;				\
			}						\
		}							\
		rem = pout[i];						\
		do {							\
			*--p = '0' + rem % 10;				\
			rem /= 10;					\
		} while (rem != 0);					\
									\
		if (negative)						\
			*--p = '-';					\
	} while (0)

char *long_to_string(PyObject *aa)
{
	PyLongObject *scratch, *a;
	char *str = NULL, *p;
	Py_ssize_t size, strlen, size_a, i, j;
	digit *pout, *pin, rem, tenpow, hi;
	int negative, d;
	twodigits z;

	a = (PyLongObject *)aa, size_a = abs(Py_SIZE(a));
	negative = Py_SIZE(a) < 0;
	d = (33 * _PyLong_DECIMAL_SHIFT) /
		(10 * PyLong_SHIFT - 33 * _PyLong_DECIMAL_SHIFT);
	size = 1 + size_a + size_a / d,  scratch = _PyLong_New(size);
	if (scratch == NULL)
		return (NULL);
	pin = a->ob_digit, pout = scratch->ob_digit,  size = 0;
	for (i = size_a; --i >= 0; )
	{
		hi = pin[i];
		for (j = 0; j < size; j++)
		{
			z = (twodigits)pout[j] << PyLong_SHIFT | hi;
			hi = (digit)(z / _PyLong_DECIMAL_BASE);
			pout[j] = (digit)(z - (twodigits)hi * _PyLong_DECIMAL_BASE);
		}
		while (hi)
		{
			pout[size++] = hi % _PyLong_DECIMAL_BASE;
			hi /= _PyLong_DECIMAL_BASE;
		}
	}
	if (size == 0)
		pout[size++] = 0;
	strlen = negative + 1 + (size - 1) * _PyLong_DECIMAL_SHIFT;
	tenpow = 10, rem = pout[size - 1];
	while (rem >= tenpow)
		tenpow *= 10, strlen++;
	str = calloc(1, strlen);
	p = str + strlen;
	WRITE_DIGITS(p);
	/* PyMem_Free(scratch); */
	return (str);
}

char *long_to_string_2(PyObject *aa)
{
	PyLongObject *scratch, *a;
	Py_ssize_t size, size_a, i, j;
	digit *pout, *pin, hi;
	int negative, d;
	twodigits z;

	a = (PyLongObject *)aa, size_a = abs(Py_SIZE(a));
	negative = Py_SIZE(a) < 0;
	d = (33 * _PyLong_DECIMAL_SHIFT) /
		(10 * PyLong_SHIFT - 33 * _PyLong_DECIMAL_SHIFT);
	size = 1 + size_a + size_a / d,  scratch = _PyLong_New(size);
	if (scratch == NULL)
		return (NULL);
	pin = a->ob_digit, pout = scratch->ob_digit,  size = 0;
	for (i = size_a; --i >= 0; )
	{
		hi = pin[i];
		for (j = 0; j < size; j++)
		{
			z = (twodigits)pout[j] << PyLong_SHIFT | hi;
			hi = (digit)(z / _PyLong_DECIMAL_BASE);
			pout[j] = (digit)(z - (twodigits)hi * _PyLong_DECIMAL_BASE);
			/* printf("J :%lu Pout: %u\n", j, pout[j]); */
		}
		while (hi)
		{
			pout[size++] = hi % _PyLong_DECIMAL_BASE;
			hi /= _PyLong_DECIMAL_BASE;
		}
	}
	if (size == 0)
		pout[size++] = 0;
	if (negative)
		putchar('-');
	for (i = size; --i >= 0;) {
		if (i == size - 1)
			printf("%u", pout[i]);
		else
			printf("%09u", pout[i]);
	}
	printf("\n");
	/* printf("****************************************\n"); */
	return (NULL);
}

/**
 * print_python_int - prints info about python int object
 * @p: Python generic object
 */
void print_python_int(PyObject *p)
{
	char *str;

	if (!p || !PyLong_Check(p))
	{
		printf("Invalid Int Object\n");
		return;
	}
	str = long_to_string_2(p);
	/* printf("%s\n", str); */
	free(str);
}
