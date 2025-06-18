#ifndef _PRIVATE_STDDRV_H
#define _PRIVATE_STDDRV_H


#define drvGetFdFlags(cb) (cb)->aiosys.sy_flags

/*======================================================================*/
struct file;
struct aiocb;
struct que_head;

int drvOpen(struct file *file, const char *filename);
void drvQueFlush(struct que_head *que,int code);
int drvCancel(struct aiocb *cb);
int drvQueFlushFd(struct que_head *que,int fildes);
struct aiocb *drvGetFirst(struct que_head *qh);
void drvInsert(struct que_head *qh,struct aiocb *cb);

#endif /* !_PRIVATE_STDDRV_H */
