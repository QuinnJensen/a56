
# line 2 "a56.y"
/*******************************************************
 *
 *  a56 - a DSP56001 assembler
 *
 *  Written by Quinn C. Jensen
 *  July 1990
 *
 *******************************************************/

/*
 * Copyright (C) 1990-1994 Quinn C. Jensen
 *
 * Permission to use, copy, modify, distribute, and sell this software
 * and its documentation for any purpose is hereby granted without fee,
 * provided that the above copyright notice appear in all copies and
 * that both that copyright notice and this permission notice appear
 * in supporting documentation.  The author makes no representations
 * about the suitability of this software for any purpose.  It is
 * provided "as is" without express or implied warranty.
 *
 */

/*
 *  a56.y - The YACC grammar for the assembler.
 *
 *  Note:  This module requires a "BIG" version of YACC.  I had to
 *  recompile YACC in the largest mode available.
 *
 *  Other notes:
 *
 *  MOVEC, MOVEM and MOVEP must be used explicitly--MOVE can't yet figure
 *  out which form to use.
 *
 */

#include "a56.h"

unsigned int w0, w1;			/* workspace for the actual generated code */
BOOL uses_w1;					/* says whether w1 is alive */
unsigned int pc;				/* current program counter */
int seg;						/* current segment P: X: Y: or L: */

int just_rep = 0;				/* keeps track of REP instruction */
int substatement = 0;			/* in a substatement */
BOOL long_symbolic_expr = FALSE; /* a parser flag */
char *new_include = NULL;		/* file to be included */

/* listing stuff */

char segs[] = "PXYL";
extern BOOL list_on_next;		/* listing to turn on or off */
BOOL list_on;					/* listing on at the moment */
extern char *cur_line;			/* points to line being lex'd */
char list_buf[1024 + 80];		/* listing buffer */
char list_buf2[1024 + 80];		/* listing buffer for two-line code */
BOOL uses_buf2 = FALSE;			/* list_buf2 is alive */
BOOL list_print_line = FALSE;	/* whether or not to print line in listing */
char *spaces(), *luntab();

struct n binary_op();
struct n unary_op();
struct n sym_ref();

#define R_R6				0x0001
#define R_R5				0x0002
#define R_R4				0x0004
#define R_DATA_ALU_ACCUM	0x0008
#define R_CTL_REG			0x0010
#define R_FUNKY_CTL_REG		0x0020
#define R_SDX				0x0040
#define R_SDY				0x0080
#define R_LSD				0x0100
#define R_AB				0x0200
#define R_XREG				0x0400
#define R_YREG				0x0800
/* registers to which short immediate move is an unsigned int */
#define R_UINT				0x1000
/* registers to which short immediate move is an signed frac */
#define R_SFRAC				0x2000

# line 83 "a56.y"
typedef union  {
	int ival;			/* integer value */
	struct n n;			/* just like in struct sym */
	double dval;		/* floating point value */
	char *sval;			/* string */
	int cval;			/* character */
	char cond;			/* condition */
	struct regs {
		int r6, r5, r4, data_alu_accum, ctl_reg, funky_ctl_reg;
		int sdx, sdy, lsd, ab, xreg, yreg;
		int flags;
	} regs;
	struct ea {
		int mode;
		int ext;
		int pp;
	} ea;
} YYSTYPE;
# define CHEX 257
# define CDEC 258
# define FRAC 259
# define AREG 260
# define BREG 261
# define MREG 262
# define NREG 263
# define RREG 264
# define XREG 265
# define YREG 266
# define OP 267
# define OPA 268
# define OPP 269
# define OP_JCC 270
# define OP_JSCC 271
# define OP_TCC 272
# define SYM 273
# define STRING 274
# define CHAR 275
# define XMEM 276
# define YMEM 277
# define LMEM 278
# define PMEM 279
# define AAAA 280
# define A10 281
# define BBBB 282
# define B10 283
# define AABB 284
# define BBAA 285
# define XXXX 286
# define YYYY 287
# define SR 288
# define MR 289
# define CCR 290
# define OMR 291
# define SP 292
# define SSH 293
# define SSL 294
# define LA 295
# define LC 296
# define EOL 297
# define EOS 298
# define LEXBAD 299
# define OP_ABS 300
# define OP_ADC 301
# define OP_ADD 302
# define OP_ADDL 303
# define OP_ADDR 304
# define OP_ASL 305
# define OP_ASR 306
# define OP_CLR 307
# define OP_CMP 308
# define OP_CMPM 309
# define OP_DIV 310
# define OP_MAC 311
# define OP_MACR 312
# define OP_MPY 313
# define OP_MPYR 314
# define OP_NEG 315
# define OP_NORM 316
# define OP_RND 317
# define OP_SBC 318
# define OP_SUB 319
# define OP_SUBL 320
# define OP_SUBR 321
# define OP_TFR 322
# define OP_TST 323
# define OP_AND 324
# define OP_ANDI 325
# define OP_EOR 326
# define OP_LSL 327
# define OP_LSR 328
# define OP_NOT 329
# define OP_OR 330
# define OP_ORI 331
# define OP_ROL 332
# define OP_ROR 333
# define OP_BCLR 334
# define OP_BSET 335
# define OP_BCHG 336
# define OP_BTST 337
# define OP_DO 338
# define OP_ENDDO 339
# define OP_LUA 340
# define OP_MOVE 341
# define OP_MOVEC 342
# define OP_MOVEM 343
# define OP_MOVEP 344
# define OP_ILLEGAL 345
# define OP_INCLUDE 346
# define OP_JMP 347
# define OP_JCLR 348
# define OP_JSET 349
# define OP_JSR 350
# define OP_JSCLR 351
# define OP_JSSET 352
# define OP_NOP 353
# define OP_REP 354
# define OP_RESET 355
# define OP_RTI 356
# define OP_RTS 357
# define OP_STOP 358
# define OP_SWI 359
# define OP_WAIT 360
# define OP_EQU 361
# define OP_ORG 362
# define OP_DC 363
# define OP_DS 364
# define OP_DSM 365
# define OP_END 366
# define OP_PAGE 367
# define OP_PSECT 368
# define OP_ALIGN 369
# define OP_INT 370
# define SHL 371
# define SHR 372
#define yyclearin yychar = -1
#define yyerrok yyerrflag = 0
extern int yychar;
extern short yyerrflag;
#ifndef YYMAXDEPTH
#define YYMAXDEPTH 150
#endif
YYSTYPE yylval, yyval;
# define YYERRCODE 256

# line 1644 "a56.y"


#include <stdio.h>

int yydebug;

struct n binary_op(a1, op, a2)
struct n a1, a2;
int op;
{
	struct n result;

	if(a1.type == UNDEF || a2.type == UNDEF) {
		result.type = UNDEF;
		return result;
	}

	/* promote to float automatically */

	if(a1.type != a2.type) {
		if(a1.type == INT) {
			a1.val.f = a1.val.i;		/* truncate */
			a1.type = FLT;
		} else {
			a2.val.f = a2.val.i;		/* truncate */
		}
	}
	result.type = a1.type;

	/* do the op */

	if(result.type == INT) {
		switch(op) {
			case '+':		result.val.i = a1.val.i + a2.val.i; break;
			case '-':		result.val.i = a1.val.i - a2.val.i; break;
			case '*':		result.val.i = a1.val.i * a2.val.i; break;
			case '/':		result.val.i = a1.val.i / a2.val.i; break;
			case '%':		result.val.i = a1.val.i % a2.val.i; break;
			case SHL:		result.val.i = a1.val.i << a2.val.i; break;
			case SHR:		result.val.i = a1.val.i >> a2.val.i; break;
			case '|':		result.val.i = a1.val.i | a2.val.i; break;
			case '&':		result.val.i = a1.val.i & a2.val.i; break;
			case '^':		result.val.i = a1.val.i ^ a2.val.i; break;
		}
	} else {
		switch(op) {
			case '+':		result.val.f = a1.val.f + a2.val.f; break;
			case '-':		result.val.f = a1.val.f - a2.val.f; break;
			case '*':		result.val.f = a1.val.f * a2.val.f; break;
			case '/':		result.val.f = a1.val.f / a2.val.f; break;
			case '%':		result.val.f = (int)a1.val.f % (int)a2.val.f; break;
			case SHL:		result.val.f = (int)a1.val.f << (int)a2.val.f; break;
			case SHR:		result.val.f = (int)a1.val.f >> (int)a2.val.f; break;
			case '|':		result.val.f = (int)a1.val.f | (int)a2.val.f; break;
			case '&':		result.val.f = (int)a1.val.f & (int)a2.val.f; break;
			case '^':		result.val.f = (int)a1.val.f ^ (int)a2.val.f; break;
		}
	}

	return result;
}

struct n unary_op(op, a1)
int op;
struct n a1;
{
	struct n result;

	if(a1.type == UNDEF) {
		result.type = UNDEF;
		return result;
	}

	result.type = a1.type;

	/* do the op */

	if(result.type == INT) {
		switch(op) {
			case '-':		result.val.i = -a1.val.i; break;
			case '~':		result.val.i = ~a1.val.i; break;
		}
	} else {
		switch(op) {
			case '-':		result.val.f = -a1.val.f; break;
			case '~':		result.val.f = ~(int)a1.val.f; break;
		}
	}

	return result;
}

n2int(n)
struct n n;
{
	if(n.type == UNDEF)
		return UNDEF;
	else if(n.type == INT)
		return n.val.i;
	else
		return n.val.f;
}

n2frac(n)
struct n n;
{
	double adval = n.val.f >= 0.0 ? n.val.f : -n.val.f;

	if(n.type == UNDEF)
		return UNDEF;
	else if(n.type == INT)
		return n.val.i;
	else if(n.val.f == -1.0)
		return 0x800000;

	adval -= (double)(int)adval;
	adval *= (double)0x800000;
	adval += 0.5;

	if(n.val.f >= 0.0)
		return adval;
	else
		return -adval;
}

extern struct {int n; char *name;} tok_tab[];
extern int n_tok;

char *tok_print(tok)
int tok;
{
	int i;
	static char buf[32];

	if(tok < 256) {
		sprintf(buf, tok < ' ' ? "\\z%02X" : "%c", tok & 0xFF);
		return buf;
	} else {
		for(i = 0; i < n_tok; i++) {
			if(tok == tok_tab[i].n)
				return tok_tab[i].name;
		}
	}
	return "*bogus*";
}

yyerror(s, a0, a1, a2, a3)
char *s, *a0, *a1, *a2, *a3;
{
	extern int error;
	char buf[1024];

	error++;
	sprintf(buf, s, a0, a1, a2, a3);

	if(pass == 2) {
		fprintf(stderr, "%s: line %d: %s (tok=%s)\n", curfile, curline,
			buf, tok_print(yychar));
		fprintf(stderr, "%s\n", cur_line);
		printf("%s: line %d: %s (tok=%s)\n", curfile, curline,
			buf, tok_print(yychar));
	}
}

char *luntab(s)
char *s;
{
	static char buf[1024];
	int p;

	strcpy(buf, s);

	untab(buf);
	p = strlen(buf);

	if(buf[p - 1] == '\n')
		buf[p - 1] = '\0';

	return buf;
}
short yyexca[] ={
-1, 1,
	0, -1,
	270, 33,
	271, 33,
	272, 33,
	297, 6,
	298, 6,
	300, 33,
	301, 33,
	302, 33,
	303, 33,
	304, 33,
	305, 33,
	306, 33,
	307, 33,
	308, 33,
	309, 33,
	310, 33,
	311, 33,
	312, 33,
	313, 33,
	314, 33,
	315, 33,
	316, 33,
	317, 33,
	318, 33,
	319, 33,
	320, 33,
	321, 33,
	322, 33,
	323, 33,
	324, 33,
	325, 33,
	326, 33,
	327, 33,
	328, 33,
	329, 33,
	330, 33,
	331, 33,
	332, 33,
	333, 33,
	334, 33,
	335, 33,
	336, 33,
	337, 33,
	338, 33,
	339, 33,
	340, 33,
	341, 33,
	342, 33,
	343, 33,
	344, 33,
	345, 33,
	347, 33,
	348, 33,
	349, 33,
	350, 33,
	351, 33,
	352, 33,
	353, 33,
	354, 33,
	355, 33,
	356, 33,
	357, 33,
	358, 33,
	359, 33,
	360, 33,
	363, 33,
	364, 33,
	365, 35,
	-2, 0,
-1, 8,
	297, 10,
	298, 10,
	365, 34,
	-2, 32,
	};
