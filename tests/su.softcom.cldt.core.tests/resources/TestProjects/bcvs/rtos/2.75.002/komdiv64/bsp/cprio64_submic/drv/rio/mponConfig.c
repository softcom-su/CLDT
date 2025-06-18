/* switch configuration descriptor */

typedef struct {
    unsigned char hop;
    unsigned char id;
    unsigned char checkPort;
    unsigned char* lut;
} RIO_SWITCH_CONFIG;

int rioForcedCfg = FALSE;                             // forced switch configuration flag

/* lookup tables */

static unsigned char tundra_p1[RIO_ID_MAX] = {  1,  1,  1,  1,  1,  1,  1,  1 };
static unsigned char tundra_p2[RIO_ID_MAX] = {  2,  2,  2,  2,  2,  2,  2,  2 };

/******************************************************************************/

// lut_tsin [id] == port of switch n where msgs to id must be sent 
static unsigned char lut_tsi3[RIO_ID_MAX] = { 2, 3, 0, 0, 0, 0, 0, -1 };
static unsigned char lut_tsi2[RIO_ID_MAX] = { 2, 2, 1, 1, 1, 0, 3, -1 };
static unsigned char lut_tsi1[RIO_ID_MAX] = { 3, 3, 0, 1, 2, 3, 3, -1 };

static RIO_SWITCH_CONFIG switches_p1_rpkb[] = {
    { 0, 0, 0, tundra_p2},	// configure onboard TSI
    { 1, 1, 2, lut_tsi3},	// configure external TSI
    { 2, 5, 0, lut_tsi2},
    { 3, 2, 1, lut_tsi1},
};
static RIO_SWITCH_CONFIG switches_p2_rpkb[] = {
    { 0, 1, 0, tundra_p2},	// configure onboard TSI
    { 1, 0, 2, lut_tsi3},	// configure external TSI
    { 2, 5, 0, lut_tsi2},
    { 3, 2, 1, lut_tsi1},
};
static RIO_SWITCH_CONFIG switches_p3_rpkb[] = {
    { 0, 2, 0, tundra_p2},	// configure onboard TSI
    { 1, 4, 2, lut_tsi1},	// configure external TSI
    { 2, 5, 3, lut_tsi2},
    { 3, 0, 2, lut_tsi3},
};
static RIO_SWITCH_CONFIG switches_p4_rpkb[] = {
    { 0, 3, 0, tundra_p2},	// configure onboard TSI
    { 1, 2, 2, lut_tsi1},	// configure external TSI
    { 2, 5, 3, lut_tsi2},
    { 3, 0, 2, lut_tsi3},
};
static RIO_SWITCH_CONFIG switches_p5_rpkb[] = {
    { 0, 4, 0, tundra_p2},	// configure onboard TSI
    { 1, 3, 2, lut_tsi1},	// configure external TSI
    { 2, 5, 3, lut_tsi2},
    { 3, 0, 2, lut_tsi3},
};
static RIO_SWITCH_CONFIG switches_p6_rpkb[] = {
    { 0, 5, 0, tundra_p2},	// configure onboard TSI
    { 1, 7, 2, lut_tsi2},	// configure external TSI
    { 2, 4, 1, lut_tsi1},
    { 2, 0, 2, lut_tsi3},
};
static RIO_SWITCH_CONFIG switches_p7_rpkb[] = {
    { 0, 6, 0, tundra_p2},	// configure onboard TSI
    { 1, 5, 2, lut_tsi2},	// configure external TSI
    { 2, 4, 1, lut_tsi1},
    { 2, 0, 2, lut_tsi3},
};

