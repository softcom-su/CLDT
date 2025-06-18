/* prsmemconfig.c */
#include <private/bindmodule.h>
#include <prsmem/prsmem.h>


#define PRSMEMLINE(n) {       \
  PRSMEM_PART_CONFIG##n,      \
  0,                          \
  PRSMEM_PART_NAME##n,        \
  PRSMEM_PART_SIZE##n,        \
  }

static struct prsmem_partconf prsmem_config [PRSMEM_MAX_PATITION] = {
#ifdef INCLUDE_PRSMEM
   PRSMEMLINE(0),
   PRSMEMLINE(1),
   PRSMEMLINE(2),
   PRSMEMLINE(3),
   PRSMEMLINE(4),
   PRSMEMLINE(5),
   PRSMEMLINE(6),
   PRSMEMLINE(7)
#endif
};

static struct prsmem_config       prsmemConfig;
int prsmemGetSize(pid_t pid)
{
   int s = 0, i;
   for (i = 0; i < PRSMEM_MAX_PATITION; i++)
   {
      if (prsmem_config[i].active && prsmem_config[i].pid == pid && (prsmem_config[i].name[0] != '\0'))
      {
         s+= prsmem_config[i].size;
      }
   }
   return s + sizeof(prsmemMAIN);
}
const struct prsmem_config *prsmemGetConfig()
{
   return &prsmemConfig;
}

/*----------- prsmemPartNumber --------------
 * Функция prsmemPartNumber - получить число частей сохраняемой памяти
 */
static int prsmemPartNumber(void)
{
   int n = 0;
   int i;
   int j;

   for (i = 0; i < PRSMEM_MAX_PATITION; i++)
      if (prsmem_config[i].active && (prsmem_config[i].name[0] != '\0'))
      {
         if (!prsmem_config[i].size)
            continue;
         /* Проверим дублирование имен */
         for ( j=i+1; j < PRSMEM_MAX_PATITION; j++ )
            if( !strcmp(prsmem_config[i].name,prsmem_config[j].name) )
               prsmem_config[j].active = 0;
         if (n != i)
         {
            prsmem_config[n] = prsmem_config[i];
            prsmem_config[i].active = 0;
         }
         ++n;
      }
   return n;
}

/*----------- prsmemInit --------------
 * Функция prsmemInit - инициализация области сохранения
 *
 */
static int prsmemInit( void  )
{
   int              rt = 0;
#ifdef INCLUDE_PRSMEM
   int              n;

   prsmemShowInit();

   if( (n=prsmemPartNumber()) == 0 )   /* получить число сконфигурированных частей     */
     return ECONFIG;                   /* нет в конфигурации                           */

   prsmemConfig.pc_allocation = PRSMEM_ALLOC;
   prsmemConfig.pc_addres = (void*)PRSMEM_ADDRESS;
   // prsmemConfig.pc_nparts = n;
   prsmemConfig.pc_parts = prsmem_config;
#  ifdef  INCLUDE_VME
   prsmemConfig.pc_shmem = boardShMem;
#  else
   prsmemConfig.pc_shmem = NULL;
#  endif

   rt = _prsmemInit( 0 );
   if( !rt && (prsmemConfig.pc_allocation == PRSMEM_ALLOC_ADR) )
      prsmeminfo = prsmemGetInfo;
#endif
   return rt;
}
