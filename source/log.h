#pragma once
#include <string>

enum LogType
{
	TYPE_TRACE,
	TYPE_DEBUG,
	TYPE_INFO,
	TYPE_WARN,
	TYPE_ERROR,
	TYPE_FATAL
};

class Log
{
public:
	static Log* GetLog();
	static void destroy();
	static void SetAppPath(std::string path) { sAppPath = path; }

	static void OUTPUTLOG(LogType,std::string);
private:
	Log();
	~Log();

	static Log*	m_Log;
	static std::string sAppPath;
};