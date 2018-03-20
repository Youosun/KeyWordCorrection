#ifndef _SM_MYLOG_H__
#define _SM_MYLOG_H__

#include <iostream>
#include <log4cpp/Priority.hh>
#include <log4cpp/Category.hh>
#include <log4cpp/OstreamAppender.hh>
#include <log4cpp/RollingFileAppender.hh>
#include <log4cpp/PatternLayout.hh>

using std::cout;
using std::ostringstream;

namespace SM
{
class GC;

class Mylog
{
public:
	static Mylog * getInstance();

	void log_err(const char * msg);
	void log_fatal(const char * msg);
	void log_warn(const char * msg);
	void log_debug(const char * msg);
	void log_info(const char * msg);

	static void destroy();
private:
	Mylog();

private:
	static Mylog * _mlog;
	log4cpp::Category & _root;

	class GC
	{
	public:
		GC(){}
		~GC()
		{
			if(Mylog::_mlog != NULL)
			{
				delete Mylog::_mlog;
				Mylog::_mlog = NULL;
			}	
		}
	};

	static GC gc;
};
}

#endif
