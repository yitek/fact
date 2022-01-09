// fact.cpp : 此文件包含 "main" 函数。程序执行将在此处开始并结束。
//

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "fact.h"

void* null_pointer = 0;
size_t no_size = 0;

fact_string* fact_string_create(const char* cstr, fact_memory_alloc_t alloc) {
	size_t len = strlen(cstr);
	fact_string* result = (fact_string*)alloc(sizeof(fact_string) + len);
	return fact_string_make(cstr, result, len);
}
fact_string* fact_string_make(const char* cstr, fact_string* str,size_t len) {
	str->length = len;
	strcpy_s(str->data,len, cstr);
	return str;
}

int fact_string_compare(const fact_string* left, const fact_string* right) {
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


size_t fact_Meta_serialize(fact_Meta* self, fact_Bind writer) {
	// flags
	size_t count = ((fact_stream_write_t)writer.method)(writer.self, &self->metaFlags, sizeof(unsigned int));
	// name
	count += fact_string_serialize(self->name, writer);
	// size
	//((fact_stream_write_t)writer.method)(writer.self, &self->size, sizeof(size_t));
	// attributes
	if (self->attributes) {
		count += ((fact_stream_write_t)writer.method)(writer.self, &self->attributes->length, sizeof(size_t));
		for (size_t i = 0, j = self->attributes->length; i < j; i++) {
			// todo write attributes
		}
	}
	else {
		count += ((fact_stream_write_t)writer.method)(writer.self, &no_size, sizeof(size_t));
	}
	// genteratedBy
	if (self->genBy) {
		fact_string* fullname = fact_Type_fullname(self->genBy);
		count += fact_string_store(fullname,writer);
	}
	else {
		count += ((fact_stream_write_t)writer.method)(writer.self, &no_size, sizeof(size_t));
	}
	// genericArguments
	if (self->generics) {
		count += ((fact_stream_write_t)writer.method)(writer.self, &self->generics->length, sizeof(size_t));
		for (size_t i = 0, j = self->generics->length; i < j; i++) {
			fact_Type* genericType = ((fact_Type*)self->generics->data)+i;
			fact_string* fullname = fact_Type_fullname(genericType);
			count += fact_string_store(fullname, writer);
		}
	}
	else {
		count += ((fact_stream_write_t)writer.method)(writer.self, &no_size, sizeof(size_t));
	}
}

void fact_Meta_deserialize(fact_Meta* self, fact_Bind reader, fact_memory_alloc_t alloc) {
	//
	self->name = fact_string_load(reader, alloc);
	size_t attrLen;
	((fact_stream_read_t)reader.method)(reader.self, &attrLen, sizeof(size_t));
	if (attrLen == 0) {
		self->attributes = 0;
	}
	else {
		//TODO: load attrs
	}
	size_t generatedFromTypeNameLen;
	((fact_stream_read_t)reader.method)(reader.self, &generatedFromTypeNameLen, sizeof(size_t));
	if (generatedFromTypeNameLen) {
		
	}
	else {
		self->genBy = 0;
	}
	size_t genericLen;
	((fact_stream_read_t)reader.method)(reader.self, &genericLen, sizeof(size_t));
	if (genericLen) {}
	else {
		self->generics = 0;
	}
}

size_t fact_Class_serialize(fact_Class* self, fact_Bind writer) {
	size_t count = fact_Type_serialize(self, writer);
	if (self->members == 0) {
		count += ((fact_stream_write_t)writer.method)(writer.self, &no_size, sizeof(size_t));
	}
	else {
		count += ((fact_stream_write_t)writer.method)(writer.self, &self->members->length, sizeof(size_t));
		for (size_t i = 0, j = self->members->length; i < j; i++) {
			fact_Member* genericType = ((fact_Member*)self->members->data) + i;
			// TODO
		}
	}
}

void fact_Class_deserialize(fact_Class* self, fact_Bind reader,fact_memory_alloc_t alloc) {
	fact_Type_deserialize(self, reader, alloc);
	size_t memberCount;
	((fact_stream_read_t)reader.method)(reader.self, &memberCount, sizeof(size_t));
	if (memberCount) {}
	else {
		self->members = 0;
	}
}

fact_TypeBuilder* fact_TypeBuilder_createClass(fact_string* name, int isStruct, fact_memory_alloc_t alloc) {
	fact_TypeBuilder* builder = alloc(sizeof(fact_TypeBuilder));
	builder->name = name;
	builder->typeFlags = isStruct?fact_TypeFlags_struct: fact_TypeFlags_class;
	builder->members = (fact_list*)alloc(sizeof(fact_list));
	fact_list_construct(builder->members);
	return builder;
}
fact_Field* fact_TypeBuilder_createField(fact_TypeBuilder* self,fact_string* name, fact_Meta* fieldType,int flags, fact_memory_alloc_t alloc) {
	
	fact_Field* field = fact_list_append(self->members,sizeof(fact_Field),alloc);
	field->name = name;
	field->fieldType = fieldType;
	field->metaFlags = flags;
	return field;
}


void fact_Field_serialize(fact_Field* self, fact_Bind write) {
	
	// 写入字段类型的全名
	fact_Meta* fieldType = self->fieldType;
	fact_string* typeName = fieldType->name;
	fact_string_serialize(typeName,write);

}

fact_Field* fact_Field_deserialize(fact_Bind read,fact_memory_alloc_t alloc) {
	fact_Field* field = alloc(sizeof(fact_Field));
	fact_string* typeName = fact_string_deserialize(read,alloc);
	field->fieldType = 0;
	return field;
}
void fact_Member_serialize(fact_Member* self, fact_Bind write) {
	//将成员的flags写入
	((fact_stream_write_t)write.method)(write.self, &self->typeFlags, sizeof(int));
	// 写入字段name
	fact_string_serialize(self->name, write);

	if (self->typeFlags & fact_MemberFlags_field) {
		fact_Field_serialize((fact_Field*)self, write);
	}
}
fact_Member* fact_Member_deserialize(fact_Bind read,fact_memory_alloc_t alloc) {
	int flags;
	//将成员的flags读出
	((fact_stream_read_t)read.method)(read.self, &flags, sizeof(int));
	fact_string* name = fact_string_deserialize(read,alloc);
	fact_Member* member=0;
	if (flags & fact_MemberFlags_field) {
		member = fact_Field_deserialize(read,alloc);
	}
	member->typeFlags = flags;
	member->name = name;
	return member;
}


void fact_TypeBuilder_serialize(fact_TypeBuilder* self,fact_Bind write) {
	//将类型flags写入
	((fact_stream_write_t)write.method)(write.self, &self->typeFlags, sizeof(int));
	//将名称写入
	fact_string_serialize(self->name, write);
	//写入成员个数
	size_t memberCount = fact_list_count(self->members);
	((fact_stream_write_t)write.method)(write.self, &memberCount, sizeof(size_t));
	//逐个写入成员
	fact_link* node = self->members->head;
	while (node) {
		fact_Member* member = (fact_Member*)((char*)node + sizeof(fact_link));
		fact_Member_serialize(member,write);
	}

}																																																				