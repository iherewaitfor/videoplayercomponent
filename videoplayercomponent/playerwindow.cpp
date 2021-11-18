#include "stdafx.h"
#include "playerwindow.h"
#include "ffmpegfunctions.h"
#include "playwindowhelper.h"
#include <Windows.h>
#include <WinUser.h>

#define MAX_AUDIO_FRAME_SIZE 192000 // 1 second of 48khz 32bit audio
#define DATASIZE 1024*200 //分次截取数据大小
void CALLBACK WaveCallback(HWAVEOUT hWave, UINT uMsg, DWORD dwInstance, DWORD dw1, DWORD dw2)//回调函数
{
	switch (uMsg)
	{
	case WOM_DONE://上次缓存播放完成,触发该事件
		{
			PlayerWindow * pPlayerWindow = (PlayerWindow*)(dwInstance);
			if(pPlayerWindow)
			{
				if(pPlayerWindow->m_playingAudio)
				{
					LPWAVEHDR pWaveHeader = (LPWAVEHDR)dw1;
					pPlayerWindow->playSound(pPlayerWindow,pWaveHeader);
				}
				InterlockedExchangeAdd(&pPlayerWindow->m_playingWAVEHDRCount, -1);
			}
			break;
		}
	}
}

LRESULT CALLBACK WndProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam);

void transformRGBRotate(uint8_t * in, uint8_t* out, int width, int height)
{
	int rgbwidth = width;
	for(int j = height; j > 0 ; j--)
	{
		for(int i= 0;  i < rgbwidth; i++)
		{
			uint8_t * pRGB = out +((height- j) * width +i)*4;
			uint8_t * pIn = in + ((j-1)*width + i)*4;
			*pRGB = *pIn; //
			*(pRGB+1) = *(pIn+1); //
			*(pRGB+2) = *(pIn+2); //
			*(pRGB+3) = *(pIn+3); //A
		}
	}
}


void transformRGB(uint8_t * in, uint8_t* out, int width, int height)
{
	int rgbwidth = width/2;
	for(int j = height; j > 0 ; j--)
	{
		for(int i= 0;  i < rgbwidth; i++)
		{
			uint8_t * pRGB = out +((height- j) * width +i)*4;
			uint8_t * pIn = in + ((j-1)*width + i)*4;
			*pRGB = *pIn; //
			*(pRGB+1) = *(pIn+1); //
			*(pRGB+2) = *(pIn+2); //
			*(pRGB+3) = *(pIn + rgbwidth *4); //A
		}
	}
}

void transformRGBhalf(uint8_t * in, uint8_t* out, int width, int height)
{ //输出一半
	int rgbwidth = width/2;
	for(int j = 0; j < height ; j++)
	{
		for(int i= 0;  i < rgbwidth; i++)
		{
			uint8_t * pRGB = out +(j *rgbwidth  +i)*4;
			uint8_t * pIn = in + (j*width + i)*4;
			*pRGB = *pIn; //
			*(pRGB+1) = *(pIn+1); //
			*(pRGB+2) = *(pIn+2); //
			*(pRGB+3) = *(pIn + rgbwidth *4); //A	
		}
	}
}

void transformRGBhalfRotate(uint8_t * in, uint8_t* out, int width, int height)
{ //输出一半,并旋转180度
	int rgbwidth = width/2;
	for(int j = height; j > 0 ; j--)
	{
		for(int i= 0;  i < rgbwidth; i++)
		{
			uint8_t * pRGB = out +((height- j) * rgbwidth +i)*4;
			uint8_t * pIn = in + ((j-1)*width + i)*4;
			*pRGB = *pIn; //
			*(pRGB+1) = *(pIn+1); //
			*(pRGB+2) = *(pIn+2); //
			*(pRGB+3) = *(pIn + rgbwidth *4); //A	
		}
	}
}

bool   CheckFolderExist( const   wstring   & strPath)
{
	WIN32_FIND_DATA  wfd;
	bool  rValue  =   false ;
	HANDLE hFind  =  FindFirstFile(strPath.c_str(),  & wfd);
	if  ((hFind  !=  INVALID_HANDLE_VALUE)  &&  (wfd.dwFileAttributes  &  FILE_ATTRIBUTE_DIRECTORY))
	{
		rValue  =   true ;   
	}
	FindClose(hFind);
	return  rValue;
}

