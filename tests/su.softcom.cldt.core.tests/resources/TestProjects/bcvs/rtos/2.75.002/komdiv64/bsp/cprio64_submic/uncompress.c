/* 
 * Compress - data uncompression program 
 */

#if defined(CPRIO64) || defined(BT128)
  #define PMON
#endif

#define BITS             16
#define HSIZE            69001                /* 95% occupancy */
#define BIT_MASK         0x1f
#define BLOCK_MASK       0x80
#define INIT_BITS        9                    /* initial number of bits/code */
#define MAXCODE(n_bits)  (((code_int) 1 << (n_bits)) - 1)
#define tab_prefixof(i)  codetab[i]
#define tab_suffixof(i)  ((char_type *)(htab))[i]
#define de_stack         ((char_type *)&tab_suffixof((code_int)1<<BITS))
#define FIRST            257        /* first free entry */
#define CLEAR            256        /* table clear output code */

extern char        edata[];                /* automatically defined by loader */
extern char        end[];                  /* automatically defined by loader */
extern char        _ftext[];	           /* automatically defined by loader */
extern char        etext[];		   /* automatically defined by loader */

typedef long int          code_int;
typedef int               (*FUNCPTR)();
typedef long int          count_int;
typedef unsigned char     char_type;
char_type magic_header[] = { "\037\235" };        /* 1F 9D */
unsigned char *masin;
unsigned char *masout =(unsigned char *)RAM_LOW_ADRS;
unsigned char *pmasout = (unsigned char *)RAM_LOW_ADRS;
int inbuf;
int cinbuf;
int n_bits;                                /* number of bits/code */
int maxbits = BITS;                        /* user settable max # bits/code */
code_int maxcode;                        /* maximum code, given n_bits */
code_int maxmaxcode = (code_int)1 << BITS; /* should NEVER generate this code */


count_int htab [HSIZE];
unsigned short codetab [HSIZE];

code_int hsize = HSIZE;                        /* for dynamic table sizing */
code_int free_ent = 0;                        /* first unused entry */
static code_int getcode();
static void decompress();
FUNCPTR startRAM;     
int block_compress = BLOCK_MASK;
int clear_flg = 0;

#ifdef	INCLUDE_COPY_COMP_IMG
void copy128(unsigned* src, unsigned* dest, unsigned bcount);
#endif	/* INCLUDE_COPY_COMP_IMG */
void fill128(unsigned* adr, unsigned bcount, unsigned value);

#ifndef PMON
void uncompress(int startType, char* rom_text_adrs)
#else
void uncompress(int startType, char* rom_text_adrs, void * a2)
#endif
{ int i;
  char x = 0;
  char *dh;
  
  fill128((unsigned*)edata,(unsigned)(end-edata),0);
  
  dh = (char *)(rom_text_adrs + (unsigned int) edata - (unsigned int)RAM_COMP_ADRS);
  
  for(i = 0, inbuf = 0; i < 8; i++)
  {
    if((dh[i] >= '0') && (dh[i] <= '9')) x = dh[i] - '0';
    if((dh[i] >= 'a') && (dh[i] <= 'f')) x = 10 + dh[i] - 'a';
    if((dh[i] >= 'A') && (dh[i] <= 'F')) x = 10 + dh[i] - 'A';
    inbuf += (x & 0xf) << (4 * (7 - i)); /* max number byte of input stream */
  }

#ifdef	INCLUDE_COPY_COMP_IMG
  copy128(dh+8,(unsigned*)RAM_COMP_IMG_ADRS,inbuf);
#endif	/* INCLUDE_COPY_COMP_IMG */
  cinbuf = 0;
  
  startRAM = (FUNCPTR)RAM_LOW_ADRS;
#ifdef	INCLUDE_COPY_COMP_IMG
  masin = (char*)(RAM_COMP_IMG_ADRS);
#else	/* INCLUDE_COPY_COMP_IMG */
  masin =(char*)(dh+8); 
#endif	/* INCLUDE_COPY_COMP_IMG */
  if ((masin[cinbuf++] != (magic_header[0] & 0xFF)) || (masin[cinbuf++] != (magic_header[1] & 0xFF))) 
      return;
  maxbits = masin[cinbuf++];       
  block_compress = maxbits & BLOCK_MASK;
  maxbits &= BIT_MASK;
  maxmaxcode = (code_int) 1 << maxbits;
  decompress();
#ifndef PMON
  startRAM(startType, rom_text_adrs);	/* Start into RAM */ 
#else
  *(unsigned *)0xa0000610 = a2;
  startRAM(startType, rom_text_adrs, a2);	/* Start into RAM */ 
#endif
 
}

