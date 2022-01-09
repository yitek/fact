/*
fact语言的内存管理
*/
#ifndef __FACT_GC__
#define __FACT_GC__
#pragma once
#include "mm.h"

#ifdef  __cplusplus
extern "C" {
#endif //  __cplusplus

	

	typedef struct fact_mm_GC_t {
		struct fact_mm_t;

	} fact_mm_GC;


	void* fact_mm_GC_aquire(size_t size);
	inline void fact_mm_GC_release(void* p);







#ifdef  __cplusplus
} // enn extern C
#endif //  __cplusplus

#endif // !__FACT_GC__