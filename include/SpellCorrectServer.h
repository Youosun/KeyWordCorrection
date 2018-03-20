#ifndef _SM_SPELLCORRECTSERVER_H__
#define _SM_SPELLCORRECTSERVER_H__

#include "TcpConnection.h"
#include "TcpServer.h"
#include "MyConfig.h"
#include "MyTask.h"
#include "Threadpool.h"

#include <pthread.h>

namespace SM
{
class SpellCorrectServer
{
public:
	static SpellCorrectServer * getInstance();
	static void init();
	static void destroy();

	void start();
	void stop();

private:

	SpellCorrectServer();
	~SpellCorrectServer();

	static void onConnection(const TcpConnectionPtr & conn);
	static void onMessage(const TcpConnectionPtr & conn);
	static void onClose(const TcpConnectionPtr & conn);

	void setAllFunc();
	int getThreadNum();
	int getTaskQueSize();
	string getIP();
	unsigned short getPort();

private:
	static SpellCorrectServer * _pInstance;
	static pthread_once_t _once;
	MyConfig * _conf;
	Threadpool _threadpool;
	TcpServer _tcpserver;
};

}

#endif
