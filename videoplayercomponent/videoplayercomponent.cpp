// videoplayercomponent.cpp : ���� DLL Ӧ�ó���ĵ���������
//

#include "stdafx.h"
#include "videoplayercomponent.h"

#include "ffmpegfunctions.h"
#include "playerwindow.h"


// ���ǵ���������һ��ʾ��
VIDEOPLAYERCOMPONENT_API int nvideoplayercomponent=0;

// ���ǵ���������һ��ʾ����
VIDEOPLAYERCOMPONENT_API int fnvideoplayercomponent(void)
{
	return 42;
}

VIDEOPLAYERCOMPONENT_API int fn_onLoadVideoplayercomponent(LPCWSTR filePath)
{
	//��ʼ��dll
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
	//����dll

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

// �����ѵ�����Ĺ��캯����
// �й��ඨ�����Ϣ������� videoplayercomponent.h
Cvideoplayercomponent::Cvideoplayercomponent()
{
	return;
}
