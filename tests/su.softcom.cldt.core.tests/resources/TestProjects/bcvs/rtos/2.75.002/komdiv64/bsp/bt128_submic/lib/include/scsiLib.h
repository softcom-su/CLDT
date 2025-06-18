/* scsiLib.h - SCSI library header file */

#ifndef __INCscsiLibh
#define __INCscsiLibh

/* 
modification history
--------------------
    23jul07,sok  remove REQUEST_SENSE type.
    01jun04,sok  set REQUEST_SENSE buffer size 20 bytes.
    15apr02,sok  add OC2000 v2.10 buf's chain support.
    15apr02,sok  remove vxWorks support.
    05oct01,sok  add sysScsiInit() declaration.
*/

#include <version.h>
#include <blkIo.h>

#ifdef __cplusplus
extern "C" {
#endif

/* SCSI OPCODES */

#define	SCSI_OPCODE_TEST_UNIT_READY	0x00
#define	SCSI_OPCODE_REZERO_UNIT		0x01
#define	SCSI_OPCODE_REQUEST_SENSE	0x03
#define	SCSI_OPCODE_FORMAT_UNIT		0x04
#define	SCSI_OPCODE_READ_BLOCK_LIMITS	0x05
#define	SCSI_OPCODE_REASSIGN_BLOCKS	0x07
#define	SCSI_OPCODE_READ_6		0x08
#define	SCSI_OPCODE_WRITE_6		0x0a
#define	SCSI_OPCODE_SEEK_6		0x0b
#define	SCSI_OPCODE_READ_REVERSE	0x0f
#define	SCSI_OPCODE_WRITE_FILEMARKS	0x10
#define	SCSI_OPCODE_SPACE		0x11
#define	SCSI_OPCODE_INQUIRY		0x12
#define	SCSI_OPCODE_RECOVER_BUF_DATA	0x14
#define	SCSI_OPCODE_MODE_SELECT		0x15
#define	SCSI_OPCODE_RESERVE		0x16
#define	SCSI_OPCODE_RELEASE		0x17
#define	SCSI_OPCODE_COPY		0x18
#define	SCSI_OPCODE_ERASE		0x19
#define	SCSI_OPCODE_MODE_SENSE		0x1a
#define	SCSI_OPCODE_START_STOP_UNIT	0x1b
#define	SCSI_OPCODE_RECEIVE_DIAGNOSTIC	0x1c
#define	SCSI_OPCODE_SEND_DIAGNOSTIC	0x1d
#define	SCSI_OPCODE_ALLOW_REMOVAL	0x1e
#define	SCSI_OPCODE_SET_WINDOW		0x24
#define	SCSI_OPCODE_READ_CAPACITY	0x25
#define	SCSI_OPCODE_READ_10		0x28
#define	SCSI_OPCODE_WRITE_10		0x2a
#define	SCSI_OPCODE_SEEK_10		0x2b
#define	SCSI_OPCODE_WRITE_VERIFY	0x2e
#define	SCSI_OPCODE_VERIFY		0x2f
#define	SCSI_OPCODE_SEARCH_HIGH		0x30
#define	SCSI_OPCODE_SEARCH_EQUAL	0x31
#define	SCSI_OPCODE_SEARCH_LOW		0x32
#define	SCSI_OPCODE_SET_LIMITS		0x33
#define	SCSI_OPCODE_PRE_FETCH		0x34
#define	SCSI_OPCODE_READ_POSITION	0x34
#define	SCSI_OPCODE_SYNCHRONIZE_CACHE	0x35
#define	SCSI_OPCODE_LOCK_UNLOCK_CACHE	0x36
#define	SCSI_OPCODE_READ_DEFECT_DATA	0x37
#define	SCSI_OPCODE_MEDIUM_SCAN		0x38
#define	SCSI_OPCODE_COMPARE		0x39
#define	SCSI_OPCODE_COPY_VERIFY		0x3a
#define	SCSI_OPCODE_WRITE_BUFFER	0x3b
#define	SCSI_OPCODE_READ_BUFFER		0x3c
#define	SCSI_OPCODE_UPDATE_BLOCK	0x3d
#define	SCSI_OPCODE_READ_LONG		0x3e
#define	SCSI_OPCODE_WRITE_LONG		0x3f
#define	SCSI_OPCODE_CHANGE_DEFINITION	0x40
#define	SCSI_OPCODE_WRITE_SAME		0x41
#define	SCSI_OPCODE_READ_TOC		0x43
#define	SCSI_OPCODE_LOG_SELECT		0x4c
#define	SCSI_OPCODE_LOG_SENSE		0x4d
#define	SCSI_OPCODE_MODE_SELECT_10	0x55
#define	SCSI_OPCODE_MODE_SENSE_10	0x5a
#define	SCSI_OPCODE_MOVE_MEDIUM		0xa5
#define	SCSI_OPCODE_READ_12		0xa8
#define	SCSI_OPCODE_WRITE_12		0xaa
#define	SCSI_OPCODE_WRITE_VERIFY_12	0xae
#define	SCSI_OPCODE_SEARCH_HIGH_12	0xb0
#define	SCSI_OPCODE_SEARCH_EQUAL_12	0xb1
#define	SCSI_OPCODE_SEARCH_LOW_12	0xb2
#define	SCSI_OPCODE_READ_ELEMENT_STATUS	0xb8
#define	SCSI_OPCODE_SEND_VOLUME_TAG	0xb6
#define	SCSI_OPCODE_WRITE_LONG_2	0xea

/* SCSI SENSE KEYS */

#define	SCSI_SENSE_KEY_NO_SENSE		0x00
#define	SCSI_SENSE_KEY_RECOVERED_ERROR	0x01
#define	SCSI_SENSE_KEY_NOT_READY	0x02
#define	SCSI_SENSE_KEY_MEDIUM_ERROR	0x03
#define	SCSI_SENSE_KEY_HARDWARE_ERROR	0x04
#define	SCSI_SENSE_KEY_ILLEGAL_REQUEST	0x05
#define	SCSI_SENSE_KEY_UNIT_ATTENTION	0x06
#define	SCSI_SENSE_KEY_DATA_PROTECT	0x07
#define	SCSI_SENSE_KEY_BLANK_CHECK	0x08
#define	SCSI_SENSE_KEY_VENDOR_SPECIFIC	0x09
#define	SCSI_SENSE_KEY_COPY_ABORTED	0x0a
#define	SCSI_SENSE_KEY_ABORTED_COMMAND	0x0b
#define	SCSI_SENSE_KEY_EQUAL		0x0c
#define	SCSI_SENSE_KEY_VOLUME_OVERFLOW	0x0d
#define	SCSI_SENSE_KEY_MISCOMPARE	0x0e
#define SCSI_SENSE_KEY_RESERVED		0x0f

/* SCSI STATUS CODES */

#define	SCSI_STATUS_MASK		0x3e
#define	SCSI_STATUS_GOOD		0x00
#define	SCSI_STATUS_CHECK_CONDITION	0x02
#define	SCSI_STATUS_CONDITION_GOOD	0x04
#define	SCSI_STATUS_BUSY		0x08
#define	SCSI_STATUS_INTERMEDIATE_GOOD	0x10
#define	SCSI_STATUS_INTERMEDIATE_C_GOOD	0x14
#define	SCSI_STATUS_RESERVATION_CONFL	0x18
#define	SCSI_STATUS_COMMAND_TERMINATED	0x22
#define	SCSI_STATUS_QUEUE_FULL		0x28

/* SCSI MESSAGE CODES */

#define	SCSI_MSG_COMMAND_COMPLETE	0x00
#define	SCSI_MSG_EXT_MESSAGE		0x01
#define	SCSI_MSG_SAVE_POINTERS		0x02
#define	SCSI_MSG_RESTORE_POINTERS	0x03
#define	SCSI_MSG_DISCONNECT		0x04
#define	SCSI_MSG_INITIATOR_ERROR	0x05
#define	SCSI_MSG_ABORT			0x06
#define	SCSI_MSG_MESSAGE_REJECT		0x07
#define	SCSI_MSG_NOP			0x08
#define	SCSI_MSG_MSG_PARITY_ERROR	0x09
#define	SCSI_MSG_LINKED_CMD_COMPLETE	0x0a
#define	SCSI_MSG_LINKED_FL_CMD_COMPLETE	0x0b
#define	SCSI_MSG_BUS_DEVICE_RESET	0x0c
#define	SCSI_MSG_ABORT_TAG		0x0d
#define	SCSI_MSG_INITIATE_RECOVERY	0x0f	/* SCSI-II only */
#define	SCSI_MSG_RELEASE_RECOVERY	0x10	/* SCSI-II only */
#define	SCSI_MSG_TERMINATE_IO_PROCESS	0x11
#define	SCSI_MSG_SIMPLE_QUEUE_TAG	0x20
#define	SCSI_MSG_HEAD_OF_QUEUE_TAG	0x21
#define	SCSI_MSG_ORDERED_QUEUE_TAG	0x22
#define	SCSI_MSG_IGNORE_WIDE_RESIDUE	0x23
#define SCSI_MSG_IDENT_DISCONNECT	0x40
#define SCSI_MSG_IDENTIFY		0x80

/* SCSI EXTENDED MESSAGE CODES */

#define	SCSI_MSG_EXT_MODIFY_DATA_PTR	0x00
#define	SCSI_MSG_EXT_SDTR		0x01
#define	SCSI_MSG_EXT_EXTENDED_IDENTIFY	0x02	/* SCSI-I only */
#define	SCSI_MSG_EXT_WDTR		0x03

/* SCSI BUS PHASES */

#define SCSI_PHASE_DATA_OUT		0x0
#define SCSI_PHASE_DATA_IN		0x1
#define SCSI_PHASE_COMMAND		0x2
#define SCSI_PHASE_STATUS		0x3
#define SCSI_PHASE_RESERVED		0x4
#define SCSI_PHASE_MSG_OUT		0x6
#define SCSI_PHASE_MSG_IN		0x7
#define SCSI_PHASE_BUS_FREE		0x8

/* SCSI DEVICE TYPE */

#define	SCSI_TYPE_DIRECT_ACCESS		0x00
#define	SCSI_TYPE_SEQUENTIAL_ACCESS	0x01
#define	SCSI_TYPE_PRINTER		0x02
#define	SCSI_TYPE_PROCESSOR		0x03
#define	SCSI_TYPE_WRITE_ONES		0x04
#define	SCSI_TYPE_CDROM			0x05
#define	SCSI_TYPE_SCANNER		0x06
#define	SCSI_TYPE_OPTICAL_MEMORY	0x07
#define	SCSI_TYPE_MEDIUM_CHANGER	0x08
#define	SCSI_TYPE_COMMUNICATIONS	0x09
#define	SCSI_TYPE_UNKNOWN		0x1f

/* HOST RETURN CODE */

#define DID_OK		0x00	/* no error */
#define DID_NO_CONNECT	0x01	/* couldn't connect before timeout period  */
#define DID_BUS_BUSY	0x02	/* bus stayed busy through time out period */
#define DID_TIME_OUT	0x03	/* timed out for other reason */
#define DID_BAD_TARGET	0x04	/* bad target */
#define DID_ABORT	0x05	/* told to abort for some other reason */
#define DID_PARITY	0x06	/* parity error */
#define DID_ERROR	0x07	/* internal error */
#define DID_RESET	0x08	/* reset by somebody */
#define DID_BAD_INTR	0x09	/* got an interrupt we weren't expecting */ 
#define DID_SOFT_ERROR	0x0b	/* the low level driver just wish a retry */

/* ABORT RETURN CODE */

#define	SCSI_ABORT_SNOOZE	0	/* we did not do anything */
#define	SCSI_ABORT_SUCCESS	1	/* command really aborted */
#define	SCSI_ABORT_PENDING	2	/* wait some time befor abort done */
#define	SCSI_ABORT_BUSY		3	/* unable to abort, wait */
#define	SCSI_ABORT_NOT_RUNNING	4	/* command probably finished */
#define	SCSI_ABORT_ERROR	5	/* something went wrong */

/* data transfer direction */

#define	DEV_TO_MEM	0	/* read from device */
#define	MEM_TO_DEV	1	/* write to device */

/* tag manipulation */

#define	TAG_NEXT	(device->tag = (device->tag + 1) & 0xff)
#define	TAG_NONE	0xffffffff

/* debug print */

#define	SCSI_DEBUG(level,msg)		\
{					\
    if(scsiDebug >= (level))		\
	logMsg msg;			\
}

#define	SCSI_INTS_DEBUG(level,msg)	\
{					\
    if(scsiIntsDebug >= (level))	\
	logMsg msg;			\
}

/* call routine for each attached device */

#define	FOR_EACH_DEVICE(host,routine, args...)		\
{							\
    int _id, _lun;					\
							\
    for(_id = 0; _id < (host)->wide; _id++)		\
    {							\
	for(_lun = 0; _lun < 8; _lun++)			\
	{						\
	    if(!(device = (host)->devs[_id][_lun]))	\
		continue;				\
							\
	    routine (args);				\
	}						\
    }							\
}

/* typedefs */

typedef struct scsiCommand SCSI_COMMAND;
typedef struct scsiDevice SCSI_DEVICE;

/* SCSI_HOST - generic SCSI host adapter info */

struct scsi_host {
    FUNCPTR		queue;		/* queue command */
    FUNCPTR		abort;		/* abort command */
    FUNCPTR		reset;		/* do bus reset */
    FUNCPTR		ioctl;		/* do something */
    SCSI_DEVICE*  	devs[16][8];	/* attached devices */
    unsigned		maxXfer;	/* maximum transaction bytes */
    int			wide;		/* 8, 16, or 32 */
#if OS_VERSION >= 210 && defined(KERNEL)
    BLK_QUEUE		bq;		/* block device queue descriptor */
#endif
};

typedef struct scsi_host SCSI_HOST;

/* SCSI_DEVICE - SCSI device info */

struct scsiDevice {
    SCSI_HOST*    host;		/* controller info */
    SCSI_COMMAND* list;		/* list of disconnected commands */
    unsigned char id, lun;	/* device's SCSI bus ID and LUN */
    unsigned char syncOffset;	/* synchronous xfer offset */
    unsigned char syncPeriod;	/* synchronous xfer period */
    unsigned timeout;		/* command done timeout in seconds */
    unsigned spinup;		/* spin up time in seconds */
    unsigned syncXfer	: 1;	/* TRUE for synchronous data transfer */
    unsigned wideXfer	: 1;	/* TRUE for wide data transfer */
    unsigned syncNeg	: 1;	/* TRUE if sync negotiation pended */
    unsigned wideNeg	: 1;	/* TRUE if wide negotiation pended */
    unsigned wideOK	: 1;	/* TRUE if wide negotiation successefully passed */
    unsigned tagged	: 1;	/* SCSI-II tagged queuing enabled */
    unsigned disconnect	: 1;	/* whether device supports disconnect */
    unsigned removable	: 1;	/* whether medium is removable */
    unsigned writeable	: 1;	/* whether medium is writeable */
    unsigned busy	: 1;	/* whether device is busy */
    unsigned changed	: 1;	/* whether get UNIT_ATTENTION sense key */
    unsigned depth;		/* current tagged commands queue depth */
    unsigned maxDepth;		/* maximum tagged commands queue depth */
    unsigned capacity;		/* device capacity in sectors */
    unsigned bytesPerBlk;	/* size of sector in bytes */
    unsigned type;		/* device type */
    unsigned tag;		/* current tag */
    char vendor[8], model[16], rev[4]; /* inquiry data */
};

/* definitions relating to SCSI commands */

struct scsiCommand {
    SCSI_COMMAND*	next;		/* used for linked list */
    SCSI_DEVICE*	device;		/* ptr to SCSI device info */
    unsigned		cmdLen;		/* command length */
    unsigned char	cmd[12];	/* command data */
    unsigned		tag;		/* command tag */
    void*		buf;		/* data buffer or head of bufs chain */
    unsigned		len;		/* total data length */
/* Sveta */
    unsigned		chained : 1;	/* TRUE if bufs chain used */
    //int			chained : 1;	/* TRUE if bufs chain used */
/* Sveta */
    unsigned		dir : 1;	/* DEV_TO_MEM (0) or MEM_TO_DEV (1) */
    //int			dir     : 1;	/* DEV_TO_MEM (0) or MEM_TO_DEV (1) */
    void*		driver1;	/* for private use by the driver */
    void*		driver2;	/* for private use by the driver */
    void*		driver3;	/* for private use by the driver */
    void*		driver4;	/* for private use by the driver */
    unsigned char	tag_msg;	/* SCSI_MSG_XXX_QUEUE_TAG */
    unsigned char	phase;		/* saved phase */
    unsigned char	result;		/* result word */
    unsigned char	status;		/* status byte */
    unsigned char	msg;		/* message byte */
    VOIDFUNCPTR		done;		/* command done routine */
    void*		spare;		/* for use by the done routine */
    struct scsiCommand*	sense;		/* sense buffer and command or NULL */
};

/* block device structure */

typedef struct {
    BLK_DEV		bd;		/* generic block device info */
    SCSI_DEVICE*	device;		/* physical device info */
    unsigned		offset;		/* start block offset */
} SCSI_BLK_DEV;

/* globals */

extern int scsiDebug;			/* task debug level */
extern int scsiIntsDebug;		/* interrupt debug level */
extern const char* scsiPhase[];		/* phase names strings */
extern const char* scsiStatus[];	/* status names strings */
extern const char* scsiMessage[];	/* message names strings */
extern const char* scsiExtMessage[];	/* extended message names strings */
extern const char* scsiDidCode[];	/* command did result name array */
extern const char* scsiSenseKey[];	/* request sense key name array */
extern SCSI_HOST* pSysScsiHost;		/* implicite ptr to system host adapter */

/* function declarations */

void scsiDebugSet(int);
void scsiIntsDebugSet(int);
void scsiSenseDump(SCSI_COMMAND* cmd);
void scsiCmdBuild(SCSI_COMMAND* cmd, SCSI_DEVICE* device, int cmdLen, int dir,
    void* buf, int len, VOIDFUNCPTR done, void* spare, SCSI_COMMAND* sense);
STATUS scsiCmdIssue(SCSI_COMMAND* cmd);

STATUS scsiReqSense(SCSI_DEVICE* device, char* buf, int len);
STATUS scsiInquiry(SCSI_DEVICE* device, char* buf, int len);
STATUS scsiTestUnitReady(SCSI_DEVICE* device, unsigned* pSenseKey);
STATUS scsiStartStopUnit(SCSI_DEVICE* device, int start, int immed, int eject);
STATUS scsiReadCapacity(SCSI_DEVICE* device, int* capacity, int* bytesPerBlk);
long scsiRead(SCSI_DEVICE* device, int sec, int nsec, char* buf);
long scsiWrite(SCSI_DEVICE* device, int sec, int nsec, char* buf);
STATUS scsiModeSelect(SCSI_DEVICE* device, int pf, int save, char* buf, int len);
STATUS scsiModeSelect10(SCSI_DEVICE* device, int pf, int save, char* buf, int len);
STATUS scsiModeSense(SCSI_DEVICE* device, int dbd, int page, char* buf, int len);
STATUS scsiModeSense10(SCSI_DEVICE* device, int dbd, int page, char* buf, int len);
STATUS scsiReadToc(SCSI_DEVICE* device, int msf, char* buf, int len);
STATUS scsiPreventAllowMediumRemoval(SCSI_DEVICE* device, int prevent);

STATUS scsiHostInit(SCSI_HOST* host, FUNCPTR queue, FUNCPTR abort,
    FUNCPTR reset, FUNCPTR ioctl, unsigned maxXfer, int wide,
    int priority);
SCSI_DEVICE* scsiDevCreate(SCSI_HOST* host, int id, int lun, int disconnect,
    int syncXfer, int wideXfer, int tagged, int timeout, int spinup);
void scsiDevDelete(SCSI_DEVICE* device);
BLK_DEV* scsiBlkDevCreate(SCSI_DEVICE* device, unsigned size, unsigned offset);
STATUS scsiBlkDevDelete(BLK_DEV* bd);
STATUS scsiBusReset(SCSI_HOST* host);
STATUS scsiSpinup(SCSI_DEVICE* device);
STATUS scsiPowerDown(SCSI_HOST* host);
STATUS scsiPowerUp(SCSI_HOST* host);
SCSI_DEVICE* scsiDevDescGet(SCSI_HOST* host, int id, int lun);
STATUS scsiAutoConfig(SCSI_HOST* host, int configLuns);
STATUS scsiShow(SCSI_HOST* host);
int scsiLibVersion(int silence);
STATUS scsiExtAutoConfig(SCSI_HOST* host, int configLuns, int disconnect,
    int sync, int wide, int tagged, int tout, int spinup);

STATUS sysScsiInit(void);

#if OS_VERSION >= 210
BLK_IO_DEV* scsiBlkDevInstall(SCSI_DEVICE* device, unsigned size,
    unsigned offset, char* name, int usePart);
#endif

#ifdef __cplusplus
}
#endif

#endif /* __INCscsiLibh */