void saveRGBAfiles(uint8_t* rgbadata, int width, int height)
{
	static int filecout = 1;

	int depth = 4;
	BITMAPFILEHEADER fileHeader;
	fileHeader.bfType = 0x4d42; // BM
	fileHeader.bfReserved1 = 0;
	fileHeader.bfReserved2 = 0 ;
	fileHeader.bfSize = width * height * depth + sizeof(BITMAPFILEHEADER) + sizeof(BITMAPINFOHEADER);
	fileHeader.bfOffBits = sizeof(BITMAPFILEHEADER) + sizeof(BITMAPINFOHEADER) ;

	BITMAPINFOHEADER infoHeader;
	infoHeader.biBitCount = depth * 8;
	infoHeader.biClrImportant = 0;
	infoHeader.biClrUsed = 0;
	infoHeader.biCompression = BI_RGB ;
	infoHeader.biHeight = height;
	infoHeader.biWidth = width;
	infoHeader.biPlanes = 1;
	infoHeader.biSize = sizeof(BITMAPINFOHEADER);
	infoHeader.biSizeImage = width * height * depth;
	infoHeader.biXPelsPerMeter = 3779;
	infoHeader.biYPelsPerMeter = 3779;


	std::wstring dir;
	dir.resize(MAX_PATH);
	::GetModuleFileNameW(NULL, &dir[0], MAX_PATH);
	dir.resize(dir.rfind('\\'));
	dir.append(L"/rgbs/");

	if (filecout == 1 && !CheckFolderExist(dir))
	{
		::CreateDirectory(dir.c_str(), NULL);

	}
	FILE * fp = NULL;
	wchar_t filename[200]={0};
	if(filecout < 10){
		swprintf(filename, L"rgb00%d.bmp", filecout++);
	}
	else if(filecout < 100){
		swprintf(filename, L"rgb0%d.bmp", filecout++);
	}
	else
	{
		swprintf(filename, L"rgb%d.bmp", filecout++);
	}
	dir.append(filename);
	fp = _wfopen(dir.c_str(), (L"wb"));
	size_t writeCount = fwrite(&fileHeader, sizeof(BITMAPFILEHEADER), 1, fp);
	writeCount = fwrite(&infoHeader, sizeof(BITMAPINFOHEADER), 1, fp);

	writeCount = fwrite(rgbadata,  width * height * depth, 1, fp);
	fclose(fp);
}

#include <windows.h>

#define WIN_WIDTH 500
#define WIN_HEIGHT 720
#define IDT_REDNER_TIMER  200514
#define IDT_CHECK_AUDIO_TIMER  200713




void PlayerWindow::releaseWinRenderResources()
{
	if(hCompatibleDC != NULL)
	{
		DeleteObject(hCompatibleDC);
		hCompatibleDC =NULL;
	}
	if(hCustomBmp != NULL )
	{
		DeleteObject(hCustomBmp); 
		hCustomBmp = NULL;
	}
}

void PlayerWindow::render(HWND hwnd, uint8_t * data, int WIDTH, int HEIGHT, bool rotate)
{

	if(NULL == hdc)
	{
		hdc = GetDC(hwnd);
	}
	if( hCompatibleDC == NULL)
	{
		hCompatibleDC = CreateCompatibleDC(NULL);
		hCustomBmp = CreateCompatibleBitmap(hdc, WIDTH, HEIGHT); //创建一副与当前DC兼容的位图
	}
	SelectObject(hCompatibleDC, hCustomBmp);

	BITMAPINFO bmpInfo;
	bmpInfo.bmiHeader.biSize = sizeof(BITMAPINFOHEADER);
	bmpInfo.bmiHeader.biWidth = WIDTH;
	bmpInfo.bmiHeader.biHeight = rotate ? HEIGHT: -1 * HEIGHT;
	bmpInfo.bmiHeader.biPlanes = 1;
	bmpInfo.bmiHeader.biCompression = BI_RGB;
	bmpInfo.bmiHeader.biBitCount = 32;

	SetDIBits(hdc, hCustomBmp, 0, HEIGHT, data, &bmpInfo, DIB_RGB_COLORS); //使用指定的DIB颜色数据来设置位图中的像素

	BLENDFUNCTION blend = { 0 };
	blend.BlendOp = AC_SRC_OVER;
	blend.SourceConstantAlpha = 255;
	blend.AlphaFormat = AC_SRC_ALPHA;//按通道混合
	POINT	pSrc = { 0, 0 };
	SIZE	sizeWnd = { WIDTH, HEIGHT };
	UpdateLayeredWindow(hwnd, hdc, NULL, &sizeWnd, hCompatibleDC, &pSrc, NULL, &blend, ULW_ALPHA);//更新分层窗口
}

int getFrameIntervalms(int num, int den)
{
	int intervalms = 50; //默认20帧/s
	if(num > 0 && den > 0 && num > den)
	{
		int tMs = 1000 * den / num;
		if(tMs < 200 && tMs > 20)
		{//     5< pts< 50， 帧率不要太高或太低
			intervalms = tMs;
		}
	}
	return intervalms;
}

bool PlayerWindow::init(HWND parentHwnd, int x, int y, int w, int h)
{
	if(m_hwnd)
	{ //只初始化一次
	    setPlayPosition( x, y, w, h);
		return true;
	}
	if(w <= 0 || h <= 0 )
	{//非合法大小 不播放 
		return false;
	}
	TCHAR		szAppName[] = TEXT("videoplayerwindow");
	WNDCLASSEX	wndClass;
	wndClass.cbSize = sizeof(WNDCLASSEX);
	wndClass.cbClsExtra = 0;
	wndClass.cbWndExtra = 0;
	wndClass.hbrBackground =  (HBRUSH)GetStockObject(BLACK_BRUSH);
	wndClass.hCursor = LoadCursor(NULL, IDC_ARROW);
	wndClass.hIcon = LoadIcon(NULL, IDI_APPLICATION);
	wndClass.hIconSm = NULL;
	wndClass.hInstance = GetModuleHandle(NULL);
	wndClass.lpszClassName = TEXT("VideoPlayerWindow");
	wndClass.lpszMenuName = NULL;
	wndClass.lpfnWndProc = WndProc; 
	wndClass.style = CS_HREDRAW | CS_VREDRAW | CS_IME | CS_DBLCLKS;

	RegisterClassEx(&wndClass);
	m_hwnd = CreateWindowEx(WS_EX_LAYERED, TEXT("VideoPlayerWindow"), szAppName, WS_CLIPCHILDREN | WS_CLIPSIBLINGS | WS_POPUP/*无边框风格*/
		, 300, 200, WIN_WIDTH, WIN_HEIGHT,
		parentHwnd, NULL, GetModuleHandle(NULL), NULL);
	setTransparent(m_bTransparent);// 设置键鼠穿透
	SetWindowLong(m_hwnd, GWL_USERDATA, (long)this);
	m_width = w;
	m_height = h;
	//设置本窗口为分层窗口支持透明
	//分层窗口没有WM_PAINT消息
	ShowWindow(m_hwnd, SW_SHOWNOACTIVATE);
	UpdateWindow(m_hwnd);
	return true;
}