static RIO_SWITCH_CONFIG boards_p1_rpkb[] = {
    { 2, 1, 3, tundra_p2},
    { 4, 2, 0, tundra_p2},
    { 4, 3, 1, tundra_p2},
    { 4, 4, 2, tundra_p2},
    { 3, 5, 0, tundra_p2},
    { 3, 6, 3, tundra_p2},
};
static RIO_SWITCH_CONFIG boards_p2_rpkb[] = {
    { 2, 0, 2, tundra_p2},
    { 4, 2, 0, tundra_p2},
    { 4, 3, 1, tundra_p2},
    { 4, 4, 2, tundra_p2},
    { 3, 5, 0, tundra_p2},
    { 3, 6, 3, tundra_p2},
};
static RIO_SWITCH_CONFIG boards_p3_rpkb[] = {
    { 4, 0, 2, tundra_p2},
    { 4, 1, 3, tundra_p2},
    { 2, 3, 1, tundra_p2},
    { 2, 4, 2, tundra_p2},
    { 3, 5, 0, tundra_p2},
    { 3, 6, 3, tundra_p2},
};
static RIO_SWITCH_CONFIG boards_p4_rpkb[] = {
    { 4, 0, 2, tundra_p2},
    { 4, 1, 3, tundra_p2},
    { 2, 2, 0, tundra_p2},
    { 2, 4, 2, tundra_p2},
    { 3, 5, 0, tundra_p2},
    { 3, 6, 3, tundra_p2},
};
static RIO_SWITCH_CONFIG boards_p5_rpkb[] = {
    { 4, 0, 2, tundra_p2},
    { 4, 1, 3, tundra_p2},
    { 2, 2, 0, tundra_p2},
    { 2, 3, 1, tundra_p2},
    { 3, 5, 0, tundra_p2},
    { 3, 6, 3, tundra_p2},
};
static RIO_SWITCH_CONFIG boards_p6_rpkb[] = {
    { 3, 0, 2, tundra_p2},
    { 3, 1, 3, tundra_p2},
    { 3, 2, 0, tundra_p2},
    { 3, 3, 1, tundra_p2},
    { 3, 4, 2, tundra_p2},
    { 2, 6, 3, tundra_p2},
};
static RIO_SWITCH_CONFIG boards_p7_rpkb[] = {
    { 3, 0, 2, tundra_p2},
    { 3, 1, 3, tundra_p2},
    { 3, 2, 0, tundra_p2},
    { 3, 3, 1, tundra_p2},
    { 3, 4, 2, tundra_p2},
    { 2, 5, 0, tundra_p2},
};

#define	RIO_N_SWITCH_RPKB	sizeof(switches_p2_rpkb)/sizeof(switches_p2_rpkb[0])
#define	RIO_N_BOARD_RPKB	sizeof(boards_p2_rpkb)/sizeof(boards_p2_rpkb[0])

RIO_SWITCH_CONFIG* rioCrateConfigRpkb[RIO_ID_MAX*2] = {
    switches_p1_rpkb,boards_p1_rpkb,
    switches_p2_rpkb,boards_p2_rpkb,
    switches_p3_rpkb,boards_p3_rpkb,
    switches_p4_rpkb,boards_p4_rpkb,
    switches_p5_rpkb,boards_p5_rpkb,
    switches_p6_rpkb,boards_p6_rpkb,
    switches_p7_rpkb,boards_p7_rpkb,
};

static RIO_SWITCH_CONFIG switches_p1_rpkb_res[] = {
    { 0, 0, 0, tundra_p1},	// configure onboard TSI
    { 1, 1, 1, lut_tsi3},	// configure external TSI
    { 2, 5, 0, lut_tsi2},
    { 3, 2, 1, lut_tsi1},
};
static RIO_SWITCH_CONFIG switches_p2_rpkb_res[] = {
    { 0, 1, 0, tundra_p1},	// configure onboard TSI
    { 1, 0, 1, lut_tsi3},	// configure external TSI
    { 2, 5, 0, lut_tsi2},
    { 3, 2, 1, lut_tsi1},
};
static RIO_SWITCH_CONFIG switches_p3_rpkb_res[] = {
    { 0, 2, 0, tundra_p1},	// configure onboard TSI
    { 1, 4, 1, lut_tsi1},	// configure external TSI
    { 2, 5, 3, lut_tsi2},
    { 3, 0, 2, lut_tsi3},
};
static RIO_SWITCH_CONFIG switches_p4_rpkb_res[] = {
    { 0, 3, 0, tundra_p1},	// configure onboard TSI
    { 1, 2, 1, lut_tsi1},	// configure external TSI
    { 2, 5, 3, lut_tsi2},
    { 3, 0, 2, lut_tsi3},
};
static RIO_SWITCH_CONFIG switches_p5_rpkb_res[] = {
    { 0, 4, 0, tundra_p1},	// configure onboard TSI
    { 1, 3, 1, lut_tsi1},	// configure external TSI
    { 2, 5, 3, lut_tsi2},
    { 3, 0, 2, lut_tsi3},
};
static RIO_SWITCH_CONFIG switches_p6_rpkb_res[] = {
    { 0, 5, 0, tundra_p1},	// configure onboard TSI
    { 1, 7, 1, lut_tsi2},	// configure external TSI
    { 2, 4, 1, lut_tsi1},
    { 2, 0, 2, lut_tsi3},
};
static RIO_SWITCH_CONFIG switches_p7_rpkb_res[] = {
    { 0, 6, 0, tundra_p1},	// configure onboard TSI
    { 1, 5, 1, lut_tsi2},	// configure external TSI
    { 2, 4, 1, lut_tsi1},
    { 2, 0, 2, lut_tsi3},
};

