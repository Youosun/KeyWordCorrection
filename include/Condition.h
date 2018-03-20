#ifndef _SM_CONDITION_H__
#define _SM_CONDITION_H__

#include "Noncopyable.h"
#include <pthread.h>

namespace SM
{
class MutexLock;

class Condition
: Noncopyable
{
public:
	explicit
	Condition(MutexLock & mutex);

	~Condition();

	void wait();
	void notify();
	void notifyall();
private:
	pthread_cond_t _cond;
	MutexLock & _mutex;
};
}//end of namespace SM

#endif