# define YYNPROD 288
# define YYLAST 1301
short yyact[]={

 136, 483, 223, 224, 486, 221, 347, 338, 205, 235,
 116,  95, 283, 278, 106,  19, 437, 285, 282, 111,
 281, 222, 284, 120,  96, 402,  97,  98,  99, 117,
  17,  18, 206, 443, 207, 133, 217, 218, 135, 399,
 283, 278, 485, 393, 442, 285, 282, 189, 281, 190,
 284, 403, 398, 104, 392, 109, 425, 526, 102, 438,
 439, 440, 141, 142, 212, 212, 212, 212, 527, 277,
 242, 246, 228, 303, 109, 304, 521, 229, 275, 241,
 245, 118, 110, 178, 179, 257, 259, 263, 516, 268,
 495, 271, 225, 355, 255, 305, 264, 277, 175, 276,
 174, 272, 273, 269, 176, 177, 464, 463, 243, 247,
 508, 184, 185, 113, 114, 115, 112, 461, 462, 307,
 308, 200, 201, 254, 533, 262, 183, 276, 182, 507,
 290,  62,  61,  88, 465,  97,  98,  99, 528, 103,
 342, 296, 283, 278, 300, 522, 216, 285, 282, 283,
 281, 519, 284, 248, 285, 517, 509, 297, 103, 284,
 301,  53,  59,  35,  41,  45,  55,  51,  42,  38,
  39,  90,  32,  33,  30,  31,  57,  89,  40,  60,
  34,  43,  47,  37,  46,  48,  94,  50,  56,  52,
  44,  49,  93,  58,  54,  83,  82,  81,  80,  70,
  71,  87,  36,  86,  85,  84,  77, 219,  64,  68,
  67,  63,  66,  65,  79,  69,  74,  78,  75,  72,
  76,  73, 283, 278,  21,  22, 227, 285, 282, 374,
 281, 515, 284, 283, 278, 250, 251, 252, 285, 282,
 466, 281, 199, 284, 233, 211, 330, 329, 510, 504,
 336, 334, 134, 459, 168, 458, 169, 339, 341, 340,
 234, 503, 344, 499, 472, 331, 348, 445, 350, 335,
  97,  98,  99, 433, 428, 228, 427, 109, 426, 277,
 229, 423, 349, 420, 351, 220, 107, 417, 108,  97,
  98,  99, 202, 203, 416, 225, 377, 415, 391, 380,
 383, 390, 213, 214, 215, 107, 389, 108, 375, 276,
 385, 237, 238, 239, 265, 384, 379, 195, 376, 378,
 394, 395, 396, 397, 360, 359, 400, 401, 358, 412,
 404, 405, 406, 407, 357, 413, 414, 381, 419, 356,
 226, 249, 249, 249, 249, 354, 280, 279, 230, 231,
 232, 100, 204, 430, 431, 432, 408, 434, 409, 436,
 353, 103, 122, 123, 197, 198, 435, 411, 228, 352,
 109, 346, 388, 229, 280, 279, 418, 441, 446, 447,
 448, 451, 422, 101, 424, 456, 457, 452, 454, 345,
 450, 429, 343, 337, 444, 104, 333, 109, 460, 286,
 102, 332, 101, 453, 455, 208, 209, 210, 328, 324,
 323, 322, 321, 320, 468, 471, 470, 319, 473, 474,
 475, 482, 318, 477, 479, 317, 488, 490, 491, 492,
 316, 348, 489, 469, 315, 493, 274, 484, 314, 484,
 478,   9, 313, 287, 288, 289, 312, 349, 500, 311,
 140, 494, 291, 505, 103, 502, 104, 292, 109, 480,
 498, 102, 487, 310, 283, 278, 309, 299, 372, 285,
 282, 236, 281, 298, 284, 511, 295, 104, 294, 109,
 302, 103, 102, 339, 341, 340, 293, 513, 512, 514,
 173, 132,  97,  98,  99, 283, 278, 303, 131, 130,
 285, 282, 484, 281, 520, 284, 181, 518, 107, 104,
 108, 109, 523, 524, 102, 129, 128, 127, 126, 125,
 529, 277, 266, 531, 261, 258, 180, 530, 253, 535,
 525, 283, 278, 534, 537, 244, 285, 282, 536, 281,
 240, 284, 103,  92,  91, 186, 187, 166, 228,  29,
 109, 276, 277, 229,  28,  27, 280, 279,  26, 124,
 104,  25, 109, 103,  24, 102, 325, 280, 279, 289,
 287, 327,  23, 109, 119,  20, 102, 302,   7, 170,
 171, 172, 276,   6, 283,  97,  98,  99, 277, 285,
 282,   5, 281,   3, 284, 103, 236, 196,   2,   1,
  13, 107, 267, 108, 270, 101, 342, 167, 306, 152,
 476, 467,  97,  98,  99, 137, 105, 361, 362, 363,
 364, 365, 366, 367, 368, 369, 370, 371, 107, 121,
 108, 373,   0,   0, 103, 122,   0,   0, 188, 191,
 192, 193, 194,   0, 386, 387, 103,   0,   0,   0,
   0,   0,   0,   0,   0,   0,   0, 103,   0,   0,
   0,   0,   4,   0,   0,   0, 139,   0, 289,   0,
   0, 140,   0,  97,  98,  99,   0,   0,   0,   8,
 410,   0,   0, 421,   0,   0,   0,   0,   0, 107,
   0, 108,   0,   0,  97,  98,  99,   0, 101, 342,
   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,
 107,   0, 108,   0,   0,   0,   0,   0,   0,   0,
   0,   0,   0,   0,   0, 101,  97,  98,  99,   0,
   0,   0,   0, 326,   0,   0,   0,   0,   0,   0,
   0,   0, 107,   0, 108,   0,   0,   0,   0,   0,
   0,   0,  15,   0,   0,   0,   0,   0,   0,   0,
   0,   0,   0,   0,   0,  97,  98,  99,  12,   0,
   0,   0,  16,  14,  11,  10,   0,  97,  98,  99,
 236, 107,   0, 108, 496, 497, 101,   0,  97,  98,
  99,   0,   0, 107,   0, 108,   0,   0, 280, 279,
   0,   0,   0,   0, 107,   0, 108, 101,   0,   0,
   0,   0,   0,   0,   0,   0,   0, 236,   0,   0,
   0, 145, 146, 151, 150, 149, 143, 144,   0, 280,
 279,   0,   0,   0,   0,   0,   0, 141, 142, 101,
 256, 147, 153, 148, 154, 157, 158, 155, 156, 159,
   0,   0, 160, 161, 162, 163, 164, 165,   0,   0,
   0,   0,   0,   0,   0, 280, 279,   0,   0,   0,
   0,   0,   0,   0,   0,   0,   0,   0, 101,   0,
   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,
 101, 145, 146, 151, 150, 149, 143, 144,   0,   0,
   0, 101,   0,   0,   0,   0,   0, 141, 142, 138,
   0, 147, 153, 148, 154, 157, 158, 155, 156, 159,
   0,   0, 160, 161, 162, 163, 164, 165, 145, 146,
 151, 150, 149, 143, 144,   0,   0,   0,   0,   0,
   0,   0,   0,   0, 141, 142,   0, 481, 147, 153,
 148, 154, 157, 158, 155, 156, 159,   0,   0, 160,
 161, 162, 163, 164, 165, 145, 146, 151, 150, 149,
 143, 144,   0,   0,   0,   0,   0,   0,   0,   0,
   0, 141, 142, 382,   0, 147, 153, 148, 154, 157,
 158, 155, 156, 159,   0,   0, 160, 161, 162, 163,
 164, 165,   0,   0,   0, 145, 146, 151, 150, 149,
 143, 144,   0,   0,   0,   0,   0,   0,   0,   0,
   0, 141, 142,   0,   0, 147, 153, 148, 154, 157,
 158, 155, 156, 159,   0,   0, 160, 161, 162, 163,
 164, 165, 145, 146, 151, 150, 149, 143, 144,   0,
   0,   0,   0,   0,   0,   0,   0,   0,   0, 449,
   0,   0, 147, 153, 148, 154, 157, 158, 155, 156,
 159,   0,   0, 160, 161, 162, 163, 164, 165, 145,
 146, 151, 150, 149, 143, 144,   0,   0,   0,   0,
   0,   0,   0,   0,   0, 141, 142,   0,   0, 147,
 153, 148, 154, 157, 158, 155, 156, 159,   0,   0,
 160, 161, 162, 163, 164, 165, 145, 146, 151, 150,
 149, 143, 144,   0,   0,   0,   0,   0,   0,   0,
   0,   0,   0, 532,   0,   0, 147, 153, 148, 154,
 157, 158, 155, 156, 159,   0,   0, 160, 161, 162,
 163, 164, 165, 145, 146, 151, 150, 149, 143, 144,
   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,
 506,   0,   0, 147, 153, 148, 154, 157, 158, 155,
 156, 159,   0,   0, 160, 161, 162, 163, 164, 165,
 145, 146, 151, 150, 149, 143, 144,   0,   0,   0,
   0,   0,   0,   0,   0,   0,   0, 501,   0,   0,
 147, 153, 148, 154, 157, 158, 155, 156, 159,   0,
   0, 160, 161, 162, 163, 164, 165, 145, 146, 151,
 150, 149, 143, 144,   0,   0,   0,   0,   0,   0,
   0,   0,   0,   0,   0,   0, 260, 147, 153, 148,
 154, 157, 158, 155, 156, 159,   0,   0, 160, 161,
 162, 163, 164, 165, 145, 146, 151, 150, 149, 143,
 144,   0,   0,   0,   0,   0,   0,   0,   0,   0,
   0,   0,   0,   0, 147, 153, 148, 154, 157, 158,
 155, 156, 159,   0,   0, 160, 161, 162, 163, 164,
 165 };
short yypact[]={

-1000, 406,-1000,-267,-282,-1000,-1000,-139,-350,-122,
 520,-191,-163,-355,-122,-193,-1000,-1000,-1000,-1000,
-1000, 355, 520,-1000,-1000, 631,-1000,-1000,-1000,-1000,
 211, 211, 211, 211,-182,-182,-1000,-154,-154,-154,
-233,-233,-233,-233,-233,-233,-233,-233,-144,-144,
-144,-248,-248,-248,-248,-248,-248,-248,-248,-250,
-250,  32,  32,  32,  32, 436, 436, 436, 436, 745,
 745,-1000,-1000,-1000,-1000,-1000,-1000,-1000,-1000,-1000,
 436, 436, 436, 436, 561, 967, 745, 410,1004,-161,
1004, 436, 436,-1000,-1000, 520,-196,-1000,-1000,-1000,
 458, 359, 520, 520, 520,-1000,-1000,-1000,-1000,-1000,
-163, 520,-1000,-1000,-1000,-1000, 520, 442,-1000, 434,
-1000,-1000, 458, 458,-1000,-1000,-1000,-1000,-1000,-1000,
-1000,-1000,-1000, 432,-1000,  32, 429, 423,  32,  13,
-169,-1000,-1000,-1000,-1000,-1000,-1000,-1000,-1000,-1000,
-1000,-1000,-1000,-1000,-1000,-1000,-1000,-1000,-1000,-1000,
-1000,-1000,-1000,-1000,-1000,-1000,-1000,-146,-1000,-1000,
-1000,-1000,-1000,-1000, 422, 419, 405, 402, 398, 394,
-1000,-1000, 390, 386, 381, 378,-1000,-1000,-1000, 373,
 369,-1000,-1000,-1000,-1000,-1000,-1000,-1000,-1000,-1000,
 368, 367,-1000,-1000,-1000,-1000,-1000,-1000,-1000,-1000,
-1000,-1000,-1000,-1000,-1000,-1000,-1000, 366, 365,-1000,
-1000,-1000,-1000,-1000,-1000, 520, 458,-1000, 469, 531,
-1000,-1000,-1000,-1000, 364,-1000, 437,-1000,-1000,-1000,
-1000,-1000,-1000,  32,-1000, 357, 352,  32,-1000, 349,
-1000,-1000,-1000,-1000, 328, 348, 508, 345,-1000, 327,
  32,-1000,  32, 325, 316, 301,-1000,-171, 295, 290,
-1000, 284, 281, 280, 458,-1000, 520, 520, 520, 520,
 520, 520, 520, 520, 520, 520, 520,-1000,-1000, 427,
 520, 185, 458,-122, 355,1004, 275, 272, 705,1004,
 271, 266, 458, 520, 520, 331, 262, 257, 254,-226,
-239,-248,-248,-248,-248,-228,-243,-248,-248,-257,
-229,-248,-248,-248,-248, 458, 315, 416, 819,-1000,
-1000,-1000, 520, 520, 253, 250, 243, 819, 239,-1000,
-1000,-1000, 520,-214, 237,-214,-223, 234,-1000,-1000,
 232, 230, 819,1004,1004, 229,1004,-248,1004,-230,
-230, 494, 196, 547, 105, 105, 112, 112,-1000,-1000,
-1000,   3,-1000, -25,-163, 223,-1000,1004,1004,1004,
 782,  32,  32,-1000,1004,1004, 458, 458, 210,-248,
-148,-159,-1000,-1000,-1000,-1000,-1000,-1000,-1000,-1000,
-1000,-1000,-1000,-1000,-1000,-1000,-1000,-1000, 210,-129,
 199, 235, 220,-1000,-1000, 520, 520, 520, 235,-1000,
 668, 458,-231,-214,-231,  32,1004,1004,1004,  32,
-1000,-1000,-1000,-174,-1000,-1000,-1000,-1000,-1000,-1000,
-1000,-1000,-1000, 520, 520,-122, 219, 930,-1000, 508,
 217, 205, 893,-1000,-1000,-1000,-1000,-1000,-134,-153,
-1000,-1000,-1000,-1000,-1000, 115,-1000, 204,-1000,-1000,
-1000,-1000, 520,-1000,-1000,-1000,-1000,-1000,-1000,-1000,
 328, 508,-1000,-1000,-1000,-1000,-1000,-231,-1000,-1000,
-1000,-1000,-1000,-1000,-1000,-1000, 458, 458, 187,-178,
 111, 508, 107,1004,-201, 101, 508,-1000,-1000,-1000,
 520,-1000,-1000,-1000,-1000,-122,-1000,-209,  94,1004,
-1000, 508, 856,  80,-1000,-1000,-1000, 508,1004,-1000,
-1000,-1000, 508,1004,-1000,-1000,-1000,-1000 };
short yypgo[]={

   0,  14,   1, 616, 340,   9, 615,   2,  21,   4,
   8,  38,   5, 260, 611,   3, 610,   6, 285, 226,
 609, 490, 506, 597, 317, 242, 352, 245, 146, 547,
 608, 607, 604,  16, 602,  19, 600,   0,   7, 599,
 598, 593, 591, 583, 578, 575, 574,  23, 572, 564,
 561, 559, 558, 555, 554, 549, 544, 543, 244, 540,
 535, 153, 528, 525, 524, 522, 519, 518, 517, 516,
 515, 499, 498, 491 };
