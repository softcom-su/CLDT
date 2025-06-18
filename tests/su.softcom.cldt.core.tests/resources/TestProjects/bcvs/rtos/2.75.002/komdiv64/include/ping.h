#ifndef _PING_H
#define _PING_H
int ping(const char *host,
		  int count,
		  int period,
		  int timeout,
		  int length,
		  int flag);
#define PING_QUIET 1

#endif /* _PING_H */
