/* channel/kernchannel.h */

#ifndef _CHANNEL_KERNCHANNEL_H
#define _CHANNEL_KERNCHANNEL_H


//#ifdef KERNEL

struct sapport_conf {
  int                  active;
  const char*          name;           /* имя порта                        */
  PORT_DIRECTION_TYPE  direction;      /* направление передачи данных      */
  int                  size;           /* максимальный размер сообщения    */
  int                  buffers;        /* число буферов                    */
  char                *protocol;       /* протокол                         */
  int                  extended;       /*                                  */
};

#if OS_MAJOR >= 3
/* Конфигурация queuing-порта */
struct qport_conf {
  int                  active;
  const char*          name;           /* имя порта */
  PORT_DIRECTION_TYPE  direction;      /* направление передачи данных                  */
  long                 size;           /* максимальный размер сообщения */
  long                 buffers;        /* число буферов */
  const char*          destination;    /* имя файла (порта), куда выводится информация */
  FILE                 *fp;
};

/* Конфигурация sampling-порта */
struct sport_conf {
  int                  active;
  const char*          name;           /* имя порта */
  PORT_DIRECTION_TYPE  direction;      /* направление передачи данных                  */
  long                 size;           /* максимальный размер сообщения */
  double               refresh;           /* максимальный размер сообщения */
  const char*          destination;    /* имя файла (порта), куда выводится информация */
};
#else
/* Конфигурация queuing-порта */
struct qport_conf {
  int                  active;
  const char*          name;           /* имя порта                        */
  PORT_DIRECTION_TYPE  direction;      /* направление передачи данных      */
  long                 size;           /* максимальный размер сообщения    */
  long                 buffers;        /* число буферов                    */
};

/* Конфигурация sampling-порта */
struct sport_conf {
  int                  active;
  const char*          name;           /* имя порта                        */
  PORT_DIRECTION_TYPE  direction;      /* направление передачи данных      */
  long                 size;           /* максимальный размер сообщения    */
  double               refresh;        /* период обновления сообщения      */
};

/* Конфигурация соединения */
struct connect_conf {
   int                  active;
   int                  channelId;     /* идентификатор канала             */
   const char*          channelName;   /* имя канала                       */
   const char*          portName;      /* имя порта                        */
   PORT_DIRECTION_TYPE  direction;     /* направление передачи данных      */
   int                  cpu;           /* партнер                          */
   int                  link;          /* номер логической связи           */
};

#endif

//#endif /*KERNEL*/

#endif /* _CHANNEL_KERNCHANNEL_H */
