#ifndef	FLAYERWINDOWINTERFACE_H
#define FLAYERWINDOWINTERFACE_H
#include <string>
#include <map>
using namespace std;


struct PlayerWindowInterface
{
	virtual bool init(HWND parentHwnd, int x, int y, int w, int h) = 0;
	virtual void setPlayPosition(int x, int y, int w, int h) = 0;
	virtual void setVisible(bool isVisible) = 0;
	virtual bool play(const string & filePath) = 0;
	virtual void stop() = 0;
	virtual int getPlayerWindowID() = 0; //����PlayerWindow����id,����ͨ��
	virtual void setPlayerWindowID(int id) = 0;
	virtual void setTransparent(bool isTransparent) = 0; //���ü���͸��Ĭ�ϴ�͸��
	virtual ~PlayerWindowInterface(){};
};

namespace VideoPlayerWindowComponent{
	const int PLAYERWINDOW_EVENTID_STOP = 1;  //��������
	const int PLAYERWINDOW_EVENTID_CLICK = 2; //�������


//PLAYERWINDOW_EVENTID_CLICK  
//��Ϣ ���ֶκ���
//msgStr:���ص���¼�����x,y������"200,300"


};

struct IPlayerWindowEventHandler
{
	virtual void playerWindowCallback(int playerWindowID, int msgid,const string & msgStr,const map<string,string> &extend) = 0;
};

struct VideoPlayerComponentHelper
{
	virtual PlayerWindowInterface * createPlayWindow() = 0;
	virtual void freePlayWindow(PlayerWindowInterface * w) = 0;

	virtual void regPlayerWindowEvent(int playerWindowID, int msgid, IPlayerWindowEventHandler*) = 0;
	virtual void unregPlayerWindowEvent(int playerWindowID, int msgid, IPlayerWindowEventHandler* eventHandler) = 0;
};

#endif //FLAYERWINDOWINTERFACE_H