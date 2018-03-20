#include "Threadpool.h"
#include "Thread.h"

#include <unistd.h>

namespace SM
{
Threadpool::Threadpool(size_t threadnum, size_t quesize)
: _threadNum(threadnum)
, _queSize(quesize)
, _taskQue(_queSize)
, _isExit(false)
{
	_threads.reserve(_threadNum);
}
	
Threadpool::~Threadpool()
{
	if(!_isExit)
	{
		stop();
	}
}

void Threadpool::start()
{
	for(size_t idx = 0; idx != _threadNum; ++idx)
	{
		shared_ptr<Thread> sp(new Thread(std::bind(&Threadpool::threadFunc, this)));
		_threads.push_back(sp);
	}

	for(auto & thread : _threads)
	{
		thread->start();
		cout << "thread started" << endl;
	}
}

void Threadpool::stop()
{
	if(!_isExit)
	{
		while(!_taskQue.empty())
		{
			::sleep(2); //等待队列中剩余的任务执行完毕
		}
		_isExit = true;
		_taskQue.wakeup();

		for(auto & thread : _threads)
		{
			thread->join();
		}
	}
}

void Threadpool::addTask(Task && task)
{
	_taskQue.push(std::move(task));
}

Task Threadpool::getTask()
{
	return _taskQue.pop();
}

void Threadpool::threadFunc()
{
	while(!_isExit)
	{
		Task task = getTask();
		if(task)
			task();
	}
}

}//end of namespace SM
