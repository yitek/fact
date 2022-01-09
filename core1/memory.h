/// 处理内存
///
/// 
#pragma once
#ifndef FACT_MEMORY
#define FACT_MEMORY
#ifdef __cplusplus 
extern "C" {
#endif

	struct fact_memory_GCMemory;

	void* fact_memory_malloc(size_t size,void* ctx);

	void* fact_memory_clear(void* p ,size_t size);

	struct fact_memory_GCUnit {
		void* type;
		long gcref;
	};

	struct  fact_memory_Page {
		struct fact_memory_Page* next;
	};

	struct  fact_memory_AlignedChunk {
		fact_memory_GCMemory* memory;
		size_t unitSize;
		// 容量，每个page的容量
		int pageCapacity;
		fact_memory_Page* headPage;
		fact_memory_Page* tailPage;
		fact_memory_AlignedChunk* next;
	};

	struct fact_memory_MemoryOptions {
		size_t pageSize;
	};

	

	struct fact_memory_GCMemory {
		fact_memory_MemoryOptions opts;
		void* threadContext;
		fact_memory_AlignedChunk* chunk;
	};


	const static long fact_memory_GCMask = !((long)0) >> 1;

	void* fact_GCUnit_object(fact_memory_GCUnit* const self) {
		return ((char*)self) + sizeof(fact_memory_GCUnit);
	}

	// chunk 初始化
	void fact_memory_AlignedChunk_construct(fact_memory_AlignedChunk* const self, struct fact_memory_GCMemory* memory, size_t objectSize) {
		self->memory = memory;
		self->unitSize = objectSize + sizeof(fact_memory_GCUnit);
		self->pageCapacity = (memory->opts.pageSize - sizeof(fact_memory_Page)) / self->unitSize;
		self->headPage = self->tailPage = 0;
	}
	// 从chunk中请求一个单元
	fact_memory_GCUnit* fact_memory_AlignedChunk_alloc(fact_memory_AlignedChunk* const self) {
		fact_memory_Page* page = self->headPage;
		while (page) {
			fact_memory_GCUnit* unit = (fact_memory_GCUnit*)((char*)page + sizeof(fact_memory_Page));
			if (unit->gcref == 0) return unit;
			for (int i = 0, j = self->pageCapacity; i < j; i++) {
				unit = (fact_memory_GCUnit*)((char*)unit + self->unitSize);
				if (unit->gcref == 0) return unit;
			}
			// 页内没找到空闲单元
			page = page->next;
		}
		// 所有页都没找到空闲单元
		//分配一个新页
		page = (fact_memory_Page*)fact_memory_malloc(self->memory->opts.pageSize,self->memory->threadContext);
		// 调整chunk里的页链表指针
		if (self->tailPage) {
			self->tailPage->next = page;
			self->tailPage = page;
		}
		else {
			self->headPage = self->tailPage = page;
		}
		// 初始化每个单元的gcref
		fact_memory_GCUnit* unit = (fact_memory_GCUnit*)((char*)page + sizeof(fact_memory_Page));
		fact_memory_GCUnit* unit0 = unit;
		for (int i = 0, j = self->pageCapacity; i < j; i++) {
			unit->gcref = 0;
			unit = (fact_memory_GCUnit*)((char*)unit + self->unitSize);
		}
		return unit0;
	}

	fact_memory_GCUnit* fact_memory_GCMemory_alloc(fact_memory_GCMemory* const self, size_t objSize) {
		size_t unitSize = objSize + sizeof(fact_memory_GCUnit);

		if (unitSize > 64) { // 40bytes
			if (unitSize > 128) { //160bytes
				if (unitSize > 512) { // 320ytes
					size_t count = unitSize / 64;
					if (unitSize % 64)count++;
					unitSize *= 64;
				}
				else {
					size_t count = unitSize / 16;
					if (unitSize % 16)count++;
					unitSize *= 16;
				}
			}
			else {
				size_t count = unitSize / 8;
				if (unitSize % 8)count++;
				unitSize *= 8;
			}
		}
		else {
			size_t count = unitSize / 4;
			if (unitSize % 4)count++;
			unitSize *= 4;
		}

		fact_memory_AlignedChunk* chunk = self->chunk;
		while (chunk) {
			if (unitSize < chunk->unitSize)break;
		}
		
		return 0;
	}









	// void* fact_alloc(size_t size, void* ctx = 0);
	// int fact_free(void* p, void* ctx = 0);
	// void* fact_gcnew(size_t size, void* ctx = 0);

#ifdef __cplusplus 
}
#endif
#endif // !FACT_MEMORY


