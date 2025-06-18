#ifndef RBDEV_H_
#define RBDEV_H_
#define ringFlush2(q1,q2) { int lock = intLock(); ringFlush(q1); ringFlush(q2); intUnlock(lock);}
#include <public/difference.h>
#include <assert.h>
#include <private/ring.h>
#include <private/mutex.h>
#include <sys/select.h>
typedef struct rb_device {
    ioDEV           rb_dev;            /* заголовок описателя устройства               */
    struct rngbuf  *rb_rrb;            /* кольцевой буфер чтения                       */
    struct rngbuf  *rb_wrb;            /* кольцевой буфер записи                       */
    queHEAD         rb_rque,           /* очередь потоков на чтение                    */
                    rb_wque;           /* очередь потоков на запись                    */
    struct selinfo  rb_siread,         /* очередь select-чтение                        */
                    rb_siwrite,        /* очередь select-запись                        */
                    rb_siex;           /* очередь select-исключительная ситуация       */
    pthread_mutex_t rb_mutex;          /* мьютекс доступа к буферу чтения              */
    int             rb_nextbytes;      /* число байт в первой ждущей заявке            */
    int             rb_prio; /* приоритет первого потока в очереди на запись или -1    */
    void           (*rb_onPutRBuf) (void *rb);
    void           (*rb_onPutWBuf) (void *rb);
    void           (*rb_onGetRBuf) (void *rb);
    void           (*rb_onGetWBuf) (void *rb);
} rbDEV;
/* Описатель файла FIFO  */
struct fifo_node {
    rbDEV    fn_rbdev;
    int      fn_in,                    /* число открытий на чтение                     */
             fn_out;                   /* число открытий на запись                     */
};
int  fifoClose(ioFILE *file, int fildes);
int  fifoSubmit(ioFILE *file, struct aiocb *cb);

int  rbInit   ( rbDEV *rb, void (*startio) (void *rb), int rbs, int wbs, queCONTROL_ID  que_type );
int  rbIoctl  ( ioFILE *file, int cmd, int arg );
void rbRThreadStart(rbDEV *rb);
void rbRThreadStartDE(void *rb);
void rbWThreadStart(rbDEV *rb);
void rbWThreadStartDE(void *rb);
int  rbSubmit ( ioFILE *file, struct aiocb *cb );
int  rbDestroy( rbDEV *rb );
void rbFlushr ( rbDEV *rb, int cause );
void rbFlushw ( rbDEV *rb, int cause );
int  rbRead   ( rbDEV *rb, struct aiocb *cb );
int  rbWrite  ( rbDEV *rb, struct aiocb *cb );
int  rbOpen   ( struct file *file, const char *filename );
int  rbClose(struct file *file,int fildes);
int  rbCancel( struct file *file, int fildes, struct aiocb *cb );

/*----------- rbLOCK -------------
 * Функция rbLOCK - захват устройства rbDEV
 * Аргумент:
 *   rb - указатель на структуру rbDEV;
 */
static __inline__ void rbLOCK( rbDEV *rb )
{
    kernDispatchDisable();             /* запрещение планирования                      */
    mtxLockInternal(rb->rb_mutex);
    assert(threadDispatchLevel == 0);  /* ошибка, если планирование запрещено          */
}
static __inline__ void rbLOCKDD( rbDEV *rb )
{
    mtxLockInternal(rb->rb_mutex);
    assert(threadDispatchLevel == 0);  /* ошибка, если планирование запрещено          */
}
/*----------- rbUNLOCK -------------
 * Функция rbUNLOCK - освобождение устройства rbDEV
 * Аргумент:
 *   rb - указатель на структуру rbDEV;
 */
