#define	I2C_ADDR	0
#define	I2C_DATA	1
#define	I2C_CTRL	2
#define	I2C_STAT	3
#define	I2C_CCR		3
#define	I2C_XADDR	4
#define	I2C_SRST	7

#define	CTRL_AACK	0x04
#define	CTRL_IFLG	0x08
#define	CTRL_STP	0x10
#define	CTRL_STA	0x20
#define	CTRL_ENAB	0x40
#define	CTRL_IEN	0x80

#define	I2C_TIMEOUT	1000

/******************************************************************************/

/* register access */

void i2cRegSet(unsigned ch, unsigned reg, unsigned char val)
{
    volatile unsigned char* p = (void*)(I2C_A_BASE + ch*0x10 + reg);
    *p = val;
}

unsigned char i2cRegGet(unsigned ch, unsigned reg)
{
    volatile unsigned char* p = (void*)(I2C_A_BASE + ch*0x10 + reg);
    return *p;
}

/******************************************************************************/

/* waiting condition */

int i2cCondWait(unsigned ch, unsigned char ctrl, unsigned char stat)
{
    unsigned ix;
    i2cRegSet(ch,I2C_CTRL,CTRL_ENAB|ctrl);
    for(ix  = 0; ix < I2C_TIMEOUT; ix++)
    {
	if(i2cRegGet(ch,I2C_STAT) == stat)
	    return 0;
	udelay(1);
    }
    return -1;
}

/******************************************************************************/

/* reset controller */

int i2cReset(unsigned ch)
{
    unsigned ix;
    i2cRegSet(ch,I2C_SRST,0);
    for(ix  = 0; ix < I2C_TIMEOUT; ix++)
    {
	if(i2cRegGet(ch,I2C_STAT) == 0xf8)
	    return 0;
	udelay(1);
    }
    return -1;
}

/******************************************************************************/

/* start condition */

int i2cStart(unsigned ch)
{
    return i2cCondWait(ch,CTRL_STA,0x08);
}

/******************************************************************************/

/* start repeat condition */

int i2cStart2(unsigned ch)
{
    return i2cCondWait(ch,CTRL_STA,0x10);
}

/******************************************************************************/

/* stop condition */

int i2cStop(unsigned ch)
{
    return i2cCondWait(ch,CTRL_STP,0xf8);
}

/******************************************************************************/

/* addr+w with ack condition */

int i2cAddrWA(unsigned ch, unsigned char addr)
{
    i2cRegSet(ch,I2C_DATA,addr);
    return i2cCondWait(ch,0,0x18);
}

/******************************************************************************/

/* addr+r with ack condition */

int i2cAddrRA(unsigned ch, unsigned char addr)
{
    i2cRegSet(ch,I2C_DATA,addr);
    return i2cCondWait(ch,0,0x40);
}

/******************************************************************************/

/* data write with ack condition */

int i2cDataWA(unsigned ch, unsigned char data)
{
    i2cRegSet(ch,I2C_DATA,data);
    return i2cCondWait(ch,0,0x28);
}

/******************************************************************************/

/* data read without ack condition */

int i2cDataR(unsigned ch)
{
    return i2cCondWait(ch,0,0x58);
}

/******************************************************************************/

/* get data register */

unsigned char i2cDataGet(unsigned ch)
{
    return i2cRegGet(ch,I2C_DATA);
}

/******************************************************************************/

/* set bus rate */

unsigned i2cRateSet(unsigned ch, unsigned rate)
{
    unsigned delta = -1, n, m, fscl, n1 = 0, m1 = 0, fscl1 = 0;

    for(n = 0; n < 8; n++)
    {
	for(m = 0; m < 16; m++)
	{
	    fscl = boardFreqRef / ((1 << n) * (m + 1) * 10);

	    if(fscl > rate)
		continue;

	    if(rate - fscl < delta)
	    {
		delta = rate - fscl;
		n1 = n;
		m1 = m;
		fscl1 = fscl;
		if(!delta)
		    break;
	    }
	}
    }

    if(!fscl1)
	return 0;

    i2cRegSet(ch,I2C_CCR,n1|(m1<<3));
    return fscl1;
}

