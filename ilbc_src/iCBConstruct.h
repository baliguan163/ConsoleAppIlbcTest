/******************************************************************
iLBC Speech Coder ANSI-C Source Code
iCBConstruct.h
Copyright (C) The Internet Society (2004).
All Rights Reserved.
******************************************************************/
#ifndef __iLBC_ICBCONSTRUCT_H
#define __iLBC_ICBCONSTRUCT_H

void index_conv_enc(int *index);

void index_conv_dec(int *index);

void iCBConstruct(float *decvector, int *index, int *gain_index,/* (i) Gain quantization indices */
                  float *mem, int lMem, int veclen, int nStages);

#endif

