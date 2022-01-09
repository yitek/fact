#include <malloc.h>
#include "mm.h"

static fact_mm_Static staticInstance = {
	.aquire=&fact_mm_Static_aquire,
	.release = &fact_mm_Static_release
};
fact_mm_Static* fact_mm_Static_instance() {
	return &staticInstance;
}