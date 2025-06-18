/* switch configuration descriptor */

typedef struct {
    unsigned char hop;
    unsigned char id;
    unsigned char checkPort;
    unsigned char* lut;
} RIO_SWITCH_CONFIG;

/* lookup tables */

/******************************************************************************/

// lut_tsin [id] == port of switch n where msgs to id must be sent 
static unsigned char lut_tsi0[RIO_ID_MAX] = { 3, 2, 1 };
static unsigned char lut_tsi1[RIO_ID_MAX] = { 2, 3, 1 };
static unsigned char lut_tsi2[RIO_ID_MAX] = { 1, 2, 3 };

static RIO_SWITCH_CONFIG switches_p0[] = {
    { 0, 0, 0, lut_tsi0},
    { 1, 1, 2, lut_tsi1},
    { 1, 2, 1, lut_tsi2},
};

static RIO_SWITCH_CONFIG switches_p1[] = {
    { 0, 1, 0, lut_tsi1},
    { 1, 0, 2, lut_tsi0},
    { 1, 2, 1, lut_tsi2},
};

static RIO_SWITCH_CONFIG switches_p2[] = {
    { 0, 2, 0, lut_tsi2},
    { 1, 0, 1, lut_tsi0},
    { 1, 1, 2, lut_tsi1},
};

RIO_SWITCH_CONFIG* rioCrateConfigBT5328[RIO_ID_MAX] = {
    switches_p0, switches_p1, switches_p2
};

/******************************************************************************/

/* check port status */

static int checkPortOK(RIO_SWITCH_CONFIG* cfg)
{
    unsigned val;

    /* check port OK */

    if(!cfg->hop)
    {
	val = rioRegs->ext.port[cfg->checkPort].errStat;
	val = CF_LE_L(val);
    }
    else
    {
	if(rioMaintIn(cfg->hop-1,PORT_ERR_STAT0+cfg->checkPort*0x20,cfg->id,&val) == -1)
	    return -1;
    }

    if(!(val & 2))
	return -2;

    return 0;
}

/******************************************************************************/

/* configure COMP_TAG and GEN_CTL regs */

static int configTag(unsigned hop, RIO_SWITCH_CONFIG* cfg)
{
    unsigned i, val;

    if(rioMaintOut(hop,CSR_COMP_TAG,cfg->id,hop|(cfg->id << 8)) == -1)
	return -1;

    for(i = 0; i < 5; i++)
    {
	if(rioMaintOut(hop,PORT_GEN_CTL,cfg->id,0x20000000) == -1)		// set discovered bit
	    return -1;

	if(i)
    	    udelay(i*2000);

	if(rioMaintIn(hop,PORT_GEN_CTL,cfg->id,&val) == -1)
	    return -1;

	if(val & 0x20000000)
	    return 0;

        udelay((i+1)*2000);

        printf("%s: set discovered bit error hop=%d id=%d val=%x retry=%d\n",__FUNCTION__,
    	    hop,cfg->id,val,i);
    }

    printf("%s: set discovered bit timeout hop=%d id=%d val=%x\n",__FUNCTION__,
        hop,cfg->id,val);
    return -1;
}

/******************************************************************************/

/* TSI500 switch configuration */

