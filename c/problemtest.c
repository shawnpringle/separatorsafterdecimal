#include <stdio.h>
#include <locale.h>
#include <string.h>


int main(int argc, char ** argv) {
	char buffer[100];
    setlocale(LC_NUMERIC, "en_US");
	snprintf(buffer, 100, "%'d", 100000);
	if (strcmp(buffer, "100,000")) {
    	printf("Expected '0.001,01' but got %s\n", buffer);
	}
	snprintf(buffer, 100, "%'f", 0.00101); // 0.001,01
    if (strcmp(buffer, "0.001,01")) {
    	printf("Expected '0.001,01' but got %s\n", buffer);
    }
	snprintf(buffer, 100, "%'g", 0.000001);
    if (strcmp(buffer, "0.001,001")) {
    	printf("Expected '0.001,001' but got %s\n", buffer);
    }
	snprintf(buffer, 100, "%'g", 0.00000001);
    if (strcmp(buffer, "0.001,000,01")) {
    	printf("Expected '0.001,000,01' but got %s\n", buffer);
    }
    return 0;
}
