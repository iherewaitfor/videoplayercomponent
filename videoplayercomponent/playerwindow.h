
#include "stdafx.h"
#ifndef	FPLAERWINDOW_H
#define FPLAERWINDOW_H
#include <windows.h>
#include <mmsystem.h>
#pragma comment(lib, "winmm.lib")

#include "exportinclude/playerwindowinterface.h"

#define __STDC_CONSTANT_MACROS

extern "C"
{
#include "libavcodec/avcodec.h"
#include "libavformat/avformat.h"
#include "libswscale/swscale.h"
#include "libavutil/imgutils.h"
#include "libswresample/swresample.h"//audio
};

#include <string>

using namespace std;

class PlayerWindow : public PlayerWindowInterface{
public:
	PlayerWindow();
	virtual ~PlayerWindow();
	//PlayerWindowInterface
	bool init(HWND parentHwnd, int x, int y, int w, int h);
	void setPlayPosition(int x, int y, int w, int h);
	void setVisible(bool isVisible);
	bool play(const string & filePath);
	void stop();
	int getPlayerWindowID();
	void setPlayerWindowID(int id);
	void setTransparent(bool isTransparent);
	void setVolume(double volume);


public://内部使用
	HWAVEOUT getHwo(){return hwo;}
	void playSound(PlayerWindow *pPlayerWindow,LPWAVEHDR pWaveHeader);


private:
	bool playVideo();
	int renderFrame();
	void render(HWND hwnd, uint8_t * data, int WIDTH, int HEIGHT, bool rotate = false);
	void releaseWinRenderResources();
	void releaseFFmpegResources();
	void clearWin();
	bool startPlaySound();
	void releaseFFmpegAudioResources();
public:
	LRESULT MyProc(HWND hWnd,UINT uMsg,WPARAM wParam,LPARAM lParam);//类成员函数,我们将会让这个类成员函数作为窗口的窗口过程

public:
	long m_playingWAVEHDRCount; 
	bool m_playingAudio;

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




	AVFormatContext	*pAudioFormatCtx;
	int				audioStreamIndex;
	AVCodecContext	*pAudioCodecCtx;
	AVCodec			*pAudioCodec;
	AVPacket		*pAudiopacket;
	uint8_t			*audio_out_buffer;
	int				audio_out_buffer_size;
	AVFrame			*pAudioFrame;
	uint32_t        audio_len ;
	int             audio_got_picture;
	int             audioIndex ;
	int64_t         in_channel_layout;
	struct SwrContext *au_convert_ctx;

	HWAVEOUT        hwo;
	WAVEHDR         wh1;
	WAVEHDR         wh2;
	WAVEHDR         wh3;
	WAVEHDR         wh4;
	WAVEFORMATEX    wfx;
	double          m_volume;


	//int m_playingWAVEHDRCount; 
	//bool m_playingAudio;



	int		m_width;
	int		m_height;
	bool    m_bLoop;

	HWND	m_hwnd;
	BOOL	ldown;
	POINT	TheFirstPoint;

	bool m_bReadFramesFinished;
	bool m_playing;
	bool m_bFreeing;
	bool m_bClearWin;


	HDC hdc;
	HDC hCompatibleDC;
	HBITMAP hCustomBmp;

	int playWindowId;
	bool m_bTransparent;
};


#endif //FPLAERWINDOW_H