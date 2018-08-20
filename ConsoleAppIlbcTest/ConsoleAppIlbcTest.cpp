// ConsoleAppIlbcTest.cpp : 定义控制台应用程序的入口点。
//

#include "stdafx.h"

/******************************************************************
iLBC Speech Coder ANSI-C Source Code
iLBC_test.c
Copyright (C) The Internet Society (2004).
All Rights Reserved.
******************************************************************/
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <time.h>
#include <memory>

#include <windows.h> 
#include <mmsystem.h>
#include <iostream>
#include<fstream> 
#include<math.h>
using namespace std;

#include "dr_wav.h"
#include "iLBC_define.h"
#include "iLBC_encode.h"
#include "iLBC_decode.h"

#define ILBCNOOFWORDS_MAX   (NO_OF_BYTES_30MS/2) //50/2
#define WAVE_HEAD_LENGTH 44 //wav头文件长度
#define m_samplefreq 16000
#define m_channels 1
#define m_channelbits 16
#define MATH_PI 3.1415


//ofstream ocout;
//ocout.open("newWave.wav", ios::out | ios::binary);//以二进制形式打开文件
//if (ocout)
//	ocout.write(pWaveBuffer, totalLen);
//else
//	return 0;
//ocout.close();

//三个参数：声道数、采样率和量化精度，将文件头信息写入后，紧接这写入PCM数据就完成了WAV文件的写入
//ocout.write((char*)&head, sizeof head);//将文件头部分写进文件

/*uint32_t length = header.fmt_data->sample_per_sec * 10 * header.fmt_data->bits_per_sample / 8;
uint8_t *data2 = new uint8_t[length];
memset(data2, 0x80, length);
printf("length:%s\n", length);*/
//fwrite((char*)&header, 1, sizeof header, wavfileid);



/*Wave_header header(1, 1600, 16, num);
printf("写入头:%d\n", sizeof(header));
printf("写入数据:%d\n", num);
fwrite((char*)&header, 1, sizeof(header), wavfileid);
fwrite((char*)pos, 1, num, wavfileid);*/

/*for (int i = 0; i < num; i++)
printf("%lf\n", pos[i]);*/
//delete(pHeader);

using namespace std;
//.wav文件的文件头结构 
typedef struct
{
	char chRIFF[4];
	DWORD dwRIFFLen;
	char chWAVE[4];
	char chFMT[4];
	DWORD dwFMTLen;
	PCMWAVEFORMAT pwf;
	char chDATA[4];
	DWORD dwDATALen;
	//UINT8* pBufer;
}WaveHeader;
void MakeWaveData(int rate, int freq, int amp, char* p, int len)//采样率、频率、音量、采样点数
{
	int flag = 0;
	if (m_channelbits == 16)        //16位
	{
		if (m_channels == 1)
		{
			for (int i = 0; i < len; i++)
			{
				INT16 v = amp / 100 * 32768 * sin(2 * MATH_PI * freq * i / rate);
				*(p + flag) = v & 0xFF;//低8位
				*(p + flag + 1) = (v >> 8) & 0xFF;//16bit量化 高8位
				flag += 2;
			}
		}
		else
		{
			for (int i = 0; i < len; i++)
			{
				INT16 vl = amp / 100 * 32768 * sin(2 * MATH_PI * freq * i / rate);
				INT16 vr = amp / 100 * 32768 * sin((2 * MATH_PI * freq * (i + 5)) / rate);
				*(p + flag) = (vl & 0xFF);
				*(p + flag + 1) = ((vl >> 8) & 0xFF);
				*(p + flag + 2) = (vr & 0xFF);
				*(p + flag + 3) = ((vr >> 8) & 0xFF);
				flag += 4;
			}
		}
	}
	else
	{
		if (m_channels == 1)
		{
			for (int i = 0; i < len; i++)
			{
				*(p + i) = sin(i * (MATH_PI * 2) / rate * freq) * amp * 128 / 100 + 128;
			}
		}
		else
		{
			for (int i = 0; i < len; i++)
			{
				*(p + flag) = sin(i * (MATH_PI * 2) / rate * freq) * amp * 128 / 100 + 128;
				*(p + flag + 1) = sin((i + 5) * (MATH_PI * 2) / rate * freq) * amp * 128 / 100 + 128;
				flag += 2;
			}
		}
	}
}






