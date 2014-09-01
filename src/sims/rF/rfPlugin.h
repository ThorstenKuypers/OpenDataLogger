#pragma once

#include <OpenDataLogger.h>
#include <BasePlugin.h>

#include "InternalsPlugin.hpp"

#define PLUGIN_NAME "OpenDataLogger"
#define PLUGIN_FULL_NAME "OpenDataLogger for rFactor"
#define PLUGIN_DESC "OpenDataLogger plugin for rFactor"
#define PLUGIN_VERSION_STR "1.0"
#define PLUGIN_VERSION 1

namespace rf_plugin
{
	using namespace OpenDataLogger;

	class rfPlugin : public InternalsPluginV3, CBasePlugin
	{
	public:	

		rfPlugin();
		~rfPlugin();

		PluginObjectInfo* GetInfo();
		void Destroy()
		{
#ifdef _DEBUG
			if (_dbg != nullptr)
				_dbg->Log(string("PluginObjectInfo destroyed"), __FILE__, __LINE__, "", __FUNCTION__);
#endif
			Shutdown();

		}

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

		//bool WantsGraphicsUpdates() { return(true); }
		//void UpdateGraphics(const GraphicsInfo &info);

	private:

		virtual void YamlUpdate(void*);
		void switchDataLogger();
		static DWORD WINAPI SoundPlaybackThreadRoutine(void* param);

		COpenDataLogger* _odl;	// openDataLogger instance

		DWORD _sessionID;

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

		bool _dataLoggerEnabled; 
		bool _dataLoggerEnableKeyDown; // status of key to enable the logger (pressed/released)
		double _dataLoggerKeyDelta;		// time delta since the last time the data logger enable key was pressed

		string _currentDir;	// current directory; usually the main directory of the sim

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

