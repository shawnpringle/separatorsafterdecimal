/** sdn.h - Contains declarations of display and parsing of standard decimal notation numbers
 **/
#pragma once
#include <stdint.h>
#include <stdlib.h>
#include <stddef.h>
#ifdef SDN_BUILD_DLL
#define SDN_API __declspec(dllexport)
#else
#define SDN_API __declspec(dllimport)
#endif
/** These routines each write a number to a string using standard decimal notation.  This means, English/American digits and decial places,
 * with group seperators on either side of the decimal point.  
 *
 * The number returned is the number of bytes written except for the null character. 
 * These routines write a null character at the end of the string.
**/
int SDN_API snprinticomma( char * buffer, size_t buffer_length, long int n );
int SDN_API snprintgcomma( char * buffer, size_t buffer_length, double f );
int SDN_API snprinti_bitcoin(char * buffer, size_t buffer_length, int64_t satoshis, unsigned short mandatory_decimal_places);