short encode(   /* (o) Number of bytes encoded */
	iLBC_Enc_Inst_t *iLBCenc_inst,/* (i/o) Encoder instance */
	short *encoded_data,    /* (o) The encoded bytes */
	short *data                 /* (i) The signal block to encode*/
	){
	float block[BLOCKL_MAX];
	int k;
	/* convert signal to float */
	for (k = 0; k<iLBCenc_inst->blockl; k++)
		block[k] = (float)data[k];
	/* do the actual encoding */
	iLBC_encode((unsigned char *)encoded_data, block, iLBCenc_inst);
	return (iLBCenc_inst->no_of_bytes);
}

short decode(       /* (o) Number of decoded samples */
	iLBC_Dec_Inst_t *iLBCdec_inst,  /* (i/o) Decoder instance */
	short *decoded_data,        /* (o) Decoded signal block*/
	short *encoded_data,        /* (i) Encoded bytes */
	short mode                       /* (i) 0=PL, 1=Normal */
	){
	int k;
	float decblock[BLOCKL_MAX], dtmp;

	/* check if mode is valid */

	if (mode<0 || mode>1) {
		printf("\nERROR - Wrong mode - 0, 1 allowed\n"); exit(3);
	}

	/* do actual decoding of block */

	iLBC_decode(decblock, (unsigned char *)encoded_data,
		iLBCdec_inst, mode);
	/* convert to short */
	for (k = 0; k<iLBCdec_inst->blockl; k++){
		dtmp = decblock[k];

		if (dtmp<MIN_SAMPLE)
			dtmp = MIN_SAMPLE;
		else if (dtmp>MAX_SAMPLE)
			dtmp = MAX_SAMPLE;
		decoded_data[k] = (short)dtmp;
	}

	return (iLBCdec_inst->blockl);
}

//bool wavWrite_int16(char *filename, int16_t *buffer, int sampleRate, uint32_t totalSampleCount) {
//	drwav_data_format format = {};
//	format.container = drwav_container_riff;     // <-- drwav_container_riff = normal WAV files, drwav_container_w64 = Sony Wave64.
//	format.format = DR_WAVE_FORMAT_PCM;          // <-- Any of the DR_WAVE_FORMAT_* codes.
//	format.channels = 1;
//	format.sampleRate = (drwav_uint32)sampleRate;
//	format.bitsPerSample = 16;
//	drwav *pWav = drwav_open_file_write(filename, &format);
//	if (pWav) {
//		drwav_uint64 samplesWritten = drwav_write(pWav, totalSampleCount, buffer);
//		drwav_uninit(pWav);
//		if (samplesWritten != totalSampleCount) {
//			fprintf(stderr, "ERROR\n");
//			return false;
//		}
//		return true;
//	}
//	return false;
//}

