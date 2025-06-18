/* saa.h - video controller header */

#ifndef __saa
#define __saa

#include <version.h>
#include <private/ios.h>
#include <semaphore.h>
#include <logLib.h>

#define SAA_DEBUG \
    if (saaDebug) logMsg

#define MAX_SAA_NUM		1

#ifndef BTMPC4_402_PCI_SDRAM_ID
#define BTMPC4_402_PCI_SDRAM_ID	0x7778
#endif
#define BTMPC4_402_PCI_VI_ID	0x777A

/* VI_CSR */

#define SAA_VI_START		0x80000000
#define SAA_VI_OVF		0x20000000
#define SAA_VI_TMO		0x10000000
#define SAA_VI_INTRI2C		0x02000000
#define SAA_VI_INTRVI		0x01000000
#define SAA_VI_IE		0x00020000
#define SAA_VI_OC		0x00010000
#define SAA_VI_ENDIAN		0x00008000

/* VI_I2C */

#define SAA_I2C_START		0x80000000
#define SAA_I2C_OP		0x40000000
#define SAA_I2C_ERROR		0x20000000
#define SAA_I2C_IE		0x10000000
#define SAA_I2C_OC		0x08000000
#define SAA_I2C_SEL		0x04000000
#define SAA_I2C_SCL		0x02000000
#define SAA_I2C_SDA		0x01000000
#define SAA_I2C_ADDRESS_MASK	0x001f0000
#define SAA_I2C_DATA_MASK	0x000000ff

typedef struct {
    int		x0, y0;		/* start point */
    int		xd, yd;		/* image dimension */
} SAA_DIMEN;

typedef struct {
    int		addr;		/* decoder register address */
    int		data;		/* decoder register value */
} SAA_REG;

/* Registers */

typedef struct {
#ifndef BT43
    volatile unsigned int vi_id;
#endif
    volatile unsigned int vi_csr;
    volatile unsigned int vi_i2c;
#ifndef BT43
    volatile unsigned int vi_ba;
#endif
} VI_REGs;

#if (OS_VERSION < 210) || ((OS_VERSION >= 210) && defined(KERNEL))

/* OC device descriptor */

typedef struct {
    ioDEV		devHdr;		/* OC device header */
    VI_REGs		*vi_regs;	/* controller registers */
    unsigned short	*vmem;		/* video RAM */
    int			ctrl_num;	/* controller number */
    int			mode;		/* TV type */
    sem_t		sem;		/* semaphore */
    int			x0, y0;		/* start point */
    int			xd, yd;		/* image dimension */
    int			created;	/* device exist */
} SAA_DEV;

#endif /* (OS_VERSION < 210) || ((OS_VERSION >= 210) && defined(KERNEL)) */

#define FIOSAASETMODE		5101
#define FIOSAASETDIMENSION	5102
#define FIOSAAREGWRITE		5103
#define FIOSAAREGREAD		5104

#define VI_PAL			0
#define VI_SECAM		1
#define VI_NTSC			2

#ifdef BT43
STATUS viInit(int);
#else
STATUS viInit();
#endif
void viIntr(int);

#define SAA_CTRL0	0
#define SAA_CTRL1	1
#define SAA_CTRL2	2
#define SAA_CTRL3	3
#define SAA_CTRL4	4
#define SAA_CTRL5	5
#define SAA_CTRL6	6
#define SAA_CTRL7	7
#define SAA_CTRL8	8
#define SAA_CTRL9	9
#define SAA_CTRL10	0xa
#define SAA_CTRL11	0xb
#define SAA_CTRL12	0xc
#define SAA_CTRL13	0xd
#define SAA_CTRL14	0xe
#define SAA_CTRL15	0xf
#define SAA_CTRL16	0x10
#define SAA_CTRL17	0x11
#define SAA_CTRL18	0x12
#define SAA_CTRL19	0x13
#define SAA_CTRL20	0x14
#define SAA_CTRL21	0x15
#define SAA_CTRL22	0x16
#define SAA_CTRL23	0x17
#define SAA_CTRL24	0x18
#define SAA_CTRL25	0x19
#define SAA_CTRL26	0x1a
#define SAA_CTRL27	0x1b
#define SAA_CTRL28	0x1c
#define SAA_CTRL29	0x1d
#define SAA_CTRL30	0x1e
#define SAA_CTRL31	0x1f

#endif /* __saa */
