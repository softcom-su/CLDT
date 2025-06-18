/* if_de.h - header for DEC 21143 10/100-Mb/s ethernet driver */

#ifndef	__INCif_deh
#define	__INCif_deh

/* transmit descriptor status bits */

#define	TDS_DE		0x00000001	/* deferred */
#define	TDS_UF		0x00000002	/* under flow error */
#define	TDS_LF		0x00000004	/* link fail report */
#define	TDS_HF		0x00000080	/* heartbeat fail */
#define	TDS_EC		0x00000100	/* excessive collisions */
#define	TDS_LC		0x00000200	/* late collision */
#define	TDS_NC		0x00000400	/* no carrier */
#define	TDS_LO		0x00000800	/* loss of carrier */
#define	TDS_TO		0x00004000	/* transmit jabber timeout */
#define	TDS_ES		0x00008000	/* error summary */

/* valid errors in Full and Half Duplex modes */

#define	TDS_ERR_FD	(TDS_UF|TDS_LF|TDS_EC|TDS_LC|TDS_TO)
#define	TDS_ERR_HD	(TDS_ES)
#define	TDS_ERR_LOOP	(TDS_UF|TDS_TO)

/* receive descriptor status bits */

#define	RDS_CE		0x00000002	/* CRC error */
#define	RDS_DB		0x00000004	/* dribbling bit */
#define	RDS_RE		0x00000008	/* report on MII error */
#define	RDS_RW		0x00000010	/* receive watchdog */
#define	RDS_FT		0x00000020	/* frame type */
#define	RDS_CS		0x00000040	/* collision seen */
#define	RDS_TL		0x00000080	/* frame too long */
#define	RDS_LS		0x00000100	/* last descriptor */
#define	RDS_FS		0x00000200	/* first descriptor */
#define	RDS_MF		0x00000400	/* multicast frame */
#define	RDS_RF		0x00000800	/* runt frame */
#define	RDS_DE		0x00004000	/* descriptor error */
#define	RDS_ES		0x00008000	/* error summary */
#define	RDS_FF		0x40000000	/* filtering fail */

/* MII media type */

#define	MII_10HD	0x0020		/* 10 Mbit/s half duplex */
#define	MII_10FD	0x0040		/* 10 Mbit/s full duplex */
#define	MII_100HD	0x0080		/* 100 Mbit/s half duplex */
#define	MII_100FD	0x0100		/* 100 Mbit/s full duplex */
#define	MII_AS		0x01e0		/* autosense: all above bits */
#define	MII_FD		0x0800		/* full duplex */
#define	MII_100		0x1000		/* 100 Mbit/s */
#define	MII_FIXED	0x2000		/* no autonegotiation for TP */
#define	MII_RESET	0x4000		/* forced reset */
#define	MII_FX		0x8000		/* forced fiber mode if bad SROM */

/* extra bits of media param */

#define	DE_SROM_BUG	0x10000		/* fix K64SMP SROM read direction bug */

/* media connection type */

#define	DE_TYPE_10BT_TP		0x0000
#define	DE_TYPE_10BT_NWAY	0x0100
#define	DE_TYPE_10BT_FD		0x0204
#define	DE_TYPE_10B2_BNC	0x0001
#define	DE_TYPE_10B5_AUI	0x0002
#define	DE_TYPE_100TX_SYM	0x0003
#define	DE_TYPE_100TX_FD	0x0205
#define	DE_TYPE_100T4		0x0006
#define	DE_TYPE_100FX		0x0007
#define	DE_TYPE_100FX_FD	0x0208
#define	DE_TYPE_MII_10BT	0x0009
#define	DE_TYPE_MII_10BT_FD	0x020a
#define	DE_TYPE_MII_100TX	0x000d
#define	DE_TYPE_MII_100TX_FD	0x020e
#define	DE_TYPE_MII_100T4	0x000f
#define	DE_TYPE_MII_100FX	0x0010	/* supported by deSromInit() */
#define	DE_TYPE_MII_100FX_FD	0x0211	/* supported by deSromInit() */
#define	DE_TYPE_DYNAMIC_AS	0x0800	/* supported by deSromInit() */
#define	DE_TYPE_POWERUP_AS	0x8800
#define	DE_TYPE_NO_MEDIA	0xffff

/* deControl() commands */

#define	DE_PORT_SET	1		/* set active MII */
#define	DE_PORT_GET	2		/* get active MII */
#define	DE_PORT_RESET	3		/* reset MII */
#define	DE_CHIP_RESET	4		/* software chip reset */
#define	DE_INT_LOOPBACK	5		/* enable internal loopback mode */
#define	DE_MII_LOOPBACK	6		/* enable MII loopback mode */
#define	DE_EXT_LOOPBACK	7		/* enable external loopback mode */

/* events enumeration */

#define	EVENT_LINK_UP		1	/* link established */
#define	EVENT_LINK_FAIL		2	/* link failed */
#define	EVENT_WATCHDOG		3	/* watchdog called */
#define	EVENT_TX_ERROR		4	/* bad tx frame status */
#define	EVENT_RX_ERROR		5	/* bad rx frame status */
#define	EVENT_RX_NOMEM		6	/* no more mbufs */
#define	EVENT_RX_DROP		7	/* no more mbufs: rx frame dropped */
#define	EVENT_RESET		8	/* reset called */
#define	EVENT_FBE		9	/* fatal bus error (from interrupt) */
#define	EVENT_ERROR		10	/* some error (from interrupt) */
#define	EVENT_IFJOBADD		11	/* if_jadd() failed (from interrupt) */

/* SC64 embedded ethernet unit number */

#define	DE_UNIT_SC64	9

/* user hook call */

#define	DE_EVENT(name,arg)				\
    if(de->userRtn)					\
	de->userRtn(de->userArg,EVENT_##name,arg);

/* controller descriptor type */

typedef struct de_ctrl DE_CTRL;

/* globals */

extern int deDebug;			/* debug level */

/* function declarations */

STATUS deDevInit(int unit, unsigned csr11, int ttm, unsigned media);
STATUS deHookSet(int unit, FUNCPTR rtn, int arg);
int deControl(int unit, int cmd, int arg);
int deDrvVersion(int silence);
void deShow(int unit);
void deBenchStart(int unit);
void deBenchStop(int unit);
void deBenchShow(int unit);
int deSromInit(unsigned unit, const char* mac, unsigned short connectionType);
int deSromShow(int unit);
unsigned short deSromCrcGet(unsigned char* data);
unsigned char deIdCrcGet(unsigned short* data);
int deSromSet(DE_CTRL* de, unsigned short* data);
void deSromGet(DE_CTRL* de, unsigned short* buf);
DE_CTRL* deIfUnit(int unit);

#endif	/* __INCif_deh */
