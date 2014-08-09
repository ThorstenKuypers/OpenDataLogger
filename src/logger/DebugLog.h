
#include <fstream>
#include <sstream>
#include <string>
#include <ctime>

#include <Windows.h>


namespace OpenDataLogger
{
	class DebugLog
	{
	public:
		DebugLog();
		DebugLog(std::wstring& path);
		~DebugLog();

		void Log(std::string& msg, const char* file, const int line, const char* cls, const char* method);

		DebugLog& operator= (DebugLog&){ return DebugLog(); }

	private:

		std::ofstream _file;
	};
}