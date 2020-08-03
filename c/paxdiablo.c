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

int snprintdoublecomma( char * buffer, size_t slen, double n ) {
	static long double l000_power_g_cap = 1;
	static short g_cap;
	short g = 0;
	long double l000tog = l000_power_g_cap;
	char * p = buffer;
	int byte_count = 0;
	int these_characters, these_digits;
	
	/* the total number of bytes printed out */
	int total_bytes = 0;
	
	/* how many groups were printed out */
	int digit_count = 0;
	
	/* after group_count_max more is just noise not in the original bits of the number even for long doubles*/
	int digit_count_max = 16;
	
	memset(buffer, 0, slen);
	
	/* generate our limits if necessary */
	if (l000_power_g_cap == 1) {
		for (int i = 0; i < 
	#ifdef LDBL_MAX_10_EXP
		LDBL_MAX_10_EXP
	#elif defined(DBL_MAX_10_EXP)
		DBL_MAX_10_EXP
	#else
		37
	#endif
	-3; i+=3) {
			l000_power_g_cap *= 1000;
			g_cap += 1;
			/*printf("10**%d\n", i);*/
		}
	}
	
	/* handle negative case */
	if (n < 0) {
		if (slen < 2)  return -1;
		*buffer = '-';
		int result = snprintdoublecomma(&buffer[1], slen-1, -n);
		return result < 0 ? result : result + 1;
	}
	
	
	/* For the first group: */
	l000tog = l000_power_g_cap;
	for (g = g_cap; g >= 0; --g, l000tog/=1000.0) {

		/*printf("10**(3*%d)\n", g);*/
		if ((these_digits = n/l000tog) != 0 || (g == 0)) {
			if ((byte_count = snprintf(p,  slen, "%d", these_digits)) < 0) {
					return byte_count;
			}
			slen -= byte_count;
			p += byte_count;
			total_bytes += byte_count;
			
			n -= these_digits * l000tog;
			l000tog /= 1000.0;
			--g;
			digit_count = byte_count;
			
			break;
		}
	}
	assert(total_bytes);
	
	/* For subsequent groups */
	
	for (; g >= 0; --g, l000tog/=1000) {
		these_digits = (int)(n/l000tog);
		n -= these_digits * l000tog;
		byte_count = snprintf(p, slen, ",%03d", these_digits);
		// should always be 4
		if (byte_count != 4) {
			if (byte_count >= 0) {
				printf("byte count=%d, digits=%d, string so far %s\n", byte_count, these_digits, buffer);
			}
			assert(byte_count < 0);
			return byte_count;
		}
		slen -= byte_count;
		total_bytes += 4;
		p += 4;
		digit_count += 3;
	}
	assert(g == -1);
	assert(l000tog < 1);
	
	if (digit_count >= digit_count_max || slen < 2) {
		return total_bytes;
	}

	short decimal_digits = digit_count_max - digit_count;
	short exp10 = 0;
	short offset = decimal_digits % 3;
	l000tog = 1;
	g = 0;
	for (int i = 3; i < decimal_digits; i+=3) {
		l000tog *= 1000.0;
		exp10 += 3;
		g += 1;
	}
	n *= l000tog * 1000;
	exp10 += 3;
	++g;
	
	*p = '.';
	--slen;
	++total_bytes;
	++p;
	
	
	int l0tok = 1000;
	while (n >= 1 && slen > 0 && digit_count < digit_count_max) {
		if (l0tok == 1) {
			*p++ = ',';
			l0tok = 100;
			l000tog /= 1000;
			++total_bytes;
		} else {
			l0tok /= 10;
		}
		--slen;
		these_digits = n / (l0tok*l000tog);
		char next_char = these_digits + '0';
		++digit_count;
		++total_bytes;
		*p = next_char;
		n -= these_digits * (l0tok*l000tog);
		++p;
	}
	
	return total_bytes;
}



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
    	

	const int *px = x;
	const char **pr = r;
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

	const struct dtest_case * pd = gx;
	while (pd != &(gx[sizeof(gx)/sizeof(*gx)])) {
		rlen = snprintdoublecomma(experiment, 100, pd->in);
		if (strcmp(experiment, pd->result)) {
			printf ("%-15g: Expected %s but got %s\n", pd->in, pd->result, experiment);
		}
		if (strlen(experiment) != rlen) {
		   printf("%-15g: Returned value from routine does not report the correct written length."
		   "  Should be %d but got %d\n", pd->in, strlen(experiment), rlen);
		}
		pd++;
	}

	const struct btctest_case * pb = btc;
	while (pb != &(btc[sizeof(btc)/sizeof(*btc)])) {
		rlen = snprinti_bitcoin(experiment, 100, pb->in, 0);
		if (strcmp(experiment, pb->result0)) {
			printf ("%-15llu: Expected %s but got %s\n", pb->in, pb->result0, experiment);
		}
		if (strlen(experiment)+1 != rlen) {
		   printf("%-15llu: Returned value from routine does not report the correct written length."
		   "  Should be %d but got %d\n", pb->in, strlen(pb->result0)+1, rlen);
		}

		rlen = snprinti_bitcoin(experiment, 100, pb->in, 3);
		if (strcmp(experiment, pb->result3)) {
			printf ("%-15llu: Expected %s but got %s\n", pb->in, pb->result3, experiment);
		}
		if (strlen(experiment)+1 != rlen) {
		   printf("%-15llu: Returned value from routine does not report the correct written length."
		   "  Should be %d but got %d\n", pb->in, strlen(pb->result3)+1, rlen);
		}

		rlen = snprinti_bitcoin(experiment, 100, pb->in, 8);
		if (strcmp(experiment, pb->result8)) {
			printf ("%-15llu: Expected %s but got %s\n", pb->in, pb->result8, experiment);
		}
		if (strlen(experiment)+1 != rlen) {
		   printf("%-15llu: Returned value from routine does not report the correct written length."
		   "  Should be %d but got %d\n", pb->in, strlen(pb->result8)+1, rlen);
		}
		pb++;
	}
	
	return 0;
}