void PlayerWindow::setPlayPosition(int x, int y, int w, int h)
{
	if(w <= 0 || h <= 0)
	{
		return;
	}
	SetWindowPos(m_hwnd,NULL, x, y, w, h,SWP_NOACTIVATE);
	if(m_width != w || m_height != h )
	{
		m_width = w;
		m_height = h;
		releaseWinRenderResources();
		delete [] outRGBA;
		outRGBA = NULL;
		delete [] out;
		out = NULL;
		if(m_pSwsContextYUV2BGRA)
		{//清理缩放上下文
			FfmpegFunctions::getInstance()->sws_freeContextPtr(m_pSwsContextYUV2BGRA);
			m_pSwsContextYUV2BGRA = NULL;
		}
	}
}

bool PlayerWindow::play(const string & filePath)
{
	stop();//做上一次视频的清理工作。
	m_filepath = filePath;

	m_playingAudio = false; //让音频播放停止取数据播放

	KillTimer(m_hwnd, IDT_CHECK_AUDIO_TIMER);
	SetTimer(m_hwnd,             // handle to main window 
		IDT_CHECK_AUDIO_TIMER,            // timer identifier 
		10,                 // ms interval 
		NULL);     // no timer callback 
	return true;
}

void PlayerWindow::stop()
{
	bool isPlaying = m_playing;
	KillTimer(m_hwnd, IDT_REDNER_TIMER); // 停掉定时器

	clearWin();

	m_playing = false;
	m_bReadFramesFinished = false;
	m_bClearWin = false;
	m_bLoop = false;
	releaseFFmpegResources();
}

