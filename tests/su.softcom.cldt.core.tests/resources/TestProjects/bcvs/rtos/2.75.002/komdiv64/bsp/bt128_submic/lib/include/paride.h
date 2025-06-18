/* paride.h - parallel IDE driver header */

/* 
modification history
--------------------
    20dec01,sok  written based on Linux.
*/

#ifndef	__INCparideh
#define	__INCparideh

#include <blkIo.h>
#include <lptDrv.h>
#include <time.h>
#include <arch/board.h>
#include <board.h>
#if OS_VERSION == 206
  #include <inttypes.h>
#endif

/* device type enumeration */

#define	PI_PD	0	/* IDE disk */
#define PI_PCD	1	/* ATAPI CDrom */
#define PI_PF   2	/* ATAPI disk */
#define PI_PT	3	/* ATAPI tape */
#define PI_PG   4       /* ATAPI generic */

/* macros exported to the protocol modules */

static inline void outb(unsigned char val, uintptr_t port)
{
    *(volatile unsigned char*)port = val;
}

static inline unsigned char inb(uintptr_t port)
{
    return *(volatile unsigned char*)port;
}

static inline void outw(unsigned short val, uintptr_t port)
{
    *(volatile unsigned short*)port = val;
}

static inline unsigned short inw(uintptr_t port)
{
    return *(volatile unsigned short*)port;
}

static inline void outl(unsigned int val, uintptr_t port)
{
    *(volatile unsigned int*)port = val;
}

static inline unsigned int inl(uintptr_t port)
{
    return *(volatile unsigned int*)port;
}

#define delay_p			(pi->delay ? udelay(pi->delay) : 0)
#define out_p(offset,byte)	outb((byte),pi->port+(offset)); delay_p;
#define in_p(offset)		(delay_p,inb(pi->port+(offset)))

#define w0(byte)                {out_p(0,byte);}
#define r0()                    (in_p(0))
#define w1(byte)                {out_p(1,byte);}
#define r1()                    (in_p(1))
#define w2(byte)                {out_p(2,byte);}
#define r2()                    (in_p(2))
#define w3(byte)                {out_p(3,byte);}
#define w4(byte)                {out_p(4,byte);}
#define r4()                    (in_p(4))
#define w4w(data)     		{outw(data,pi->port+4); delay_p;}
#define w4l(data)     		{outl(data,pi->port+4); delay_p;}
#define r4w()         		(delay_p,inw(pi->port+4))
#define r4l()         		(delay_p,inl(pi->port+4))

/* parallel ide protocol descriptor */

typedef struct pi_protocol {
    char	name[8];	/* name for this protocol */
    int		index;		/* index into protocol table (unused) */
    int		max_mode;	/* max mode number */
    int		epp_first;	/* modes >= this use 8 ports (unused) */
    int		default_delay;  /* delay parameter if not specified in us */
    int		max_units;	/* max chained units probed for */
    FUNCPTR	write_regr;	/* read IDE register */
    FUNCPTR	read_regr;	/* write IDE register */
    FUNCPTR	write_block;	/* read sector */
    FUNCPTR	read_block;	/* write sector */
    FUNCPTR	connect;	/* connect adapter */
    FUNCPTR	disconnect;	/* disconnect adapter */
    FUNCPTR	test_port;	/* hardware reset */
    FUNCPTR	probe_unit;	/* probe specified unit in chain */
    FUNCPTR	test_proto;	/* test current pi mode */
    FUNCPTR	log_adapter;	/* display information about adapter */
    FUNCPTR	init_proto;	/* unused */
    FUNCPTR	release_proto;	/* unused */
} PIP;

/* parallel ide adapter descriptor */

typedef struct pi_adapter {
    LPT_CTRL*	lpt;		/* controller descriptor */
    PIP*	proto;		/* adapter protocol */
    uintptr_t	port;		/* base address of parallel port */
    int		mode;		/* transfer mode in use */
    int		delay;		/* adapter delay setting */
    int		devtype;	/* device type: PI_PD etc. */
    char*	device;		/* name of driver */
    int		unit;		/* unit number for chained adapters */
    int		saved_r0;	/* saved port state */
    int		saved_r2;	/* saved port state */
    int		reserved;	/* number of ports reserved (unused) */
    int		private;	/* for protocol module (unused) */
} PIA;

/* functions exported by paride to the high level drivers */

static inline void pi_write_regr( PIA *pi, int cont, int regr, int val)

{	pi->proto->write_regr(pi,cont,regr,val);
}

static inline int pi_read_regr( PIA *pi, int cont, int regr)

{	return pi->proto->read_regr(pi,cont,regr);
}

static inline void pi_write_block( PIA *pi, char * buf, int count)

{	pi->proto->write_block(pi,buf,count);
}

static inline void pi_read_block( PIA *pi, char * buf, int count)

{       pi->proto->read_block(pi,buf,count);
}

static inline void pi_connect( PIA *pi)

{	pi->proto->connect(pi);
}

static inline void pi_disconnect( PIA *pi)

{	pi->proto->disconnect(pi);
}

extern void udelay(int us);

static inline void mdelay(int ms)
{
    if(ms < 1000 / boardClkRateGet())
    {
	udelay(ms*1000);
    }
    else
    {
	struct timespec delay = {
	    tv_sec:	ms / 1000,
	    tv_nsec:	(ms % 1000) * 1000000
	};
	nanosleep(&delay,NULL);
    }
}

/* globals */

extern PIP on26;		/* OnSpec 90c26 protocol structure */

/* function declarations */

extern int pi_init(PIA *pi, 
	LPT_CTRL* lpt, 		/* base port address */
	int mode, 		/* -1 for autoprobe */
	int unit,		/* unit number, if supported */
	PIP* protocol, 		/* protocol to use */
	int delay, 		/* -1 to use adapter specific default */
	char * scratch, 	/* address of 512 byte buffer */
	int devtype,		/* device type: PI_PD, PI_PCD, etc ... */
	int verbose,		/* log verbose data while probing */
	char *device		/* name of the driver */
	);			/* returns 0 on failure, 1 on success */

#endif	/* __INCparideh */
