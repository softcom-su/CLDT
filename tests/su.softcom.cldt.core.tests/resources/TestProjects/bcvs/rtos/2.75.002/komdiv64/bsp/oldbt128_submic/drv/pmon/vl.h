/*
 * Copyright (c) 2011 Vyacheslav A. Naumenko (naumenko@niisi.msk.ru)
 *
 * THIS SOFTWARE IS PROVIDED BY THE AUTHOR ``AS IS'' AND ANY EXPRESS
 * OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
 * WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
 * ARE DISCLAIMED.  IN NO EVENT SHALL THE AUTHOR BE LIABLE FOR ANY
 * DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
 * DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS
 * OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION)
 * HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT
 * LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY
 * OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF
 * SUCH DAMAGE.
 *
 * OC PB code added by Y.M. Lazutin - 09apr2012
 */

#ifndef	__VL_H
#define	__VL_H

#define VL_MAX_NAME_LENGTH	0x100
#define VL_MAX_VALUE_LENGTH	0x1000
#define VL_BASE_ADDR           (char *)0xafc00000   /* 0xafc00000 */
#define VL_SAVE_ADDR           (char *)0xa0001000
#define PMONS_VARS_MAX_LEN     (16*1024)

int vl_count(const char *vl);
int vl_get(const char *vl, int num, char *name, char *value);
int vl_get_var(const char *vl, const char *name, char *value);
int vl_set_var(char *vl, const char *name, const char *value);
int vl_set_vl(char *dest, char *src);
int vl_unset_var(char *vl, const char *name);

#endif /* __VL_H */