short yyr1[]={

   0,  39,  39,  40,  40,  40,  41,  41,  41,  41,
  41,  42,  43,  43,  43,  43,  43,  43,  43,  43,
  43,  43,  43,  43,  46,  46,  47,  47,  35,  35,
  35,  35,  44,  44,  36,  36,  45,  48,  48,  48,
  50,  50,  50,  50,  50,  50,  50,  50,  50,  50,
  50,  50,  50,  50,  50,  50,  50,  50,  50,  50,
  50,  50,  50,  50,  50,  50,  50,  50,  50,  50,
  50,  29,  31,  31,  31,  30,  30,  30,  30,  21,
  21,  21,  21,  21,  21,  22,  22,  22,  22,  23,
  23,  23,  23,  24,  25,  25,  26,  27,  28,  28,
  10,  10,  49,  49,  49,  49,  55,  55,  55,  55,
  56,  56,  57,  57,  52,  52,  52,  52,  52,  52,
  52,  52,  52,  52,  52,  52,  52,  52,  52,  52,
  52,  52,  52,  60,  60,  60,  60,  60,  59,  59,
  59,  59,  59,  58,  58,  14,  14,  14,  14,  53,
  53,  53,  53,  61,  61,  16,  16,  16,  54,  54,
  54,  54,  54,  65,  65,  34,  32,  64,  64,  64,
  64,  64,  64,  62,  62,  62,  62,  62,  62,  38,
  38,  38,  63,  63,  13,  11,  11,  17,  17,  18,
  18,  12,  12,  15,  15,  15,  15,  19,  19,  19,
  19,  37,  37,  37,  37,  37,  37,  37,  37,  37,
  37,  37,  37,  37,  37,  37,  37,  20,  20,  20,
  20,  20,  20,  20,  33,  33,  33,  51,  51,  51,
  51,  51,  51,  51,  51,  66,  71,  67,  68,  68,
  68,  68,  68,  69,  69,  69,  70,  70,  70,  73,
  73,  73,  73,  72,  72,  72,  72,   1,   1,   1,
   5,   5,   6,   7,   8,   9,   2,   2,   2,   3,
   3,   3,   3,   4,   4,   4,   4,   4,   4,   4,
   4,   4,   4,   4,   4,   4,   4,   4 };
short yyr2[]={

   0,   0,   2,   2,   2,   2,   0,   1,   1,   2,
   1,   3,   3,   2,   2,   6,   3,   6,   3,   3,
   3,   8,   2,   1,   3,   1,   1,   1,   1,   1,
   1,   1,   1,   0,   1,   0,   1,   1,   1,   2,
   2,   2,   2,   2,   2,   2,   1,   2,   2,   2,
   2,   2,   2,   2,   2,   2,   2,   2,   2,   2,
   2,   2,   2,   2,   2,   2,   2,   2,   2,   2,
   2,   4,   1,   1,   0,   3,   3,   3,   3,   3,
   3,   3,   3,   3,   3,   3,   3,   3,   3,   1,
   1,   3,   3,   1,   3,   3,   1,   1,   3,   3,
   1,   1,   1,   1,   1,   1,   4,   2,   4,   4,
   1,   1,   1,   1,   2,   2,   2,   2,   2,   2,
   2,   2,   2,   2,   1,   1,   1,   1,   1,   1,
   1,   1,   1,   3,   3,   4,   4,   4,   1,   1,
   2,   2,   2,   6,   5,   1,   1,   1,   1,   2,
   2,   2,   2,   4,   3,   1,   1,   1,   2,   2,
   2,   4,   2,   1,   4,   3,   3,   4,   4,   3,
   4,   4,   3,   5,   4,   5,   5,   4,   4,   1,
   1,   1,   4,   4,   1,   1,   1,   1,   1,   1,
   1,   1,   1,   1,   3,   5,   4,   5,   5,   4,
   4,   1,   1,   1,   1,   1,   1,   1,   1,   1,
   1,   1,   1,   1,   1,   1,   1,   1,   1,   1,
   1,   1,   1,   1,   1,   1,   1,   1,   1,   1,
   1,   1,   1,   1,   1,   3,   3,   1,   4,   4,
   4,   4,   3,   7,   6,   7,   7,   6,   7,   4,
   4,   4,   4,   8,   8,   8,   8,   1,   1,   1,
   2,   3,   3,   1,   2,   2,   1,   1,   1,   1,
   1,   1,   1,   4,   3,   3,   3,   3,   3,   3,
   3,   3,   3,   3,   2,   2,   3,   1 };
short yychk[]={

-1000, -39, -40, -41, 256, -42, -43, -44, 273,  35,
 369, 368, 362, -36, 367, 346, 366, 297, 298, 297,
 -45, 363, 364, -48, -49, -50, -52, -53, -54, -55,
 313, 314, 311, 312, 319, 302, 341, 322, 308, 309,
 317, 303, 307, 320, 329, 304, 323, 321, 324, 330,
 326, 306, 328, 300, 333, 305, 327, 315, 332, 301,
 318, 271, 270, 350, 347, 352, 351, 349, 348, 354,
 338, 339, 358, 360, 355, 357, 359, 345, 356, 353,
 337, 336, 335, 334, 344, 343, 342, 340, 272, 316,
 310, -56, -57, 331, 325, 361,  -1, 257, 258, 259,
  -4, 370,  45, 126,  40,  -3,  -1, 273, 275,  42,
 273, -35, 279, 276, 277, 278, 365,  -1, 274, -46,
 -47, 274,  -4,  -4, -51, -66, -67, -68, -69, -70,
 -71, -72, -73,  -5, -19, -11, -37,  -6, 278,  35,
  40, 276, 277, 265, 266, 260, 261, 280, 282, 264,
 263, 262, -20, 281, 283, 286, 287, 284, 285, 288,
 291, 292, 293, 294, 295, 296, -29, -31,  43,  45,
 -29, -29, -29, -21, 282, 280, 286, 287, 265, 266,
 -21, -22, 282, 280, 265, 266, -22, -22, -23, 280,
 282, -23, -23, -23, -23, -24, -23, -24, -24, -25,
 265, 266, -25, -25, -26, -10, 280, 282, -26, -26,
 -26, -27, -10, -27, -27, -27, -28, 286, 287, -28,
 -18, -12,  -8,  -7, -15,  60,  -4, -19,  40,  45,
 -18, -18, -18, -58, -13,  -5,  35, -58, -58, -58,
 -59,  -5, -37, -11, -60,  -5, -37, -11, -61, -13,
 -61, -61, -61, -62, -11,  -5, 279, -37, -63, -37,
 279, -64, -11, -37,  -5, -19, -65, -34, -37, 264,
 -32, -37,  -5,  -5,  -4, 274, 124,  94,  38, 372,
 371,  45,  43,  37,  47,  42,  40,  -4,  -4,  -4,
 -35,  -4,  -4,  44,  44,  44, -12,  -8,  44,  44,
 -12,  -8,  -4,  60,  62, 264, -30, 265, 266,  44,
  44,  44,  44,  44,  44,  44,  44,  44,  44,  44,
  44,  44,  44,  44,  44,  -4, 264,  40,  44, -15,
  -7,  -8,  44,  44, -15,  -8,  -7,  44, -38,  -7,
  -9, -15, 371,  44, -12,  44,  44, -17, -12,  -8,
 -12,  -8,  44,  44,  44, 264,  44,  44,  44,  44,
  44,  -4,  -4,  -4,  -4,  -4,  -4,  -4,  -4,  -4,
  -4,  -4,  41,  -4,  44,  -1, -47, -37,  44,  44,
 -37, -11, 278, -37,  44,  44,  -4,  -4,  41,  44,
  44,  44, 280, 282, -10, -10, -10, -10, 280, 282,
 -10, -10, 282, 280, -10, -10, -10, -10,  41,  43,
 264, -11, -37,  -7,  -7,  44,  44,  44, -11, -37,
  44,  -4, -11,  44, -11, 279,  44,  44,  44, -11,
 -37, -37, -37,  44, -37, -10, -37, -33, 289, 290,
 291, -33,  41,  58, -35,  44, -37, -37, -37, 277,
  -5, -37, -12,  -8, -12,  -8, -37, -37,  45,  43,
 -10, 265, 266, 266, 265, 263,  41, -14,  -7,  -8,
  -9, -15,  44,  -7,  -7,  -7, -16,  -9,  -8, -12,
 -11, 279, -37,  -2,  -1, 273,  -9, -11,  -2, -17,
 -37, -37, -37, -12,  -8, 264,  -4,  -4,  -1,  44,
 -37, 277, -12,  44,  44, -37, 277, 263, 263,  41,
  44,  -7, -38, -12,  -2,  44, 266,  44, -12,  44,
 -37, 277,  44, -12,  -7,  -1, 266, 277,  44, -37,
 -12, -37, 277,  44, -12, -37, -12, -37 };
short yydef[]={

   1,  -2,   2,   0,   0,   7,   8,   0,  -2,   0,
   0,   0,   0,   0,   0,   0,  23,   3,   4,   5,
   9,   0,   0,  36,  37,  38, 102, 103, 104, 105,
  74,  74,  74,  74,   0,   0,  46,   0,   0,   0,
   0,   0,   0,   0,   0,   0,   0,   0, 112, 110,
   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,
   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,
   0, 124, 125, 126, 127, 128, 129, 130, 131, 132,
   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,
   0,   0,   0, 111, 113,   0,   0, 257, 258, 259,
  13,   0,   0,   0,   0, 287, 269, 270, 271, 272,
  14,   0,  28,  29,  30,  31,   0,   0,  22,  18,
  25,  26,  27,  19,  39, 227, 228, 229, 230, 231,
 232, 233, 234,   0, 237,   0,   0,   0,   0,   0,
   0, 185, 186, 201, 202, 203, 204, 205, 206, 207,
 208, 209, 210, 211, 212, 213, 214, 215, 216, 217,
 218, 219, 220, 221, 222, 223,  40,   0,  72,  73,
  41,  42,  43,  44,   0,   0,   0,   0,   0,   0,
  45,  47,   0,   0,   0,   0,  48,  49,  50,  89,
  90,  51,  52,  53,  54,  55,  93,  56,  57,  58,
   0,   0,  59,  60,  61,  96, 100, 101,  62,  63,
  64,  65,  97,  66,  67,  68,  69,   0,   0,  70,
 114, 189, 190, 191, 192,   0, 263, 193,   0,   0,
 115, 116, 117, 118,   0, 184,   0, 119, 120, 121,
 122, 138, 139,   0, 123,   0,   0,   0, 149,   0,
 150, 151, 152, 158,   0,   0,   0,   0, 159,   0,
   0, 160,   0,   0,   0,   0, 162, 163,   0,   0,
 107,   0,   0,   0,  12,  11,   0,   0,   0,   0,
   0,   0,   0,   0,   0,   0,   0, 284, 285,   0,
   0,  16,  20,   0,   0,   0,   0,   0,   0,   0,
   0,   0, 260,   0,   0,   0,   0,   0,   0,   0,
   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,
   0,   0,   0,   0,   0, 264,   0,   0,   0, 140,
 141, 142,   0,   0,   0,   0,   0,   0,   0, 179,
 180, 181,   0,   0,   0,   0,   0,   0, 187, 188,
   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,
   0, 274, 275, 276, 277, 278, 279, 280, 281, 282,
 283,   0, 286,   0,   0,   0,  24, 235,   0,   0,
 236,   0,   0, 242,   0,   0, 261, 262,   0,   0,
   0,   0,  79,  80,  81,  82,  83,  84,  85,  86,
  87,  88,  92,  91,  94,  95,  98,  99, 194,   0,
   0,   0,   0, 133, 134,   0,   0,   0,   0, 154,
   0, 265,   0,   0,   0,   0,   0,   0,   0,   0,
 172, 169, 161,   0, 165, 106, 166, 108, 224, 225,
 226, 109, 273,   0,   0,   0,   0, 238, 239,   0,
   0,   0, 240, 241, 250, 252, 249, 251, 199, 200,
  71,  75,  77,  76,  78,   0, 196,   0, 145, 146,
 147, 148,   0, 135, 136, 137, 153, 155, 156, 157,
   0,   0, 178, 174, 266, 267, 268,   0, 177, 182,
 183, 167, 170, 168, 171, 164,  15,  17,   0,   0,
   0,   0,   0,   0,   0,   0,   0, 197, 198, 195,
   0, 144, 173, 176, 175,   0, 244,   0,   0,   0,
 247,   0,   0,   0, 143,  21, 243,   0,   0, 246,
 248, 245,   0,   0, 254, 253, 256, 255 };
