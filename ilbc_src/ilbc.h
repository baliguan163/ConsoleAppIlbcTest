#ifndef _ILBC_H
#define _ILBC_H

#include <mmsystem.h> 
//宏定义,如果系统没有声明WINAPI,则进行声明,_stdcall为标准调用方式
#ifndef WINAPI
	#define WINAPI _stdcall    
#endif 

//////////////////////////////////////////

#ifdef __iLBC_DLL_IMPORT

	#include "iLBC_encode.h"
	#include "iLBC_decode.h"

	iLBC_Enc_Inst_t iLBCenc_inst;
	iLBC_Dec_Inst_t iLBCdec_inst;

	#define iLBC_API(type, name , args) \
		extern "C" type __declspec(dllexport) WINAPI name  args;
#else
	#define iLBC_API(type, name , args) \
		extern "C" type __declspec(dllimport) WINAPI name  args;
#endif 

///////////////////////////////////////////////////////////////////
iLBC_API(bool, ilbc_init, ());
iLBC_API(int, ilbc_encoder, (short *pin,unsigned char *pout));
iLBC_API(int, ilbc_decoder, (unsigned char *pin,short *pout));

#endif