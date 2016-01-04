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
#include "rf2Plugin.h"

// plugin information
// exported plugin interface
extern "C" __declspec(dllexport)
const char * __cdecl GetPluginName()                   { return("OpenDataLogger"); }
extern "C" __declspec(dllexport)
PluginObjectType __cdecl GetPluginType()               { return(PO_INTERNALS); }
extern "C" __declspec(dllexport)
int __cdecl GetPluginVersion()                         { return(6); }
extern "C" __declspec(dllexport)
PluginObject * __cdecl CreatePluginObject()            { return((PluginObject *) new rf2Plugin); }
extern "C" __declspec(dllexport)
void __cdecl DestroyPluginObject(PluginObject *obj)  { delete((rf2Plugin *)obj); }
///////////////////////////////////////////////////////////////////////////////////////////////////


rf2Plugin::rf2Plugin() : CBasePlugin()
{
	char dir[1024];
	GetCurrentDirectoryA(1024, dir);

	_currentDir = string(dir);
	_dataLoggerEnabled = false;

	_odl = make_unique<COpenDataLogger>(std::string("rFactor 2"));

#ifdef _DEBUG
	_dbg = make_unique<DebugLog>(std::string(dir));
	_dbg->Log(std::string("Constructor"), __FILE__, __LINE__, nullptr, __FUNCTION__);
#endif
}

rf2Plugin::~rf2Plugin()
{
#ifdef _DEBUG
	_dbg->Log(std::string("Destructor"), __FILE__, __LINE__, nullptr, __FUNCTION__);
#endif
}

