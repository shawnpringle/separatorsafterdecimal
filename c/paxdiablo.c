// See: https://stackoverflow.com/questions/1449805/how-to-format-a-number-from-1123456789-to-1-123-456-789-in-c

/**************************************************************************************************************************************
 Program tests the routines for formatting numbers.  These routines are : snprinticomma: for long int, and snprintgcomma for doubles.
 Should normally produce no output.  Output indicates something is not working properly.
**************************************************************************************************************************************/


#include "sdn.h"
#include <string.h>
#include <stdio.h>

struct dtest_case {
	double in;
	char * result;
};


int main (void) {
	char experiment[100];
	size_t  rlen;
	const int x[] = {
		-1234567890, -123456, -12345, -1000, -999, -1,
		0, 1, 999, 1000, 12345, 123456, 1234567890			};
	const char* r[] = {
		"-1,234,567,890", "-123,456", "-12,345", "-1,000", "-999", "-1", "0", "1", "999",
		"1,000", "12,345", "123,456", "1,234,567,890" 			};
	const struct dtest_case gx[] = { {3.141592, "3.141,592"}, {-0.22222222L,"-0.222,222,22"} };


	int *px = x;
	char **pr = r;
	while (px != &(x[sizeof(x)/sizeof(*x)])) {
		rlen = snprinticomma(experiment, 100, *px);
		if (strcmp(experiment, *pr)) {
			printf ("%-15d: Expected %s but got %s\n", *px, *pr, experiment);
		}
		if (strlen(experiment) != rlen) {
		   printf("%-15d: Returned value from routine does not report the correct written length."
		   "  Should be %d but got %d\n", *px, strlen(experiment), rlen);
		}
		pr++;
		px++;
	}

	struct dtest_case * pd = gx;
	while (pd != &(gx[sizeof(gx)/sizeof(*gx)])) {
		rlen = snprintgcomma(experiment, 100, pd->in);
		if (strcmp(experiment, pd->result)) {
			printf ("%-15g: Expected %s but got %s\n", pd->in, pd->result, experiment);
		}
		if (strlen(experiment) != rlen) {
		   printf("%-15g: Returned value from routine does not report the correct written length."
		   "  Should be %d but got %d\n", pd->in, strlen(experiment), rlen);
		}
		pd++;
	}

	return 0;
}