/******************************************************************************/

/* globals */

int boardTSensorInitDone = FALSE;

/* locals */

static pthread_mutex_t max1989Mutex;

/******************************************************************************/

/* initialization */

int boardTSensorInit(void)
{
    pthread_mutex_init(&max1989Mutex,NULL);
    i2cRateSet(0,100000);
    boardTSensorInitDone = TRUE;
    return 0;
}

/******************************************************************************/

/* read sensor register */

int max1989RegGet(unsigned char reg, unsigned char* val)
{
    int res = 0;
    if(!boardTSensorInitDone)
	return -1;
    pthread_mutex_lock(&max1989Mutex);
    i2cReset(0);
    if(i2cStart(0) == -1)
	goto fail;
    if(i2cAddrWA(0,0x30) == -1)
	goto fail;
    if(i2cDataWA(0,reg) == -1)
	goto fail;
    if(i2cStart2(0) == -1)
	goto fail;
    if(i2cAddrRA(0,0x31) == -1)
	goto fail;
    if(i2cDataR(0) == -1)
	goto fail;
    *val = i2cRegGet(0,I2C_DATA);
    if(i2cStop(0) == -1)
	goto fail;
done:
    pthread_mutex_unlock(&max1989Mutex);
    return res;
fail:
    res = -1;
    goto done;
}

/******************************************************************************/

/* write sensor register */

int max1989RegSet(unsigned char reg, unsigned char val)
{
    int res = 0;
    if(!boardTSensorInitDone)
	return -1;
    pthread_mutex_lock(&max1989Mutex);
    i2cReset(0);
    if(i2cStart(0) == -1)
	goto fail;
    if(i2cAddrWA(0,0x30) == -1)
	goto fail;
    if(i2cDataWA(0,reg) == -1)
	goto fail;
    if(i2cDataWA(0,val) == -1)
	goto fail;
    if(i2cStop(0) == -1)
	goto fail;
done:
    pthread_mutex_unlock(&max1989Mutex);
    return res;
fail:
    res = -1;
    goto done;
}

/******************************************************************************/

/* read sensor temperature, connection: 0 - internal, 1 - PCI-RIO, 2 - TUNDRA, 3 - CPU */

int boardTSensorGet(unsigned ch, unsigned char* val)
{
    if(!val || ch > 3)
	return -1;
    return max1989RegGet(ch,val);
}

#if 0

void mr(unsigned char cmd)
{
    unsigned char val; char* state;
    i2cReset(0);
    i2cRateSet(0,100000);
    state = "start";
    if(i2cStart(0) == -1)
	goto fail;
    state = "addr+wa";
    if(i2cAddrWA(0,0x30) == -1)
	goto fail;
    state = "data_w+a";
    if(i2cDataWA(0,cmd) == -1)
	goto fail;
    state = "start2";
    if(i2cStart2(0) == -1)
	goto fail;
    state = "addr+ra";
    if(i2cAddrRA(0,0x31) == -1)
	goto fail;
    state = "data_r+a";
    if(i2cDataR(0) == -1)
	goto fail;
    val = i2cRegGet(0,I2C_DATA);
    printf("reg=%02x val=%02x\n",cmd,val);
    state = "stop";
    if(i2cStop(0) == -1)
	goto fail;
    return;
fail:
    val = i2cRegGet(0,I2C_STAT);
    printf("state=%s stat=%02x\n",state,val);
}

void mw(unsigned char cmd, unsigned char val)
{
    char* state;
    i2cReset(0);
    i2cRateSet(0,100000);
    state = "start";
    if(i2cStart(0) == -1)
	goto fail;
    state = "addr+wa";
    if(i2cAddrWA(0,0x30) == -1)
	goto fail;
    state = "reg_w+a";
    if(i2cDataWA(0,cmd) == -1)
	goto fail;
    state = "data_w+a";
    if(i2cDataWA(0,val) == -1)
	goto fail;
    state = "stop";
    if(i2cStop(0) == -1)
	goto fail;
    return;
fail:
    printf("state=%s stat=%02x\n",state,i2cRegGet(0,I2C_STAT));
}

#endif
