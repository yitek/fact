#include <malloc.h>
#include <stdio.h>
#include "base.h"

void* fact_aquirem(size_t size) {
	return malloc(size);
}

void fact_releasem(void* p) {
	free(p);
}

void fact_copym(void* dest, const void* src, size_t size) {

	while (size>sizeof(int)) {
		*((int*)dest) = *((int*)src);
		++((int*)dest);
		++((int*)src);
		size -= sizeof(int);
	}
	while (size) {
		*((char*)dest) = *((char*)src);
		++((char*)dest);
		++((char*)src);
		size -= sizeof(char);
	}
}


void* fact_openm(char* filename, char* mode) {
	MemoryStream* stream= fact_aquirem(sizeof(MemoryStream));

}
bool fact_closem(void* stream) {
	fact_list* list = (fact_list*)stream;
	fact_list_clear(list , fact_releasem);
	fact_releasem(list);
}

size_t fact_writem(void* _stream, void* data, size_t _size) {
	MemoryStream* stream = (MemoryStream*)_stream;
	size_t size = _size;
	fact_aquirem_t aquire = virt_func(_stream,0,0);
	size_t dataPos = 0;
	while (size) {
		size_t bytes;
		size_t cap =stream->pageSize - stream->pagePos;
		if (cap==0) {
			fact_link* prev = stream->curr;
			if (prev) stream->curr = prev->next;
			if (!stream->curr) {
				stream->curr = (fact_link*)aquire(stream->pageSize+sizeof(fact_link));
				stream->curr->next = 0;
				if (prev) prev->next = stream->curr;
			}
			stream->pagePos = sizeof(fact_link);
		}
		bytes = size >= cap ? cap : size;
		fact_copym(stream->curr +  stream->pagePos, (char*)data + dataPos, bytes);
		stream->pos += bytes;
		stream->pagePos += bytes;
		size -= bytes;
	}
	return size;
}
fact_writem(){ }


void* fact_openf(char* filename,char* mode) {
	return fopen(filename, mode);
}
size_t fact_writeFile(void* stream, void* data,size_t size) {
	return fwrite(data, 1, size, stream);
}
static fact_write_t fact_writef = &fact_writeFile;

size_t fact_readFile(void* stream, void* buffer, size_t size) {
	return fread(buffer, 1, size, stream);
}

static fact_read_t fact_readf = fact_readFile;
void fact_closef(void* file) {
	fclose((FILE*)file);
}
