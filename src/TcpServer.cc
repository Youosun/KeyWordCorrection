#include "TcpServer.h"
#include "SocketUtil.h"

namespace SM
{
TcpServer::TcpServer(unsigned short port)
: _acceptor(createSocketFd(), InetAddress(port))
, _epollfd(_acceptor)
{
	_acceptor.ready();
}

TcpServer::TcpServer(const char * pIp, unsigned short port)
: _acceptor(createSocketFd(), InetAddress(pIp, port))
, _epollfd(_acceptor)
{
	_acceptor.ready();
}

void TcpServer::start()
{
	_epollfd.loop();
}

void TcpServer::stop()
{
	_epollfd.unloop();
}

void TcpServer::setConnectionCallback(TcpServerCallback cb)
{
	_epollfd.setConnectionCallback(cb);
}

void TcpServer::setMessageCallback(TcpServerCallback cb)
{
	_epollfd.setMessageCallback(cb);
}

void TcpServer::setCloseCallback(TcpServerCallback cb)
{
	_epollfd.setCloseCallback(cb);
}

}
