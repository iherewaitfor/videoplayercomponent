// ���� ifdef ���Ǵ���ʹ�� DLL �������򵥵�
// ��ı�׼�������� DLL �е������ļ��������������϶���� VIDEOPLAYERCOMPONENT_EXPORTS
// ���ű���ġ���ʹ�ô� DLL ��
// �κ�������Ŀ�ϲ�Ӧ����˷��š�������Դ�ļ��а������ļ����κ�������Ŀ���Ὣ
// VIDEOPLAYERCOMPONENT_API ������Ϊ�Ǵ� DLL ����ģ����� DLL ���ô˺궨���
// ������Ϊ�Ǳ������ġ�
#ifdef VIDEOPLAYERCOMPONENT_EXPORTS
#define VIDEOPLAYERCOMPONENT_API __declspec(dllexport)
#else
#define VIDEOPLAYERCOMPONENT_API __declspec(dllimport)
#endif
#include <string>
#include "exportinclude/playerwindowinterface.h"
using namespace std;
// �����Ǵ� videoplayercomponent.dll ������
class VIDEOPLAYERCOMPONENT_API Cvideoplayercomponent {
public:
	Cvideoplayercomponent(void);
	// TODO: �ڴ�������ķ�����
};

extern VIDEOPLAYERCOMPONENT_API int nvideoplayercomponent;

VIDEOPLAYERCOMPONENT_API int fnvideoplayercomponent(void);


extern "C" VIDEOPLAYERCOMPONENT_API int fn_onLoadVideoplayercomponent(LPCWSTR filePath);

extern "C" VIDEOPLAYERCOMPONENT_API PlayerWindowInterface * fn_CreatePlayWindowt();

VIDEOPLAYERCOMPONENT_API void fn_unLoadVideoplayercomponent();
