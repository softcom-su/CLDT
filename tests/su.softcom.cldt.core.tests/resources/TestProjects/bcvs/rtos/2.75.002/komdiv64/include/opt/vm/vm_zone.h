/*
 * Copyright (c) 1997, 1998 John S. Dyson
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 * 1. Redistributions of source code must retain the above copyright
 *	notice immediately at the beginning of the file, without modification,
 *	this list of conditions, and the following disclaimer.
 * 2. Absolutely no warranty of function or purpose is made by the author
 *	John S. Dyson.
 *
 * $FreeBSD: src/sys/vm/vm_zone.h,v 1.12.2.1 1999/08/29 16:33:41 peter Exp $
 */

#ifndef _SYS_ZONE_H

#define _SYS_ZONE_H

#define ZONE_INTERRUPT 1 /* Use this if you need to allocate at int time */
#define ZONE_BOOT 16	 /* This is an internal flag used by zbootinit */

#include	<arch/lock.h>
#include <assert.h>

typedef struct vm_zone
{
   /*	struct simplelock zlock;	*//* lock for data structure */
   void *zitems; /* linked list of items */
   int zfreecnt; /* free entries */
   int zfreemin; /* minimum number of free entries */
   int znalloc; /* number of allocations */
   /*	vm_offset_t	zkva;		*//* Base kva of zone */
   /*	int		zpagecount;	*//* Total # of allocated pages */
   /*	int		zpagemax;	*//* Max address space */
   int zmax; /* Max number of entries allocated */
   int ztotal; /* Total entries allocated now */
   int zsize; /* size of each entry */
   int zalloc; /* hint for # of pages to alloc */
   int zflags; /* flags for zone */
   /*	int		zallocflag;	*//* flag for allocation */
   /*	struct vm_object *zobj;		*//* object to hold zone */
   char *zname; /* name for diags */
   struct vm_zone *znext; /* list of zones for sysctl */
}*vm_zone_t;

void  zerror (int) __dead2;
vm_zone_t zinit (char *name, int size, int nentries, int flags, int zalloc);
int   zinitna (vm_zone_t z, int obj, char *name, int size, int nentries, int flags, int zalloc);
void  *zalloci (vm_zone_t z);
void  zfreei (vm_zone_t z, void *item);
void  *zalloc(vm_zone_t z);
void  zfree(vm_zone_t z, void *item);
void  zbootinit (vm_zone_t z, char *name, int size, void *item, int nitems);
void  *_zget (vm_zone_t z);

#define ZONE_ERROR_INVALID 0
#define ZONE_ERROR_NOTFREE 1
#define ZONE_ERROR_ALREADYFREE 2

#define ZONE_ROUNDING	32

#define ZENTRY_FREE	0x12342378
/*
 * void *zalloc(vm_zone_t zone) --
 * Returns an item from a specified zone.
 *
 * void zfree(vm_zone_t zone, void *item) --
 *  Frees an item back to a specified zone.
 */
static __inline__ void *
_zalloc(vm_zone_t z)
{
   void *item;
   assert(splhigh() != 0);

#ifdef INVARIANTS
   if (z == 0)
   zerror(ZONE_ERROR_INVALID);
#endif
   if (z->zfreecnt <= z->zfreemin)
      return _zget(z);
   item = z->zitems;
   z->zitems = ((void **) item)[0];
   z->zfreecnt--;
   z->znalloc++;
   return item;
}

static __inline__ void _zfree(vm_zone_t z, void *item)
{
   assert(splhigh() != 0);
   ((void **) item)[0] = z->zitems;
   z->zitems = item;
   z->zfreecnt++;
}

#endif
