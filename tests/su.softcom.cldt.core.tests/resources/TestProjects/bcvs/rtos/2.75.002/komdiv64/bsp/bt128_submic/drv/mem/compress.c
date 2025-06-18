/* compress.c - NVRAM compress routines */

#include <system.h>
#include <board.h>

/* compress table */

static char symbol[28][4] = {
    {'\0', '\0', '\0', '\0'},
    {'a',  'A',  '1',  '?'},
    {'b',  'B',  '2',  '>'},
    {'c',  'C',  '3',  '<'},
    {'d',  'D',  '4',  '|'},
    {'e',  'E',  '5',  '}'},
    {'f',  'F',  '6',  '{'},
    {'g',  'G',  '7',  ']'},
    {'h',  'H',  '8',  '['},
    {'i',  'I',  '9',  ';'},
    {'j',  'J',  '0',  ':'},
    {'k',  'K',  '~',  '\"'}, 	//" mc bug fix
    {'l',  'L',  '!',  '\''},
    {'m',  'M',  '@',  '\\'},
    {'n',  'N',  '#',  ','},
    {'o',  'O',  '$',  '0'},
    {'p',  'P',  '%',  '1'},
    {'q',  'Q',  '^',  '2'},
    {'r',  'R',  '&',  '3'},
    {'s',  'S',  '*',  '='},
    {'t',  'T',  '(',  ' '},
    {'u',  'U',  ')',  ' '},
    {'v',  'V',  '_',  ' '},
    {'w',  'W',  '+',  ' '},
    {'x',  'X',  '.',  ' '},
    {'y',  'Y',  '-',  ' '},
    {'z',  'Z',  '=',  ' '},
    {'/',  '/',  ' ',  ' '}
};

/******************************************************************************/

/* compress line and return length of compressed image */

int nv_compress(unsigned short str[], char string[])
{
    char u[BOOT_LINE_SIZE+2]; int i, j, k, alf, a, l, d, c, p;

    for(alf = 0,i = k = 0, l=strlen(string); i < l; i++)
    {
	for(a = alf, p = 0; p < 4; a = (a+1) % 4, p++)
	{
    	    for(j = 1; j < 28; j++)
            {
		if(string[i] == symbol[j][a])
		{
            	    if(a != alf)
                    {
                	u[k++] = (alf = a) + 28;

                        if(k == BOOT_LINE_SIZE)
			    return -1;
	            }

		    u[k++] = j;

                    if(k == BOOT_LINE_SIZE)
			return -2;

		    goto ok ;
		}
	    }
	}

	return -3;
ok: continue;
    }

    u[k++] = '\0' ;

    for(d = p = i = l = 0; (l*16) < (k*5);)
    {
	while(p < 28)
        {
	    d |= ((c = u[i++]) & 0xff) << p; p += 5;
        }
        str[l++]  = d & 0xffff;
        d = (d >> 16) & 0xffff;
        p -= 16;
    }

    return l*2;
}

/******************************************************************************/

/* uncompress boot line and return it's original length */

int nv_uncompress(unsigned short str[], char string[])
{
    char u[BOOT_LINE_SIZE+3]; int p, d, c, l, i, k, alf;
	
    p = d = c = i = l = 0 ;

    do
    {
	if(p < 17)
        {
	    d |= ((c = str[i++]) & 0xffff) << p;
            p += 16;
        }

	while(p > 7)
	{
	    u[l++] = d & 0x1f; d >>= 5 ; p -= 5 ;
	}
    } while(u[l-1] != 0 && l < BOOT_LINE_SIZE);

    for(alf = 0, k = i = 0; k < l; k++)
    {
	if(u[k] == 0)
        {
	    string[i] = 0; break;
	}
        else if(u[k] > 27)
	{
    	    alf = u[k] - 28;
	}
        else
	{
    	    string[i++] = symbol[(int)u[k]][alf];
	}

        if(i >= BOOT_LINE_SIZE)
    	    return -1;
    }

    return i;
}

