/*
fact语言的内存管理
*/
#ifndef __FACT_MM__
#define __FACT_MM__
#ifdef  __cplusplus
extern "C" {
#endif //  __cplusplus
	#pragma once
	/// <summary>
	/// 申请内存函数类型
	/// </summary>
	typedef void* (*fact_mm_aquire_t)(size_t size);
	/// <summary>
	/// 释放内存函数类型
	/// </summary>
	typedef void (*fact_mm_release_t)(void* p);

	/// <summary>
	/// 通用的内存管理器
	/// </summary>
	typedef struct fact_mm_t {
		fact_mm_aquire_t aquire;
		fact_mm_release_t release;
	} fact_mm;

	typedef struct fact_mm_opts_t {
		size_t pageSize;
	} fact_mm_opts;

	
	typedef struct fact_mm_sys_t {
		struct fact_mm_t;
	} fact_mm_sys;

	void* fact_mm_sys_aquire(size_t size);
	void fact_mm_sys_release(void* p);

	typedef struct fact_mm_static_t {
		struct fact_mm_t;

	} fact_mm_static;


	inline void* fact_mm_static_aquire(size_t size) {
		return fact_mm_sys_aquire(size);
	}
	inline void fact_mm_static_release(void* p) {
		
	}

	fact_mm_static* fact_mm_static_instance();






#ifdef  __cplusplus
} // enn extern C
#endif //  __cplusplus

#endif // !__FACT_MM__