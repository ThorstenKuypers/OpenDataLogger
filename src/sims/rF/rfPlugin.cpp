

#include "rfPlugin.h"

using namespace rf_plugin;

rfPluginInfo g_pluginInfo;

/// DLL exports
extern "C" __declspec(dllexport) const char* GetPluginName() { return PLUGIN_NAME; }
extern "C" __declspec(dllexport) const unsigned GetPluginVersion() { return PLUGIN_VERSION; }
extern "C" __declspec(dllexport) const unsigned GetPluginObjectCount() { return 1; }
extern "C" __declspec(dllexport) PluginObjectInfo* GetPluginObjectInfo(const unsigned idx)
{

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


rfPlugin::rfPlugin() : CBasePlugin()
{
	char dir[1024];
	GetCurrentDirectoryA(1024, dir);

	_currentDir = string(dir);
	_dataLoggerEnabled = false;

	_odl = make_unique<COpenDataLogger>(std::string("rFactor"));

#ifdef _DEBUG
	_dbg = make_unique<DebugLog>(std::string(dir));
	_dbg->Log(string("Constructor"), __FILE__, __LINE__, "", __FUNCTION__);
	_dbg->Log(string(string("current directory: " + _currentDir)), __FILE__, __LINE__, "", __FUNCTION__);
#endif
}


rfPlugin::~rfPlugin()
{
#ifdef _DEBUG

	_dbg->Log(string("Destructor"), __FILE__, __LINE__, "", __FUNCTION__);

#endif
}


///
/// void Startup()
/// - called when plugin is loaded (at sim start)
///
void rfPlugin::Startup()
{

#ifdef _DEBUG
	char msg[512];
	sprintf_s(msg, 512, "DataLogger started! <ODL ref: _odl=%08X>", &_odl);
	_dbg->Log(string(msg), __FILE__, __LINE__, "", __FUNCTION__);
#endif
}

/// 
/// void Shutdown()
/// - called when plugin is unloaded (at sim shutdown)
///
void rfPlugin::Shutdown()
{
#ifdef _DEBUG
	char msg[512];
	sprintf_s(msg, 512, "DataLogger shut down! <ODL ref: _odl=%08X>", &_odl);
	_dbg->Log(string(msg), __FILE__, __LINE__, "", __FUNCTION__);
#endif
}

///
/// StartSession()
/// - called when session is started (during track loading)
///
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

	srand((UINT)time(NULL));
	_sessionID = (DWORD)rand();

	_odl->StartSession();
}

///
/// EndSession
/// - called when leaving the session
///
void rfPlugin::EndSession()
{
	_odl->StopSession();
}

///
/// EnterRealtime
/// - called when leaving the garage (monitor screen)
///
void rfPlugin::EnterRealtime()
{

	_maxEngineRpm = -1;
	_sampleRate = -1;
	_isInGarage = false;

	switchDataLogger();

	_stintRunning = true;

#ifdef _DEBUG
	_dbg->Log(string("Enter Realtime called"), __FILE__, __LINE__, "", __FUNCTION__);
#endif
}

///
/// ExitRealtime
/// - called when returning to garage (monitor screen)
/// 
void rfPlugin::ExitRealtime()
{
	_isInGarage = true;
	_startFuel = -1;

	if (_stintRunning && _dataLoggerEnabled) {
		_odl->StopStint();
	}

	_stintRunning = false;

#ifdef _DEBUG
	_dbg->Log(string("Exit Realtime called"), __FILE__, __LINE__, "", __FUNCTION__);
#endif
}

