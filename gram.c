
# line 2 "a56.y"
/*******************************************************
 *
 *  a56 - a DSP56001 assembler
 *
 *  Written by Quinn C. Jensen
 *  July 1990
 *  jensenq@npd.novell.com (or jensenq@qcj.icon.com)
 *
 *******************************************************\

/*
 * Copyright (C) 1990, 1991 Quinn C. Jensen
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
 */

#include "a56.h"
unsigned int w0, w1, pc;
extern BOOL list_on;
BOOL uses_w1;
int just_rep = 0;
extern char inline[];
char *spaces(), *luntab();
char segs[] = "PXYL";
int seg;
int substatement = 0;

#define R_R6			0x0001
#define R_R5			0x0002
#define R_R4			0x0004
#define R_DATA_ALU_ACCUM	0x0008
#define R_CTL_REG		0x0010
#define R_FUNKY_CTL_REG		0x0020
#define R_SDX			0x0040
#define R_SDY			0x0080
#define R_LSD			0x0100
#define R_AB			0x0200
#define R_XREG			0x0400
#define R_YREG			0x0800

# line 58 "a56.y"
typedef union  {
	int ival;	/* integer value */
	char *sval;	/* string */
	int cval;	/* character */
	char cond;	/* condition */
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
# define COMMENT 276
# define XMEM 277
# define YMEM 278
# define LMEM 279
# define PMEM 280
# define AAAA 281
# define A10 282
# define BBBB 283
# define B10 284
# define AABB 285
# define BBAA 286
# define XXXX 287
# define YYYY 288
# define SR 289
# define MR 290
# define CCR 291
# define OMR 292
# define SP 293
# define SSH 294
# define SSL 295
# define LA 296
# define LC 297
# define EOL 298
# define EOS 299
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
# define OP_END 364
# define OP_PAGE 365
# define OP_PSECT 366
# define SHL 367
# define SHR 368
#define yyclearin yychar = -1
#define yyerrok yyerrflag = 0
extern int yychar;
extern short yyerrflag;
#ifndef YYMAXDEPTH
#define YYMAXDEPTH 150
#endif
YYSTYPE yylval, yyval;
# define YYERRCODE 256

# line 1421 "a56.y"


#include <stdio.h>

int yydebug;

extern struct {int n; char *name;} tok_tab[];
extern int n_tok;

char *tok_print(tok)
int tok;
{
    int i;
    static char buf[32];

    if(tok < 256) {
	sprintf(buf, "'%c'", tok);
	return(buf);
    } else {
	for(i = 0; i < n_tok; i++) {
	    if(tok == tok_tab[i].n)
		return(tok_tab[i].name);
	}
    }
    return("<unknown>");
}

yyerror(s)
char *s;
{
    extern int error;
    error++;

    if(pass == 2) {
	fprintf(stderr, "%s: line %d: %s (tok=%s)\n", curfile, curline,
	    s, tok_print(yychar));
	fprintf(stderr, "%s\n", inline); 
	printf("%s: line %d: %s (tok=%s)\n", curfile, curline,
	    s, tok_print(yychar));
	printf("%s\n", inline); 
    }
}

char *luntab(s)
char *s;
{
    static char buf[256];

    strcpy(buf, s);

    untab(buf);
    return(buf);
}short yyexca[] ={
-1, 1,
	0, -1,
	270, 29,
	271, 29,
	272, 29,
	298, 6,
	299, 6,
	300, 29,
	301, 29,
	302, 29,
	303, 29,
	304, 29,
	305, 29,
	306, 29,
	307, 29,
	308, 29,
	309, 29,
	310, 29,
	311, 29,
	312, 29,
	313, 29,
	314, 29,
	315, 29,
	316, 29,
	317, 29,
	318, 29,
	319, 29,
	320, 29,
	321, 29,
	322, 29,
	323, 29,
	324, 29,
	325, 29,
	326, 29,
	327, 29,
	328, 29,
	329, 29,
	330, 29,
	331, 29,
	332, 29,
	333, 29,
	334, 29,
	335, 29,
	336, 29,
	337, 29,
	338, 29,
	339, 29,
	340, 29,
	341, 29,
	342, 29,
	343, 29,
	344, 29,
	345, 29,
	347, 29,
	348, 29,
	349, 29,
	350, 29,
	351, 29,
	352, 29,
	353, 29,
	354, 29,
	355, 29,
	356, 29,
	357, 29,
	358, 29,
	359, 29,
	360, 29,
	363, 29,
	-2, 0,
-1, 8,
	298, 32,
	299, 32,
	-2, 28,
	};
# define YYNPROD 283
# define YYLAST 1359
short yyact[]={

 129, 216, 217, 330, 332, 427, 393, 472, 114, 339,
  17, 214,  14,  15, 222, 390, 220, 171, 172, 223,
 100, 384, 219, 128, 394, 416,  95, 428, 429, 430,
 229, 210, 211, 168, 218, 167, 199, 389, 200, 169,
 170, 182, 109, 183, 280, 275, 222, 383, 220, 282,
 279, 223, 278, 126, 281, 509, 280, 275,  17, 514,
 104, 282, 279,  94, 278, 432, 281, 236, 240, 280,
 275, 515,   4, 367, 282, 279, 504, 278, 215, 281,
 134, 135, 251, 253, 257, 483, 262, 107, 265,   8,
 237, 241,   5, 453, 452,  93, 347, 235, 239, 296,
 111, 274, 263, 280, 275, 248, 496, 256, 282, 279,
 354, 278, 249, 281, 258, 495, 268, 454, 270, 266,
 267, 269,  97,  98,  99,  96, 274, 450, 451, 177,
 178, 273, 111, 283, 284, 285, 280, 275, 298, 299,
 287, 282, 279, 291, 278, 176, 281, 175, 227, 101,
 102, 103, 193, 194, 197, 293, 273, 101, 102, 103,
 274, 209,  12, 280, 275, 474, 213, 521, 282, 279,
 192, 278, 221, 281, 448, 242, 447, 516,  10, 280,
  13,  11,   9, 510, 282, 279, 161, 278, 162, 281,
 273, 228, 280, 274, 507, 127, 505, 282, 503, 497,
 498, 492, 281, 491, 201, 202, 203, 288, 487, 461,
 292, 434, 231, 232, 233, 423, 419, 195, 196, 212,
 274, 204, 418, 273, 417, 414, 224, 225, 226, 112,
 411, 101, 102, 103, 110, 408, 407, 406, 188, 322,
 321, 316, 382, 328, 326, 285, 283, 115, 381, 116,
 331, 333, 380, 293, 244, 245, 246, 259, 376, 399,
 346, 400, 336, 101, 102, 103, 340, 375, 342, 243,
 243, 243, 243, 370, 369, 206, 207, 208, 352, 115,
 355, 116, 190, 191, 351, 350, 349, 368, 348, 345,
 371, 374, 353, 344, 338, 109, 357, 358, 359, 360,
 361, 362, 363, 364, 365, 366, 337, 335, 329, 198,
 325, 324, 320, 372, 315, 111, 323, 377, 378, 314,
 327, 403, 313, 312, 311, 310, 404, 405, 309, 308,
 410, 307, 306, 341, 305, 343, 304, 303, 302, 301,
 300, 334, 285, 290, 402, 421, 422, 289, 286, 424,
 272, 426, 271, 409, 455, 379, 133, 412, 431, 413,
 356, 415, 205, 205, 205, 205, 230, 125, 420, 435,
 436, 437, 440, 334, 277, 276, 445, 446, 124, 123,
 122, 433, 166, 121, 441, 443, 277, 276, 280, 275,
 120, 119, 118, 282, 279, 260, 278, 255, 281, 277,
 276, 252, 439, 174, 457, 460, 247, 459, 462, 463,
 464, 222, 471, 220, 466, 173, 223, 238, 478, 479,
 480, 468, 473, 476, 473, 132, 477, 234, 340, 159,
 133, 218, 481, 277, 276, 469, 189, 488, 475, 179,
 180,  89, 493, 486,  88,  26, 101, 102, 103,  25,
 490, 442, 444, 401,  24, 484, 485,  23, 163, 164,
 165,  16, 115, 499, 116, 117, 277, 276, 230,  22,
  92, 331, 333, 500, 181, 184, 185, 186, 187,  21,
 105, 458, 501, 502, 473,  20, 106,  18, 467,   7,
   6,   3, 508, 277, 276, 341,   2, 111,   1, 482,
 512, 506, 261,  59,  58,  85, 511, 264, 517, 160,
 297, 519, 513, 130, 145, 465, 456, 523, 113,   0,
   0, 518, 525,   0,   0,   0,   0, 522,   0,   0,
   0,   0, 524,  50,  56,  32,  38,  42,  52,  48,
  39,  35,  36,  87,  29,  30,  27,  28,  54,  86,
  37,  57,  31,  40,  44,  34,  43,  45,  91,  47,
  53,  49,  41,  46,  90,  55,  51,  80,  79,  78,
  77,  67,  68,  84,  33,  83,  82,  81,  74,   0,
  61,  65,  64,  60,  63,  62,  76,  66,  71,  75,
  72,  69,  73,  70, 230, 222,  19, 220,   0,   0,
 223,   0,   0,   0,   0,   0,   0,   0,   0,   0,
   0,   0, 385, 386, 387, 388,   0,   0, 391, 392,
   0,   0, 395, 396, 397, 398,   0,   0, 101, 102,
 103,   0, 230,   0,   0,   0,   0,   0,   0,   0,
   0,   0,   0,   0, 115,   0, 116,   0,   0,   0,
 138, 139, 144, 143, 142, 136, 137,   0,   0, 425,
   0,   0,   0,   0,   0,   0,   0, 134, 135, 131,
   0, 140, 146, 141, 147, 150, 151, 148, 149, 152,
   0, 111, 153, 154, 155, 156, 157, 158,   0,   0,
 449,   0,   0, 138, 139, 144, 143, 142, 136, 137,
   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,
 134, 135,   0, 250, 140, 146, 141, 147, 150, 151,
 148, 149, 152,   0,   0, 153, 154, 155, 156, 157,
 158, 138, 139, 144, 143, 142, 136, 137,   0,   0,
   0,   0,   0,   0,   0,   0,   0,   0, 134, 135,
   0, 470, 140, 146, 141, 147, 150, 151, 148, 149,
 152,   0,   0, 153, 154, 155, 156, 157, 158, 138,
 139, 144, 143, 142, 136, 137,   0,   0,   0,   0,
   0,   0,   0,   0,   0,   0, 134, 135, 373,   0,
 140, 146, 141, 147, 150, 151, 148, 149, 152,   0,
   0, 153, 154, 155, 156, 157, 158,   0,   0,   0,
   0,   0, 101, 102, 103,   0,   0,   0,   0, 138,
 139, 144, 143, 142, 136, 137,   0,   0, 115,   0,
 116,   0,   0,   0,   0,   0, 134, 135,   0,   0,
 140, 146, 141, 147, 150, 151, 148, 149, 152,   0,
   0, 153, 154, 155, 156, 157, 158, 138, 139, 144,
 143, 142, 136, 137,   0,   0,   0,   0,   0,   0,
   0,   0,   0,   0,   0, 438,   0,   0, 140, 146,
 141, 147, 150, 151, 148, 149, 152,   0,   0, 153,
 154, 155, 156, 157, 158, 138, 139, 144, 143, 142,
 136, 137,   0,   0,   0,   0,   0,   0,   0,   0,
   0,   0, 134, 135,   0,   0, 140, 146, 141, 147,
 150, 151, 148, 149, 152,   0,   0, 153, 154, 155,
 156, 157, 158, 138, 139, 144, 143, 142, 136, 137,
 112,   0, 220, 112,   0, 110,   0,   0, 110,   0,
   0, 520,   0,   0, 140, 146, 141, 147, 150, 151,
 148, 149, 152,   0,   0, 153, 154, 155, 156, 157,
 158, 138, 139, 144, 143, 142, 136, 137, 112,   0,
   0,   0,   0, 110,   0,   0,   0,   0,   0, 494,
   0,   0, 140, 146, 141, 147, 150, 151, 148, 149,
 152,   0,   0, 153, 154, 155, 156, 157, 158, 138,
 139, 144, 143, 142, 136, 137, 112,   0, 317, 112,
   0, 110,   0,   0, 110,   0, 111, 489,   0, 111,
 140, 146, 141, 147, 150, 151, 148, 149, 152, 294,
   0, 153, 154, 155, 156, 157, 158, 138, 139, 144,
 143, 142, 136, 137, 112,   0,   0,   0, 319, 110,
   0,   0,   0, 110, 111,   0,   0, 254, 140, 146,
 141, 147, 150, 151, 148, 149, 152,   0,   0, 153,
 154, 155, 156, 157, 158, 138, 139, 144, 143, 142,
 136, 137,   0,   0,   0,   0,   0,   0,   0,   0,
   0,   0, 111,   0,   0, 111, 140, 146, 141, 147,
 150, 151, 148, 149, 152,   0,   0, 153, 154, 155,
 156, 157, 158, 112,   0,   0,   0,   0, 110,   0,
   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,
 111,   0,   0, 294, 111, 295,   0,   0,   0,   0,
   0,   0,   0,   0,   0,   0,   0, 101, 102, 103,
 101, 102, 103,   0,   0,   0,   0,   0,   0,   0,
   0,   0,   0, 115,   0, 116, 115, 108, 116,   0,
   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,
   0,   0,   0,   0,   0, 101, 102, 103,   0,   0,
   0,   0, 318,   0,   0,   0,   0,   0,   0, 111,
   0, 115,   0, 116,   0,   0,   0,   0,   0,   0,
   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,
   0,   0,   0, 101, 102, 103, 101, 102, 103,   0,
   0,   0,   0,   0,   0,   0,   0,   0,   0, 115,
   0, 116, 115,   0, 116,   0,   0,   0,   0,   0,
   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,
   0, 101, 102, 103,   0, 101, 102, 103,   0,   0,
   0,   0,   0,   0,   0,   0,   0, 115,   0, 116,
   0, 115,   0, 116,   0,   0,   0,   0,   0,   0,
   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,
   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,
   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,
   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,
 101, 102, 103,   0,   0,   0,   0,   0,   0,   0,
   0,   0,   0,   0,   0,   0, 115,   0, 116 };
short yypact[]={

-1000,-184,-1000,-286,-1000,-1000,-218, 233,-266,-210,
-155,-100,-214,-1000,-1000,-1000,-1000,-1000,-218, 903,
-1000,-1000, 390,-1000,-1000,-1000,-1000, 143, 143, 143,
 143,-248,-248,-1000,-136,-136,-136,-240,-240,-240,
-240,-240,-240,-240,-240,-113,-113,-113,-245,-245,
-245,-245,-245,-245,-245,-245,-256,-256, 371, 371,
 371, 371, 331, 331, 331, 331, 559, 559,-1000,-1000,
-1000,-1000,-1000,-1000,-1000,-1000,-1000, 331, 331, 331,
 331, 433, 787, 559, 316, 825,-162, 825, 331, 331,
-1000,-1000,-1000,1014,-155,1014,-1000,-1000,-1000,-1000,
 308,-1000,-1000,-1000,-1000,-1000, 306,-1000,-1000,  99,
1014,1014,1014,-1000,-1000,-1000,-1000,-1000,-1000,-1000,
-1000,-1000,-1000,-1000,-1000,-1000, 304,-1000, 371, 303,
 299, 371,1083,-165,-1000,-1000,-1000,-1000,-1000,-1000,
-1000,-1000,-1000,-1000,-1000,-1000,-1000,-1000,-1000,-1000,
-1000,-1000,-1000,-1000,-1000,-1000,-1000,-1000,-1000,-1000,
-127,-1000,-1000,-1000,-1000,-1000,-1000, 296, 295, 294,
 293, 292, 290,-1000,-1000, 288, 287, 285, 284,-1000,
-1000,-1000, 281, 280,-1000,-1000,-1000,-1000,-1000,-1000,
-1000,-1000,-1000, 279, 278,-1000,-1000,-1000,-1000,-1000,
-1000,-1000,-1000,-1000,-1000,-1000,-1000,-1000,-1000,-1000,
 275, 270,-1000,-1000,-1000,-1000,-1000,-1000, 976,  99,
-1000,-1000, 938,1018,-1000,-1000,-1000,-1000, 268,-1000,
 979,-1000,-1000,-1000,-1000,-1000,-1000, 371,-1000, 267,
 266, 371,-1000, 264,-1000,-1000,-1000,-1000,   6, 263,
 555, 262,-1000, 250, 371,-1000, 371, 249, 245, 825,
-1000,-168, 244, 242,-1000, 241, 240, 234,  99,1014,
  66,-100, 903,1014,1014,1014,1014,1014,1014,1014,
1014,1014,1014,-1000,-1000,  32, 825, 230, 229, 509,
 825, 223, 214,  99,1014,1014, 314, 208, 204, 198,
-234,-262,-245,-245,-245,-245,-244,-268,-245,-245,
-277,-257,-245,-245,-245,-245,  99,-1000, 218, 189,
 635,-1000,-1000,-1000, 900, 900, 193, 192, 191, 635,
 186,-1000,-1000,-1000,1014,-197, 181,-197,-255, 180,
-1000,-1000, 178, 172, 635, 825,-1000, 171, 825,-245,
 825,-263,-263,   7,-155, 167,-1000, 126,  19, 142,
 351, 351, 155, 155,-1000,-1000,-1000,-1000, 825, 825,
 825, 597, 371, 371,-1000, 825, 825,  99,  99, 131,
-245,-138,-172,-1000,-1000,-1000,-1000,-1000,-1000,-1000,
-1000,-1000,-1000,-1000,-1000,-1000,-1000,-1000,-1000, 131,
-146, 313, -26, 165,-1000,-1000, 900, 900, 900, -26,
-1000, 471,  99,-108,-197,-108, 371, 825, 825, 825,
 371,-1000,-1000,-179,-1000,-1000,-1000,-1000,-1000,-1000,
-1000,-1000,1014,1014,-100, 164, 749,-1000, 555, 159,
 157, 711,-1000,-1000,-1000,-1000,-1000,-148,-157,-1000,
-1000,-1000,-1000,-1000, 158,-1000, 156,-1000,-1000,-1000,
-1000, 900,-1000,-1000,-1000,-1000,-1000,-1000,-1000,   6,
 555,-1000,-1000,-1000,-1000,-108,-1000,-1000,-1000,-1000,
-1000,-1000,-1000,-1000,  99,  99, 154,-190, 152, 555,
 150, 825,-223, 139, 555,-1000,-1000,-1000, 900,-1000,
-1000,-1000,-1000,-100,-1000,-207, 133, 825,-1000, 555,
 673, 123,-1000,-1000,-1000, 555, 825,-1000,-1000,-1000,
 555, 825,-1000,-1000,-1000,-1000 };
short yypgo[]={

   0,   8,   7,   1,  78,   4, 518, 309,  23,  11,
 191, 516,   2, 515,   9, 166, 172, 514,  30, 513,
 382, 403, 436, 238, 170, 154, 221, 161, 429, 510,
 509, 507,   5, 502,  26,   0,   3,  22, 498, 496,
 491, 490, 461, 489, 487, 486,  87, 485, 479, 469,
 465, 457, 454, 449, 445, 444, 441, 148, 427, 417,
 175, 406, 401, 397, 395, 392, 391, 390, 383, 380,
 379, 378, 367 };
short yyr1[]={

   0,  38,  38,  39,  39,  39,  40,  40,  40,  40,
  40,  41,  41,  41,  41,  41,  41,  41,  41,  41,
  45,  45,  46,  46,  34,  34,  34,  34,  43,  43,
  44,  42,  42,  47,  47,  47,  49,  49,  49,  49,
  49,  49,  49,  49,  49,  49,  49,  49,  49,  49,
  49,  49,  49,  49,  49,  49,  49,  49,  49,  49,
  49,  49,  49,  49,  49,  49,  49,  28,  30,  30,
  30,  29,  29,  29,  29,  20,  20,  20,  20,  20,
  20,  21,  21,  21,  21,  22,  22,  22,  22,  23,
  24,  24,  25,  26,  27,  27,   7,   7,  48,  48,
  48,  48,  54,  54,  54,  54,  55,  55,  56,  56,
  51,  51,  51,  51,  51,  51,  51,  51,  51,  51,
  51,  51,  51,  51,  51,  51,  51,  51,  51,  59,
  59,  59,  59,  59,  58,  58,  58,  58,  58,  57,
  57,  11,  11,  11,  11,  52,  52,  52,  52,  60,
  60,  13,  13,  13,  53,  53,  53,  53,  53,  64,
  64,  33,  31,  63,  63,  63,  63,  63,  63,  61,
  61,  61,  61,  61,  61,  36,  36,  36,  62,  62,
  10,   8,   8,  14,  14,  15,  15,   9,   9,  12,
  12,  12,  12,  16,  16,  16,  16,  35,  35,  35,
  35,  35,  35,  35,  35,  35,  35,  35,  35,  35,
  35,  35,  35,  17,  17,  17,  17,  17,  17,  17,
  32,  32,  32,  50,  50,  50,  50,  50,  50,  50,
  50,  65,  70,  66,  67,  67,  67,  67,  67,  68,
  68,  68,  69,  69,  69,  72,  72,  72,  72,  71,
  71,  71,  71,   1,   1,   1,  18,  18,  19,   3,
   3,   4,   4,   5,   2,   2,   6,   6,   6,  37,
  37,  37,  37,  37,  37,  37,  37,  37,  37,  37,
  37,  37,  37 };
short yyr2[]={

   0,   0,   2,   2,   2,   1,   0,   1,   2,   3,
   2,   3,   2,   6,   3,   6,   3,   8,   2,   1,
   3,   1,   1,   1,   1,   1,   1,   1,   1,   0,
   1,   1,   0,   1,   1,   2,   2,   2,   2,   2,
   2,   2,   1,   2,   2,   2,   2,   2,   2,   2,
   2,   2,   2,   2,   2,   2,   2,   2,   2,   2,
   2,   2,   2,   2,   2,   2,   2,   4,   1,   1,
   0,   3,   3,   3,   3,   3,   3,   3,   3,   3,
   3,   3,   3,   3,   3,   1,   1,   3,   3,   1,
   3,   3,   1,   1,   3,   3,   1,   1,   1,   1,
   1,   1,   4,   2,   4,   4,   1,   1,   1,   1,
   2,   2,   2,   2,   2,   2,   2,   2,   2,   2,
   1,   1,   1,   1,   1,   1,   1,   1,   1,   3,
   3,   4,   4,   4,   1,   1,   2,   2,   2,   6,
   5,   1,   1,   1,   1,   2,   2,   2,   2,   4,
   3,   1,   1,   1,   2,   2,   2,   3,   2,   1,
   4,   3,   3,   4,   4,   3,   4,   4,   3,   5,
   4,   5,   5,   4,   4,   1,   1,   1,   4,   4,
   1,   1,   1,   1,   1,   1,   1,   1,   1,   1,
   3,   5,   4,   5,   5,   4,   4,   1,   1,   1,
   1,   1,   1,   1,   1,   1,   1,   1,   1,   1,
   1,   1,   1,   1,   1,   1,   1,   1,   1,   1,
   1,   1,   1,   1,   1,   1,   1,   1,   1,   1,
   1,   3,   3,   1,   4,   4,   4,   4,   3,   7,
   6,   7,   7,   6,   7,   4,   4,   4,   4,   8,
   8,   8,   8,   1,   1,   1,   2,   3,   3,   1,
   1,   2,   2,   2,   1,   1,   1,   1,   1,   3,
   3,   3,   3,   3,   3,   3,   3,   3,   3,   2,
   2,   3,   1 };
short yychk[]={

-1000, -38, -39, -40, 256, 276, -41, -43, 273, 366,
 362, 365, 346, 364, 298, 299, -42, 276, -44, 363,
 -47, -48, -49, -51, -52, -53, -54, 313, 314, 311,
 312, 319, 302, 341, 322, 308, 309, 317, 303, 307,
 320, 329, 304, 323, 321, 324, 330, 326, 306, 328,
 300, 333, 305, 327, 315, 332, 301, 318, 271, 270,
 350, 347, 352, 351, 349, 348, 354, 338, 339, 358,
 360, 355, 357, 359, 345, 356, 353, 337, 336, 335,
 334, 344, 343, 342, 340, 272, 316, 310, -55, -56,
 331, 325, -42, 361, 273, -34, 280, 277, 278, 279,
  -1, 257, 258, 259, 274, -42, -45, -46, 274, -37,
  45, 126,  40,  -6,  -1, 273, 275, -50, -65, -66,
 -67, -68, -69, -70, -71, -72, -18, -16,  -8, -35,
 -19, 279,  35,  40, 277, 278, 265, 266, 260, 261,
 281, 283, 264, 263, 262, -17, 282, 284, 287, 288,
 285, 286, 289, 292, 293, 294, 295, 296, 297, -28,
 -30,  43,  45, -28, -28, -28, -20, 283, 281, 287,
 288, 265, 266, -20, -21, 283, 281, 265, 266, -21,
 -21, -22, 281, 283, -22, -22, -22, -22, -23, -22,
 -23, -23, -24, 265, 266, -24, -24, -25,  -7, 281,
 283, -25, -25, -25, -26,  -7, -26, -26, -26, -27,
 287, 288, -27, -15,  -9,  -4,  -3, -12,  60, -37,
  42, -16,  40,  45, -15, -15, -15, -57, -10, -18,
  35, -57, -57, -57, -58, -18, -35,  -8, -59, -18,
 -35,  -8, -60, -10, -60, -60, -60, -61,  -8, -18,
 280, -35, -62, -35, 280, -63,  -8, -35, -18, -16,
 -64, -33, -35, 264, -31, -35, -18, -18, -37, -34,
 -37,  44,  44, 124,  94,  38, 368, 367,  45,  43,
  37,  47,  42, -37, -37, -37,  44,  -9,  -4,  44,
  44,  -9,  -4, -37,  60,  62, 264, -29, 265, 266,
  44,  44,  44,  44,  44,  44,  44,  44,  44,  44,
  44,  44,  44,  44,  44,  44, -37,  42, 264,  40,
  44, -12,  -3,  -4,  44,  44, -12,  -4,  -3,  44,
 -36,  -3,  -5, -12, 367,  44,  -9,  44,  44, -14,
  -9,  -4,  -9,  -4,  44,  44, -35, 264,  44,  44,
  44,  44,  44, -37,  44,  -1, -46, -37, -37, -37,
 -37, -37, -37, -37, -37, -37, -37,  41, -35,  44,
  44, -35,  -8, 279, -35,  44,  44, -37, -37,  41,
  44,  44,  44, 281, 283,  -7,  -7,  -7,  -7, 281,
 283,  -7,  -7, 283, 281,  -7,  -7,  -7,  -7,  41,
  43, 264,  -8, -35,  -3,  -3,  44,  44,  44,  -8,
 -35,  44, -37,  -8,  44,  -8, 280,  44,  44,  44,
  -8, -35, -35,  44, -35,  -7, -35, -32, 290, 291,
 292, -32,  58, -34,  44, -35, -35, -35, 278, -18,
 -35,  -9,  -4,  -9,  -4, -35, -35,  45,  43,  -7,
 265, 266, 266, 265, 263,  41, -11,  -3,  -4,  -5,
 -12,  44,  -3,  -3,  -3, -13,  -5,  -4,  -9,  -8,
 280, -35,  -2,  -1, 273,  -8,  -2, -14, -35, -35,
 -35,  -9,  -4, 264, -37, -37,  -1,  44, -35, 278,
  -9,  44,  44, -35, 278, 263, 263,  41,  44,  -3,
 -36,  -9,  -2,  44, 266,  44,  -9,  44, -35, 278,
  44,  -9,  -3,  -1, 266, 278,  44, -35,  -9, -35,
 278,  44,  -9, -35,  -9, -35 };
short yydef[]={

   1,  -2,   2,   0,   5,   7,  32,   0,  -2,   0,
   0,   0,   0,  19,   3,   4,   8,  31,  32,   0,
  30,  33,  34,  98,  99, 100, 101,  70,  70,  70,
  70,   0,   0,  42,   0,   0,   0,   0,   0,   0,
   0,   0,   0,   0,   0, 108, 106,   0,   0,   0,
   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,
   0,   0,   0,   0,   0,   0,   0,   0, 120, 121,
 122, 123, 124, 125, 126, 127, 128,   0,   0,   0,
   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,
 107, 109,  10,   0,  12,   0,  24,  25,  26,  27,
   0, 253, 254, 255,  18,   9,  16,  21,  22,  23,
   0,   0,   0, 282, 266, 267, 268,  35, 223, 224,
 225, 226, 227, 228, 229, 230,   0, 233,   0,   0,
   0,   0,   0,   0, 181, 182, 197, 198, 199, 200,
 201, 202, 203, 204, 205, 206, 207, 208, 209, 210,
 211, 212, 213, 214, 215, 216, 217, 218, 219,  36,
   0,  68,  69,  37,  38,  39,  40,   0,   0,   0,
   0,   0,   0,  41,  43,   0,   0,   0,   0,  44,
  45,  46,  85,  86,  47,  48,  49,  50,  51,  89,
  52,  53,  54,   0,   0,  55,  56,  57,  92,  96,
  97,  58,  59,  60,  61,  93,  62,  63,  64,  65,
   0,   0,  66, 110, 185, 186, 187, 188,   0, 259,
 260, 189,   0,   0, 111, 112, 113, 114,   0, 180,
   0, 115, 116, 117, 118, 134, 135,   0, 119,   0,
   0,   0, 145,   0, 146, 147, 148, 154,   0,   0,
   0,   0, 155,   0,   0, 156,   0,   0,   0,   0,
 158, 159,   0,   0, 103,   0,   0,   0,  11,   0,
  14,   0,   0,   0,   0,   0,   0,   0,   0,   0,
   0,   0,   0, 279, 280,   0,   0,   0,   0,   0,
   0,   0,   0, 256,   0,   0,   0,   0,   0,   0,
   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,
   0,   0,   0,   0,   0,   0, 261, 262,   0,   0,
   0, 136, 137, 138,   0,   0,   0,   0,   0,   0,
   0, 175, 176, 177,   0,   0,   0,   0,   0,   0,
 183, 184,   0,   0,   0,   0, 157,   0,   0,   0,
   0,   0,   0,   0,   0,   0,  20, 269, 270, 271,
 272, 273, 274, 275, 276, 277, 278, 281, 231,   0,
   0, 232,   0,   0, 238,   0,   0, 257, 258,   0,
   0,   0,   0,  75,  76,  77,  78,  79,  80,  81,
  82,  83,  84,  88,  87,  90,  91,  94,  95, 190,
   0,   0,   0,   0, 129, 130,   0,   0,   0,   0,
 150,   0, 263,   0,   0,   0,   0,   0,   0,   0,
   0, 168, 165,   0, 161, 102, 162, 104, 220, 221,
 222, 105,   0,   0,   0,   0, 234, 235,   0,   0,
   0, 236, 237, 246, 248, 245, 247, 195, 196,  67,
  71,  73,  72,  74,   0, 192,   0, 141, 142, 143,
 144,   0, 131, 132, 133, 149, 151, 152, 153,   0,
   0, 174, 170, 264, 265,   0, 173, 178, 179, 163,
 166, 164, 167, 160,  13,  15,   0,   0,   0,   0,
   0,   0,   0,   0,   0, 193, 194, 191,   0, 140,
 169, 172, 171,   0, 240,   0,   0,   0, 243,   0,
   0,   0, 139,  17, 239,   0,   0, 242, 244, 241,
   0,   0, 250, 249, 252, 251 };
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
# line 212 "a56.y"
{substatement = 0;
			if(NOT check_psect(seg, pc) && pass == 2)
				yyerror("psect violation");
			} break;
case 4:
# line 217 "a56.y"
{substatement++;
			if(NOT check_psect(seg, pc) && pass == 2)
				yyerror("psect violation");
			} break;
case 6:
# line 226 "a56.y"
{if(pass == 2 && list_on) {
				printf("\n");
			}} break;
case 7:
# line 230 "a56.y"
{if(pass == 2 && NOT substatement && list_on) {
				printf("%s%s\n", spaces(0), luntab(inline));
			}} break;
case 9:
# line 235 "a56.y"
{char *printcode();
			if(pass == 2) {
				gencode(seg, pc, w0);
				if(list_on) printf("%c:%04X %s %s\n", segs[seg], pc, 
					printcode(w0), substatement ? "" :
						luntab(inline));
				pc++;
				if(uses_w1) {
					gencode(seg, pc, w1);
					if(list_on) printf("%c:%04X %s\n", segs[seg], pc,
	       					printcode(w1 & 0xFFFFFF));
					pc++;
				}
			} else {
				pc++;
				if(uses_w1)
					pc++;
			}
			w0 = w1 = 0; uses_w1 = FALSE;} break;
case 10:
# line 255 "a56.y"
{sym_def(yypvt[-1].sval, pc);
			free(yypvt[-1].sval);
			if(pass == 2) {
				if(list_on) printf("%c:%04X%s%s\n", segs[seg], pc, 
					spaces(14-8), substatement ? "" :
						luntab(inline));
			}} break;
case 11:
# line 266 "a56.y"
{sym_def(yypvt[-2].sval, yypvt[-0].ival);
			free(yypvt[-2].sval);
			if(pass == 2) {
				if(list_on) printf("%06X%s%s\n", yypvt[-0].ival & 0xFFFFFF, 
					spaces(14-6-2), 
					substatement ? "" : luntab(inline));
			}} break;
case 12:
# line 274 "a56.y"
{struct psect *pp = find_psect(yypvt[-0].sval);
			if(NOT pp) {
				if(pass == 2)
					yyerror("undefined psect");
			} else {
				seg = pp->seg;
				pc = pp->pc;
				set_psect(pp);
				if(pass == 2) {
					if(list_on) printf("%c:%04X%s%s\n", 
						segs[seg], pc, 
						spaces(14-8), substatement ? "" : luntab(inline));
				}
			}
			free(yypvt[-0].sval);} break;
case 13:
# line 290 "a56.y"
{new_psect(yypvt[-4].sval, yypvt[-3].ival, yypvt[-2].ival, yypvt[-0].ival);
			if(pass == 2) {
				if(list_on) printf("%c:%04X %04X%s%s\n", 
					segs[yypvt[-3].ival], yypvt[-2].ival, yypvt[-0].ival, spaces(14-8+4+1), 
					substatement ? "" : luntab(inline));
			}} break;
case 14:
# line 297 "a56.y"
{pc = yypvt[-0].ival;
			seg = yypvt[-1].ival;
			if(pass == 2) {
				if(list_on) printf("%c:%04X%s%s\n", segs[seg], pc, 
					spaces(14-8), substatement ? "" : luntab(inline));
			}} break;
case 15:
# line 304 "a56.y"
{pc = yypvt[-3].ival;
			seg = yypvt[-4].ival;
			if(pass == 2) {
				if(list_on) printf("%c:%04X%s%s\n", segs[seg], pc, 
					spaces(14-8), substatement ? "" : luntab(inline));
			}} break;
case 17:
# line 312 "a56.y"
{if(pass == 2 && NOT substatement && list_on) {
				printf("%s%s\n", spaces(0), luntab(inline));
			}} break;
case 18:
# line 316 "a56.y"
{if(pass == 2 && NOT substatement && list_on) {
				printf("%s%s\n", spaces(0), luntab(inline));
			}
			include(yypvt[-0].sval); /* free($2); */
			} break;
case 19:
# line 322 "a56.y"
{if(pass == 2 && NOT substatement && list_on) {
				printf("%s%s\n", spaces(0), luntab(inline));
			}} break;
case 22:
# line 334 "a56.y"
{int len = strlen(yypvt[-0].sval), i; char *cp; w0 = 0;
			if(len % 3 == 2)
				len++;	/* force empty word */
			for(i = 0, cp = yypvt[-0].sval; i < len; i++, cp++) {
				w0 |= (*cp & 0xFF) << (2 - (i % 3)) * 8;
				if(i % 3 == 2 || i == len - 1) {
					if(pass == 2) {
						if(list_on) printf("%c:%04X %06X%s%s\n",
							segs[seg], pc, w0, 
							spaces(14-6+5), 
							substatement ? "" : luntab(inline));
						gencode(seg, pc, w0);
						substatement++;
					}
					pc++; w0 = 0;
				}
			}
			free(yypvt[-0].sval);} break;
case 23:
# line 353 "a56.y"
{if(pass == 2) {
				if(list_on) printf("%c:%04X %06X%s%s\n", 
					segs[seg], pc,
					yypvt[-0].ival & 0xFFFFFF, spaces(14-6+5),
					substatement ? "" : luntab(inline));
				gencode(seg, pc, yypvt[-0].ival);
				substatement++;
			}
			pc++;} break;
case 24:
# line 364 "a56.y"
{yyval.ival = PROG;} break;
case 25:
# line 366 "a56.y"
{yyval.ival = XDATA;} break;
case 26:
# line 368 "a56.y"
{yyval.ival = YDATA;} break;
case 27:
# line 370 "a56.y"
{yyval.ival = LDATA;} break;
case 28:
# line 375 "a56.y"
{sym_def(yypvt[-0].sval, pc);
			free(yypvt[-0].sval);} break;
case 30:
# line 382 "a56.y"
{if(just_rep) 
				just_rep--;} break;
case 34:
# line 394 "a56.y"
{w0 |= 0x200000;} break;
case 36:
# line 403 "a56.y"
{w0 |= 0x80 | yypvt[-0].ival << 2;} break;
case 37:
# line 405 "a56.y"
{w0 |= 0x81 | yypvt[-0].ival << 2;} break;
case 38:
# line 407 "a56.y"
{w0 |= 0x82 | yypvt[-0].ival << 2;} break;
case 39:
# line 409 "a56.y"
{w0 |= 0x83 | yypvt[-0].ival << 2;} break;
case 40:
# line 412 "a56.y"
{w0 |= 0x04 | yypvt[-0].ival << 3;} break;
case 41:
# line 414 "a56.y"
{w0 |= 0x00 | yypvt[-0].ival << 3;} break;
case 42:
# line 416 "a56.y"
{w0 |= 0x00;} break;
case 43:
# line 419 "a56.y"
{w0 |= 0x01 | yypvt[-0].ival << 3;} break;
case 44:
# line 421 "a56.y"
{w0 |= 0x05 | yypvt[-0].ival << 3;} break;
case 45:
# line 423 "a56.y"
{w0 |= 0x07 | yypvt[-0].ival << 3;} break;
case 46:
# line 426 "a56.y"
{w0 |= 0x11 | yypvt[-0].ival << 3;} break;
case 47:
# line 428 "a56.y"
{w0 |= 0x12 | yypvt[-0].ival << 3;} break;
case 48:
# line 430 "a56.y"
{w0 |= 0x13 | yypvt[-0].ival << 3;} break;
case 49:
# line 432 "a56.y"
{w0 |= 0x16 | yypvt[-0].ival << 3;} break;
case 50:
# line 434 "a56.y"
{w0 |= 0x17 | yypvt[-0].ival << 3;} break;
case 51:
# line 437 "a56.y"
{w0 |= 0x02 | yypvt[-0].ival << 3;} break;
case 52:
# line 439 "a56.y"
{w0 |= 0x03 | yypvt[-0].ival << 3;} break;
case 53:
# line 441 "a56.y"
{w0 |= 0x06 | yypvt[-0].ival << 3;} break;
case 54:
# line 444 "a56.y"
{w0 |= 0x46 | yypvt[-0].ival << 3;} break;
case 55:
# line 446 "a56.y"
{w0 |= 0x42 | yypvt[-0].ival << 3;} break;
case 56:
# line 448 "a56.y"
{w0 |= 0x43 | yypvt[-0].ival << 3;} break;
case 57:
# line 451 "a56.y"
{w0 |= 0x22 | yypvt[-0].ival << 3;} break;
case 58:
# line 453 "a56.y"
{w0 |= 0x23 | yypvt[-0].ival << 3;} break;
case 59:
# line 455 "a56.y"
{w0 |= 0x26 | yypvt[-0].ival << 3;} break;
case 60:
# line 457 "a56.y"
{w0 |= 0x27 | yypvt[-0].ival << 3;} break;
case 61:
# line 460 "a56.y"
{w0 |= 0x32 | yypvt[-0].ival << 3;} break;
case 62:
# line 462 "a56.y"
{w0 |= 0x33 | yypvt[-0].ival << 3;} break;
case 63:
# line 464 "a56.y"
{w0 |= 0x36 | yypvt[-0].ival << 3;} break;
case 64:
# line 466 "a56.y"
{w0 |= 0x37 | yypvt[-0].ival << 3;} break;
case 65:
# line 469 "a56.y"
{w0 |= 0x21 | yypvt[-0].ival << 3;} break;
case 66:
# line 471 "a56.y"
{w0 |= 0x25 | yypvt[-0].ival << 3;} break;
case 67:
# line 475 "a56.y"
{yyval.ival = yypvt[-3].ival | yypvt[-0].ival << 1 | yypvt[-2].ival << 2;} break;
case 68:
# line 480 "a56.y"
{yyval.ival = 0;} break;
case 69:
# line 482 "a56.y"
{yyval.ival = 1;} break;
case 70:
# line 484 "a56.y"
{yyval.ival = 0;} break;
case 71:
# line 489 "a56.y"
{switch (yypvt[-2].ival << 4 | yypvt[-0].ival) {
				case 0x00: yyval.ival = 0x0; break;
				case 0x01: 
				case 0x10: yyval.ival = 0x2; break;
				case 0x11: 
					yyerror("illegal source operands"); 
					break;
			}} break;
case 72:
# line 498 "a56.y"
{switch (yypvt[-2].ival << 4 | yypvt[-0].ival) {
				case 0x00: yyval.ival = 0x1; break;
				case 0x01: 
				case 0x10: yyval.ival = 0x3; break;
				case 0x11: 
					yyerror("illegal source operands"); 
					break;
			}} break;
case 73:
# line 507 "a56.y"
{switch (yypvt[-2].ival << 4 | yypvt[-0].ival) {
				case 0x00: yyval.ival = 0x5; break;
				case 0x01: yyval.ival = 0x4; break;
				case 0x10: yyval.ival = 0x6; break;
				case 0x11: yyval.ival = 0x7; break;
			}} break;
case 74:
# line 514 "a56.y"
{switch (yypvt[-2].ival << 4 | yypvt[-0].ival) {
				case 0x00: yyval.ival = 0x5; break;
				case 0x01: yyval.ival = 0x6; break;
				case 0x10: yyval.ival = 0x4; break;
				case 0x11: yyval.ival = 0x7; break;
			}} break;
case 75:
# line 523 "a56.y"
{yyval.ival = 0x2;} break;
case 76:
# line 525 "a56.y"
{yyval.ival = 0x3;} break;
case 77:
# line 527 "a56.y"
{yyval.ival = 0x4 | yypvt[-0].ival;} break;
case 78:
# line 529 "a56.y"
{yyval.ival = 0x6 | yypvt[-0].ival;} break;
case 79:
# line 531 "a56.y"
{yyval.ival = 0x8 | yypvt[-2].ival << 2 | yypvt[-0].ival;} break;
case 80:
# line 533 "a56.y"
{yyval.ival = 0xA | yypvt[-2].ival << 2 | yypvt[-0].ival;} break;
case 81:
# line 537 "a56.y"
{yyval.ival = 0x2;} break;
case 82:
# line 539 "a56.y"
{yyval.ival = 0x3;} break;
case 83:
# line 541 "a56.y"
{yyval.ival = 0x8 | yypvt[-2].ival << 2 | yypvt[-0].ival;} break;
case 84:
# line 543 "a56.y"
{yyval.ival = 0xA | yypvt[-2].ival << 2 | yypvt[-0].ival;} break;
case 85:
# line 547 "a56.y"
{yyval.ival = 0x0;} break;
case 86:
# line 549 "a56.y"
{yyval.ival = 0x1;} break;
case 87:
# line 551 "a56.y"
{yyval.ival = 0x0;} break;
case 88:
# line 553 "a56.y"
{yyval.ival = 0x1;} break;
case 89:
# line 557 "a56.y"
{yyval.ival = yypvt[-0].ival;} break;
case 90:
# line 561 "a56.y"
{yyval.ival = 0x0 | yypvt[-2].ival << 2 | yypvt[-0].ival;} break;
case 91:
# line 563 "a56.y"
{yyval.ival = 0x2 | yypvt[-2].ival << 2 | yypvt[-0].ival;} break;
case 92:
# line 567 "a56.y"
{yyval.ival = yypvt[-0].ival;} break;
case 93:
# line 571 "a56.y"
{yyval.ival = yypvt[-0].ival;} break;
case 94:
# line 575 "a56.y"
{yyval.ival = 0x0 | yypvt[-0].ival;} break;
case 95:
# line 577 "a56.y"
{yyval.ival = 0x2 | yypvt[-0].ival;} break;
case 96:
# line 581 "a56.y"
{yyval.ival = 0;} break;
case 97:
# line 583 "a56.y"
{yyval.ival = 1;} break;
case 98:
# line 588 "a56.y"
{if(just_rep == 1)
				yyerror("instruction not allowed after REP");} break;
case 102:
# line 599 "a56.y"
{w0 |= 0x01D815 | yypvt[-2].ival << 8 | yypvt[-0].ival << 3;} break;
case 103:
# line 601 "a56.y"
{w0 |= 0x018040 | yypvt[-0].ival << 3;} break;
case 104:
# line 603 "a56.y"
{w0 |= 0x0000F8 | (yypvt[-2].ival & 0xFF) << 8 | yypvt[-0].ival;} break;
case 105:
# line 605 "a56.y"
{w0 |= 0x0000B8 | (yypvt[-2].ival & 0xFF) << 8 | yypvt[-0].ival;} break;
case 110:
# line 620 "a56.y"
{if(yypvt[-0].ival) {
				w0 |= 0x0BC0A0 | yypvt[-1].cond << 0;
			} else {
				w0 |= 0x0F0000 | yypvt[-1].cond << 12;
			}} break;
case 111:
# line 626 "a56.y"
{if(yypvt[-0].ival) {
				w0 |= 0x0AC0A0 | yypvt[-1].cond << 0;
			} else {
				w0 |= 0x0E0000 | yypvt[-1].cond << 12;
			}} break;
case 112:
# line 632 "a56.y"
{if(yypvt[-0].ival) {
				w0 |= 0x0BC080;
			} else {
				w0 |= 0x0D0000;
			}} break;
case 113:
# line 638 "a56.y"
{if(yypvt[-0].ival) {
				w0 |= 0x0AC080;
			} else {
				w0 |= 0x0C0000;
			}} break;
case 114:
# line 645 "a56.y"
{w0 |= 0x0B0020;} break;
case 115:
# line 647 "a56.y"
{w0 |= 0x0B0000;} break;
case 116:
# line 649 "a56.y"
{w0 |= 0x0A0020;} break;
case 117:
# line 651 "a56.y"
{w0 |= 0x0A0000;} break;
case 118:
# line 654 "a56.y"
{just_rep = 2;} break;
case 119:
# line 656 "a56.y"
{uses_w1++;} break;
case 120:
# line 658 "a56.y"
{w0 |= 0x00008C;} break;
case 121:
# line 660 "a56.y"
{w0 |= 0x000087;} break;
case 122:
# line 662 "a56.y"
{w0 |= 0x000086;} break;
case 123:
# line 664 "a56.y"
{w0 |= 0x000084;} break;
case 124:
# line 666 "a56.y"
{w0 |= 0x00000C;} break;
case 125:
# line 668 "a56.y"
{w0 |= 0x000006;} break;
case 126:
# line 670 "a56.y"
{w0 |= 0x000005;} break;
case 127:
# line 672 "a56.y"
{w0 |= 0x000004;} break;
case 128:
# line 674 "a56.y"
{w0 |= 0x000000;
			just_rep = 0;} break;
case 129:
# line 679 "a56.y"
{w0 |= 0x060080 | (yypvt[-2].ival & 0xFF) << 8 | (yypvt[-2].ival & 0xF00)>> 8;
			if(yypvt[-2].ival > 0xFFF && pass == 2) {
				yyerror("warning: immediate operand truncated");
			}
			w1 |= yypvt[-0].ival-1;} break;
case 130:
# line 685 "a56.y"
{w0 |= 0x06C000 | yypvt[-2].regs.r6 << 8;
			w1 |= yypvt[-0].ival-1;} break;
case 131:
# line 688 "a56.y"
{w0 |= 0x064000 | yypvt[-2].ival << 8 | yypvt[-3].ival << 6;
			w1 |= yypvt[-0].ival-1;} break;
case 132:
# line 691 "a56.y"
{w0 |= 0x060000 | (yypvt[-2].ival & 0x3F) << 8 | yypvt[-3].ival << 6;
			if(yypvt[-2].ival > 0x003F && pass == 2)
				yyerror("warning: address operand truncated");
			w1 |= yypvt[-0].ival-1;} break;
case 133:
# line 696 "a56.y"
{w0 |= 0x060000 | (yypvt[-2].ival & 0x3F) << 8 | yypvt[-3].ival << 6;
			if(yypvt[-2].ival > 0x003F && pass == 2)
				yyerror("warning: address operand truncated");
			w1 |= yypvt[-0].ival-1;} break;
case 134:
# line 704 "a56.y"
{w0 |= 0x0600A0 | (yypvt[-0].ival & 0xFF) << 8 | (yypvt[-0].ival & 0xF00)>> 8;
			if(yypvt[-0].ival > 0xFFF && pass == 2) {
				yyerror("warning: immediate operand truncated");
			}} break;
case 135:
# line 709 "a56.y"
{w0 |= 0x06C020 | yypvt[-0].regs.r6 << 8;} break;
case 136:
# line 711 "a56.y"
{w0 |= 0x064020 | yypvt[-1].ival << 6 | yypvt[-0].ival << 8;} break;
case 137:
# line 713 "a56.y"
{w0 |= 0x060020 | yypvt[-1].ival << 6 | (yypvt[-0].ival & 0x3F) << 8;
			if(yypvt[-0].ival > 0x003F && pass == 2)
				yyerror("warning: address operand truncated");
			} break;
case 138:
# line 718 "a56.y"
{w0 |= 0x060020 | yypvt[-1].ival << 6 | (yypvt[-0].ival & 0x3F) << 8;
			if(yypvt[-0].ival > 0x003F && pass == 2)
				yyerror("warning: address operand truncated");
			} break;
case 139:
# line 726 "a56.y"
{w0 |= yypvt[-5].ival << 0 | yypvt[-3].ival << 6;
			uses_w1++;
			w1 = yypvt[-0].ival;} break;
case 140:
# line 730 "a56.y"
{w0 |= 0x00C000 | yypvt[-4].ival << 0 | yypvt[-2].regs.r6 << 8;
			uses_w1++;
			w1 = yypvt[-0].ival;} break;
case 141:
# line 741 "a56.y"
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
case 142:
# line 752 "a56.y"
{if(yypvt[-0].ival != -1) {
				if(yypvt[-0].ival > 0x3F && pass == 2)
					yyerror("warning: address operand truncated");
				w0 |= 0x000080 | (yypvt[-0].ival & 0x3F) << 8;
			}} break;
case 143:
# line 758 "a56.y"
{if(yypvt[-0].ival != -1) {
				if(yypvt[-0].ival < 0xFFC0 && pass == 2)
					yyerror("warning: address operand truncated");
				w0 |= 0x008080 | (yypvt[-0].ival & 0x3F) << 8;
			}} break;
case 144:
# line 764 "a56.y"
{w0 |= 0x004080 | yypvt[-0].ival << 8;} break;
case 145:
# line 771 "a56.y"
{w0 |= 0x0B0020;} break;
case 146:
# line 773 "a56.y"
{w0 |= 0x0B0000;} break;
case 147:
# line 775 "a56.y"
{w0 |= 0x0A0020;} break;
case 148:
# line 777 "a56.y"
{w0 |= 0x0A0000;} break;
case 149:
# line 782 "a56.y"
{w0 |= yypvt[-3].ival << 0 | yypvt[-1].ival << 6;} break;
case 150:
# line 784 "a56.y"
{w0 |= 0x00C040 | yypvt[-2].ival << 0 | yypvt[-0].regs.r6 << 8;} break;
case 151:
# line 789 "a56.y"
{if(yypvt[-0].ival != -1) {
				w0 |= (yypvt[-0].ival & 0x3F) << 8 | 0x008000;
				if(yypvt[-0].ival < 0xFFC0 && pass == 2)
					yyerror("warning: address operand truncated");
			}} break;
case 152:
# line 795 "a56.y"
{if(yypvt[-0].ival != -1) {
				w0 |= (yypvt[-0].ival & 0x3F) << 8 | 0x000000;
				if(yypvt[-0].ival > 0x003F && pass == 2)
					yyerror("warning: address operand truncated");
			}} break;
case 153:
# line 801 "a56.y"
{w0 |= 0x004000;} break;
case 157:
# line 811 "a56.y"
{w0 |= 0x044010 | yypvt[-1].ival << 8 | yypvt[-0].regs.r4;} break;
case 158:
# line 813 "a56.y"
{w0 |= yypvt[-1].cond << 12;} break;
case 159:
# line 818 "a56.y"
{w0 |= 0x020000 | yypvt[-0].ival << 3;} break;
case 160:
# line 820 "a56.y"
{w0 |= 0x030000 | yypvt[-3].ival << 3 | yypvt[-2].ival << 8 | yypvt[-0].ival;} break;
case 161:
# line 824 "a56.y"
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
case 162:
# line 838 "a56.y"
{if(yypvt[-2].regs.flags & R_XREG && yypvt[-0].regs.flags & R_AB) {
				yyval.ival = yypvt[-2].regs.xreg << 2 | yypvt[-0].regs.ab;
			} else if(yypvt[-2].regs.flags & R_YREG && yypvt[-0].regs.flags & R_AB) {
				yyval.ival = yypvt[-2].regs.yreg << 2 | yypvt[-0].regs.ab;
			}} break;
case 163:
# line 847 "a56.y"
{if(yypvt[-3].ival == 0) {
				w0 |= 0x05C020 | yypvt[-0].regs.ctl_reg;
			} else {
				w0 |= 0x05C060 | yypvt[-0].regs.ctl_reg;
			}} break;
case 164:
# line 853 "a56.y"
{if(yypvt[-1].ival == 0) {
				w0 |= 0x054020 | yypvt[-3].regs.ctl_reg;
			} else {
				w0 |= 0x054060 | yypvt[-3].regs.ctl_reg;
			}} break;
case 165:
# line 859 "a56.y"
{if(yypvt[-2].ival < 256) {
				w0 |= 0x0500A0 | (yypvt[-2].ival & 0xFF) << 8 | yypvt[-0].regs.ctl_reg; 
			} else {
				w0 |= 0x05C020 | 0x003400 | yypvt[-0].regs.ctl_reg;
				uses_w1++; w1 = yypvt[-2].ival & 0xFFFF;
			}} break;
case 166:
# line 866 "a56.y"
{if(yypvt[-3].ival == 0) {
				w0 |= 0x058020 | (yypvt[-2].ival & 0x3F) << 8 | yypvt[-0].regs.ctl_reg;
			} else {
				w0 |= 0x058060 | (yypvt[-2].ival & 0x3F) << 8 | yypvt[-0].regs.ctl_reg;
			}
			if(yypvt[-2].ival > 0x003F && pass == 2)
				yyerror("warning: address operand truncated");
			} break;
case 167:
# line 875 "a56.y"
{if(yypvt[-1].ival == 0) {
				w0 |= 0x050020 | (yypvt[-0].ival & 0x3F) << 8 | yypvt[-3].regs.ctl_reg;
			} else {
				w0 |= 0x050060 | (yypvt[-0].ival & 0x3F) << 8 | yypvt[-3].regs.ctl_reg;
			}
			if(yypvt[-0].ival > 0x003F && pass == 2)
				yyerror("warning: address operand truncated");
			} break;
case 168:
# line 884 "a56.y"
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
case 169:
# line 899 "a56.y"
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
case 170:
# line 928 "a56.y"
{w0 |= 0x084080;
			w0 |= yypvt[-1].ival << 16 | 1 << 15 | 0x34 << 8 | (yypvt[-0].ival & 0x3F);
			uses_w1++;
			w1 = yypvt[-3].ival;} break;
case 171:
# line 933 "a56.y"
{w0 |= 0x084040;
			w0 |= yypvt[-1].ival << 16 | 1 << 15 | (yypvt[-0].ival & 0x3F);} break;
case 172:
# line 936 "a56.y"
{w0 |= 0x084040;
			if(yypvt[-3].ea.mode != 0x003000 && yypvt[-3].ea.mode != 0)
				yyerror("illegal MOVEP");
			w0 |= yypvt[-4].ival << 16 | 0 << 15 | (yypvt[-3].ea.ext & 0x3F);} break;
case 173:
# line 941 "a56.y"
{w0 |= 0x084000;
			w0 |= yypvt[-1].ival << 16 | 1 << 15 | yypvt[-3].regs.r6 << 8 | (yypvt[-0].ival & 0x3F);} break;
case 174:
# line 944 "a56.y"
{w0 |= 0x084000;
			if(!yypvt[-2].ea.pp)
				yyerror("illegal MOVEP");
			w0 |= yypvt[-3].ival << 16 | 0 << 15 | yypvt[-0].regs.r6 << 8 | (yypvt[-2].ea.ext & 0x3F);} break;
case 175:
# line 952 "a56.y"
{if(yypvt[-0].ival != -1 && yypvt[-0].ival >= 0xFFC0) {
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
case 176:
# line 964 "a56.y"
{yyval.ea.pp = 1;
			yyval.ea.mode = 0;
			if(yypvt[-0].ival < 0xFFC0 && pass == 2)
				yyerror("warning: address operand truncated");
			yyval.ea.ext = yypvt[-0].ival;} break;
case 177:
# line 970 "a56.y"
{yyval.ea.mode = yypvt[-0].ival << 8;
			yyval.ea.ext = yypvt[-0].ival;} break;
case 178:
# line 976 "a56.y"
{w0 |= 0x070000 | 0 << 15 | yypvt[-3].regs.r6;} break;
case 179:
# line 978 "a56.y"
{w0 |= 0x070000 | 1 << 15 | yypvt[-0].regs.r6;} break;
case 180:
# line 985 "a56.y"
{yyval.ival = yypvt[-0].ival; if(yypvt[-0].ival > 0x17) yyerror("illegal bit number");} break;
case 181:
# line 989 "a56.y"
{yyval.ival = 0;} break;
case 182:
# line 991 "a56.y"
{yyval.ival = 1;} break;
case 183:
# line 997 "a56.y"
{w0 |= 0x004080;} break;
case 184:
# line 999 "a56.y"
{w0 |= (yypvt[-0].ival & 0x3F) << 8;
			if(yypvt[-0].ival > 0x003F && pass == 2)
				yyerror("warning: address operand truncated");
			} break;
case 185:
# line 1006 "a56.y"
{yyval.ival = 1;} break;
case 186:
# line 1008 "a56.y"
{w0 |= yypvt[-0].ival & 0xFFF; yyval.ival = 0;
			if(yypvt[-0].ival > 0x0FFF && pass == 2)
				yyerror("warning: address operand truncated");
			} break;
case 187:
# line 1015 "a56.y"
{w0 |= 0x003000;
			uses_w1++;
			w1 |= yypvt[-0].ival;
			yyval.ival = 0x003000;} break;
case 188:
# line 1020 "a56.y"
{w0 |= yypvt[-0].ival << 8;
			yyval.ival = yypvt[-0].ival << 8;} break;
case 189:
# line 1026 "a56.y"
{yyval.ival = yypvt[-0].ival;} break;
case 190:
# line 1028 "a56.y"
{yyval.ival = 4 << 3 | yypvt[-1].ival;} break;
case 191:
# line 1030 "a56.y"
{yyval.ival = 5 << 3 | yypvt[-3].ival;
			if(yypvt[-3].ival != yypvt[-1].ival) yyerror("Rn and Nn must be same number");} break;
case 192:
# line 1033 "a56.y"
{yyval.ival = 7 << 3 | yypvt[-1].ival;} break;
case 193:
# line 1038 "a56.y"
{yyval.ival = 0 << 3 | yypvt[-3].ival;
			if(yypvt[-3].ival != yypvt[-0].ival) yyerror("Rn and Nn must be same number");} break;
case 194:
# line 1041 "a56.y"
{yyval.ival = 1 << 3 | yypvt[-3].ival;
			if(yypvt[-3].ival != yypvt[-0].ival) yyerror("Rn and Nn must be same number");} break;
case 195:
# line 1044 "a56.y"
{yyval.ival = 2 << 3 | yypvt[-2].ival;} break;
case 196:
# line 1046 "a56.y"
{yyval.ival = 3 << 3 | yypvt[-2].ival;} break;
case 197:
# line 1052 "a56.y"
{yyval.regs.r6 = yyval.regs.r5 = 0x04 | yypvt[-0].ival;
			yyval.regs.sdx = yypvt[-0].ival;
			yyval.regs.xreg = yypvt[-0].ival;
			yyval.regs.flags = R_R6|R_R5|R_XREG|R_SDX;} break;
case 198:
# line 1057 "a56.y"
{yyval.regs.r6 = yyval.regs.r5 = 0x06 | yypvt[-0].ival;
			yyval.regs.sdy = yypvt[-0].ival;
			yyval.regs.yreg = yypvt[-0].ival;
			yyval.regs.flags = R_R6|R_R5|R_SDY|R_YREG;} break;
case 199:
# line 1062 "a56.y"
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
			yyval.regs.flags = R_R6|R_R5;} break;
case 200:
# line 1075 "a56.y"
{switch(yypvt[-0].ival) {
				case 0: 
					yyval.regs.r6 = yyval.regs.r5 = 0x08 | 1; break;
				case 1: 
					yyval.regs.r6 = yyval.regs.r5 = 0x08 | 5; break;
				case 2: 
					yyval.regs.r6 = yyval.regs.r5 = 0x08 | 3; break;
			}
			yyval.regs.flags = R_R6|R_R5;} break;
case 201:
# line 1085 "a56.y"
{yyval.regs.r6 = yyval.regs.r5 = 0x0E;
			yyval.regs.sdx = yyval.regs.sdy = 0x2;
			yyval.regs.ab = 0;
			yyval.regs.lsd = 4;
			yyval.regs.flags = R_R6|R_R5|R_SDX|R_SDY|R_AB|R_LSD;} break;
case 202:
# line 1091 "a56.y"
{yyval.regs.r6 = yyval.regs.r5 = 0x0F;
			yyval.regs.sdx = yyval.regs.sdy = 0x3;
			yyval.regs.ab = 1;
			yyval.regs.lsd = 5;
			yyval.regs.flags = R_R6|R_R5|R_SDX|R_SDY|R_AB|R_LSD;} break;
case 203:
# line 1097 "a56.y"
{yyval.regs.r6 = yyval.regs.r5 = 0x10 | yypvt[-0].ival;
			yyval.regs.r4 = 0x00 | yypvt[-0].ival;
			yyval.regs.flags = R_R6|R_R5|R_R4;} break;
case 204:
# line 1101 "a56.y"
{yyval.regs.r6 = yyval.regs.r5 = 0x18 | yypvt[-0].ival;
			yyval.regs.r4 = 0x08 | yypvt[-0].ival;
			yyval.regs.flags = R_R6|R_R5|R_R4;} break;
case 205:
# line 1105 "a56.y"
{yyval.regs.r6 = 0x20 | yypvt[-0].ival;
			yyval.regs.r5 = -1;
			yyval.regs.ctl_reg = yypvt[-0].ival;
			yyval.regs.flags = R_R6|R_R5|R_CTL_REG;} break;
case 206:
# line 1110 "a56.y"
{yyval.regs.r6 = 0x38 | yypvt[-0].ival;
			yyval.regs.r5 = -1;
			yyval.regs.ctl_reg = 0x18 | yypvt[-0].ival;
			yyval.regs.flags = R_R6|R_R5|R_CTL_REG;} break;
case 207:
# line 1115 "a56.y"
{yyval.regs.lsd  = 0;
			yyval.regs.flags = R_LSD;} break;
case 208:
# line 1118 "a56.y"
{yyval.regs.lsd = 1;
			yyval.regs.flags = R_LSD;} break;
case 209:
# line 1121 "a56.y"
{yyval.regs.lsd = 2;
			yyval.regs.flags = R_LSD;} break;
case 210:
# line 1124 "a56.y"
{yyval.regs.lsd = 3;
			yyval.regs.flags = R_LSD;} break;
case 211:
# line 1127 "a56.y"
{yyval.regs.lsd = 6;
			yyval.regs.flags = R_LSD;} break;
case 212:
# line 1130 "a56.y"
{yyval.regs.lsd = 7;
			yyval.regs.flags = R_LSD;} break;
case 213:
# line 1136 "a56.y"
{yyval.ival = 1;} break;
case 214:
# line 1138 "a56.y"
{yyval.ival = 2;} break;
case 215:
# line 1140 "a56.y"
{yyval.ival = 3;} break;
case 216:
# line 1142 "a56.y"
{yyval.ival = 4;} break;
case 217:
# line 1144 "a56.y"
{yyval.ival = 5;} break;
case 218:
# line 1146 "a56.y"
{yyval.ival = 6;} break;
case 219:
# line 1148 "a56.y"
{yyval.ival = 7;} break;
case 220:
# line 1153 "a56.y"
{yyval.ival = 0;} break;
case 221:
# line 1155 "a56.y"
{yyval.ival = 1;} break;
case 222:
# line 1157 "a56.y"
{yyval.ival = 2;} break;
case 231:
# line 1174 "a56.y"
{if(yypvt[-2].ival <= 0xFF) {
				w0 |= 0x200000 | (yypvt[-2].ival & 0xFF) << 8 | 
					yypvt[-0].regs.r5 << 16;
			} else { /* go long, use X: format */
				w0 |= 0x400000 | 0x00F400 | 
					(yypvt[-0].regs.r5 >> 3 & 3) << 20 | 
					(yypvt[-0].regs.r5 & 7) << 16;
				uses_w1++; w1 = yypvt[-2].ival;
			}} break;
case 232:
# line 1186 "a56.y"
{
				if(yypvt[-2].regs.flags & R_R5 & yypvt[-0].regs.flags) 
					w0 |= 0x200000 | yypvt[-0].regs.r5 << 8 | yypvt[-2].regs.r5 << 13;
				else
					yyerror("illegal R move");
			} break;
case 233:
# line 1195 "a56.y"
{w0 |= 0x204000 | yypvt[-0].ival << 8;} break;
case 234:
# line 1200 "a56.y"
{w0 |= 0x40C000 | yypvt[-3].ival << 19;
			w0 |= (yypvt[-0].regs.r5 >> 3 & 3) << 20 | (yypvt[-0].regs.r5 & 7) << 16;} break;
case 235:
# line 1203 "a56.y"
{w0 |= 0x408000 | yypvt[-3].ival << 19 | (yypvt[-2].ival & 0x3F) << 8;
			if(yypvt[-2].ival > 0x003F && pass == 2)
				yyerror("warning: address operand truncated");
			w0 |= (yypvt[-0].regs.r5>> 3 & 3) << 20 | (yypvt[-0].regs.r5 & 7) << 16;} break;
case 236:
# line 1208 "a56.y"
{w0 |= 0x404000 | yypvt[-1].ival << 19;
			w0 |= (yypvt[-3].regs.r5 >> 3 & 3) << 20 | (yypvt[-3].regs.r5 & 7) << 16;} break;
case 237:
# line 1211 "a56.y"
{w0 |= 0x400000 | yypvt[-1].ival << 19 | (yypvt[-0].ival & 0x3F) << 8;
			if(yypvt[-0].ival > 0x003F && pass == 2)
				yyerror("warning: address operand truncated");
			w0 |= (yypvt[-3].regs.r5 >> 3 & 3) << 20 | (yypvt[-3].regs.r5 & 7) << 16;} break;
case 238:
# line 1216 "a56.y"
{w0 |= 0x400000 | 0x00F400 | (yypvt[-0].regs.r5 >> 3 & 3) << 20 |
			    (yypvt[-0].regs.r5 & 7) << 16;
			uses_w1++; w1 = yypvt[-2].ival;
			} break;
case 239:
# line 1223 "a56.y"
{if(yypvt[-6].ival == 0 && yypvt[-2].regs.flags & R_AB) {
				w0 |= 0x108000 | yypvt[-3].regs.sdx << 18 | yypvt[-2].regs.ab << 17 |
					yypvt[-0].ival << 16;
			} else {
				yyerror("illegal X:R move");
			}} break;
case 240:
# line 1230 "a56.y"
{if(yypvt[-2].regs.flags & R_AB) {
				w0 |= 0x10B400 | yypvt[-3].regs.sdx << 18 | yypvt[-2].regs.ab << 17 |
					yypvt[-0].ival << 16;
				uses_w1++;
				w1 |= yypvt[-5].ival & 0xFFFFFF;
			} else {
				yyerror("illegal X:R move");
			}} break;
case 241:
# line 1239 "a56.y"
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
case 242:
# line 1258 "a56.y"
{if(yypvt[-4].regs.flags & R_XREG && yypvt[-0].regs.flags & (R_YREG|R_AB)) {
				w0 |= 0x10C000 | yypvt[-6].regs.ab << 19 | yypvt[-4].regs.yreg << 18 |
					yypvt[-0].regs.sdy << 16;
			} else {
				yyerror("illegal R:Y move");
			}} break;
case 243:
# line 1265 "a56.y"
{if(yypvt[-3].regs.flags & R_XREG && yypvt[-0].regs.flags & (R_YREG|R_AB)) {
				w0 |= 0x10F400 | yypvt[-5].regs.ab << 19 | yypvt[-3].regs.xreg << 18 |
					yypvt[-0].regs.sdy << 16;
				uses_w1++;
				w1 |= yypvt[-2].ival & 0xFFFFFF;
			} else {
				yyerror("illegal R:Y move");
			}} break;
case 244:
# line 1274 "a56.y"
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
case 245:
# line 1292 "a56.y"
{w0 |= 0x40C000 | (yypvt[-0].regs.lsd & 3) << 16 | (yypvt[-0].regs.lsd >> 2) << 19;} break;
case 246:
# line 1294 "a56.y"
{w0 |= 0x404000 | (yypvt[-3].regs.lsd & 3) << 16 | (yypvt[-3].regs.lsd >> 2) << 19;} break;
case 247:
# line 1296 "a56.y"
{w0 |= 0x408000 | (yypvt[-0].regs.lsd & 3) << 16 | (yypvt[-0].regs.lsd >> 2) << 19;
			if(yypvt[-2].ival > 0x003F && pass == 2)
				yyerror("warning: address operand truncated");
			w0 |= (yypvt[-2].ival & 0x3F) << 8;} break;
case 248:
# line 1301 "a56.y"
{w0 |= 0x400000 | (yypvt[-3].regs.lsd & 3) << 16 | (yypvt[-3].regs.lsd >> 2) << 19;
			if(yypvt[-0].ival > 0x003F && pass == 2)
				yyerror("warning: address operand truncated");
			w0 |= (yypvt[-0].ival & 0x3F) << 8;} break;
case 249:
# line 1309 "a56.y"
{int eax = yypvt[-6].ival, eay = yypvt[-2].ival;
			w0 = 0xC08000;	/* both write */
	      		if(eax & 0x4 == eay & 0x4)
				yyerror("registers must be in opposite halves");
			w0 |= eax << 8 | (eay & 3) << 13 | (eay >> 3) << 18;} break;
case 250:
# line 1315 "a56.y"
{int eax = yypvt[-6].ival, eay = yypvt[-0].ival;
			w0 = 0x808000;	/* X:write, Y:read */
	      		if(eax & 0x4 == eay & 0x4)
				yyerror("registers must be in opposite halves");
			w0 |= eax << 8 | (eay & 3) << 13 | (eay >> 3) << 18;} break;
case 251:
# line 1321 "a56.y"
{int eax = yypvt[-4].ival, eay = yypvt[-2].ival;
	      		w0 = 0xC00000;	/* X:read, Y:write */
			if(eax & 0x4 == eay & 0x4)
				yyerror("registers must be in opposite halves");
			w0 |= eax << 8 | (eay & 3) << 13 | (eay >> 3) << 18;} break;
case 252:
# line 1327 "a56.y"
{int eax = yypvt[-4].ival, eay = yypvt[-0].ival;
	      		w0 = 0x800000;	/* both read */
			if(eax & 0x4 == eay & 0x4)
				yyerror("registers must be in opposite halves");
			w0 |= eax << 8 | (eay & 3) << 13 | (eay >> 3) << 18;} break;
case 253:
# line 1337 "a56.y"
{yyval.ival = yypvt[-0].ival;} break;
case 254:
# line 1339 "a56.y"
{yyval.ival = yypvt[-0].ival;} break;
case 255:
# line 1341 "a56.y"
{yyval.ival = yypvt[-0].ival;} break;
case 256:
# line 1345 "a56.y"
{yyval.ival = yypvt[-0].ival;} break;
case 257:
# line 1347 "a56.y"
{yyval.ival = yypvt[-0].ival & 0xFF;} break;
case 258:
# line 1351 "a56.y"
{yyval.ival = yypvt[-0].ival;} break;
case 259:
# line 1356 "a56.y"
{yyval.ival = yypvt[-0].ival;} break;
case 260:
# line 1358 "a56.y"
{yyval.ival = pc;} break;
case 261:
# line 1363 "a56.y"
{yyval.ival = yypvt[-0].ival;} break;
case 262:
# line 1365 "a56.y"
{yyval.ival = pc;} break;
case 263:
# line 1370 "a56.y"
{yyval.ival = yypvt[-0].ival;} break;
case 264:
# line 1375 "a56.y"
{yyval.ival = yypvt[-0].ival & 0xFFFFFF;} break;
case 265:
# line 1377 "a56.y"
{yyval.ival = sym_ref(yypvt[-0].sval); free(yypvt[-0].sval);} break;
case 266:
# line 1382 "a56.y"
{yyval.ival = yypvt[-0].ival;} break;
case 267:
# line 1384 "a56.y"
{yyval.ival = sym_ref(yypvt[-0].sval); free(yypvt[-0].sval);} break;
case 268:
# line 1386 "a56.y"
{yyval.ival = yypvt[-0].cval;} break;
case 269:
# line 1390 "a56.y"
{yyval.ival = yypvt[-2].ival | yypvt[-0].ival;} break;
case 270:
# line 1392 "a56.y"
{yyval.ival = yypvt[-2].ival ^ yypvt[-0].ival;} break;
case 271:
# line 1394 "a56.y"
{yyval.ival = yypvt[-2].ival & yypvt[-0].ival;} break;
case 272:
# line 1396 "a56.y"
{yyval.ival = yypvt[-2].ival >> yypvt[-0].ival;} break;
case 273:
# line 1398 "a56.y"
{yyval.ival = yypvt[-2].ival << yypvt[-0].ival;} break;
case 274:
# line 1400 "a56.y"
{yyval.ival = yypvt[-2].ival - yypvt[-0].ival;} break;
case 275:
# line 1402 "a56.y"
{yyval.ival = yypvt[-2].ival + yypvt[-0].ival;} break;
case 276:
# line 1404 "a56.y"
{yyval.ival = yypvt[-2].ival % yypvt[-0].ival;} break;
case 277:
# line 1406 "a56.y"
{yyval.ival = yypvt[-2].ival / yypvt[-0].ival;} break;
case 278:
# line 1408 "a56.y"
{yyval.ival = yypvt[-2].ival * yypvt[-0].ival;} break;
case 279:
# line 1410 "a56.y"
{yyval.ival = - yypvt[-0].ival;} break;
case 280:
# line 1412 "a56.y"
{yyval.ival = ~ yypvt[-0].ival;} break;
case 281:
# line 1414 "a56.y"
{yyval.ival = yypvt[-1].ival;} break;
case 282:
# line 1416 "a56.y"
{yyval.ival = yypvt[-0].ival;} break;
		}
		goto yystack;  /* stack new state and value */

	}
