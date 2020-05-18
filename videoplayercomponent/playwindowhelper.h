
#include "stdafx.h"
#ifndef	PLAYWINDOWHELPER_H
#define PLAYWINDOWHELPER_H
#include "exportinclude/playerwindowinterface.h"
#include <map>
#include <set>
using namespace std;

struct PlayWindowProtocolId
{
	int windowId;
	int msgId;
	PlayWindowProtocolId(int windowId, int msgId):windowId(windowId),msgId(msgId){}
	bool operator = (const PlayWindowProtocolId &y) const
	{
		return   (windowId == y.windowId) && (msgId == y.msgId); 
	}

	bool operator < (const PlayWindowProtocolId &y) const
	{
		if(windowId < y.windowId)
		{
			return true;
		} else if( windowId == y.windowId)
		{
			return msgId < y.msgId;
		}
		return false;
	}

	bool operator > (const PlayWindowProtocolId &y) const
	{
		if(windowId > y.windowId)
		{
			return true;
		} else if( windowId == y.windowId)
		{
			return msgId > y.msgId;
		}
		return false;
	}


};
class PlayWindowHelperImple: public VideoPlayerComponentHelper
{
public:
	PlayerWindowInterface * createPlayWindow();
	void freePlayWindow(PlayerWindowInterface * w);
	void regPlayerWindowEvent(int playerWindowID, int msgid, IPlayerWindowEventHandler*eventHandler);
	void unregPlayerWindowEvent(int playerWindowID, int msgid,IPlayerWindowEventHandler* eventHandler);

	void sendEvent(int playerWindowID, int msgid,const string & msgStr,const map<string,string> &extend);


	static void createInstance();
	static void desctroyInstance();
	static PlayWindowHelperImple * getInstance();
	~PlayWindowHelperImple();
private:
	PlayWindowHelperImple();
	map<PlayWindowProtocolId, set<IPlayerWindowEventHandler*>> mapMsgHandler;
};

#endif //PLAYWINDOWHELPER_H