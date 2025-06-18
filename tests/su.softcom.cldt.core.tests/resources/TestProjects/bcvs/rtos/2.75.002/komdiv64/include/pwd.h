#ifndef _PWD_H
#define _PWD_H

#include <sys/types.h>

struct passwd {
    char    *pw_name;       /* user name */
    char    *pw_passwd;     /* user password */
    uid_t   pw_uid;         /* user id */
    gid_t   pw_gid;         /* group id */
    char    *pw_gecos;      /* real name */
    char    *pw_dir;        /* home directory */
    char    *pw_shell;      /* shell program */
    };

uid_t getuid();
struct passwd *getpwnam(const char * name);
struct passwd *getpwuid(uid_t uid);
struct passwd *getpwnam_r(const char * name, \
	struct passwd *pwd, char *buf, int buf_size);
struct passwd *getpwuid_r(const char * name, \
	struct passwd *pwd, char *buf, int buf_size);

#endif
