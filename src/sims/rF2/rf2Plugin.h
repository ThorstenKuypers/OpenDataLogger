#pragma once

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

