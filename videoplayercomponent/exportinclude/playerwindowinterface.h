#ifndef	FLAYERWINDOWINTERFACE_H
#define FLAYERWINDOWINTERFACE_H
#include <string>
using namespace std;

struct PlayerWindowInterface
{
	virtual bool init(HWND parentHwnd, int x, int y, int w, int h) = 0;
	virtual void setPlayPosition(int x, int y, int w, int h) = 0;
	virtual bool Play(const string & filePath) = 0;
	virtual void stop() = 0;
	virtual ~PlayerWindowInterface(){};
};


#endif //FLAYERWINDOWINTERFACE_H