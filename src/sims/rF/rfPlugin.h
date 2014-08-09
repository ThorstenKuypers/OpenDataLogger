#pragma once

#include <OpenDataLogger.h>

#include "InternalsPlugin.hpp"

#define PLUGIN_NAME "OpenDataLogger"
#define PLUGIN_FULL_NAME "OpenDataLogger for rFactor"
#define PLUGIN_DESC "OpenDataLogger plugin for rFactor"
#define PLUGIN_VERSION_STR "1.0"
#define PLUGIN_VERSION 1

namespace rf_plugin
{
	using namespace OpenDataLogger;

	class rfPlugin : public InternalsPluginV3
	{
	public:	

		rfPlugin();
		~rfPlugin() {}

		PluginObjectInfo* GetInfo();
		void Destroy() { Shutdown(); }
		PluginObjectProperty* GetProperty(const char* szName){ return 0; }
		PluginObjectProperty* GetProperty(const unsigned idx){ return 0; }
		unsigned GetPropertyCount() const { return 0; }

		void Startup();
		void Shutdown();

		void StartSession();
		void EndSession();

		void EnterRealtime();
		void ExitRealtime();

		bool WantsTelemetryUpdates() { return true; }
		void UpdateTelemetry(const TelemInfoV2& info);

		bool WantsScoringUpdates() { return(true); } 
		void UpdateScoring(const ScoringInfoV2 &info);

	private:

		COpenDataLogger* _odl;	// openDataLogger instance

		double _sessionTime;
		float _maxEngineRpm;
		bool _isInGarage;
		bool _stintRunning;

		bool _newSession;

		int _sampleRate;

		float _startFuel; // initial fuel level (as in setup)
		float _trackLength;
		long _playerVehicleIdx;	// array index of players vehicle

		ScoringInfoV2 _scoringInfo;	// latest scoring info (partial info; without vehicle array update)
		VehicleScoringInfoV2 _playerVehicleInfo; // VehicleScoringInfo of player

#ifdef _DEBUG
		DebugLog* _dbg;
		double _sr; // measure UpdateScoring call rate
#endif
	};

}

class rfPluginInfo : public PluginObjectInfo
{
public:
	rfPluginInfo() { sprintf_s(_fullName, 128, "%s - v%s", PLUGIN_FULL_NAME, PLUGIN_VERSION_STR); }
	virtual ~rfPluginInfo() {}

	const char* GetDesc() const { return (const char*)PLUGIN_DESC; }
	const char* GetFullName() const { return (const char*)_fullName; }
	const char* GetName() const { return (const char*)PLUGIN_NAME; }
	const char* GetSubType() const { return (const char*)"Internals"; }
	const unsigned GetType() const { return (const unsigned)PO_INTERNALS; }
	const unsigned GetVersion() const { return (const unsigned)3; }
	void* Create() const { return new rf_plugin::rfPlugin(); }
private:
	char _fullName[128];
};

