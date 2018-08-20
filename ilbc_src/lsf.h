/******************************************************************
   iLBC Speech Coder ANSI-C Source Code
   lsf.h
   Copyright (C) The Internet Society (2004).
   All Rights Reserved.
******************************************************************/
#ifndef __iLBC_LSF_H
#define __iLBC_LSF_H

void a2lsf(float *freq, float *a);

void lsf2a(float *a_coef, float *freq);

#endif

