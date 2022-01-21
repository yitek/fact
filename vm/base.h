/*
memory management for fact lang
*/
#ifndef __FACT_BASE__
#define __FACT_BASE__
#pragma once
#ifndef  size_t
typedef unsigned long long size_t;
#endif // ! size_t

#ifndef bool
typedef int bool;
#endif // !bool

#ifndef true
#define true 1
#endif // !true

#ifndef false
#define false 0
#endif // !true


#ifdef  __cplusplus
extern "C" {
#endif //  __cplusplus

	struct fact_Type_t;
	typedef struct fact_Object_t {
		struct fact_Type_t* __type__;
	} fact_Object;

	typedef struct fact_GCObject_t {
		size_t __gc__;
		struct fact_Object_t;
	} fact_GCObject;

	inline void* virt_func(const void* obj,size_t funcIndex,size_t ifcIndex) {
		void** vtbl = (void**)obj + ifcIndex*sizeof(void**);
		return vtbl + funcIndex;
	}


	/// <summary>
	/// the function declare for apply memory
	/// </summary>
	typedef void* (*fact_aquirem_t)(size_t size);
	/// <summary>
	/// the function declare for release memory
	/// </summary>
	typedef void (*fact_releasem_t)(void* p);

	typedef struct fact_AllocatorVTBL_t{
		fact_aquirem_t aquire;
		fact_releasem_t release;
	}fact_AllocatorVTBL;
	
	void* fact_aquirem(size_t size);
	void fact_releasem(void * p);
	void fact_copym(void* dest,void* src,size_t size);

	/// <summary>
	/// the function declare for writting to stream
	/// </summary>
	typedef size_t(*fact_write_t)(void* stream, void* buffer, size_t size);
	/// <summary>
	/// the function declare for reading from stream
	/// </summary>
	typedef size_t(*fact_read_t)(void* stream, void* buffer, size_t size);

	typedef bool (*fact_rwpos_t)(void* stream,size_t pos);

	typedef bool (*fact_close_t)(void* stream);



	typedef struct fact_StreamVTBL_t {
		fact_rwpos_t pos;
		fact_close_t close;
		fact_write_t write;
		fact_read_t read;
	}fact_StreamVTL;

	typedef struct fact_Stream_t {
		fact_StreamVTL* __vtbl__;
	}fact_Stream;

	typedef struct MemoryStream_t {
		fact_StreamVTL* __stream_vtbl__;
		fact_AllocatorVTBL* __allocator_vtbl__;
		struct fact_link_t;
		fact_link* curr;
		size_t pageSize;
		size_t pos;
		size_t pagePos;
	}MemoryStream;

	size_t fact_writem(void* _stream, void* data, size_t _size);
	static fact_read_t fact_readm;
	static fact_stream_pos_t fact_streamPos;

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
		/// next node
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
		/// link header
		/// </summary>
		fact_link* head;
		/// <summary>
		/// link tail
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

	inline void fact_list_clear(fact_list* const self, fact_releasem_t release) {
		fact_link* it = self->head;
		fact_link* end = self->tail;
		fact_link* next;
		while (it != end) {
			next = it->next;
			release(it);
			it = next;
		}
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
	inline fact_array* fact_array_create(size_t elemCount, size_t elemSize, fact_aquirem_t aquire) {
		fact_array* arr = aquire(sizeof(fact_array) + elemCount* elemSize);
		arr->length = elemCount;
		return arr;
	}

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
	

	inline fact_str* fact_str_load(fact_bind reader, fact_aquirem_t alloc,size_t* count) {
		size_t length;
		*count =((fact_read_t)reader.method)(reader.self, &length, sizeof(size_t));
		fact_str* str = (fact_str*)alloc(sizeof(size_t) + length + 1);
		str->length = length;
		*count+= ((fact_read_t)reader.method)(reader.self, str->data, length);
		*(str->data + length) = 0;
		return str;
	}
	
#ifdef  __cplusplus
} // enn extern C
#endif //  __cplusplus

#endif // !__FACT_GC__