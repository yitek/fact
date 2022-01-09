#include <malloc.h>
#include "mm.h"

void* fact_mm_sys_aquire(size_t size) {
	return malloc(size);
}

void fact_mm_sys_release(void* p) {
	free(p);
}
static fact_mm_static staticInstance;
fact_mm_static* fact_mm_static_instance() {
	// staticInstance.
	return &staticInstance;
}