long int in_count = 1;                        /* length of input */
long int bytes_out;                        /* length of compressed output */
long int out_count = 0;                        /* # of codes output (for debugging) */

char_type lmask[9] = {0xff, 0xfe, 0xfc, 0xf8, 0xf0, 0xe0, 0xc0, 0x80, 0x00};
char_type rmask[9] = {0x00, 0x01, 0x03, 0x07, 0x0f, 0x1f, 0x3f, 0x7f, 0xff};

void decompress() {

    register char_type *stackp;
    register int finchar;
    register code_int code, oldcode, incode;

    maxcode = MAXCODE(n_bits = INIT_BITS);
    for ( code = 255; code >= 0; code-- ) {
        tab_prefixof(code) = 0;
        tab_suffixof(code) = (char_type)code;
    }
    free_ent = ((block_compress) ? FIRST : 256 );

    finchar = oldcode = getcode();
    if(oldcode == -1)        /* EOF already? */
        return;                        /* Get out of here */
    *pmasout=(char)finchar;
     pmasout++; 
    stackp = de_stack;

    while ( (code = getcode()) > -1 ) {

        if ( (code == CLEAR) && block_compress ) {
            for ( code = 255; code >= 0; code-- )
                tab_prefixof(code) = 0;
            clear_flg = 1;
            free_ent = FIRST - 1;
            if ( (code = getcode ()) == -1 )        /* O, untimely death! */
                break;
        }
        incode = code;

        if ( code >= free_ent ) {
            *stackp++ = finchar;
            code = oldcode;
        }

        while ( code >= 256 ) {
            *stackp++ = tab_suffixof(code);
            code = tab_prefixof(code);
        }
        *stackp++ = finchar = tab_suffixof(code);

        do{
          *pmasout = *--stackp;
           pmasout++;
          }
        while ( stackp > de_stack );

        if ( (code=free_ent) < maxmaxcode ) {
            tab_prefixof(code) = (unsigned short)oldcode;
            tab_suffixof(code) = finchar;
            free_ent = code+1;
        } 
        oldcode = incode;
    }
}

code_int getcode() {
    register code_int code;
    int i;
    static int offset = 0, size = 0;
    static char_type buf[BITS];
    register int r_off, bits;
    register char_type *bp=buf;
    
    if ( clear_flg > 0 || offset >= size || free_ent > maxcode ) {

        if ( free_ent > maxcode ) {
            n_bits++;
            if ( n_bits == maxbits )
                maxcode = maxmaxcode;        /* won't get any bigger now */
            else
                maxcode = MAXCODE(n_bits);
        }
        if ( clear_flg > 0) {
            maxcode = MAXCODE (n_bits = INIT_BITS);
            clear_flg = 0;
        }
        if((n_bits + cinbuf) <= inbuf)
        {
          size = n_bits;
          for(i = 0; i < n_bits; i++)
           buf[i] = masin[cinbuf++];
         } else { size = inbuf - cinbuf; 
                  for(i = 0;i < size; i++)
                    buf[i]=masin[cinbuf++];
                 }
        if ( size <= 0 )
            return -1;                        /* end of file */
        offset = 0;
        size = (size << 3) - (n_bits - 1);
    }
    r_off = offset;
    bits = n_bits;
        bp += (r_off >> 3);
        r_off &= 7;
        code = (*bp++ >> r_off);
        bits -= (8 - r_off);
        r_off = 8 - r_off;                /* now, offset into code word */
        if ( bits >= 8 ) {
            code |= *bp++ << r_off;
            r_off += 8;
            bits -= 8;
        }
        code |= (*bp & rmask[bits]) << r_off;
        offset += n_bits;
    
    return code;
}
