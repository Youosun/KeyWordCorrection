#ifndef _SM_CONFIGURATION_H__
#define _SM_CONFIGURATION_H__

#include <string>
#include <map>
#include <memory>
using std::shared_ptr;
using std::string;
using std::map;

namespace SM
{

class Configuration
{
public:
	Configuration(const string & filepath);
	map<string, string> & getConfigMap();

	const string find(const char * item);

	static const string DELIM;

private:
	map<string, string> readConfig(const string & filepath);
private:
	string _filepath;	
	map<string, string> _configMap;
};

}//end of namespace SM
#endif
