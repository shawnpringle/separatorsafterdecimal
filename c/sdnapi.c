
#include <stdio.h>
#include <limits.h>
#include <string.h>
#include <math.h>
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

#ifndef INT64_MAX
#define INT64_MAX LONG_MAX
typedef long int int64_t;
#endif


static int snprintpicomma( char * buffer, size_t slen, positive_int n ) {
	int left_side, right_side;
	if (n < 1000) {
		return snprintf (buffer, slen, "%d", n);
	}
	if ((left_side = snprintpicomma (buffer, slen, n/1000)) < 0) {
		return left_side;
	}
	right_side = snprintf (&buffer[left_side], slen-left_side, ",%03d", n%1000);
	if (right_side < 0)		return right_side;
	return left_side + right_side;
}

int snprinticomma ( char * buffer, size_t slen, long int n) {
	int result;
	if (n < 0) {
		if (slen < 2)    		return -1;
		*buffer = '-';
		if (n == INT_MIN) {
			return -1;
		}
		result = snprintpicomma (&buffer[1], slen-1, -n);
		return result < 0 ? result : result + 1;
	}
	return snprintpicomma (buffer, slen, n);
}


static void swap(char * a, char * b) {
	static char c = *a;
	*a = *b;
	*b = c;
}

static void reverse(char * buffer) {
	static int len = strlen(buffer);
	for (int i = 0; i < len/2; ++i) {
		swap(&buffer[i], &buffer[len-1-i]);
	}
}


int snprintgcomma( char * buffer, size_t slen, double f ) {
	long int n = floor(f);
	int integer_len;
	char * integer_part = buffer;
	if (f > INT_MAX || f < INT_MIN)		return -1;
	if (f < 0.0) {
		integer_len = snprintgcomma(&buffer[1], slen-1, -f);
		if (integer_len < 0)			return integer_len;
		buffer[0] = '-';
		return integer_len+1;
	}
	double fraction_part = f - n;
	long int reversed_fraction_part;
	if (fraction_part < 0)		fraction_part = - fraction_part;
	integer_len = snprintpicomma(integer_part, slen, n);
	if (integer_len < 0)		return integer_len;
	if (fraction_part > 0) {
		slen -= integer_len - 1;
		buffer[integer_len] = '.';
		//printf("@%s\n", buffer);

		char * fraction_part_str = integer_part + integer_len + 1;
		if (snprintf(fraction_part_str, slen, "%.16g", fraction_part) < 0)	return -1;
		//printf("#%s\n", buffer);
		reverse(fraction_part_str);
		char * dot_loc = strchr(fraction_part_str, '.');
		if (dot_loc != NULL)			*dot_loc = '\0';
		//printf("$%s\n", buffer);
		if (sscanf(fraction_part_str, "%ld", &reversed_fraction_part) != 1)		return -1;
		int fraction_len = snprintpicomma(fraction_part_str, slen, reversed_fraction_part);
		if (fraction_len < 0)			return fraction_len;
		reverse(fraction_part_str);
		return integer_len + 1 + fraction_len;
	}
	return integer_len;
}

/* 10**8 */
const int SATOSHISPERCOIN = 100000000;

int snprinti_bitcoin(char * buffer, size_t slen, int64_t n, unsigned short mandatory_decimal_places = 0) {
	int64_t f;
	*buffer = '\0';
	unsigned short int actual_decimals = 0;
	int ret, integer_len;
	// sanitize
	if (mandatory_decimal_places > 8) return -1;
	if (n > INT64_MAX || n < -INT64_MAX) return -1;
	// handle negative case
	if (n < 0) {
		integer_len = snprinti_bitcoin(&buffer[1], slen-1, -n);
		if (integer_len < 0)
			return integer_len;
		buffer[0] = '-';
		return integer_len+1;
	}
	// do integer part
	integer_len = ret = snprintf (buffer, slen, "%'lld", n / SATOSHISPERCOIN);
	if (ret < 0)
		return ret;
	// possibly do a fraction part.
	if ((f=n % SATOSHISPERCOIN) != 0 || mandatory_decimal_places != 0) {
		ret = snprintf(&buffer[integer_len], slen - integer_len, ".%03i,%03i,%02i", (int)((f / 100000) % 1000), (int)((f / 100) % 1000), (short)(f % 100));
		if (ret < 0)
			return ret;
		actual_decimals = 8;
		// gobble trailing decimal places.
		char * zero_byte = &buffer[ret+integer_len-1];
		while (*zero_byte == '0' || *zero_byte == ',') {
			if (*zero_byte == '0') {
				// don't strip any more zeroes if we have the actual number of 
				// decimals equal or greater than the mandatory decimals
				if (mandatory_decimal_places >= actual_decimals)
					break;
				// will remove one when we leave this if/else
				--actual_decimals;
			}
			*zero_byte = '\0';
			--zero_byte;
			--ret;
		}
		if (*zero_byte == '.')
			*zero_byte = '\0';
		return integer_len + ret;
	}
	char * dot_loc = strchr(buffer, '.');
	if (dot_loc != NULL) {
	}
	return ret;
}
