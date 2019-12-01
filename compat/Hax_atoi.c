#ifdef HAX_FREESTANDING
#include "Hax_stdlib.h"
#include "Hax_ctype.h"

int Hax_atoi(const char *s)
{
	int n=0, neg=0;
	while (Hax_isspace(*s)) s++;
	switch (*s) {
	case '-': neg=1;
	case '+': s++;
	}
	/* Compute n as a negative number to avoid overflow on INT_MIN */
	while (Hax_isdigit(*s))
		n = 10*n - (*s++ - '0');
	return neg ? n : -n;
}
#endif
