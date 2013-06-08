#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define STR(x)        #x
#define BIN_NAME(N) "data/" STR(N) "kb.dat"
#define KB 1048576

int main(int argc, char** argv) {
	char bin_name[] = BIN_NAME(KB);
	long num_kbytes = KB;
	FILE *file = fopen(bin_name, "w");
	int str_len = 1024;
	char string[str_len];
	long i;
	for(i=0; i < str_len; i++) {
		string[i] = i%26 + 'a';
	}
	for(i=0; i<num_kbytes; i++) {
		fwrite(string, sizeof(char), str_len, file);
	}
	fclose(file);

	return 0;
}