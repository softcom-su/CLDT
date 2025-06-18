/* arinc/logbook.h */
/*
 *--------------------------------------------------------------------------
 * Logbook system constant and type definitions and management services 
 *--------------------------------------------------------------------------
 */
#ifndef _ARINC_LOGBOOK_H
#define _ARINC_LOGBOOK_H

#include <sys/apextypes.h>

typedef unsigned int LOGBOOK_ID_TYPE;  /* идентификатор журнала во флеш память */
typedef NAME_TYPE  LOGBOOK_NAME_TYPE;   /* имя журнала во флеш память          */

/* Возможное состояние сообщения в журнале во флеш память */
typedef enum {
ABORTED = 0,
IN_PROGRESS = 1,
COMPLETE = 2
} WRITE_STATUS_TYPE;

/* Структура описателя состояния буфера журнала во флеш память */
typedef struct {
APEX_INTEGER MAX_MESSAGE_SIZE;
APEX_INTEGER MAX_NB_LOGGED_MESSAGES;
APEX_INTEGER MAX_NB_IN_PROGRESS_MESSAGES;
APEX_INTEGER NB_LOGGED_MESSAGES;
APEX_INTEGER NB_IN_PROGRESS_MESSAGES;
APEX_INTEGER NB_ABORTED_MESSAGES;
} LOGBOOK_STATUS_TYPE;
/*                        in     in   in    in   out out  */
#define  CREATE_LOGBOOK(name, size, nb_1, nb_2, id, ret)  \
    *(ret) = logbookCreate(name, size, nb_1, nb_2, id)
/*                      in    in    in   out              */
#define  WRITE_LOGBOOK(id, mess, size, ret)               \
    *(ret) = logbookWrite(id, mess, size)
/*                          in    in   in   out           */
#define  OVERWRITE_LOGBOOK(id, mess, size, ret)           \
    *(ret) = logbookOverWrite(id, mess, size)
/*                     in    in    in     in    out   out */
#define  READ_LOGBOOK(id, entry, mess, size, status, ret) \
    *(ret) = logbookRead(id, entry, mess, size, status)
/*                      in   out                          */
#define  CLEAR_LOGBOOK(id, ret)                           \
    *(ret) = logbookClear(id)
/*                         in  out  out                   */
#define  GET_LOGBOOK_ID(name, id, ret)                    \
    *(ret) = logbookGetId(name, id)
/*                           in   out    out              */
#define  GET_LOGBOOK_STATUS(id, status, ret)              \
    *(ret) = logbookGetStatus (id, status)
extern int  logbookCreate(LOGBOOK_NAME_TYPE name, MESSAGE_SIZE_TYPE size,
    MESSAGE_RANGE_TYPE max_nm, MESSAGE_RANGE_TYPE max_pr, LOGBOOK_ID_TYPE *id) ;
extern int logbookWrite(LOGBOOK_ID_TYPE id, MESSAGE_ADDR_TYPE mess, MESSAGE_SIZE_TYPE size) ;
extern int logbookOverWrite(LOGBOOK_ID_TYPE id, MESSAGE_ADDR_TYPE mess, MESSAGE_SIZE_TYPE size) ;
extern int logbookRead(LOGBOOK_ID_TYPE id, MESSAGE_RANGE_TYPE entry,
                    MESSAGE_ADDR_TYPE mess, MESSAGE_SIZE_TYPE *size, WRITE_STATUS_TYPE *status) ;
extern int logbookClear(LOGBOOK_ID_TYPE id) ;
extern int logbookGetId(LOGBOOK_NAME_TYPE name, LOGBOOK_ID_TYPE *id) ;
extern int logbookGetStatus (LOGBOOK_ID_TYPE id, LOGBOOK_STATUS_TYPE *status) ;

typedef union {
  LOGBOOK_STATUS_TYPE  *status ;
  MESSAGE_ADDR_TYPE     mess   ;
  char *     name   ;
} LGB_POINTER ;

typedef  enum {
  LGB_CREATE,
  LGB_WRITE,
  LGB_OVERWRITE,
  LGB_READ,
  LGB_CLEAR,
  LGB_GETID,
  LGB_GETSTATUS,
} LGB_FUNC ;

typedef struct {
  LOGBOOK_ID_TYPE   idLog ;
  LGB_POINTER       pnt ;
  APEX_INTEGER      max_size ;
  APEX_INTEGER      max_numb ;
  APEX_INTEGER      max_prog ;
  APEX_INTEGER      size ;
  APEX_INTEGER      entry ;
  WRITE_STATUS_TYPE *stat ;
  APEX_INTEGER      *size_out ;
  LOGBOOK_ID_TYPE   *idLog_out ;
} LGB_PARMS ;

int __logbookSystem(LGB_FUNC  func, LGB_PARMS *parms) ;
#endif /* _ARINC_LOGBOOK_H */


