#if OS_VERSION >= 400
int modMemIsUncached(void *ptr);
static inline int isUncachedPtr(void* ptr)
{
   return modMemIsUncached(ptr);
}
#elif OS_VERSION >= 300
static inline int isUncachedPtr(void* ptr)
{
    unsigned p = (unsigned)ptr;
    if((p >= BOARD_UNCACHED_MEM_BASE && p < BOARD_UNCACHED_MEM_BASE + BOARD_UNCACHED_MEM_SIZE) || (p >= K1BASE && p < K2BASE))
       return TRUE;
    return FALSE;
}
#else
#define	isUncachedPtr	IS_KSEG1
#endif
