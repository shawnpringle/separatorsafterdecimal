// See: https://stackoverflow.com/questions/1449805/how-to-format-a-number-from-1123456789-to-1-123-456-789-in-c

/**************************************************************************************************************************************
 Program tests the routines for formatting numbers.  These routines are : snprinticomma: for long int, and snprintgcomma for doubles.
 Should normally produce no output.  Output indicates something is not working properly.
**************************************************************************************************************************************/


#include "sdn.h"
#include <string.h>
#include <stdio.h>

#include <assert.h>
#include <float.h>
#include <limits.h>
struct dtest_case {
	double in;
	char * result;
};

struct btctest_case {
	uint64_t in;
	char * result0;
	char * result3;
	char * result8;
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
	
	const struct dtest_case gx[] = { {3.141592, "3.141,592"}, {-0.22222222L,"-0.222,222,22"}, {0.123456, "0.123,456" }, 
	{0.12345, "0.123,45" }, {0.1234, "0.123,4" }, {100000000000000000.0, "100,000,000,000,000,000"}, {0.1, "0.1"} };
    const struct btctest_case btc[] = {    	
    	{10, "0.000,000,1", "0.000,000,1", "0.000,000,10"},
    	{1000000000000L, "10,000", "10,000.000", "10,000.000,000,00"},
    	{0, "0", "0.000", "0.000,000,00"}, 
    	{1, "0.000,000,01", "0.000,000,01", "0.000,000,01"},
    	{100,"0.000,001", "0.000,001", "0.000,001,00"}, 
    	{10000, "0.000,1", "0.000,1", "0.000,100,00"},
    	{5000000LL, "0.05", "0.050", "0.050,000,00"},
    	{10000000LL, "0.1", "0.100", "0.100,000,00"},
    	{314159200, "3.141,592", "3.141,592", "3.141,592,00"}
    };
    	

    int buffer_not_overran_flag = 1;
    int required_size_not_wrong_flag = 1;
    int result_string_not_wrong_flag = 1;
    for (int len = 100; len >= 0; len -= 20) {
		const int *px = x;
		const char **pr = r;
		
		while (px != &(x[sizeof(x)/sizeof(*x)])) {
			memset(experiment, 'A', 99);
			experiment[99] ='\0';
			rlen = snprinticomma(experiment, len, *px);
			for (int i = rlen+1; i < 99; ++i) {
				if (experiment[i] != 'A') {
					printf("%-15d:Buffer overrun for case %s with length %d.  Byte written in position %d.  Data should have been only written up to %d.\n", *px, *pr, len, i, rlen+1);
					printf("%s\n", experiment);
					buffer_not_overran_flag = 0;
					break;
				}
			}
			if (buffer_not_overran_flag && rlen < len && strcmp(experiment, *pr)) {
				printf ("%-15d: Expected %s but got %s\n", *px, *pr, experiment);
			}
			if (buffer_not_overran_flag && strlen(*pr) != rlen) {
			   printf("%-15d: Returned value from routine does not report the correct written length."
			   "  Passed in %d.  Should be %d but got %d\n", *px, len, strlen(*pr), rlen);
			   required_size_not_wrong_flag = 0;
			}
			pr++;
			px++;
		}
		
		
	
		const struct dtest_case * pd = gx;
		while (pd != &(gx[sizeof(gx)/sizeof(*gx)])) {
			memset(experiment, 'A', 99);
			experiment[99] ='\0';
			rlen = snprintgcomma(experiment, len, pd->in);
			for (int i = rlen+1; i < 99; ++i) {
				if (experiment[i] != 'A') {
					printf("%-15g:Buffer overrun for case %s '%s' with length %d.  Byte written in position %d.  Data should have been only written up to %d.\n", pd->in, pd->result, experiment, len, i, rlen);
					buffer_not_overran_flag = 0;
					break;
				}
			}
			if (buffer_not_overran_flag && strcmp(experiment, pd->result)) {
				printf ("%-15g: Expected %s but got %s\n", pd->in, pd->result, experiment);
			}
			if (buffer_not_overran_flag && strlen(experiment) != rlen) {
			   printf("%-15g: Returned value from routine does not report the correct written length."
			   "  Should be %d but got %d\n", pd->in, strlen(experiment), rlen);
			   required_size_not_wrong_flag = 0;
			}
			pd++;
		}
	
		const struct btctest_case * pb = btc;
		while (pb != &(btc[sizeof(btc)/sizeof(*btc)])) {
			memset(experiment, 'A', 99);
			experiment[99] ='\0';
			rlen = snprinti_bitcoin(experiment, len, pb->in, 0);
			for (int i = rlen+1; i < 99; ++i) {
				if (experiment[i] != 'A') {
					printf("%-15d:Buffer overrun for case %s '%s' with length %d.  Byte written in position %d.  Data should have been only written up to %d.\n", pb->in, pb->result0, experiment, len, i, rlen);
					printf("%s\n", experiment);
					buffer_not_overran_flag = 0;
					break;
				}
			}
			if (buffer_not_overran_flag && strcmp(experiment, pb->result0)) {
				printf ("%-15llu: Expected %s but got %s\n", pb->in, pb->result0, experiment);
			}
			if (buffer_not_overran_flag && strlen(experiment) != rlen) {
			   printf("%-15llu: Returned value from routine does not report the correct written length."
			   "  Should be %d but got %d\n", pb->in, strlen(pb->result0), rlen);
			   required_size_not_wrong_flag = 0;
			}
	
			memset(experiment, 'A', 99);
			experiment[99] ='\0';
			rlen = snprinti_bitcoin(experiment, len, pb->in, 3);
			for (int i = rlen+1; i < 99; ++i) {
				if (experiment[i] != 'A') {
					printf("%-15llu:Buffer overrun for case %s '%s' with length %d.  Byte written in position %d.  Data should have been only written up to %d.\n", pb->in, pb->result3, experiment, len, i, rlen);
					buffer_not_overran_flag = 0;
					break;
				}
			}
			if (buffer_not_overran_flag && strcmp(experiment, pb->result3)) {
				printf ("%-15llu: Expected %s but got %s\n", pb->in, pb->result3, experiment);
			}
			if (buffer_not_overran_flag && strlen(experiment) != rlen) {
			   printf("%-15llu: Returned value from routine does not report the correct written length."
			   "  Should be %d but got %d\n", pb->in, strlen(pb->result3), rlen);
			   required_size_not_wrong_flag = 0;
			}
	
			memset(experiment, 'A', 99);
			experiment[99] ='\0';
			rlen = snprinti_bitcoin(experiment, len, pb->in, 8);
			for (int i = rlen+1; i < 99; ++i) {
				if (experiment[i] != 'A') {
					printf("%-15llu:Buffer overrun for case %s '%s' with length %d.  Byte written in position %d.  Data should have been only written up to %d.\n", pb->in, pb->result8, experiment, len, i, rlen);
					buffer_not_overran_flag = 0;
					break;
				}
			}
			if (buffer_not_overran_flag && strcmp(experiment, pb->result8)) {
				printf ("%-15llu: Expected %s but got %s\n", pb->in, pb->result8, experiment);
			}
			if (buffer_not_overran_flag && strlen(experiment) != rlen) {
			   printf("%-15llu: Returned value from routine does not report the correct written length."
			   "  Should be %d but got %d\n", pb->in, strlen(pb->result8), rlen);
				required_size_not_wrong_flag = 0;
			}
			pb++;
		}
	}
	return 0;
}
