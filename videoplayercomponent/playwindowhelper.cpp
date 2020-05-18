#include "stdafx.h"
#include "playwindowhelper.h"
#include "playerwindow.h"

PlayWindowHelperImple* g_pPlayWindowHelperImple = NULL;

PlayWindowHelperImple::PlayWindowHelperImple()
{

}

void PlayWindowHelperImple::regPlayerWindowEvent(int playerWindowID, int msgid, IPlayerWindowEventHandler* eventHandler)
{

	map<PlayWindowProtocolId, set<IPlayerWindowEventHandler*>>::iterator it = mapMsgHandler.find(PlayWindowProtocolId(playerWindowID, msgid));
	if(it != mapMsgHandler.end())
	{
		set<IPlayerWindowEventHandler*>::iterator itHandler = it->second.find(eventHandler);
		if(itHandler == it->second.end())
		{
			it->second.insert(eventHandler);
		}
	}
	else
	{
		set<IPlayerWindowEventHandler*> setHandler;
		setHandler.insert(eventHandler);
		mapMsgHandler.insert(std::pair<PlayWindowProtocolId, set<IPlayerWindowEventHandler*> >(PlayWindowProtocolId(playerWindowID, msgid), setHandler));
	}
}

void PlayWindowHelperImple::unregPlayerWindowEvent(int playerWindowID, int msgid,IPlayerWindowEventHandler* eventHandler)
{
	map<PlayWindowProtocolId, set<IPlayerWindowEventHandler*>>::iterator it = mapMsgHandler.find(PlayWindowProtocolId(playerWindowID, msgid));
	if(it != mapMsgHandler.end())
	{
		set<IPlayerWindowEventHandler*>::iterator itHandler = it->second.find(eventHandler);
		if(itHandler != it->second.end())
		{
			it->second.erase(itHandler);
		}
	}
}

void PlayWindowHelperImple::sendEvent(int playerWindowID, int msgid,const string & msgStr,const map<string,string> &extend)
{
	map<PlayWindowProtocolId, set<IPlayerWindowEventHandler*>>::iterator it = mapMsgHandler.find(PlayWindowProtocolId(playerWindowID, msgid));
	if(it != mapMsgHandler.end())
	{
		set<IPlayerWindowEventHandler*>::iterator itHandler = it->second.begin();
		while(itHandler != it->second.end())
		{
			set<IPlayerWindowEventHandler*>::iterator itHandle2 = itHandler ;
			itHandler++; //先递增，指向下一节点，以防回调中处理，导致当前迭代器失效无法自增。
			(*itHandle2)->playerWindowCallback(playerWindowID,msgid,msgStr, extend);
		}
	}
}

void PlayWindowHelperImple::createInstance()
{
	g_pPlayWindowHelperImple = new PlayWindowHelperImple();
}

void PlayWindowHelperImple::desctroyInstance()
{
	if(g_pPlayWindowHelperImple)
	{
		delete g_pPlayWindowHelperImple;
		g_pPlayWindowHelperImple = NULL;
	}
}

PlayWindowHelperImple * PlayWindowHelperImple::getInstance()
{
	return g_pPlayWindowHelperImple;
}

PlayWindowHelperImple::~PlayWindowHelperImple()
{
	mapMsgHandler.clear();
}

PlayerWindowInterface * PlayWindowHelperImple::createPlayWindow()
{
	return new PlayerWindow();
}

void PlayWindowHelperImple::freePlayWindow(PlayerWindowInterface * w)
{
	if(w)
	{
		delete(w);
	}
}
