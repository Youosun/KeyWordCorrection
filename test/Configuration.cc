#include "Configuration.h"
#include <fstream>
#include <sstream>
#include <iostream>
using std::cout;
using std::endl;
using std::ifstream;
using std::istringstream;

namespace SM
{
Configuration::Configuration(const string & filepath)
: _filepath(filepath)
, _configMap(readConfig(_filepath))
{
}

map<string, string> Configuration::readConfig(const string & filepath)
{
	map<string, string> res;
	ifstream ifs(filepath);
	if(!ifs.good())
	{
		cout << "open configfile error" << endl;
		ifs.close();
		return res;
	}
	string line;
	string key, value;
	while(getline(ifs, line))
	{
		istringstream iss(line);
		iss >> key >> value; 
		res.insert(std::make_pair(key, value));
	}
	return res;	
}

map<string, string> & Configuration::getConfigMap()
{
	return _configMap;
}

const string Configuration::find(const char * item)
{
	map<string, string>::iterator it;
	it = _configMap.find(string(item));
	if(it == _configMap.end())
		return std::string();
	else 
		return it->second;
}

const string Configuration::DELIM = ".,:;`'/\"+-_(){}[]<>*&^%$#@!?~/|\\=1234567890 \t\n";

} //end of namespace SM