#ifndef lint
static char yaccpar_sccsid[] = "@(#)yaccpar	4.1	(Berkeley)	2/11/83";
#endif not lint

#
# define YYFLAG -1000
# define YYERROR goto yyerrlab
# define YYACCEPT return(0)
# define YYABORT return(1)

/*	parser for yacc output	*/

#ifdef YYDEBUG
int yydebug = 0; /* 1 for debugging */
#endif
YYSTYPE yyv[YYMAXDEPTH]; /* where the values are stored */
int yychar = -1; /* current input token number */
int yynerrs = 0;  /* number of errors */
short yyerrflag = 0;  /* error recovery flag */

yyparse() {

	short yys[YYMAXDEPTH];
	short yyj, yym;
	register YYSTYPE *yypvt;
	register short yystate, *yyps, yyn;
	register YYSTYPE *yypv;
	register short *yyxi;

	yystate = 0;
	yychar = -1;
	yynerrs = 0;
	yyerrflag = 0;
	yyps= &yys[-1];
	yypv= &yyv[-1];

 yystack:    /* put a state and value onto the stack */

#ifdef YYDEBUG
	if( yydebug  ) printf( "state %d, char 0%o\n", yystate, yychar );
#endif
		if( ++yyps> &yys[YYMAXDEPTH] ) { yyerror( "yacc stack overflow" ); return(1); }
		*yyps = yystate;
		++yypv;
		*yypv = yyval;

 yynewstate:

	yyn = yypact[yystate];

	if( yyn<= YYFLAG ) goto yydefault; /* simple state */

	if( yychar<0 ) if( (yychar=yylex())<0 ) yychar=0;
	if( (yyn += yychar)<0 || yyn >= YYLAST ) goto yydefault;

	if( yychk[ yyn=yyact[ yyn ] ] == yychar ){ /* valid shift */
		yychar = -1;
		yyval = yylval;
		yystate = yyn;
		if( yyerrflag > 0 ) --yyerrflag;
		goto yystack;
		}

 yydefault:
	/* default state action */

	if( (yyn=yydef[yystate]) == -2 ) {
		if( yychar<0 ) if( (yychar=yylex())<0 ) yychar = 0;
		/* look through exception table */

		for( yyxi=yyexca; (*yyxi!= (-1)) || (yyxi[1]!=yystate) ; yyxi += 2 ) ; /* VOID */

		while( *(yyxi+=2) >= 0 ){
			if( *yyxi == yychar ) break;
			}
		if( (yyn = yyxi[1]) < 0 ) return(0);   /* accept */
		}

	if( yyn == 0 ){ /* error */
		/* error ... attempt to resume parsing */

		switch( yyerrflag ){

		case 0:   /* brand new error */

			yyerror( "syntax error" );
		yyerrlab:
			++yynerrs;

		case 1:
		case 2: /* incompletely recovered error ... try again */

			yyerrflag = 3;

			/* find a state where "error" is a legal shift action */

			while ( yyps >= yys ) {
			   yyn = yypact[*yyps] + YYERRCODE;
			   if( yyn>= 0 && yyn < YYLAST && yychk[yyact[yyn]] == YYERRCODE ){
			      yystate = yyact[yyn];  /* simulate a shift of "error" */
			      goto yystack;
			      }
			   yyn = yypact[*yyps];

			   /* the current yyps has no shift onn "error", pop stack */

#ifdef YYDEBUG
			   if( yydebug ) printf( "error recovery pops state %d, uncovers %d\n", *yyps, yyps[-1] );
#endif
			   --yyps;
			   --yypv;
			   }

			/* there is no state on the stack with an error shift ... abort */

	yyabort:
			return(1);


		case 3:  /* no shift yet; clobber input char */

#ifdef YYDEBUG
			if( yydebug ) printf( "error recovery discards char %d\n", yychar );
#endif

			if( yychar == 0 ) goto yyabort; /* don't discard EOF, quit */
			yychar = -1;
			goto yynewstate;   /* try again in the same state */

			}

		}

	/* reduction by production yyn */

#ifdef YYDEBUG
		if( yydebug ) printf("reduce %d\n",yyn);
#endif
		yyps -= yyr2[yyn];
		yypvt = yypv;
		yypv -= yyr2[yyn];
		yyval = yypv[1];
		yym=yyn;
			/* consult goto table to find next state */
		yyn = yyr1[yyn];
		yyj = yypgo[yyn] + *yyps + 1;
		if( yyj>=YYLAST || yychk[ yystate = yyact[yyj] ] != -yyn ) yystate = yyact[yypgo[yyn]];
		switch(yym){
			
case 3:
# line 248 "a56.y"
{
			if(pass == 2 && list_on && list_print_line) {
				printf(ldebug ? "\n(%s|%s)\n" : "%s%s\n",
					list_buf, substatement == 0 ? luntab(cur_line) : "");
				if(uses_buf2)
					printf(ldebug ? "\n(%s|)\n" : "%s\n",
						list_buf2);
				list_buf[0] = list_buf2[0] = '\0';
			}
			curline++;
			uses_buf2 = FALSE;
			list_print_line = TRUE;
			list_on = list_on_next;
			substatement = 0;
			if(NOT check_psect(seg, pc) && pass == 2)
				yyerror("%04X: psect violation", pc);
			} break;
case 4:
# line 266 "a56.y"
{
			if(pass == 2 && list_on && list_print_line) {
				printf(ldebug ? "\n(%s" : "%s", list_buf);
				if(substatement == 0)
					printf(ldebug ? "|%s)\n" : "%s\n", luntab(cur_line));
				else
					printf(ldebug ? ")\n" : "\n");
				if(uses_buf2)
					printf(ldebug ? "\n(%s|)\n" : "%s\n",
						list_buf2);
				list_buf[0] = list_buf2[0] = '\0';
			}
			substatement++;
			uses_buf2 = FALSE;
			list_print_line = TRUE;
			list_on = list_on_next;
			if(NOT check_psect(seg, pc) && pass == 2)
				yyerror("%04X: psect violation", pc);
			} break;
case 5:
# line 286 "a56.y"
{curline++; substatement = 0;} break;
case 6:
# line 291 "a56.y"
{sprintf(list_buf, "%s", spaces(0));} break;
case 7:
# line 293 "a56.y"
{list_print_line = FALSE;} break;
case 8:
# line 295 "a56.y"
{long_symbolic_expr = FALSE;} break;
case 9:
# line 297 "a56.y"
{char *printcode();
			if(pass == 2) {
				gencode(seg, pc, w0);
				sprintf(list_buf, "%c:%04X %s ", segs[seg], pc, printcode(w0));
				pc++;
				if(uses_w1) {
					gencode(seg, pc, w1);
					sprintf(list_buf2, "%c:%04X %s", segs[seg], pc,
	       					printcode(w1 & 0xFFFFFF));
					uses_buf2++;
					pc++;
				}
			} else {
				pc++;
				if(uses_w1)
					pc++;
			}
			w0 = w1 = 0; uses_w1 = FALSE; 
			long_symbolic_expr = FALSE;} break;
case 10:
# line 317 "a56.y"
{sym_def(yypvt[-0].sval, INT, pc);
			free(yypvt[-0].sval);
			if(pass == 2 && list_on) {
				sprintf(list_buf, "%c:%04X%s", segs[seg], pc, spaces(14-8));
			long_symbolic_expr = FALSE;
			}} break;
case 11:
# line 327 "a56.y"
{if(strlen(yypvt[-0].sval) > 0)
				curfile = yypvt[-0].sval;
			else
				curfile = "<stdin>";
			curline = yypvt[-1].n.val.i - 1;} break;
case 12:
# line 336 "a56.y"
{sym_def(yypvt[-2].sval, yypvt[-0].n.type, yypvt[-0].n.val.i, yypvt[-0].n.val.f);
			free(yypvt[-2].sval);
			if(pass == 2 && list_on) {
				if(yypvt[-0].n.type == INT)
					sprintf(list_buf, "%06X%s",
						yypvt[-0].n.val.i & 0xFFFFFF,
						spaces(14-8));
				else
					sprintf(list_buf, "%10g%s", yypvt[-0].n.val.f,
						spaces(14-4));
			}} break;
case 13:
# line 348 "a56.y"
{int ival = n2int(yypvt[-0].n);
			if(yypvt[-0].n.type == UNDEF) {
				yyerror("illegal forward reference");
			} else if (ival <= 1) {
				yyerror("%d: illegal alignment", ival);
			} else {
				if(pc % ival != 0)
					pc += ival - pc % ival;
			}
			if(pass == 2 && list_on)
				sprintf(list_buf, "%c:%04X%s", segs[seg], pc, 
					spaces(14-8));
			} break;
case 14:
# line 362 "a56.y"
{struct psect *pp = find_psect(yypvt[-0].sval);
			if(NOT pp) {
				if(pass == 2)
					yyerror("%s: undefined psect", yypvt[-0].sval);
			} else {
				seg = pp->seg;
				pc = pp->pc;
				set_psect(pp);
				if(pass == 2 && list_on)
					sprintf(list_buf, "%c:%04X%s", segs[seg], pc,
						spaces(14-8));
			}
			free(yypvt[-0].sval);} break;
case 15:
# line 376 "a56.y"
{new_psect(yypvt[-4].sval, yypvt[-3].ival, n2int(yypvt[-2].n), n2int(yypvt[-0].n));
			if(pass == 2 && list_on)
				sprintf(list_buf, "%c:%04X %04X%s", 
					segs[yypvt[-3].ival], n2int(yypvt[-2].n), n2int(yypvt[-0].n), spaces(14-8+4+1));
			} break;
case 16:
# line 382 "a56.y"
{pc = n2int(yypvt[-0].n);
			seg = yypvt[-1].ival;
			if(pass == 2 && list_on)
				sprintf(list_buf, "%c:%04X%s", segs[seg], pc, 
					spaces(14-8));
			} break;
case 17:
# line 389 "a56.y"
{pc = n2int(yypvt[-3].n);
			seg = yypvt[-4].ival;
			if(pass == 2 && list_on)
				sprintf(list_buf, "%c:%04X%s", segs[seg], pc, 
					spaces(14-8));
			} break;
case 19:
# line 397 "a56.y"
{pc += n2int(yypvt[-0].n);
			} break;
case 20:
# line 400 "a56.y"
{int size = n2int(yypvt[-0].n);
			if(size)
			{    int align = 1;
			     while(align < size)
				 align <<= 1;
			     pc += (align - (pc % align));
			}
			if(yypvt[-2].sval)
			{   sym_def(yypvt[-2].sval, INT, pc);
 			    free(yypvt[-2].sval);
			}
			pc += size;
			} break;
case 21:
# line 414 "a56.y"
{if(pass == 2 && list_on) {
				sprintf(list_buf, "%s", spaces(0));
			}} break;
case 22:
# line 418 "a56.y"
{if(pass == 2 && list_on) {
				printf(ldebug ? "\n(%s|%s)\n" : "%s%s\n",
					spaces(0), luntab(cur_line));
				list_print_line = FALSE;
			}
			include(yypvt[-0].sval); /* free($2); */
			} break;
case 23:
# line 426 "a56.y"
{if(pass == 2 && list_on) {
				sprintf(list_buf, "%s", spaces(0));
			}} break;
case 26:
# line 438 "a56.y"
{int len = strlen(yypvt[-0].sval), i; char *cp; w0 = 0;
			if(len % 3 == 2)
				len++;	/* force empty word */
			for(i = 0, cp = yypvt[-0].sval; i < len; i++, cp++) {
				w0 |= (*cp & 0xFF) << (2 - (i % 3)) * 8;
				if(i % 3 == 2 || i == len - 1) {
					if(pass == 2) {
						if(list_on) sprintf(list_buf, "%c:%04X %06X%s",
							segs[seg], pc, w0, 
							spaces(14-6+5));
						gencode(seg, pc, w0);
					}
					pc++; w0 = 0;
				}
			}
			free(yypvt[-0].sval);} break;
case 27:
# line 455 "a56.y"
{int frac = n2frac(yypvt[-0].n);
			if(pass == 2) {
				if(list_on) {
					sprintf(list_buf, "%c:%04X %06X%s", segs[seg], pc, 
						frac & 0xFFFFFF, spaces(14-6+5));
				}
				gencode(seg, pc, frac);
			}
			pc++;} break;
case 28:
# line 466 "a56.y"
{yyval.ival = PROG;} break;
case 29:
# line 468 "a56.y"
{yyval.ival = XDATA;} break;
case 30:
# line 470 "a56.y"
{yyval.ival = YDATA;} break;
case 31:
# line 472 "a56.y"
{yyval.ival = LDATA;} break;
case 32:
# line 477 "a56.y"
{sym_def(yypvt[-0].sval, INT, pc);
			free(yypvt[-0].sval);} break;
case 34:
# line 483 "a56.y"
{yyval.sval = yypvt[-0].sval;} break;
case 35:
# line 484 "a56.y"
{yyval.sval = NULL;} break;
case 36:
# line 489 "a56.y"
{if(just_rep) 
				just_rep--;} break;
case 38:
# line 496 "a56.y"
{w0 |= 0x200000;} break;
case 40:
# line 505 "a56.y"
{w0 |= 0x80 | yypvt[-0].ival << 2;} break;
case 41:
# line 507 "a56.y"
{w0 |= 0x81 | yypvt[-0].ival << 2;} break;
case 42:
# line 509 "a56.y"
{w0 |= 0x82 | yypvt[-0].ival << 2;} break;
case 43:
# line 511 "a56.y"
{w0 |= 0x83 | yypvt[-0].ival << 2;} break;
case 44:
# line 514 "a56.y"
{w0 |= 0x04 | yypvt[-0].ival << 3;} break;
case 45:
# line 516 "a56.y"
{w0 |= 0x00 | yypvt[-0].ival << 3;} break;
case 46:
# line 518 "a56.y"
{w0 |= 0x00;} break;
case 47:
# line 521 "a56.y"
{w0 |= 0x01 | yypvt[-0].ival << 3;} break;
case 48:
# line 523 "a56.y"
{w0 |= 0x05 | yypvt[-0].ival << 3;} break;
case 49:
# line 525 "a56.y"
{w0 |= 0x07 | yypvt[-0].ival << 3;} break;
case 50:
# line 528 "a56.y"
{w0 |= 0x11 | yypvt[-0].ival << 3;} break;
case 51:
# line 530 "a56.y"
{w0 |= 0x12 | yypvt[-0].ival << 3;} break;
case 52:
# line 532 "a56.y"
{w0 |= 0x13 | yypvt[-0].ival << 3;} break;
case 53:
# line 534 "a56.y"
{w0 |= 0x16 | yypvt[-0].ival << 3;} break;
case 54:
# line 536 "a56.y"
{w0 |= 0x17 | yypvt[-0].ival << 3;} break;
case 55:
# line 539 "a56.y"
{w0 |= 0x02 | yypvt[-0].ival << 3;} break;
case 56:
# line 541 "a56.y"
{w0 |= 0x03 | yypvt[-0].ival << 3;} break;
case 57:
# line 543 "a56.y"
{w0 |= 0x06 | yypvt[-0].ival << 3;} break;
case 58:
# line 546 "a56.y"
{w0 |= 0x46 | yypvt[-0].ival << 3;} break;
case 59:
# line 548 "a56.y"
{w0 |= 0x42 | yypvt[-0].ival << 3;} break;
case 60:
# line 550 "a56.y"
{w0 |= 0x43 | yypvt[-0].ival << 3;} break;
case 61:
# line 553 "a56.y"
{w0 |= 0x22 | yypvt[-0].ival << 3;} break;
case 62:
# line 555 "a56.y"
{w0 |= 0x23 | yypvt[-0].ival << 3;} break;
case 63:
# line 557 "a56.y"
{w0 |= 0x26 | yypvt[-0].ival << 3;} break;
case 64:
# line 559 "a56.y"
{w0 |= 0x27 | yypvt[-0].ival << 3;} break;
case 65:
# line 562 "a56.y"
{w0 |= 0x32 | yypvt[-0].ival << 3;} break;
case 66:
# line 564 "a56.y"
{w0 |= 0x33 | yypvt[-0].ival << 3;} break;
case 67:
# line 566 "a56.y"
{w0 |= 0x36 | yypvt[-0].ival << 3;} break;
case 68:
# line 568 "a56.y"
{w0 |= 0x37 | yypvt[-0].ival << 3;} break;
case 69:
# line 571 "a56.y"
{w0 |= 0x21 | yypvt[-0].ival << 3;} break;
case 70:
# line 573 "a56.y"
{w0 |= 0x25 | yypvt[-0].ival << 3;} break;
case 71:
# line 577 "a56.y"
{yyval.ival = yypvt[-3].ival | yypvt[-0].ival << 1 | yypvt[-2].ival << 2;} break;
case 72:
# line 582 "a56.y"
{yyval.ival = 0;} break;
case 73:
# line 584 "a56.y"
{yyval.ival = 1;} break;
case 74:
# line 586 "a56.y"
{yyval.ival = 0;} break;
case 75:
# line 591 "a56.y"
{switch (yypvt[-2].ival << 4 | yypvt[-0].ival) {
				case 0x00: yyval.ival = 0x0; break;
				case 0x01: 
				case 0x10: yyval.ival = 0x2; break;
				case 0x11: 
					yyerror("illegal source operands"); 
					break;
			}} break;
case 76:
# line 600 "a56.y"
{switch (yypvt[-2].ival << 4 | yypvt[-0].ival) {
				case 0x00: yyval.ival = 0x1; break;
				case 0x01: 
				case 0x10: yyval.ival = 0x3; break;
				case 0x11: 
					yyerror("illegal source operands"); 
					break;
			}} break;
case 77:
# line 609 "a56.y"
{switch (yypvt[-2].ival << 4 | yypvt[-0].ival) {
				case 0x00: yyval.ival = 0x5; break;
				case 0x01: yyval.ival = 0x4; break;
				case 0x10: yyval.ival = 0x6; break;
				case 0x11: yyval.ival = 0x7; break;
			}} break;
case 78:
# line 616 "a56.y"
{switch (yypvt[-2].ival << 4 | yypvt[-0].ival) {
				case 0x00: yyval.ival = 0x5; break;
				case 0x01: yyval.ival = 0x6; break;
				case 0x10: yyval.ival = 0x4; break;
				case 0x11: yyval.ival = 0x7; break;
			}} break;
case 79:
# line 625 "a56.y"
{yyval.ival = 0x2;} break;
case 80:
# line 627 "a56.y"
{yyval.ival = 0x3;} break;
case 81:
# line 629 "a56.y"
{yyval.ival = 0x4 | yypvt[-0].ival;} break;
case 82:
# line 631 "a56.y"
{yyval.ival = 0x6 | yypvt[-0].ival;} break;
case 83:
# line 633 "a56.y"
{yyval.ival = 0x8 | yypvt[-2].ival << 2 | yypvt[-0].ival;} break;
case 84:
# line 635 "a56.y"
{yyval.ival = 0xA | yypvt[-2].ival << 2 | yypvt[-0].ival;} break;
case 85:
# line 639 "a56.y"
{yyval.ival = 0x0;} break;
case 86:
# line 641 "a56.y"
{yyval.ival = 0x1;} break;
case 87:
# line 643 "a56.y"
{yyval.ival = 0x8 | yypvt[-2].ival << 2 | yypvt[-0].ival;} break;
case 88:
# line 645 "a56.y"
{yyval.ival = 0xA | yypvt[-2].ival << 2 | yypvt[-0].ival;} break;
case 89:
# line 649 "a56.y"
{yyval.ival = 0x0;} break;
case 90:
# line 651 "a56.y"
{yyval.ival = 0x1;} break;
case 91:
# line 653 "a56.y"
{yyval.ival = 0x0;} break;
case 92:
# line 655 "a56.y"
{yyval.ival = 0x1;} break;
case 93:
# line 659 "a56.y"
{yyval.ival = yypvt[-0].ival;} break;
case 94:
# line 663 "a56.y"
{yyval.ival = 0x0 | yypvt[-2].ival << 2 | yypvt[-0].ival;} break;
case 95:
# line 665 "a56.y"
{yyval.ival = 0x2 | yypvt[-2].ival << 2 | yypvt[-0].ival;} break;
case 96:
# line 669 "a56.y"
{yyval.ival = yypvt[-0].ival;} break;
case 97:
# line 673 "a56.y"
{yyval.ival = yypvt[-0].ival;} break;
case 98:
# line 677 "a56.y"
{yyval.ival = 0x0 | yypvt[-0].ival;} break;
case 99:
# line 679 "a56.y"
{yyval.ival = 0x2 | yypvt[-0].ival;} break;
case 100:
# line 683 "a56.y"
{yyval.ival = 0;} break;
case 101:
# line 685 "a56.y"
{yyval.ival = 1;} break;
case 102:
# line 690 "a56.y"
{if(just_rep == 1)
				yyerror("instruction not allowed after REP");} break;
case 106:
# line 701 "a56.y"
{w0 |= 0x01D815 | yypvt[-2].ival << 8 | yypvt[-0].ival << 3;} break;
case 107:
# line 703 "a56.y"
{w0 |= 0x018040 | yypvt[-0].ival << 3;} break;
case 108:
# line 705 "a56.y"
{w0 |= 0x0000F8 | (n2int(yypvt[-2].n) & 0xFF) << 8 | yypvt[-0].ival;} break;
case 109:
# line 707 "a56.y"
{w0 |= 0x0000B8 | (n2int(yypvt[-2].n) & 0xFF) << 8 | yypvt[-0].ival;} break;
case 114:
# line 722 "a56.y"
{if(yypvt[-0].ival) {
				w0 |= 0x0BC0A0 | yypvt[-1].cond << 0;
			} else {
				w0 |= 0x0F0000 | yypvt[-1].cond << 12;
			}} break;
case 115:
# line 728 "a56.y"
{if(yypvt[-0].ival) {
				w0 |= 0x0AC0A0 | yypvt[-1].cond << 0;
			} else {
				w0 |= 0x0E0000 | yypvt[-1].cond << 12;
			}} break;
case 116:
# line 734 "a56.y"
{if(yypvt[-0].ival) {
				w0 |= 0x0BC080;
			} else {
				w0 |= 0x0D0000;
			}} break;
case 117:
# line 740 "a56.y"
{if(yypvt[-0].ival) {
				w0 |= 0x0AC080;
			} else {
				w0 |= 0x0C0000;
			}} break;
case 118:
# line 747 "a56.y"
{w0 |= 0x0B0020;} break;
case 119:
# line 749 "a56.y"
{w0 |= 0x0B0000;} break;
case 120:
# line 751 "a56.y"
{w0 |= 0x0A0020;} break;
case 121:
# line 753 "a56.y"
{w0 |= 0x0A0000;} break;
case 122:
# line 756 "a56.y"
{just_rep = 2;} break;
case 123:
# line 758 "a56.y"
{uses_w1++;} break;
case 124:
# line 760 "a56.y"
{w0 |= 0x00008C;} break;
case 125:
# line 762 "a56.y"
{w0 |= 0x000087;} break;
case 126:
# line 764 "a56.y"
{w0 |= 0x000086;} break;
case 127:
# line 766 "a56.y"
{w0 |= 0x000084;} break;
case 128:
# line 768 "a56.y"
{w0 |= 0x00000C;} break;
case 129:
# line 770 "a56.y"
{w0 |= 0x000006;} break;
case 130:
# line 772 "a56.y"
{w0 |= 0x000005;} break;
case 131:
# line 774 "a56.y"
{w0 |= 0x000004;} break;
case 132:
# line 776 "a56.y"
{w0 |= 0x000000;
			just_rep = 0;} break;
case 133:
# line 781 "a56.y"
{int ival = n2int(yypvt[-2].n);
			w0 |= 0x060080 | (ival & 0xFF) << 8 | (ival & 0xF00)>> 8;
			if(ival > 0xFFF && pass == 2) {
				yyerror("warning: immediate operand truncated");
			}
			w1 |= yypvt[-0].ival-1;} break;
case 134:
# line 788 "a56.y"
{w0 |= 0x06C000 | yypvt[-2].regs.r6 << 8;
			w1 |= yypvt[-0].ival-1;} break;
case 135:
# line 791 "a56.y"
{w0 |= 0x064000 | yypvt[-2].ival << 8 | yypvt[-3].ival << 6;
			w1 |= yypvt[-0].ival-1;} break;
case 136:
# line 794 "a56.y"
{w0 |= 0x060000 | (yypvt[-2].ival & 0x3F) << 8 | yypvt[-3].ival << 6;
			if(yypvt[-2].ival > 0x003F && pass == 2)
				yyerror("warning: address operand truncated");
			w1 |= yypvt[-0].ival-1;} break;
case 137:
# line 799 "a56.y"
{w0 |= 0x060000 | (yypvt[-2].ival & 0x3F) << 8 | yypvt[-3].ival << 6;
			if(yypvt[-2].ival > 0x003F && pass == 2)
				yyerror("warning: address operand truncated");
			w1 |= yypvt[-0].ival-1;} break;
case 138:
# line 807 "a56.y"
{int ival = n2int(yypvt[-0].n);
			w0 |= 0x0600A0 | (ival & 0xFF) << 8 | (ival & 0xF00)>> 8;
			if(ival > 0xFFF && pass == 2) {
				yyerror("warning: immediate operand truncated");
			}} break;
case 139:
# line 813 "a56.y"
{w0 |= 0x06C020 | yypvt[-0].regs.r6 << 8;} break;
case 140:
# line 815 "a56.y"
{w0 |= 0x064020 | yypvt[-1].ival << 6 | yypvt[-0].ival << 8;} break;
case 141:
# line 817 "a56.y"
{w0 |= 0x060020 | yypvt[-1].ival << 6 | (yypvt[-0].ival & 0x3F) << 8;
			if(yypvt[-0].ival > 0x003F && pass == 2)
				yyerror("warning: address operand truncated");
			} break;
case 142:
# line 822 "a56.y"
{w0 |= 0x060020 | yypvt[-1].ival << 6 | (yypvt[-0].ival & 0x3F) << 8;
			if(yypvt[-0].ival > 0x003F && pass == 2)
				yyerror("warning: address operand truncated");
			} break;
case 143:
# line 830 "a56.y"
{w0 |= yypvt[-5].ival << 0 | yypvt[-3].ival << 6;
			uses_w1++;
			w1 = yypvt[-0].ival;} break;
case 144:
# line 834 "a56.y"
{w0 |= 0x00C000 | yypvt[-4].ival << 0 | yypvt[-2].regs.r6 << 8;
			uses_w1++;
			w1 = yypvt[-0].ival;} break;
case 145:
# line 846 "a56.y"
{if(yypvt[-0].ival != -1) {	/* symbol defined */
				w0 |= (yypvt[-0].ival & 0x3F) << 8;
				if(yypvt[-0].ival >= 0xFFC0) {
					w0 |= 0x008080;
				} else {
					w0 |= 0x000080;
					if(yypvt[-0].ival > 0x003F && pass == 2)
						yyerror("warning: address operand truncated");
				}
			}} break;
case 146:
# line 857 "a56.y"
{if(yypvt[-0].ival != -1) {
				if(yypvt[-0].ival > 0x3F && pass == 2)
					yyerror("warning: address operand truncated");
				w0 |= 0x000080 | (yypvt[-0].ival & 0x3F) << 8;
			}} break;
case 147:
# line 863 "a56.y"
{if(yypvt[-0].ival != -1) {
				if(yypvt[-0].ival < 0xFFC0 && pass == 2)
					yyerror("warning: address operand truncated");
				w0 |= 0x008080 | (yypvt[-0].ival & 0x3F) << 8;
			}} break;
case 148:
# line 869 "a56.y"
{w0 |= 0x004080 | yypvt[-0].ival << 8;} break;
case 149:
# line 876 "a56.y"
{w0 |= 0x0B0020;} break;
case 150:
# line 878 "a56.y"
{w0 |= 0x0B0000;} break;
case 151:
# line 880 "a56.y"
{w0 |= 0x0A0020;} break;
case 152:
# line 882 "a56.y"
{w0 |= 0x0A0000;} break;
case 153:
# line 887 "a56.y"
{w0 |= yypvt[-3].ival << 0 | yypvt[-1].ival << 6;} break;
case 154:
# line 889 "a56.y"
{w0 |= 0x00C040 | yypvt[-2].ival << 0 | yypvt[-0].regs.r6 << 8;} break;
case 155:
# line 894 "a56.y"
{if(yypvt[-0].ival != -1) {
				w0 |= (yypvt[-0].ival & 0x3F) << 8 | 0x008000;
				if(yypvt[-0].ival < 0xFFC0 && pass == 2)
					yyerror("warning: address operand truncated");
			}} break;
case 156:
# line 900 "a56.y"
{if(yypvt[-0].ival != -1) {
				w0 |= (yypvt[-0].ival & 0x3F) << 8 | 0x000000;
				if(yypvt[-0].ival > 0x003F && pass == 2)
					yyerror("warning: address operand truncated");
			}} break;
case 157:
# line 906 "a56.y"
{w0 |= 0x004000;} break;
case 161:
# line 916 "a56.y"
{w0 |= 0x044010 | yypvt[-2].ival << 8 | yypvt[-0].regs.r4;} break;
case 162:
# line 918 "a56.y"
{w0 |= yypvt[-1].cond << 12;} break;
case 163:
# line 923 "a56.y"
{w0 |= 0x020000 | yypvt[-0].ival << 3;} break;
case 164:
# line 925 "a56.y"
{w0 |= 0x030000 | yypvt[-3].ival << 3 | yypvt[-2].ival << 8 | yypvt[-0].ival;} break;
case 165:
# line 929 "a56.y"
{if(yypvt[-2].regs.flags & R_AB && yypvt[-0].regs.flags & R_AB) {
				if(yypvt[-2].regs.ab == yypvt[-0].regs.ab) 
					yyerror("source and dest must be different");
				yyval.ival = yypvt[-0].regs.ab;
			} else if(yypvt[-2].regs.flags & R_XREG && yypvt[-0].regs.flags & R_AB) {
				yyval.ival = 0x8 | yypvt[-2].regs.xreg << 2 | yypvt[-0].regs.ab;
			} else if(yypvt[-2].regs.flags & R_YREG && yypvt[-0].regs.flags & R_AB) {
				yyval.ival = 0xA | yypvt[-2].regs.yreg << 2 | yypvt[-0].regs.ab;
			} else 
				yyerror("illegal TCC operands");
			} break;
case 166:
# line 943 "a56.y"
{if(yypvt[-2].regs.flags & R_XREG && yypvt[-0].regs.flags & R_AB) {
				yyval.ival = yypvt[-2].regs.xreg << 2 | yypvt[-0].regs.ab;
			} else if(yypvt[-2].regs.flags & R_YREG && yypvt[-0].regs.flags & R_AB) {
				yyval.ival = yypvt[-2].regs.yreg << 2 | 2 | yypvt[-0].regs.ab;
			}} break;
case 167:
# line 952 "a56.y"
{if(NOT (yypvt[-0].regs.flags & R_CTL_REG))
				yyerror("bad MOVEC target register");
			if(yypvt[-3].ival == 0) {
				w0 |= 0x05C020 | yypvt[-0].regs.ctl_reg;
			} else {
				w0 |= 0x05C060 | yypvt[-0].regs.ctl_reg;
			}} break;
case 168:
# line 960 "a56.y"
{if(NOT (yypvt[-3].regs.flags & R_CTL_REG))
				yyerror("bad MOVEC source register");
			if(yypvt[-1].ival == 0) {
				w0 |= 0x054020 | yypvt[-3].regs.ctl_reg;
			} else {
				w0 |= 0x054060 | yypvt[-3].regs.ctl_reg;
			}} break;
case 169:
# line 968 "a56.y"
{int ival = n2int(yypvt[-2].n);
			if(NOT (yypvt[-0].regs.flags & R_CTL_REG))
				yyerror("bad MOVEC target register");
			if(ival < 256 && NOT long_symbolic_expr) {
				w0 |= 0x0500A0 | (ival & 0xFF) << 8 | yypvt[-0].regs.ctl_reg; 
			} else {
				w0 |= 0x05C020 | 0x003400 | yypvt[-0].regs.ctl_reg;
				uses_w1++; w1 = ival & 0xFFFF;
			}} break;
case 170:
# line 978 "a56.y"
{if(yypvt[-3].ival == 0) {
				w0 |= 0x058020 | (yypvt[-2].ival & 0x3F) << 8 | yypvt[-0].regs.ctl_reg;
			} else {
				w0 |= 0x058060 | (yypvt[-2].ival & 0x3F) << 8 | yypvt[-0].regs.ctl_reg;
			}
			if(NOT (yypvt[-0].regs.flags & R_CTL_REG))
				yyerror("bad MOVEC target register");
			if(yypvt[-2].ival > 0x003F && pass == 2)
				yyerror("warning: address operand truncated");
			} break;
case 171:
# line 989 "a56.y"
{if(yypvt[-1].ival == 0) {
				w0 |= 0x050020 | (yypvt[-0].ival & 0x3F) << 8 | yypvt[-3].regs.ctl_reg;
			} else {
				w0 |= 0x050060 | (yypvt[-0].ival & 0x3F) << 8 | yypvt[-3].regs.ctl_reg;
			}
			if(NOT (yypvt[-3].regs.flags & R_CTL_REG))
				yyerror("bad MOVEC source register");
			if(yypvt[-0].ival > 0x003F && pass == 2)
				yyerror("warning: address operand truncated");
			} break;
case 172:
# line 1000 "a56.y"
{if(yypvt[-2].regs.flags & R_CTL_REG) {
				w0 |= 0x0440A0 | yypvt[-0].regs.r6 << 8 | yypvt[-2].regs.ctl_reg;
			} else if(yypvt[-0].regs.flags & R_CTL_REG) {
				w0 |= 0x04C0A0 | yypvt[-2].regs.r6 << 8 | yypvt[-0].regs.ctl_reg;
			} else if(yypvt[-2].regs.flags & yypvt[-0].regs.flags & R_CTL_REG) {
				/* bogus? $$$ */
	       			w0 |= 0x04C0A0 | (yypvt[-2].regs.ctl_reg | 0x20) << 8 | 
				yypvt[-0].regs.ctl_reg;
			} else {
				yyerror("MOVEC must reference a control reg");
			}} break;
case 173:
# line 1015 "a56.y"
{w0 |= 0x084080;
			switch(yypvt[-3].ea.pp << 1 | yypvt[-0].ea.pp) {
				case 0:	case 3:
					yyerror("illegal MOVEP; can't move EA to EA or IO to IO");
					break;
				case 1:	/* ea, pp */
					w0 |= yypvt[-1].ival << 16 | 1 << 15 | yypvt[-4].ival << 6 |
						(yypvt[-0].ea.ext & 0x3F);
					if(yypvt[-3].ea.mode == 0x003000) {
						w0 |= 0x003000;
						uses_w1++;
						w1 = yypvt[-3].ea.ext;
					} else {
						w0 |= yypvt[-3].ea.mode;
					}
					break;
				case 2:	/* pp, ea */
					w0 |= yypvt[-4].ival << 16 | 0 << 15 | yypvt[-1].ival << 6 |
						(yypvt[-3].ea.ext & 0x3F);
					if(yypvt[-0].ea.mode == 0x003000) {
						w0 |= 0x003000;
						uses_w1++;
						w1 = yypvt[-0].ea.ext;
					} else {
						w0 |= yypvt[-0].ea.mode;
					}
					break;
			}} break;
case 174:
# line 1044 "a56.y"
{w0 |= 0x084080;
			w0 |= yypvt[-1].ival << 16 | 1 << 15 | 0x34 << 8 | 
				(n2int(yypvt[-0].n) & 0x3F);
			uses_w1++;
			w1 = n2int(yypvt[-3].n);} break;
case 175:
# line 1050 "a56.y"
{w0 |= 0x084040;
			w0 |= yypvt[-1].ival << 16 | 1 << 15 | (n2int(yypvt[-0].n) & 0x3F);} break;
case 176:
# line 1053 "a56.y"
{w0 |= 0x084040;
			if(yypvt[-3].ea.mode != 0x003000 && yypvt[-3].ea.mode != 0)
				yyerror("illegal MOVEP");
			w0 |= yypvt[-4].ival << 16 | 0 << 15 | (yypvt[-3].ea.ext & 0x3F);} break;
case 177:
# line 1058 "a56.y"
{w0 |= 0x084000;
			w0 |= yypvt[-1].ival << 16 | 1 << 15 | yypvt[-3].regs.r6 << 8 | 
				(n2int(yypvt[-0].n) & 0x3F);} break;
case 178:
# line 1062 "a56.y"
{w0 |= 0x084000;
			if(!yypvt[-2].ea.pp)
				yyerror("illegal MOVEP");
			w0 |= yypvt[-3].ival << 16 | 0 << 15 | yypvt[-0].regs.r6 << 8 | (yypvt[-2].ea.ext & 0x3F);} break;
case 179:
# line 1070 "a56.y"
{if(yypvt[-0].ival != UNDEF && yypvt[-0].ival >= 0xFFC0) {
				/* defined symbol or constant, in i/o range */
				yyval.ea.pp = 1;
				yyval.ea.mode = 0;
			} else {
				/* either out of i/o range or symbol not */
				/* yet defined:  assume ea extension */
				yyval.ea.pp = 0;
				yyval.ea.mode = 0x003000;
			}
			yyval.ea.ext = yypvt[-0].ival;} break;
case 180:
# line 1082 "a56.y"
{yyval.ea.pp = 1;
			yyval.ea.mode = 0;
			if(yypvt[-0].ival < 0xFFC0 && pass == 2)
				yyerror("warning: address operand truncated");
			yyval.ea.ext = yypvt[-0].ival;} break;
case 181:
# line 1088 "a56.y"
{yyval.ea.pp = 0;
			yyval.ea.mode = yypvt[-0].ival << 8;
			yyval.ea.ext = yypvt[-0].ival;} break;
case 182:
# line 1095 "a56.y"
{w0 |= 0x070000 | 0 << 15 | yypvt[-3].regs.r6;} break;
case 183:
# line 1097 "a56.y"
{w0 |= 0x070000 | 1 << 15 | yypvt[-0].regs.r6;} break;
case 184:
# line 1104 "a56.y"
{int ival = n2int(yypvt[-0].n);
			yyval.ival = ival; if(ival > 0x17) 
				yyerror("%d: illegal bit number", ival);} break;
case 185:
# line 1110 "a56.y"
{yyval.ival = 0;} break;
case 186:
# line 1112 "a56.y"
{yyval.ival = 1;} break;
case 187:
# line 1118 "a56.y"
{w0 |= 0x004080;} break;
case 188:
# line 1120 "a56.y"
{w0 |= (yypvt[-0].ival & 0x3F) << 8;
			if(yypvt[-0].ival > 0x003F && pass == 2)
				yyerror("warning: address operand truncated");
			} break;
case 189:
# line 1127 "a56.y"
{yyval.ival = 1;} break;
case 190:
# line 1129 "a56.y"
{w0 |= yypvt[-0].ival & 0xFFF; yyval.ival = 0;
			if(yypvt[-0].ival > 0x0FFF && pass == 2)
				yyerror("warning: address operand truncated");
			} break;
case 191:
# line 1136 "a56.y"
{w0 |= 0x003000;
			uses_w1++;
			w1 |= yypvt[-0].ival;
			yyval.ival = 0x003000;} break;
case 192:
# line 1141 "a56.y"
{w0 |= yypvt[-0].ival << 8;
			yyval.ival = yypvt[-0].ival << 8;} break;
case 193:
# line 1147 "a56.y"
{yyval.ival = yypvt[-0].ival;} break;
case 194:
# line 1149 "a56.y"
{yyval.ival = 4 << 3 | yypvt[-1].ival;} break;
case 195:
# line 1151 "a56.y"
{yyval.ival = 5 << 3 | yypvt[-3].ival;
			if(yypvt[-3].ival != yypvt[-1].ival) yyerror("Rn and Nn must be same number");} break;
case 196:
# line 1154 "a56.y"
{yyval.ival = 7 << 3 | yypvt[-1].ival;} break;
case 197:
# line 1159 "a56.y"
{yyval.ival = 0 << 3 | yypvt[-3].ival;
			if(yypvt[-3].ival != yypvt[-0].ival) yyerror("Rn and Nn must be same number");} break;
case 198:
# line 1162 "a56.y"
{yyval.ival = 1 << 3 | yypvt[-3].ival;
			if(yypvt[-3].ival != yypvt[-0].ival) yyerror("Rn and Nn must be same number");} break;
case 199:
# line 1165 "a56.y"
{yyval.ival = 2 << 3 | yypvt[-2].ival;} break;
case 200:
# line 1167 "a56.y"
{yyval.ival = 3 << 3 | yypvt[-2].ival;} break;
case 201:
# line 1173 "a56.y"
{yyval.regs.r6 = yyval.regs.r5 = 0x04 | yypvt[-0].ival;
			yyval.regs.sdx = yypvt[-0].ival;
			yyval.regs.xreg = yypvt[-0].ival;
			yyval.regs.flags = R_R6|R_R5|R_XREG|R_SDX|R_SFRAC;} break;
case 202:
# line 1178 "a56.y"
{yyval.regs.r6 = yyval.regs.r5 = 0x06 | yypvt[-0].ival;
			yyval.regs.sdy = yypvt[-0].ival;
			yyval.regs.yreg = yypvt[-0].ival;
			yyval.regs.flags = R_R6|R_R5|R_SDY|R_YREG|R_SFRAC;} break;
case 203:
# line 1183 "a56.y"
{switch(yypvt[-0].ival) {
				case 0: 
					yyval.regs.r6 = yyval.regs.r5 = 0x08 | 0; 
					break;
				case 1: 
					yyval.regs.r6 = yyval.regs.r5 = 0x08 | 4; 
					break;
				case 2: 
					yyval.regs.r6 = yyval.regs.r5 = 0x08 | 2; 
					break;
			}
			yyval.regs.flags = R_R6|R_R5|R_UINT;} break;
case 204:
# line 1196 "a56.y"
{switch(yypvt[-0].ival) {
				case 0: 
					yyval.regs.r6 = yyval.regs.r5 = 0x08 | 1; break;
				case 1: 
					yyval.regs.r6 = yyval.regs.r5 = 0x08 | 5; break;
				case 2: 
					yyval.regs.r6 = yyval.regs.r5 = 0x08 | 3; break;
			}
			yyval.regs.flags = R_R6|R_R5|R_UINT;} break;
case 205:
# line 1206 "a56.y"
{yyval.regs.r6 = yyval.regs.r5 = 0x0E;
			yyval.regs.sdx = yyval.regs.sdy = 0x2;
			yyval.regs.ab = 0;
			yyval.regs.lsd = 4;
			yyval.regs.flags = R_R6|R_R5|R_SDX|R_SDY|R_AB|R_LSD|R_SFRAC;} break;
case 206:
# line 1212 "a56.y"
{yyval.regs.r6 = yyval.regs.r5 = 0x0F;
			yyval.regs.sdx = yyval.regs.sdy = 0x3;
			yyval.regs.ab = 1;
			yyval.regs.lsd = 5;
			yyval.regs.flags = R_R6|R_R5|R_SDX|R_SDY|R_AB|R_LSD|R_SFRAC;} break;
case 207:
# line 1218 "a56.y"
{yyval.regs.r6 = yyval.regs.r5 = 0x10 | yypvt[-0].ival;
			yyval.regs.r4 = 0x00 | yypvt[-0].ival;
			yyval.regs.flags = R_R6|R_R5|R_R4|R_UINT;} break;
case 208:
# line 1222 "a56.y"
{yyval.regs.r6 = yyval.regs.r5 = 0x18 | yypvt[-0].ival;
			yyval.regs.r4 = 0x08 | yypvt[-0].ival;
			yyval.regs.flags = R_R6|R_R5|R_R4|R_UINT;} break;
case 209:
# line 1226 "a56.y"
{yyval.regs.r6 = 0x20 | yypvt[-0].ival;
			yyval.regs.r5 = -1;
			yyval.regs.ctl_reg = yypvt[-0].ival;
			yyval.regs.flags = R_R6|R_R5|R_CTL_REG|R_UINT;} break;
case 210:
# line 1231 "a56.y"
{yyval.regs.r6 = 0x38 | yypvt[-0].ival;
			yyval.regs.r5 = -1;
			yyval.regs.ctl_reg = 0x18 | yypvt[-0].ival;
			yyval.regs.flags = R_R6|R_R5|R_CTL_REG|R_UINT;} break;
case 211:
# line 1236 "a56.y"
{yyval.regs.lsd  = 0;
			yyval.regs.flags = R_LSD;} break;
case 212:
# line 1239 "a56.y"
{yyval.regs.lsd = 1;
			yyval.regs.flags = R_LSD;} break;
case 213:
# line 1242 "a56.y"
{yyval.regs.lsd = 2;
			yyval.regs.flags = R_LSD;} break;
case 214:
# line 1245 "a56.y"
{yyval.regs.lsd = 3;
			yyval.regs.flags = R_LSD;} break;
case 215:
# line 1248 "a56.y"
{yyval.regs.lsd = 6;
			yyval.regs.flags = R_LSD;} break;
case 216:
# line 1251 "a56.y"
{yyval.regs.lsd = 7;
			yyval.regs.flags = R_LSD;} break;
case 217:
# line 1257 "a56.y"
{yyval.ival = 1;} break;
case 218:
# line 1259 "a56.y"
{yyval.ival = 2;} break;
case 219:
# line 1261 "a56.y"
{yyval.ival = 3;} break;
case 220:
# line 1263 "a56.y"
{yyval.ival = 4;} break;
case 221:
# line 1265 "a56.y"
{yyval.ival = 5;} break;
case 222:
# line 1267 "a56.y"
{yyval.ival = 6;} break;
case 223:
# line 1269 "a56.y"
{yyval.ival = 7;} break;
case 224:
# line 1274 "a56.y"
{yyval.ival = 0;} break;
case 225:
# line 1276 "a56.y"
{yyval.ival = 1;} break;
case 226:
# line 1278 "a56.y"
{yyval.ival = 2;} break;
case 235:
# line 1295 "a56.y"
{int ival = n2int(yypvt[-2].n);
			int frac = n2frac(yypvt[-2].n);
			int value;
			BOOL shortform = FALSE;
			if(yypvt[-0].regs.flags & R_CTL_REG) {
				yyerror("please use MOVEC for control register moves");
				break;
			}
			if((yypvt[-0].regs.flags & R_SFRAC) && yypvt[-2].n.type == FLT) {
				if((frac & 0xFFFF) == 0 && 
					NOT long_symbolic_expr) {
					value = frac >> 16;
					shortform++;
				} else {
					value = frac;
				}
			} else {
				if(ival <= 0xFF && ival >= -0xFF && NOT long_symbolic_expr) {
					value = ival;
					shortform++;
				} else {
					value = ival;
				}
			}

			if(shortform) {
				w0 |= 0x200000 | (value & 0xFF) << 8 |
					yypvt[-0].regs.r5 << 16;
			} else {
				w0 |= 0x400000 | 0x00F400 |
					(yypvt[-0].regs.r5 >> 3 & 3) << 20 | 
					(yypvt[-0].regs.r5 & 7) << 16;
				uses_w1++; w1 = value;
			}} break;
case 236:
# line 1332 "a56.y"
{
				if(yypvt[-0].regs.flags & R_CTL_REG) {
					yyerror("please use MOVEC for control register moves");
					break;
				}
				if(yypvt[-2].regs.flags & R_R5 & yypvt[-0].regs.flags) 
					w0 |= 0x200000 | yypvt[-0].regs.r5 << 8 | yypvt[-2].regs.r5 << 13;
				else
					yyerror("illegal R move");
			} break;
case 237:
# line 1345 "a56.y"
{w0 |= 0x204000 | yypvt[-0].ival << 8;} break;
case 238:
# line 1350 "a56.y"
{w0 |= 0x40C000 | yypvt[-3].ival << 19;
			if(yypvt[-0].regs.flags & R_CTL_REG) {
				yyerror("please use MOVEC for control register moves");
				break;
			}
			w0 |= (yypvt[-0].regs.r5 >> 3 & 3) << 20 | (yypvt[-0].regs.r5 & 7) << 16;} break;
case 239:
# line 1357 "a56.y"
{w0 |= 0x408000 | yypvt[-3].ival << 19 | (yypvt[-2].ival & 0x3F) << 8;
			if(yypvt[-0].regs.flags & R_CTL_REG) {
				yyerror("please use MOVEC for control register moves");
				break;
			}
			if(yypvt[-2].ival > 0x003F && pass == 2)
				yyerror("warning: address operand truncated");
			w0 |= (yypvt[-0].regs.r5>> 3 & 3) << 20 | (yypvt[-0].regs.r5 & 7) << 16;} break;
case 240:
# line 1366 "a56.y"
{w0 |= 0x404000 | yypvt[-1].ival << 19;
			if(yypvt[-3].regs.flags & R_CTL_REG) {
				yyerror("please use MOVEC for control register moves");
				break;
			}
			w0 |= (yypvt[-3].regs.r5 >> 3 & 3) << 20 | (yypvt[-3].regs.r5 & 7) << 16;} break;
case 241:
# line 1373 "a56.y"
{w0 |= 0x400000 | yypvt[-1].ival << 19 | (yypvt[-0].ival & 0x3F) << 8;
			if(yypvt[-3].regs.flags & R_CTL_REG) {
				yyerror("please use MOVEC for control register moves");
				break;
			}
			if(yypvt[-0].ival > 0x003F && pass == 2)
				yyerror("warning: address operand truncated");
			w0 |= (yypvt[-3].regs.r5 >> 3 & 3) << 20 | (yypvt[-3].regs.r5 & 7) << 16;} break;
case 242:
# line 1382 "a56.y"
{w0 |= 0x400000 | 0x00F400 | (yypvt[-0].regs.r5 >> 3 & 3) << 20 |
			    (yypvt[-0].regs.r5 & 7) << 16;
			if(yypvt[-0].regs.flags & R_CTL_REG) {
				yyerror("please use MOVEC for control register moves");
				break;
			}
			uses_w1++; w1 = n2frac(yypvt[-2].n);
			} break;
case 243:
# line 1393 "a56.y"
{if(yypvt[-6].ival == 0 && yypvt[-2].regs.flags & R_AB) {
				w0 |= 0x108000 | yypvt[-3].regs.sdx << 18 | yypvt[-2].regs.ab << 17 |
					yypvt[-0].ival << 16;
			} else {
				yyerror("illegal X:R move");
			}} break;
case 244:
# line 1400 "a56.y"
{if(yypvt[-2].regs.flags & R_AB) {
				w0 |= 0x10B400 | yypvt[-3].regs.sdx << 18 | yypvt[-2].regs.ab << 17 |
					yypvt[-0].ival << 16;
				uses_w1++;
				w1 |= n2frac(yypvt[-5].n) & 0xFFFFFF;
			} else {
				yyerror("illegal X:R move");
			}} break;
case 245:
# line 1409 "a56.y"
{if(yypvt[-6].regs.flags & R_SDX && yypvt[-4].ival == 0 && yypvt[-2].regs.flags & R_AB &&
				yypvt[-0].regs.flags & R_YREG) {
				w0 |= 0x100000 | yypvt[-6].regs.sdx << 18 | yypvt[-2].regs.ab << 17 |
					yypvt[-0].regs.yreg << 16;
			} else if(yypvt[-6].regs.flags & R_AB && yypvt[-4].ival == 0 && 
				yypvt[-2].regs.flags & R_XREG && yypvt[-0].regs.flags & R_AB) {
				if(yypvt[-2].regs.xreg != 0) yyerror("must use X0");
				if(yypvt[-6].regs.ab == 0 && yypvt[-0].regs.ab == 0)
					w0 |= 0x080000;
				else if(yypvt[-6].regs.ab == 1 && yypvt[-0].regs.ab == 1)
					w0 |= 0x090000;
				else
					yyerror("illegal X:R move");
			} else {
				yyerror("illegal X:R move");
			}} break;
case 246:
# line 1428 "a56.y"
{if(yypvt[-4].regs.flags & R_XREG && yypvt[-0].regs.flags & (R_YREG|R_AB)) {
				w0 |= 0x10C000 | yypvt[-6].regs.ab << 19 | yypvt[-4].regs.xreg << 18 |
					yypvt[-0].regs.sdy << 16;
			} else {
				yyerror("illegal R:Y move");
			}} break;
case 247:
# line 1435 "a56.y"
{if(yypvt[-3].regs.flags & R_XREG && yypvt[-0].regs.flags & (R_YREG|R_AB)) {
				w0 |= 0x10F400 | yypvt[-5].regs.ab << 19 | yypvt[-3].regs.xreg << 18 |
					yypvt[-0].regs.sdy << 16;
				uses_w1++;
				w1 |= n2frac(yypvt[-2].n) & 0xFFFFFF;
			} else {
				yyerror("illegal R:Y move");
			}} break;
case 248:
# line 1444 "a56.y"
{if(yypvt[-6].regs.flags & R_AB && yypvt[-4].regs.flags & R_XREG) {
				w0 |= 0x104000 | yypvt[-6].regs.ab << 19 | yypvt[-4].regs.xreg << 18 |
				yypvt[-3].regs.sdy << 16;
			} else if (yypvt[-6].regs.flags & R_YREG && yypvt[-4].regs.flags & R_AB &&
				yypvt[-3].regs.flags & R_AB) {
				if(yypvt[-6].regs.yreg != 0) yyerror("must use Y0");
				if(yypvt[-4].regs.ab == 0 && yypvt[-3].regs.ab == 0)
					w0 |= 0x088000;
				else if(yypvt[-4].regs.ab == 1 && yypvt[-3].regs.ab == 1)
					w0 |= 0x098000;
				else
					yyerror("illegal R:Y move");
			} else {
				yyerror("illegal R:Y move");
			}} break;
case 249:
# line 1462 "a56.y"
{if(yypvt[-0].regs.flags & R_CTL_REG) {
				yyerror("please use MOVEC for control register moves");
				break;
			}
			w0 |= 0x40C000 | (yypvt[-0].regs.lsd & 3) << 16 | (yypvt[-0].regs.lsd >> 2) << 19;} break;
case 250:
# line 1468 "a56.y"
{if(yypvt[-3].regs.flags & R_CTL_REG) {
				yyerror("please use MOVEC for control register moves");
				break;
			}
			w0 |= 0x404000 | (yypvt[-3].regs.lsd & 3) << 16 | (yypvt[-3].regs.lsd >> 2) << 19;} break;
case 251:
# line 1474 "a56.y"
{w0 |= 0x408000 | (yypvt[-0].regs.lsd & 3) << 16 | (yypvt[-0].regs.lsd >> 2) << 19;
			if(yypvt[-0].regs.flags & R_CTL_REG) {
				yyerror("please use MOVEC for control register moves");
				break;
			}
			if(yypvt[-2].ival > 0x003F && pass == 2)
				yyerror("warning: address operand truncated");
			w0 |= (yypvt[-2].ival & 0x3F) << 8;} break;
case 252:
# line 1483 "a56.y"
{w0 |= 0x400000 | (yypvt[-3].regs.lsd & 3) << 16 | (yypvt[-3].regs.lsd >> 2) << 19;
			if(yypvt[-3].regs.flags & R_CTL_REG) {
				yyerror("please use MOVEC for control register moves");
				break;
			}
			if(yypvt[-0].ival > 0x003F && pass == 2)
				yyerror("warning: address operand truncated");
			w0 |= (yypvt[-0].ival & 0x3F) << 8;} break;
case 253:
# line 1494 "a56.y"
{int eax = yypvt[-6].ival, eay = yypvt[-2].ival,
			     regx = (yypvt[-4].regs.flags & R_AB) ? yypvt[-4].regs.ab | 2 : yypvt[-4].regs.xreg,
			     regy = (yypvt[-0].regs.flags & R_AB) ? yypvt[-0].regs.ab | 2 : yypvt[-0].regs.yreg;
			if((eax & 0x400) == (eay & 0x400))
				yyerror("registers must be in opposite halves");
			if(!(yypvt[-4].regs.flags & (R_AB | R_XREG)))
				yyerror("invalid X move register");
			if(!(yypvt[-0].regs.flags & (R_AB | R_YREG)))
				yyerror("invalid Y move register");
			if(yypvt[-4].regs.flags & R_AB &&
			   yypvt[-0].regs.flags & R_AB &&
			   yypvt[-4].regs.ab == yypvt[-0].regs.ab)
				yyerror("duplicate destination register");
			w0 = w0 & 0xFF | 0xC08000;	/* both write */
			w0 |= eax & 0x1f00 | (eay & 0x300) << 5 | (eay & 0x1800) << 9 | regx << 18 | regy << 16;} break;
case 254:
# line 1510 "a56.y"
{int eax = yypvt[-6].ival, eay = yypvt[-0].ival,
			     regx = (yypvt[-4].regs.flags & R_AB) ? yypvt[-4].regs.ab | 2 : yypvt[-4].regs.xreg,
			     regy = (yypvt[-3].regs.flags & R_AB) ? yypvt[-3].regs.ab | 2 : yypvt[-3].regs.yreg;
			if((eax & 0x400) == (eay & 0x400))
				yyerror("registers must be in opposite halves");
			if(!(yypvt[-4].regs.flags & (R_AB | R_XREG)))
				yyerror("invalid X move register");
			if(!(yypvt[-3].regs.flags & (R_AB | R_YREG)))
				yyerror("invalid Y move register");
			w0 = w0 & 0xFF | 0x808000;	/* X:write, Y:read */
			w0 |= eax & 0x1f00 | (eay & 0x300) << 5 | (eay & 0x1800) << 9 | regx << 18 | regy << 16;} break;
case 255:
# line 1522 "a56.y"
{int eax = yypvt[-4].ival, eay = yypvt[-2].ival,
			     regx = (yypvt[-7].regs.flags & R_AB) ? yypvt[-7].regs.ab | 2 : yypvt[-7].regs.xreg,
			     regy = (yypvt[-0].regs.flags & R_AB) ? yypvt[-0].regs.ab | 2 : yypvt[-0].regs.yreg;
			if((eax & 0x400) == (eay & 0x400))
				yyerror("registers must be in opposite halves");
			if(!(yypvt[-7].regs.flags & (R_AB | R_XREG)))
				yyerror("invalid X move register");
			if(!(yypvt[-0].regs.flags & (R_AB | R_YREG)))
				yyerror("invalid Y move register");
	      		w0 = w0 & 0xFF | 0xC00000;	/* X:read, Y:write */
			w0 |= eax & 0x1f00 | (eay & 0x300) << 5 | (eay & 0x1800) << 9 | regx << 18 | regy << 16;} break;
case 256:
# line 1534 "a56.y"
{int eax = yypvt[-4].ival, eay = yypvt[-0].ival,
			     regx = (yypvt[-7].regs.flags & R_AB) ? yypvt[-7].regs.ab | 2 : yypvt[-7].regs.xreg,
			     regy = (yypvt[-3].regs.flags & R_AB) ? yypvt[-3].regs.ab | 2 : yypvt[-3].regs.yreg;
			if((eax & 0x400) == (eay & 0x400))
				yyerror("registers must be in opposite halves");
			if(!(yypvt[-7].regs.flags & (R_AB | R_XREG)))
				yyerror("invalid X move register");
			if(!(yypvt[-3].regs.flags & (R_AB | R_YREG)))
				yyerror("invalid Y move register");
	      		w0 = w0 & 0xFF | 0x800000;	/* both read */
			w0 |= eax & 0x1f00 | (eay & 0x300) << 5 | (eay & 0x1800) << 9 | regx << 18 | regy << 16;} break;
case 257:
# line 1550 "a56.y"
{yyval.n = yypvt[-0].n;} break;
case 258:
# line 1552 "a56.y"
{yyval.n = yypvt[-0].n;} break;
case 259:
# line 1554 "a56.y"
{yyval.n = yypvt[-0].n;} break;
case 260:
# line 1558 "a56.y"
{yyval.n = yypvt[-0].n;} break;
case 261:
# line 1560 "a56.y"
{yyval.n.val.i = n2int(yypvt[-0].n) & 0xFF;
			yyval.n.type = INT;
			long_symbolic_expr = FALSE;} break;
case 262:
# line 1566 "a56.y"
{yyval.n = yypvt[-0].n;} break;
case 263:
# line 1571 "a56.y"
{yyval.ival = n2int(yypvt[-0].n);} break;
case 264:
# line 1576 "a56.y"
{yyval.ival = n2int(yypvt[-0].n);} break;
case 265:
# line 1581 "a56.y"
{yyval.ival = n2int(yypvt[-0].n);} break;
case 266:
# line 1586 "a56.y"
{yyval.n = yypvt[-0].n;} break;
case 267:
# line 1588 "a56.y"
{yyval.n = sym_ref(yypvt[-0].sval); free(yypvt[-0].sval);} break;
case 268:
# line 1590 "a56.y"
{yyval.n.type = INT; yyval.n.val.i = yypvt[-0].ival;} break;
case 269:
# line 1595 "a56.y"
{yyval.n = yypvt[-0].n;} break;
case 270:
# line 1597 "a56.y"
{yyval.n = sym_ref(yypvt[-0].sval); free(yypvt[-0].sval); long_symbolic_expr++;} break;
case 271:
# line 1599 "a56.y"
{yyval.n.type = INT; yyval.n.val.i = yypvt[-0].cval & 0xFFFFFF;} break;
case 272:
# line 1601 "a56.y"
{yyval.n.type = INT; yyval.n.val.i = pc;} break;
case 273:
# line 1606 "a56.y"
{yyval.n.type = INT; 
				if(yypvt[-1].n.type == INT)
					yyval.n.val.i = yypvt[-1].n.val.i;
				else
					yyval.n.val.i = yypvt[-1].n.val.f;
			} break;
case 274:
# line 1613 "a56.y"
{yyval.n = binary_op(yypvt[-2].n, '|', yypvt[-0].n);} break;
case 275:
# line 1615 "a56.y"
{yyval.n = binary_op(yypvt[-2].n, '^', yypvt[-0].n);} break;
case 276:
# line 1617 "a56.y"
{yyval.n = binary_op(yypvt[-2].n, '&', yypvt[-0].n);} break;
case 277:
# line 1619 "a56.y"
{yyval.n = binary_op(yypvt[-2].n, SHR, yypvt[-0].n);} break;
case 278:
# line 1621 "a56.y"
{yyval.n = binary_op(yypvt[-2].n, SHL, yypvt[-0].n);} break;
case 279:
# line 1623 "a56.y"
{yyval.n = binary_op(yypvt[-2].n, '-', yypvt[-0].n);} break;
case 280:
# line 1625 "a56.y"
{yyval.n = binary_op(yypvt[-2].n, '+', yypvt[-0].n);} break;
case 281:
# line 1627 "a56.y"
{yyval.n = binary_op(yypvt[-2].n, '%', yypvt[-0].n);} break;
case 282:
# line 1629 "a56.y"
{yyval.n = binary_op(yypvt[-2].n, '/', yypvt[-0].n);} break;
case 283:
# line 1631 "a56.y"
{yyval.n = binary_op(yypvt[-2].n, '*', yypvt[-0].n);} break;
case 284:
# line 1633 "a56.y"
{yyval.n = unary_op('-', yypvt[-0].n);} break;
case 285:
# line 1635 "a56.y"
{yyval.n = unary_op('~', yypvt[-0].n);} break;
case 286:
# line 1637 "a56.y"
{yyval.n = yypvt[-1].n;} break;
case 287:
# line 1639 "a56.y"
{yyval.n = yypvt[-0].n;} break;
		}
		goto yystack;  /* stack new state and value */

	}