static __inline__ void rbUNLOCK(rbDEV *rb)
{
    kernDispatchDisable();             /* запрещение планирования                      */
    mtxUnlockInternal(rb->rb_mutex);
    kernDispatchEnable();
}
static __inline__ void rbUNLOCKDD(rbDEV *rb)
{
    mtxUnlockInternal(rb->rb_mutex);
}
/*----------- rbStartThread -------------
 * Функция rbStartThread - активация (первого) потока, стоящего в указанной очереди
 * Аргументы:
 *   que   - очередь потоков управления;
 *   cause   - причина выхода из состояния ожидания, может принимать следующие значения:
 *     PTHREAD_WAIT_OK      - нормальное заверщение ожидания
 *                            (получен ожидаемый ресурс или произошло ожидаемое событие);
 *     PTHREAD_WAIT_DESTROY - объект ожидания был разрушен, ожидание прервано.
 * При вызове rbStartThread не используются:
 *     PTHREAD_WAIT_TIMEOUT - истекло время ожидания;
 *     PTHREAD_WAIT_SIGNAL  - ожидание было прервано сигналом;
 *     PTHREAD_WAIT_CANCEL  - получен запрос на удаление;
 * Возвращаемые значения:
 *   0 - поток запущен;
 *   1 - очередь пуста.
 *
 * Перед вызовом функции планирование должно быть запрещено.
 */
static __inline__ int rbStartThread( queHEAD *que, int cause )
{
    threadTCB *pThread;                /* указатель на описатель потока управления     */
    queNODE   *node;
    assert( threadDispatchLevel == 1 );/* ошибка, если планирование не запрещено       */

    if( (node=queExtract( que , NULL )) != NULL )
    {
        /* Очередь не пуста */
        pThread = (threadTCB*)queGetNodeOwner( node );/* указатель на описатель потока */
        pThread->m_status &= ~PTHREAD_PENDED;
        kernActivateThread( pThread, cause );    /* объявление потока работоспособным  */
        return 0;
    }
    return 1;
}
/*----------- rbEnd -------------------
 * Функция rbEnd - завершение обмена
 * Аргументы:
 *   cb    - заявка на обмен (указатель на структуру aiocb);
 *   error - код ошибки (error status);
 *   einv_error - замена для EINVAL.
 * Функция производит трансляцию кода возврата kernBlockThread в errno.

    [ECANCELED] - получен запрос на удаление -- не получаем никогда (сразу выходим)
    [EINVAL]    - объект ожидания был разрушен -> (EPIPE или 0);
    [ETIMEDOUT] - истекло время ожидания -> 0;
    [EINTR]     - ожидание было прервано сигналом -> EINTR.

 */
static __inline__ void rbEnd(struct aiocb *cb, int error, int einv_error)
{
    if( error == EINVAL)  /* Если закрыта другая сторона pipe */
        error = einv_error;
    if( error == ETIMEDOUT)
        error = 0;
    if(cb->__offset) ioEnd(cb, 0, cb->__offset); /* передано __offset байтов           */
    else             ioEnd(cb, error, 0);        /* не было передано ни одного байта   */
}
/*----------- rbCopy -------------
 * Функция rbCopy - копирование очередной порции выводимой информации
 *                  в кольцевой буфер чтения
 * Аргумент:
 *   rb - кольцевой буфер;
 *   cb - заявка на обмен (указатель на структуру aiocb);
 * Возвращаемое значение.
 *   Функция возвращает количество скопированных  (обработанных) байтов.
 */
static __inline__ int rbCopy(struct rngbuf *rb, struct aiocb *cb)
{
    int l = ringPut(rb, (char *)cb->aio_buf + cb->__offset,
                                                        cb->aio_nbytes-cb->__offset);
    cb->__offset += l;
    return l;
}
/*----------- flush -------------
 * Функция flush - сброс входной и выходной очереди к устройству
 * Аргументы:
 *   rb  - описатель rb-устройства.
 */
static __inline__ void rbFlush(rbDEV *rb)
{
    ringFlush2(rb->rb_rrb,rb->rb_wrb);
}

#endif /*RBDEV_H_*/