int PlayerWindow::renderFrame()
{
	if(!m_playing)
	{
		return -1;
	}
	HWND hwnd = m_hwnd;

	if(!m_bReadFramesFinished)
	{
		bool needbreak = false;
		while(FfmpegFunctions::getInstance()->av_read_framePtr(pFormatCtx, packet)>=0){
			if(packet->stream_index==videoindex){
				int ret = FfmpegFunctions::getInstance()->avcodec_decode_video2Ptr(pCodecCtx, pFrame, &got_picture, packet);
				if(ret < 0){
					//printf("Decode Error.\n");
					FfmpegFunctions::getInstance()->av_packet_unrefPtr(packet);
					FfmpegFunctions::getInstance()->av_free_packetPtr(packet);
					return -1;
				}
				if(got_picture){
					FfmpegFunctions::getInstance()->sws_scalePtr(img_convert_ctx, (const unsigned char* const*)pFrame->data, pFrame->linesize, 0, pCodecCtx->height, 
						pFrameYUV->data, pFrameYUV->linesize);

					if(!m_pSwsContextYUV2BGRA)
					{
						m_pSwsContextYUV2BGRA = FfmpegFunctions::getInstance()->sws_getContextPtr(pCodecCtx->width, pCodecCtx->height, AV_PIX_FMT_YUV420P,
							m_width * 2, m_height, AV_PIX_FMT_BGRA, SWS_FAST_BILINEAR,
							NULL, NULL, NULL);
					}
					if(!out)
					{
						int size = m_width * 2 * m_height * 4;
						out = new uint8_t[size];
						outRGBA = new uint8_t[size];
					}
					int outWidth = m_width * 2;
					uint8_t *data[AV_NUM_DATA_POINTERS] = {0};
					data[0] = (uint8_t *)out;
					int linesize[AV_NUM_DATA_POINTERS] = { 0 };
					linesize[0] = outWidth * 4;
					int h = FfmpegFunctions::getInstance()->sws_scalePtr(m_pSwsContextYUV2BGRA, pFrameYUV->data, pFrameYUV->linesize, 0, pCodecCtx->height, data, linesize);
					if (h > 0){
						//printf("(%d)", h);
					}
					transformRGBhalf(out, outRGBA,m_width * 2,m_height);
					//transformRGBhalfRotate(out, outRGBA,m_width * 2,m_height);
					//saveRGBAfiles(outRGBA, m_width, m_height);
					render(hwnd, outRGBA, m_width,m_height);

					needbreak = true;

				}
			}
			FfmpegFunctions::getInstance()->av_packet_unrefPtr(packet);
			FfmpegFunctions::getInstance()->av_free_packetPtr(packet);
			if(needbreak)
			{
				break;
			}
		} // end of while
		if(!needbreak)
		{ //while循环结束
			m_bReadFramesFinished = true;
		}
	}//if m_bReadFramesFinished;

	if(m_bReadFramesFinished)
	{
		if(!m_bClearWin){
			bool isLastFrame= true;
			while(FfmpegFunctions::getInstance()->avcodec_decode_video2Ptr(pCodecCtx, pFrame, &got_picture, packet) >= 0) {
				if (!got_picture)
					break;
				FfmpegFunctions::getInstance()->sws_scalePtr(img_convert_ctx, (const unsigned char* const*)pFrame->data, pFrame->linesize, 0, pCodecCtx->height, 
					pFrameYUV->data, pFrameYUV->linesize);

					if(!m_pSwsContextYUV2BGRA)
					{
						m_pSwsContextYUV2BGRA = FfmpegFunctions::getInstance()->sws_getContextPtr(pCodecCtx->width, pCodecCtx->height, AV_PIX_FMT_YUV420P,
							m_width * 2, m_height, AV_PIX_FMT_BGRA, SWS_FAST_BILINEAR,
							NULL, NULL, NULL);
					}
					if(!out)
					{
						int size = m_width * 2 * m_height * 4;
						out = new uint8_t[size];
						outRGBA = new uint8_t[size];
					}
					int outWidth = m_width * 2;
					uint8_t *data[AV_NUM_DATA_POINTERS] = {0};
					data[0] = (uint8_t *)out;
					int linesize[AV_NUM_DATA_POINTERS] = { 0 };
					linesize[0] = outWidth * 4;
					int h = FfmpegFunctions::getInstance()->sws_scalePtr(m_pSwsContextYUV2BGRA, pFrameYUV->data, pFrameYUV->linesize, 0, pCodecCtx->height, data, linesize);
					if (h > 0){
						//printf("(%d)", h);
					}

					transformRGBhalf(out, outRGBA,m_width * 2,m_height);
					//transformRGBhalfRotate(out, outRGBA,m_width * 2,m_height); 
					//saveRGBAfiles(outRGBA, m_width, m_height);
					render(hwnd, outRGBA, m_width,m_height);

				isLastFrame = false;
				break;
			} // end of while
			if(isLastFrame)
			{
				// 清空画布
				m_bClearWin = true;
			}

			FfmpegFunctions::getInstance()->av_packet_unrefPtr(packet);
			FfmpegFunctions::getInstance()->av_free_packetPtr(packet);
		}
		else
		{//播完后清空画布，防止最后一帧占屏
			if(m_bLoop)
			{ // 循环播放
				FfmpegFunctions::getInstance()->av_seek_framePtr(pFormatCtx, videoindex, 0, AVSEEK_FLAG_BACKWARD);
				m_bReadFramesFinished = false;
			}
			else
			{
				stop();
				//只有正常播放结束才会发stop信号。其他情况不发。
				map<string,string> extend;
				if(PlayWindowHelperImple::getInstance())
				{
					PlayWindowHelperImple::getInstance()->sendEvent(playWindowId,VideoPlayerWindowComponent::PLAYERWINDOW_EVENTID_STOP,"end", extend);
				}
				
			}
		}
	}
	return 0;
}
static int idseed  = 1;
PlayerWindow::PlayerWindow():m_bReadFramesFinished(false),m_playing(false)
,m_bLoop(false)
,m_bClearWin(false)
,ldown(false)
,m_width(0)
,m_height(0)
,m_hwnd(NULL)
,playWindowId(idseed++)
,m_bTransparent(true)
,m_bFreeing(false)
{

	hdc = NULL;
	hCompatibleDC = NULL;
	hCustomBmp = NULL;
	TheFirstPoint.x = 0;
	TheFirstPoint.y = 0;


	pFormatCtx = NULL;
	videoindex = -1;
	pCodecCtx = NULL;
	pCodec = NULL;
	pFrame = NULL;
	pFrameYUV = NULL;
	out_buffer = NULL;
	packet = NULL;
	img_convert_ctx = NULL;
	m_filepath = "";
	m_pSwsContextYUV2BGRA = NULL;

	out = NULL;
	outRGBA = NULL;


	//audio
	audioStreamIndex = -1;
	pAudioCodecCtx = NULL;
	pAudioCodec = NULL;
	pAudiopacket = NULL;
	audio_out_buffer = NULL;
	pAudioFrame = NULL;
	audio_len = 0;
	audioIndex = 0; //count
	au_convert_ctx = NULL;
	pAudioFormatCtx = NULL;
	hwo = NULL;
	wh1.lpData = NULL;
	wh2.lpData = NULL;
	wh3.lpData = NULL;
	wh4.lpData = NULL;
	m_playingWAVEHDRCount = 0;
	m_volume = 1.0;

}


LRESULT PlayerWindow::MyProc(HWND hwnd,UINT msg,WPARAM wParam,LPARAM lParam)
{
	//static BOOL		ldown;
	//static POINT	TheFirstPoint;
	int x = 0;
	int y = 0;
	string str = "";

	switch (msg)
	{
	case WM_LBUTTONDOWN:
		//ldown = TRUE;
		//SetCapture(hwnd);
		//TheFirstPoint.x = LOWORD(lParam);
		//TheFirstPoint.y = HIWORD(lParam);
		break;
	case WM_LBUTTONUP:
		//ldown = FALSE;
		//ReleaseCapture();

		x = (int)LOWORD(lParam);//取低位
		y = (int)HIWORD(lParam);//取高位
		char buffer[256];
		memset(buffer,0,256);
		sprintf_s (buffer,256,"%d,%d",x,y);
		str.append(buffer);
		if(PlayWindowHelperImple::getInstance())
		{
			PlayWindowHelperImple::getInstance()->sendEvent(playWindowId,VideoPlayerWindowComponent::PLAYERWINDOW_EVENTID_CLICK,str, map<string,string>());
		}

		break;
	//case WM_MOUSEMOVE:
	//	if (ldown)
	//	{
	//		POINT	pt;
	//		GetCursorPos(&pt);
	//		pt.x -= TheFirstPoint.x;
	//		pt.y -= TheFirstPoint.y;
	//		SetWindowPos(hwnd, NULL, pt.x, pt.y, NULL, NULL, SWP_NOREDRAW |
	//			SWP_NOSIZE | SWP_NOZORDER);
	//	}
	//	break;
	case WM_TIMER:
		if(wParam == IDT_REDNER_TIMER)
		{
			renderFrame();
		}
		else if(wParam == IDT_CHECK_AUDIO_TIMER)
		{
			if(m_playingWAVEHDRCount == 0)
			{
				KillTimer(m_hwnd, IDT_CHECK_AUDIO_TIMER); // 停掉定时器
				releaseFFmpegAudioResources();//清理音频资源
				playVideo();
			}
		}
		break;
	}
	return DefWindowProc(hwnd, msg, wParam, lParam);
}

