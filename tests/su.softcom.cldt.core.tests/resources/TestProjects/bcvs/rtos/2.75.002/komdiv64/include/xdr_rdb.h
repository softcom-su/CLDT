/* xdr_rdb.h - xdr for additional rdb structures */

/* Copyright 1992 Free Software Foundation, Inc.

   This code was donated by Wind River Systems, Inc. */


#ifndef __INCxdrrdbh
#define __INCxdrrdbh

#include  <rpc/types.h>

enum arg_type {
 T_UNKNOWN = 0,
 T_BYTE = 1,
 T_WORD = 2,
 T_INT = 3,
 T_FLOAT = 4,
 T_DOUBLE = 5
};
typedef enum arg_type arg_type;
bool_t xdr_arg_type();

struct arg_value {
 arg_type type;
 union {
  char v_byte;
  short v_word;
  int v_int;
  float v_fp;
  double v_dp;
 } arg_value_u;
};
typedef struct arg_value arg_value;
bool_t xdr_arg_value();

struct func_call {
 long func_addr;
 struct {
  u_int args_len;
  arg_value *args_val;
 } args;
};
typedef struct func_call func_call;
bool_t xdr_func_call();


typedef char *arg_one;
bool_t xdr_arg_one();


typedef struct {
 u_int arg_array_len;
 arg_one *arg_array_val;
} arg_array;
bool_t xdr_arg_array();


enum NOTIFICATION_TYPE {
 NOTIFICATION_BREAK = 0,
 NOTIFICATION_STOP = 1,
 NOTIFICATION_EXIT = 2,
 NOTIFICATION_BUS_ERR = 3,
 NOTIFICATION_SUSPEND = 4,
 NOTIFICATION_ZERO_DIV = 5,
 NOTIFICATION_OCSIGNAL = 6,
 NOTIFICATION_START = 7,
 NOTIFICATION_CREATETHREAD = 8,
};
typedef enum NOTIFICATION_TYPE NOTIFICATION_TYPE;


struct RDB_NOTIFICATION {
	int status;
	int thread;
 NOTIFICATION_TYPE notificationType;
 int sigType;
};
typedef struct RDB_NOTIFICATION RDB_NOTIFICATION;
/* For NOTIFICATION_BREAK sigType is breackpoint type. */
/* Breackpoints types: */
#define BRK_UNKNOWN  0
#define BRK_COM      1
#define BRK_HW       2

struct THREAD_START {
 int status;
 int pid;
};
typedef struct THREAD_START THREAD_START;


struct SYMBOL_ADDR {
 int status;
 unsigned long  addr;
};
typedef struct SYMBOL_ADDR SYMBOL_ADDR;

struct SOURCE_STEP {
	int threadId;
	unsigned long startAddr;
	unsigned long endAddr;
};
typedef struct SOURCE_STEP SOURCE_STEP;

#define MAX_ARG_CNT 10
#define MAX_FUNC_ARGS 100
#define MAX_ARG_LEN   100


bool_t xdr_NOTIFICATION_TYPE();
bool_t xdr_RDB_NOTIFICATION();
bool_t xdr_THREAD_START();
bool_t xdr_SYMBOL_ADDR();
bool_t xdr_SOURCE_STEP();

#define RDBPROG (uint32_t) 0x44444444
#define RDBVERS (uint32_t) 3

/* Идентификатор потока и макросы извлечения из него информации: */
typedef uint32_t  rdbThreadId ;
#ifndef GET_PID
# define GET_PID(rdbId) (((rdbId) & 0x7F000000) ? (((rdbId & 0x7F000000)>>24)-0x13) : 0)
# define GET_THREAD(rdbId) (((rdbId) & 0xFFFFFF) ? (((rdbId) & 0xFFFFFF) | 0x93000000) : 0)
#endif 

/* Status: */
#define STATE_DELAYED     0x0001 /* Поток помещён в очередь к часам D */
#define STATE_DEAD        0x0002 /* Поток завершен и ждёт pthread_join() E */
#define STATE_PENDED      0x0004 /* Поток в очереди к ресурсу P */
#define STATE_READY       0x0008 /* Поток находится  в очереди готовых R */
#define STATE_SUSPENDED   0x0010 /* Поток приостановлен S */
#define STATE_TERMINATION 0x0020 /* Поток находится в стадии завершения T */
#define STATE_WAITED      0x0040 /* Поток в ожидании события(ий) W */
#define ATTR_UNBREAKABLE  0x0080 /* Поток unbreakable U */

/* Flags: */
#define ATTR_ASYNCHRONOS   0x00010000 /* поток допускает ассинхронное удаление A */
#define REQUEST_CANCEL     0x00020000 /* запрос на удаление потока C */
#define ATTR_DETACHED      0x00040000 /* описатель потока можно освобождать сразу D */
#define STATE_SAFE         0x00080000 /* поток не может быть удален N */
#define CANCELLATION_POINT 0x00100000 /* поток находится в точке удаления P */

#endif	/* __INCxdrrdbh */
