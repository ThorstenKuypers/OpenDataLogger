#ifndef _RF2_PLUGIN_H_
#define _RF2_PLUGIN_H_
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

#include <OpenDataLogger.h>
#include <BasePlugin.h>

#include "InternalsPlugin.hpp"

using namespace std;
using namespace OpenDataLogger;

class rf2Plugin : public InternalsPluginV06, CBasePlugin
{
public:
	rf2Plugin();
	~rf2Plugin();

	virtual void Startup(long version) {}                      // sim startup with version * 1000
	virtual void Shutdown() {}                                   // sim shutdown

	virtual void Load();                                       // scene/track load
	virtual void Unload();                                     // scene/track unload

	virtual void StartSession();                               // session started
	virtual void EndSession();                                 // session ended

	virtual void EnterRealtime();                              // entering realtime (where the vehicle can be driven)
	virtual void ExitRealtime();                               // exiting realtime

	// SCORING OUTPUT
	virtual bool WantsScoringUpdates() { return(true); }      // whether we want scoring updates
	virtual void UpdateScoring(const ScoringInfoV01 &info);  // update plugin with scoring info (approximately five times per second)

	// GAME OUTPUT
	virtual long WantsTelemetryUpdates() { return(1); }        // whether we want telemetry updates (0=no 1=player-only 2=all vehicles)
	virtual void UpdateTelemetry(const TelemInfoV01 &info);  // update plugin with telemetry info

	// CONDITIONS CONTROL
	virtual bool WantsWeatherAccess()                                   { return(true); } // change to true in order to read or write weather with AccessWeather() call:
	virtual bool AccessWeather(double trackNodeSize, WeatherControlInfoV01 &info); // current weather is passed in; return true if you want to change it

private:

	virtual void YamlUpdate(void*);

	ScoringInfoV01 _scoringInfo;	// latest scoring info (partial info; without vehicle array update)
	VehicleScoringInfoV01 _playerVehicleInfo; // VehicleScoringInfo of player
};

#endif // _RF2_PLUGIN_H_