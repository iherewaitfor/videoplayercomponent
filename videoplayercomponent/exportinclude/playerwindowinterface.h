#ifndef	FLAYERWINDOWINTERFACE_H
#define FLAYERWINDOWINTERFACE_H
#include <string>
#include <map>
using namespace std;


struct PlayerWindowInterface
{
	virtual bool init(HWND parentHwnd, int x, int y, int w, int h) = 0;
	virtual void setPlayPosition(int x, int y, int w, int h) = 0;
	virtual bool Play(const string & filePath) = 0;
	virtual void stop() = 0;
	virtual int getPlayerWindowID() = 0; //返回PlayerWindow对象id,用于通信
	virtual ~PlayerWindowInterface(){};
};

namespace VideoPlayerWindowComponent{
	const int PLAYERWINDOW_EVENTID_STOP = 0;
};

struct IPlayerWindowEventHandler
{
	virtual void playerWindowCallback(int playerWindowID, int msgid,const string & msgStr,const map<string,string> &extend) = 0;
};

struct VideoPlayerComponentHelper
{
	virtual void regPlayerWindowEvent(int playerWindowID, int msgid, IPlayerWindowEventHandler*) = 0;
	virtual void unregPlayerWindowEvent(int playerWindowID, int msgid, IPlayerWindowEventHandler* eventHandler) = 0;
};

#endif //FLAYERWINDOWINTERFACE_H