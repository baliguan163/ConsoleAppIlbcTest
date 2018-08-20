//////////////////////////////////////////////////////////////////////
/*	
//		ilbc.cpp: interface of ILBC Codec.
//
//					CHINA DIM PROPRIETARY INFORMATION
//			This ILBC Codec Dynamic Library	 is programmed by China DIM,this supply the 
//		codec interface for audio data.iLBC audio codec designed for providing steady
//		IP voice communication; sampling frequency is 8000(8kHz) ; iLBC codec suport tow
//		type of frame:  30ms(13.3kps) , 20ms(15.2kps).

  Copyright (c) 2002-2003 China DIM Corporation. All Rights Reserved.
//
//
//                  China DIM(R) ILBC Codec Dynamic Library	
//
//		Developer:			JasonLiu
//		Accomplish Date:	2007-06-07
*/////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////
#include "stdafx.h"


#ifndef __iLBC_DLL_IMPORT
#define __iLBC_DLL_IMPORT
#include "ilbc.h"
#endif

BOOL APIENTRY DllMain( HANDLE hModule, 
                       DWORD  ul_reason_for_call, 
                       LPVOID lpReserved
					 )
{
	switch (ul_reason_for_call)
	{
	case DLL_PROCESS_ATTACH:
		//printf("\nprocess attach of dll");
		break;
	case DLL_THREAD_ATTACH:
		//printf("\nthread attach of dll");
		break;
	case DLL_THREAD_DETACH:
		//printf("\nthread detach of dll");
		break;
	case DLL_PROCESS_DETACH:
		//printf("\nprocess detach of dll");
		break;
	}
    return TRUE;
}

////////////////////////////////////////////////////////////////////
/*��������: ilbc_init()
 *��������:	��ʼ��ILBC
 *�����:������(JasonLiu)
 *���ʱ��: 2007-06-06
 *����޸�ʱ��: 2007-06-07
 */
////////////////////////////////////////////////////////////////////////
bool WINAPI ilbc_init()
{
	//���¼��ϵͳ��WAVEFORMATEX�����Ƿ����ILBC���������
	//if(waveFormat->nSamplesPerSec != 8000 ||
	//		waveFormat->wBitsPerSample != 16 )
	//	return false;

	//��ʼ��ILBC,��ʼ��ILBC����
	//����: 1. �ṹ��iLBC_Enc_Inst_t����
	//		2. �����frame����,����Ϊ30ms����20ms
	initEncode(&iLBCenc_inst, 20);

	//��ʼ��ILBC,��ʼ��ILBC����
	//����: 1. �ṹ��iLBC_Dnc_Inst_t����
	//		2. �����frame����,����Ϊ30ms����20ms
	//		3. 1 to use enhancer 0 to run without enhancer
	initDecode(&iLBCdec_inst, 20, 1);
	return true;
}

////////////////////////////////////////////////////////////////////
/*��������: ilbc_encoder(short *pin,unsigned char* pout)
 *��������:	ILBC����
 *����˵��:
 *		@short *pin
 *		@unsigned char* pout
 *�����:������(JasonLiu)
 *���ʱ��: 2007-06-06
 *����޸�ʱ��: 2007-06-07
 */
////////////////////////////////////////////////////////////////////////
int WINAPI ilbc_encoder(short *pin,unsigned char* pout)
{	
	int k;
	float block[BLOCKL_MAX];

	/* convert signal to float */
	for (k=0; k<iLBCenc_inst.blockl; k++)
		block[k] = (float)pin[k];

	/* do the actual encoding */

	iLBC_encode(pout, block, &iLBCenc_inst);

	return (iLBCenc_inst.no_of_bytes);
}

int WINAPI ilbc_decoder(unsigned char *pin ,short *pout)
{
	int k;
	float decblock[BLOCKL_MAX], dtmp;

		/* do actual decoding of block */

	iLBC_decode(decblock, (unsigned char *)pin, &iLBCdec_inst, 1);

	/* convert to short */

	for (k=0; k<iLBCdec_inst.blockl; k++)
	{
		dtmp=decblock[k];
		if (dtmp<MIN_SAMPLE)
		{
			dtmp=MIN_SAMPLE;
		}
		else if (dtmp>MAX_SAMPLE)
		{
			dtmp=MAX_SAMPLE;
		}

		pout[k] = (short)dtmp;
	}

	return (iLBCdec_inst.blockl);
}