/*---------------------------------------------------------------*
*  Main program to test iLBC encoding and decoding
*
*  Usage:
*    exefile_name.exe <infile> <bytefile> <outfile> <channel>
*
*    <infile>   : Input file, speech for encoder (16-bit pcm file)
*    <bytefile> : Bit stream output from the encoder
*    <outfile>  : Output file, decoded speech (16-bit pcm file)
*    <channel>  : Bit error file, optional (16-bit)
*                     1 - Packet received correctly
*                     0 - Packet Lost
*
*--------------------------------------------------------------*/
//测试命令参数： 30 m0002.wav  m0002Encode.bin  m0002Decode.bin  m0002Decode.wav
int main(int argc, char* argv[])
{
	float starttime;
	float runtime;
	float outtime;

	FILE *ifileid, *efileid, *ofileid, *cfileid, *wavfileid;
	short data[BLOCKL_MAX];//240
	short encoded_data[ILBCNOOFWORDS_MAX], decoded_data[BLOCKL_MAX];//  50/2   240
	int len;
	short pli, mode;
	int blockcount = 0;
	int packetlosscount = 0;

	/* Create structs */
	iLBC_Enc_Inst_t Enc_Inst;
	iLBC_Dec_Inst_t Dec_Inst;

	/* get arguments and open files */
	if ((argc != 6) && (argc != 7)) {
		fprintf(stderr,"\n*-----------------------------------------------*\n");
		fprintf(stderr,"   %s <20,30> input encoded decoded (channel)\n\n",argv[0]);
		fprintf(stderr,"   mode    : Frame size for the encoding/decoding\n");
		fprintf(stderr,"                 20 - 20 ms\n");
		fprintf(stderr,"                 30 - 30 ms\n");
		fprintf(stderr,"   input   : Speech for encoder (16-bit pcm file)\n");
		fprintf(stderr,"   encoded : Encoded bit stream\n");
		fprintf(stderr,"   decoded : Decoded speech (16-bit pcm file)\n");
		fprintf(stderr,"   channel : Packet loss pattern, optional (16-bit)\n");
		fprintf(stderr,"                  1 - Packet received correctly\n");
		fprintf(stderr,"                  0 - Packet Lost\n");
		fprintf(stderr,"*-----------------------------------------------*\n\n");
		getchar();
		exit(1);
	}

	mode = atoi(argv[1]);
	if (mode != 20 && mode != 30) {
		fprintf(stderr, "Wrong mode %s, must be 20, or 30\n",argv[1]);
		getchar();
		exit(2);
	}
	if ((ifileid = fopen(argv[2], "rb")) == NULL) {
		fprintf(stderr, "Cannot open input file %s\n", argv[2]);
		getchar();
		exit(2);
	}
	if ((efileid = fopen(argv[3], "wb")) == NULL) {
		fprintf(stderr, "Cannot open encoded file %s\n",
			argv[3]); exit(1);
	}
	if ((ofileid = fopen(argv[4], "wb")) == NULL) {
		fprintf(stderr, "Cannot open decoded file %s\n",argv[4]); 
		getchar();
		exit(1);
	}
	if ((wavfileid = fopen(argv[5], "wb")) == NULL) {
		fprintf(stderr, "Cannot open decoded wav file %s\n", argv[5]);
		getchar();
		exit(1);
	}

	if (argc == 6) 
	{
		if ((cfileid = fopen(argv[5], "rb")) == NULL) 
		{
			fprintf(stderr, "Cannot open channel file %s\n",argv[5]);
			getchar();
			exit(1);
		}
	}
	else {
		cfileid = NULL;
	}

	/* print info */
	fprintf(stderr,"*---------------------------------------------------*\n");
	fprintf(stderr,"*                                                   *\n");
	fprintf(stderr,"*                     iLBC测试程序                   *\n");
	fprintf(stderr,"*                                                   *\n");
	fprintf(stderr,"*                                                   *\n");
	fprintf(stderr,"*---------------------------------------------------*\n");
	fprintf(stderr, "Mode           : %2d ms\n", mode);
	fprintf(stderr, "Input file     : %s\n", argv[2]);
	fprintf(stderr, "Encoded file   : %s\n", argv[3]);
	fprintf(stderr, "Output file    : %s\n", argv[4]);
	fprintf(stderr, "Output wav file: %s\n", argv[5]);
	if (argc == 7) {
		fprintf(stderr, "Channel file   : %s\n", argv[5]);
	}
	fprintf(stderr, "\n");

	/* 初始化 */
	initEncode(&Enc_Inst, mode);
	initDecode(&Dec_Inst, mode, 1);

	/* Runtime statistics */
	starttime = clock() / (float)CLOCKS_PER_SEC; //1000
	/* loop over input blocks */  //每次读240short
	int readDateLen = fread(data, sizeof(short), Enc_Inst.blockl, ifileid);
	printf("初始化Enc_Inst.blockl:%d\n", Enc_Inst.blockl);

	while (readDateLen == Enc_Inst.blockl)
	{
		blockcount++;
		/* 编码 */
		len = encode(&Enc_Inst, encoded_data, data);//240short每次编码出25short
		//printf("编码块%d(240->%d)", blockcount, len);
		/* write byte file */
		fwrite(encoded_data, sizeof(unsigned char), len, efileid);
		/* get channel data if provided */
		if (argc == 7)
		{
			if (fread(&pli, sizeof(short), 1, cfileid)) {
				if ((pli != 0) && (pli != 1)) {
					printf("Error in channel file\n");
					exit(0);
				}

				if (pli == 0)
				{
					/* Packet loss -> remove info from frame */
					memset(encoded_data, 0, sizeof(short)*ILBCNOOFWORDS_MAX);
					packetlosscount++;
				}
			}
			else
			{
				printf("Error. Channel file too short\n");
				getchar();
				exit(0);
			}
		}
		else {
			pli = 1;
		}

		/*解码*/
		len = decode(&Dec_Inst, decoded_data, encoded_data, pli);
		//printf("解码块%d(25->%d) ", blockcount, len);
		/*写入解码文件*/
		fwrite(decoded_data, sizeof(short), len, ofileid);
		//printf("读取数据len:%d\n", readDateLen);
		readDateLen = fread(data, sizeof(short), Enc_Inst.blockl, ifileid);
	}

	runtime = (float)(clock()/(float)CLOCKS_PER_SEC-starttime);
	outtime = (float)((float)blockcount*(float)mode /1000.0);
	printf("\n\nLength of speech file: %.1f s\n", outtime);
	printf("Packet loss          : %.1f%%\n",100.0*(float)packetlosscount / (float)blockcount);
	printf("Time to run iLBC     :");
	printf(" %.1f s (%.1f %% of realtime)\n\n", runtime,(100 * runtime / outtime));
	fclose(ofileid);

	//生成wav文件
	printf("生成wav文件:%s\n", argv[5]);
	printf("打开文件:%s\n", argv[4]);
	FILE *fid = fopen(argv[4], "rb");
	//获取文件大小
	fseek(fid, 0, SEEK_END);
	long lSize = ftell(fid);
	rewind(fid);
	//开辟存储空间
	int num = lSize / sizeof(short);
	short *pos = (short*)malloc(sizeof(short)*num);
	if (pos == NULL)
	{
		printf("开辟空间出错");
		return 0;
	}
	printf("开辟空间:%d\n", num);
	fread(pos, sizeof(short), num, fid);
	free(pos);//释放内存
	fclose(fid);

	//int Create(int freq, int volume, int durations)//频率、音量、持续时间
	WaveHeader *pHeader = new WaveHeader;
	DWORD totalLen = num + 44;
	//DWORD totalLen = (m_samplefreq * m_channels * m_channelbits / 8) * durations + 44;//文件总长度=(采样率 * 通道数 * 比特数 / 8) * 持续时间(s)
	//WAVE数据文件的默认字节排序是小端。 文件使用大端字节排序编写的标识符为RIFX而不是RIFF；
	pHeader->chRIFF[0] = 'R';
	pHeader->chRIFF[1] = 'I';
	pHeader->chRIFF[2] = 'F';
	pHeader->chRIFF[3] = 'F';
	pHeader->dwRIFFLen = totalLen - 8;//文件的总长度-8bits

	pHeader->chWAVE[0] = 'W';
	pHeader->chWAVE[1] = 'A';
	pHeader->chWAVE[2] = 'V';
	pHeader->chWAVE[3] = 'E';

	pHeader->chFMT[0] = 'f';
	pHeader->chFMT[1] = 'm';
	pHeader->chFMT[2] = 't';
	pHeader->chFMT[3] = ' ';

	pHeader->dwFMTLen = 0x0010;//一般情况下Size为16，如果为18则最后多了2个字节的附加信息
	pHeader->pwf.wf.wFormatTag = 0x0001;//编码方式
	pHeader->pwf.wf.nChannels = m_channels; //1为单通道，2为双通道
	pHeader->pwf.wf.nSamplesPerSec = m_samplefreq;  //=16KHz
	pHeader->pwf.wf.nAvgBytesPerSec = m_samplefreq * m_channels * m_channelbits / 8;//每秒所需字节数
	pHeader->pwf.wf.nBlockAlign = m_channels * m_channelbits / 8;//一个采样的字节数
	pHeader->pwf.wBitsPerSample = m_channelbits;//16位，即设置PCM的方式为16位立体声(双通道)

	pHeader->chDATA[0] = 'd';
	pHeader->chDATA[1] = 'a';
	pHeader->chDATA[2] = 't';
	pHeader->chDATA[3] = 'a';
	pHeader->dwDATALen = totalLen - WAVE_HEAD_LENGTH;//数据的长度，=文件总长度-头长度(44bit)

	char *pWaveBuffer = new char[totalLen]; //音频数据
	memcpy(pWaveBuffer, pHeader, WAVE_HEAD_LENGTH);//44
	memcpy(pWaveBuffer + WAVE_HEAD_LENGTH, (char *)pos, num);//

	////10000, 100, 5
	////采样率、频率、音量、采样点数 
	//MakeWaveData(pHeader->pwf.wf.nSamplesPerSec,16000, 100, pWaveBuffer + WAVE_HEAD_LENGTH, m_samplefreq*durations);//采样点数
	printf("写入头:%d\n",44);
	printf("写入数据:%d\n", num);
	fwrite((char*)pWaveBuffer, 1, num + 44, wavfileid);

	

	/* close files */
	fclose(ifileid);  
	fclose(efileid);
	fclose(wavfileid);
	if (argc == 6) {
		fclose(cfileid);
	}
	getchar();
	return(0);
}

