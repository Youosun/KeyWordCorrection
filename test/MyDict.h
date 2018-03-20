#ifndef _SM_MYDICT_H__
#define _SM_MYDICT_H__

#include "Configuration.h"
#include <map>
#include <set>
#include <string>
#include <vector>
#include <iostream>
using std::cout;
using std::endl;
using std::map;
using std::set;
using std::string;
using std::pair;
using std::vector;

namespace SM
{

class MyDict
{
public:
	MyDict(Configuration & config);


	void buildDict();;
	void buildIndex();

	void storeDict();
	void storeIndex();

	void readDict();

	void showYuliaoFiles()
	{
		for(size_t idx = 0; idx != _yuliaoFiles.size(); ++idx)
		{
			cout << _yuliaoFiles[idx] << endl;
		}
	}

	void showIndex()
	{
		for(auto it = _index.begin(); it != _index.end(); ++it)
		{
			cout << it->first << endl;
			for(auto ie = it->second.begin(); ie != it->second.end(); ++ie)
			{
				cout << *ie << endl;
			}
		}
	}

private:
	void getYuliaoFiles();
	void addDictMap(const string & yuliaoItem, map<string, int> & dict);

	const string getDictDir();
	const string getYuliaoDir();


private:
	vector<string> _yuliaoFiles;
	string _dictDir;
	string _yuliaoDir;
	vector<pair<string, int>> _dict;
	map<string, set<int> > _index;

	Configuration & _config;
};
}

#endif
