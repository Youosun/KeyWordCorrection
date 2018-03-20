#include "EpollPoller.h"
#include "SocketUtil.h"
#include "Acceptor.h"
#include <assert.h>

namespace SM
{
EpollPoller::EpollPoller(Acceptor & acceptor)
: _acceptor(acceptor)
, _epollfd(createEpollFd())
, _listenfd(_acceptor.fd())
, _wakeupfd(createEventFd())
, _isLooping(false)
, _eventlist(1024)
{
	addEpollFdRead(_epollfd, _listenfd);
	addEpollFdRead(_epollfd, _wakeupfd);
}

EpollPoller::~EpollPoller()
{
	::close(_epollfd);
}

void EpollPoller::loop()
{
	_isLooping = true;
	while(_isLooping)
	{
		waitEpollfd();
	}
}

void EpollPoller::unloop()
{
	if(_isLooping)
		_isLooping = false;
}

void EpollPoller::runInLoop(const Functor & cb)
{
	{
		MutexLockGuard mlg(_mutex);
		_pendingFunctors.push_back(cb);
	}
	wakeup();
}

void EpollPoller::doPendingFunctors()
{
	printf("doPendingFunctors()\n");
	std::vector<Functor> functors;
	{
		MutexLockGuard mlg(_mutex);
		functors.swap(_pendingFunctors);
	}

	for(size_t i = 0; i < functors.size(); ++i)
	{
		functors[i]();
	}
}

void EpollPoller::wakeup()
{
	uint64_t one = 1;
	ssize_t n = ::write(_wakeupfd, &one, sizeof(one));
	if(n != sizeof(one))
	{
		perror("EpollPoller::wakeup() n != 8");
	}
}

void EpollPoller::handleRead()
{
	uint64_t one = 1;
	ssize_t n = ::read(_wakeupfd, &one, sizeof(one));
	if(n != sizeof(one))
	{
		perror("EpollPoller::handleRead() n != 8");
	}
	printf("handleRead wakeup\n");
}

void EpollPoller::setConnectionCallback(EpollCallback cb)
{
	_onConnectionCb = cb;
}

void EpollPoller::setMessageCallback(EpollCallback cb)
{
	_onMessageCb = cb;
}

void EpollPoller::setCloseCallback(EpollCallback cb)
{
	_onCloseCb = cb;
}

void EpollPoller::waitEpollfd()
{
	int nready;
	do{
		nready = ::epoll_wait(_epollfd, &(*_eventlist.begin()),
								_eventlist.size(), 5000);
	}while(nready == -1 && errno == EINTR);

	if(nready == -1)
	{
		perror("epoll_wait error");
		exit(EXIT_FAILURE);
	}
	else if(nready == 0)
	{
		printf("epoll_wait timeout\n");
	}
	else
	{
		if(nready == static_cast<int>(_eventlist.size()))
		{
			_eventlist.resize(_eventlist.size() * 2);
		}
		for(int idx = 0; idx != nready; ++idx)
		{
			if(_eventlist[idx].data.fd == _listenfd)
			{
				if(_eventlist[idx].events & EPOLLIN)
				{
					handleConnection();
				}
			}
			else if(_eventlist[idx].data.fd == _wakeupfd)
			{
				printf("wakeup light\n");
				if(_eventlist[idx].events & EPOLLIN)
				{
					handleRead();
					doPendingFunctors();
				}
			}
			else
			{
				if(_eventlist[idx].events & EPOLLIN)
				{
					handleMessage(_eventlist[idx].data.fd);
				}
			}
		}
	}
}

void EpollPoller::handleConnection()
{
	int peerfd = _acceptor.acceptor();
	addEpollFdRead(_epollfd, peerfd);

	TcpConnectionPtr conn(new TcpConnection(peerfd, this));
	conn->setConnectionCallback(_onConnectionCb);
	conn->setMessageCallback(_onMessageCb);
	conn->setCloseCallback(_onCloseCb);

	std::pair<ConnectionMap::iterator, bool> ret;
	ret = _connMap.insert(std::make_pair(peerfd, conn));
	assert(ret.second == true);

	conn->handleConnectionCallback();
}

void EpollPoller::handleMessage(int peerfd)
{
	bool isClosed = isConnectionClosed(peerfd);
	ConnectionMap::iterator it = _connMap.find(peerfd);
	assert(it != _connMap.end());

	if(isClosed)
	{
		it->second->handleCloseCallback();
		delEpollReadFd(_epollfd, peerfd);
		_connMap.erase(it);
	}
	else{
		it->second->handleMessageCallback();
	}
}

}
