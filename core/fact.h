#ifndef __FACT__
#define __FACT__
#ifdef  __cplusplus
extern "C" {
#endif //  __cplusplus

#pragma once
	typedef void* (*fact_memory_alloc_t)(size_t size);
	typedef void (*fact_memory_free_t)(void* p);

	typedef struct fact_memory_t {
		fact_memory_alloc_t alloc;
		fact_memory_free_t free;
	}fact_memory;

	typedef int (*fact_stream_write_t)(void* stream,void* buffer,size_t size);
	typedef int (*fact_stream_read_t)(void* stream, void* buffer, size_t size);

	typedef struct fact_Bind_t {
		void* self;
		void* method;
	}fact_Bind;
	typedef struct fact_link_t {
		/// <summary>
		/// 下一个节点指针
		/// </summary>
		struct fact_link_t* next;
	}fact_link;

	inline size_t fact_link_count(struct fact_link_t* self) {
		size_t count = 0; while (self) { count++; self = self->next; } return count;
	}
	inline void* fact_link_index(struct fact_link_t* self, size_t index) {
		size_t at = 0; while (self) {
			if (at == index) return ((char*)self)+sizeof(struct fact_link_t);
			at++; self = self->next;
		};
		return 0;
	}

	typedef struct fact_list_t {
		/// <summary>
		/// 链头
		/// </summary>
		fact_link* head;
		/// <summary>
		/// 链尾
		/// </summary>
		fact_link* tail;
	} fact_list;
	inline void fact_list_construct(fact_list* const self) { self->head = self->tail = 0; }
	inline size_t fact_list_count(fact_list* const self) { return  fact_link_count(self->head); }
	inline void* fact_list_index(fact_list* const self, size_t index) { return  fact_link_index(self->head, index); }
	inline void* fact_list_insert(fact_list* const self, fact_link* node) {
		if (self->head == 0) {
			self->head = self->tail = node;
		}
		else {
			self->tail->next = node; self->tail = node;
		}
		return ((char*)node) + sizeof(fact_link);
	}
	inline void* fact_list_append(fact_list* const self, size_t itemSize,fact_memory_alloc_t alloc) {
		fact_link* node = alloc(sizeof(fact_link) + itemSize);
		return fact_list_insert(self, node);
	}

	typedef struct fact_array_t {
		/// <summary>
		/// 长度
		/// </summary>
		size_t length;
		/// <summary>
		/// 数据区
		/// </summary>
		unsigned char data[];
	} fact_array;
	inline size_t fact_array_length(fact_array* const self) { return self->length; }

	typedef struct fact_string_t{
		struct fact_array_t;
	} fact_string;

	fact_string* fact_string_create(const char* str, fact_memory_alloc_t alloc);
	fact_string* fact_string_make(const char* cstr, fact_string* str, size_t len);

	int fact_string_compare(const fact_string* left, const fact_string* right);
	inline int fact_string_serialize(fact_string* self, fact_Bind write) {
		((fact_stream_write_t)write.method)(write.self, &self->length, sizeof(size_t));
		((fact_stream_write_t)write.method)(write.self, self->data, self->length);
		return 0;
	}
	inline fact_string* fact_string_deserialize(fact_Bind read,fact_memory_alloc_t alloc) {
		size_t length;
		((fact_stream_read_t)read.method)(read.self, &length, sizeof(size_t));
		fact_string* str = (fact_string*)alloc(sizeof(size_t) + length + 1);
		str->length = length;
		((fact_stream_read_t)read.method)(read.self, str->data, length);
		*(str->data + length) = 0;
		return str;
	}

	/// <summary>
	/// fact语言的类型元数据顶级对象
	/// 所有类型类都是以这个类作为基类
	/// </summary>
	struct fact_Type;
	/// <summary>
	/// fact中的class表示堆上分配的对象
	/// 堆上分配的对象除了与fact_Struct一样有个type字段外,还有gcref字段
	/// 用于垃圾回收
	/// </summary>
	typedef struct fact_Object_t {
		struct fact_Type* meta;
	} fact_Object;

	typedef struct fact_Type_t {
		struct fact_Object_t;
		/// <summary>
		/// 名称
		/// </summary>
		void* name;
		/// <summary>
		/// 类型标记
		/// 见 fact_TypeFlags_XX宏
		/// </summary>
		unsigned int type_flags;
		size_t size;
		/// <summary>
		/// 类型上标注的属性
		/// </summary>
		fact_array* attributes;
		/// <summary>
		/// 该类型是泛型生成
		/// 模板类型
		/// </summary>
		struct fact_Type_t* generatedBy;
		/// <summary>
		/// 泛型参数
		/// </summary>
		fact_array* genericArguments;
	} fact_Type;
	// 是否是原生数据: bool,byte,char,int,double,long,date,time,datetime,uuid,decimal,为原生
#define fact_TypeFlags_static 1		// 是否静态: 静态数据将在全局空间分配
#define fact_TypeFlags_func 1<<1		// 是否是函数
#define fact_TypeFlags_byval 1<<2		// 是否传值: 传值数据将在栈上分配，其他的在动态堆上分配
#define fact_TypeFlags_pointer 1<<3	// 是否是指针(数据指针)
#define fact_TypeFlags_wrapper 1<<4	// 是否是包装器
#define fact_TypeFlags_virtual 1<<5	// 是否有虚表
#define fact_TypeFlags_generic 1<<6	// 是否是泛型
#define fact_TypeFlags_function fact_TypeFlags_static|fact_TypeFlags_byval|fact_TypeFlags_func
#define fact_TypeFlags_struct fact_TypeFlags_static|fact_TypeFlags_byval
#define fact_TypeFlags_class fact_TypeFlags_static


	typedef struct fact_Func_t {
		struct fact_Type_t;
		/// <summary>
		/// 函数的种类
		/// </summary>
		int func_flags;
		/// <summary>
		/// 函数的返回值的类型
		/// </summary>
		fact_Type* returnType;
		/// <summary>
		/// 函数的参数
		/// </summary>
		fact_list argumentTypes;
		/// <summary>
		/// 函数的代码指针
		/// </summary>
		void* pointer;
	} fact_Func;

	typedef struct fact_Class_t {
		struct fact_Type_t;
		fact_array members;
	}fact_Class;

	typedef struct fact_Member_t {
		struct fact_Type_t;
		int memberFlags;
	}fact_Member;
	#define fact_MemberFlags_field		1<<0
	#define fact_MemberFlags_method		1<<1
	#define fact_MemberFlags_accessor	1<<2
	#define fact_MemberFlags_public		1<<8
	#define fact_MemberFlags_friend		1<<9
	#define fact_MemberFlags_projected	1<<10
	#define fact_MemberFlags_private	1<<11
	

	typedef struct fact_Field_t {
		struct fact_Member_t;
		fact_Type* fieldType;

	} fact_Field;

	typedef struct fact_Method_t {
		fact_Member;
		fact_Func func;
	} fact_Method;

	typedef struct fact_TypeBuilder_t {
		int typeFlags;
		fact_string* name;
		fact_list* members;
	}fact_TypeBuilder;
	fact_TypeBuilder* fact_TypeBuilder_createClass(fact_string* name,int flags, fact_memory_alloc_t alloc);
	fact_Field* fact_TypeBuilder_createField(fact_TypeBuilder* self, fact_string* name, fact_Type* fieldType, int flags, fact_memory_alloc_t alloc);
	void fact_TypeBuilder_serialize(fact_TypeBuilder* self, fact_Bind write);
/*

// fact语言的实例
struct fact_Fact;

/// <summary>
/// 一个Isolation 略等同一个线程
/// Isolation内部没有锁
/// Isolation之间的要通信要加锁
/// Isolation表示一组(与其他Isolation)隔离的内存分配方案
/// </summary>
struct fact_Isolation;

/// <summary>
/// 内存页
/// </summary>
struct fact_MemoryPage;

/// <summary>
/// StaticMemory 表示永远不会回收的内存
/// 比如static变量，元数据等
/// </summary>
struct fact_StaticMemory;
/// <summary>
/// AlignedMemory表示对齐分配的内存
/// 即分配只能4，8，16这样的一块
/// 大部分运行期的堆内存都是这种
/// </summary>
// struct fact_AlignedMemory;
/// <summary>
/// 可做垃圾回收的内存，必须要类型系统支持
/// 在fact里面，GC内存也是对齐的
/// </summary>
struct fact_GCMemory;



/// <summary>
/// fact语言的一个包
/// </summary>
struct fact_Package;

/// <summary>
/// fact语言的一个模块
/// </summary>
struct fact_Module;
/// <summary>
/// fact语言的一个可执行单元
/// </summary>
struct fact_Execution;


struct fact_Package {
	fact_list modules;
};

struct fact_Module {
	fact_list types;
};

#ifndef __FACT_INSTANCE__
#define __FACT_INSTANCE__
fact_Fact fact;
#endif // !fact_Fact_Instance

struct fact_Isolation {
	fact_Package* startPackage;
	fact_Module* startModule;
	fact_GCMemory gc;
};


struct fact_StaticMemory {

};
struct fact_GCMemory {

};


struct fact_Fact {
	struct fact_Object;
	/// <summary>
	/// 所有静态的数据都是全局
	/// </summary>
	fact_StaticMemory staticMemory;
	/// <summary>
	/// 所有的包，都是全局加载
	/// </summary>
	fact_list packages;
	/// <summary>
	/// 实例里面有多个Isolation;
	/// </summary>
	fact_list isolations;
	/// <summary>
	/// 第一个运行的isolation;
	/// </summary>
	fact_Isolation* mainIsolation;
};

*/


#ifdef  __cplusplus
} // enn extern C
#endif //  __cplusplus

#endif // !__FACT__