static int tsiConfig(RIO_SWITCH_CONFIG* cfg)
{
    unsigned i, port, p, ix, id = boardProcNumGet() + 1, val, nPort;

    // errata: first cycle after reset must be write

    for(ix = 0; ix < 4; ix++)
	rioMaintOut(cfg->hop,FAB_TEA_CTL0+ix*4,cfg->id,TUNDRA_FAB_TEA_CTL_VAL);

    udelay(1000);

    if(rioMaintIn(cfg->hop,PORT_GEN_CTL,cfg->id,&val) == -1)
	return -1;

    if(val & 0x20000000)	// check discovered bit
	return 1;

    if(rioMaintIn(cfg->hop,CSR_HB_DEV_ID_LOCK,cfg->id,&val) == -1)
	return -1;

    for(i = 0; i < 10 && (val & 0xffff) == 0xffff; i++)
    {
	udelay(10 + i*1000);

	if(rioMaintOut(cfg->hop,CSR_HB_DEV_ID_LOCK,cfg->id,id) == -1)
	    return -1;

	udelay(10 + i*1000);

	if(rioMaintIn(cfg->hop,CSR_HB_DEV_ID_LOCK,cfg->id,&val) == -1)
	    return -1;
    }

    if((val & 0xffff) == 0xffff)
    {
	printf("%s: error: id_lock=0xffff hop=%d id=%d\n",__FUNCTION__,
	    cfg->hop,cfg->id);
	return -1;
    }

    if(i > 1)
	printf("%s: retry=%d id_lock=%x hop=%d id=%d\n",__FUNCTION__,
	    i,val,cfg->hop,cfg->id);

    if((val & 0xffff) != id)
    {
	for(i = 0; i < 1000; i++)
	{
	    if(rioMaintIn(cfg->hop,PORT_GEN_CTL,cfg->id,&val) == -1)
		return -1;

	    if(val & 0x20000000)
		return 1;

	    udelay(10);
	}

	if(rioMaintIn(cfg->hop,CSR_HB_DEV_ID_LOCK,cfg->id,&id) == -1)
	    return -1;

	printf("%s: timeout hop=%d id=%d ctl=%08x id_lock=%x\n",__FUNCTION__,
	    cfg->hop,cfg->id,val,id);

	return -1;
    }

    for(i = 0, val = 0x88888888; i < RIO_ID_MAX; i++)
    {
        if(cfg->lut[i] == 0xff)
    	    continue;

	p = cfg->lut[i];

	val = (val & ~(0xf << i*4)) | (p << i*4);
    }

    for(port = 0; port < 4; port++)
    {
	if(rioMaintOut(cfg->hop,0x10a00+port*0x20000,cfg->id,val) == -1)
	    return -1;
    }

    if(rioMaintIn(cfg->hop,CAR_SWITCH_PI,cfg->id,&val) == -1)
	return -1;

    nPort = (val >> 8) & 0xff;

    for(ix = 0; ix < nPort; ix++)
    {
	if(rioMaintOut(cfg->hop,PORT_CTL+ix*0x20,cfg->id,0x44000000) == -1)
	    return -1;
	if(rioMaintOut(cfg->hop,PORT_ERR_STAT0+ix*0x20,cfg->id,0xffffffff) == -1)
	    return -1;
	if(rioMaintOut(cfg->hop,PORT_UNRECOV_ERR_ENABLE+ix*0x20000,cfg->id,0x00400d00) == -1)
	    return -1;
    }

    for(ix = 0; ix < 4; ix++)
    {
	if(rioMaintOut(cfg->hop,FAB_TEA_CTL0+ix*4,cfg->id,TUNDRA_FAB_TEA_CTL_VAL) == -1)
	    return -1;
    }

    if(rioMaintOut(cfg->hop,PORT_LT_CTL,cfg->id,TUNDRA_LT_CTL_VAL) == -1)
        return -1;

    return configTag(cfg->hop,cfg);
}

/******************************************************************************/

/* configure all switchs and boards */

int rioCfg(RIO_SWITCH_CONFIG* sw)
{
    int ix, res;
    RIO_SWITCH_CONFIG cfg;

    for(ix = 0; ix < RIO_ID_MAX; ix++)
    {
	if((res = checkPortOK(&sw[ix])) < 0)
	{
	    if(res == -2)
		printf("%s: hop=%d (switch=%d) id=%d port=%d not ready\n",__FUNCTION__,sw[ix].hop,ix, sw[ix].id,sw[ix].checkPort);
	    return res;
	}

        if((res = tsiConfig(&sw[ix])) < 0)
	    return res;

	printf("%s: switch %d configured OK\n\r",__FUNCTION__,ix);
    }

    cfg.hop = 2;
    cfg.checkPort = 3;

    for(ix = 0; ix < RIO_ID_MAX; ix++)
    {
	if(ix == boardProcNumGet())
	    continue;

	cfg.id = ix;

	if((res = checkPortOK(&cfg)) < 0)
	{
	    printf("%s: board=%d access failed\n",__FUNCTION__,ix);
	    return res;
	}

	if(rioMaintOut(2,CSR_BASE_DEV_ID,ix,(ix<<16)|ix) == -1)
	    return -1;

	if(configTag(2,&cfg) == -1)
	    return -1;
    }

    rioRegs->ext.genCtl |= CT_LE_L(0x20000000);
    printf("%s: DONE\n",__FUNCTION__);
    return 0;
}
