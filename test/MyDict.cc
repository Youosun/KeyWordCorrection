#include "MyDict.h"
#include "Configuration.h"
#include <sys/types.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <dirent.h>

#include <iostream>
#include <fstream>
#include <sstream>
using std::ofstream;
using std::ifstream;
using std::endl;
using std::istringstream;

namespace SM
{
MyDict::MyDict(Configuration & config)
: _config(config)
{
	getYuliaoFiles();
}

void MyDict::getYuliaoFiles()
{
	const string dir = getYuliaoDir();

	DIR * d;
	d = opendir(dir.c_str());
	if(NULL == d)
	{
		perror("opendir failed\n");
		return;
	}
	
	struct dirent *p;
	while((p=readdir(d)) != NULL)
	{
		if(p->d_type == 8)
			_yuliaoFiles.push_back(string(p->d_name));	
	}
	closedir(d);
}

const string MyDict::getDictDir()
{
	return _config.find("DICTDIR");
}

const string MyDict::getYuliaoDir()
{
	return _config.find("YULIAODIR");
}

string strtolower(string && word)
{
	for(size_t idx = 0; idx != word.size(); ++idx)
	{
		word[idx] = tolower(word[idx]);
	}
	return word;
}

void MyDict::addDictMap(const string & yuliaoname, map<string, int> & dict)
{
	const string delim = Configuration::DELIM;
	const string yuliaodir = _config.find("YULIAODIR");
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
		perror("stat error");
		return;
	}
	file = (char*)malloc(sb.st_size);
	if(NULL == file)
		return;

	fread(file, sizeof(char), sb.st_size, fp);

	word = strtok(file, delim.c_str());

	while(word != NULL)
	{
		if(strlen(word) < 1)
		{
			word = strtok(NULL, delim.c_str());
			continue;
		}

		string str = strtolower(string(word));
		dict[str]++;
		word = strtok(NULL, delim.c_str());
	}
}

void MyDict::buildDict()
{
	map<string, int> dict;	
	for(size_t idx = 0; idx != _yuliaoFiles.size(); ++idx)
	{
		addDictMap(_yuliaoFiles[idx], dict);
	}

	for(auto it = dict.begin(); it != dict.end(); ++it)
	{
		_dict.push_back(*it);
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

void MyDict::storeDict()
{
	string dictdir = getDictDir();	
	string dictname = _config.find("DICTNAME");
	ofstream ofs(dictdir + dictname);
	if(!ofs.good())
	{
		perror("create Dictfile error");
		ofs.close();
		return;
	}
	for(size_t idx = 0; idx != _dict.size(); ++idx)
	{
		ofs << _dict[idx].first << "  " << _dict[idx].second << endl;
	}
}

void MyDict::storeIndex()
{
	string indexdir = getDictDir();
	string indexname = _config.find("INDEXNAME");
	ofstream ofs(indexdir + indexname);
	if(!ofs.good())
	{
		perror("create index file error");
		ofs.close();
		return;
	}
	for(auto it = _index.begin(); it != _index.end(); ++it)
	{
		ofs << it->first << endl;
		for(auto ie = it->second.begin(); ie != it->second.end(); ++ie)
		{
			ofs << _dict[*ie].first << " " << _dict[*ie].second << endl;
		}
	}
}

void MyDict::readDict()
{
	string dictdir = getDictDir();
	string dictname = _config.find("DICTNAME");
	ifstream ifs(dictdir + dictname);
	if(!ifs.good())
	{
		perror("open dict file error");
		ifs.close();
		return;
	}
	string line;
	string word;
	int frequency;
	while(getline(ifs, line))
	{
		istringstream iss(line);			
		iss >> word >> frequency;
		_dict.push_back(std::make_pair(word, frequency));
	}
}

}