PlayerWindow::~PlayerWindow()
{
	m_playingAudio = false;//停掉音频播放
	KillTimer(m_hwnd, IDT_CHECK_AUDIO_TIMER); // 停掉定时器


	m_bFreeing = true;
	stop();
	if(hdc)
	{
		ReleaseDC(m_hwnd, hdc);
		hdc = NULL;
	}
	releaseWinRenderResources();


	while(m_playingWAVEHDRCount > 0)
	{
		Sleep(1);
	}
	releaseFFmpegAudioResources();
	
	if(m_hwnd)
	{
		DestroyWindow(m_hwnd);
		m_hwnd = NULL;
	}
}

void PlayerWindow::releaseFFmpegResources()
{
	if(out != NULL)
	{
		delete[] out;
		out = NULL;
	}
	if(outRGBA != NULL)
	{
		delete[] outRGBA;
		outRGBA = NULL;
	}

	if(packet)
	{
		FfmpegFunctions::getInstance()->av_freePtr(packet);
		packet = NULL;
	}

	if(out_buffer)
	{
		FfmpegFunctions::getInstance()->av_freePtr(out_buffer);
		out_buffer = NULL;
	}
	if(m_pSwsContextYUV2BGRA != NULL)
	{
		FfmpegFunctions::getInstance()->sws_freeContextPtr(m_pSwsContextYUV2BGRA);
		m_pSwsContextYUV2BGRA = NULL;
	}
	if(img_convert_ctx != NULL)
	{
		FfmpegFunctions::getInstance()->sws_freeContextPtr(img_convert_ctx);
		img_convert_ctx = NULL;
	}
	if(pFrameYUV != NULL)
	{
		FfmpegFunctions::getInstance()->av_frame_freePtr(&pFrameYUV);
		pFrameYUV = NULL;
	}
	if(pFrame != NULL)
	{
		FfmpegFunctions::getInstance()->av_frame_freePtr(&pFrame);
		pFrame = NULL;
	}
	if(pCodecCtx != NULL)
	{
		FfmpegFunctions::getInstance()->avcodec_closePtr(pCodecCtx);
		pCodecCtx = NULL;
	}
	if(pFormatCtx != NULL)
	{
		FfmpegFunctions::getInstance()->avformat_close_inputPtr(&pFormatCtx);
		pFormatCtx = NULL;
	}
	//sws_freeContext(m_pSwsContextYUV2BGRA);
	//sws_freeContext(img_convert_ctx);
	//av_frame_free(&pFrameYUV);
	//av_frame_free(&pFrame);
	//avcodec_close(pCodecCtx);
	//avformat_close_input(&pFormatCtx);
}

void PlayerWindow::clearWin()
{
	if(m_hwnd && outRGBA)
	{
		int size = m_width *2 * m_height * 4 ;
		memset(outRGBA,0,size); //初始化位图
		render(m_hwnd, outRGBA, m_width, m_height ,true);
	}
}

int PlayerWindow::getPlayerWindowID()
{
	return playWindowId;
}

void PlayerWindow::setVisible(bool isVisible)
{
	::ShowWindow(m_hwnd, isVisible?SW_SHOWNOACTIVATE:SW_HIDE);
}

void PlayerWindow::setPlayerWindowID(int id)
{
	playWindowId = id;
}

void PlayerWindow::setTransparent(bool isTransparent)
{
	m_bTransparent = isTransparent;
	if(m_hwnd)
	{
		if(m_bTransparent)
		{
			SetWindowLong(m_hwnd, GWL_EXSTYLE, (GetWindowLong(m_hwnd, GWL_EXSTYLE) | WS_EX_TRANSPARENT) );
		}
		else
		{
			SetWindowLong(m_hwnd, GWL_EXSTYLE, (GetWindowLong(m_hwnd, GWL_EXSTYLE) &(~WS_EX_TRANSPARENT)) );
		}
	}
}


void RaiseVolume(char* buf, int size, UINT32 uRepeat, double vol)//buf为需要调节音量的音频数据块首地址指针，size为长度，uRepeat为重复次数，通常设为1，vol为增益倍数,可以小于1
{
	if (size <= 0)
	{
		return;
	}
	for (int i = 0; i < size - 1; i += 2)
	{
		short wData;
		wData = MAKEWORD(buf[i], buf[i + 1]);
		long dwData = wData;
		for (int j = 0; j < uRepeat; j++)
		{
			dwData = dwData * vol;
			if (dwData < -0x8000)
			{
				dwData = -0x8000;
			}
			else if (dwData > 0x7FFF)
			{
				dwData = 0x7FFF;
			}
		}
		wData = LOWORD(dwData);
		buf[i] = LOBYTE(wData);
		buf[i + 1] = HIBYTE(wData);
	}
}

