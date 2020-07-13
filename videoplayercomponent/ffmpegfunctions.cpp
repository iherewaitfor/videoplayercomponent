#include "stdafx.h"
#include "ffmpegfunctions.h"


FfmpegFunctions * pFfmpegFunctions  = NULL;

FfmpegFunctions::FfmpegFunctions(wstring filePath)
{
	m_filePath = filePath;
	//::LoadLibraryA("avformat-55.dll");
	//HMODULE h_avformat = GetModuleHandleA("avformat-55.dll");

	av_register_allPtr = NULL;
	avformat_network_initPtr = NULL;
	avformat_network_deinitPtr = NULL;
	avformat_alloc_contextPtr = NULL;
	avformat_open_inputPtr = NULL;
	avformat_find_stream_infoPtr = NULL;
	av_dump_formatPtr= NULL;
	av_read_framePtr= NULL;
	avformat_close_inputPtr = NULL;
	av_seek_framePtr = NULL;


	//LoadLibraryA("avcodec-55.dll");
	//HMODULE h_aavcodec = GetModuleHandleA("avcodec-55.dll");

	avcodec_find_decoderPtr= NULL;
	avcodec_open2Ptr= NULL;
	avcodec_decode_video2Ptr = NULL;
	av_free_packetPtr = NULL;
	avcodec_closePtr =NULL;
	av_packet_unrefPtr = NULL;
	avcodec_decode_audio4Ptr = NULL;


	//LoadLibraryA("avutil-52.dll");
	//HMODULE h_avutil= GetModuleHandleA("avutil-52.dll");
	av_frame_allocPtr = NULL;
	av_mallocPtr = NULL;
	av_freePtr = NULL;
	av_image_get_buffer_sizePtr = NULL;
	av_image_fill_arraysPtr = NULL;
	av_frame_freePtr = NULL;

	av_get_channel_layout_nb_channelsPtr = NULL;
	av_samples_get_buffer_sizePtr = NULL;

	//LoadLibraryA("swscale-2.dll");
	//HMODULE h_swscale= GetModuleHandleA("swscale-2.dll");
	sws_getContextPtr = NULL;
	sws_scalePtr = NULL;
	sws_freeContextPtr = NULL;

	//LoadLibraryA("swresample-0.dll");
	//HMODULE h_swresample= GetModuleHandleA("swresample-0.dll");
	swr_allocPtr = NULL;
	swr_alloc_set_optsPtr = NULL;
	swr_initPtr = NULL;
	swr_convertPtr = NULL;
	swr_freePtr = NULL;
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

bool FfmpegFunctions::initFns()
{
	wchar_t szCurDir[MAX_PATH] = { 0 };
	GetCurrentDirectory(MAX_PATH, szCurDir);
	SetCurrentDirectory(m_filePath.c_str());

	wstring tempPath = m_filePath;
	tempPath.append(L"avformat-55.dll");
	HMODULE h_avformat = ::LoadLibrary(tempPath.c_str());

	av_register_allPtr = (Av_register_allPtr)GetProcAddress(h_avformat, "av_register_all");

	avformat_network_initPtr = (Avformat_network_initPtr)GetProcAddress(h_avformat, "avformat_network_init");

	avformat_network_deinitPtr = (Avformat_network_deinitPtr)GetProcAddress(h_avformat, "avformat_network_deinit"); 

	avformat_alloc_contextPtr = (Avformat_alloc_contextPtr)GetProcAddress(h_avformat, "avformat_alloc_context");


	avformat_open_inputPtr = (Avformat_open_inputPtr)GetProcAddress(h_avformat, "avformat_open_input");


	avformat_find_stream_infoPtr = (Avformat_find_stream_infoPtr)GetProcAddress(h_avformat, "avformat_find_stream_info");


	av_dump_formatPtr = (Av_dump_formatPtr)GetProcAddress(h_avformat, "av_dump_format");

	av_read_framePtr = (Av_read_framePtr)GetProcAddress(h_avformat, "av_read_frame");

	avformat_close_inputPtr = (Avformat_close_inputPtr)GetProcAddress(h_avformat, "avformat_close_input");

	av_seek_framePtr = (Av_seek_framePtr)GetProcAddress(h_avformat, "av_seek_frame");
	


	tempPath = m_filePath;
	tempPath.append(L"avcodec-55.dll");
	HMODULE h_aavcodec = ::LoadLibrary(tempPath.c_str());

	avcodec_find_decoderPtr = (Avcodec_find_decoderPtr)GetProcAddress(h_aavcodec, "avcodec_find_decoder");

	avcodec_open2Ptr = (Avcodec_open2Ptr)GetProcAddress(h_aavcodec, "avcodec_open2");

	avcodec_decode_video2Ptr = (Avcodec_decode_video2Ptr)GetProcAddress(h_aavcodec, "avcodec_decode_video2");

	av_free_packetPtr = (Av_free_packetPtr)GetProcAddress(h_aavcodec, "av_free_packet");

	avcodec_closePtr = (Avcodec_closePtr)GetProcAddress(h_aavcodec, "avcodec_close");

	av_packet_unrefPtr = (Av_packet_unrefPtr)GetProcAddress(h_aavcodec, "av_packet_unref"); 

	avcodec_decode_audio4Ptr = (Avcodec_decode_audio4Ptr)GetProcAddress(h_aavcodec, "avcodec_decode_audio4"); 

	

	tempPath = m_filePath;
	tempPath.append(L"avutil-52.dll");
	HMODULE h_avutil= ::LoadLibrary(tempPath.c_str());

	av_frame_allocPtr = (Av_frame_allocPtr)GetProcAddress(h_avutil, "av_frame_alloc");

	av_frame_freePtr = (Av_frame_freePtr)GetProcAddress(h_avutil, "av_frame_free");

	av_mallocPtr = (Av_mallocPtr)GetProcAddress(h_avutil, "av_malloc");

	av_freePtr = (Av_freePtr)GetProcAddress(h_avutil, "av_free");

	av_get_channel_layout_nb_channelsPtr = (Av_get_channel_layout_nb_channelsPtr)GetProcAddress(h_avutil, "av_get_channel_layout_nb_channels");

	av_samples_get_buffer_sizePtr = (Av_samples_get_buffer_sizePtr)GetProcAddress(h_avutil, "av_samples_get_buffer_size");

	av_get_default_channel_layoutPtr = (Av_get_default_channel_layoutPtr)GetProcAddress(h_avutil, "av_get_default_channel_layout");

	

	

	


	


	av_image_get_buffer_sizePtr = (Av_image_get_buffer_sizePtr)GetProcAddress(h_avutil, "av_image_get_buffer_size");

	av_image_fill_arraysPtr = (Av_image_fill_arraysPtr)GetProcAddress(h_avutil, "av_image_fill_arrays");




	tempPath = m_filePath;
	tempPath.append(L"swscale-2.dll");
	HMODULE h_swscale= ::LoadLibrary(tempPath.c_str());

	sws_getContextPtr = (Sws_getContextPtr)GetProcAddress(h_swscale, "sws_getContext");

	sws_scalePtr = (Sws_scalePtr)GetProcAddress(h_swscale, "sws_scale");
	sws_freeContextPtr = (Sws_freeContextPtr)GetProcAddress(h_swscale, "sws_freeContext");



	tempPath = m_filePath;
	tempPath.append(L"swresample-0.dll");
	HMODULE h_swresample= ::LoadLibrary(tempPath.c_str());

	swr_allocPtr = (Swr_allocPtr)GetProcAddress(h_swresample, "swr_alloc");

	swr_alloc_set_optsPtr = (Swr_alloc_set_optsPtr)GetProcAddress(h_swresample, "swr_alloc_set_opts");

	swr_initPtr = (Swr_initPtr)GetProcAddress(h_swresample, "swr_init");

	swr_convertPtr = (Swr_convertPtr)GetProcAddress(h_swresample, "swr_convert");

	swr_freePtr = (Swr_freePtr)GetProcAddress(h_swresample, "swr_free");
	

	
	


	SetCurrentDirectory(szCurDir);

	if(!h_avutil || !h_swscale || !h_aavcodec || !h_avformat || !h_swresample)
	{
		return false;
	}
	return true;
}

void FfmpegFunctions::createInstance(wstring filePath)
{

	if(pFfmpegFunctions == NULL)
	{
		pFfmpegFunctions = new FfmpegFunctions(filePath);
	}
}

FfmpegFunctions::~FfmpegFunctions()
{
	HMODULE h_avformat = ::GetModuleHandle (L"avformat-55.dll");
	if(h_avformat)
	{
		FreeLibrary(h_avformat);
	}

	HMODULE h_aavcodec = ::GetModuleHandle (L"avcodec-55.dll");
	if(h_aavcodec)
	{
		FreeLibrary(h_aavcodec);
	}

	HMODULE h_swscale = ::GetModuleHandle (L"swscale-2.dll");
	if(h_swscale)
	{
		FreeLibrary(h_swscale);
	}

	HMODULE h_swresample = ::GetModuleHandle (L"swresample-0.dll");
	if(h_swresample)
	{
		FreeLibrary(h_swresample);
	}

	HMODULE h_avutil = ::GetModuleHandle (L"avutil-52.dll");
	if(h_avutil)
	{
		FreeLibrary(h_avutil);
	}
}
