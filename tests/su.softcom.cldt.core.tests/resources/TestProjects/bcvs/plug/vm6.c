
#if !defined(__has_feature) || !(__has_feature(address_sanitizer) || __has_feature(leak_sanitizer) || __has_feature(memory_sanitizer))
#ifndef KOMDIV
extern void free(void *memblock);
extern void *malloc(long long size);
extern void *calloc(long long number, long long size);
extern void *memcpy(void *dest, const void *src, long long n);
#endif

char *strcpy (char *dest, const char *src)
{
  return memcpy (dest, src, strlen (src) + 1);
}
#endif

#ifndef KOMDIV
extern void *memset(void *memptr, int val, long long num);
extern int printf(const char *format, ...) {}
extern int puts(const char *str);
extern double asin(double agl);
extern double atan(double agl);
extern double cos(double x);
extern float cosf(float x);
extern float sin(float x);
extern float sinf(float x);
extern double sqrt(double x);
extern float sqrtf(float x);
extern float tan(float x);
extern float atan2f(float y, float x);
extern double atan2(double y, double x);
extern double acos(double x);
#endif

double pow( double base, double exponent ){}

#ifndef _GCC_SIZE_T
#define _GCC_SIZE_T
#ifndef __SIZE_TYPE__
#define __SIZE_TYPE__ long unsigned int
#endif
typedef __SIZE_TYPE__ size_t;
#endif  /* _GCC_SIZE_T */


void sem_post(void *sem) {}
int socket(int domain, int type, int protocol) {}
long inet_addr(char *cp) {}
int sendto(int socket, char *buffer, int length, int flags,
		   struct sockaddr *address, int address_len) {}
int select(int *s, short int noreads, short int nowrites, short int noexcepts, long int timeout) {}
int errno(int err) {}
char *strerror(int errornum) {}
int bind(int bi) {}
void *inet_ntoa(void *in) {}
int pthread_mutex_lock(void *mutex) {
	return 0;
}
int recvfrom(int socket, char *buffer, int length, int flags, struct sockaddr *address, int *address_length) {}
int pthread_mutex_unlock(void *mutex) {
	return 0;
}
int shutdown(int s, int how) {}
int close(int s) {}
int pciConfigInLong(int buf, int sd, int adr, int canal) {}
int pciMemToLocal(int buf) {}
int mramWrite(int num, void *data, int muv, int symb) {}
int mramRead(int num, int muv, void *data, int symb) {}
int pscLiveInc(int num) {
	return 0;
}
int pscRead(int num) {

}
int rioInitAll(int num) {}
int mp_init(int errorInit, int errorSend, int *errorRecv, int *ok) {}
int mp_recv_create(void *conf, void *port) {}
int mp_ioctl(void *set, void *port, int ok) {}
int mp_recv_setbuf(void *port, int ok, int *buff) {}
int mp_send_create(void *conf, void *port) {}
int mp_send_setbuf(void *port, int ok, int *buff) {}
int pscWrite(int num, void *rio) {}
int mp_send_get(void *port, void *buf) {}
int mp_buf_getattr(void *recv, void *attr) {}
int mp_buf_getmem(void *bufRecv) {}
int mp_buf_putattr(void *send, void *attr) {}
int cacheDataFlush(int buff, int size) {}
int mp_send_put(void *port, void *buf) {}
int mp_recv_get(void *port, void *buf) {}
int cacheDataInvalidate(int buff, int size) {}
int mp_recv_put(void *port, void *buf) {}
int pscDesc(int num) {}
int boardAux2ClkConnect(int sec, int start) {}
int boardAux2ClkRateSet(int sec, int start) {}
int boardAux2ClkEnable(int sec, int start) {}
int boardAux2ClkDisable(int stop) {}
int sem_trywait(void *sem, int time) {}

float acosf(float x) {}
int netconfig(int num) {
	return 0;
}
int pscInit(int num) {
	return 0;
}
int exptstGetState(int num) {
	return 0;
}

int boardUserLatch(int num) {
	return 0;
}

int exptstGetResult(int num) {
	return 0;
}



int pthread_attr_setdetachstate(void *attr, void *create) {}
int pthread_attr_setschedparam(void *attr, void *param) {}
int pthread_create(void *id, void *attr, void *getData, void *num) {}
int sem_open(char strin, void *create, int symb, int num) {}
int pthread_attr_init_s(void *symb, long long param) {}
int pthread_mutex_init(void *mutex, void *param) {}
int tlmBufInit(int num) {
	return 0;
}
int boardAuxClkConnect(void *mutex, int param) {}
int boardAuxClkRateSet(void *tact) {}
int tlmSetCount(int param, long long CountPack) {
	return 0;
}
int manch_open(int device, int chanel, void *manch) {}
int manch_hint_attach(int id, int getData, int num) {}
int manch_write_buf(int id, int symb, int num, void *outData) {
	return 0; //Возможно тут останов нужен
}
int boardAuxClkRateGet(int num) {}
int boardAuxClkEnable(int num) {}
int boardAuxClkDisable(int num) {}
int sem_wait(int num) {}
int sem_close(int num) {}
extern size_t strlen (const char *str, size_t maxlen);
char * strncpy(char *s1, const char *s2, size_t n){
  size_t size = strlen (s2, n);
  if (size != n)
    memset (s1 + size, '\0', n - size);
  return memcpy (s1, s2, size);
}
extern void *osThreadMSPO(void);

extern 
int main()
{
	osThreadMSPO();
	return 0;
}