static RIO_SWITCH_CONFIG boards_p1_rpkb_res[] = {
    { 2, 1, 3, tundra_p1},
    { 4, 2, 0, tundra_p1},
    { 4, 3, 1, tundra_p1},
    { 4, 4, 2, tundra_p1},
    { 3, 5, 0, tundra_p1},
    { 3, 6, 3, tundra_p1},
};
static RIO_SWITCH_CONFIG boards_p2_rpkb_res[] = {
    { 2, 0, 2, tundra_p1},
    { 4, 2, 0, tundra_p1},
    { 4, 3, 1, tundra_p1},
    { 4, 4, 2, tundra_p1},
    { 3, 5, 0, tundra_p1},
    { 3, 6, 3, tundra_p1},
};
static RIO_SWITCH_CONFIG boards_p3_rpkb_res[] = {
    { 4, 0, 2, tundra_p1},
    { 4, 1, 3, tundra_p1},
    { 2, 3, 1, tundra_p1},
    { 2, 4, 2, tundra_p1},
    { 3, 5, 0, tundra_p1},
    { 3, 6, 3, tundra_p1},
};
static RIO_SWITCH_CONFIG boards_p4_rpkb_res[] = {
    { 4, 0, 2, tundra_p1},
    { 4, 1, 3, tundra_p1},
    { 2, 2, 0, tundra_p1},
    { 2, 4, 2, tundra_p1},
    { 3, 5, 0, tundra_p1},
    { 3, 6, 3, tundra_p1},
};
static RIO_SWITCH_CONFIG boards_p5_rpkb_res[] = {
    { 4, 0, 2, tundra_p1},
    { 4, 1, 3, tundra_p1},
    { 2, 2, 0, tundra_p1},
    { 2, 3, 1, tundra_p1},
    { 3, 5, 0, tundra_p1},
    { 3, 6, 3, tundra_p1},
};
static RIO_SWITCH_CONFIG boards_p6_rpkb_res[] = {
    { 3, 0, 2, tundra_p1},
    { 3, 1, 3, tundra_p1},
    { 3, 2, 0, tundra_p1},
    { 3, 3, 1, tundra_p1},
    { 3, 4, 2, tundra_p1},
    { 2, 6, 3, tundra_p1},
};
static RIO_SWITCH_CONFIG boards_p7_rpkb_res[] = {
    { 3, 0, 2, tundra_p1},
    { 3, 1, 3, tundra_p1},
    { 3, 2, 0, tundra_p1},
    { 3, 3, 1, tundra_p1},
    { 3, 4, 2, tundra_p1},
    { 2, 5, 0, tundra_p1},
};

RIO_SWITCH_CONFIG* rioCrateConfigRpkbRes[RIO_ID_MAX*2] = {
    switches_p1_rpkb_res,boards_p1_rpkb_res,
    switches_p2_rpkb_res,boards_p2_rpkb_res,
    switches_p3_rpkb_res,boards_p3_rpkb_res,
    switches_p4_rpkb_res,boards_p4_rpkb_res,
    switches_p5_rpkb_res,boards_p5_rpkb_res,
    switches_p6_rpkb_res,boards_p6_rpkb_res,
    switches_p7_rpkb_res,boards_p7_rpkb_res,
};

/******************************************************************************/

/* NIISI D9 and D10 configuration */

static unsigned char lut_p9_d2[RIO_ID_MAX] =  { -1,  3, -1,  3,  3, -1,  3,  0 };
static unsigned char lut_p9_d1[RIO_ID_MAX] =  { -1,  0, -1,  0,  0, -1,  0,  2 };
static unsigned char lut_p10_d1[RIO_ID_MAX] = { -1,  2, -1,  2,  3, -1,  3,  1 };
static unsigned char lut_p10_d2[RIO_ID_MAX] = { -1,  0, -1,  1,  2, -1,  2,  3 };
static unsigned char lut_p10_d3[RIO_ID_MAX] = { -1,  0, -1,  0,  3, -1,  1,  2 };

/* NIISI D10 configuration */

static unsigned char lut_p10_d2x[RIO_ID_MAX] = { -1,   0, -1,   1,  2, -1,  2,  -1 };
static unsigned char lut_p10_d3x[RIO_ID_MAX] = { -1,   0, -1,   0,  3, -1,  1,  -1 };

