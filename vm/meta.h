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
#ifndef __FACT_META__
#define __FACT_META__

#pragma once
#include "base.h"

#ifdef  __cplusplus
extern "C" {
#endif //  __cplusplus

	struct fact_meta_Meta_t;
	size_t fact_meta_Meta_deserialize(struct fact_meta_Meta_t* self, fact_bind reader, fact_aquirem_t aquire);
	struct fact_meta_Type_t;
	size_t fact_meta_Type_deserialize(struct fact_meta_Type_t* self, fact_bind reader, fact_aquirem_t aquire);
	struct fact_meta_Type_t fact_meta_Type_resolve(fact_str* fullname);
	struct fact_meta_Struct_t;
	size_t fact_meta_Struct_deserialize(struct fact_meta_Struct_t* self, fact_bind reader, fact_aquirem_t aquire);

//	struct fact_meta_Bind_t;
	struct fact_meta_Pointer_t;
	size_t fact_meta_Pointer_deserialize(struct fact_meta_Pointer_t* self, fact_bind reader, fact_aquirem_t aquire);
	//struct fact_meta_Callable_t;
	struct fact_meta_Function_t;
	size_t fact_meta_Function_deserialize(struct fact_meta_Pointer_t* self, fact_bind reader, fact_aquirem_t aquire);

	struct fact_meta_Virtual_t;
	struct fact_meta_Reference_t;
	struct fact_meta_Member_t;
	struct fact_meta_Field_t;
	struct fact_meta_Method_t;
	struct fact_meta_Variable_t;

	#define fact_meta_FlagType unsigned int

	#define fact_meta_Kinds_type			(1<<1)
	#define fact_meta_Kinds_member		(1<<2)
	#define fact_meta_Kinds_variable		(1<<3)
	#define fact_meta_Kinds_metaBits 3
	#define fact_meta_Kinds_struct	(fact_meta_Kinds_type | (1<<(fact_meta_Kinds_metaBits+1)))
	#define fact_meta_Kinds_pointer	(fact_meta_Kinds_type | (1<<(fact_meta_Kinds_metaBits+2)))
	#define fact_meta_Kinds_primary	(fact_meta_Kinds_type | (1<<(fact_meta_Kinds_metaBits+3)))
	#define fact_meta_Kinds_class	(fact_meta_Kinds_type | (1<<(fact_meta_Kinds_metaBits+4)))
	#define fact_meta_Kinds_bind		(fact_meta_Kinds_type | (1<<(fact_meta_Kinds_metaBits+4)))
	#define fact_meta_Kinds_typeBits  fact_meta_Kinds_metaBits + 4

	#define fact_meta_Kinds_callable	(fact_meta_Kinds_pointer | (1<<(fact_meta_Kinds_typeBits+1)))
	#define fact_meta_Kinds_reference	(fact_meta_Kinds_pointer | (1<<(fact_meta_Kinds_typeBits+2)))
	#define fact_meta_Kinds_pointerBits  fact_meta_Kinds_typeBits + 2

	#define fact_meta_Kinds_function		(fact_meta_Kinds_callable | (1<<(fact_meta_Kinds_pointerBits+1)))
	#define fact_meta_Kinds_virtual		(fact_meta_Kinds_callable | (1<<(fact_meta_Kinds_pointerBits+1)))
	#define fact_meta_Kinds_callableBits  fact_meta_Kinds_typeBits + 2

	#define fact_meta_Kinds_field		(fact_meta_Kinds_member | (1<<(fact_meta_Kinds_callableBits+1)))
	#define fact_meta_Kinds_method		(fact_meta_Kinds_member | (1<<(fact_meta_Kinds_callableBits+2)))

	#define fact_meta_Type_Flags_private 1<<1
	#define fact_meta_Type_Flags_protected 1<<2
	#define fact_meta_Type_Flags_internal 1<<3
	#define fact_meta_Type_Flags_friend 1<<4
	#define fact_meta_Type_Flags_public 1<<5
	#define fact_meta_Type_Flags_accessBits  5

	#define fact_meta_Type_Flags_abstract (1<<(fact_meta_Type_Flags_accessBits+1))
	#define fact_meta_Type_Flags_interface (fact_meta_Type_Flags_abstract | (1<<(fact_meta_Type_Flags_accessBits+2)))
	#define fact_meta_Type_Flags_generated	(1<<(fact_meta_Type_Flags_accessBits+3))
	#define fact_meta_Type_Flags_generic	(1<<(fact_meta_Type_Flags_accessBits+4))
	#define fact_meta_Type_Flags_dataBits   fact_meta_Type_Flags_accessBits+5

	#define fact_meta_Type_Flags_vircall (1<<(fact_meta_Type_Flags_dataBits+1))
	#define fact_meta_Type_Flags_cocall (1<<(fact_meta_Type_Flags_dataBits+2))


	typedef struct fact_meta_Meta_t {
		struct fact_Object_t;
		fact_meta_FlagType metaKind;
		fact_str* name;
	} fact_meta_Meta;
	inline size_t fact_meta_Meta_store(fact_meta_Meta* self, fact_bind writer) {
		size_t count = ((fact_write_t)writer.method)(writer.self, &self->metaKind, sizeof(fact_meta_FlagType));
		fact_str_store(self->name, writer);
	}
	inline size_t fact_meta_Meta_load(fact_bind reader, fact_aquirem_t aquire) {
		fact_meta_FlagType kind;
		size_t count = ((fact_read_t)reader.method)(reader.self, &kind, sizeof(fact_meta_FlagType));
		if (kind && fact_meta_Kinds_struct) {
			fact_meta_Struct* structMeta = aquire(sizeof(fact_meta_Struct));
			fact_meta_Struct_deserialize(structMeta, reader, aquire);
		}
	}
	inline size_t fact_meta_Meta_deserialize(fact_meta_Meta* self, fact_bind reader,fact_aquirem_t aquire) {
		self->name = fact_str_load(reader, aquire);
	}

	typedef struct fact_meta_Type_t {
		struct fact_meta_Meta_t;
		fact_meta_FlagType typeFlags;
		size_t size;
		fact_array* gerenics;
		struct fact_meta_Type_t* generic;
	} fact_meta_Type;
	inline size_t fact_meta_Type_deserialize(fact_meta_Type* self, fact_bind reader, fact_aquirem_t aquire) {
		size_t count = ((fact_read_t)reader.method)(reader.self, &self->typeFlags, sizeof(fact_meta_FlagType));
		count += ((fact_read_t)reader.method)(reader.self, &self->size, sizeof(size_t));
		size_t genericCount;
		count += ((fact_read_t)reader.method)(reader.self, &genericCount, sizeof(size_t));
		if (genericCount) {
			self->gerenics = aquire(sizeof(fact_array)+sizeof(void*)*(genericCount+1));
			// self->generics.
		}
	}

	typedef struct fact_meta_Struct_t {
		struct fact_meta_Type_t;
		fact_array* fields;
		fact_array* methods;
	} fact_meta_Struct;

	typedef struct fact_meta_Pointer_t {
		struct fact_meta_Type_t;
		struct fact_Meta_Type* targetType;
	} fact_meta_Pointer;

	typedef struct fact_meta_Function_t {
		struct fact_meta_Pointer_t;
		struct fact_Meta_Type* returnType;
		struct fact_array* arguments;
		void* entryAddress;
	} fact_meta_Function_t;

	typedef struct fact_meta_Member_t {
		struct fact_meta_Meta_t;
	} fact_meta_Member_t;

	typedef struct fact_meta_Field_t {
		struct fact_meta_Member_t;
		size_t offset;
	} fact_meta_Field_t;








	

#ifdef  __cplusplus
} // enn extern C
#endif //  __cplusplus

#endif // !__FACT_GC__