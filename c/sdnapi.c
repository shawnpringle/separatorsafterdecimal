
#include <stdio.h>
#include <limits.h>
#include <string.h>
#include <math.h>
#include <float.h>
#include <limits.h>
#include "sdn.h"
#ifdef WIN32__
#include <windows.h>
BOOL WINAPI DllMain(
  _In_ HINSTANCE hinstDLL,
  _In_ DWORD     fdwReason,
  _In_ LPVOID    lpvReserved
) {
	return TRUE;
}
#endif
#include <assert.h>

#ifndef INT64_MAX
#define INT64_MAX LONG_MAX
typedef long int int64_t;
#endif

typedef long int positive_int;

/* Writes a positive long int number (not an unsigned long int) to buffer as
 * a string.  The number of non-null characters written is
 * returned.  A null byte will be written to the end of the string.
 
 * If the buffer is NULL or there is not enough size indicated by slen,
 * it may be partially written and the number of characters that would
 * have been written is returned.
 */
static int snprintpicomma( char * buffer, size_t slen, positive_int n ) {
	int left_side, right_side;
	if (n < 1000) {
		// reported length includes NULL byte or not?
		return snprintf(buffer, slen, "%d", n);
	}
	if ((left_side = snprintpicomma (buffer, slen, n/1000)) < 0) {
		return left_side;
	}
	if (left_side > slen)
		right_side = 4;
	else
		right_side = snprintf (&buffer[left_side], slen-left_side, ",%03d", n%1000);
	if (right_side < 0)	
		return right_side;
	return left_side + right_side;
}

int snprinticomma ( char * buffer, size_t slen, long int n) {
	int result;
	if (n < 0) {
		if (slen < 2) {
			return 1 + snprintpicomma(buffer, 0, -n);
		}
		*buffer = '-';
		if (n == LONG_MIN) {
			return -1;
		}
		result = snprintpicomma (&buffer[1], slen-1, -n);
		return result < 0 ? result : result + 1;
	}
	return snprintpicomma(buffer, slen, n);
}


static void swap(char * a, char * b) {
	char c = *a;
	*a = *b;
	*b = c;
}

static void reverse(char * buffer) {
	int len = strlen(buffer);
	for (int i = 0; i < len/2; ++i) {
		swap(&buffer[i], &buffer[len-1-i]);
	}
}

static short min(short a, short b) {
	if (a<b)
		return a;
	else
		return b;
}



int snprintgcomma( char * buffer, size_t slen, double n ) {
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
	
	#define can_do (buffer!=NULL && slen>=1)
	
	/* generate our limits if necessary */
	if (l000_power_g_cap == 1) {
		const double our_max = DBL_MAX / 1000;
		g = 0;
		l000tog = 1;
		while (1) {
			if (l000tog > our_max) {
				break;
			}
			++g;
			l000tog *= 1000;
		}
		l000_power_g_cap = l000tog;
		g_cap = g;
	}
	
	if (n > 1e17) {
		int buffer_length = (log(n) / log(1000))*4 + 1;
		if (slen < buffer_length || buffer == NULL)
			return buffer_length;
	}
	
	
	if (slen < 28 || buffer == NULL) {
		char buf2[100];
		int result = snprintgcomma(buf2, 100, n);
		if (buffer != NULL) {
			strncpy(buffer, buf2, slen > result ? result + 1 : slen);
		}
		return result;
	}
	
	/* handle negative case */
	if (n < 0) {
		int result;
		*buffer = '-';
		result = snprintgcomma(&buffer[1], slen-1, -n);
		return result < 0 ? result : result + 1;
	}
	
	
	/* For the first group: */
	l000tog = l000_power_g_cap;
	for (g = g_cap; g >= 0; --g, l000tog/=1000.0) {

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
	
	if (digit_count >= digit_count_max || slen < 2 || n == 0) {
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
	
	*p = '\0';
	return p - buffer;
}


/* 10**8 */
const long double SATOSHISPERCOIN = 100000000;

int snprinti_bitcoin(char * buffer, size_t slen, int64_t n, unsigned short mandatory_decimal_places) {
	int64_t power_10_order = SATOSHISPERCOIN*SATOSHISPERCOIN;
	short order = 8;
	short total_bytes = 0;
	char * p = buffer;
	for (;order > 0; --slen, --order, (power_10_order/=10)) {
		short expansion = n/power_10_order;
		if (expansion != 0)
			break;
	}
	for (;order >= 0; --slen, --order, (power_10_order/=10), ++p, total_bytes++) {
		short expansion = n/power_10_order;
		*p = (char)expansion + '0';
		n -= expansion * power_10_order;
		if (order && order % 3 == 0) {
			++p;
			*p = ',';
			++total_bytes;
			--slen;
		}
	}
		
	if (n==0 && mandatory_decimal_places==0) {
		*p = '\0';
		++p;
		return p - buffer - 1;
	}
	
	*p = '.';
	++p;
	
	int dumb_cnt = -3;
	for (;(n > 0 || mandatory_decimal_places!=0) && order >= -8; --slen, --order, (power_10_order/=10), ++p, ++total_bytes) {
		if (mandatory_decimal_places)
			mandatory_decimal_places--;
		if (++dumb_cnt % 3 == 1) {
			*p = ',';
			++p;
			--slen;
		}
		short expansion = n/power_10_order;
		*p = (char)expansion + '0';
		n -= expansion * power_10_order;
	}
	
	*p = '\0';
	++p;
	return p - buffer - 1;
}
