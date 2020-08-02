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
typedef long int positive_int;
int SDN_API snprinticomma( char * buffer, size_t buffer_length, positive_int n );
int SDN_API snprintgcomma( char * buffer, size_t buffer_length, double f );
int SDN_API snprinti_bitcoin(char * buffer, size_t buffer_length, int64_t satoshis, unsigned short mandatory_decimal_places);
