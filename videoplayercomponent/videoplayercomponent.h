// 下列 ifdef 块是创建使从 DLL 导出更简单的
// 宏的标准方法。此 DLL 中的所有文件都是用命令行上定义的 VIDEOPLAYERCOMPONENT_EXPORTS
// 符号编译的。在使用此 DLL 的
// 任何其他项目上不应定义此符号。这样，源文件中包含此文件的任何其他项目都会将
// VIDEOPLAYERCOMPONENT_API 函数视为是从 DLL 导入的，而此 DLL 则将用此宏定义的
// 符号视为是被导出的。
#ifdef VIDEOPLAYERCOMPONENT_EXPORTS
#define VIDEOPLAYERCOMPONENT_API __declspec(dllexport)
#else
#define VIDEOPLAYERCOMPONENT_API __declspec(dllimport)
#endif
#include <string>
#include "exportinclude/playerwindowinterface.h"
using namespace std;
// 此类是从 videoplayercomponent.dll 导出的
class VIDEOPLAYERCOMPONENT_API Cvideoplayercomponent {
public:
	Cvideoplayercomponent(void);
	// TODO: 在此添加您的方法。
};

extern VIDEOPLAYERCOMPONENT_API int nvideoplayercomponent;

VIDEOPLAYERCOMPONENT_API int fnvideoplayercomponent(void);


extern "C" VIDEOPLAYERCOMPONENT_API int fn_onLoadVideoplayercomponent(LPCWSTR filePath);

extern "C" VIDEOPLAYERCOMPONENT_API PlayerWindowInterface * fn_CreatePlayWindowt();

VIDEOPLAYERCOMPONENT_API void fn_unLoadVideoplayercomponent();
