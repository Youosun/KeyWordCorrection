#include "MyConfig.h"
#include <pthread.h>
#include <stdlib.h>
#include <iostream>
#include <fstream>
#include <sstream>
using std::cout;
using std::endl;
using std::istringstream;
using std::ifstream;

namespace SM
{

MyConfig * MyConfig::_pInstance = NULL;
pthread_once_t MyConfig::_once = PTHREAD_ONCE_INIT;
string MyConfig::_configFilepath = "../config/Configuration";

MyConfig * MyConfig::getInstance()
{
	pthread_once(&_once, init);
	return _pInstance;
}

void MyConfig::init()
{
	_pInstance = new MyConfig;
	atexit(destroy);
}

void MyConfig::destroy()
{
	if(_pInstance)
		delete _pInstance;
}

MyConfig::MyConfig()
{
	initConfig();
}

MyConfig::~MyConfig()
{}

void MyConfig::initConfig()
{
	ifstream ifs(_configFilepath);	
	if(!ifs.good())
	{
		perror("open config file error");
		ifs.close();
		return;
	}

	string line;
	string key, value;
	while(getline(ifs, line))
	{
		istringstream iss(line);			
		iss >> key >> value;
		_conf.insert(std::make_pair(key, value));
	}
}

const string MyConfig::find(const string & item)
{
	map<string, string>::iterator it;
	it = _conf.find(item);
	if(it == _conf.end())
		return std::string();
	else
		return it->second;
}

void MyConfig::showConf()const
{
	for(auto it = _conf.begin(); it != _conf.end(); ++it)
	{
		cout << it->first << " --> " << it->second << endl;
	}
}

void MyConfig::showConfDir()const
{
	cout << _configFilepath << endl;
}

}
