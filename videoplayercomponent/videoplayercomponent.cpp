// videoplayercomponent.cpp : ���� DLL Ӧ�ó���ĵ���������
//

#include "stdafx.h"
#include "videoplayercomponent.h"

#include "ffmpegfunctions.h"
#include "playerwindow.h"
#include "playwindowhelper.h"


// ���ǵ���������һ��ʾ��
VIDEOPLAYERCOMPONENT_API int nvideoplayercomponent=0;

// ���ǵ���������һ��ʾ����
VIDEOPLAYERCOMPONENT_API int fnvideoplayercomponent(void)
{
	return 42;
}

VIDEOPLAYERCOMPONENT_API bool fn_onLoadVideoplayercomponent(LPCWSTR filePath)
{
	//��ʼ��dll
	wstring temPath;
	temPath.append(filePath);
	FfmpegFunctions::createInstance(filePath);
	if(FfmpegFunctions::getInstance()->initFns())
	{
		if(FfmpegFunctions::getInstance()->av_register_allPtr)
		{
			FfmpegFunctions::getInstance()->av_register_allPtr();
		}
		if(FfmpegFunctions::getInstance()->avformat_network_initPtr)
		{
			FfmpegFunctions::getInstance()->avformat_network_initPtr();
		}

		PlayWindowHelperImple::createInstance();
		return true;
	}
	return false;

}


extern "C" VIDEOPLAYERCOMPONENT_API void fn_unLoadVideoplayercomponent()
{
	//����dll
	if(FfmpegFunctions::getInstance()->avformat_network_deinitPtr)
	{
		FfmpegFunctions::getInstance()->avformat_network_deinitPtr();
	}
	FfmpegFunctions::desctroyInstance();
	PlayWindowHelperImple::desctroyInstance();
	
}

extern "C" VIDEOPLAYERCOMPONENT_API VideoPlayerComponentHelper* fn_getVideoPlayerComponentHelper()
{
	return PlayWindowHelperImple::getInstance();
}

// �����ѵ�����Ĺ��캯����
// �й��ඨ�����Ϣ������� videoplayercomponent.h
Cvideoplayercomponent::Cvideoplayercomponent()
{
	return;
}
