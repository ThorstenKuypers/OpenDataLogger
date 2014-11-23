#ifndef _DEBUG_LOG_H_
#define _DEBUG_LOG_H_
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

	private:

		ofstream _file;
	};
}

#endif // _DEBUG_LOH_H_