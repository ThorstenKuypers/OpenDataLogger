/*****************************************************************************
** openDataLogger - implementation of openGameAPI for use in plugins for
several racing simulations

** (C) 2014 Thorsten Kuypers

** This file is part of openDataLogger project.

** openDataLogger is free software : you can redistribute it and / or modify
** it under the terms of the GNU General Public License as published by
** the Free Software Foundation, either version 3 of the License.

** openDataLogger is distributed in the hope that it will be useful,
** but WITHOUT ANY WARRANTY; without even the implied warranty of
** MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.See the
** GNU General Public License for more details.

** You should have received a copy of the GNU General Public License
** along with openDataLogger. If not, see <http://www.gnu.org/licenses/>.
******************************************************************************/

#include "DebugLog.h"

using namespace OpenDataLogger;

DebugLog::DebugLog()
{
}

DebugLog::DebugLog(std::string& path) :
_file(std::ofstream())
{
	std::stringstream wss;
	wss << path << "\\odldbg.log";
	std::string dir = std::string(wss.str().c_str());

	_file.open(dir, ios::out);
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