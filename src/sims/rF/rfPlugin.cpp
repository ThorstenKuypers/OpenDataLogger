

#include "rfPlugin.h"

using namespace rf_plugin;

rfPluginInfo g_pluginInfo;

/// DLL exports
extern "C" __declspec(dllexport) const char* GetPluginName() { return PLUGIN_NAME; }
extern "C" __declspec(dllexport) const unsigned GetPluginVersion() { return PLUGIN_VERSION; }
extern "C" __declspec(dllexport) const unsigned GetPluginObjectCount() { return 1; }
extern "C" __declspec(dllexport) PluginObjectInfo* GetPluginObjectInfo(const unsigned idx) {

	switch (idx) {

	case 0:
		return &g_pluginInfo;
		break;
	default:
		return 0;
	}
}
///----------------------------------------------------------------------------


PluginObjectInfo* rfPlugin::GetInfo() { return (PluginObjectInfo*)&g_pluginInfo; }


rfPlugin::rfPlugin()
{
	_odl = NULL;

#ifdef _DEBUG
	wchar_t dir[1024];
	GetCurrentDirectoryW(1024, dir);

	_dbg = new DebugLog(std::wstring(dir));

#endif
}


///
/// void Startup()
/// - called when plugin is loaded (at sim start)
void rfPlugin::Startup()
{
	_odl = new COpenDataLogger(std::string("rFactor"));

#ifdef _DEBUG
	char msg[512];
	sprintf_s(msg, 512, "ODL ref: _odl=%08X", _odl);
	_dbg->Log(string(msg), __FILE__, __LINE__, "", __FUNCTION__);
#endif
}

/// 
/// void Shutdown()
/// - called when plugin is unloaded (at sim shutdown)
void rfPlugin::Shutdown()
{
#ifdef _DEBUG
	char msg[512];
	sprintf_s(msg, 512, "ODL ref: _odl=%08X", _odl);
	_dbg->Log(string(msg), __FILE__, __LINE__, "", __FUNCTION__);
#endif
	if (_odl) {
		delete _odl;
		_odl = NULL;
	}

#ifdef _DEBUG
	if (_dbg != NULL)
		delete _dbg;
#endif
}

///
/// StartSession()
/// - called when session is started (during track loading)
void rfPlugin::StartSession()
{
#ifdef _DEBUG
	_sr = 0;
#endif

	_trackLength = -1;
	_isInGarage = true;
	_sessionTime = 0;
	_stintRunning = false;
	_newSession = true;
	_playerVehicleIdx = -1;
	_startFuel = -1;

	_odl->StartSession();
}

///
/// EndSession
/// - 
void rfPlugin::EndSession()
{
	_odl->StopSession();
}

///
/// EnterRealtime
/// -
void rfPlugin::EnterRealtime()
{
	_maxEngineRpm = -1;
	_sampleRate = -1;
	_isInGarage = false;

	if (!_stintRunning) {
		_odl->StartStint();
		_stintRunning = true;
	}

#ifdef _DEBUG
	_dbg->Log(string("Enter Realtime called"), __FILE__, __LINE__, "", __FUNCTION__);
#endif
}

///
/// ExitRealtime
// - 
void rfPlugin::ExitRealtime()
{
	_isInGarage = true;
	_startFuel = -1;

	if (_stintRunning) {
		_odl->StopStint();
		_stintRunning = false;
	}

#ifdef _DEBUG
	_dbg->Log(string("Exit Realtime called"), __FILE__, __LINE__, "", __FUNCTION__);
#endif
}

