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
 */

#include <version.h>
#include "drv/pmon/vl.h"

#include <ctype.h>
#include <string.h>

static char *vl_get_line(const char *vl, int num)
{
	if (!vl || num < 0) return NULL;

	while (num--) while ('\n' != *vl++);

	return (char*) vl;
}

int vl_count(const char *vl)
{
	int count = 0;

	if (vl) {
		do {
			if (-1 == vl_get(vl, 0, NULL, NULL)) return 0;
			vl = strchr(vl, '\n');
			if (!vl) return 0;
			count++;
		} while (*++vl);
	}

	return count;
}

int vl_get(const char *vl, int num, char *name, char *value)
{
	int count;

	if (!vl || 0 > num || !(vl = vl_get_line(vl, num))) return -1;

	count = VL_MAX_NAME_LENGTH;
	if (name) {
		while (--count && (isprint(*vl) || isspace(*vl)) && !strchr(":\n", *vl)) *name++ = *vl++;
		*name = '\0';
	}
	else {
		while (--count && (isprint(*vl) || isspace(*vl)) && !strchr(":\n", *vl)) vl++;
	}

	if (*vl != ':') {
		return -1;
	}
	else {
		vl++;
	}

	count = VL_MAX_VALUE_LENGTH;
	if (value) {
		while (--count && (isprint(*vl) || isspace(*vl)) && '\n' != *vl) *value++ = *vl++;
		*value = '\0';
	}
	else {
		while (--count && (isprint(*vl) || isspace(*vl)) && '\n' != *vl) vl++;
	}

	if ('\n' != *vl) return -1;

	return num;
}

int vl_get_var(const char *vl, const char *name, char *value)
{
	if (vl && name) {
		char tmp_name[VL_MAX_NAME_LENGTH];
		char tmp_value[VL_MAX_VALUE_LENGTH];
		int num;
		int count = vl_count(vl);

		for (num = 0; num < count; num++) {
			vl_get(vl, num, tmp_name, tmp_value);
			if (!strcmp(tmp_name, name)) {
				if (value) strcpy(value, tmp_value);
				return num;
			}
		}
	}

	return -1;
}

int vl_set_var(char *vl, const char *name, const char *value)
{
	if (vl &&
	name && !strchr(name, ':') && !strchr(name, '\n') && strlen(name) < VL_MAX_NAME_LENGTH &&
	value && !strchr(value, '\n') && strlen(value) < VL_MAX_VALUE_LENGTH) {
		char *beg;
		char *end;
		int num;
		int len;

		num = vl_get_var(vl, name, NULL);
		beg = vl_get_line(vl, num);

		if (!beg) {
			int count = vl_count(vl);

			if (!count) {
				strcpy(vl, name);
				strcat(vl, ":");
				strcat(vl, value);
				strcat(vl, "\n");
				return 0;
			}
			else {
				strcat(vl, name);
				strcat(vl, ":");
				strcat(vl, value);
				strcat(vl, "\n");
				return count;
			}
		}

		beg = &beg[strlen(name) + strlen(":")];
		for (end = beg; '\n' != *end; end++);

		len = strlen(value);
		bcopy(end, &end[len - (int) (end - beg)], strlen(end) + 1);
		bcopy(value, beg, len);

		return num;
	}

	return -1;
}

int vl_set_vl(char *dest, char *src)
{
	if (dest && src) {
		char name[VL_MAX_NAME_LENGTH];
		char value[VL_MAX_VALUE_LENGTH];
		int num;
		int count = vl_count(src);

		for (num = 0; num < count; num++) {
			vl_get(src, num, name, value);
			if (-1 == vl_set_var(dest, name, value)) return -1;
		}

		return 0;
	}

	return -1;
}

int vl_unset_var(char *vl, const char *name)
{
	if (vl && name) {
		int num;
		char *beg;
		char *end;

		num = vl_get_var(vl, name, NULL);
		if (!(beg = vl_get_line(vl, num))) return -1;

		for (end = beg; '\n' != *end++;);
		bcopy(end, beg, strlen(end) + 1);

		return num;
	}

	return -1;
}
/*******************************************************/
static char ETS_results [128]={0,0,0,0,0,0,0,0};

void check_cku_results (void)
{ char *pv = VL_SAVE_ADDR;
  #if OS_VERSION > 300
    pv = (char *)((0xffffff & (unsigned)pv) | 0xc8000000);
  #endif
//   MODCALL(copyfrom(0, (const void *)VL_BASE_ADDR, (void *)SV_ADDR, 0x8000)); 
    if (vl_count(pv))
      { if (vl_get_var(pv, "cku_results", ETS_results) == (-1))
          { strncpy (ETS_results, "no_cku_!", 8); ETS_results [8] = 0;
	  }
      }
    else
      { 
	strncpy (ETS_results, "no_vars_!", 9); ETS_results [9] = 0;
      }
}


