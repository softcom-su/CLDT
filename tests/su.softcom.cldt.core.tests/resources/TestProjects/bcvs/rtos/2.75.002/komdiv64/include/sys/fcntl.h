/*-
 * Copyright (c) 1983, 1990, 1993
 *	The Regents of the University of California.  All rights reserved.
 * (c) UNIX System Laboratories, Inc.
 * All or some portions of this file are derived from material licensed
 * to the University of California by American Telephone and Telegraph
 * Co. or Unix System Laboratories, Inc. and are reproduced herein with
 * the permission of UNIX System Laboratories, Inc.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 * 1. Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 * 2. Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in the
 *    documentation and/or other materials provided with the distribution.
 * 3. All advertising materials mentioning features or use of this software
 *    must display the following acknowledgement:
 *	This product includes software developed by the University of
 *	California, Berkeley and its contributors.
 * 4. Neither the name of the University nor the names of its contributors
 *    may be used to endorse or promote products derived from this software
 *    without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE REGENTS AND CONTRIBUTORS ``AS IS'' AND
 * ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
 * ARE DISCLAIMED.  IN NO EVENT SHALL THE REGENTS OR CONTRIBUTORS BE LIABLE
 * FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
 * DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS
 * OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION)
 * HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT
 * LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY
 * OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF
 * SUCH DAMAGE.
 *
 *	@(#)fcntl.h	8.3 (Berkeley) 1/21/94
 * $FreeBSD: src/sys/sys/fcntl.h,v 1.7.2.1 1999/08/29 16:32:21 peter Exp $
 */

#ifndef _SYS_FCNTL_H_
#define	_SYS_FCNTL_H_

/*
 * This file includes the definitions for open and fcntl
 * described by POSIX for <fcntl.h>; it also includes
 * related kernel definitions.
 */

#include <sys/types.h>


#define O_RDONLY	0x00000000	/*	открыть файл только для чтения; */
#define O_WRONLY	0x00000001	/*	открыть файл только для записи; */
#define O_RDWR		0x00000002	/*	открыть файл для чтения и записи. 
								При открытии именованного программного 
								канала (файла типа FIFO) 
								результат не определен.*/
 
/* Следующие флаги могут присутствовать в значении oflag в любой комбинации.*/

#define O_NOCTTY	0x00000040	/*	не объявлять терминал управляющим 
								(используется при открытии терминала); */

#define O_NONBLOCK	0x00000080	/*	не блокировать поток управления при 
								выполнении функции open() (см. ниже); */

 
#define O_CREAT		0x00000100	/*	создание файла; */
#define O_TRUNC		0x00000200	/*	если файл существует, является регулярным файлом и 
								если он успешно открыт в режиме O_RDWR или O_WRONLY, 
								он будет усечен до нулевой длины. */
#define O_EXCL		0x00000400	/*	если установлены флаги O_EXCL и O_CREAT, 
								функция завершится с ошибкой, если файл существует; */
#define O_APPEND	0x00000800	/*	добавление записей в конец файла; */

								
#define O_DSYNC		0x00001000	/*	операции записи, использующие полученный в результате 
								открытия дескриптор файла, должны обеспечивать целостность 
								данных по завершению операции; */
 
 
#define  O_RSYNC	0x00002000	/*	операции чтения, использующие полученный в результате 
								открытия дескриптор файла, должны обеспечивать целостность 
								данных (если также установлен флаг O_DSYNC) 
								или файла (если также установлен флаг O_SYNC) 
								по завершению операции; */
 
#define  O_SYNC		0x00004000	/*	операции записи, использующие полученный в результате 
								открытия дескриптор файла, должны обеспечивать целостность 
								файла по завершению операции; */

#define  O_MP_OBJ	0x00008000	/*	создание объекта в общей памяти; */
#define  O_REFSTOP 0x00010000  /* не проваливаться по символьной ссылке */ 

#define	O_ACCMODE	(O_RDONLY|O_WRONLY|O_RDWR)

/*
 * Kernel encoding of open mode; separate read and write bits that are
 * independently testable: 1 greater than the above.
 *
 * XXX
 * FREAD and FWRITE are excluded from the #ifdef KERNEL so that TIOCFLUSH,
 * which was documented to use FREAD/FWRITE, continues to work.
 */
