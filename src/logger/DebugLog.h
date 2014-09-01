
#include <fstream>
#include <sstream>
#include <string>
#include <ctime>

#include <Windows.h>

using namespace std;

namespace OpenDataLogger
{
	class DebugLog
	{
	public:
		DebugLog();
		DebugLog(std::string& path);
		~DebugLog();

		void Log(std::string& msg, const char* file, const int line, const char* cls, const char* method);

		//DebugLog& operator= (DebugLog&){ return new DebugLog(); }

	private:

		ofstream _file;
	};
}