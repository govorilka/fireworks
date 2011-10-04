#include "fwchartype.h"

//Types of ASCII chars
const quint8 Fw::chars_type[CHARS_COUNT] =
{
          /*0 */ /*1 */ /*2 */ /*3 */ /*4 */ /*5 */ /*6 */ /*7 */
/*  0 */  C_Err, C_Err, C_Err, C_Err, C_Err, C_Err, C_Err, C_Err,
/*  8 */  C_Err, C_Sp,  C_Sp,  C_Err, C_Err, C_Sp,  C_Err, C_Err,
/* 16 */  C_Err, C_Err, C_Err, C_Err, C_Err, C_Err, C_Err, C_Err,
/* 24 */  C_Err, C_Err, C_Err, C_Err, C_Err, C_Err, C_Err, C_Err,

/* 32 */  C_Sp,  C_Uni, C_Str, C_Uni, C_Uni, C_Uni, C_Uni, C_Uni,
/* 40 */  C_Uni, C_Uni, C_Uni, C_Sig, C_Sep, C_Sig, C_Fra, C_Uni,
/* 48 */  C_Num, C_Num, C_Num, C_Num, C_Num, C_Num, C_Num, C_Num,
/* 56 */  C_Num, C_Num, C_Col, C_Sep, C_Uni, C_Uni, C_Uni, C_Que,

/* 64 */  C_Uni, C_AZ,  C_AZ,  C_AZ,  C_AZ,  C_Ee,  C_AZ,  C_AZ,
/* 72 */  C_AZ,  C_AZ,  C_AZ,  C_AZ,  C_AZ,  C_AZ,  C_AZ,  C_AZ,
/* 80 */  C_AZ,  C_AZ,  C_AZ,  C_AZ,  C_AZ,  C_AZ,  C_AZ,  C_AZ,
/* 88 */  C_AZ,  C_AZ,  C_AZ,  C_LSq, C_Esc, C_RSq, C_Uni, C_Uni,

/* 96 */  C_Uni, C_AZ,  C_AZ,  C_AZ,  C_AZ,  C_Ee,  C_AZ,  C_AZ,
/* 104*/  C_AZ,  C_AZ,  C_AZ,  C_AZ,  C_AZ,  C_AZ,  C_AZ,  C_AZ,
/* 112*/  C_AZ,  C_AZ,  C_AZ,  C_AZ,  C_AZ,  C_AZ,  C_AZ,  C_AZ,
/* 120*/  C_AZ,  C_AZ,  C_AZ,  C_LCu, C_Uni, C_RCu, C_Uni, C_Err,

/* 128*/  C_Uni, C_Uni, C_Uni, C_Uni, C_Uni, C_Uni, C_Uni, C_Uni,
/* 136*/  C_Uni, C_Uni, C_Uni, C_Uni, C_Uni, C_Uni, C_Uni, C_Uni,
/* 144*/  C_Uni, C_Uni, C_Uni, C_Uni, C_Uni, C_Uni, C_Uni, C_Uni,
/* 152*/  C_Uni, C_Uni, C_Uni, C_Uni, C_Uni, C_Uni, C_Uni, C_Uni,

/* 160*/  C_Uni, C_Uni, C_Uni, C_Uni, C_Uni, C_Uni, C_Uni, C_Uni,
/* 168*/  C_Uni, C_Uni, C_Uni, C_Uni, C_Uni, C_Uni, C_Uni, C_Uni,
/* 176*/  C_Uni, C_Uni, C_Uni, C_Uni, C_Uni, C_Uni, C_Uni, C_Uni,
/* 184*/  C_Uni, C_Uni, C_Uni, C_Uni, C_Uni, C_Uni, C_Uni, C_Uni,

/* 192*/  C_Uni, C_Uni, C_Uni, C_Uni, C_Uni, C_Uni, C_Uni, C_Uni,
/* 200*/  C_Uni, C_Uni, C_Uni, C_Uni, C_Uni, C_Uni, C_Uni, C_Uni,
/* 208*/  C_Uni, C_Uni, C_Uni, C_Uni, C_Uni, C_Uni, C_Uni, C_Uni,
/* 216*/  C_Uni, C_Uni, C_Uni, C_Uni, C_Uni, C_Uni, C_Uni, C_Uni,

/* 224*/  C_Uni, C_Uni, C_Uni, C_Uni, C_Uni, C_Uni, C_Uni, C_Uni,
/* 232*/  C_Uni, C_Uni, C_Uni, C_Uni, C_Uni, C_Uni, C_Uni, C_Uni,
/* 240*/  C_Uni, C_Uni, C_Uni, C_Uni, C_Uni, C_Uni, C_Uni, C_Uni,
/* 248*/  C_Uni, C_Uni, C_Uni, C_Uni, C_Uni, C_Uni, C_Uni, C_Uni
};
