#ifndef _RIOREGS_H_
#define _RIOREGS_H_

#define CAR_DEV_ID            0x0000
#define CAR_DEV_INFO          0x0004
#define CAR_ASBLY_ID          0x0008

/* Assembly Information CAR
 * 0-15  - assembly revision level
 * 16-31 - extended features ptr
 */
#define CAR_ASBLY_INFO        0x000C

/* Processing Element Features CAR:
 * 0  - device can bridge to another interface
 * 1  - device has local physically addressable memory
 * 2  - device has a local processor
 * 3  - device is a switch
 */
#define CAR_PE_FEAT           0x0010
enum CAR_PE_FEAT_BITS {
    PEF_BRIDGE    = 1 << 31,
    PEF_MEMORY    = 1 << 30,
    PEF_PROCESSOR = 1 << 29,
    PEF_SWITCH    = 1 << 28
};

/* Switch Port Information CAR:
 * 16-23 - number of ports
 * 24-31 - port number through witch the device is being accessed
 */
#define CAR_SWITCH_PI         0x0014
enum CAR_SWITCH_PI_BITS {
    SPI_PORT_TOTAL_OFFSET = 8,
    SPI_PORT_TOTAL_MASK   = 0xff,
    SPI_PORT_NUM_MASK     = 0xff
};

#define CAR_SOURCE_OP         0x0018
#define CAR_DEST_OP           0x001C
#define CAR_RTBL_DSTID_LIM    0x0034
#define CSR_BASE_DEV_ID       0x0060
#define CSR_HB_DEV_ID_LOCK    0x0068
#define CSR_COMP_TAG          0x006C
#define CSR_ROUTE_CFG_DESTID  0x0070
#define CSR_ROUTE_CFG_PORT    0x0074
#define CSR_ROUTE_DEF_PORT    0x0078

/* Port General Control CSR:
 * 0 - host
 * 1 - master enable
 * 2 - discovered
 */
#define CSR_PORT_GEN_CTL_OFFS    0x003C
enum CSR_PORT_GEN_CTL_BITS {
    PGC_HOST          = 1 << 31,
    PGC_MASTER_ENABLE = 1 << 30,
    PGC_DISCOVERED    = 1 << 29
};

/* Port n Error and Status CSR:
 * 30 - port OK
 */
#define CSR_PORTN_ERR_STAT_OFFS  0x0058
enum CSR_PORTN_ERR_STAT_BITS {
    PNES_PORT_OK = 1 << 1
};

/* Port n Control CSR:
 * 31 - port type (0 - parallel, 1 - serial)
 * Parallel-specific:
 * 1 - output port enable (allow packets other than MAINENANCE to come out)
 * 5 - input port enable (allow packets other than MAINENANCE to come in)
 * Serial-specific:
 * 9 - output port enable
 * 10 - input port enable
 */
#define CSR_PORTN_CTRL_OFFS      0x005C
enum CSR_PORTN_CTRL_BITS {
    PNC_PARALLEL_OUTPORT_ENABLE = 1 << 30,
    PNC_PARALLEL_INPORT_ENABLE  = 1 << 26,
    PNC_SERIAL_OUTPORT_ENABLE   = 1 << 22,
    PNC_SERIAL_INPORT_ENABLE    = 1 << 21,
    PNC_SERIAL_PORT             = 1
};

#endif /* _RIOREGS_H_ */
