#include <malloc.h>
#include "mm.h"
#include "gc.h"

void* fact_mm_GC_aquire(size_t size) {
	return malloc(size);
}

void fact_mm_GC_release(void* p) {
	free(p);
}