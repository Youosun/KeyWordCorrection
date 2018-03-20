#include "MyDict.h"
#include "MyConfig.h"
#include <sys/types.h>
#include <sys/stat.h>
#include <string.h>
#include <stdlib.h>
#include <dirent.h>
#include <stdio.h>

#include <iostream>
#include <fstream>
#include <sstream>
using std::ofstream;
using std::ifstream;
using std::endl;
using std::cout;
using std::istringstream;

namespace SM
{

MyDict * MyDict::_pInstance = NULL;
pthread_once_t MyDict::_once = PTHREAD_ONCE_INIT;
const string MyDict::delim = ".,:;`'/\"+-_(){}[]<>*&^%$#@!?~/|\\=1234567890 \t\n";

MyDict * MyDict::getInstance()
{
	pthread_once(&_once, init);
	return _pInstance;
}

void MyDict::init()
{
	_pInstance = new MyDict;
	atexit(destroy);
}

void MyDict::destroy()
{
	if(_pInstance)
		delete _pInstance;
}

vector<pair<string, int>> & MyDict::getDict()
{
	return _dict;
}

map<string, set<int>> & MyDict::getIndex()
{
	return _index;
}

void MyDict::showDict()
{
	for(size_t idx = 0; idx != _dict.size(); ++idx)
	{
		cout << _dict[idx].first << " " << _dict[idx].second << endl;
	}
}

void MyDict::showIndex()
{
	for(auto it = _index.begin(); it != _index.end(); ++it)
	{
		cout << it->first << endl;
		for(auto ie = it->second.begin(); ie != it->second.end(); ++ie)
		{
			cout << _dict[*ie].first << endl;
		}
	}
}

MyDict::MyDict()
{
	buildDict();
	storeDict();
	buildIndex();
}

MyDict::~MyDict()
{}

vector<string> getYuliaoFiles(const string & dir)
{
	vector<string> res;
	DIR * d = opendir(dir.c_str());	
	if(NULL == d)
	{
		perror("open yuliaoDir failed\n");
		return vector<string>();
	}
	struct dirent * p;
	while((p = readdir(d)) != NULL)
	{
		if(p->d_type == 8)
			res.push_back(string(p->d_name));
	}
	return res;
}

string strtolower(string && word)
{
	for(size_t idx = 0; idx != word.size(); ++idx)
	{
		word[idx] = tolower(word[idx]);
	}
	return word;
}

void MyDict::addDict(const string & yuliaoname, map<string, int> & temp)
{
	SM::MyConfig * conf = SM::MyConfig::getInstance();
	const string yuliaodir = conf->find("YULIAODIR");
	const string filename = yuliaodir + yuliaoname;

	char * word;
	char * file;
	struct stat sb;

	FILE * fp = fopen(filename.c_str(), "r");
	if(NULL == fp)
	{
		perror("fopen error");
		return;
	}

	if(stat(filename.c_str(), &sb))
	{
		perror("stat failed\n");	
	}

	file = (char *)malloc(sb.st_size);
	if(NULL == file)
	{
		return;
	}

	fread(file, sizeof(char), sb.st_size, fp);

	word = strtok(file, MyDict::delim.c_str());

	while(word != NULL)
	{
		if(strlen(word) < 1)
		{
			word = strtok(NULL, MyDict::delim.c_str());
			continue;
		}
		string str = strtolower(string(word));
		temp[str]++;
		word = strtok(NULL, MyDict::delim.c_str());
	}
	free(file);
}

void MyDict::buildDict()
{
	SM::MyConfig * conf = SM::MyConfig::getInstance();	
	string yuliaoDir = conf->find("YULIAODIR");
	vector<string> files = getYuliaoFiles(yuliaoDir);
	
	map<string, int> tempDict;
	for(size_t idx = 0; idx != files.size(); ++idx)
	{
		addDict(files[idx], tempDict);
	}
	for(auto it = tempDict.begin(); it != tempDict.end(); ++it)
	{
		_dict.push_back(*it);
	}
}

void MyDict::storeDict()
{
	SM::MyConfig * conf = SM::MyConfig::getInstance();
	string dictFilePath = conf->find("DICTDIR") + conf->find("DICTNAME");
	
	ofstream ofs(dictFilePath);
	if(!ofs.good())
	{
		perror("create dict file failed\n");
		return;
	}

	for(size_t idx = 0; idx != _dict.size(); ++idx)
	{
		ofs	<< _dict[idx].first << " " << _dict[idx].second << endl;
	}
}

void MyDict::buildIndex()
{
	string word;
	size_t idx;
	for(idx = 0; idx != _dict.size(); ++idx)
	{
		word = _dict[idx].first;
		for(size_t i = 0; i != word.size(); ++i)
		{
			string ind = {word[i]};
			_index[ind].insert(static_cast<int>(idx));
		}
	}
}

}
