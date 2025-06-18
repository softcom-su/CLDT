typedef struct __ts_info {
    int             cause;
    unsigned int    address;
    void           *arg;
} tryINFO;

struct __ts
{
    jmp_buf   jb;
    struct  __ts *ts;
    tryINFO __ts_info;
	u_int     nestlevel;
};

extern struct __ts *tryList;
/*----------- tryStop -------------
 * Р¤СѓРЅРєС†РёСЏ tryStop - СЃР±СЂРѕСЃ СЃРїРёСЃРєР° Р°РєС‚РёРІРЅС‹С… TRY-CATCH Р±Р»РѕРєРѕРІ РІРїР»РѕС‚СЊ РґРѕ СѓРєР°Р·Р°РЅРЅРѕРіРѕ
 * РђСЂРіСѓРјРµРЅС‚:
 *   ts - СѓРєР°Р·Р°С‚РµР»СЊ РЅР° TRY-CATCH Р±Р»РѕРє.
 */
static __inline__ void tryStop(struct __ts *ts)
{
    if( tryList == ts )
        tryList = ts->ts; 
}

#define tryBegin()                                         \
    { struct __ts __ts;                                    \
      __ts.ts  = tryList;                                  \
	  __ts.nestlevel=intNestLevel;						   \
      bzero( &__ts.__ts_info, sizeof(__ts.__ts_info));	   \
      tryList  = &__ts;                                    \
      if(!setjmp(__ts.jb)) {

#define tryCatch()      } else {

#define tryEnd()        } tryStop(&__ts); }

#define tryThrow(p) throwException(__ts.__ts_info.cause,p)

#define tryInfo(pinfo) *(pinfo) = __ts.__ts_info

void throwException( int cause, void *arg );

