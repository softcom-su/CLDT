#ifndef _CTYPELOCALE_H
#define _CTYPELOCALE_H


#if 0
/*1-ая половина повторяет "C"*/
static unsigned char ctypeLocale[256] = {
_C,_C,_C,_C,_C,_C,_C,_C,		    	        /* 0-7 */
_C,_C|_S,_C|_S,_C|_S,_C|_S,_C|_S,_C,_C,		/* 8-15 */
_C,_C,_C,_C,_C,_C,_C,_C,			            /* 16-23 */
_C,_C,_C,_C,_C,_C,_C,_C,			            /* 24-31 */
_S|_SP,_P,_P,_P,_P,_P,_P,_P,			        /* 32-39 */
_P,_P,_P,_P,_P,_P,_P,_P,			            /* 40-47 */
_D,_D,_D,_D,_D,_D,_D,_D,			            /* 48-55 */
_D,_D,_P,_P,_P,_P,_P,_P,			            /* 56-63 */
_P,_U|_X,_U|_X,_U|_X,_U|_X,_U|_X,_U|_X,_U,/* 64-71 */
_U,_U,_U,_U,_U,_U,_U,_U,			            /* 72-79 */
_U,_U,_U,_U,_U,_U,_U,_U,			            /* 80-87 */
_U,_U,_U,_P,_P,_P,_P,_P,			            /* 88-95 */
_P,_L|_X,_L|_X,_L|_X,_L|_X,_L|_X,_L|_X,_L,/* 96-103 */
_L,_L,_L,_L,_L,_L,_L,_L,			            /* 104-111 */
_L,_L,_L,_L,_L,_L,_L,_L,			            /* 112-119 */
_L,_L,_L,_P,_P,_P,_P,_C,			            /* 120-127 */
#endif

unsigned char ctypeLocale[128] = {
#if LANG_CP == LANG_CP_KOI8
  /* 0x80 128 */        0, /* box drawings light horizontal, sometimes used as dash */
  /* 0x81 129 */        0, /* box drawings light vertical */
  /* 0x82 130 */        0, /* box drawings light down and right */
  /* 0x83 131 */        0, /* box drawings light down and left */
  /* 0x84 132 */        0, /* box drawings light up and right */
  /* 0x85 133 */        0, /* box drawings light up and left */
  /* 0x86 134 */        0, /* box drawings light vertical and right */
  /* 0x87 135 */        0, /* box drawings light vertical and left */
  /* 0x88 136 */        0, /* box drawings light down and horizontal */
  /* 0x89 137 */        0, /* box drawings light up and horizontal */
  /* 0x8A 138 */        0, /* box drawings light vertical and horizontal */
  /* 0x8B 139 */        0, /* upper half block */
  /* 0x8C 140 */        0, /* lower half block */
  /* 0x8D 141 */        0, /* full block */
  /* 0x8E 142 */        0, /* left half block */
  /* 0x8F 143 */        0, /* right half block */
  /* 0x90 144 */        0, /* light shade */
  /* 0x91 145 */        0, /* medium shade */
  /* 0x92 146 */        0, /* dark shade */
  /* 0x93 147 */        0, /* top half integral */
  /* 0x94 148 */        0, /* black square, win - number sign */
  /* 0x95 149 */        0, /* bullet operator */
  /* 0x96 150 */        0, /* square root */
  /* 0x97 151 */        0, /* almost equal to */
  /* 0x98 152 */        0, /* less-than or equal to / х */
  /* 0x99 153 */        0, /* greater-than or equal to */
  /* 0x9A 154 */        0, /* no-break space */
  /* 0x9B 155 */        0, /* bottom half integral */
  /* 0x9C 156 */        0, /* degree sign */
  /* 0x9D 157 */        0, /* turtle (sun) sign / superscript two */
  /* 0x9E 158 */        0, /* middle dot */
  /* 0x9F 159 */        0, /* division sign */
  /* 0xA0 160 */        0, /* box drawings double horizontal */
  /* 0xA1 161 */        0, /* box drawings double vertical */
  /* 0xA2 162 */        0, /* box drawings down single and right double */
  /* 0xA3 163 */ _L, /* small letter yoe - koi8r, win: plus-minus sign - alt */
  /* 0xA4 164 */        0, /* box drawings down double and right single */
  /* 0xA5 165 */        0, /* box drawings double down and right */
  /* 0xA6 166 */        0, /* box drawings down single and left double */
  /* 0xA7 167 */        0, /* box drawings down double and left single */
  /* 0xA8 168 */        0, /* box drawings double down and left */
  /* 0xA9 169 */        0, /* box drawings up single and right double */
  /* 0xAA 170 */        0, /* box drawings up double and right single */
  /* 0xAB 171 */        0, /* box drawings double up and right */
  /* 0xAC 172 */        0, /* box drawings up single and left double */
  /* 0xAD 173 */        0, /* box drawings up double and left single */
  /* 0xAE 174 */        0, /* box drawings double up and left */
  /* 0xAF 175 */        0, /* box drawings vertical single and right double */
  /* 0xB0 176 */        0, /* box drawings vertical double and right single */
  /* 0xB1 177 */        0, /* box drawings double vertical and right */
  /* 0xB2 178 */        0, /* box drawings vertical single and left double */
  /* 0xB3 179 */ _U, /* capital letter yoe - koi8r, win: identity sign - alt */
  /* 0xB4 180 */        0, /* box drawings vertical double and left single */
  /* 0xB5 181 */        0, /* box drawings double vertical and left */
  /* 0xB6 182 */        0, /* box drawings down single and horizontal double */
  /* 0xB7 183 */        0, /* box drawings down double and horizontal single */
  /* 0xB8 184 */        0, /* box drawings double down and horizontal */
  /* 0xB9 185 */        0, /* box drawings up single and horizontal double */
  /* 0xBA 186 */        0, /* box drawings up double and horizontal single */
  /* 0xBB 187 */        0, /* box drawings double up and horizontal */
  /* 0xBC 188 */        0, /* box drawings vertical single and horizontal double */
  /* 0xBD 189 */        0, /* box drawings vertical double and horizontal single */
  /* 0xBE 190 */        0, /* box drawings double vertical and horizontal */
  /* 0xBF 191 */        0, /* copyright sign /number sign */
  /* 0xC0 192 */ _L, /* small letter yu */
  /* 0xC1 193 */ _L, /* small letter a */
  /* 0xC2 194 */ _L, /* small letter be */
  /* 0xC3 195 */ _L, /* small letter tse */
  /* 0xC4 196 */ _L, /* small letter de */
  /* 0xC5 197 */ _L, /* small letter ie */
  /* 0xC6 198 */ _L, /* small letter ef */
  /* 0xC7 199 */ _L, /* small letter ghe */
  /* 0xC8 200 */ _L, /* small letter kha */
  /* 0xC9 201 */ _L, /* small letter i */
  /* 0xCA 202 */ _L, /* small letter shorti */
  /* 0xCB 203 */ _L, /* small letter ka */
  /* 0xCC 204 */ _L, /* small letter el */
  /* 0xCD 205 */ _L, /* small letter em */
  /* 0xCE 206 */ _L, /* small letter en */
  /* 0xCF 207 */ _L, /* small letter o */
  /* 0xD0 208 */ _L, /* small letter pe */
  /* 0xD1 209 */ _L, /* small letter ya */
  /* 0xD2 210 */ _L, /* small letter er */
  /* 0xD3 211 */ _L, /* small letter es */
  /* 0xD4 212 */ _L, /* small letter te */
  /* 0xD5 213 */ _L, /* small letter u */
  /* 0xD6 214 */ _L, /* small letter zhe */
  /* 0xD7 215 */ _L, /* small letter ve */
  /* 0xD8 216 */ _L, /* small letter softsign(ierik) */
  /* 0xD9 217 */ _L, /* small letter y(iery) */
  /* 0xDA 218 */ _L, /* small letter z */
  /* 0xDB 219 */ _L, /* small letter sha */
  /* 0xDC 220 */ _L, /* small letter reverse rounded e */
  /* 0xDD 221 */ _L, /* small lettershcha */
  /* 0xDE 222 */ _L, /* small letter che */
  /* 0xDF 223 */ _L, /* small letter hardsign(ier) */
  /* 0xE0 224 */ _U, /* capital letter yu */
  /* 0xE1 225 */ _U, /* capital letter a */
  /* 0xE2 226 */ _U, /* capital letter be */
  /* 0xE3 227 */ _U, /* capital letter tse */
  /* 0xE4 228 */ _U, /* capital letter de */
  /* 0xE5 229 */ _U, /* capital letter ie */
  /* 0xE6 230 */ _U, /* capital letter ef */
  /* 0xE7 231 */ _U, /* capital letter ghe */
  /* 0xE8 232 */ _U, /* capital letter kha */
  /* 0xE9 233 */ _U, /* capital letter i */
  /* 0xEA 234 */ _U, /* capital letter shorti */
  /* 0xEB 235 */ _U, /* capital letter ka */
  /* 0xEC 236 */ _U, /* capital letter el */
  /* 0xED 237 */ _U, /* capital letter em */
  /* 0xEE 238 */ _U, /* capital letter en */
  /* 0xEF 239 */ _U, /* capital letter o */
  /* 0xF0 240 */ _U, /* capital letter pe */
  /* 0xF1 241 */ _U, /* capital letter ya */
  /* 0xF2 242 */ _U, /* capital letter er */
  /* 0xF3 243 */ _U, /* capital letter es */
  /* 0xF4 244 */ _U, /* capital letter te */
  /* 0xF5 245 */ _U, /* capital letter u */
  /* 0xF6 246 */ _U, /* capital letter zhe */
  /* 0xF7 247 */ _U, /* capital letter ve */
  /* 0xF8 248 */ _U, /* capital letter softsign(loshad') */
  /* 0xF9 249 */ _U, /* capital letter y(as in ryba) */
  /* 0xFA 250 */ _U, /* capital letter ze */
  /* 0xFB 251 */ _U, /* capital letter sha */
  /* 0xFC 252 */ _U, /* capital letter reverse rounded e, as in <eto> */
  /* 0xFD 253 */ _U, /* capital letter shcha */
  /* 0xFE 254 */ _U, /* capital letter che */
  /* 0xFF 255 */ _U  /* capital letter hardsign(as in pod'ezd) */
#endif

#if  LANG_CP == LANG_CP_WIN
  /* 0x80 128 */        0, /*  */
  /* 0x81 129 */        0, /*  */
  /* 0x82 130 */        0, /*  */
  /* 0x83 131 */        0, /*  */
  /* 0x84 132 */        0, /*  */
  /* 0x85 133 */        0, /*  */
  /* 0x86 134 */        0, /*  */
  /* 0x87 135 */        0, /*  */
  /* 0x88 136 */        0, /*  */
  /* 0x89 137 */        0, /*  */
  /* 0x8A 138 */        0, /*  */
  /* 0x8B 139 */        0, /*  */
  /* 0x8C 140 */        0, /*  */
  /* 0x8D 141 */        0, /*  */
  /* 0x8E 142 */        0, /*  */
  /* 0x8F 143 */        0, /*  */
  /* 0x90 144 */        0, /*  */
  /* 0x91 145 */        0, /* left apostrophe */
  /* 0x92 146 */        0, /* right apostrophe */
  /* 0x93 147 */        0, /* left quote */
  /* 0x94 148 */        0, /* right quote */
  /* 0x95 149 */        0, /* bullet operator */
  /* 0x96 150 */        0, /* short horizontal */
  /* 0x97 151 */        0, /* box drawings light horizontal, sometimes used as dash */
  /* 0x98 152 */        0, /* empty box */
  /* 0x99 153 */        0, /* trade mark */
  /* 0x9A 154 */        0, /*  */
  /* 0x9B 155 */        0, /*  */
  /* 0x9C 156 */        0, /*  */
  /* 0x9D 157 */        0, /*  */
  /* 0x9E 158 */        0, /*  */
  /* 0x9F 159 */        0, /*  */
  /* 0xA0 160 */        0, /* no-break space */
  /* 0xA1 161 */        0, /*  */
  /* 0xA2 162 */        0, /*  */
  /* 0xA3 163 */        0, /*  */
  /* 0xA4 164 */        0, /* turtle (sun) sign / superscript two */
  /* 0xA5 165 */        0, /*  */
  /* 0xA6 166 */        0, /*  */
  /* 0xA7 167 */        0, /*  */
  /* 0xA8 168 */ _U, /* capital letter yoe - koi8r, win: identity sign - alt */
  /* 0xA9 169 */        0, /* copyright sign /number sign */
  /* 0xAA 170 */        0, /*  */
  /* 0xAB 171 */        0, /*  */
  /* 0xAC 172 */        0, /*  */
  /* 0xAD 173 */        0, /*  */
  /* 0xAE 174 */        0, /*  */
  /* 0xAF 175 */        0, /*  */
  /* 0xB0 176 */        0, /* degree sign */
  /* 0xB1 177 */        0, /*  */
  /* 0xB2 178 */        0, /*  */
  /* 0xB3 179 */        0, /*  */
  /* 0xB4 180 */        0, /*  */
  /* 0xB5 181 */        0, /*  */
  /* 0xB6 182 */        0, /*  */
  /* 0xB7 183 */        0, /* middle dot */
  /* 0xB8 184 */ _L, /* small letter yoe - koi8r, win: plus-minus sign - alt */
  /* 0xB9 185 */        0, /* black square, win - number sign */
  /* 0xBA 186 */        0, /*  */
  /* 0xBB 187 */        0, /*  */
  /* 0xBC 188 */        0, /*  */
  /* 0xBD 189 */        0, /*  */
  /* 0xBE 190 */        0, /*  */
  /* 0xBF 191 */        0, /* less-than or equal to / х */
  /* 0xC0 192 */ _U, /* capital letter a */
  /* 0xC1 193 */ _U, /* capital letter be */
  /* 0xC2 194 */ _U, /* capital letter ve */
  /* 0xC3 195 */ _U, /* capital letter ghe */
  /* 0xC4 196 */ _U, /* capital letter de */
  /* 0xC5 197 */ _U, /* capital letter ie */
  /* 0xC6 198 */ _U, /* capital letter zhe */
  /* 0xC7 199 */ _U, /* capital letter ze */
  /* 0xC8 200 */ _U, /* capital letter i */
  /* 0xC9 201 */ _U, /* capital letter shorti */
  /* 0xCA 202 */ _U, /* capital letter ka */
  /* 0xCB 203 */ _U, /* capital letter el */
  /* 0xCC 204 */ _U, /* capital letter em */
  /* 0xCD 205 */ _U, /* capital letter en */
  /* 0xCE 206 */ _U, /* capital letter o */
  /* 0xCF 207 */ _U, /* capital letter pe */
  /* 0xD0 208 */ _U, /* capital letter er */
  /* 0xD1 209 */ _U, /* capital letter es */
  /* 0xD2 210 */ _U, /* capital letter te */
  /* 0xD3 211 */ _U, /* capital letter u */
  /* 0xD4 212 */ _U, /* capital letter ef */
  /* 0xD5 213 */ _U, /* capital letter kha */
  /* 0xD6 214 */ _U, /* capital letter tse */
  /* 0xD7 215 */ _U, /* capital letter che */
  /* 0xD8 216 */ _U, /* capital letter sha */
  /* 0xD9 217 */ _U, /* capital letter shcha */
  /* 0xDA 218 */ _U, /* capital letter hardsign(as in pod'ezd) */
  /* 0xDB 219 */ _U, /* capital letter y(as in ryba) */
  /* 0xDC 220 */ _U, /* capital letter softsign(loshad') */
  /* 0xDD 221 */ _U, /* capital letter reverse rounded e, as in <eto> */
  /* 0xDE 222 */ _U, /* capital letter yu */
  /* 0xDF 223 */ _U, /* capital letter ya */
  /* 0xE0 224 */ _L, /* small letter a */
  /* 0xE1 225 */ _L, /* small letter be */
  /* 0xE2 226 */ _L, /* small letter ve */
  /* 0xE3 227 */ _L, /* small letter ghe */
  /* 0xE4 228 */ _L, /* small letter de */
  /* 0xE5 229 */ _L, /* small letter ie */
  /* 0xE6 230 */ _L, /* small letter zhe */
  /* 0xE7 231 */ _L, /* small letter z */
  /* 0xE8 232 */ _L, /* small letter i */
  /* 0xE9 233 */ _L, /* small letter shorti */
  /* 0xEA 234 */ _L, /* small letter ka */
  /* 0xEB 235 */ _L, /* small letter el */
  /* 0xEC 236 */ _L, /* small letter em */
  /* 0xED 237 */ _L, /* small letter en */
  /* 0xEE 238 */ _L, /* small letter o */
  /* 0xEF 239 */ _L, /* small letter pe */
  /* 0xF0 240 */ _L, /* small letter er */
  /* 0xF1 241 */ _L, /* small letter es */
  /* 0xF2 242 */ _L, /* small letter te */
  /* 0xF3 243 */ _L, /* small letter u */
  /* 0xF4 244 */ _L, /* small letter ef */
  /* 0xF5 245 */ _L, /* small letter kha */
  /* 0xF6 246 */ _L, /* small letter tse */
  /* 0xF7 247 */ _L, /* small letter che */
  /* 0xF8 248 */ _L, /* small letter sha */
  /* 0xF9 249 */ _L, /* small lettershcha */
  /* 0xFA 250 */ _L, /* small letter hardsign(ier) */
  /* 0xFB 251 */ _L, /* small letter y(iery) */
  /* 0xFC 252 */ _L, /* small letter softsign(ierik) */
  /* 0xFD 253 */ _L, /* small letter reverse rounded e */
  /* 0xFE 254 */ _L, /* small letter yu */
  /* 0xFF 255 */ _L  /* small letter ya */
#endif

#if LANG_CP == LANG_CP_ALT
  /* 0x80 128 */ _U, /* capital letter a */
  /* 0x81 129 */ _U, /* capital letter be */
  /* 0x82 130 */ _U, /* capital letter ve */
  /* 0x83 131 */ _U, /* capital letter ghe */
  /* 0x84 132 */ _U, /* capital letter de */
  /* 0x85 133 */ _U, /* capital letter ie */
  /* 0x86 134 */ _U, /* capital letter zhe */
  /* 0x87 135 */ _U, /* capital letter ze */
  /* 0x88 136 */ _U, /* capital letter i */
  /* 0x89 137 */ _U, /* capital letter shorti */
  /* 0x8A 138 */ _U, /* capital letter ka */
  /* 0x8B 139 */ _U, /* capital letter el */
  /* 0x8C 140 */ _U, /* capital letter em */
  /* 0x8D 141 */ _U, /* capital letter en */
  /* 0x8E 142 */ _U, /* capital letter o */
  /* 0x8F 143 */ _U, /* capital letter pe */
  /* 0x90 144 */ _U, /* capital letter er */
  /* 0x91 145 */ _U, /* capital letter es */
  /* 0x92 146 */ _U, /* capital letter te */
  /* 0x93 147 */ _U, /* capital letter u */
  /* 0x94 148 */ _U, /* capital letter ef */
  /* 0x95 149 */ _U, /* capital letter kha */
  /* 0x96 150 */ _U, /* capital letter tse */
  /* 0x97 151 */ _U, /* capital letter che */
  /* 0x98 152 */ _U, /* capital letter sha */
  /* 0x99 153 */ _U, /* capital letter shcha */
  /* 0x9A 154 */ _U, /* capital letter hardsign(as in pod'ezd) */
  /* 0x9B 155 */ _U, /* capital letter y(as in ryba) */
  /* 0x9C 156 */ _U, /* capital letter softsign(loshad') */
  /* 0x9D 157 */ _U, /* capital letter reverse rounded e, as in <eto> */
  /* 0x9E 158 */ _U, /* capital letter yu */
  /* 0x9F 159 */ _U, /* capital letter ya */
  /* 0xA0 160 */ _L, /* small letter a */
  /* 0xA1 161 */ _L, /* small letter be */
  /* 0xA2 162 */ _L, /* small letter ve */
  /* 0xA3 163 */ _L, /* small letter ghe */
  /* 0xA4 164 */ _L, /* small letter de */
  /* 0xA5 165 */ _L, /* small letter ie */
  /* 0xA6 166 */ _L, /* small letter zhe */
  /* 0xA7 167 */ _L, /* small letter z */
  /* 0xA8 168 */ _L, /* small letter i */
  /* 0xA9 169 */ _L, /* small letter shorti */
  /* 0xAA 170 */ _L, /* small letter ka */
  /* 0xAB 171 */ _L, /* small letter el */
  /* 0xAC 172 */ _L, /* small letter em */
  /* 0xAD 173 */ _L, /* small letter en */
  /* 0xAE 174 */ _L, /* small letter o */
  /* 0xAF 175 */ _L, /* small letter pe */
  /* 0xB0 176 */        0, /* light shade */
  /* 0xB1 177 */        0, /* medium shade */
  /* 0xB2 178 */        0, /* dark shade */
  /* 0xB3 179 */        0, /* box drawings light vertical */
  /* 0xB4 180 */        0, /* box drawings light vertical and left */
  /* 0xB5 181 */        0, /* box drawings vertical single and left double */
  /* 0xB6 182 */        0, /* box drawings vertical double and left single */
  /* 0xB7 183 */        0, /* box drawings up double and left single */
  /* 0xB8 184 */        0, /* box drawings up single and left double */
  /* 0xB9 185 */        0, /* box drawings double vertical and left */
  /* 0xBA 186 */        0, /* box drawings double vertical */
  /* 0xBB 187 */        0, /* box drawings double up and left */
  /* 0xBC 188 */        0, /* box drawings double down and left */
  /* 0xBD 189 */        0, /* box drawings down double and left single */
  /* 0xBE 190 */        0, /* box drawings down single and left double */
  /* 0xBF 191 */        0, /* box drawings light up and left */
  /* 0xC0 192 */        0, /* box drawings light down and right */
  /* 0xC1 193 */        0, /* box drawings light up and horizontal */
  /* 0xC2 194 */        0, /* box drawings light down and horizontal */
  /* 0xC3 195 */        0, /* box drawings light vertical and right */
  /* 0xC4 196 */        0, /* box drawings light horizontal, sometimes used as dash */
  /* 0xC5 197 */        0, /* box drawings light vertical and horizontal */
  /* 0xC6 198 */        0, /* box drawings vertical single and right double */
  /* 0xC7 199 */        0, /* box drawings vertical double and right single */
  /* 0xC8 200 */        0, /* box drawings double down and right */
  /* 0xC9 201 */        0, /* box drawings double up and right */
  /* 0xCA 202 */        0, /* box drawings double up and horizontal */
  /* 0xCB 203 */        0, /* box drawings double down and horizontal */
  /* 0xCC 204 */        0, /* box drawings double vertical and right */
  /* 0xCD 205 */        0, /* box drawings double horizontal */
  /* 0xCE 206 */        0, /* box drawings double vertical and horizontal */
  /* 0xCF 207 */        0, /* box drawings up single and horizontal double */
  /* 0xD0 208 */        0, /* box drawings up double and horizontal single */
  /* 0xD1 209 */        0, /* box drawings down single and horizontal double */
  /* 0xD2 210 */        0, /* box drawings down double and horizontal single */
  /* 0xD3 211 */        0, /* box drawings down double and right single */
  /* 0xD4 212 */        0, /* box drawings down single and right double */
  /* 0xD5 213 */        0, /* box drawings up single and right double */
  /* 0xD6 214 */        0, /* box drawings up double and right single */
  /* 0xD7 215 */        0, /* box drawings vertical double and horizontal single */
  /* 0xD8 216 */        0, /* box drawings vertical single and horizontal double */
  /* 0xD9 217 */        0, /* box drawings light down and left */
  /* 0xDA 218 */        0, /* box drawings light up and right */
  /* 0xDB 219 */        0, /* full block */
  /* 0xDC 220 */        0, /* lower half block */
  /* 0xDD 221 */        0, /* left half block */
  /* 0xDE 222 */        0, /* right half block */
  /* 0xDF 223 */        0, /* upper half block */
  /* 0xE0 224 */ _L, /* small letter er */
  /* 0xE1 225 */ _L, /* small letter es */
  /* 0xE2 226 */ _L, /* small letter te */
  /* 0xE3 227 */ _L, /* small letter u */
  /* 0xE4 228 */ _L, /* small letter ef */
  /* 0xE5 229 */ _L, /* small letter kha */
  /* 0xE6 230 */ _L, /* small letter tse */
  /* 0xE7 231 */ _L, /* small letter che */
  /* 0xE8 232 */ _L, /* small letter sha */
  /* 0xE9 233 */ _L, /* small lettershcha */
  /* 0xEA 234 */ _L, /* small letter hardsign(ier) */
  /* 0xEB 235 */ _L, /* small letter y(iery) */
  /* 0xEC 236 */ _L, /* small letter softsign(ierik) */
  /* 0xED 237 */ _L, /* small letter reverse rounded e */
  /* 0xEE 238 */ _L, /* small letter yu */
  /* 0xEF 239 */ _L, /* small letter ya */
  /* 0xF0 240 */ _U, /* capital letter yoe - koi8r, win: identity sign - alt */
  /* 0xF1 241 */ _L, /* small letter yoe - koi8r, win: plus-minus sign - alt */
  /* 0xF2 242 */        0, /*  */
  /* 0xF3 243 */        0, /*  */
  /* 0xF4 244 */        0, /*  */
  /* 0xF5 245 */        0, /* less-than or equal to / х */
  /* 0xF6 246 */        0, /*  */
  /* 0xF7 247 */        0, /*  */
  /* 0xF8 248 */        0, /* degree sign */
  /* 0xF9 249 */        0, /* bullet operator */
  /* 0xFA 250 */        0, /* middle dot */
  /* 0xFB 251 */        0, /* square root */
  /* 0xFC 252 */        0, /* copyright sign /number sign */
  /* 0xFD 253 */        0, /* turtle (sun) sign / superscript two */
  /* 0xFE 254 */        0, /* black square, win - number sign */
  /* 0xFF 255 */        0  /* no-break space */
#endif
};

#endif /*_CTYPELOCALE_H*/
