/* flashmemconfig.c */

#include            <stdio.h>
#include <public/flashmem.h>

#ifdef INCLUDE_FLASHMEM
#if  OS_MAJOR == 2
void *mlp_flashMemory = 0 ;
#include  <sys/malloc.h>
#endif

/*--------- В конфигураторе заданы области во flash-памяти --------*/
#define FLASHMEMLINE(n) {       \
  FLASHMEM_PART_CONFIG##n,      \
  FLASHMEM_PART_NAME##n,        \
  FLASHMEM_PART_OFFSET##n,      \
  FLASHMEM_PART_SIZE##n,        \
  }
static const struct flashmem_area flashsmem_config [FLASHMEM_MAX_AREAS] = {
   FLASHMEMLINE(0),
   FLASHMEMLINE(1),
   FLASHMEMLINE(2),
   FLASHMEMLINE(3),
   FLASHMEMLINE(4),
   FLASHMEMLINE(5),
   FLASHMEMLINE(6),
   FLASHMEMLINE(7)
};
/*-------------------------- flashmemInit -------------------------*/
/* Функция flashmemInit - инициализация описателя устройства       */
/*  flash-памяти целевого модуля                                   */
/*-----------------------------------------------------------------*/
int flashmemInit(void)
{
 int i, j, nsec_a, c_ok, ret = 0 ;
 struct flash_memory d_flash,*sm_flash ;/* Описатель устройства РПЗУ */
 flashInfo(&d_flash.nsec,&d_flash.ssec,(int **)&d_flash.baseAddr);   /* Чтение описания флеш-памяти */
 for (c_ok = d_flash.flags = i = 0 ; i < FLASHMEM_MAX_AREAS ; i++)
 {
     if (flashsmem_config[i].active == 0)
        continue ;  /* Пропустить неактивную область */
     if (d_flash.flags & (1<<i))
        continue ;  /* Пропустить ошибочную область */
     nsec_a = flashsmem_config[i].offset/d_flash.ssec ; /* Начальный сектор области */
     if (nsec_a * d_flash.ssec != flashsmem_config[i].offset)
     {/* Ошибка адрес начала области не совпадает с началом сектора */
         d_flash.flags |= (1<<i) ;   /* Пометить ошибочную область */
         printf("flashmemInit: offset of flash area %d (0x%lx) is wrong\r\n",
                     i+1,flashsmem_config[i].offset) ;
         continue ;
     }
     if (nsec_a < 0 || nsec_a >= d_flash.nsec)
     {/* Слишком большой offset */
         d_flash.flags |= (1<<i) ;   /* Пометить ошибочную область */
         printf("flashmemInit: offset of flash area %d (0x%lx) is wrong\r\n",
                     i+1,flashsmem_config[i].offset) ;
         continue ;
     }
     if (nsec_a + flashsmem_config[i].count > d_flash.nsec || nsec_a + flashsmem_config[i].count <= 0)
     {/* Слишком большой размер */
         d_flash.flags |= (1<<i) ;   /* Пометить ошибочную область */
         printf("flashmemInit: numbers of sectors in area %d (0x%x) is wrong\r\n",
                     i+1,flashsmem_config[i].count) ;
         continue ;
     }
     c_ok++ ; /* Счётчик активных областей */
     for (j = i+1 ; j < FLASHMEM_MAX_AREAS ; j++)
     {
        if (flashsmem_config[j].active == 0)
           continue ;  /* Пропустить неактивную область */
        if (d_flash.flags & (1<<j))
           continue ;  /* Пропустить ошибочную область */
        if (strcmp(flashsmem_config[i].name,flashsmem_config[j].name)==0)
        {/* Ошибка совпадение имён у различных областей */
             d_flash.flags |= (1<<j) ;   /* Пометить ошибочную область */
             printf("flashmemInit: name of flash area %d (%s) is wrong\r\n",
                     j+1,flashsmem_config[j].name) ;
        }
     }
 }
 if (c_ok==0)
    ret = ENOMEM ;

#if OS_MAJOR == 2
 sm_flash = (struct flash_memory *)syspool_malloc(sizeof(d_flash),0,0);
 mlp_flashMemory = sm_flash;
 if (sm_flash)
    *sm_flash = d_flash ;
#else
 MODCALL(
/* Указатель на описатель устройства РПЗУ в описатель модуля */
 sm_flash = (struct flash_memory *)modMPDSharedAlloc(shmemCACHED_SYSDATA, sizeof(d_flash), 8);
 modLinkPageInternal->mlp_flashMemory = sm_flash;
 if (sm_flash)
    *sm_flash = d_flash ;
 );
#endif
 if (sm_flash==0)
    return  ENOMEM ;
 return ret ;
}
/*---------------------- getIndexFlashArea ------------------------*/
/* Функция getIndexFlashArea по имени области, возвращает индекс   */
/* описания области в массиве flashsmem_config                     */
/*-----------------------------------------------------------------*/
int getIndexFlashArea(const char *name)
{
 int ind = -1, i ;
 for (i = 0 ; i < FLASHMEM_MAX_AREAS ; i++)
 {
     if (flashsmem_config[i].active==0)
         continue ;
     if (strcmp(flashsmem_config[i].name,name)== 0)
     {
        ind = i ;
        break ;
     }
 }
 return ind ;
}
/*------------------- getFlashAreaDisc ----------------------------*/
/* Функция getFlashAreaDisc возвращает указатель на описатель      */
/* области памяти в РПЗУ (flash-памяти)                            */
/*-----------------------------------------------------------------*/
const struct flashmem_area *getFlashAreaDisc(int ind)
{
 const struct flashmem_area *disc ;
 if (ind < 0 || ind >= sizeof(flashsmem_config)/sizeof(struct flashmem_area))
    disc = 0 ;
 else
    disc = &flashsmem_config[ind] ;
 return disc ;
}
/*-----------------------------------------------------------------*/
# if  (OS_MAJOR == 2) && defined(malloc)
#  undef  malloc
# endif
#else
/*---------------- Нет областей во flash-памяти -------------------*/
int getIndexFlashArea(const char *name)
{
    return -1 ;
}
const struct flashmem_area *getFlashAreaDisc(int ind)
{
    return 0 ;
}
#endif 
