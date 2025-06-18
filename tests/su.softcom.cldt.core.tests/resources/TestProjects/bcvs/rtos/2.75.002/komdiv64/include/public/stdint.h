#ifndef _STDINT_H
#define _STDINT_H

typedef char                      int8_t;
typedef unsigned char             uint8_t;
typedef short                     int16_t;
typedef unsigned short            uint16_t;
typedef int                       int32_t;
typedef long long int             int64_t;
typedef unsigned int              uint32_t;
typedef unsigned long long int    uint64_t;

typedef char                      int_least8_t;
typedef unsigned char             uint_least8_t;
typedef short                     int_least16_t;
typedef unsigned short            uint_least16_t;
typedef int                       int_least32_t;
typedef unsigned int              uint_least32_t;

typedef char                      int_fast8_t;
typedef unsigned char             uint_fast8_t;
typedef int                       int_fast16_t;
typedef unsigned int              uint_fast16_t;
typedef int                       int_fast32_t;
typedef unsigned int              uint_fast32_t;

typedef long                      intptr_t;
typedef unsigned long             uintptr_t;

typedef unsigned long long int    uintmax_t;
typedef long long int             intmax_t;

#define  INT8_MIN          (0x80)
#define  INT16_MIN         (0x8000)
#define  INT32_MIN         (0x80000000)
#define  INT64_MIN         (0x8000000000000000LL)

#define  INT8_MAX          (0x7f)
#define  INT16_MAX         (0x7fff)
#define  INT32_MAX         (0x7fffffff)
#define  INT64_MAX         (0x7FFFFFFFFFFFFFFFLL)

#define  UINT8_MAX         (0xff)
#define  UINT16_MAX        (0xffff)
#define  UINT32_MAX        (0xffffffffU)
#define  UINT64_MAX        (0xFFFFFFFFFFFFFFFFULL)

#define  INT_LEAST8_MAX    INT8_MAX
#define  INT_LEAST8_MIN    INT8_MIN
#define  UINT_LEAST8_MAX   UINT8_MAX
#define  INT_LEAST16_MAX   INT16_MAX
#define  INT_LEAST16_MIN   INT16_MIN
#define  UINT_LEAST16_MAX  UINT16_MAX
#define  INT_LEAST32_MAX   INT32_MAX
#define  INT_LEAST32_MIN   INT32_MIN
#define  UINT_LEAST32_MAX  UINT32_MAX
#define  INT_LEAST64_MAX   INT64_MAX
#define  INT_LEAST64_MIN   INT64_MIN
#define  UINT_LEAST64_MAX  UINT64_MAX

#define  INT_FAST8_MAX     INT8_MAX
#define  INT_FAST8_MIN     INT8_MIN
#define  UINT_FAST8_MAX    UINT8_MAX
#define  INT_FAST16_MAX    INT16_MAX
#define  INT_FAST16_MIN    INT16_MIN
#define  UINT_FAST16_MAX   UINT16_MAX
#define  INT_FAST32_MAX    INT32_MAX
#define  INT_FAST32_MIN    INT32_MIN
#define  UINT_FAST32_MAX   UINT32_MAX
#define  INT_FAST64_MAX    INT64_MAX
#define  INT_FAST64_MIN    INT64_MIN
#define  UINT_FAST64_MAX   UINT64_MAX

#define  INTMAX_MAX        INT64_MAX
#define  INTMAX_MIN        INT64_MIN
#define  UINTMAX_MAX       UINT64_MAX


#endif  /* !_STDINT_H */
