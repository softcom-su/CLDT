/*
 * Copyright
 */


#ifndef _PRIVATE_IOS_H
#define _PRIVATE_IOS_H

#ifdef KERNEL

#include <private/si.h>
#include <private/objcore.h>
#include <private/aio.h>

struct file;
struct io_dev;
struct buf;

typedef struct io_drv {
    int		(*drv_open)     (struct file *file, const char *filename);
    int		(*drv_close)    (struct file *file,int fildes);
    int		(*drv_ioctl)    (struct file *, int cmd,int arg);
    int		(*drv_submit)   (struct file *,struct aiocb *cb);
    int		(*drv_cancel)   (struct file *,int fildes,struct aiocb *cb);
    int		(*drv_fsync)    (struct file *,int,struct aiocb *cb);
    int		(*drv_destroy)  (struct io_dev *);
    int		(*drv_strategy)  (struct io_dev *,struct buf *);
	int		vnode_type;
} ioDRV;


/*
 * Описатель устройства.
 * Устройство может быть символьным/блочным/сокет/пайп
 *
 */

typedef struct  io_dev {
	objHEADER		dev_objhdr;
	int				dev_flags;				    /* Тип устройства					*/
	char	 		*dev_name;				    /* Имя устройства например: "fd0"	*/
	char	 		dev_fullname[NAME_MAX+5];	/* Имя устройства например: "/dev/fd0"	*/
	ioDRV			*dev_drv;				    /* Указатель на структуру драйвера  */
	struct vnode    *dev_vp;
	struct specinfo dev_si;
} ioDEV;

#define iodev2rdev(d) ((d)->dev_si.si_rdev)


typedef struct file {
	objHEADER     m_objCore;
#define 	ioAIO_FDFLAGS_MASK	~O_NONBLOCK /* этот флаг не влияет на aio опрерации */
	int    		fd_flags;
	mode_t 		fd_mode;
	int    		fd_cnt;
	ioDEV		*fd_dev;
	off_t		fd_offset;
	void	*	fd_ext;
#define f_data fd_ext
	int			fd_type;
#define  DTYPE_FIFO			1
#define  DTYPE_VNODE		2
#define  DTYPE_SOCKET		3
#define  DTYPE_PIPE			4

}ioFILE;

struct io_fd {
#define		ifdFILE_OPEN		0x80000000
#define		ifdFILE_LOCK		0x40000000
#define		ifdFLAGS			ifdFILE_LOCK|ifdFILE_OPEN
	int    		ifd_flags;
	ioFILE		*ifd_file;
};

extern unsigned int		fdNumber; /*max число дескрипторов файлов*/

/* Для этих макросов 0-ok,1-error */

#define ioPRIO_CHECK(cb) (cb->aio_reqprio<0 || cb->aio_reqprio> AIO_PRIO_DELTA_MAX)

int		ioDevIns(ioDEV * dev,const char *devname,ioDRV *drvaddr,int blksize,dev_t rdev);
int		ioDevDelete(ioDEV *devp);
int		ioEnd( struct aiocb *aiocbp,int error,size_t count);
ioDEV 	*ioDevFind(const char *name);

int fdalloc(int want, int *result);
int falloc(struct file **resultfp,int *resultfd);

#define FGET_CHECKOPEN   0x01
#define FGET_SHOULDFREE  0x02
#define FGET_SHOULDOPEN  0x04
#define FGET_ADDREF		 0x10

int fget(int fd,struct file **resultfp,int shoudfree);
int __close(int fildes,int flags);

extern objCONTROL_ID      ioFileObjControlId;

#define fdptr(i) ( (i<3) ? (struct io_fd*)&threadExecuting->m_context.m_stdFD[i]:(struct io_fd*)&ioFdTable[i])



void ioFdFill(ioFILE *fpp,ioDEV *dev,int flags,mode_t mode,void *ext,int type);

extern ioDEV *ioConsole;
#define ioCB_TO_FCB(cb) (ioFdTable + (cb)->aio_fildes)

#endif /* KERNEL */
int		ioInit( int nFd );
void	ioCloseFiles();
void fileCloseAll();

#endif /* !_PRIVATE_IOS_H */
