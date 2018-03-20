#ifndef _SM_TCPSERVER_H__
#define _SM_TCPSERVER_H__

#include "InetAddress.h"
#include "Acceptor.h"
#include "EpollPoller.h"

namespace SM
{
class TcpServer
{
public:
	typedef TcpConnection::TcpConnectionCallback TcpServerCallback;

	TcpServer(unsigned short port);
	TcpServer(const char * pIp, unsigned short port);

	void start();
	void stop();

	void setConnectionCallback(TcpServerCallback cb);
	void setMessageCallback(TcpServerCallback cb);
	void setCloseCallback(TcpServerCallback cb);
private:
	Acceptor _acceptor;
	EpollPoller _epollfd;

	TcpServerCallback _onConnectionCb;
	TcpServerCallback _onMessageCb;
	TcpServerCallback _onCloseCb;
};
}

#endif
