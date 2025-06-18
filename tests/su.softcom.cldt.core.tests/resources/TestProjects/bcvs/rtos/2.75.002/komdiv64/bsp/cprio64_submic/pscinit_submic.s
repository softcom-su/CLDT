/* include from flashinit*/

#ifdef FOR_SUBMICRON

#define INVERT32_(reg)				\
	srl	v1, reg, 24;			\
	sll	v0, reg, 24;			\
	or	v0, v1;				\
	andi	v1, reg, 0xff00;		\
	sll	v1, 8;				\
	or	v0, v1;				\
	srl	v1, reg, 8;			\
	andi	v1, 0xff00;			\
	or	v0, v1;				\
	move	reg, v0


#define	PCI_CONFREG_BASE    0xbe000000
#define PCI_CONFREG(bus,dev,fun,reg)   (PCI_CONFREG_BASE | (bus << 16) | (dev << 11) | (fun << 8) | reg) 

/* write register PCI and check*/
#define PCI_WRITE_REG(dev,reg,value)  \
   li t1,PCI_CONFREG(0,dev,0,reg);  \
   li t3,value;          \
   INVERT32_(t3);         /*invert value*/ \
   sw t3,0(t1);           /*write value in reg PCI*/ \
  

/*
preliminary initialization PCI for increment cod readiness
use registers:
t1-t3,
t7-t8 - trace debug
*/
init_pci_pre_submic:
/* PCI init 1890VM6 pre*/       
       PCI_WRITE_REG(0,0x10,PCI_REG_BAR_val)          /* BAR 1890VM6 init */
       PCI_WRITE_REG(0,0x40,PCI_REG_TRANS_val)        /* TRAN REG init*/
       PCI_WRITE_REG(0,0x58,PCI_REG_MASK_TRANS_val)   /* MASK TRAN REG init*/

       PCI_WRITE_REG(0,0x7c,0)                        /*  interdiction interrupt */
       PCI_WRITE_REG(0,0xe4,PCI_REG_ENARET_val)       /* enable retry */
       PCI_WRITE_REG(0,0x04,PCI_REG_COMSTAT_val)      /* COMMAND init*/

/* PCI init PSC pre*/
       PCI_WRITE_REG(2,0x10,PCI_PSC_REG_BAR_val)      /* BAR PSC init */
       PCI_WRITE_REG(2,0x04,PCI_PSC_REG_COMSTAT_val)  /* COMMAND init*/
end_init_pci_pre_submic:
        move v0,zero
        jr ra             
        nop
       .ent init_pci_pre_submic

inc_sVM6LIVEINC:
        li   t1,0xb0004000 /* read from 1000h*/
        lw   t2,0(t1)
        add  t2,t2,0x0100 /* increment sVM6LIVEINC*/ /* big_endian -> little_endian*/
        and  t2,t2,0x0f00 
        sw   t2,0(t1)      /* write to  1000h*/
        jr ra             
        nop
        .ent inc_sVM6LIVEINC

inc_sVM7LIVEINC:
        li   t1,0xb0004004 /* read from 1001h*/
        lw   t2,0(t1)
        add  t2,t2,0x0100 /* increment sVM7LIVEINC*/
        and  t2,t2,0x0f00 
        sw   t2,0(t1)      /* write to  1001h*/
        jr ra             
        nop
        .ent inc_sVM7LIVEINC
#endif  /*FOR_SUBMICRON*/