/* NIISI D9 and D10 configuration version 2 */

static unsigned char lut_p9_d3_v2[RIO_ID_MAX] =  {  0,  2,  3,  2,  2, -1,  2,  0 };
static unsigned char lut_p9_d2_v2[RIO_ID_MAX] =  {  1,  3,  2,  3,  3, -1,  3,  0 };
static unsigned char lut_p9_d1_v2[RIO_ID_MAX] =  {  2,  0,  3,  0,  0, -1,  0,  2 };
static unsigned char lut_p10_d1_v2[RIO_ID_MAX] = {  1,  2,  1,  2,  3, -1,  3,  1 };
static unsigned char lut_p10_d2_v2[RIO_ID_MAX] = {  3,  0,  3,  1,  2, -1,  2,  3 };
static unsigned char lut_p10_d3_v2[RIO_ID_MAX] = {  2,  0,  2,  0,  3, -1,  1,  2 };

/******************************************************************************/

static RIO_SWITCH_CONFIG switches_p2_niisi2s[] = {
    { 0, 1, 0, tundra_p2},
    { 1, 3, 2, lut_p10_d2},
    { 2, 4, 2, lut_p10_d3},
    { 2, 7, 3, lut_p10_d1},
    { 3, 7, 1, lut_p9_d1},
    { 4, 7, 2, lut_p9_d2},
};

static RIO_SWITCH_CONFIG switches_p4_niisi2s[] = {
    { 0, 3, 0, tundra_p2},
    { 1, 1, 2, lut_p10_d2},
    { 2, 4, 2, lut_p10_d3},
    { 2, 7, 3, lut_p10_d1},
    { 3, 7, 1, lut_p9_d1},
    { 4, 7, 2, lut_p9_d2},
};

static RIO_SWITCH_CONFIG switches_p5_niisi2s[] = {
    { 0, 4, 0, tundra_p1},
    { 1, 6, 1, lut_p10_d3},
    { 2, 1, 0, lut_p10_d2},
    { 2, 7, 2, lut_p10_d1},
    { 3, 7, 1, lut_p9_d1},
    { 4, 7, 2, lut_p9_d2},
};

static RIO_SWITCH_CONFIG switches_p7_niisi2s[] = {
    { 0, 6, 0, tundra_p1},
    { 1, 4, 1, lut_p10_d3},
    { 2, 1, 0, lut_p10_d2},
    { 2, 7, 2, lut_p10_d1},
    { 3, 7, 1, lut_p9_d1},
    { 4, 7, 2, lut_p9_d2},
};

static RIO_SWITCH_CONFIG switches_p8_niisi2s[] = {
    { 0, 7, 0, tundra_p2},
    { 1, 1, 2, lut_p9_d2},
    { 2, 1, 3, lut_p9_d1},
    { 3, 1, 0, lut_p10_d1},
    { 4, 1, 2, lut_p10_d2},
    { 4, 4, 3, lut_p10_d3},
};

static RIO_SWITCH_CONFIG boards_p2_niisi2s[] = {
    { 2, 3, 1, tundra_p2},
    { 3, 4, 3, tundra_p1},
    { 3, 6, 1, tundra_p1},
    { 5, 7, 0, tundra_p2},
};

static RIO_SWITCH_CONFIG boards_p4_niisi2s[] = {
    { 2, 1, 0, tundra_p2},
    { 3, 4, 3, tundra_p1},
    { 3, 6, 1, tundra_p1},
    { 5, 7, 0, tundra_p2},
};

static RIO_SWITCH_CONFIG boards_p5_niisi2s[] = {
    { 2, 6, 1, tundra_p1},
    { 3, 1, 0, tundra_p2},
    { 3, 3, 1, tundra_p2},
    { 5, 7, 0, tundra_p2},
};

static RIO_SWITCH_CONFIG boards_p7_niisi2s[] = {
    { 2, 4, 3, tundra_p1},
    { 3, 1, 0, tundra_p2},
    { 3, 3, 1, tundra_p2},
    { 5, 7, 0, tundra_p2},
};

static RIO_SWITCH_CONFIG boards_p8_niisi2s[] = {
    { 5, 1, 0, tundra_p2},
    { 5, 3, 1, tundra_p2},
    { 5, 4, 3, tundra_p2},
    { 5, 6, 1, tundra_p1},
};

#define	RIO_N_SWITCH_NIISI2S	sizeof(switches_p2_niisi2s)/sizeof(switches_p2_niisi2s[0])
#define	RIO_N_BOARD_NIISI2S	sizeof(boards_p2_niisi2s)/sizeof(boards_p2_niisi2s[0])

