#ifndef _PRIVATE_ALIAS_TABLE_H
#define _PRIVATE_ALIAS_TABLE_H
#include <sys/queue.h>
struct alias {
    char *shell_name;
    char *library_name;
    int   argcargv;
};
struct alias_table {
    SLIST_ENTRY(alias_table) next;
    struct alias *a;
};
const char *shellCheckAlias(const char *name, int *argc);
const char *shellAlias2Name(const char *name);
const char *shellName2Alias(const char *alias);
void shellDeleteAliasTable(struct alias_table *a);
void shellAddAliasTable(struct alias_table *a);
void shellSetInput(void *v);
const char *shellCommandGenerator (const char *text, int state );
#endif
