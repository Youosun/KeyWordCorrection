#ifndef _SM_THREADPOOL_H__
#define _SM_THREADPOOL_H__

#include "TaskQueue.h"

#include <vector>
#include <memory>
#include <functional>

using std::vector;
using std::shared_ptr;

namespace SM
{
typedef std::function<void()> Task;
class Thread;

class Threadpool
{
public:
	Threadpool(size_t threadnum, size_t quesize);
	~Threadpool();

	void start();
	void stop();
	void addTask(Task && task);

private:
	void threadFunc();
	Task getTask();

private:
	size_t _threadNum;
	size_t _queSize;
	vector<shared_ptr<Thread>> _threads;
	TaskQueue _taskQue;
	bool _isExit;
};
}// end of namespace SM

#endif
