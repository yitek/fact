/*
fact语言的内存管理
*/
#ifndef __FACT_BASE__
#define __FACT_BASE__
#pragma once




#ifdef  __cplusplus
extern "C" {
#endif //  __cplusplus

	struct fact_Type_t;
	typedef struct fact_Object_t {
		struct fact_Type_t* type;
	} fact_Object;

	/// <summary>
	/// 申请内存函数类型
	/// </summary>
	typedef void* (*fact_aquirem_t)(size_t size);
	/// <summary>
	/// 释放内存函数类型
	/// </summary>
	typedef void (*fact_releasem_t)(void* p);

	void* fact_aquirem(size_t size);
	void fact_releasem(void * p);

	typedef size_t(*fact_write_t)(void* stream, void* buffer, size_t size);
	typedef size_t(*fact_read_t)(void* stream, void* buffer, size_t size);

	static fact_write_t fact_writem;
	static fact_read_t fact_readm;

	void* fact_openf(char* filename,char* mode);
	static fact_write_t fact_writef;
	static fact_read_t fact_readf;
	void fact_closef(void* file);

	typedef struct fact_bind_t{
		void* self;
		void* method;
	} fact_bind;

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
			if (at == index) return ((char*)self) + sizeof(struct fact_link_t);
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
	inline void* fact_list_append(fact_list* const self, size_t itemSize, fact_aquirem_t aquire) {
		fact_link* node = (fact_link*)aquire(sizeof(fact_link) + itemSize);
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

	typedef struct fact_str_t {
		struct fact_array_t;
	} fact_str;

	//fact_str* fact_str_create(const char* str, fact_aquirem_t aquire);
	//fact_str* fact_str_make(const char* cstr, fact_str* str, size_t len);

	inline int fact_str_compare(const fact_str* left, const fact_str* right) {
		if (left == 0) {
			return right ? 1 : 0;
		}
		else {
			if (right == 0) return 1;
			size_t at = 0;
			do {
				unsigned char lch = left->data[at];
				unsigned char rch = right->data[at];
				if (lch > rch) return 1;
				else if (lch < rch) return -1;
				else if (lch == 0) return 0;
			} while (++at);
		}
		return 0;
	}
	inline size_t fact_str_store(fact_str* self, fact_bind writer) {
		size_t count = ((fact_write_t)writer.method)(writer.self, &self->length, sizeof(size_t));
		count += ((fact_write_t)writer.method)(writer.self, &self->data, self->length);
		return count;
	}
	inline fact_str* fact_str_load(fact_bind reader, fact_aquirem_t alloc) {
		size_t length;
		((fact_read_t)reader.method)(reader.self, &length, sizeof(size_t));
		fact_str* str = (fact_str*)alloc(sizeof(size_t) + length + 1);
		str->length = length;
		((fact_read_t)reader.method)(reader.self, str->data, length);
		*(str->data + length) = 0;
		return str;
	}
	
#ifdef  __cplusplus
} // enn extern C
#endif //  __cplusplus

#endif // !__FACT_GC__