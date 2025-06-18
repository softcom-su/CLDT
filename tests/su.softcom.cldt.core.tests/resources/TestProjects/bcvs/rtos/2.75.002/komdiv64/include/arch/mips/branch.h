#ifndef BRANCH_H_
#define BRANCH_H_
union mipscmd {
    struct { 
        unsigned int cop: 6;
        unsigned int rs : 5;
        unsigned int rt : 5;
        int          offset: 16;
    } f1;
    struct { 
        unsigned int cop: 6;
        unsigned int target: 26;
    } f2;
    struct { 
        unsigned int cop: 6;
        unsigned int rs : 5;
        unsigned int o : 5;
        unsigned int rd : 5;
        unsigned int o1 : 5;
        unsigned int cop1 : 6;
    } f3;
    struct { 
        unsigned int cop: 6;
        unsigned int bc : 5;
        unsigned int cc : 3;
        unsigned int ndtf : 2;
        int          offset: 16;
    } bc1;
    struct { 
        unsigned int cop: 6;
        unsigned int fmt : 5;
        unsigned int ft : 5;
        unsigned int fs : 5;
        unsigned int cc : 3;
        unsigned int space : 1;
        unsigned int a : 1;
        unsigned int fc : 2;
        unsigned int cond : 4;
    } c;
    struct { 
        unsigned int cop: 6;
        unsigned int fmt : 5;
        unsigned int ft : 5;
        unsigned int fs : 5;
        unsigned int fd : 5;
        unsigned int cop1 : 6;
    } flt;
    unsigned int cmd;
};

extern int __compute_return_epc(struct pt_regs *regs);
void   __mips_condbit(union mipscmd fpacmd,int set);

#endif /*BRANCH_H_*/