RIO_SWITCH_CONFIG* rioCrateConfigNiisi2s[RIO_ID_MAX*2] = {
    NULL,NULL,
    switches_p2_niisi2s,boards_p2_niisi2s,
    NULL,NULL,
    switches_p4_niisi2s,boards_p4_niisi2s,
    switches_p5_niisi2s,boards_p5_niisi2s,
    NULL,NULL,
    switches_p7_niisi2s,boards_p7_niisi2s,
    switches_p8_niisi2s,boards_p8_niisi2s,
};

/******************************************************************************/

static RIO_SWITCH_CONFIG switches_p1_niisi2s_v2[] = {
    { 0, 0, 0, tundra_p1},
    { 1, 1, 1, lut_p9_d2_v2},
    { 2, 2, 2, lut_p9_d3_v2},
    { 2, 1, 2, lut_p9_d1_v2},
    { 3, 1, 0, lut_p10_d1_v2},
    { 4, 1, 2, lut_p10_d2_v2},
    { 4, 4, 3, lut_p10_d3_v2},
};

static RIO_SWITCH_CONFIG switches_p2_niisi2s_v2[] = {
    { 0, 1, 0, tundra_p2},
    { 1, 3, 2, lut_p10_d2_v2},
    { 2, 4, 2, lut_p10_d3_v2},
    { 2, 7, 3, lut_p10_d1_v2},
    { 3, 7, 1, lut_p9_d1_v2},
    { 4, 7, 2, lut_p9_d2_v2},
    { 4, 2, 3, lut_p9_d3_v2},
};

static RIO_SWITCH_CONFIG switches_p3_niisi2s_v2[] = {
    { 0, 2, 0, tundra_p1},
    { 1, 1, 1, lut_p9_d3_v2},
    { 2, 7, 0, lut_p9_d2_v2},
    { 2, 1, 2, lut_p9_d1_v2},
    { 3, 1, 0, lut_p10_d1_v2},
    { 4, 1, 2, lut_p10_d2_v2},
    { 4, 4, 3, lut_p10_d3_v2},
};

static RIO_SWITCH_CONFIG switches_p4_niisi2s_v2[] = {
    { 0, 3, 0, tundra_p2},
    { 1, 1, 2, lut_p10_d2_v2},
    { 2, 4, 2, lut_p10_d3_v2},
    { 2, 7, 3, lut_p10_d1_v2},
    { 3, 7, 1, lut_p9_d1_v2},
    { 4, 7, 2, lut_p9_d2_v2},
    { 4, 2, 3, lut_p9_d3_v2},
};

static RIO_SWITCH_CONFIG switches_p5_niisi2s_v2[] = {
    { 0, 4, 0, tundra_p1},
    { 1, 6, 1, lut_p10_d3_v2},
    { 2, 1, 0, lut_p10_d2_v2},
    { 2, 7, 2, lut_p10_d1_v2},
    { 3, 7, 1, lut_p9_d1_v2},
    { 4, 7, 2, lut_p9_d2_v2},
    { 4, 2, 3, lut_p9_d3_v2},
};

static RIO_SWITCH_CONFIG switches_p7_niisi2s_v2[] = {
    { 0, 6, 0, tundra_p1},
    { 1, 4, 1, lut_p10_d3_v2},
    { 2, 1, 0, lut_p10_d2_v2},
    { 2, 7, 2, lut_p10_d1_v2},
    { 3, 7, 1, lut_p9_d1_v2},
    { 4, 7, 2, lut_p9_d2_v2},
    { 4, 2, 3, lut_p9_d3_v2},
};

static RIO_SWITCH_CONFIG switches_p8_niisi2s_v2[] = {
    { 0, 7, 0, tundra_p2},
    { 1, 1, 2, lut_p9_d2_v2},
    { 2, 1, 3, lut_p9_d1_v2},
    { 2, 2, 2, lut_p9_d3_v2},
    { 3, 1, 0, lut_p10_d1_v2},
    { 4, 1, 2, lut_p10_d2_v2},
    { 4, 4, 3, lut_p10_d3_v2},
};

static RIO_SWITCH_CONFIG boards_p1_niisi2s_v2[] = {
    { 2, 7, 0, tundra_p2},
    { 3, 2, 3, tundra_p1},
    { 5, 1, 0, tundra_p2},
    { 5, 3, 1, tundra_p2},
    { 5, 4, 3, tundra_p1},
    { 5, 6, 1, tundra_p1},
};

