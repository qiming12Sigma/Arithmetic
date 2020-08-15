#include "log.h"

#include <iomanip>
#include <mutex>
#include <log4cplus/logger.h>
#include <log4cplus/log4cplus.h>
#include <log4cplus/loggingmacros.h>
#include <log4cplus/configurator.h>
#include <log4cplus/initializer.h>

using namespace log4cplus;
using namespace log4cplus::helpers;

Log* Log::m_Log = nullptr;
std::string Log::sAppPath = "";
Log::Log()
{

}

Log::~Log()
{

}

Log* Log::GetLog()
{
	if (m_Log == nullptr)
	{
		std::mutex  m_lock;
		m_lock.lock();
		m_Log = new Log();
		m_lock.unlock();

		log4cplus::Initializer initializer;
		log4cplus::SharedAppenderPtr appender(new log4cplus::ConsoleAppender());
		appender->setName(LOG4CPLUS_TEXT("console"));
		appender->setLayout(std::unique_ptr<log4cplus::Layout>(new log4cplus::SimpleLayout));
		log4cplus::Logger logger = log4cplus::Logger::getInstance(LOG4CPLUS_TEXT("test"));
		logger.setLogLevel(log4cplus::INFO_LOG_LEVEL);
		logger.addAppender(appender);
		LOG4CPLUS_INFO(logger, LOG4CPLUS_TEXT("Hello world"));

		int mm = 0;
	}
	return m_Log;
}

void Log::destroy()
{
	if (m_Log != nullptr)
	{
		delete m_Log;
		m_Log = nullptr;
	}
}

void Log::OUTPUTLOG(LogType type, std::string msg)
{
	Logger mLogger = log4cplus::Logger::getInstance(LOG4CPLUS_TEXT("main"));
	
	switch (type)
	{
	case TYPE_TRACE:
	{
		LOG4CPLUS_TRACE(mLogger, msg.c_str());
	}
		break;
	case TYPE_DEBUG:
	{
		LOG4CPLUS_DEBUG(mLogger, msg.c_str());
	}
		break;
	case TYPE_INFO:
	{
		LOG4CPLUS_INFO(mLogger, msg.c_str());
	}
		break;
	case TYPE_WARN:
	{
		LOG4CPLUS_WARN(mLogger, msg.c_str());
	}
		break;
	case TYPE_ERROR:
	{
		LOG4CPLUS_ERROR(mLogger, msg.c_str());
	}
		break;
	case TYPE_FATAL:
	{
		LOG4CPLUS_FATAL(mLogger, msg.c_str());
	}
		break;
	default:
		break;
	}
}