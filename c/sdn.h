// sdn.h - Contains declarations of display and parsing of standard decimal notation numbers
#pragma once
#include <stdint.h>
#include <stdlib.h>
#include <stddef.h>
#define SDN_API __declspec(dllexport)
typedef long int positive_int;
int snprinticomma( char * buffer, int buffer_length, positive_int n );
int snprintgcomma( char * buffer, int buffer_length, double f );
int snprinti_bitcoin(char * buffer, int buffer_length, int64_t satoshis);