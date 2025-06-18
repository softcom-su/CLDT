#ifndef _PRIVATE_FENV_H
#define _PRIVATE_FENV_H

#ifndef __ASSEMBLER__

typedef struct fp_data
{
    int                 cop;
    int                 flags;
    feOP                op1;
    feOP                op2;
    feOP                reslt;
    void               *addr;
} feDATA;
#endif

/* Код операции может принимать следующие значения */

#define FPOP_SUB_S   1
#define FPOP_SUB_D   2
#define FPOP_ADD_S   3
#define FPOP_ADD_D   4
#define FPOP_DIV_S   5
#define FPOP_DIV_D   6
#define FPOP_MUL_S   7
#define FPOP_MUL_D   8
#define FPOP_ABS_S   9
#define FPOP_ABS_D   10
#define FPOP_MOV_S   11
#define FPOP_MOV_D   12
#define FPOP_NEG_S   13
#define FPOP_NEG_D   14
#define FPOP_CVT_S_D 15
#define FPOP_CVT_S_W 16
#define FPOP_CVT_D_S 17
#define FPOP_CVT_D_W 18
#define FPOP_CVT_W_S 19
#define FPOP_CVT_W_D 20
#define FPOP_CMP_S   21
#define FPOP_CMP_D   22

#define FPOP_SQRT_S  23
#define FPOP_SQRT_D  24

#define FPOP_CVT_D_L 25
#define FPOP_CVT_S_L 26
#define FPOP_CVT_L_D 27
#define FPOP_CVT_L_S 28

#define FPOP_RAISE   99


#define FPOP_COP_OFFSET      0
#define FPOP_FLAGS_OFFSET    4
#define FPOP_OP1_OFFSET      8
#define FPOP_OP2_OFFSET      16
#define FPOP_RESLT_OFFSET    24

#ifdef KERNEL
#define FPC_EXCEPTION_UNIMPL    1<<13
#endif
#endif /* !_PRIVATE_FENV_H */
