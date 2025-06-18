/*
 * dirent.h
 *
 *  Created on: 09.12.2008
 *      Author: nkigor
 */

#ifndef PRIVATE_DIRENT_H_
#define PRIVATE_DIRENT_H_
/* definitions for library routines operating on directories. */
#define    DIRBLKSIZ    1024

/* structure describing an open directory. */
struct _dirdesc {
    int        dd_fd;        /* file descriptor associated with directory */
    int        dd_loc;       /* offset in current buffer */
    int        dd_size;      /* amount of data returned by getdirentries */
    char       dd_buf[DIRBLKSIZ];    /* data buffer */
    int        dd_seek;     /* magic cookie returned by getdirentries */
    void      *dd_ddloc;    /* Linked list of ddloc structs for telldir/seekdir */
    pthread_mutex_t dd_mtx;
};
struct dirpos {
   void       *dp_next;
   int         dp_loc;       /* offset in current buffer */
   int         dp_seek;     /* magic cookie returned by getdirentries */
};
struct dirent;
int _readdir_locked(struct _dirdesc *pdir,struct dirent **result);

#endif /* PRIVATE_DIRENT_H_ */