static RIO_SWITCH_CONFIG boards_p2_niisi2s_v2[] = {
    { 2, 3, 1, tundra_p2},
    { 3, 4, 3, tundra_p1},
    { 3, 6, 1, tundra_p1},
    { 5, 7, 0, tundra_p2},
    { 5, 0, 1, tundra_p1},
    { 5, 2, 3, tundra_p1},
};

static RIO_SWITCH_CONFIG boards_p3_niisi2s_v2[] = {
    { 3, 0, 1, tundra_p1},
    { 3, 7, 0, tundra_p2},
    { 5, 1, 0, tundra_p2},
    { 5, 3, 1, tundra_p2},
    { 5, 4, 3, tundra_p1},
    { 5, 6, 1, tundra_p1},
};

static RIO_SWITCH_CONFIG boards_p4_niisi2s_v2[] = {
    { 2, 1, 0, tundra_p2},
    { 3, 4, 3, tundra_p1},
    { 3, 6, 1, tundra_p1},
    { 5, 7, 0, tundra_p2},
    { 5, 0, 1, tundra_p1},
    { 5, 2, 3, tundra_p1},
};

static RIO_SWITCH_CONFIG boards_p5_niisi2s_v2[] = {
    { 2, 6, 1, tundra_p1},
    { 3, 1, 0, tundra_p2},
    { 3, 3, 1, tundra_p2},
    { 5, 7, 0, tundra_p2},
    { 5, 0, 1, tundra_p1},
    { 5, 2, 3, tundra_p1},
};

static RIO_SWITCH_CONFIG boards_p7_niisi2s_v2[] = {
    { 2, 4, 3, tundra_p1},
    { 3, 1, 0, tundra_p2},
    { 3, 3, 1, tundra_p2},
    { 5, 7, 0, tundra_p2},
    { 5, 0, 1, tundra_p1},
    { 5, 2, 3, tundra_p1},
};

static RIO_SWITCH_CONFIG boards_p8_niisi2s_v2[] = {
    { 2, 0, 1, tundra_p1},
    { 3, 2, 3, tundra_p1},
    { 5, 1, 0, tundra_p2},
    { 5, 3, 1, tundra_p2},
    { 5, 4, 3, tundra_p1},
    { 5, 6, 1, tundra_p1},
};

#define	RIO_N_SWITCH_NIISI2S_V2	sizeof(switches_p2_niisi2s_v2)/sizeof(switches_p2_niisi2s_v2[0])
#define	RIO_N_BOARD_NIISI2S_V2	sizeof(boards_p2_niisi2s_v2)/sizeof(boards_p2_niisi2s_v2[0])

RIO_SWITCH_CONFIG* rioCrateConfigNiisi2sv2[RIO_ID_MAX*2] = {
    switches_p1_niisi2s_v2,boards_p1_niisi2s_v2,
    switches_p2_niisi2s_v2,boards_p2_niisi2s_v2,
    switches_p3_niisi2s_v2,boards_p3_niisi2s_v2,
    switches_p4_niisi2s_v2,boards_p4_niisi2s_v2,
    switches_p5_niisi2s_v2,boards_p5_niisi2s_v2,
    NULL,NULL,
    switches_p7_niisi2s_v2,boards_p7_niisi2s_v2,
    switches_p8_niisi2s_v2,boards_p8_niisi2s_v2,
};

/******************************************************************************/

static RIO_SWITCH_CONFIG switches_p2_niisi1s[] = {
    { 0, 1, 0, tundra_p2},
    { 1, 3, 2, lut_p10_d2x},
    { 2, 4, 2, lut_p10_d3x},
};

static RIO_SWITCH_CONFIG switches_p4_niisi1s[] = {
    { 0, 3, 0, tundra_p2},
    { 1, 1, 2, lut_p10_d2x},
    { 2, 4, 2, lut_p10_d3x},
};

static RIO_SWITCH_CONFIG switches_p5_niisi1s[] = {
    { 0, 4, 0, tundra_p1},
    { 1, 6, 1, lut_p10_d3x},
    { 2, 1, 0, lut_p10_d2x},
};

static RIO_SWITCH_CONFIG switches_p7_niisi1s[] = {
    { 0, 6, 0, tundra_p1},
    { 1, 4, 1, lut_p10_d3x},
    { 2, 1, 0, lut_p10_d2x},
};

static RIO_SWITCH_CONFIG boards_p2_niisi1s[] = {
    { 2, 3, 1, tundra_p2},
    { 3, 4, 3, tundra_p1},
    { 3, 6, 1, tundra_p1},
};

