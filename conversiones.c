#include "conversiones.h"

char *utoa(unsigned int x, char *buf, char terminador)
{
	char *ptr = buf;
	char *p = ptr;
	char *p1;
	char c;

	if (!x)
		*ptr++ = '0';
	else
	{
		// save start pointer
		p = ptr;

		// convert (reverse order)
		while (x)
		{
			*p++ = '0' + x % 10;
			x /= 10;
		}

		// save end pos
		p1 = p;

		// reverse result
		while (p > ptr)
		{
			c = *--p;
			*p = *ptr;
			*ptr++ = c;
		}

		// restore end pos
		ptr = p1;
	}

	*ptr++ = terminador;
	return ptr;
}

#define MAX_PRECISION	(10)
static const double rounders[MAX_PRECISION + 1] =
{
	0.5,				// 0
	0.05,				// 1
	0.005,				// 2
	0.0005,				// 3
	0.00005,			// 4
	0.000005,			// 5
	0.0000005,			// 6
	0.00000005,			// 7
	0.000000005,		// 8
	0.0000000005,		// 9
	0.00000000005		// 10
};

char *ftoa(double f, char *buf, unsigned int precision, char terminador)
{
	char *ptr = buf;
	char *p = ptr;
	char *p1;
	char c;
	long intPart;

	// check precision bounds
	if (precision > MAX_PRECISION)
		precision = MAX_PRECISION;
	if (precision == 0)
		precision = 1;

	// sign stuff
	if (f < 0)
	{
		f = -f;
		*ptr++ = '-';
	}

	// round value according the precision
	f += rounders[precision];

	// integer part...
	intPart = f;
	f -= intPart;

	if (!intPart)
		*ptr++ = '0';
	else
	{
		// save start pointer
		p = ptr;

		// convert (reverse order)
		while (intPart)
		{
			*p++ = '0' + intPart % 10;
			intPart /= 10;
		}

		// save end pos
		p1 = p;

		// reverse result
		while (p > ptr)
		{
			c = *--p;
			*p = *ptr;
			*ptr++ = c;
		}

		// restore end pos
		ptr = p1;
	}

	// place decimal point
	*ptr++ = '.';

	// convert
	while (precision--)
	{
		f *= 10.0;
		c = f;
		*ptr++ = '0' + c;
		f -= c;
	}

	// terminating zero
	*ptr++ = terminador;

	return ptr;
}