///
///
///
void rfPlugin::UpdateTelemetry(const TelemInfoV2& info)
{
	if (_newSession) {

		// if this is the first call after entering realtime, set vehicle and track name
		// (used in logfile name)
		_odl->SetVehicleAndTrackName(string(info.mVehicleName), string(info.mTrackName));
		_newSession = false;

	}
	
	if (_sampleRate == -1) {

		_sampleRate = (int)floor(((float)1.0f / (float)info.mDeltaTime) + (float)0.5f);
		_odl->SetSampleRate(_sampleRate);
	}

	_sessionTime += info.mDeltaTime; // session time continues to run even if not on track

	CarData cd = { 0 };
	SessionData sd = { 0 };

#ifdef _DEBUG

	if (_sr == 0)
		_sr = _sessionTime;
#endif


	if (!_isInGarage) {

		sd.sessionTime = _sessionTime;

#ifdef _DEBUG
		stringstream ss;
		ss << "elapsed time: " << sd.sessionTime << endl;
		_dbg->Log(string(ss.str().c_str()), __FILE__, __LINE__, "", __FUNCTION__);
#endif

		if (_playerVehicleIdx != -1) {

			cd.isOnPitRoad = _playerVehicleInfo.mInPits;
			cd.lapDist = _playerVehicleInfo.mLapDist;
			cd.lapDistPct = (float)(_playerVehicleInfo.mLapDist / _trackLength);

		}

		cd.altPos = info.mPos.y;
		cd.brake = info.mUnfilteredBrake;
		cd.brakeBias = -1;
		cd.cfsRideHeight = -1;
		cd.clutch = info.mUnfilteredClutch;
		cd.engineWarnings = 0;
		cd.fuelLevel = info.mFuel;
		cd.fuelLevelPct = -1;
		if (_startFuel > 0)
			cd.fuelLevelPct = (float)(info.mFuel / _startFuel);
		cd.fuelPress = -1;
		cd.gear = info.mGear;
		//cd.isOnPitRoad = -1;
		cd.isOnTrack = !_isInGarage;
		cd.lap = info.mLapNumber;
		//cd.lapDist = 0;
		//cd.lapDistPct = 0;
		cd.latAccel = info.mLocalAccel.x;
		cd.latPos = info.mPos.x;
		cd.longAccel = info.mLocalAccel.z;
		cd.longPos = info.mPos.z;
		cd.manifoldPress = -1;
		cd.oilLevel = -1;
		cd.oilTemp = info.mEngineOilTemp;
		//cd.pitch
		cd.pitchRate = info.mLocalRot.x;
		//cd.roll
		cd.rollRate = info.mLocalRot.z;
		cd.rpm = info.mEngineRPM;
		cd.speed = (float)(sqrt((float)(pow(info.mLocalVel.x, 2.0f) + pow(info.mLocalVel.y, 2.0f) + pow(info.mLocalVel.z, 2.0f))));

#ifdef _DEBUG
		ss << "speed: " << cd.speed << endl;
		_dbg->Log(string(ss.str().c_str()), __FILE__, __LINE__, "", __FUNCTION__);
#endif
		cd.steeringWheelAngle = -1;
		cd.throttle = info.mUnfilteredThrottle;
		cd.tractionControl = -1;
		cd.velocity[0] = info.mLocalVel.x;
		cd.velocity[1] = info.mLocalVel.y;
		cd.velocity[2] = info.mLocalVel.z;
		cd.vertAccel = info.mLocalAccel.y;
		cd.voltage = -1;
		cd.waterLevel = -1;
		cd.waterTemp = info.mEngineWaterTemp;
		//cd.yaw
		cd.yawRate = info.mLocalRot.y;

		cd.wheels[0].brakeLinePress = -1;
		cd.wheels[0].pressure = info.mWheel[0].mPressure;
		cd.wheels[0].rideHeight = info.mWheel[0].mRideHeight;
		cd.wheels[0].speed = info.mWheel[0].mRotation;
		cd.wheels[0].tempL = info.mWheel[0].mTemperature[0];
		cd.wheels[0].tempM = info.mWheel[0].mTemperature[1];
		cd.wheels[0].tempR = info.mWheel[0].mTemperature[2];
		cd.wheels[0].wearL = info.mWheel[0].mWear;
		cd.wheels[0].wearM = info.mWheel[0].mWear;
		cd.wheels[0].wearR = info.mWheel[0].mWear;
		
		cd.wheels[1].brakeLinePress = -1;
		cd.wheels[1].pressure = info.mWheel[1].mPressure;
		cd.wheels[1].rideHeight = info.mWheel[1].mRideHeight;
		cd.wheels[1].speed = info.mWheel[1].mRotation;
		cd.wheels[1].tempL = info.mWheel[1].mTemperature[0];
		cd.wheels[1].tempM = info.mWheel[1].mTemperature[1];
		cd.wheels[1].tempR = info.mWheel[1].mTemperature[2];
		cd.wheels[1].wearL = info.mWheel[1].mWear;
		cd.wheels[1].wearM = info.mWheel[1].mWear;
		cd.wheels[1].wearR = info.mWheel[1].mWear;

		cd.wheels[2].brakeLinePress = -1;
		cd.wheels[2].pressure = info.mWheel[2].mPressure;
		cd.wheels[2].rideHeight = info.mWheel[2].mRideHeight;
		cd.wheels[2].speed = info.mWheel[2].mRotation;
		cd.wheels[2].tempL = info.mWheel[2].mTemperature[0];
		cd.wheels[2].tempM = info.mWheel[2].mTemperature[1];
		cd.wheels[2].tempR = info.mWheel[2].mTemperature[2];
		cd.wheels[2].wearL = info.mWheel[2].mWear;
		cd.wheels[2].wearM = info.mWheel[2].mWear;
		cd.wheels[2].wearR = info.mWheel[2].mWear;

		cd.wheels[3].brakeLinePress = -1;
		cd.wheels[3].pressure = info.mWheel[3].mPressure;
		cd.wheels[3].rideHeight = info.mWheel[3].mRideHeight;
		cd.wheels[3].speed = info.mWheel[3].mRotation;
		cd.wheels[3].tempL = info.mWheel[3].mTemperature[0];
		cd.wheels[3].tempM = info.mWheel[3].mTemperature[1];
		cd.wheels[3].tempR = info.mWheel[3].mTemperature[2];
		cd.wheels[3].wearL = info.mWheel[3].mWear;
		cd.wheels[3].wearM = info.mWheel[3].mWear;
		cd.wheels[3].wearR = info.mWheel[3].mWear;
		
		
		_maxEngineRpm = info.mEngineMaxRPM;

		_odl->UpdateSessionInfo(sd);
		_odl->UpdateTelemetryInfo(cd);

		_odl->UpdateData();
	}
	else {

		// vehicle in garage (at the monitor)
		_startFuel = info.mFuel;
	}
}