static RIO_SWITCH_CONFIG boards_p4_niisi1s[] = {
    { 2, 1, 0, tundra_p2},
    { 3, 4, 3, tundra_p1},
    { 3, 6, 1, tundra_p1},
};

static RIO_SWITCH_CONFIG boards_p5_niisi1s[] = {
    { 2, 6, 1, tundra_p1},
    { 3, 1, 0, tundra_p2},
    { 3, 3, 1, tundra_p2},
};

static RIO_SWITCH_CONFIG boards_p7_niisi1s[] = {
    { 2, 4, 3, tundra_p1},
    { 3, 1, 0, tundra_p2},
    { 3, 3, 1, tundra_p2},
};

#define	RIO_N_SWITCH_NIISI1S	3
#define	RIO_N_BOARD_NIISI1S	3

RIO_SWITCH_CONFIG* rioCrateConfigNiisi1s[RIO_ID_MAX*2] = {
    NULL,NULL,
    switches_p2_niisi1s,boards_p2_niisi1s,
    NULL,NULL,
    switches_p4_niisi1s,boards_p4_niisi1s,
    switches_p5_niisi1s,boards_p5_niisi1s,
    NULL,NULL,
    switches_p7_niisi1s,boards_p7_niisi1s,
    NULL,NULL,
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
	{
//	#warning test code
//	printf("hop=%d id=%d set discoverd bit OK\n",hop,cfg->id);
	    return 0;
	}

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

static int tsiConfig(RIO_SWITCH_CONFIG* cfg, int onboard)
{
    unsigned i, j, port, p, ix, id = boardProcNumGet() + 1, val, nPort;

    // errata: first cycle after reset must be write

    for(ix = 0; ix < 4; ix++)
    {
	rioMaintOut(cfg->hop,FAB_TEA_CTL0+ix*4,cfg->id,TUNDRA_FAB_TEA_CTL_VAL);
    }

    udelay(1000);

    if(rioMaintIn(cfg->hop,PORT_GEN_CTL,cfg->id,&val) == -1)
	return -1;

    if(!rioForcedCfg && (val & 0x20000000))	// check discovered bit
    {
//#warning test code
//    printf("hop=%d id=%d discovered!\n",cfg->hop,cfg->id);
	return 1;
    }

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
//	unsigned l = (val & 0xffff);
	for(i = 0; i < 1000; i++)
	{
	    if(rioMaintIn(cfg->hop,PORT_GEN_CTL,cfg->id,&val) == -1)
		return -1;

	    if(val & 0x20000000)
	    {
//#warning test code
		if(rioForcedCfg)
		    goto config;
//    printf("hop=%d id=%d discovered after lock=%04x !\n",cfg->hop,cfg->id,l);
		return 1;
	    }

	    udelay(10);
	}

	if(rioMaintIn(cfg->hop,CSR_HB_DEV_ID_LOCK,cfg->id,&id) == -1)
	    return -1;

	printf("%s: timeout hop=%d id=%d ctl=%08x id_lock=%x\n",__FUNCTION__,
	    cfg->hop,cfg->id,val,id);

	return -1;
    }
config:
    for(i = 0; i < RIO_ID_MAX; i += 8)
    {
	val = 0x88888888;

	for(j = 0; j < 8 && i + j < RIO_ID_MAX; j++)
	{
	    if(cfg->lut[i+j] == 0xff)
		continue;

	    if(onboard && i+j == cfg->id)
	    {
		p = 3;
	    }
	    else
	    {
		p = cfg->lut[i+j];
	    }

	    val = (val & ~(0xf << j*4)) | (p << j*4);
	}

	for(port = 0; port < 4; port++)
	{
	    if(rioMaintOut(cfg->hop,0x10a00+i/2+port*0x20000,cfg->id,val) == -1)
		return -1;
	}
    }

//#warning test code
//    printf("hop=%d id=%d route=%x\n",cfg->hop,cfg->id,val);

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

/* configure MPON board: tundra and endpoint */

static int boardConfig(RIO_SWITCH_CONFIG* cfg)
{
    int res;
    if((res = tsiConfig(cfg,TRUE)) != 0)
	return res;
    if(rioMaintOut(cfg->hop+1,CSR_BASE_DEV_ID,cfg->id,(cfg->id<<16)|cfg->id) == -1)
	return -1;
    if(configTag(cfg->hop+1,cfg) == -1)
	return -1;
    printf("%s: hop=%d id=%d OK\n",__FUNCTION__,cfg->hop,cfg->id);
    return 0;
}


/******************************************************************************/

/* standard switch configuration */

static int stdConfig(RIO_SWITCH_CONFIG* cfg)
{
    unsigned i, ix, id = boardProcNumGet() + 1, val, nPort;

#ifndef	VG18TEST
    if(rioMaintIn(cfg->hop,PORT_GEN_CTL,cfg->id,&val) == -1)
	return -1;

    if(!rioForcedCfg && (val & 0x20000000))	// check discovered bit
	return 1;
#endif

    if(rioMaintIn(cfg->hop,CSR_HB_DEV_ID_LOCK,cfg->id,&val) == -1)
	return -1;

    if((val & 0xffff) == 0xffff)
    {
	if(rioMaintOut(cfg->hop,CSR_HB_DEV_ID_LOCK,cfg->id,id) == -1)
	    return -1;

	if(rioMaintIn(cfg->hop,CSR_HB_DEV_ID_LOCK,cfg->id,&val) == -1)
	    return -1;

	if((val & 0xffff) == 0xffff)
	{
	    printf("%s: ID_LOCK second try!!!\n",__FUNCTION__);
	    if(rioMaintOut(cfg->hop,CSR_HB_DEV_ID_LOCK,cfg->id,id) == -1)
		return -1;

	    if(rioMaintIn(cfg->hop,CSR_HB_DEV_ID_LOCK,cfg->id,&val) == -1)
		return -1;
	}
    }

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

    for(i = 0; i < RIO_ID_MAX; i++)
    {
	if(rioMaintOut(cfg->hop,CSR_ROUTE_CFG_DESTID,cfg->id,i) == -1)
	    return -1;
	if(rioMaintOut(cfg->hop,CSR_ROUTE_CFG_PORT,cfg->id,cfg->lut[i]) == -1)
	    return -1;
    }

    if(rioMaintIn(cfg->hop,CAR_SWITCH_PI,cfg->id,&val) == -1)
	return -1;

    nPort = (val >> 8) & 0xff;

    for(ix = 0; ix < nPort; ix++)
    {
	if(rioMaintIn(cfg->hop,PORT_CTL+ix*0x20,cfg->id,&val) == -1)
	    return -1;
	if(rioMaintOut(cfg->hop,PORT_CTL+ix*0x20,cfg->id,(val & 1) ? 0x00600000 : 0x44000000) == -1)
	    return -1;
	if(rioMaintOut(cfg->hop,PORT_ERR_STAT0+ix*0x20,cfg->id,0xffffffff) == -1)
	    return -1;
    }

    if(rioMaintOut(cfg->hop,PKT_TTL,cfg->id,NIISI_PKT_TTL_VAL) == -1)
        return -1;

    return configTag(cfg->hop,cfg);
}

/******************************************************************************/

/* configure all switchs and boards */

int rioCfg(RIO_SWITCH_CONFIG* sw, int nSw, RIO_SWITCH_CONFIG* brd, int nBrd)
{
    unsigned ix; int res;

    if((res = checkPortOK(&sw[0])) < 0)
    {
	printf("%s: board port0 not ready\n",__FUNCTION__);
	return res;
    }

    if((res = tsiConfig(&sw[0],TRUE)) < 0)
	return res;

    for(ix = 1; ix < nSw; ix++)
    {
	if((res = checkPortOK(&sw[ix])) < 0)
	{
	    if(res == -2)
		printf("%s: hop=%d (switch=%d) id=%d port=%d not ready\n",__FUNCTION__,sw[ix].hop,ix, sw[ix].id,sw[ix].checkPort);
	    continue ;   /* ??? laz  return res; */
	}
	if(rioCrossType == RIO_CROSS_RPKB || rioCrossType == RIO_CROSS_RPKB_RESERVE || rioCrossType == RIO_CROSS_RPKB_FULL)
	{
	    if((res = tsiConfig(&sw[ix],FALSE)) < 0)
		return res;
	}
	else
	{
	    if((res = stdConfig(&sw[ix])) < 0)
		return res;
	}
	printf("%s: switch %d configured OK\n\r",__FUNCTION__,ix);
    }
    for(ix = 0; ix < nBrd; ix++)
    {
	/* check that this board attached */

	if((res = checkPortOK(&brd[ix])) < 0)
	{
	    if(res == -2)
		continue;
	    return res;
	}
	if((res = boardConfig(&brd[ix])) < 0)
	    return res;
    }
    rioRegs->ext.genCtl |= CT_LE_L(0x20000000);
    printf("%s: DONE\n",__FUNCTION__);
    return 0;
}
