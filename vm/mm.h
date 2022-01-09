/*
fact语言的内存管理
*/
#ifndef __FACT_MM__
#define __FACT_MM__
#include "base.h"
#ifdef  __cplusplus
extern "C" {
#endif //  __cplusplus
	#pragma once
	

	

	/// <summary>
	/// 通用的内存管理器
	/// </summary>
	typedef struct fact_mm_t {
		fact_aquirem_t aquire;
		fact_releasem_t release;
	} fact_mm;


	typedef struct fact_mm_opts_t {
		size_t pageSize;
	} fact_mm_opts;

	




	typedef struct fact_mm_Static_t {
		struct fact_mm_t;

	} fact_mm_Static;


	inline void* fact_mm_Static_aquire(size_t size) {
		return fact_aquirem(size);
	}
	inline void fact_mm_Static_release(void* p) {
		
	}

	fact_mm_Static* fact_mm_Static_instance();






#ifdef  __cplusplus
} // enn extern C
#endif //  __cplusplus

#endif // !__FACT_MM__