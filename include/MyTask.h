#ifndef _SM_MYTASK_H__
#define _SM_MYTASK_H__

#include "TcpConnection.h"
#include "MyResult.h"
#include <string>
#include <queue>
#include <vector>
#include <set>
using std::priority_queue;
using std::string;
using std::vector;
using std::set;

#define DEBUG

namespace SM
{

struct MyCompare
{
	bool operator()(const MyResult & lhs, const MyResult & rhs)
	{
		if(lhs._iDist < rhs._iDist)
			return false;
		else if(lhs._iDist > rhs._iDist)
			return true;
		else if(lhs._iFeq < rhs._iFeq)
			return true;
		else 
			return false;
	}
};

class MyTask
{
public:
	MyTask(const string & query,const SM::TcpConnectionPtr & conn);
//	MyTask(const string & query);
	~MyTask();

#ifdef DEBUG
	void showPrioQue();
#endif

	void execute();
private:
	void queryIndexTable();
	void statistics(int idx, set<int> &iset);
	int distance(const string & rhs);
	void response();

private:
	string _query;
	TcpConnectionPtr _conn;
	priority_queue<MyResult, vector<MyResult>, MyCompare> _resultQue;
};

}

#endif
