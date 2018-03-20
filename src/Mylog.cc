#define LOG4CPP_FIX_ERROR_COLLISION 1

#include "Mylog.h"

namespace SM
{
Mylog * Mylog::_mlog = NULL;
Mylog::GC Mylog::gc;

Mylog * Mylog::getInstance()
{
	if(_mlog == NULL)
		_mlog = new Mylog();
	return _mlog;
}

void Mylog::log_err(const char * msg)
{
	ostringstream oss;
	oss << msg << "[" << __FILE__ << "]"
			   << "[" << __LINE__ << "]";
	_root.error(oss.str());
}

void Mylog::log_fatal(const char * msg)
{
	ostringstream oss;
	oss << msg << "[" << __FILE__ << "]"
			   << "[" << __LINE__ << "]";
	_root.fatal(oss.str());	
}

void Mylog::log_warn(const char * msg)
{
	ostringstream oss;
	oss << msg << "[" << __FILE__ << "]"
			   << "[" << __LINE__ << "]";
	_root.warn(oss.str());	
}

void Mylog::log_debug(const char * msg)
{
	ostringstream oss;
	oss << msg << "[" << __FILE__ << "]"
			   << "[" << __LINE__ << "]";
	_root.debug(oss.str());	
}

void Mylog::log_info(const char * msg)
{
	ostringstream oss;
	oss << msg << "[" << __FILE__ << "]"
			   << "[" << __LINE__ << "]";
	_root.info(oss.str());	
}

void Mylog::destroy()
{
	_mlog->log_info("DESTRUY");
	_mlog->_root.shutdown();
}

Mylog::Mylog()
: _root(log4cpp::Category::getRoot())
{
	log4cpp::PatternLayout *plyout1 = new log4cpp::PatternLayout();
	log4cpp::PatternLayout *plyout2 = new log4cpp::PatternLayout();
	plyout1->setConversionPattern("%d:[%7p] %m %n");
	plyout2->setConversionPattern("%d:[%7p] %m %n");

	log4cpp::RollingFileAppender * rfAppender = new log4cpp::RollingFileAppender("rfAppender", "my.log", 5 * 1024, 1);
	rfAppender->setLayout(plyout1);

	log4cpp::OstreamAppender * osAppender = new log4cpp::OstreamAppender("osAppender", &cout);
	osAppender->setLayout(plyout2);

	_root.setPriority(log4cpp::Priority::DEBUG);
	_root.addAppender(rfAppender);
	_root.addAppender(osAppender);
}

}
