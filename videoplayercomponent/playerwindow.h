
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
	virtual ~PlayerWindow();
	bool init(HWND parentHwnd, int x, int y, int w, int h);
	void setPlayPosition(int x, int y, int w, int h);
	bool Play(const string & filePath);
	void stop();
	int getPlayerWindowID();

private:
	int renderFrame();
	void render(HWND hwnd, uint8_t * data, int WIDTH, int HEIGHT, bool rotate = false);
	void releaseWinRenderResources();
	void releaseFFmpegResources();
	void clearWin();
public:
	LRESULT MyProc(HWND hWnd,UINT uMsg,WPARAM wParam,LPARAM lParam);//类成员函数,我们将会让这个类成员函数作为窗口的窗口过程

private:
	AVFormatContext	*pFormatCtx;
	int				videoindex;
	AVCodecContext	*pCodecCtx;
	AVCodec			*pCodec;
	AVFrame			*pFrame,*pFrameYUV;
	unsigned char	*out_buffer;
	AVPacket		*packet;
	int		got_picture;
	struct SwsContext *img_convert_ctx;
	struct SwsContext *	m_pSwsContextYUV2BGRA;

	string m_filepath;

	uint8_t * out;
	uint8_t * outRGBA;




	int		m_width;
	int		m_height;
	bool    m_bLoop;

	HWND	m_hwnd;
	BOOL	ldown;
	POINT	TheFirstPoint;

	bool m_bReadFramesFinished;
	bool m_playing;
	bool m_bClearWin;


	HDC hCompatibleDC;
	HBITMAP hCustomBmp;

	int playWindowId;
};


#endif //FPLAERWINDOW_H