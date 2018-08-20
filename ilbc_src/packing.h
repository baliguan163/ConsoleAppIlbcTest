/******************************************************************
   iLBC Speech Coder ANSI-C Source Code
   packing.h
   Copyright (C) The Internet Society (2004).
   All Rights Reserved.
******************************************************************/
#ifndef __PACKING_H
#define __PACKING_H

void packsplit(int *index, int *firstpart, int *rest, int bitno_firstpart, int bitno_total);

void packcombine(int *index, int rest, int bitno_rest);

void dopack(unsigned char **bitstream, int index, int bitno, int *pos);

void unpack(unsigned char **bitstream, int *index, int bitno, int *pos);

#endif

