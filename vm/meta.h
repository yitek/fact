/*
fact语言的元数据结构与操作
	Meta 元数据
	|- Type
	| |- Primary 基本数据 指令可以直接处理的byte,int等
	| |- Struct 结构体/Class 类(存储在动态堆上的结构体)
	| |- Bind 函数代理，带着上下文
	| |- Pointer 指针
	| | |- Callable 函数指针
	| | | |- Function 函数指针
	| | | |- Virtual 虚函数指针
	| | |- Reference 数据指针
	|- Member
	| |- Field 字段
	| |- Method 方法
	|- Variable 变量
*/
#ifndef __FACT_TYPE__
#define __FACT_TYPE__
#pragma once
#include "base.h"



#ifdef  __cplusplus
extern "C" {
#endif //  __cplusplus	
	#define fact_Meta_Flags_byval = 1<<0;	//
	#define fact_Meta_Flags_primary = 1<<1;	//

	#define fact_Meta_Flags_public = 1<<2;	//
	#define fact_Meta_Flags_projected = 1<<3;	//
	#define fact_Meta_Flags_friend = 1<<4;	//
	#define fact_Meta_Flags_internal = 1<<5;	//

	#define fact_Meta_Flags_virt = 1<<6;	//
	#define fact_Meta_Flags_co = 1<<7;	//

	#define fact_Meta_Flags_static = 1<<8;	//
	#define fact_Meta_Flags_const = 1<<9;	//

	struct fact_Meta_Type_t;

	typedef struct fact_Meta_t {
		struct fact_Object_t;
		int flags;
		fact_str* name;
		fact_array* attrs;
	} fact_Meta;

	size_t fact_Meta_store(fact_Meta* meta, fact_write_t writer);


	typedef struct fact_Meta_Type_t {
		struct fact_Meta_t;
		size_t size;
		fact_array* fields;
		fact_array* methods;
		fact_array* generics;
		struct fact_Meta_Type_t* generated;
	} fact_Meta_Type;

	typedef struct fact_Meta_Func_t {
		struct fact_Meta_Type_t;
		fact_Meta_Type* ret;
		fact_array* args;
		void* entry;
	} fact_Meta_Func;

	typedef struct fact_Meta_Struct_t {
		struct fact_Meta_Type_t;
		fact_array* accessors;
	} fact_Meta_Struct;

	typedef struct fact_Meta_Variable_t {
		struct fact_Meta_t;
		fact_Meta_Type* variableType;
	} fact_Meta_Variable;

	typedef struct fact_Meta_Accessor_t {
		struct fact_Meta_Variable_t;
		fact_Meta_Func* getter;
		fact_Meta_Func* setter;
	} fact_Meta_Accessor;

	typedef struct fact_Meta_Parameter_t {
		struct fact_Meta_Variable_t;
		int hasDefaultValue;
		void* defaultValue;
	} fact_Meta_Parameter;

	typedef struct fact_Meta_Module_t {
		struct fact_Meta_t;
		fact_array* types;
	} fact_Meta_Module;

	typedef struct fact_Meta_Assembly_t {
		struct fact_Meta_t;
		size_t version;
		fact_array* modules;
	} fact_Meta_Assembly;

	typedef struct fact_Meta_AssemblyLayout_t {
		size_t depSize;
		size_t constVarSize;
		size_t metaSize;
		size_t codeSize;

	} fact_Meta_AssemblyLayout;

	typedef struct fact_Meta_AssemblyLayoutInfo_t {
		struct fact_Meta_AssemblyLayout_t;
		size_t depCount;
		size_t constVarCount;
		size_t metaCount;
		size_t codeCount;
	} fact_Meta_AssemblyLayoutInfo;

	typedef struct fact_Meta_AssemblyWriter_t {
		
		struct fact_Meta_AssemblyLayoutInfo;
		fact_write_t depWriter;
		fact_write_t constVarWriter;
		fact_write_t metaWriter;
		fact_write_t codeWriter;

	} fact_Meta_AssemblyWriter;

	bool fact_Meta_Assembly_store(fact_Meta_Assembly*const self, fact_write_t writer);
	

	
	
#ifdef  __cplusplus
} // enn extern C
#endif //  __cplusplus

#endif // !__FACT_TYPE__