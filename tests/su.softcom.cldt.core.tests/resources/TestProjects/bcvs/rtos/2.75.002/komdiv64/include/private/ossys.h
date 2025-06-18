void osBoardInit(void);
void  osKernelInit(void);
void *osRootThread(void *arg);
void  osSysClockTick (int arg);
void *osDemoThread (void *arg);
void *osUserThread (void *arg);
void *osShell(void *arg);

char *boardMemTop(void);
char *boardModel(void);
void  boardLogo(void);
int   mqShowInit( void );
int   sigShowInit( void );
int   mqpPxInit( int num, int que32 );
void  pthreadSetDefaults(pthread_attr_t *attr);

