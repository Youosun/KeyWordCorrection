#include "Configuration.h"
#include <map>
#include <string>
#include <iostream>
using std::map;
using std::endl;
using std::cout;
using std::string;

int main(void)
{
	SM::Configuration config("/home/sbon/test/ArticalSimilarity/stage1/offline/config/Configuration");
	map<string, string> res;
	res = config.getConfigMap();
	for(auto it = res.begin(); it != res.end(); ++it)
	{
		cout << it->first << " --> " << it->second << endl;
	}
	string ip = "not";
	map<string, string>::iterator it = res.find(ip);
	if(it == res.end())
	{
		cout << "NOT FOUND" << endl;
		return -1;
	}
	cout << it->second << endl;
	return 0;
}
