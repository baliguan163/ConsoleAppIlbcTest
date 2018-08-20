/******************************************************************
    iLBC Speech Coder ANSI-C Source Code
    iLBC_decode.h
    Copyright (C) The Internet Society (2004).
    All Rights Reserved.
******************************************************************/
#ifndef __iLBC_ILBCDECODE_H
#define __iLBC_ILBCDECODE_H

#include "iLBC_define.h"

short initDecode(iLBC_Dec_Inst_t *iLBCdec_inst, int mode, int use_enhancer);
void iLBC_decode(float *decblock, unsigned char *bytes, iLBC_Dec_Inst_t *iLBCdec_inst, int mode);

#endif

