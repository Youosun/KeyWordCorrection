#ifndef _SM_TASKQUEUE_H__
#define _SM_TASKQUEUE_H__

#include "MutexLock.h"
#include "Condition.h"

#include <iostream>
#include <queue>
#include <functional>
using std::cout;
using std::endl;
using std::queue;

namespace SM
{
class TaskQueue
{
public:
	typedef std::function<void()> ElemType;

	TaskQueue(size_t size);

	void push(const ElemType & elem);
	ElemType pop();

	bool full()const;
	bool empty()const;

	void wakeup();
private:
	size_t _size;
	queue<ElemType> _que;
	MutexLock _mutex;
	Condition _notFull;
	Condition _notEmpty;
	bool _flag;
};
}//end of namespace SM

#endif
