/*
 * Copyright (c) 1997, by Steve Passe
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 * 1. Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 * 2. The name of the developer may NOT be used to endorse or promote products
 *    derived from this software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE AUTHOR AND CONTRIBUTORS ``AS IS'' AND
 * ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
 * ARE DISCLAIMED.  IN NO EVENT SHALL THE AUTHOR OR CONTRIBUTORS BE LIABLE
 * FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
 * DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS
 * OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION)
 * HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT
 * LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY
 * OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF
 * SUCH DAMAGE.
 *
 * $FreeBSD: src/sys/alpha/include/lock.h,v 1.3.2.1 1999/08/29 15:56:59 peter Exp $
 */


#ifndef _MACHINE_LOCK_H_
#define _MACHINE_LOCK_H_
struct simplelock {
   void *lock_owner;
   int   lock_wait;
};
#ifdef SIMPLELOCK_DEBUG
void _simple_unlock __P((struct simplelock *alp, const char *, int));
#define simple_unlock(alp) _simple_unlock(alp, __FILE__, __LINE__)
int _simple_lock_try __P((struct simplelock *alp, const char *, int));
#define simple_lock_try(alp) _simple_lock_try(alp, __FILE__, __LINE__)
void _simple_lock __P((struct simplelock *alp, const char *, int));
#define simple_lock(alp) _simple_lock(alp, __FILE__, __LINE__)
void simple_lock_init __P((struct simplelock *alp));
#else /* !SIMPLELOCK_DEBUG */
void simple_unlock (struct simplelock *alp);
int simple_lock_try (struct simplelock *alp);
void simple_lock (struct simplelock *alp);
void simple_lock_init (struct simplelock *alp);
#endif /* !SIMPLELOCK_DEBUG */

#endif /* !_MACHINE_LOCK_H_ */
