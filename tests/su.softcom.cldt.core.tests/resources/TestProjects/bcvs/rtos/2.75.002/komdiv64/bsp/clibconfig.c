/*clibconfig.c*/
/*
 * конфигурирование библиотек C -
 * включение в образ указанных стандартных библиотек
 */
#ifdef   INCLUDE_CTYPE 
static vfuncptr_t __UNUSED_F ctypeFunctions[] = {
    (vfuncptr_t) tolower,
    (vfuncptr_t) toupper
  };
#endif

#ifdef   INCLUDE_LOCALE
static vfuncptr_t __UNUSED_F localeFunctions[] = {
    (vfuncptr_t) setlocale,
    (vfuncptr_t) localeconv
};
#endif

#ifdef   INCLUDE_MATH
void __math_all();
static vfuncptr_t __UNUSED_F mathFunctions = __math_all;
#endif

#ifdef   INCLUDE_STDIO
#undef feof
#undef ferror
int feof(FILE *);
int ferror(FILE *);
static vfuncptr_t __UNUSED_F stdioFunctions[] = {
/*    (vfuncptr_t) clearerr, //macro	*/
    (vfuncptr_t) fclose,
    (vfuncptr_t) feof,
    (vfuncptr_t) ferror,
    (vfuncptr_t) fflush,
    (vfuncptr_t) fgetc,
    (vfuncptr_t) fgetpos,
    (vfuncptr_t) fgets,
    (vfuncptr_t) fopen,
    (vfuncptr_t) fprintf,
    (vfuncptr_t) fputc,
    (vfuncptr_t) fputs,
    (vfuncptr_t) fread,
    (vfuncptr_t) freopen,
    (vfuncptr_t) fscanf,
    (vfuncptr_t) fseek,
    (vfuncptr_t) fsetpos,
    (vfuncptr_t) ftell,
    (vfuncptr_t) fwrite,
    (vfuncptr_t) getc,
/*    (vfuncptr_t) getchar, ->getc */	
    (vfuncptr_t) gets,
    (vfuncptr_t) perror,
    (vfuncptr_t) putc,
/*    (vfuncptr_t) putchar,  ->putc */
    (vfuncptr_t) puts,
    (vfuncptr_t) rename, 
    (vfuncptr_t) remove, 
    (vfuncptr_t) rewind,
    (vfuncptr_t) scanf,
    (vfuncptr_t) setbuf,
    (vfuncptr_t) setvbuf,
    (vfuncptr_t) sprintf,
    (vfuncptr_t) sscanf,
    (vfuncptr_t) tmpfile,
    (vfuncptr_t) tmpnam,
    (vfuncptr_t) ungetc,
    (vfuncptr_t) vfprintf,
    (vfuncptr_t) vprintf,
    (vfuncptr_t) vsprintf,
    (vfuncptr_t) fdopen,
    (vfuncptr_t) fileno
  };
#endif

#ifdef   INCLUDE_STDLIB
static vfuncptr_t __UNUSED_F stdlibFunctions[] = {
    (vfuncptr_t) abort,
    (vfuncptr_t) abs,
/*    (vfuncptr_t) atexit,	*/
    (vfuncptr_t) atof,
    (vfuncptr_t) atoi,
    (vfuncptr_t) atol,
    (vfuncptr_t) calloc,
    (vfuncptr_t) div,
    (vfuncptr_t) exit,
    (vfuncptr_t) free,
    (vfuncptr_t) getenv,
    (vfuncptr_t) labs,
    (vfuncptr_t) ldiv,
    (vfuncptr_t) malloc,
    (vfuncptr_t) qsort,
    (vfuncptr_t) rand,
    (vfuncptr_t) realloc,
    (vfuncptr_t) strtod,
    (vfuncptr_t) strtol,
    (vfuncptr_t) strtoul,
    (vfuncptr_t) system
  };
#endif

#ifdef   INCLUDE_STRING
static vfuncptr_t __UNUSED_F stringFunctions[] = {
    (vfuncptr_t) memchr,
    (vfuncptr_t) memcmp,
    (vfuncptr_t) memcpy,
    (vfuncptr_t) memset,
    (vfuncptr_t) memmove,
    (vfuncptr_t) strcat,
    (vfuncptr_t) strchr,
    (vfuncptr_t) strcmp,
/*    (vfuncptr_t) strcoll, */
    (vfuncptr_t) strcpy,
    (vfuncptr_t) strcspn,
    (vfuncptr_t) strerror,
    (vfuncptr_t) strlen,
    (vfuncptr_t) strncat,
    (vfuncptr_t) strncmp,
    (vfuncptr_t) strncpy,
    (vfuncptr_t) strpbrk,
    (vfuncptr_t) strrchr,
    (vfuncptr_t) strspn,
    (vfuncptr_t) strstr,
    (vfuncptr_t) strtok
/*    (vfuncptr_t) strtok_r, */
/*    (vfuncptr_t) strxfrm */
  };
#endif

#ifdef   INCLUDE_TIME
static vfuncptr_t __UNUSED_F timeFunctions[] = {
    (vfuncptr_t) asctime,
    (vfuncptr_t) ctime,
    (vfuncptr_t) difftime,
    (vfuncptr_t) gmtime,
    (vfuncptr_t) localtime,
    (vfuncptr_t) mktime,
    (vfuncptr_t) strftime,
    (vfuncptr_t) time
  };
#endif
