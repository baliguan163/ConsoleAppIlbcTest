/******************************************************************
   iLBC Speech Coder ANSI-C Source Code
   StateSearchW.h
   Copyright (C) The Internet Society (2004).
   All Rights Reserved.
******************************************************************/
#ifndef __iLBC_STATESEARCHW_H
#define __iLBC_STATESEARCHW_H

void AbsQuantW(iLBC_Enc_Inst_t *iLBCenc_inst, float *in, float *syntDenum, 
               float *weightDenum, int *out, int len, int state_first);

void StateSearchW(iLBC_Enc_Inst_t *iLBCenc_inst, float *residual, float *syntDenum, 
                  float *weightDenum, int *idxForMax, int *idxVec, int len, int state_first);

#endif

