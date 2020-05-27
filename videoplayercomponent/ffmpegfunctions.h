
#include "stdafx.h"
#ifndef	FFMPEGFUNCTIONS_H
#define FFMPEGFUNCTIONS_H

#define __STDC_CONSTANT_MACROS

extern "C"
{
#include "libavcodec/avcodec.h"
#include "libavformat/avformat.h"
#include "libswscale/swscale.h"
#include "libavutil/imgutils.h"

};

#include <string>

using namespace std;
//::LoadLibraryA("avformat-55.dll");
//HMODULE h_avformat = GetModuleHandleA("avformat-55.dll");

typedef void (*Av_register_allPtr)();
typedef void (*Avformat_network_initPtr)();
typedef AVFormatContext * (*Avformat_alloc_contextPtr)();
typedef int (*Avformat_open_inputPtr)(AVFormatContext **, const char *, AVInputFormat *, AVDictionary **);
typedef int (*Avformat_find_stream_infoPtr)(AVFormatContext *, AVDictionary **);
typedef void (*Av_dump_formatPtr)(AVFormatContext *, int , const char *, int);
typedef int (*Av_read_framePtr)(AVFormatContext *, AVPacket *);
typedef void (*Avformat_close_inputPtr)(AVFormatContext **);
typedef int (*Av_seek_framePtr)(AVFormatContext *, int , int64_t ,int );



//LoadLibraryA("avcodec-55.dll");
//HMODULE h_aavcodec = GetModuleHandleA("avcodec-55.dll");

typedef AVCodec* (*Avcodec_find_decoderPtr)(enum AVCodecID );
typedef int (*Avcodec_open2Ptr)(AVCodecContext *, const AVCodec *, AVDictionary ** );
typedef int (*Avcodec_decode_video2Ptr)(AVCodecContext *, AVFrame *, int *, const AVPacket *);
typedef void (*Av_free_packetPtr)(AVPacket *);
typedef int (*Avcodec_closePtr)(AVCodecContext *);
typedef void (*Av_packet_unrefPtr)(AVPacket *);

//LoadLibraryA("avutil-52.dll");
//HMODULE h_avutil= GetModuleHandleA("avutil-52.dll");

typedef AVFrame * (*Av_frame_allocPtr)();
typedef unsigned char * (*Av_mallocPtr)(size_t);
typedef void(*Av_freePtr)(void *);
typedef int (*Av_image_get_buffer_sizePtr)(enum AVPixelFormat , int , int , int );
typedef int (*Av_image_fill_arraysPtr)(uint8_t *[], int[],const uint8_t *,enum AVPixelFormat , int , int , int );
typedef void (*Av_frame_freePtr)(AVFrame **);


//LoadLibraryA("swscale-2.dll");
//HMODULE h_swscale= GetModuleHandleA("swscale-2.dll");

typedef struct SwsContext * (*Sws_getContextPtr)(int , int , enum AVPixelFormat ,
												 int , int , enum AVPixelFormat ,
												 int , SwsFilter *,
												 SwsFilter *, const double *);

typedef int(*Sws_scalePtr)(struct SwsContext *, const uint8_t *const [],
						   const int [], int , int ,
						   uint8_t *const [], const int []);

typedef void(*Sws_freeContextPtr)(struct SwsContext *);




struct FfmpegFunctions
{

	//::LoadLibraryA("avformat-55.dll");
	//HMODULE h_avformat = GetModuleHandleA("avformat-55.dll");

	Av_register_allPtr av_register_allPtr;
	Avformat_network_initPtr avformat_network_initPtr ;
	Avformat_alloc_contextPtr avformat_alloc_contextPtr;
	Avformat_open_inputPtr avformat_open_inputPtr;
	Avformat_find_stream_infoPtr avformat_find_stream_infoPtr;
	Av_dump_formatPtr av_dump_formatPtr;
	Av_read_framePtr av_read_framePtr;
	Avformat_close_inputPtr avformat_close_inputPtr;
	Av_seek_framePtr av_seek_framePtr;


	//LoadLibraryA("avcodec-55.dll");
	//HMODULE h_aavcodec = GetModuleHandleA("avcodec-55.dll");

	Avcodec_find_decoderPtr avcodec_find_decoderPtr;
	Avcodec_open2Ptr avcodec_open2Ptr;
	Avcodec_decode_video2Ptr avcodec_decode_video2Ptr;
	Av_free_packetPtr av_free_packetPtr;
	Avcodec_closePtr avcodec_closePtr;
	Av_packet_unrefPtr av_packet_unrefPtr;