void PlayerWindow::playSound(PlayerWindow *pPlayerWindow,LPWAVEHDR pWaveHeader)
{
	int got_audio = 0;
	while(FfmpegFunctions::getInstance()->av_read_framePtr(pPlayerWindow->pAudioFormatCtx, pPlayerWindow->pAudiopacket)>=0){
		if(pAudiopacket->stream_index==audioStreamIndex){
			int ret = FfmpegFunctions::getInstance()->avcodec_decode_audio4Ptr( pPlayerWindow->pAudioCodecCtx,pPlayerWindow->pAudioFrame,&got_audio, pPlayerWindow->pAudiopacket);
			if ( ret < 0 ) {
				printf("Error in decoding audio frame.\n");
				FfmpegFunctions::getInstance()->av_packet_unrefPtr(pPlayerWindow->pAudiopacket);
				FfmpegFunctions::getInstance()->av_free_packetPtr(pPlayerWindow->pAudiopacket);
				return;
			}
			if ( got_audio > 0 ){

				FfmpegFunctions::getInstance()->swr_convertPtr(pPlayerWindow->au_convert_ctx,&pPlayerWindow->audio_out_buffer, MAX_AUDIO_FRAME_SIZE,(const uint8_t **)pPlayerWindow->pAudioFrame->data , pPlayerWindow->pAudioFrame->nb_samples);
				pWaveHeader->dwBufferLength = pPlayerWindow->audio_out_buffer_size;
				memcpy_s(pWaveHeader->lpData,DATASIZE,pPlayerWindow->audio_out_buffer, pPlayerWindow->audio_out_buffer_size);

				//调整音量大小
				int bufsize = pPlayerWindow->audio_out_buffer_size < DATASIZE ? pPlayerWindow->audio_out_buffer_size: DATASIZE;
				RaiseVolume(pWaveHeader->lpData,bufsize, 1, m_volume);

				waveOutPrepareHeader(pPlayerWindow->getHwo(), pWaveHeader, sizeof(WAVEHDR));
				waveOutWrite(pPlayerWindow->getHwo(), pWaveHeader, sizeof(WAVEHDR));

				InterlockedExchangeAdd(&pPlayerWindow->m_playingWAVEHDRCount, 1);
				FfmpegFunctions::getInstance()->av_packet_unrefPtr(pPlayerWindow->pAudiopacket);
				FfmpegFunctions::getInstance()->av_free_packetPtr(pPlayerWindow->pAudiopacket);
				break;
			}
		}
		FfmpegFunctions::getInstance()->av_packet_unrefPtr(pPlayerWindow->pAudiopacket);
		FfmpegFunctions::getInstance()->av_free_packetPtr(pPlayerWindow->pAudiopacket);
	}
	FfmpegFunctions::getInstance()->av_packet_unrefPtr(pPlayerWindow->pAudiopacket);
	FfmpegFunctions::getInstance()->av_free_packetPtr(pPlayerWindow->pAudiopacket);
}

