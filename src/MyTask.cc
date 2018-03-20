#include "MyTask.h"
#include "MyDict.h"
#include "MyResult.h"
#include "MyConfig.h"
#include "TcpConnection.h"
#include <stdlib.h>

#include <iostream>
#include <queue>
using std::priority_queue;
using std::cout;
using std::endl;

namespace SM
{

MyTask::MyTask(const string & query, const SM::TcpConnectionPtr & conn)
: _query(query)
, _conn(conn)
{}

MyTask::~MyTask()
{}

void MyTask::execute()
{
	//通过indextable与最小距离算法计算并生成优先队列
//	_conn->sendInLoop();//多次sendInLoop  注册多个函数
	queryIndexTable();
	response();
}

void MyTask::showPrioQue()
{
	MyResult temp;
	while(!_resultQue.empty())
	{
		temp = _resultQue.top();
		_resultQue.pop();
		cout << temp._word << " " << temp._iFeq << endl;
	}
}

void MyTask::statistics(int idx, set<int> & iset)
{
	map<string, set<int>> index = SM::MyDict::getInstance()->getIndex();
	for(auto it = index.begin(); it != index.end(); ++it)
	{
		if(string{_query[idx]} == it->first)
		{
			for(auto ie = it->second.begin(); ie != it->second.end(); ++ie)
			{
				iset.insert(*ie);
			}
		}
	}
}

void MyTask::queryIndexTable()
{
	MyDict * mydict = SM::MyDict::getInstance();
	vector<pair<string, int>> dict = mydict->getDict();
	set<int> res;
	for(size_t idx = 0; idx != _query.size(); ++idx)
	{
		statistics(idx, res);
	}
	
	for(auto it = res.begin(); it != res.end(); ++it)
	{
		struct MyResult myresult;
		myresult._word = dict[*it].first;
		myresult._iFeq = dict[*it].second;
		myresult._iDist = distance(dict[*it].first);

		_resultQue.push(myresult);
	}
}

int min(int a, int b)
{
	return a < b ? a : b;
}

int MyTask::distance(const string & rhs)
{
	int max1 = _query.size();
	int max2 = rhs.size();

	int **ptr = new int*[max1 + 1];
	for(int i = 0; i < max1 + 1; ++i)
	{
		ptr[i] = new int[max2 + 1];
	}

	for(int i = 0; i < max1 + 1; ++i)
	{
		ptr[i][0] = i;
	}

	for(int i = 0; i < max2 + 1; ++i)
	{
		ptr[0][i] = i;
	}

	for(int i = 1; i < max1 + 1; ++i)
	{
		for(int j = 1; j < max2 + 1; ++j)
		{
			int d;
			int temp = min(ptr[i-1][j] + 1, ptr[i][j-1] + 1);
			if(_query[i-1] == rhs[j-1])
			{
				d = 0;
			}
			else
			{
				d = 1;
			}
			ptr[i][j] = min(temp, ptr[i-1][j-1] + d);
		}
	}
	int ret = ptr[max1][max2];

	for(int i = 0; i < max1 + 1; ++i)
	{
		delete [] ptr[i];
		ptr[i] = NULL;
	}
	delete [] ptr;
	ptr = NULL;

	return ret;
}

void MyTask::response()
{
	MyConfig * conf = SM::MyConfig::getInstance();
	int candidateNum = atoi(conf->find("CANDIDATENUM").c_str());

	string res;
	for(int i = 0; i < candidateNum; ++i)
	{
		res = res + _resultQue.top()._word + " ";
		_resultQue.pop();
	}
	res += "\n";

	_conn->sendInLoop(res);
}

}
