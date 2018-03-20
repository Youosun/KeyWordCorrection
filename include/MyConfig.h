#ifndef _SM_MYCONFIG_H__
#define _SM_MYCONFIG_H__

#include <pthread.h>

#include <string>
#include <map>
using std::map;
using std::string;

#define DEBUG

namespace SM
{
class MyConfig
{
public:
	static MyConfig * getInstance();

	static void init();

	static void destroy();

	const string find(const string & item);

#ifdef DEBUG
	void showConf()const;
	void showConfDir()const;
#endif

private:
	MyConfig();
	~MyConfig();

	void initConfig();
private:
	static MyConfig * _pInstance;
	static pthread_once_t _once;
	static string _configFilepath;
	map<string, string> _conf;
};

}
#endif
