// videoplayercomponent.cpp : 定义 DLL 应用程序的导出函数。
//

#include "stdafx.h"
#include "videoplayercomponent.h"

#include "ffmpegfunctions.h"
#include "playerwindow.h"


// 这是导出变量的一个示例
VIDEOPLAYERCOMPONENT_API int nvideoplayercomponent=0;

// 这是导出函数的一个示例。
VIDEOPLAYERCOMPONENT_API int fnvideoplayercomponent(void)
{
	return 42;
}

VIDEOPLAYERCOMPONENT_API int fn_onLoadVideoplayercomponent(LPCWSTR filePath)
{
	//初始化dll
	wstring temPath;
	temPath.append(filePath);
	FfmpegFunctions::createInstance(filePath);
	FfmpegFunctions::getInstance()->initFns();
	FfmpegFunctions::getInstance()->av_register_allPtr();
	FfmpegFunctions::getInstance()->avformat_network_initPtr();
	
	return 0;

}


VIDEOPLAYERCOMPONENT_API void fn_unLoadVideoplayercomponent()
{
	//清理dll

}

VIDEOPLAYERCOMPONENT_API PlayerWindowInterface * fn_CreatePlayWindowt()
{
	return new PlayerWindow();
}

extern "C" VIDEOPLAYERCOMPONENT_API void fn_FreePlayWindow(PlayerWindowInterface * w)
{
	if(w)
	{
		delete(w);
	}
}

// 这是已导出类的构造函数。
// 有关类定义的信息，请参阅 videoplayercomponent.h
Cvideoplayercomponent::Cvideoplayercomponent()
{
	return;
}
