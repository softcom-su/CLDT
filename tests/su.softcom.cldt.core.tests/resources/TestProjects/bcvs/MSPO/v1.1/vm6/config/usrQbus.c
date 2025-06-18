/* usrQbus.c - example of QBUS initialization */

#include <system.h>
#include <qbusLib.h>

/******************************************************************************/

/* globals */

int qbusDefTimings[QBUS_TIMINGS_NUMBER] = {
    CT_LE_L(0x00000000),	/* 0, address setup               */
    CT_LE_L(0x00000000),	/* 1, address hold                */
    CT_LE_L(0x00000000),	/* 2, data setup                  */
    CT_LE_L(0x00000000),	/* 3, data/SYN hold               */
    CT_LE_L(0x000f0000),	/* 4, AN high timeout             */
    CT_LE_L(0x000f0000),	/* 5, AN low timeout              */
    CT_LE_L(0x00000000),	/* 6, WR hold                     */
    CT_LE_L(0x00000000),	/* 7, RD hold                     */
    CT_LE_L(0x00000000),	/* 8, RD to ERQ preset            */
    CT_LE_L(0x000a0000),	/* 9, ERQ to AN low timeout       */
    CT_LE_L(0x00000000),	/* A, RD,ERQ hold                 */
    CT_LE_L(0x000a0000),	/* B, ERQ to AN high timeout      */
    CT_LE_L(0x00000000),	/* C, bus turn-around             */
    CT_LE_L(0x00060000),	/* D, BAK high timeout            */
    CT_LE_L(0x00000000),	/* E, not used                    */
    CT_LE_L(0x00000000)		/* F, not used                    */
};

/* descriptor of system qbus controller */

QBUS_ID sysQbusId = NULL;

/* notify dma tasks end semaphore */

sem_t qbusTaskEndSem;

/******************************************************************************/

/* controller initialization routine, called from boardHwInit3() */

STATUS sysQbusInit()
{
    /* allocate and initialize qbus controller */

    if(!(sysQbusId = qbusCtrlFind(0)))
	return -1;

    /* program qbus master timings */

    qbusTimingsSet(sysQbusId,qbusDefTimings);

    return 0;
}

/******************************************************************************/

/* dma tasks end interrupt service routine */

void qbusTaskEndIntr(QBUS_ID qbusId, sem_t* syncSem)
{
    /* acknowledge interrupt */

    *QBUS_REG(qbusId,QBUS_REG_TASK_COUNT) = CT_LE_L(0);

    sem_post(syncSem);
}

/******************************************************************************/

/* copy data from host memory to qbus via dparm dma */

STATUS qbusBlkWrite(QBUS_ID qbusId, short* pHost, unsigned pQbus, int nBytes)
{
    int ix; unsigned* tbl; unsigned* buf; unsigned nWords = nBytes >> 1;

    /* validate qbus descriptor */

    if(!qbusId && !(qbusId = sysQbusId))
    {
	errno = EINVAL; return -1;
    }

    /* maximum size is (DPRAM_SIZE - table - mailbox) */

    if(nBytes > (QBUS_DPRAM_SIZE >> 1) - 10 || (nBytes & 1))
    {
	errno = EINVAL; return -1;
    }

    /* qbus address should be 4 byte aligned */

    if((pQbus & 3) || (pQbus + nBytes) > (QBUS_BUS_SIZE >> 1))
    {
	errno = EINVAL; return -1;
    }

    /* table started at 0 DPRAM address, data buffer at 6 DPRAM address */

    tbl = (int*)QBUS_DPRAM_TO_LOCAL(qbusId,0);
    buf = tbl + 3;

    tbl[0] = CT_LE_L(QBUS_TASK_GO | QBUS_D2Q_TASK | (nWords - 1));
    tbl[1] = CT_LE_L(6 >> 1);
    tbl[2] = CT_LE_L(pQbus >> 2);

    /* copy data from host memory to dpram */

    for(ix = 0; ix < nWords; ix++)
	buf[ix] = CT_LE_L(pHost[ix]);

    /* start dma transfer */

    if(qbusXferStart(qbusId,0,1) == -1)
    {
	errno = EIO; return -1;
    }

    return sem_wait(&qbusTaskEndSem);
}

/******************************************************************************/

/* copy data from qbus to host memory via dparm dma */

STATUS qbusBlkRead(QBUS_ID qbusId, short* pHost, unsigned pQbus, int nBytes)
{
    int ix; unsigned* tbl; unsigned* buf; unsigned nWords = nBytes >> 1;

    /* validate qbus descriptor */

    if(!qbusId && !(qbusId = sysQbusId))
    {
	errno = EINVAL; return -1;
    }

    /* maximum size is (DPRAM_SIZE - table - mailbox) */

    if(nBytes > (QBUS_DPRAM_SIZE >> 1) - 10 || (nBytes & 1))
    {
	errno = EINVAL; return -1;
    }

    /* qbus address should be 4 byte aligned */

    if((pQbus & 3) || (pQbus + nBytes) > (QBUS_BUS_SIZE >> 1))
    {
	errno = EINVAL; return -1;
    }

    /* table started at 0 DPRAM address, data buffer at 6 DPRAM address */

    tbl = (int*)QBUS_DPRAM_TO_LOCAL(qbusId,0);
    buf = tbl + 3;

    tbl[0] = CT_LE_L(QBUS_TASK_GO | QBUS_Q2D_TASK | (nWords - 1));
    tbl[1] = CT_LE_L(6 >> 1);
    tbl[2] = CT_LE_L(pQbus >> 2);

    /* start dma transfer */

    if(qbusXferStart(qbusId,0,1) == -1)
    {
	errno = EIO; return -1;
    }

    if(sem_wait(&qbusTaskEndSem) == -1)
	return -1;

    /* copy data from dpram to host memory */

    for(ix = 0; ix < nWords; ix++)
	pHost[ix] = CF_LE_L(buf[ix]);

    return 0;
}

