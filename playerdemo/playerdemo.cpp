#include <Windows.h>
#include <stdio.h>
#include <string>
#define __STDC_CONSTANT_MACROS

extern "C"
{
#include "libavcodec/avcodec.h"
#include "libavformat/avformat.h"
#include "libswscale/swscale.h"
#include "libavutil/imgutils.h"

};

#include "../videoplayercomponent/exportinclude/playerwindowinterface.h"

using namespace std;

#define WIDTH 200
#define HEIGHT 200

int c= 0;
char * data = NULL;
HDC hCompatibleDC = NULL;
HBITMAP hCustomBmp = NULL;

void releaseResources()
{
	if(data != NULL)delete[] data;
	if(hCompatibleDC != NULL)DeleteObject(hCompatibleDC);
	if(hCustomBmp != NULL )DeleteObject(hCustomBmp);
}

void render(HWND hwnd)
{
	HDC hdc;
	hdc = GetDC(hwnd);
	if( hCompatibleDC == NULL)
	{
		hCompatibleDC = CreateCompatibleDC(NULL);
		hCustomBmp = CreateCompatibleBitmap(hdc, WIDTH, HEIGHT); //����һ���뵱ǰDC���ݵ�λͼ
		SelectObject(hCompatibleDC, hCustomBmp);
	}

	BITMAPINFO bmpInfo;
	bmpInfo.bmiHeader.biSize = sizeof(BITMAPINFOHEADER);
	bmpInfo.bmiHeader.biWidth = WIDTH;
	bmpInfo.bmiHeader.biHeight = -HEIGHT;
	bmpInfo.bmiHeader.biPlanes = 1;
	bmpInfo.bmiHeader.biCompression = BI_RGB;
	bmpInfo.bmiHeader.biBitCount = 32;

	if(data == NULL)
	{
		data = new char[WIDTH * HEIGHT * 4 + 1];
	}
	memset(data,0,WIDTH*HEIGHT*4); //��ʼ��λͼ
	for(int i = 0 ; i < HEIGHT; i++)
	{
		c = (c+2) % 255;
		for(int j = 0; j <  WIDTH; j++)
		{
			if(i < 20 && j >= 30 && j < 60)
			{
				continue;// ȫ͸
			}
			char * pdata = data + (i*WIDTH+j)*4;
			*pdata = 0;   //Blue
			*(pdata+1)= c; //Ggeen
			*(pdata+2)= 0; //Red
			*(pdata+3)= 150; //Alpha
			if(i < 20 && j < 30)
			{
				*(pdata+3)= 255; //Alpha,��͸
			}
		}
	}
	SetDIBits(hdc, hCustomBmp, 0, HEIGHT, data, &bmpInfo, DIB_RGB_COLORS); //ʹ��ָ����DIB��ɫ����������λͼ�е�����

	BLENDFUNCTION blend = { 0 };
	blend.BlendOp = AC_SRC_OVER;
	blend.SourceConstantAlpha = 255;
	blend.AlphaFormat = AC_SRC_ALPHA;//��ͨ�����
	POINT	pSrc = { 0, 0 };
	SIZE	sizeWnd = { WIDTH, HEIGHT };
	UpdateLayeredWindow(hwnd, hdc, NULL, &sizeWnd, hCompatibleDC, &pSrc, NULL, &blend, ULW_ALPHA);//���·ֲ㴰��

	ReleaseDC(hwnd, hdc);
}

LRESULT CALLBACK WndProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam);
int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR szCmdLine, int
	nCmdShow)
{

	LoadLibrary(L"videoplayercomponent.dll");
	HMODULE h_videoplayercomponent = GetModuleHandle(L"videoplayercomponent.dll");


	typedef int (*Fn_onLoadVideoplayercomponentPtr)(wchar_t const *);
	Fn_onLoadVideoplayercomponentPtr fn_onLoadVideoplayercomponentPtr = (Fn_onLoadVideoplayercomponentPtr)GetProcAddress(h_videoplayercomponent, "fn_onLoadVideoplayercomponent");
	fn_onLoadVideoplayercomponentPtr(L"./");


	typedef PlayerWindowInterface * (*Fn_CreatePlayWindowtPtr)();
	Fn_CreatePlayWindowtPtr fn_CreatePlayWindowtPtr = (Fn_CreatePlayWindowtPtr)GetProcAddress(h_videoplayercomponent, "fn_CreatePlayWindowt");

	PlayerWindowInterface * playwindow = fn_CreatePlayWindowtPtr();
	playwindow->init(NULL,200,200,500,500);
	playwindow->Play("./out.mp4");

	TCHAR		szAppName[] = TEXT("layerwindowTest");
	WNDCLASSEX	wndClass;
	MSG			msg;
	HWND		hwnd;
	wndClass.cbSize = sizeof(WNDCLASSEX);
	wndClass.cbClsExtra = 0;
	wndClass.cbWndExtra = 0;
	wndClass.hbrBackground =  (HBRUSH)GetStockObject(BLACK_BRUSH);
	wndClass.hCursor = LoadCursor(NULL, IDC_ARROW);
	wndClass.hIcon = LoadIcon(NULL, IDI_APPLICATION);
	wndClass.hIconSm = NULL;
	wndClass.hInstance = hInstance;
	wndClass.lpszClassName = TEXT("MyAppTest");
	wndClass.lpszMenuName = NULL;
	wndClass.lpfnWndProc = WndProc; 
	wndClass.style = CS_HREDRAW | CS_VREDRAW | CS_IME | CS_DBLCLKS;


	RegisterClassEx(&wndClass);
	hwnd = CreateWindowEx(WS_EX_LAYERED, TEXT("MyAppTest"), szAppName, WS_CLIPCHILDREN | WS_CLIPSIBLINGS | WS_POPUP/*�ޱ߿���*/
		, 300, 200, WIDTH, HEIGHT,
		NULL, NULL, hInstance, NULL);
	//���ñ�����Ϊ�ֲ㴰��֧��͸��
	//�ֲ㴰��û��WM_PAINT��Ϣ
	ShowWindow(hwnd, SW_SHOW);
	UpdateWindow(hwnd);
	
	while (GetMessage(&msg, NULL, 0, 0))
	{
		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}

	return 0;
}

#define IDT_TIMER 100
void CALLBACK TimerProc(HWND hwnd,UINT uMsg,UINT_PTR idEvent,DWORD dwTime)
{
	render(hwnd);
}
LRESULT CALLBACK WndProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	static BOOL		ldown;
	static POINT	TheFirstPoint;

	switch (msg)
	{
	case WM_CREATE:
		SetTimer(hwnd, IDT_TIMER, 200, (TIMERPROC)TimerProc);
		break;
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
	case WM_LBUTTONDBLCLK:
		DestroyWindow(hwnd);
		releaseResources();
	case WM_DESTROY:
		PostQuitMessage(0);
		break;
	}
	return DefWindowProc(hwnd, msg, wParam, lParam);
}