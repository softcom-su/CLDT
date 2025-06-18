#ifndef LANGPACK_H
#define LANGPACK_H


/* элемент таблицы словаря, содержит только указатель на сообщение */
struct lngTableEntry
{
	char *msg;
};
/* таблица словаря */
struct lngTable
{
	char *language;
	char *encoding;
	char *extensions;
	struct lngTableEntry *entries;
};

/* элемент списка таблиц */
struct lngTableListEntry
{
	struct lngTable *table;
	struct lngTableListEntry *next;
};

/* описатель словаря */
struct lngDesc
{
	struct lngTable *tables;              /* базовые языковые таблицы      */
	int idsCount;                         /* количество сообщений в таблице */
};

int lngInternalGetMessage(struct lngDesc *desc, int id, char **msg);

#endif