bool PlayerWindow::startPlaySound()
{
	string errorStr;
	pAudioFormatCtx = FfmpegFunctions::getInstance()->avformat_alloc_contextPtr();

	if(FfmpegFunctions::getInstance()->avformat_open_inputPtr(&pAudioFormatCtx,m_filepath.c_str(),NULL,NULL)!=0){
		errorStr.append("Couldn't open input stream.");
		return false;
	}
	if(FfmpegFunctions::getInstance()->avformat_find_stream_infoPtr(pAudioFormatCtx,NULL)<0){
		errorStr.append("Couldn't find stream information..");
		return false;
	}

	audioStreamIndex = -1;
	for( int i=0; i < pAudioFormatCtx->nb_streams; i++)
	{
		if(pAudioFormatCtx->streams[i]->codec->codec_type==AVMEDIA_TYPE_AUDIO){
			audioStreamIndex=i;
			break;
		}
	}
	if(audioStreamIndex == -1){
		return false;
	}
	// Get a pointer to the codec context for the audio stream
	pAudioCodecCtx=pAudioFormatCtx->streams[audioStreamIndex]->codec;

	// Find the decoder for the audio stream
	pAudioCodec=FfmpegFunctions::getInstance()->avcodec_find_decoderPtr(pAudioCodecCtx->codec_id);
	if(pAudioCodec==NULL){
		return false;
	}
	if(FfmpegFunctions::getInstance()->avcodec_open2Ptr(pAudioCodecCtx, pAudioCodec,NULL)<0){
		errorStr.append("Could not open codec.");
		return false;
	}

	pAudiopacket=(AVPacket *)FfmpegFunctions::getInstance()->av_mallocPtr(sizeof(AVPacket));
	//FfmpegFunctions::getInstance()->avini(pAudiopacket);

	//Out Audio Param
	uint64_t out_channel_layout=AV_CH_LAYOUT_STEREO;
	//nb_samples: AAC-1024 MP3-1152
	int out_nb_samples=pAudioCodecCtx->frame_size;
	AVSampleFormat out_sample_fmt=AV_SAMPLE_FMT_S16;
	int out_sample_rate=44100;
	if(pAudioCodecCtx)
	{
		out_sample_rate = pAudioCodecCtx->sample_rate;
	}
	int out_channels=FfmpegFunctions::getInstance()->av_get_channel_layout_nb_channelsPtr(out_channel_layout);

	//Out Buffer Size
	audio_out_buffer_size = FfmpegFunctions::getInstance()->av_samples_get_buffer_sizePtr(NULL,out_channels ,out_nb_samples,out_sample_fmt, 1);

	audio_out_buffer=(uint8_t *)FfmpegFunctions::getInstance()->av_mallocPtr(MAX_AUDIO_FRAME_SIZE*2);
	pAudioFrame=FfmpegFunctions::getInstance()->av_frame_allocPtr();

	//FIX:Some Codec's Context Information is missing
	in_channel_layout=FfmpegFunctions::getInstance()->av_get_default_channel_layoutPtr(pAudioCodecCtx->channels);
	//Swr

	au_convert_ctx = FfmpegFunctions::getInstance()->swr_allocPtr();
	au_convert_ctx=FfmpegFunctions::getInstance()->swr_alloc_set_optsPtr(au_convert_ctx,out_channel_layout, out_sample_fmt, out_sample_rate,
		in_channel_layout,pAudioCodecCtx->sample_fmt , pAudioCodecCtx->sample_rate,0, NULL);
	FfmpegFunctions::getInstance()->swr_initPtr(au_convert_ctx);


	wfx.wFormatTag = WAVE_FORMAT_PCM;//设置波形声音的格式
	wfx.nChannels = pAudioCodecCtx->channels;//2;//设置音频文件的通道数量
	//wfx.nSamplesPerSec = 48000;//设置每个声道播放和记录时的样本频率,看具体文件，一般44100
	wfx.nSamplesPerSec = out_sample_rate;
	wfx.wBitsPerSample = pAudioCodecCtx->bits_per_coded_sample;// 16;
	wfx.nBlockAlign = (wfx.wBitsPerSample >> 3) * wfx.nChannels;
	wfx.nAvgBytesPerSec = wfx.nBlockAlign * wfx.nSamplesPerSec;
	wfx.cbSize = 0;//额外信息的大小
	//打开一个给定的波形音频输出装置来进行声音播放，方式为回调函数方式。如果是对话框程序，可以将第五个参数改为(DWORD)this，操作跟本Demo程序相似
	int ret =  waveOutOpen(&hwo, WAVE_MAPPER, &wfx, (DWORD)WaveCallback, (DWORD)this, CALLBACK_FUNCTION);
	if( ret != MMSYSERR_NOERROR)
	{
		errorStr.append("waveOutOpen error");
		return false;
	}
	//音量设置
	setVolume(m_volume);

	m_playingAudio = true;

	wh1.dwLoops = 0L;//播放区一
	wh1.lpData = new char[DATASIZE];
	wh1.dwFlags = 0L;
	playSound(this,&wh1);

	wh2.dwLoops = 0L;//播放区二，基本同上
	wh2.lpData = new char[DATASIZE];
	wh2.dwFlags = 0L;
	playSound(this,&wh2);

	wh3.dwLoops = 0L;//播放区三，基本同上
	wh3.lpData = new char[DATASIZE];
	wh3.dwFlags = 0L;
	playSound(this,&wh3);

	wh4.dwLoops = 0L;//播放区四，基本同上
	wh4.lpData = new char[DATASIZE];
	wh4.dwFlags = 0L;
	playSound(this,&wh4);
	return true;
}

void PlayerWindow::releaseFFmpegAudioResources()
{
	//需注意与音频播放线程同步。需等音频播放线程停止工作，才能释放对应的音频播放资源。

	//swr_free(&au_convert_ctx);
	//av_free(out_buffer);
	//avcodec_close(pCodecCtx);
	//avformat_close_input(&pFormatCtx);


	if(hwo)
	{
		waveOutUnprepareHeader(hwo, &wh1, sizeof(WAVEHDR));//清理数据
		waveOutUnprepareHeader(hwo, &wh2, sizeof(WAVEHDR));
		waveOutUnprepareHeader(hwo, &wh3, sizeof(WAVEHDR));
		waveOutUnprepareHeader(hwo, &wh4, sizeof(WAVEHDR));
		waveOutClose(hwo);
		hwo = NULL;
		if(wh1.lpData)
		{
			delete []wh1.lpData;
			wh1.lpData = NULL;
		}
		if(wh2.lpData)
		{
			delete []wh2.lpData;
			wh2.lpData = NULL;
		}
		if(wh3.lpData)
		{
			delete []wh3.lpData;
			wh3.lpData = NULL;
		}
		if(wh4.lpData)
		{
			delete []wh4.lpData;
			wh4.lpData = NULL;
		}
	}

	if(pAudiopacket)
	{
		FfmpegFunctions::getInstance()->av_freePtr(pAudiopacket);
		pAudiopacket = NULL;
	}

	if(au_convert_ctx)
	{
		FfmpegFunctions::getInstance()->swr_freePtr(&au_convert_ctx);
		au_convert_ctx = NULL;
	}

	if(audio_out_buffer)
	{
		FfmpegFunctions::getInstance()->av_freePtr(audio_out_buffer);
		audio_out_buffer = NULL;
	}
	if(pAudioFrame != NULL)
	{
		FfmpegFunctions::getInstance()->av_frame_freePtr(&pAudioFrame);
		pAudioFrame = NULL;
	}
	if(pAudioCodecCtx != NULL)
	{
		FfmpegFunctions::getInstance()->avcodec_closePtr(pAudioCodecCtx);
		pAudioCodecCtx = NULL;
	}
	if(pAudioFormatCtx != NULL)
	{
		FfmpegFunctions::getInstance()->avformat_close_inputPtr(&pAudioFormatCtx);
		pAudioFormatCtx = NULL;
	}

}

