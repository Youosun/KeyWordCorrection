#include "SpellCorrectServer.h"
#include "MyTask.h"
#include "TcpConnection.h"
#include "MyConfig.h"
#include <iostream>
using std::cout;
using std::endl;

namespace SM
{

SpellCorrectServer * SpellCorrectServer::_pInstance = NULL;
pthread_once_t SM::SpellCorrectServer::_once = PTHREAD_ONCE_INIT;

SpellCorrectServer * SpellCorrectServer::getInstance()
{
	pthread_once(&_once, init);
	return _pInstance;
}

void SpellCorrectServer::init()
{
	_pInstance = new SpellCorrectServer;
	atexit(destroy);
}

void SpellCorrectServer::destroy()
{
	if(_pInstance)
		delete _pInstance;
	_pInstance = NULL;
}


SpellCorrectServer::SpellCorrectServer()
: _conf(SM::MyConfig::getInstance())
, _threadpool(getThreadNum(), getTaskQueSize())
, _tcpserver(getIP().c_str(), getPort())
{}

void SpellCorrectServer::onConnection(const TcpConnectionPtr & conn)
{
	cout << conn->toString() << endl;	
	conn->send("hello, welcome to Chat Server.\r\n");
}

void SpellCorrectServer::onMessage(const TcpConnectionPtr & conn)
{
	std::string s(conn->receive());
	cout << s << endl;

	MyTask task(s, conn);
	_pInstance->_threadpool.addTask(std::bind(&MyTask::execute, task));
}

void SpellCorrectServer::onClose(const TcpConnectionPtr & conn)
{
	cout << conn->toString() << " close" << endl;
}

void SpellCorrectServer::setAllFunc()
{
	_tcpserver.setConnectionCallback(&SpellCorrectServer::onConnection);
	_tcpserver.setMessageCallback(&SpellCorrectServer::onMessage);
	_tcpserver.setCloseCallback(&SpellCorrectServer::onClose);
}

void SpellCorrectServer::start()
{
	_threadpool.start();
	setAllFunc();
	_tcpserver.start();
}

void SpellCorrectServer::stop()
{
	_tcpserver.stop();
	_threadpool.stop();
}

SpellCorrectServer::~SpellCorrectServer()
{}

int SpellCorrectServer::getThreadNum()
{
	MyConfig * conf = SM::MyConfig::getInstance();
	return atoi(conf->find("THREADNUM").c_str());
}

int SpellCorrectServer::getTaskQueSize()
{
	MyConfig * conf = SM::MyConfig::getInstance();
	return atoi(conf->find("TASKQUESIZE").c_str());
}

string SpellCorrectServer::getIP()
{
	MyConfig * conf = SM::MyConfig::getInstance();
	return conf->find("IP");
}

unsigned short SpellCorrectServer::getPort()
{
	MyConfig * conf = SM::MyConfig::getInstance();
	return atoi(conf->find("PORT").c_str());	
}

}
