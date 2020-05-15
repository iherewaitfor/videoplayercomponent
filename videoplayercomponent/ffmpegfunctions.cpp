#include "stdafx.h"
#include "ffmpegfunctions.h"


FfmpegFunctions * pFfmpegFunctions  = NULL;

FfmpegFunctions::FfmpegFunctions(wstring filePath)
{
	m_filePath = filePath;
	//::LoadLibraryA("avformat-55.dll");
	//HMODULE h_avformat = GetModuleHandleA("avformat-55.dll");

	Av_register_allPtr av_register_allPtr = NULL;
	Avformat_network_initPtr avformat_network_initPtr = NULL;
	Avformat_alloc_contextPtr avformat_alloc_contextPtr = NULL;
	Avformat_open_inputPtr avformat_open_inputPtr = NULL;
	Avformat_find_stream_infoPtr avformat_find_stream_infoPtr = NULL;
	Av_dump_formatPtr av_dump_formatPtr= NULL;
	Av_read_framePtr av_read_framePtr= NULL;
	Avformat_close_inputPtr avformat_close_inputPtr = NULL;
	av_seek_framePtr = NULL;


	//LoadLibraryA("avcodec-55.dll");
	//HMODULE h_aavcodec = GetModuleHandleA("avcodec-55.dll");

	Avcodec_find_decoderPtr avcodec_find_decoderPtr= NULL;
	Avcodec_open2Ptr avcodec_open2Ptr= NULL;
	Avcodec_decode_video2Ptr avcodec_decode_video2Ptr = NULL;
	Av_free_packetPtr av_free_packetPtr = NULL;
	Avcodec_closePtr avcodec_closePtr =NULL;


	//LoadLibraryA("avutil-52.dll");
	//HMODULE h_avutil= GetModuleHandleA("avutil-52.dll");
	Av_frame_allocPtr av_frame_allocPtr = NULL;
	Av_mallocPtr av_mallocPtr = NULL;
	Av_image_get_buffer_sizePtr av_image_get_buffer_sizePtr = NULL;
	Av_image_fill_arraysPtr av_image_fill_arraysPtr = NULL;
	Av_frame_freePtr av_frame_freePtr = NULL;

	//LoadLibraryA("swscale-2.dll");
	//HMODULE h_swscale= GetModuleHandleA("swscale-2.dll");
	Sws_getContextPtr sws_getContextPtr = NULL;
	Sws_scalePtr sws_scalePtr = NULL;
	Sws_freeContextPtr sws_freeContextPtr = NULL;
}

FfmpegFunctions * FfmpegFunctions::getInstance()
{
	return pFfmpegFunctions;
}

void FfmpegFunctions::desctroyInstance()
{
	if(pFfmpegFunctions != NULL)
	{
		delete(pFfmpegFunctions);
		pFfmpegFunctions = NULL;
	}
}

void FfmpegFunctions::initFns()
{
	wstring tempPath = m_filePath;
	tempPath.append(L"avformat-55.dll");
	::LoadLibrary(tempPath.c_str());
	HMODULE h_avformat = GetModuleHandleA("avformat-55.dll");
	av_register_allPtr = (Av_register_allPtr)GetProcAddress(h_avformat, "av_register_all");

	avformat_network_initPtr = (Avformat_network_initPtr)GetProcAddress(h_avformat, "avformat_network_init");


	avformat_alloc_contextPtr = (Avformat_alloc_contextPtr)GetProcAddress(h_avformat, "avformat_alloc_context");


	avformat_open_inputPtr = (Avformat_open_inputPtr)GetProcAddress(h_avformat, "avformat_open_input");


	avformat_find_stream_infoPtr = (Avformat_find_stream_infoPtr)GetProcAddress(h_avformat, "avformat_find_stream_info");


	av_dump_formatPtr = (Av_dump_formatPtr)GetProcAddress(h_avformat, "av_dump_format");

	av_read_framePtr = (Av_read_framePtr)GetProcAddress(h_avformat, "av_read_frame");

	avformat_close_inputPtr = (Avformat_close_inputPtr)GetProcAddress(h_avformat, "avformat_close_input");

	av_seek_framePtr = (Av_seek_framePtr)GetProcAddress(h_avformat, "av_seek_frame");
	


	tempPath = m_filePath;
	tempPath.append(L"avcodec-55.dll");
	::LoadLibrary(tempPath.c_str());
	HMODULE h_aavcodec = GetModuleHandleA("avcodec-55.dll");

	avcodec_find_decoderPtr = (Avcodec_find_decoderPtr)GetProcAddress(h_aavcodec, "avcodec_find_decoder");

	avcodec_open2Ptr = (Avcodec_open2Ptr)GetProcAddress(h_aavcodec, "avcodec_open2");

	avcodec_decode_video2Ptr = (Avcodec_decode_video2Ptr)GetProcAddress(h_aavcodec, "avcodec_decode_video2");

	av_free_packetPtr = (Av_free_packetPtr)GetProcAddress(h_aavcodec, "av_free_packet");

	avcodec_closePtr = (Avcodec_closePtr)GetProcAddress(h_aavcodec, "avcodec_close");

	tempPath = m_filePath;
	tempPath.append(L"avutil-52.dll");
	::LoadLibrary(tempPath.c_str());
	HMODULE h_avutil= GetModuleHandleA("avutil-52.dll");

	av_frame_allocPtr = (Av_frame_allocPtr)GetProcAddress(h_avutil, "av_frame_alloc");

	av_frame_freePtr = (Av_frame_freePtr)GetProcAddress(h_avutil, "av_frame_free");

	av_mallocPtr = (Av_mallocPtr)GetProcAddress(h_avutil, "av_malloc");


	av_image_get_buffer_sizePtr = (Av_image_get_buffer_sizePtr)GetProcAddress(h_avutil, "av_image_get_buffer_size");

	av_image_fill_arraysPtr = (Av_image_fill_arraysPtr)GetProcAddress(h_avutil, "av_image_fill_arrays");




	tempPath = m_filePath;
	tempPath.append(L"swscale-2.dll");
	::LoadLibrary(tempPath.c_str());
	HMODULE h_swscale= GetModuleHandleA("swscale-2.dll");

	sws_getContextPtr = (Sws_getContextPtr)GetProcAddress(h_swscale, "sws_getContext");

	sws_scalePtr = (Sws_scalePtr)GetProcAddress(h_swscale, "sws_scale");
	sws_freeContextPtr = (Sws_freeContextPtr)GetProcAddress(h_swscale, "sws_freeContext");
}

void FfmpegFunctions::createInstance(wstring filePath)
{

	if(pFfmpegFunctions == NULL)
	{
		pFfmpegFunctions = new FfmpegFunctions(filePath);
	}
}
