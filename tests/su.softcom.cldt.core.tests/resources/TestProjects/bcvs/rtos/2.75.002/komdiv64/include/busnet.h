#ifndef _BUSNET_H
#define _BUSNET_H

/* BUSNET TYPE */
#define bntNONE  1 /* NONE */
#define bntAV    2 /* ANSI/VITA */
#define bntMP    3 /* Old share memory busnet */
#define BN_MAGIC "BusNet\0"
#define SIZE(stru,field) sizeof(((stru *)0)->field)
/*===============================================================================*/
/*  ND method field */

#define M_WSD8          4
#define M_WSD32         6 

#define DATA_PACKET     0
struct _BN_PKT
{
       unsigned destination;
       unsigned source;
       unsigned packet_type;
       unsigned packet_data_length;
       unsigned char     reserved1[16];
       unsigned char     reserved2[32];
       unsigned char     data[0];
};
struct _BN_HEADER
{
	unsigned char     magic_number[8];
	unsigned anchor_offset;
	unsigned anchor_space;
	unsigned anchor_method;
	unsigned protocol_revision;
	unsigned logical_address;
   unsigned char     region_valid_flag;
   unsigned char     reserved1[3];
   unsigned char     region_update_flag;
   unsigned char     reserved2[7];
   unsigned char     token_passing_enable_flag;
   unsigned char     reserved3[3];
	unsigned timeout_token;
   struct {
   unsigned    char     token_request_flag;
   unsigned    char     reserved[3];
   }        token_request_flag[31];
   unsigned char     reserved4[84];
   unsigned int      local_buffer_offset[32][2];  /*MY_ADDITION*/
};
/* protocol descriptor */
struct _BN_PD
{
   unsigned char     transmit_status;
   unsigned char     reserved1[3];
   unsigned char     receive_status;
   unsigned char     reserved2[3];
   unsigned packet_buffer_offset;
   unsigned packet_buffer_size;
   unsigned sequence_number;
   unsigned char     reserved3[12];
};
/* participant access descriptor */
struct _BN_PAD
{
   unsigned char     PAD_valid_flag;
   unsigned char     reserved1[3];
   unsigned region_start;
   unsigned region_end;
   unsigned region_space;
   unsigned region_method;
   unsigned timeout_value;
   unsigned char     reserved2[4];
};
/* notification descriptor */
struct _BN_ND
{
   unsigned char     ND_valid_flag;
   unsigned char     reserved1[3];
   unsigned method;
   unsigned value1;
   unsigned value2;
   unsigned value3;
   unsigned value4;
   unsigned char     reserved2[4];
};
/* participant descriptor  */
struct _BN_PaticipantD
{
   struct _BN_PD  pdd[2];
   struct _BN_PAD pad;
   struct _BN_ND  nd;
};
struct _BN_SMR
{
     struct _BN_HEADER        header;
     struct _BN_PaticipantD   pd[32];
};
/*==================================================================================*/
/* Function descriptions*/
int bn_send_interrupt(int pt);
unsigned bn_get_buffer();
/*==================================================================================*/
/* Configuration parameters */

struct _BN_CONFIG
{
    unsigned bn_master;         /* Является ли данный модуль мастером */
    unsigned bn_logical_addr;
    unsigned bn_p_nd_method;
    unsigned bn_p_nd_value1;
    unsigned bn_p_nd_value2;
    unsigned bn_p_nd_value3;
    unsigned bn_p_nd_value4;
/*====================================================================================*/
    unsigned bn_p_offset_start;
    unsigned bn_p_offset_end;
    unsigned bn_p_method;
    unsigned bn_p_space;
/*====================================================================================*/
    unsigned bn_master_offset_start;
    unsigned bn_master_offset_end;
    unsigned bn_master_space;
    unsigned bn_master_method;
/*===================================================================================*/
    unsigned bn_packet_size;
    unsigned bn_p_probe_startdelay; /* microseconds */
    unsigned bn_p_probe_mininterval;
    unsigned bn_p_probe_maxcount;
    unsigned bn_p_probe_backoffcount;
    unsigned bn_p_probe_maxinterval;
    unsigned bn_vme_offset_start;
    int        bn_nocopy;  
};
extern struct _BN_CONFIG bn_config;
int bn_init(struct _BN_CONFIG *config);
/*=================================================================================*/

struct mbuf;
int bn_send_pkt(int pr,int packet_type,struct mbuf *m0);
int bn_send_broadcast(int packet_type,struct mbuf *m0);
struct mbuf *bn_recv_pkt(int *packet_type);
void  bn_interrupt_connect(void (h)(int),int p);
#endif
