#include <malloc.h>
#include <stdio.h>
#include "base.h"

void* fact_aquirem(size_t size) {
	return malloc(size);
}

void fact_releasem(void* p) {
	free(p);
}


void* fact_openf(char* filename,char* mode) {
	return fopen(filename, mode);
}
size_t fact_writeFile(void* stream, void* data,size_t size) {
	return fwrite(data, 1, size, stream);
}
static fact_write_t fact_writef = &fact_writeFile;

size_t fact_readFile(void* stream, void* buffer, size_t size) {
	return fread(buffer, 1, size, stream);
}

static fact_read_t fact_readf = fact_readFile;
void fact_closef(void* file) {
	fclose((FILE*)file);
}
