#ifndef  CALL_F_H
# define CALL_F_H
/*-----------------------------------------------------------------------*/
#define CALLF_MAX_MESSAGE_SIZE  120
#define CALLF_MAX_MESSAGE_NUMBER 10
#define CALLF_MAX_NUMBER_PARAMS  10
typedef struct sym_func 
{
    const char *name; const char *types; funcptr_t func;
} symFUNC ;
int createThreadCallFunction(int pri, int stackSize, symFUNC *symF) ;
/*-----------------------------------------------------------------------*/
#endif
