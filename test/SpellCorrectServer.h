#ifndef _SM_SPELLCORRECTSERVER_H__
#define _SM_SPELLCORRECTSERVER_H__

#include "Threadpool.h"
#include "TcpServer.h"
#include "Configuration.h"

namespace SM
{
class SpellCorrectServer
{
public:
	SpellCorrectServer(Configuration & config);
	
	void start();
	void stop();

	void onConnection(const SM::TcpConnectionPtr & conn);
	void onMessage(const SM::TcpConnectionPtr & conn);
	void onClose(const SM::TcpConnectionPtr & conn)
private:
	Threadpool _threadpool;
	TcpServer _tcpserver;
	Configuration & _config;
	MyTask & _task;
};
}


#endif
