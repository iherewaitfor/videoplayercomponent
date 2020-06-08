// videoplayercomponent.cpp : 定义 DLL 应用程序的导出函数。
//

#include "stdafx.h"
#include "videoplayercomponent.h"

#include "ffmpegfunctions.h"
#include "playerwindow.h"
#include "playwindowhelper.h"


// 这是导出变量的一个示例
VIDEOPLAYERCOMPONENT_API int nvideoplayercomponent=0;

// 这是导出函数的一个示例。
VIDEOPLAYERCOMPONENT_API int fnvideoplayercomponent(void)
{
	return 42;
}

VIDEOPLAYERCOMPONENT_API bool fn_onLoadVideoplayercomponent(LPCWSTR filePath)
{
	//初始化dll
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
	//清理dll
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

// 这是已导出类的构造函数。
// 有关类定义的信息，请参阅 videoplayercomponent.h
Cvideoplayercomponent::Cvideoplayercomponent()
{
	return;
}
