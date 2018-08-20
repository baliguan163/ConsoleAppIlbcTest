/******************************************************************
iLBC Speech Coder ANSI-C Source Code
createCB.h
Copyright (C) The Internet Society (2004).
All Rights Reserved.
******************************************************************/

#ifndef __iLBC_CREATECB_H
#define __iLBC_CREATECB_H

void filteredCBvecs(float *cbvectors, float *mem, int lMem);

void searchAugmentedCB(int low, int high, int stage, int startIndex, float *target, 
                       float *buffer, float *max_measure, int *best_index,
                       float *gain, float *energy, float *invenergy);

void createAugmentedVec(int index, float *buffer, float *cbVec);

#endif

