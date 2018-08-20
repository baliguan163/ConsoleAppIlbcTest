/******************************************************************
iLBC Speech Coder ANSI-C Source Code
gainquant.h
Copyright (C) The Internet Society (2004).
All Rights Reserved.
******************************************************************/
#ifndef __iLBC_GAINQUANT_H
#define __iLBC_GAINQUANT_H

float gainquant(float in, float maxIn, int cblen, int *index);

float gaindequant(int index, float maxIn, int cblen);

#endif

