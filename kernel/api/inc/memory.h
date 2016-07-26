#ifndef __MEMORY_H__
#define __MEMORY_H__

#ifdef __cplusplus
extern "C"
{
#endif

/*
需对各个模块的内存分配函数定义
libbb_udhcp.h   //udhcp
defs.h					//mlinux
usr_cfg.h				//mlan
*/
#define USE_MEM_DEBUG			1
#define LWIP_ALLOW_MEM_FREE_FROM_OTHER_CONTEXT	0	
#define MEM_ALIGNMENT     		4
//#define USE_MEM_ASSERT

#ifdef USE_MEM_ASSERT
	#define LWIP_ASSERT(message, assertion) do { if((int)(assertion) == 0)u_printf(DBG,message); } while(0)
#else
	#define LWIP_ASSERT(message, assertion) 
#endif /* USE_MEM_ASSERT */
	
/* MEM_SIZE would have to be aligned, but using 64000 here instead of
 * 65535 leaves some room for alignment...
 */

typedef unsigned int mem_size_t;


typedef unsigned int mem_ptr_t;

#define mem_init()

void sys_meminit(void *pool, unsigned int size);

#if USE_MEM_DEBUG

void *mem_malloc_ex(const char *name, mem_size_t size);
void *mem_calloc_ex(const char *name, mem_size_t count, mem_size_t size);

#define mem_malloc(SIZE) mem_malloc_ex(__FUNCTION__, SIZE)
#define mem_calloc(SIZE, COUNT) mem_calloc_ex(__FUNCTION__, (SIZE), (COUNT))

#define my_malloc(SIZE) mem_malloc_ex(__FUNCTION__, SIZE)
#define my_calloc(SIZE, COUNT) mem_calloc_ex(__FUNCTION__, (SIZE), (COUNT))

#else
void *mem_malloc(mem_size_t size);
void *mem_calloc(mem_size_t count, mem_size_t size);
#endif

#define malloc 		mem_malloc
#define free 		mem_free

#define my_free	mem_free

void mem_slide_check(int show_list);

uint32_t mem_free(void *rmem);
uint32_t get_mem_size(void *rmem);

#ifdef __cplusplus
}

#endif

#endif /*__LWIP_MEM_H__*/
