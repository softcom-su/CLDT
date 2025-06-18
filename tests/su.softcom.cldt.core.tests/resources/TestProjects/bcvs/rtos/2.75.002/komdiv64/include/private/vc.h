#ifndef _PRIVATE_VC_H
#define _PRIVATE_VC_H
#ifdef KERNEL
#define NMAXCONS 10
/* Структура устройства */
struct  vc_data {
	struct vcs_data
	{
		termDEV vcs_td;
		struct rngbuf	*vcs_rb;
	} vc_dev[NMAXCONS];    				
	int		vc_current;
	int		vc_count;
	termDEV *vc_realdev;    				
} ;

#define VDTOTD(vd) &vd->vc_dev[vd->vc_current].vcs_td
void vc_setconsole(termDEV *rd,int n);
#endif /* KERNEL */
void vc_setconsolebyname(const char *rd,int n);

#endif /* !_PRIVATE_VC_H */