///
///
///
void rfPlugin::UpdateTelemetry(const TelemInfoV2& info)
{
	_sessionTime += info.mDeltaTime; // session time continues to run even if not on track

	if (_newSession) {

		// if this is the first call after loading a new session, set vehicle and track name
		// (used in logfile name)
		_odl->SetVehicleAndTrackName(string(info.mVehicleName), string(info.mTrackName));
		_newSession = false;

	}

	if (_sampleRate == -1) {

		_sampleRate = (int)round(((float)1.0f / (float)info.mDeltaTime)/* + (float)0.5f*/);

#ifdef _DEBUG
		char msg[512];
		sprintf_s(msg, 512, "Sample rate @: %i", _sampleRate);
		_dbg->Log(string(msg), __FILE__, __LINE__, "", __FUNCTION__);
#endif
		_odl->SetSampleRate(_sampleRate);
	}


	CarData cd = { 0 };
	SessionData sd = { 0 };

	if (!_isInGarage) {
		// only log data to file if car is on track and logger is running

		checkLoggerKeyState();
	}

	if (!_isInGarage && _dataLoggerEnabled) {

		sd.sessionTime = _sessionTime;

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
		cd.latAccel = (float)info.mLocalAccel.x;
		cd.latPos = (float)info.mPos.x;
		cd.longAccel = (float)(info.mLocalAccel.z * -1);
		cd.longPos = (float)info.mPos.z;
		cd.manifoldPress = -1;
		cd.oilLevel = -1;
		cd.oilTemp = info.mEngineOilTemp;
		cd.pitch = (float)info.mLocalRot.x;
		//cd.pitchRate = 
		cd.roll = (float)info.mLocalRot.z;
		//cd.rollRate = (float)info.mLocalRot.z;
		cd.rpm = info.mEngineRPM;
		cd.speed = (float)(sqrt((float)(pow(info.mLocalVel.x, 2.0f) + pow(info.mLocalVel.y, 2.0f) + pow(info.mLocalVel.z, 2.0f))));

		cd.steeringWheelAngle = -1;
		cd.throttle = info.mUnfilteredThrottle;
		cd.tractionControl = -1;
		cd.velocity[0] = (float)(info.mLocalVel.z * -1);
		cd.velocity[1] = (float)(info.mLocalVel.x * -1);;
		cd.velocity[2] = (float)info.mLocalVel.y;
		cd.vertAccel = (float)(info.mLocalAccel.y * -1);
		cd.voltage = -1;
		cd.waterLevel = -1;
		cd.waterTemp = info.mEngineWaterTemp;
		cd.yaw = (float)info.mLocalRot.y;
		//cd.yawRate = 

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

		if (_maxEngineRpm == -1)
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

	YamlUpdate((void*)&info);

#ifdef _DEBUG

	stringstream ss;
	ss << "-- _isInGarage: " << _isInGarage << endl;
	//ss << "-- scoring update delta: " << dt << endl;
	ss << "-- players car idx: " << _playerVehicleIdx << endl;
	ss << "-- player max RPM: " << _maxEngineRpm << endl;

	_dbg->Log(string(ss.str().c_str()), __FILE__, __LINE__, "", __FUNCTION__);

	FILE* fp = NULL;
	fopen_s(&fp, "ResultsStream.txt", "w");
	if (fp != NULL) {
		fwrite(info.mResultsStream, 1, strlen(info.mResultsStream) + 1, fp);
		//fprintf(fp, "%s", info.mResultsStream);
	}
	fclose(fp);
#endif

}

void rfPlugin::YamlUpdate(void* data)
{
	ScoringInfoV2* info = nullptr;
	std::string sessionInfoStr = std::string();

	if (data != nullptr)
	{
		info = reinterpret_cast<ScoringInfoV2*>(data);
	}

	memset(&_weekendInfo, 0, sizeof(_weekendInfo));
	memset(&_sessionInfo, 0, sizeof(_sessionInfo));
	memset(&_driverInfo, 0, sizeof(_driverInfo));

	// WeekendInfo
	_weekendInfo.TrackName = info->mTrackName;
	_weekendInfo.SessionID = _sessionID;
	_weekendInfo.WeekendOptions.NumStarters = info->mNumVehicles;
	_weekendInfo.TrackLength = (float)info->mLapDist / 1000;

	if (info->mDarkCloud < 0.25)
		_weekendInfo.TrackSkies = "Clear";
	else if (info->mDarkCloud < 0.5)
		_weekendInfo.TrackSkies = "Partly Cloudy";
	else if (info->mDarkCloud < 0.5)
		_weekendInfo.TrackSkies = "Mostly Cloudy";
	else
		_weekendInfo.TrackSkies = "Overcast";

	// mRaining not used
	_weekendInfo.TrackAirTemp = (float)info->mAmbientTemp;
	_weekendInfo.TrackSurfaceTemp = (float)info->mTrackTemp;

	// Wind isn't actually enabled, meh...
	_weekendInfo.TrackWindVel = sqrtf((info->mWind.x * info->mWind.x) + (info->mWind.y * info->mWind.y));
	// direction A·B = ||A|| ||B|| cos theta
	if (_weekendInfo.TrackWindVel > 0.0)
		_weekendInfo.TrackWindDir = acosf((info->mWind.x + info->mWind.y) / (sqrtf((info->mWind.x * info->mWind.x) + (info->mWind.y * info->mWind.y)) * sqrtf(1)));
	else {
		_weekendInfo.TrackWindDir = 0.0;
	}

	// mOnPathWetness not used
	// mOffPathWetness not used

	// _sessionInfo
	_sessionInfo.Sessions[info->mSession].SessionNum = info->mSession;
	_sessionInfo.Sessions[info->mSession].SessionTime = (float)info->mCurrentET;

#ifdef _DEBUG

#endif

	switch (info->mSession) // (0=testday 1-4=practice 5-8=qual 9=warmup 10-13=race)
	{
	case 0:
	case 1:
	case 2:
	case 3:
	case 4:
		_sessionInfo.Sessions[info->mSession].SessionType = "Practice";
		break;
	case 5:
	case 6:
	case 7:
	case 8:
		_sessionInfo.Sessions[info->mSession].SessionType = "Qualify";
		break;
	case 9:
		_sessionInfo.Sessions[info->mSession].SessionType = "Warmup";
		break;
	case 10:
	case 11:
	case 12:
	case 13:
		_sessionInfo.Sessions[info->mSession].SessionType = "Race";
		break;
	default:
		break;
	}

	if (info->mMaxLaps >= 2147483647)
		_sessionInfo.Sessions[info->mSession].SessionLaps = "unlimited";
	else
		_sessionInfo.Sessions[info->mSession].SessionLaps = std::to_string(info->mMaxLaps);


	_sessionInfo.Sessions[info->mSession].ResultsFastestLap.FastestTime = 999999.9f;

	// _driverInfo
	for (long i = 0; i < info->mNumVehicles; ++i)
	{
		VehicleScoringInfoV2 &vinfo = info->mVehicle[i];
		if ((float)vinfo.mBestLapTime < _sessionInfo.Sessions[info->mSession].ResultsFastestLap.FastestTime) {
			_sessionInfo.Sessions[info->mSession].ResultsFastestLap.CarIdx = i;// vinfo.mPlace;
			_sessionInfo.Sessions[info->mSession].ResultsFastestLap.FastestTime = (float)vinfo.mBestLapTime;
			//_sessionInfoSessions[info->mSession].ResultsFastestLap.FastestLap = vinfo.mBestLapTime;
		}

		//_sessionInfo.Sessions[info.mSession].ResultsPositions[i].CarIdx = vinfo.mID;
		_sessionInfo.Sessions[info->mSession].ResultsPositions[i].Position = vinfo.mPlace;
		_sessionInfo.Sessions[info->mSession].ResultsPositions[i].ClassPosition = vinfo.mPlace; // TODO
		_sessionInfo.Sessions[info->mSession].ResultsPositions[i].FastestTime = (float)vinfo.mBestLapTime;
		_sessionInfo.Sessions[info->mSession].ResultsPositions[i].Lap = vinfo.mTotalLaps;
		_sessionInfo.Sessions[info->mSession].ResultsPositions[i].LastTime = (float)vinfo.mLastLapTime;
		_sessionInfo.Sessions[info->mSession].ResultsPositions[i].LapsComplete = vinfo.mTotalLaps;
		_sessionInfo.Sessions[info->mSession].ResultsPositions[i].LapsDriven = vinfo.mTotalLaps; //(float)((double)vinfo->mTotalLaps + (vinfo->mLapDist/info->mLapDist));
		_sessionInfo.Sessions[info->mSession].ResultsPositions[i].ReasonOutId = (int)vinfo.mFinishStatus;

		switch (vinfo.mFinishStatus) // 0=none, 1=finished, 2=dnf, 3=dq
		{
		case 0:
			_sessionInfo.Sessions[info->mSession].ResultsPositions[i].ReasonOutStr = "Running";
			break;
		case 1:
			_sessionInfo.Sessions[info->mSession].ResultsPositions[i].ReasonOutStr = "Finished";
			break;
		case 2:
			_sessionInfo.Sessions[info->mSession].ResultsPositions[i].ReasonOutStr = "DNF";
			break;
		case 3:
			_sessionInfo.Sessions[info->mSession].ResultsPositions[i].ReasonOutStr = "Disqualified";
			break;
		default:
			break;
		}

		//_driverInfo.Drivers[i].CarIdx = vinfo->mID;
		//_driverInfo.Drivers[i].UserID = djb2((unsigned char *)&vinfo.mDriverName) & 0xFFFFFF;
		//_driverInfo.Drivers[i].CarID = djb2((unsigned char *)&vinfo.mVehicleName) & 0xFFFFFF;
		//_driverInfo.Drivers[i].CarClassID = djb2((unsigned char *)&vinfo.mVehicleClass) & 0xFFFFFF;
		//strcpy_s(_driverInfo.Drivers[i].UserName, IRSDK_MAX_STRING, vinfo.mDriverName);
		_driverInfo.Drivers[i].UserName = vinfo.mDriverName;
		//strcpy_s(_driverInfo.Drivers[i].CarPath, IRSDK_MAX_STRING, vinfo.mVehicleName);
		_driverInfo.Drivers[i].CarPath = vinfo.mVehicleName;
		//strcpy_s(_driverInfo.Drivers[i].CarClassShortName, IRSDK_MAX_STRING, vinfo.mVehicleClass);
		_driverInfo.Drivers[i].CarClassShortName = vinfo.mVehicleClass;

		//if (vinfo.mIsPlayer) {
		//	_driverInfo.DriverCarIdx = i; // vinfo.mPlace;
		//}
		_driverInfo.DriverCarIdx = _playerVehicleIdx;
		_driverInfo.DriverCarRedLine = _maxEngineRpm;

	}

	CYaml::GenerateSessionInfo(_weekendInfo, _sessionInfo, _driverInfo, &sessionInfoStr);

	_odl->UpdateSessionInfoString(sessionInfoStr);
}