#ifndef _POSIX_SOURCE
#define	FREAD		0x0001
#define	FWRITE		0x0002
#endif
#ifndef _POSIX_SOURCE
#define	O_SHLOCK	0x00020000		/* open with shared file lock */
#define	O_EXLOCK	0x00040000		/* open with exclusive file lock */
#define	O_ASYNC		0x00080000		/* signal pgrp when data ready */
#define	O_FSYNC		O_SYNC		/* synchronous writes */
#define	O_NOFOLLOW	O_REFSTOP
#endif
#ifdef KERNEL
#define	FMARK		0x00100000		/* mark during gc() */
#define	FDEFER		0x00200000		/* defer for next gc pass */
#define	FHASLOCK	0x00400000		/* descriptor holds advisory lock */
#endif

#ifdef KERNEL
/* convert from open() flags to/from fflags; convert O_RD/WR to FREAD/FWRITE */
#define	FFLAGS(oflags)	((oflags) + 1)
#define	OFLAGS(fflags)	((fflags) - 1)

/* bits to save after open */
#define	FMASK		(FREAD|FWRITE|FAPPEND|FASYNC|FFSYNC|FNONBLOCK)
/* bits settable by fcntl(F_SETFL, ...) */
#define	FCNTLFLAGS	(FAPPEND|FASYNC|FFSYNC|FNONBLOCK)
#endif

/*
 * The O_* flags used to have only F* names, which were used in the kernel
 * and by fcntl.  We retain the F* names for the kernel f_flags field
 * and for backward compatibility for fcntl.
 */
#ifndef _POSIX_SOURCE
#define	FAPPEND		O_APPEND	/* kernel/compat */
#define	FASYNC		O_ASYNC		/* kernel/compat */
#define	FFSYNC		O_FSYNC		/* kernel */
#define	FNONBLOCK	O_NONBLOCK	/* kernel */
#define	FNDELAY		O_NONBLOCK	/* compat */
#define	O_NDELAY	O_NONBLOCK	/* compat */
#endif

/*
 * Constants used for fcntl(2)
 */

/* command values */
#define	F_DUPFD		0		/* duplicate file descriptor */
#define	F_GETFD		1		/* get file descriptor flags */
#define	F_SETFD		2		/* set file descriptor flags */
#define	F_GETFL		3		/* get file status flags */
#define	F_SETFL		4		/* set file status flags */
#ifndef _POSIX_SOURCE
#define	F_GETOWN	5		/* get SIGIO/SIGURG proc/pgrp */
#define F_SETOWN	6		/* set SIGIO/SIGURG proc/pgrp */
#endif
#define	F_GETLK		7		/* get record locking information */
#define	F_SETLK		8		/* set record locking information */
#define	F_SETLKW	9		/* F_SETLK; wait if blocked */

/* file descriptor flags (F_GETFD, F_SETFD) */
#define	FD_CLOEXEC	1		/* close-on-exec flag */

/* record locking flags (F_GETLK, F_SETLK, F_SETLKW) */
#define	F_RDLCK		1		/* shared or read lock */
#define	F_UNLCK		2		/* unlock */
#define	F_WRLCK		3		/* exclusive or write lock */
#ifdef KERNEL
#define	F_WAIT		0x010		/* Wait until lock is granted */
#define	F_FLOCK		0x020	 	/* Use flock(2) semantics for lock */
#define	F_POSIX		0x040	 	/* Use POSIX semantics for lock */
#endif

/*
 * Advisory file segment locking data type -
 * information passed to system by user
 */
struct flock {
	off_t	l_start;	/* starting offset */
	off_t	l_len;		/* len = 0 means until end of file */
	pid_t	l_pid;		/* lock owner */
	short	l_type;		/* lock type: read/write, etc. */
	short	l_whence;	/* type of l_start */
};


#ifndef _POSIX_SOURCE
/* lock operations for flock(2) */
#define	LOCK_SH		0x01		/* shared file lock */
#define	LOCK_EX		0x02		/* exclusive file lock */
#define	LOCK_NB		0x04		/* don't block when locking */
#define	LOCK_UN		0x08		/* unlock file */
#endif


#ifndef KERNEL
#include <sys/cdefs.h>

__BEGIN_DECLS
int	open __P((const char *, int, ...));
int	creat __P((const char *, mode_t));
int	fcntl __P((int, int, ...));
#ifndef _POSIX_SOURCE
/* int	flock __P((int, int)); */
#endif /* !_POSIX_SOURCE */
__END_DECLS
#endif

#endif /* !_SYS_FCNTL_H_ */
