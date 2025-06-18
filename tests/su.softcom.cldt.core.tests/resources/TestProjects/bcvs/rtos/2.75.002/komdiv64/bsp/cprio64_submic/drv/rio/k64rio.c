
/******************************************************************************/

int boardRioIdGet(void)
{
    unsigned val;
    if(!rioRegs)
	return -1;
    val = rioRegs->csr.baseDevID;
    return (CF_LE_L(val) >> 16) & 0xff;
}

/******************************************************************************/

extern void k64MsgInit(void);
void _k64MsgInit(int syncId, unsigned syncTout);
extern int rregTagMasterSync(unsigned id, unsigned tout); //gmu: for bcvs
extern int submicronSyncResult;  //gmu: for bcvs

void rioInit(int unused)
{
    unsigned id = 0;

    rioRegs = (void*)RIO_REGS_BASE;

    id = boardRioIdGet();
    boardProcNumSet(id);
    PRINTF("%s: this board ID=%d\n",__FUNCTION__,id);

    if((*(volatile unsigned*)(SYSTEM_REGS+0x154) >> 16) < 0xb023)
    {
        rioCtrlRev = 0;
        rioMsgMode = 0;
    }
    else
    {
        rioCtrlRev = 1;
#ifdef FORCED_RIO_OLD_MSG
        rioMsgMode = 0;
        rioRegs->user.pciRioMode &= ~CT_LE_L(0x20);
        printf("%s: controller configured for old message interface!!!\n",__FUNCTION__,id);
#else
        rioRegs->user.pciRioMode |= CT_LE_L(0x20);
        rioMsgMode = 1;
#endif
    }

#if defined(INCLUDE_SHOW_INIT)
    rioDate(); //gmu: вызываем только если нужно выводить сообщения об инициализации
#endif

    rioRegs->csr.cfgLBAR = CT_LE_L(0x1000000 >> 24);    // [9:0] = bridge regs on RIO / 16MB

    rioRegs->err.status1 = 0xffffffff;                  // clear additional status
    rioRegs->ext.ltCtl = CT_LE_L(HOST_LT_CTL_VAL);      // Link Time-Out
#warning test RT_CTL value
    rioRegs->ext.rtCtl = CT_LE_L(0xff00);      // Response Time-Out

    // inherit SRIO settings, enable regs access, doorbell's, messages and NWRITE_R?
    rioRegs->spec.enable = (rioRegs->spec.enable & CT_LE_L(0x18000000)) | CT_LE_L(0x04c06000);
    rioRegs->errCtrl.logErrDetEn = CT_LE_L(0xdfc00000); // enable low level errors detection
    rioRegs->errCtrl.logErrDet = 0;                     // clear low level error flags
    rioRegs->errCtrl.pktTTL = CT_LE_L(HOST_PKT_TTL_VAL);// set TTL
    rioRegs->endp.intrEnable = 0;                       // disable all interrupts

    if(boardProcNumGet() && boardProcNumGet() != 0xff)
    {
#ifdef FOR_SUBMICRON
   #warning   !!!! SUBMICRON version of k64rio.c !!!!!!!!!!!        
	// _k64MsgInit(SUBMICRON_SYNC_ID,SUBMICRON_SYNC_TOUT); // gmu:old
        // gmu:for bcvs 
        if(((*IO_PIN_B)&7)!=2) //gmu: if no technical mode
        {
            PRINTF("Waiting sync with VM7 ...");
            submicronSyncResult = rregTagMasterSync(SUBMICRON_SYNC_ID,SUBMICRON_SYNC_TOUT);
        }
        else
        {
           _k64MsgInit(SUBMICRON_SYNC_ID,SUBMICRON_SYNC_TOUT);
        }
        time_sync_VM6_VM7 = boardCountGet(); //время синхронизации по РИО
        // gmu: --- ---            
#else
	k64MsgInit();
#endif
    }

    intConnect(IV_RIO,(void*)rioIntr,0);
    boardIvEnable(IV_RIO);

#if OS_VERSION >= 300
    pshared_mutex_init(&maintMutex,"rioMaint");
#else
    pthread_mutex_init(&maintMutex,NULL);
#endif

    dbInInit(1);
    rioInitDoneFlag = TRUE;
    return;
}
