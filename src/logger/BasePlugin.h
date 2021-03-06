#ifndef _ODL_BASE_PLUGIN_H_
#define _ODL_BASE_PLUGIN_H_
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
#include <memory>

#include "OpenDataLogger.h"
#include "yaml.h"

// Definition of Base plugin class - implements functionality common to all plugins
using namespace std;

struct _thData
{
	std::wstring dir;
	bool status;
};

namespace OpenDataLogger {

	class CBasePlugin
	{
	public:
		CBasePlugin();
		virtual ~CBasePlugin() {};

		static DWORD WINAPI SoundPlaybackThreadRoutine(void* param);

	protected:

		// must be implemented in actual plugin implementation 
		virtual void YamlUpdate(void*) = 0;

		void switchDataLogger();
		void checkLoggerKeyState();

		WeekendInfo _weekendInfo;
		SessionInfo _sessionInfo;
		DriverInfo _driverInfo;

		// OpenDataLogger class instance
		unique_ptr<COpenDataLogger> _odl;

		DWORD _sessionID;

		// current session time (elapsed time since begin of session)
		double _sessionTime;

		// redline RPM for current car
		float _maxEngineRpm;
		bool _isInGarage;
		bool _stintRunning;

		bool _newSession;

		int _sampleRate;

		float _startFuel; // initial fuel level (as in setup)
		float _trackLength;
		long _playerVehicleIdx;	// array index of players vehicle
		bool _dataLoggerEnabled;
		bool _dataLoggerEnableKeyDown; // status of key to enable the logger (pressed/released)
		double _dataLoggerKeyDelta;		// time delta since the last time the data logger enable key was pressed

		string _currentDir;	// current directory; usually the main directory of the sim

#ifdef _DEBUG
		unique_ptr<DebugLog> _dbg;
		double _sr; // measure UpdateScoring call rate
#endif

	};
} // namespace OpenDataLogger

#endif //_ODL_BASE_PLUGIN_H_