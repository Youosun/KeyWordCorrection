#ifndef _SM_MYDICT_H__
#define _SM_MYDICT_H__

#include <pthread.h>

#include <map>
#include <string>
#include <set>
#include <vector>
using std::map;
using std::string;
using std::set;
using std::vector;
using std::pair;

#define DEBUG

namespace SM
{

class MyDict
{
public:
	static MyDict * getInstance();
	static void init();
	static void destroy();

	vector<pair<string, int>> & getDict();
	map<string, set<int>> & getIndex();

#ifdef DEBUG
	void showDict();
	void showIndex();
#endif

private:
	MyDict();
	~MyDict();

	void buildDict();
	void storeDict();
	void buildIndex();

	void addDict(const string & filename, map<string, int> & dict);

private:
	static MyDict * _pInstance;
	static pthread_once_t _once;
	static const string delim;

	vector<pair<string, int>> _dict;
	map<string, set<int>> _index;
};

}

#endif
