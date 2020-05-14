
#include "stdafx.h"
#ifndef	FPLAERWINDOW_H
#define FPLAERWINDOW_H

#include "exportinclude/playerwindowinterface.h"

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

class PlayerWindow : public PlayerWindowInterface{
public:
	PlayerWindow();
	bool init(HWND parentHwnd, int x, int y, int w, int h);
	void setPlayPosition(int x, int y, int w, int h);
	int Play(const string & filePath);
	void stop();

private:
	int renderFrame();
	void render(HWND hwnd, uint8_t * data, int WIDTH, int HEIGHT, bool rotate = false);
	void releaseResources();
public:
	LRESULT MyProc(HWND hWnd,UINT uMsg,WPARAM wParam,LPARAM lParam);//类成员函数,我们将会让这个类成员函数作为窗口的窗口过程

private:
	AVFormatContext	*pFormatCtx;
	int				videoindex;
	AVCodecContext	*pCodecCtx;
	AVCodec			*pCodec;
	AVFrame	*pFrame,*pFrameYUV;
	unsigned char *out_buffer;
	AVPacket *packet;
	int y_size;
	int ret, got_picture;
	struct SwsContext *img_convert_ctx;
	string m_filepath;

	uint8_t * out;
	uint8_t * outRGBA;


	struct SwsContext *	m_pSwsContextYUV2BGRA;

	HWND	m_hwnd;
	BOOL	ldown;
	POINT	TheFirstPoint;

	bool m_bReadFramesFinished;
	bool m_playing;


	HDC hCompatibleDC;
	HBITMAP hCustomBmp;
};


#endif //FPLAERWINDOW_H