#ifndef __FACT_PLATFORM__
#define __FACT_PLATFORM__
#ifdef  __cplusplus
extern "C" {
#endif //  __cplusplus
#pragma once

void* fact_alloc(size_t size);
void fact_free(void* p);
void* fact_file_open(const char* filename,int flags);
int fact_file_close(void* file);
int fact_file_read(void* file,char* buffer,size_t size);
int fact_file_write(void* file,char* buffer,size_t size);
void fact_stdout(const char* message);
int fact_stdin(const char* buffer);


#ifdef  __cplusplus
} // enn extern C
#endif //  __cplusplus

#endif // !__FACT_PLATFORM__
