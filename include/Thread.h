#ifndef _SM_THREAD_H__
#define _SM_THREAD_H__

#include "Noncopyable.h"
#include <pthread.h>
#include <functional>

namespace SM
{
class Thread
: Noncopyable
{
public:
	typedef std::function<void()> ThreadCallback;

	Thread(ThreadCallback && cb);
	~Thread();

	void start();
	void join();
private:
	static void * threadFunc(void *);
private:
	pthread_t _pthId;
	bool _isRunning;
	ThreadCallback _cb;
};

}//end of namespace SM

#endif
