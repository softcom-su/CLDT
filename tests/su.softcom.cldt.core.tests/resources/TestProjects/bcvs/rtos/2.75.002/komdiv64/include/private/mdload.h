/* mdload.h - структуры и функции для работы с модулями.  */

#ifndef __INCmdloadh
#define __INCmdloadh

#include <limits.h>
#include <private/objcore.h>
#include <semaphore.h>
#ifdef __cplusplus
extern "C" {
#endif

/* максимальное число элементов для списков зависимостей: */
#define MAX_DEPEND_REF  200

/* первичное число элементов в списке модулей: */ 
#define F_NUMBER_MODULE  20

/* макимальное число сегментов модуля: */
#define MAX_SEG   3

/* типы сегментов: */
#define SEG_TEXT  1 
#define SEG_DATA  2 
#define SEG_BSS   3 

/* флаги сегментов: */
#define FSEG_CHECK 0x01

/* флаги модулей: */
#define FMOD_RELOAD 0x01

 typedef struct segment {
  void *m_pFree ;	/* адрес для сдачи памяти */
  void *m_pBeg ;  /* адрес начала сегмента */
  int		m_size ;		  /* размер сегмента */
  int		m_type ;		  /* тип сегмента */
  int		m_flags ;		 /* флаги сегмента */
  unsigned short	m_checksum;	/* контрольная сумма, возвращаемая функцией checksum() */
 } SEGMENT ;



 typedef struct module {
  objHEADER       m_objHdr;
  u_long           m_flags;	/* флаги */
  u_long           m_module ; /* идентификатор модуля */
  SEGMENT m_seg[MAX_SEG] ;   /* список сегментов модуля */
  sdlHEAD	m_dep ;	  /* список модулей от которых зависит этот */
  sdlHEAD	m_ref ;	  /* список модулей, которые ссылаются на этот  */
  char	m_name [NAME_MAX]; /* имя модуля */
  char	m_path [PATH_MAX]; /* путь к модулю */
 } MODULE, *MODULE_ID;


 typedef struct depend {
  sdlNODE 	  node;	       /* узел элемента */
  int m_refMod ;          /* ссылка на модуль */
 } DEPREF ;

#define MOD_SHOW_DEF  0x1
#define MOD_SHOW_USER 0x2

/* Описание функций: */
int load(char *modName) ;
MODULE_ID elfload(int fd, char *module) ;
int unload(char *name) ;
int addSegment(MODULE_ID module, int type, int flags, void *pF, void *pA, int size) ;
int setNameModule(MODULE_ID module, char *modName) ;
MODULE_ID getNewModule(void) ;
int removeModule(MODULE_ID module) ;
int unlockModMutex (void) ;
MODULE_ID findModuleByName(char *name) ;
MODULE_ID findModuleById(int modId) ;
void InitElfLoad(void) ;

void cycleModList(funcptr_t usFunc, void *usParm) ; 
/* usFunc(MODULE_ID mod, vois usParm, int sizeModList) */

#define MOD_DEP_REF_ADD    0
#define MOD_DEP_REF_REMOVE 1
int changeModuleRefList(MODULE_ID module, int refId, int op) ;
int changeModuleDepList(MODULE_ID module, int depId, int op) ;

#ifdef __cplusplus
}
#endif

#endif /* __INCmdloadh */
