
#include "DebugLog.h"

using namespace OpenDataLogger;

DebugLog::DebugLog()
{
}

DebugLog::DebugLog(std::wstring& path)
{
	std::wstringstream wss;
	wss << path << "\\odldbg.log";
	std::wstring dir = std::wstring(wss.str().c_str());

	_file = std::ofstream(dir, std::ios_base::out);

}

DebugLog::~DebugLog()
{
	if (_file.is_open())
	{
		_file.flush();
		_file.close();
	}
}

void DebugLog::Log(std::string& msg, const char* file, const int line, const char* cls, const char* method)
{
	time_t t;
	time(&t);

	tm lt;
	localtime_s(&lt, (const time_t*)&t);
	char ts[1024];
	strftime(ts, 1024, "[%H:%M:%S]: ", (const tm*)&lt);

	std::string fn = std::string(file);
	size_t pos = fn.rfind("\\");
	if (pos != std::string::npos)
		fn = fn.substr(pos+1);

	std::stringstream ss;
	ss << fn << "\t" << line << " @ " << method << std::endl;
	ss << ts << msg << std::endl;
	std::string str = std::string(ss.str().c_str());

	_file.write(str.c_str(), str.length());
	_file.flush();
}