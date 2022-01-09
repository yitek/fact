#include <stdio.h>
#include <stdlib.h>
#include <malloc.h>
#include "platform.h"

void* fact_alloc(size_t size) {
	return malloc(size);
}
void fact_free(void* p) {
	free(p);
}
void* fact_file_open(const char* filename, const char * mode) {
	return fopen(filename, mode);
}
int fact_file_close(void* file) {
	return fclose(file);
}
int fact_file_read(void* file, char* buffer, size_t size) {
	fread(buffer, 1, size, file);
}
int fact_file_write(void* file, char* buffer, size_t size);
void fact_stdout(const char* message);
int fact_stdin(const char* buffer);