bool PlayerWindow::playVideo()
{
	string errorStr;

	pFormatCtx = FfmpegFunctions::getInstance()->avformat_alloc_contextPtr();

	if(FfmpegFunctions::getInstance()->avformat_open_inputPtr(&pFormatCtx,m_filepath.c_str(),NULL,NULL)!=0){
		errorStr.append("Couldn't open input stream.");
		return false;
	}
	if(FfmpegFunctions::getInstance()->avformat_find_stream_infoPtr(pFormatCtx,NULL)<0){
		errorStr.append("Couldn't find stream information..");
		return false;
	}
	videoindex=-1;
	for(int i=0; i<pFormatCtx->nb_streams; i++) 
	{
		if(pFormatCtx->streams[i]->codec->codec_type==AVMEDIA_TYPE_VIDEO){
			videoindex=i;
			break;
		}
	}
	if(videoindex==-1){
		errorStr.append("Didn't find a video stream.");
		return false;
	}

	pCodecCtx=pFormatCtx->streams[videoindex]->codec; 
	pCodec= FfmpegFunctions::getInstance()->avcodec_find_decoderPtr(pCodecCtx->codec_id);
	if(pCodec==NULL){
		errorStr.append("Codec not found.");
		return false;
	}
	if(FfmpegFunctions::getInstance()->avcodec_open2Ptr(pCodecCtx, pCodec,NULL)<0){
		errorStr.append("Could not open codec.");
		return false;
	}

	pFrame=FfmpegFunctions::getInstance()->av_frame_allocPtr();
	pFrameYUV=FfmpegFunctions::getInstance()->av_frame_allocPtr();
	out_buffer=(unsigned char *)FfmpegFunctions::getInstance()->av_mallocPtr(FfmpegFunctions::getInstance()->av_image_get_buffer_sizePtr(AV_PIX_FMT_YUV420P,  pCodecCtx->width, pCodecCtx->height,1));
	FfmpegFunctions::getInstance()->av_image_fill_arraysPtr(pFrameYUV->data, pFrameYUV->linesize,out_buffer,
		AV_PIX_FMT_YUV420P,pCodecCtx->width, pCodecCtx->height,1);

	packet=(AVPacket *)FfmpegFunctions::getInstance()->av_mallocPtr(sizeof(AVPacket));

	FfmpegFunctions::getInstance()->av_dump_formatPtr(pFormatCtx,0,m_filepath.c_str(),0);

	img_convert_ctx = FfmpegFunctions::getInstance()->sws_getContextPtr(pCodecCtx->width, pCodecCtx->height, pCodecCtx->pix_fmt, 
		pCodecCtx->width, pCodecCtx->height, AV_PIX_FMT_YUV420P, SWS_FAST_BILINEAR, NULL, NULL, NULL); 


	m_pSwsContextYUV2BGRA = FfmpegFunctions::getInstance()->sws_getContextPtr(pCodecCtx->width, pCodecCtx->height, AV_PIX_FMT_YUV420P,
		m_width * 2, m_height, AV_PIX_FMT_BGRA, SWS_FAST_BILINEAR,
		NULL, NULL, NULL);
	int size = m_width * 2 * m_height * 4 ;
	out = new uint8_t[size];
	memset(out,0,size); //初始化位图
	outRGBA = new uint8_t[size];
	memset(outRGBA,0,size); //初始化位图


	//pFormatCtx->streams[videoindex]->r_frame_rate，帧率
	int intervalms = getFrameIntervalms(pFormatCtx->streams[videoindex]->r_frame_rate.num, pFormatCtx->streams[videoindex]->r_frame_rate.den);
	SetTimer(m_hwnd,             // handle to main window 
		IDT_REDNER_TIMER,            // timer identifier 
		intervalms,                 // ms interval 
		NULL);     // no timer callback 


	m_playing = true;

	//audio************************************start

	bool needPlaySoud = false;
	for(int i=0; i<pFormatCtx->nb_streams; i++) 
	{
		if(pFormatCtx->streams[i]->codec->codec_type==AVMEDIA_TYPE_AUDIO){
			needPlaySoud = true;
			break;
		}
	}
	if(m_volume < 0.0001)
	{
		needPlaySoud = false;
	}
	if(needPlaySoud)
	{
		startPlaySound();
	}
	//audio************************************end

	return true;
}

void PlayerWindow::setVolume(double volume)
{
	m_volume = volume;
	//to to: delete,后期可删掉以下设置
	if(hwo)
	{
		//音量设置
		//https://docs.microsoft.com/zh-cn/windows/win32/api/mmeapi/nf-mmeapi-waveoutsetvolume?redirectedfrom=MSDN
		DWORD tVolume;
		DWORD MaxVolume = 0xFFFF;
		//double v = m_volume;
		double v = 1.0; // 用于恢复所在程序的音量为100%，可通用AppAudioConfig.exe查看。
		v = (v < 1.0 && v>= 0)? v : 1.0;
		tVolume = v * MaxVolume;
		tVolume = (tVolume << 16) + tVolume;
		waveOutSetVolume(hwo,tVolume);
	}
}


LRESULT CALLBACK WndProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	PlayerWindow *w = (PlayerWindow *) GetWindowLong(hwnd, GWL_USERDATA);
	if (w)
		return w->MyProc(hwnd, msg, wParam, lParam);
	else
		return DefWindowProc(hwnd, msg, wParam, lParam);
}









