#include "Threadpool.h"
#include "MyTask.h"

int main(void)
{
	SM::Threadpool threadpool(4, 10);
	threadpool.start();

	int cnt = 20;
	while(cnt--)
	{
		threadpool.addTask(std::bind(&MyTask::process, MyTask()));
		cout << "cnt = " << cnt << endl;
	}
	threadpool.stop();

	return 0;
}
