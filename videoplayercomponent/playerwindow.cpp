#include "stdafx.h"
#include "playerwindow.h"
#include "ffmpegfunctions.h"
#include <Windows.h>
#include <WinUser.h>


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
	for(int j = height; j > 0 ; j--)
	{
		for(int i= 0;  i < rgbwidth; i++)
		{
			uint8_t * pRGB = out +((height- j) *rgbwidth  +i)*4;
			uint8_t * pIn = in + ((j-1)*width + i)*4;
			*pRGB = *pIn; //
			*(pRGB+1) = *(pIn+1); //
			*(pRGB+2) = *(pIn+2); //
			*(pRGB+3) = *(pIn + rgbwidth *4); //A	
		}
	}
}


void saveRGBAfiles(uint8_t* rgbadata, int width, int height)
{
	return;
	static int filecout = 0;

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

	FILE * fp = NULL;
	char filename[200]={0};
	sprintf(filename, "./rgbs/rgb%d.bmp", filecout++);
	fp = fopen(filename, ("wb"));
	size_t writeCount = fwrite(&fileHeader, sizeof(BITMAPFILEHEADER), 1, fp);
	writeCount = fwrite(&infoHeader, sizeof(BITMAPINFOHEADER), 1, fp);

	writeCount = fwrite(rgbadata,  width * height * depth, 1, fp);
	fclose(fp);
}

#include <windows.h>

#define WIN_WIDTH 500
#define WIN_HEIGHT 720
#define IDT_REDNER_TIMER  200514




void PlayerWindow::releaseResources()
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
	HDC hdc;
	hdc = GetDC(hwnd);
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

	ReleaseDC(hwnd, hdc);
}



bool PlayerWindow::init(HWND parentHwnd, int x, int y, int w, int h)
{
	if(w <= 0 || h <= 0 )
	{//非合法大小 不播放 
		return false;
	}
	TCHAR		szAppName[] = TEXT("videoplayerwindow");
	WNDCLASSEX	wndClass;
	MSG			msg;
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
	//设置本窗口为分层窗口支持透明
	//分层窗口没有WM_PAINT消息

	SetWindowLong(m_hwnd, GWL_USERDATA, (long)this);
	ShowWindow(m_hwnd, SW_SHOWNOACTIVATE);
	UpdateWindow(m_hwnd);
	return true;
}

void PlayerWindow::setPlayPosition(int x, int y, int w, int h)
{

}

