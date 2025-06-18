/* htrace/trregistration.h */
#ifndef _TRACE_TRREGISTRATION_H
#define _TRACE_TRREGISTRATION_H

/* Действующая маска событий */
#if OS_MAJOR == 2
   /* Определение указателя на заголовок буфера трассы */
#  define traceGET_HEADBUFFER(p)                                                \
      traceHEADBUF   p = (traceHEADBUF)traceRingBuf
   /* Получить адрес глобальной маски событий */
#  define traceEventSet()    &traceGMask
#else
   /* OS_MAJOR >= 3 */
   /* Определение указателя на заголовок буфера трассы */
#  define traceGET_HEADBUFFER(p)                                                \
    traceHEADBUF p=traceGetProcHeaderAddress()
   /* Получить адрес глобальной маски событий */
#  define traceEventSet()    (&modLinkPage->mlp_traceFilter)
#endif /* OS_MAJOR == 2 */

/* Локальные переменные, объявляемые при регистрации события */
#define traceVARIABLES(tr)                                                      \
        u_int  traceInterrupt;                                                  \
        tr*    traceBuf;                                                        \
        traceGET_HEADBUFFER( pBufHeader ); if( pBufHeader == 0 ) return

/* Локальные переменные, объявляемые при регистрации события */
#define traceVARIABLESE(tr, e)                                                  \
        u_int  traceInterrupt;                                                  \
        tr*    traceBuf;                                                        \
        traceGET_HEADBUFFER( pBufHeader ); if( pBufHeader == 0 ) return e


/* Трассируется ли указанное событие? */
#define traceEVENT(id)                                                          \
      traceEVENTSET_ISMEMBER(id,traceEventSet())

/* Прерывание начатого протоколирования */
#define  traceBEGIN(pBufHeader) /* начать протоколирование */                   \
  traceInterrupt = pBufHeader->traceInterrupt
/* Протоколирование события прервано */
#define  traceINTERRUPT(pBufHeader)                                             \
  ++pBufHeader->traceInterrupt
#define  traceIS_INTERRUPT(pBufHeader)                                          \
  (traceInterrupt != pBufHeader->traceInterrupt)

/* Прочитать размер записи */
#define  traceGET_RECORD_SIZE(r,size)                                           \
    size = ((trcHEADER_RECORD_LIST*)(r))->size_record

/* Формирование группы записей одного события
 * задаем вид записи, событие, дополнительное пространство
 * в записи для информации клиента(clientSize), пространство для протоколирования
 * переменных (varSize).
 * Если формирование записи прерывалось, оно возобновляется
 */
#define traceCREATE_GROUP(tr,event,clientSize,varSize)                          \
{                                                                               \
  int size = traceGET_RECORDSIZE(sizeof(tr)+clientSize);                        \
  int listsize;   /* общий размер группы записей   */                           \
  tr* pEvent;     /* адрес записи события          */                           \
  do {                                                                          \
    traceBEGIN(pBufHeader);                                                     \
    if( (traceBuf=traceBegin(pBufHeader,event,size,varSize)) == NULL )          \
      break; /*нет места для протокола*/

/* Завершение формирования группы записей, на выходе tracebuf должен указывать
 * на запись события
 */
#define traceCOMPLETE_GROUP(adr,tr,event,rt)                                    \
    if( traceIS_INTERRUPT(pBufHeader) && !pBufHeader->traceInterruptIgnore )    \
      /* Повторяем протоколирование */                                          \
      continue;                                                                 \
    /* Завершаем формирование группы коррекцией заголовка */                    \
    /* Вначале заносим заголовок события, который следует за trcBEGIN_RECORD */ \
    pEvent = (tr*)((trcBEGIN_RECORD*)traceBuf+1);                               \
    traceSET_TYPESIZE( pEvent, event, size );                                   \
    /* Читаем размер группы из заголовка и заносим в trcBEGIN_RECORD */         \
    traceGET_RECORD_SIZE( traceBuf, listsize );                                 \
    traceSET_LIST_SIZE( traceBuf, listsize );                                   \
    /* Заносим контекст события, адрес и код завершения                      */ \
    traceSetHeader( pBufHeader, (trcHEADER_RECORD*)traceBuf, adr, rt );         \
    /* Осталось скорректировать заголовок trcBEGIN_RECORD,                   */ \
    /*  он еще имеет traceServiceBad и размер всей группы                    */ \
    traceSET_TYPESIZE( traceBuf, traceServiceBegin, sizeof(trcBEGIN_RECORD) );  \
    traceBuf = pEvent;                                                          \
    break;     /* протоколирование не прерывалось или прерывание игнорировано */\
  } while( 1 );                                                                 \
}

/* Формирование одной записи события
 * задаем вид записи, событие, дополнительное пространство
 * в записи для информации клиента.
 * Если формирование записи прерывалось, оно возобновляется
 */
#define traceCREATE_RECORD(tr,event,clientSize)                                 \
{                                                                               \
  int size = traceGET_RECORDSIZE(sizeof(tr)+clientSize);                        \
  do {                                                                          \
    traceBEGIN(pBufHeader);                                                     \
    if( (traceBuf=(tr*)traceAlloc(pBufHeader,size)) == NULL ) {                 \
      break; /* нет места для протокола */                                      \
    }

/* Завершение формирования одной записи события */
#define traceCOMPLETE_RECORD(adr,event,rt)                                      \
    if( traceIS_INTERRUPT(pBufHeader) && !pBufHeader->traceInterruptIgnore ) {  \
      /* Запись остается как bad (traceServiceBad) */                           \
      continue;                                                                 \
    }                                                                           \
    traceSetHeader( pBufHeader, (trcHEADER_RECORD*)traceBuf, adr, rt );         \
    traceSET_RECORD_TYPE( traceBuf, event ); /* заголовок записи создан */      \
    break; /*протоколирование не прерывалось*/                                  \
  } while( 1 );                                                                 \
}

/* Определить по адресу заголовка события адрес заголовка группы записей */
#define traceHLIST_TO_HEAD(hlist)                                               \
   (struct trcHeaderRecord*)((char*)(hlist) - sizeof(trcBEGIN_RECORD))

#endif /* _TRACE_TRREGISTRATION_H */