///
/// UpdateScoring
/// - called 2 times a second to update scoring and competitor data
///
void rfPlugin::UpdateScoring(const ScoringInfoV2& info)
{
	if (_trackLength == -1) {

		_trackLength = info.mLapDist;
		_odl->SetTrackLength(_trackLength);
	}
	
	if (_playerVehicleIdx == -1) {

		// find index of players vehicle in vehicle info array
		for (long i = 0; i < info.mNumVehicles; i++) {

			if (info.mVehicle[i].mIsPlayer) {

				// players vehicle found
				_playerVehicleIdx = i;
				break;
			}
		}
	}

	memset(&_scoringInfo, 0, sizeof(ScoringInfoV2));
	memset(&_playerVehicleInfo, 0, sizeof(VehicleScoringInfoV2));

	if (_playerVehicleIdx != -1) {

		memcpy(&_playerVehicleInfo, &info.mVehicle[_playerVehicleIdx], sizeof(VehicleScoringInfoV2));
	}

#ifdef _DEBUG
	
	double dt = _sessionTime - _sr;
	_sr = 0;

	stringstream ss;
	ss << "-- _isInGarage: " << _isInGarage << endl;
	ss << "-- scoring update delta: " << dt << endl;
	ss << "-- players car idx: " << _playerVehicleIdx << endl;

	_dbg->Log(string(ss.str().c_str()), __FILE__, __LINE__, "", __FUNCTION__);

#endif

}