bool PlayerWindow::Play(const string & filePath)
{

	releaseResources();
	stop();
	//做上一次的清理工作。
	string errorStr;

	m_playing = true;

	m_filepath = filePath;

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
		if(pFormatCtx->streams[i]->codec->codec_type==AVMEDIA_TYPE_VIDEO){
			videoindex=i;
			break;
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
		//Output Info-----------------------------
		//printf("--------------- File Information ----------------\n");
		FfmpegFunctions::getInstance()->av_dump_formatPtr(pFormatCtx,0,m_filepath.c_str(),0);
		//printf("-------------------------------------------------\n");
		img_convert_ctx = FfmpegFunctions::getInstance()->sws_getContextPtr(pCodecCtx->width, pCodecCtx->height, pCodecCtx->pix_fmt, 
			pCodecCtx->width, pCodecCtx->height, AV_PIX_FMT_YUV420P, SWS_BICUBIC, NULL, NULL, NULL); 



		m_pSwsContextYUV2BGRA = FfmpegFunctions::getInstance()->sws_getContextPtr(pCodecCtx->width, pCodecCtx->height, AV_PIX_FMT_YUV420P,
			pCodecCtx->width, pCodecCtx->height, AV_PIX_FMT_BGRA, SWS_BICUBIC,
			NULL, NULL, NULL);
		out = NULL;
		int size = pCodecCtx->width * pCodecCtx->height * 4 ;
		out = new uint8_t[size];
		memset(out,0,size); //初始化位图

		outRGBA = NULL;
		outRGBA = new uint8_t[size];
		memset(outRGBA,0,size); //初始化位图


		SetWindowPos(m_hwnd, NULL, 300, 200, pCodecCtx->width/2, pCodecCtx->height, SWP_NOMOVE );

		SetTimer(m_hwnd,             // handle to main window 
			IDT_REDNER_TIMER,            // timer identifier 
			50,                 // ms interval 
			NULL);     // no timer callback 
		return 0;
}

void PlayerWindow::stop()
{
	KillTimer(m_hwnd, IDT_REDNER_TIMER); // 停掉定时器

	m_playing = false;
	m_bReadFramesFinished = false;
	m_bClearWin = false;
	m_filepath = "";
	releaseFFmpegResources();
	releaseResources();
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
					return -1;
				}
				if(got_picture){
					FfmpegFunctions::getInstance()->sws_scalePtr(img_convert_ctx, (const unsigned char* const*)pFrame->data, pFrame->linesize, 0, pCodecCtx->height, 
						pFrameYUV->data, pFrameYUV->linesize);
					int outWidth = pCodecCtx->width;
					uint8_t *data[AV_NUM_DATA_POINTERS] = {0};
					data[0] = (uint8_t *)out;
					int linesize[AV_NUM_DATA_POINTERS] = { 0 };
					linesize[0] = outWidth * 4;
					int h = FfmpegFunctions::getInstance()->sws_scalePtr(m_pSwsContextYUV2BGRA, pFrameYUV->data, pFrameYUV->linesize, 0, pCodecCtx->height, data, linesize);
					if (h > 0){
						//printf("(%d)", h);
					}

					transformRGBhalf(out, outRGBA,pCodecCtx->width,pCodecCtx->height);
					saveRGBAfiles(outRGBA,pCodecCtx->width/2,pCodecCtx->height);
					render(hwnd, outRGBA, pCodecCtx->width/2,pCodecCtx->height,true);


					//SetWindowPos(hwnd, NULL, 300, 200, pCodecCtx->width, pCodecCtx->height, SWP_SHOWWINDOW );
					//saveRGBAfiles(out,pCodecCtx->width,pCodecCtx->height);
					////transformRGBRotate(out, outRGBA,pCodecCtx->width,pCodecCtx->height);
					////render(hwnd, outRGBA, pCodecCtx->width,pCodecCtx->height );
					//render(hwnd, out, pCodecCtx->width,pCodecCtx->height );
					//Sleep(45);


					needbreak = true;

				}
			}
			FfmpegFunctions::getInstance()->av_free_packetPtr(packet);
			//packet = NULL;
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

				int outWidth = pCodecCtx->width;
				uint8_t *data[AV_NUM_DATA_POINTERS] = {0};
				data[0] = (uint8_t *)out;
				int linesize[AV_NUM_DATA_POINTERS] = { 0 };
				linesize[0] = outWidth * 4;
				int h = FfmpegFunctions::getInstance()->sws_scalePtr(m_pSwsContextYUV2BGRA, pFrameYUV->data, pFrameYUV->linesize, 0, pCodecCtx->height, data, linesize);
				if (h > 0){
					//printf("(%d)", h);
				}

				transformRGBhalf(out, outRGBA,pCodecCtx->width,pCodecCtx->height);
				saveRGBAfiles(outRGBA,pCodecCtx->width/2,pCodecCtx->height);
				render(hwnd, outRGBA, pCodecCtx->width/2,pCodecCtx->height ,true);


				//saveRGBAfiles(out,pCodecCtx->width,pCodecCtx->height);
				////transformRGBRotate(out, outRGBA,pCodecCtx->width,pCodecCtx->height);
				////render(hwnd, outRGBA, pCodecCtx->width,pCodecCtx->height );
				//render(hwnd, out, pCodecCtx->width,pCodecCtx->height );

				isLastFrame = false;
				break;
			} // end of while
			if(isLastFrame)
			{
				// 清空画布
				m_bClearWin = true;
			}
		}
		else
		{//播完后清空画布，防止最后一帧占屏
			int size = pCodecCtx->width * pCodecCtx->height * 4 ;
			memset(outRGBA,0,size); //初始化位图
			render(hwnd, outRGBA, pCodecCtx->width/2,pCodecCtx->height ,true);
			m_bClearWin = false;
			m_playing = false;
			KillTimer(m_hwnd, IDT_REDNER_TIMER); // 停掉定时器
			releaseFFmpegResources(); // 清理资源
		}
	}
	return 0;
}

PlayerWindow::PlayerWindow():m_bReadFramesFinished(false),m_playing(false)
,m_bClearWin(false)
,ldown(false)
,m_width(0)
,m_height(0)
{
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

}


LRESULT PlayerWindow::MyProc(HWND hwnd,UINT msg,WPARAM wParam,LPARAM lParam)
{
	//static BOOL		ldown;
	//static POINT	TheFirstPoint;
	switch (msg)
	{
	case WM_LBUTTONDOWN:
		ldown = TRUE;
		SetCapture(hwnd);
		TheFirstPoint.x = LOWORD(lParam);
		TheFirstPoint.y = HIWORD(lParam);
		break;
	case WM_LBUTTONUP:
		ldown = FALSE;
		ReleaseCapture();
		break;
	case WM_MOUSEMOVE:
		if (ldown)
		{
			POINT	pt;
			GetCursorPos(&pt);
			pt.x -= TheFirstPoint.x;
			pt.y -= TheFirstPoint.y;
			SetWindowPos(hwnd, NULL, pt.x, pt.y, NULL, NULL, SWP_NOREDRAW |
				SWP_NOSIZE | SWP_NOZORDER);
		}
		break;
	case WM_TIMER:
		renderFrame();
		break;
	}
	return DefWindowProc(hwnd, msg, wParam, lParam);
}

PlayerWindow::~PlayerWindow()
{
	stop();
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


LRESULT CALLBACK WndProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	PlayerWindow *w = (PlayerWindow *) GetWindowLong(hwnd, GWL_USERDATA);
	if (w)
		return w->MyProc(hwnd, msg, wParam, lParam);
	else
		return DefWindowProc(hwnd, msg, wParam, lParam);
}









