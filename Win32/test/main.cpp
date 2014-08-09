
#include <cstdlib>
#include <cstdio>
#include <ctime>
#include <sstream>
#include <fstream>
#include <string>
#include <Windows.h>
#include <ShlObj.h>

using namespace std;

#define ODL_PATH		"\\OpenDataLogger"
#define LOGFILE_SUB_PATH "\\"
#define DATA_LOGFILE_SUB_PATH "\\LOG"

wstring odlPath;

// checks if ODL path and sub-paths exists; if not those are created
void CheckOdlPath()
{
	WCHAR* path = NULL;
	bool dirExists = false;

	HRESULT hr = SHGetKnownFolderPath(FOLDERID_Documents, KF_FLAG_DEFAULT_PATH | KF_FLAG_NOT_PARENT_RELATIVE, NULL, &path);
	if (SUCCEEDED(hr)) {

		std::wstringstream wss;
		wss << path << ODL_PATH;

		wstring ws = wstring(wss.str().c_str());
		odlPath = ws;

		if (!CreateDirectoryW((LPCWSTR)ws.c_str(), NULL)) {

			if (GetLastError() == ERROR_ALREADY_EXISTS) {

				// dir exists
				dirExists = true;
			}

		}

		//if (!dirExists)
		//	SHCreateDirectory(NULL, (PCWSTR)ws.c_str());

		dirExists = false;
		wss << DATA_LOGFILE_SUB_PATH;

		ws = wstring(wss.str().c_str());

		if (!CreateDirectoryW((LPCWSTR)ws.c_str(), NULL)) {

			if (GetLastError() == ERROR_ALREADY_EXISTS) {

				// dir exists
				dirExists = true;
			}

		}

		//if (!dirExists)
		//	SHCreateDirectory(NULL, (PCWSTR)ws.c_str());

		CoTaskMemFree(path);
	}

}

class CLogfile
{
public:
	CLogfile(string& simName)
	{

		_log = ofstream();
	}

	~CLogfile()
	{
	}

	void LogMessage(wstring& msg);

private:
	std::ofstream _log;
	std::wstring _logPath;
};


int main(void)
{
	CheckOdlPath();

	wchar_t d[1024];
	GetCurrentDirectoryW(1024, d);

	wstringstream wss;
	wss << odlPath.c_str() << LOGFILE_SUB_PATH << "odl.log";
	wstring logfile = wstring(wss.str().c_str());
	ofstream log = ofstream(logfile, std::ios_base::out);

	time_t t;
	time(&t);

	tm lt;
	localtime_s(&lt, (const time_t*)&t);
	char ts[MAX_PATH];
	strftime(ts, MAX_PATH, "[%H:%M:%S]:", (const tm*)&lt);

	std::string fn = std::string(__FILE__);
	size_t pos = fn.rfind("\\");
	if (pos != std::string::npos)
		fn = fn.substr(pos+1);

	stringstream ss;
	string s = string("rFactor");
	ss << ts << " Hello World " << fn << __LINE__ << __FUNCTION__ << endl;

	string str = string(ss.str().c_str());
	log.write(str.c_str(), str.length());

	log.flush();
	log.close();

	return 0;
}