/******************************************************************************/

/* dummy dma task end interrupt handler */

static void taskIntr(QBUS_ID qbusId, int arg)
{
    /* acknowledge interrupt */

    *QBUS_REG(qbusId,QBUS_REG_TASK_COUNT) = CT_LE_L(0);

    /* do something */
}

/******************************************************************************/

/* dummy slave interrupt handler */

static void slaveIntr(QBUS_ID qbusId, int irq)
{
    unsigned value, count, mask;

    /* acknowledge interrupt (suspend/resume dma transfer in needed) */

    qbusDmaSuspend(qbusId,&count,&mask);
    value = *QBUS_REG(qbusId,QBUS_REG_INT_VEC);
    qbusDmaResume(qbusId,count,mask);

    value = CF_LE_L(value) & 0xffff;

    /* do something */
}

/******************************************************************************/

/* dummy mailbox interrupt handler */

static void mboxIntr(QBUS_ID qbusId, int arg)
{
    register unsigned value;

    /* acknowledge interrupt */

    value = *((int*)QBUS_DPRAM_TO_LOCAL(qbusId,QBUS_MBOX));
    value = CF_LE_L(value) & 0xffff;

    /* do something */
}

/******************************************************************************/

/* dummy SCI interrupt handler */

static void sciIntr(QBUS_ID qbusId, int line)
{
    /* disable SCI interrupts */

    *QBUS_REG(sysQbusId,QBUS_REG_INT_ENA) &= ~CT_LE_L(QBUS_INT_SCI0 << line);

    /* do something */
}

/******************************************************************************/

/* NOTE: befor use qbusBlkXX() routines should be passed initialization: */

#if FALSE
    sem_init(&qbusTaskEndSem,0,0);

    qbusIntConnect(sysQbusId,QBUS_IV_TASK,qbusTaskEndIntr,(int)&qbusTaskEndSem);

    *QBUS_REG(sysQbusId,QBUS_REG_CTRL) |= CT_LE_L(QBUS_CTRL_MASTER_ON);

    *QBUS_REG(sysQbusId,QBUS_REG_INT_ENA) |= CT_LE_L(QBUS_INT_TASK);
#endif

/******************************************************************************/

/* additional qbus initialization routine, called from boardHwInit3() */

STATUS usrQbusConfig()
{
    if(!sysQbusId)
    {
	printf("usrQbusConfig: sysQbusInit failed\n\r");
	return -1;
    }

#if 0
    *QBUS_REG(sysQbusId,QBUS_REG_CTRL) |= CT_LE_L(QBUS_CTRL_MASTER_ON);

    /* this is example for stub initialization (be careful with interrupts) */

    /* setup slave window in dpram on qbus */

    if(qbusSlaveWindowSet(sysQbusId,0,QBUS_SLAVE_WIN_16K) == -1)
    {
	printf("usrQbusConfig: qbusSlaveWindowSet failed\n");
	return -1;
    }

    /* connect tasks end interrupt handler */

    if(qbusIntConnect(sysQbusId,QBUS_IV_TASK,taskIntr,0) == -1)
    {
	printf("usrQbusConfig: qbusIntConnect failed\n");
	return -1;
    }

    /* connect slave interrupts handlers */

    if(qbusIntConnect(sysQbusId,QBUS_IV_IRQ1,slaveIntr,1) == -1)
    {
	printf("usrQbusConfig: qbusIntConnect failed\n");
	return -1;
    }

    if(qbusIntConnect(sysQbusId,QBUS_IV_IRQ2,slaveIntr,2) == -1)
    {
	printf("usrQbusConfig: qbusIntConnect failed\n");
	return -1;
    }

    /* connect mailbox interrupt handler */

    if(qbusIntConnect(sysQbusId,QBUS_IV_MBOX,mboxIntr,0) == -1)
    {
	printf("usrQbusConfig: qbusIntConnect failed\n");
	return -1;
    }

    /* connect SCI0 interrupt handler */

    if(qbusIntConnect(sysQbusId,QBUS_IV_SCI0,sciIntr,0) == -1)
    {
	printf("usrQbusConfig: qbusIntConnect failed\n");
	return -1;
    }

    /* setup slave interrupt vector address */

    *QBUS_REG(sysQbusId,QBUS_REG_SLAVE_IRQ1) = CT_LE_L(0 << 15);
    *QBUS_REG(sysQbusId,QBUS_REG_SLAVE_IRQ2) = CT_LE_L(4 << 15);

    /* enable master and slave control */

    *QBUS_REG(sysQbusId,QBUS_REG_CTRL) |= CT_LE_L(QBUS_CTRL_MASTER_ON |
	QBUS_CTRL_SLAVE_ON);

    /* enable tasks end, mailbox, SCI0 and slave interrupts */

    *QBUS_REG(sysQbusId,QBUS_REG_INT_ENA) |= CT_LE_L(QBUS_INT_TASK |
	QBUS_INT_IRQ1 | QBUS_INT_IRQ2 | QBUS_INT_MBOX | QBUS_INT_SCI0);

#endif

    return 0;
}
