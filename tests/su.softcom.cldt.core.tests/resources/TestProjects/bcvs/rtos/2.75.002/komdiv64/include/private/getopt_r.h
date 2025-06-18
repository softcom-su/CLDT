#ifndef _PRIVATE_GETOPT_R
#define _PRIVATE_GETOPT_R
struct getopt_t 
{
    int	opterr,	/* if error message should be printed */
	optind,		/* index into parent argv vector */
	optopt,			/* character checked for validity */
	optreset;		/* reset getopt */
    char	*optarg;		/* argument associated with option */
    char *place;
};
#define	GETOPT_R_INITILIZER {1,1,0,0,0,""}
int getopt_r(int nargc,char * const *nargv,const char *ostr,struct getopt_t *gt);
#endif