	//LoadLibraryA("avutil-52.dll");
	//HMODULE h_avutil= GetModuleHandleA("avutil-52.dll");
	Av_frame_allocPtr av_frame_allocPtr;
	Av_mallocPtr av_mallocPtr;
	Av_freePtr av_freePtr;
	Av_image_get_buffer_sizePtr av_image_get_buffer_sizePtr;
	Av_image_fill_arraysPtr av_image_fill_arraysPtr;
	Av_frame_freePtr av_frame_freePtr;

	//LoadLibraryA("swscale-2.dll");
	//HMODULE h_swscale= GetModuleHandleA("swscale-2.dll");
	Sws_getContextPtr sws_getContextPtr;
	Sws_scalePtr sws_scalePtr;
	Sws_freeContextPtr sws_freeContextPtr;

	FfmpegFunctions(wstring filePath);
	~FfmpegFunctions();
	static FfmpegFunctions * getInstance();
	static void createInstance(wstring filePath);
	static void desctroyInstance();
	bool initFns();
	wstring m_filePath;




	//::LoadLibraryA("avformat-55.dll");
	//HMODULE h_avformat = GetModuleHandleA("avformat-55.dll");

	//typedef void (*Av_register_allPtr)();
	//Av_register_allPtr av_register_allPtr = NULL;

	//typedef void (*Avformat_network_initPtr)();
	//Avformat_network_initPtr avformat_network_initPtr = = NULL;


	//typedef AVFormatContext * (*Avformat_alloc_contextPtr)();
	//Avformat_alloc_contextPtr avformat_alloc_contextPtr = NULL;


	//typedef int (*Avformat_open_inputPtr)(AVFormatContext **, const char *, AVInputFormat *, AVDictionary **);
	//Avformat_open_inputPtr avformat_open_inputPtr = NULL;


	//typedef int (*Avformat_find_stream_infoPtr)(AVFormatContext *, AVDictionary **);
	//Avformat_find_stream_infoPtr avformat_find_stream_infoPtr = NULL;


	//typedef void (*Av_dump_formatPtr)(AVFormatContext *, int , const char *, int);
	//Av_dump_formatPtr av_dump_formatPtr= NULL;

	//typedef int (*Av_read_framePtr)(AVFormatContext *, AVPacket *);
	//Av_read_framePtr av_read_framePtr= NULL;


	//LoadLibraryA("avcodec-55.dll");
	//HMODULE h_aavcodec = GetModuleHandleA("avcodec-55.dll");

	//typedef AVCodec* (*Avcodec_find_decoderPtr)(enum AVCodecID );
	//Avcodec_find_decoderPtr avcodec_find_decoderPtr= NULL;

	//typedef int (*Avcodec_open2Ptr)(AVCodecContext *, const AVCodec *, AVDictionary ** );
	//Avcodec_open2Ptr avcodec_open2Ptr= NULL;

	//typedef int (*Avcodec_decode_video2Ptr)(AVCodecContext *, AVFrame *, int *, const AVPacket *);
	//Avcodec_decode_video2Ptr avcodec_decode_video2Ptr = NULL;

	//typedef void (*Av_free_packetPtr)(AVPacket *);
	//Av_free_packetPtr av_free_packetPtr = NULL;


	//LoadLibraryA("avutil-52.dll");
	//HMODULE h_avutil= GetModuleHandleA("avutil-52.dll");

	//typedef AVFrame * (*Av_frame_allocPtr)();
	//Av_frame_allocPtr av_frame_allocPtr = NULL;

	//typedef unsigned char * (*Av_mallocPtr)(size_t);
	//Av_mallocPtr av_mallocPtr = NULL;


	//typedef int (*Av_image_get_buffer_sizePtr)(enum AVPixelFormat , int , int , int );
	//Av_image_get_buffer_sizePtr av_image_get_buffer_sizePtr = NULL

	//typedef int (*Av_image_fill_arraysPtr)(uint8_t *[], int[],const uint8_t *,enum AVPixelFormat , int , int , int );
	//Av_image_fill_arraysPtr av_image_fill_arraysPtr = NULL;

	//LoadLibraryA("swscale-2.dll");
	//HMODULE h_swscale= GetModuleHandleA("swscale-2.dll");

	//typedef struct SwsContext * (*Sws_getContextPtr)(int , int , enum AVPixelFormat ,
	//	int , int , enum AVPixelFormat ,
	//	int , SwsFilter *,
	//	SwsFilter *, const double *);
	//Sws_getContextPtr sws_getContextPtr = NULL;

	//typedef int(*Sws_scalePtr)(struct SwsContext *, const uint8_t *const [],
	//	const int [], int , int ,
	//	uint8_t *const [], const int []);
	//Sws_scalePtr sws_scalePtr = NULL;
};

#endif //FFMPEGFUNCTIONS_H