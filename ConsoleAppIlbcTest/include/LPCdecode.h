/******************************************************************
   iLBC Speech Coder ANSI-C Source Code
   LPC_decode.h
   Copyright (C) The Internet Society (2004).
   All Rights Reserved.
******************************************************************/
#ifndef __iLBC_LPC_DECODE_H
#define __iLBC_LPC_DECODE_H

void LSFinterpolate2a_dec(float *a, float *lsf1, float *lsf2, float coef, int length);

void SimplelsfDEQ(float *lsfdeq, int *index, int lpc_n);

void DecoderInterpolateLSF(float *syntdenum, float *weightdenum, float *lsfdeq, int length, iLBC_Dec_Inst_t *iLBCdec_inst);

#endif

