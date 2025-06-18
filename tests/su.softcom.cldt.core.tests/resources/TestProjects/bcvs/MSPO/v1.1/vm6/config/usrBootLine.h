//-------------------------------------------------------------------------------------	
// 		usrBootLine.c
//-------------------------------------------------------------------------------------	
#ifndef		_USR_BOOT_LINE_H
#define		_USR_BOOT_LINE_H
//-------------------------------------------------------------------------------------	

#define BOOT_DEV_LEN		30	/* max chars in device name */
#define BOOT_HOST_LEN		20	/* max chars in host name */
#define BOOT_ADDR_LEN		30	/* max chars in net addr */
#define BOOT_FILE_LEN		80	/* max chars in file name */
#define BOOT_USR_LEN		20	/* max chars in user name */
#define BOOT_PASSWORD_LEN	20	/* max chars in password */

#define BOOT_OTHER_LEN		180	/* max chars in "other" field */

//-------------------------------------------------------------------------------------	

typedef struct 				/* BOOT_PARAMS */
   {
    char bootDev [BOOT_DEV_LEN];	/* boot device code */
    char hostName [BOOT_HOST_LEN];	/* name of host */
    char targetName [BOOT_HOST_LEN];	/* name of target */
    char ead [BOOT_ADDR_LEN];		/* ethernet internet addr */
    char bad [BOOT_ADDR_LEN];		/* backplane internet addr */
    char had [BOOT_ADDR_LEN];		/* host internet addr */
    char gad [BOOT_ADDR_LEN];		/* gateway internet addr */
    char bootFile [BOOT_FILE_LEN];	/* name of boot file */
    char startupScript [BOOT_OTHER_LEN];/* name of startup script file */
    char usr [BOOT_USR_LEN];		/* user name */
    char passwd [BOOT_PASSWORD_LEN];	/* password */
    char other [BOOT_OTHER_LEN];	/* available for applications */
    int	 flags;
//------------------------
    char ead_mask [BOOT_ADDR_LEN];
    char bad_mask [BOOT_ADDR_LEN];
    char had_mask [BOOT_ADDR_LEN];
    char gad_mask [BOOT_ADDR_LEN];
//------------------------    
    char *netDev;
    int net_arg0;
    int net_arg1;
    int net_arg2; 
//------------------------    
    char *dev;
    int arg0;
    int	arg1;
    int arg2;
//------------------------        
    int smDev;
//------------------------        
    int boot_arg0;
    int	boot_arg1;
    int boot_arg2;    
//------------------------        
    int other_arg0;
    int	other_arg1;
    int other_arg2;
//------------------------    
    char nfsMount[BOOT_FILE_LEN];
    int	uid;
    int	gid;    
//------------------------    
   }boot_params;
   
//-------------------------------------------------------------------------------------    
extern int bootStringToStruct(char * ,boot_params *);
extern int bootNetmaskExtract(char *,int *);
extern int bootParamsCopy(boot_params *);
//-------------------------------------------------------------------------------------    
#endif			// _USR_BOOT_LINE_H   
//------------------------------------------------------------------------------------- 
//------------------------------------------------------------------------------------- 