void rf2Plugin::StartSession()
{
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

void rf2Plugin::EndSession()
{
	_odl->StopSession();
}

void rf2Plugin::EnterRealtime()
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

void rf2Plugin::ExitRealtime()
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

void rf2Plugin::Load()
{

}

void rf2Plugin::Unload()
{

}

void rf2Plugin::SetPhysicsOptions(PhysicsOptionsV01 &options)
{
	TC = options.mTractionControl;
	ABS = options.mAntiLockBrakes;

}

bool rf2Plugin::ForceFeedback(double &forceValue)
{
	ffb = forceValue;

	return false;
}

void rf2Plugin::UpdateScoring(const ScoringInfoV01& info)
{
	if (_trackLength == -1) {

		_trackLength = (float)info.mLapDist;
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

	memset(&_scoringInfo, 0, sizeof(ScoringInfoV01));
	memset(&_playerVehicleInfo, 0, sizeof(VehicleScoringInfoV01));

	if (_playerVehicleIdx != -1) {

		memcpy(&_playerVehicleInfo, &info.mVehicle[_playerVehicleIdx], sizeof(VehicleScoringInfoV01));

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
	}
	fclose(fp);
#endif
}

void rf2Plugin::UpdateTelemetry(const TelemInfoV01& info)
{
	_sessionTime = info.mElapsedTime; // session time continues to run even if not on track

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
			cd.lapDist = (float)_playerVehicleInfo.mLapDist;
			cd.lapDistPct = (float)(_playerVehicleInfo.mLapDist / _trackLength);
		}
		cd.altPos = (float)info.mPos.y;
		cd.brake = (float)info.mUnfilteredBrake;
		cd.brakeBias = (float)info.mRearBrakeBias;
		cd.cfsRideHeight = (float)info.mFrontWingHeight;
		cd.clutch = (float)info.mUnfilteredClutch;
		cd.engineWarnings = 0;
		cd.fuelLevel = (float)info.mFuel;
		cd.fuelLevelPct = -1;
		if (_startFuel > 0)
			cd.fuelLevelPct = (float)(info.mFuel / _startFuel);
		cd.fuelPress = -1;
		cd.gear = info.mGear;
		//cd.isOnPitRoad = 
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
		cd.oilTemp = (float)info.mEngineOilTemp;
		cd.pitch = (float)info.mLocalRot.x;
		//cd.pitchRate = 
		cd.roll = (float)info.mLocalRot.z;
		//cd.rollRate = (float)info.mLocalRot.z;
		cd.rpm = (float)info.mEngineRPM;
		cd.speed = (float)(sqrt((float)(pow(info.mLocalVel.x, 2.0f) + pow(info.mLocalVel.y, 2.0f) + pow(info.mLocalVel.z, 2.0f))));

		cd.steeringWheelAngle = info.mUnfilteredSteering;
		cd.mSteeringShaftTorque = info.mSteeringShaftTorque;

		cd.throttle = (float)info.mUnfilteredThrottle;
		cd.tractionControl = -1;
		cd.velocity[0] = (float)(info.mLocalVel.z * -1);
		cd.velocity[1] = (float)(info.mLocalVel.x * -1);;
		cd.velocity[2] = (float)info.mLocalVel.y;
		cd.vertAccel = (float)(info.mLocalAccel.y * -1);
		cd.voltage = -1;
		cd.waterLevel = -1;
		cd.waterTemp = (float)info.mEngineWaterTemp;
		cd.yaw = (float)info.mLocalRot.y;
		//cd.yawRate = 

		cd.wheels[0].brakeLinePress = (float)info.mWheel[0].mBrakePressure;
		cd.wheels[0].pressure = (float)info.mWheel[0].mPressure;
		cd.wheels[0].rideHeight = (float)info.mWheel[0].mRideHeight;
		cd.wheels[0].speed = (float)info.mWheel[0].mRotation;
		// temperature is in °Kelvin so subtract 271 from the value to get °C
		cd.wheels[0].tempL = (float)(info.mWheel[0].mTemperature[0] - 273.15);
		cd.wheels[0].tempM = (float)(info.mWheel[0].mTemperature[1] - 273.15);
		cd.wheels[0].tempR = (float)(info.mWheel[0].mTemperature[2] - 273.15);
		cd.wheels[0].wearL = (float)info.mWheel[0].mWear;
		cd.wheels[0].wearM = (float)info.mWheel[0].mWear;
		cd.wheels[0].wearR = (float)info.mWheel[0].mWear;

		cd.wheels[1].brakeLinePress = (float)info.mWheel[1].mBrakePressure;
		cd.wheels[1].pressure = (float)info.mWheel[1].mPressure;
		cd.wheels[1].rideHeight = (float)info.mWheel[1].mRideHeight;
		cd.wheels[1].speed = (float)info.mWheel[1].mRotation;
		cd.wheels[1].tempL = (float)(info.mWheel[1].mTemperature[0] - 273.15);
		cd.wheels[1].tempM = (float)(info.mWheel[1].mTemperature[1] - 273.15);
		cd.wheels[1].tempR = (float)(info.mWheel[1].mTemperature[2] - 273.15);
		cd.wheels[1].wearL = (float)info.mWheel[1].mWear;
		cd.wheels[1].wearM = (float)info.mWheel[1].mWear;
		cd.wheels[1].wearR = (float)info.mWheel[1].mWear;

		cd.wheels[2].brakeLinePress = (float)info.mWheel[2].mBrakePressure;
		cd.wheels[2].pressure = (float)info.mWheel[2].mPressure;
		cd.wheels[2].rideHeight = (float)info.mWheel[2].mRideHeight;
		cd.wheels[2].speed = (float)info.mWheel[2].mRotation;
		cd.wheels[2].tempL = (float)(info.mWheel[2].mTemperature[0] - 273.15);
		cd.wheels[2].tempM = (float)(info.mWheel[2].mTemperature[1] - 273.15);
		cd.wheels[2].tempR = (float)(info.mWheel[2].mTemperature[2] - 273.15);
		cd.wheels[2].wearL = (float)info.mWheel[2].mWear;
		cd.wheels[2].wearM = (float)info.mWheel[2].mWear;
		cd.wheels[2].wearR = (float)info.mWheel[2].mWear;

		cd.wheels[3].brakeLinePress = (float)info.mWheel[3].mBrakePressure;
		cd.wheels[3].pressure = (float)info.mWheel[3].mPressure;
		cd.wheels[3].rideHeight = (float)info.mWheel[3].mRideHeight;
		cd.wheels[3].speed = (float)info.mWheel[3].mRotation;
		cd.wheels[3].tempL = (float)(info.mWheel[3].mTemperature[0] - 273.15);
		cd.wheels[3].tempM = (float)(info.mWheel[3].mTemperature[1] - 273.15);
		cd.wheels[3].tempR = (float)(info.mWheel[3].mTemperature[2] - 273.15);
		cd.wheels[3].wearL = (float)info.mWheel[3].mWear;
		cd.wheels[3].wearM = (float)info.mWheel[3].mWear;
		cd.wheels[3].wearR = (float)info.mWheel[3].mWear;

		cd.dcABS = ABS;
		cd.dcTC = TC;
		cd.wheelForce = ffb;

		if (_maxEngineRpm == -1)
			_maxEngineRpm = (float)info.mEngineMaxRPM;

		_odl->UpdateSessionInfo(sd);
		_odl->UpdateTelemetryInfo(cd);

		_odl->UpdateData();
	}
	else {

		// vehicle in garage (at the monitor)
		_startFuel = (float)info.mFuel;
	}
}

bool rf2Plugin::AccessWeather(double trackNodeSize, WeatherControlInfoV01 &info)
{
	return false;
}

void rf2Plugin::YamlUpdate(void* data)
{
	ScoringInfoV01* info = nullptr;
	std::string sessionInfoStr = std::string();

	if (data != nullptr)
	{
		info = reinterpret_cast<ScoringInfoV01*>(data);
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
	_weekendInfo.TrackWindVel = sqrtf(((float)info->mWind.x *(float)info->mWind.x) + ((float)info->mWind.y * (float)info->mWind.y));
	// direction A·B = ||A|| ||B|| cos theta
	if (_weekendInfo.TrackWindVel > 0.0)
		_weekendInfo.TrackWindDir = acosf(((float)info->mWind.x + (float)info->mWind.y) / (sqrtf(((float)info->mWind.x * (float)info->mWind.x) + ((float)info->mWind.y *(float)info->mWind.y)) * sqrtf(1)));
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
		VehicleScoringInfoV01 &vinfo = info->mVehicle[i];
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