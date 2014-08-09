#include <ShlObj.h>

#include "OpenDataLogger.h"

using namespace OpenDataLogger;

COpenDataLogger::COpenDataLogger(string& simName)
{
	// register irsdk variables with server class
	// (default iRacing variables)
	irsdkVar("SessionTime", &_sessionData.sessionTime, irsdk_double, 1, "Seconds since session start", "s", IRSDK_LOG_ALL);
	irsdkVar("SessionState", &_sessionData.sessionState, irsdk_int, 1, "Session state", "irsdk_SessionState", IRSDK_LOG_ALL);
	irsdkVar("SessionTimeRemain", &_sessionData.sessionTimeLeft, irsdk_double, 1, "Seconds left till session ends", "s", IRSDK_LOG_ALL);
	irsdkVar("SessionLapsRemain", &_sessionData.sessionLapsLeft, irsdk_int, 1, "Laps left till session ends", "", IRSDK_LOG_ALL);

	irsdkVar("SteeringWheelAngle", &_carData.steeringWheelAngle, irsdk_float, 1, "Steering wheel angle", "rad", IRSDK_LOG_ALL);
	irsdkVar("Throttle", &_carData.throttle, irsdk_float, 1, "0=off throttle to 1=full throttle", "%", IRSDK_LOG_ALL);
	irsdkVar("Brake", &_carData.brake, irsdk_float, 1, "0=brake released to 1=max pedal force", "%", IRSDK_LOG_ALL);
	irsdkVar("Clutch", &_carData.clutch, irsdk_float, 1, "0=disengaged to 1=fully engaged", "%", IRSDK_LOG_ALL);
	irsdkVar("Gear", &_carData.gear, irsdk_int, 1, "Gear", "", IRSDK_LOG_ALL, 1, 0);
	irsdkVar("RPM", &_carData.rpm, irsdk_float, 1, "Engine RPM", "revs/min", IRSDK_LOG_ALL);

	irsdkVar("Lap", &_carData.lap, irsdk_int, 1, "Lap count", "", IRSDK_LOG_ALL);
	irsdkVar("LapDist", &_carData.lapDist, irsdk_float, 1, "Meters traveled from S/F this lap", "m", IRSDK_LOG_ALL);
	irsdkVar("LapDistPct", &_carData.lapDistPct, irsdk_float, 1, "Percentage distance around lap", "%", IRSDK_LOG_ALL);

	irsdkVar("LapBestLap", &_timingData.LapBestLap, irsdk_int, 1, "Players best lap number", "s", IRSDK_LOG_ALL);
	irsdkVar("LapBestLapTime", &_timingData.LapBestLapTime, irsdk_float, 1, "Players best lap time", "s", IRSDK_LOG_ALL);
	irsdkVar("LapLastLapTime", &_timingData.LapLastLapTime, irsdk_float, 1, "Players last lap time", "s", IRSDK_LOG_ALL);
	irsdkVar("LapCurrentLapTime", &_timingData.LapCurrentLapTime, irsdk_int, 1, "Estimate of players current lap time", "s", IRSDK_LOG_ALL);

	irsdkVar("LongAccel", &_carData.longAccel, irsdk_float, 1, "Longitudinal acceleration", "m/s^2", IRSDK_LOG_ALL);
	irsdkVar("LatAccel", &_carData.latAccel, irsdk_float, 1, "Lateral acceleration", "m/s^2", IRSDK_LOG_ALL);
	irsdkVar("VertAccel", &_carData.vertAccel, irsdk_float, 1, "Vertical acceleration", "m/s^2", IRSDK_LOG_ALL);
	irsdkVar("Speed", &_carData.speed, irsdk_float, 1, "Car speed", "m/s", IRSDK_LOG_ALL);

	irsdkVar("VelocityX", &_carData.velocity[0], irsdk_float, 1, "X velocity", "m/s", IRSDK_LOG_ALL);
	irsdkVar("VelocityY", &_carData.velocity[1], irsdk_float, 1, "Y velocity", "m/s", IRSDK_LOG_ALL);
	irsdkVar("VelocityZ", &_carData.velocity[2], irsdk_float, 1, "Z velocity", "m/s", IRSDK_LOG_ALL);

	irsdkVar("Yaw", &_carData.yaw, irsdk_float, 1, "Yaw orientation", "rad", IRSDK_LOG_DISK);
	irsdkVar("Pitch", &_carData.pitch, irsdk_float, 1, "Pitch orientation", "rad", IRSDK_LOG_DISK);
	irsdkVar("Roll", &_carData.roll, irsdk_float, 1, "Roll orientation", "rad", IRSDK_LOG_DISK);

	irsdkVar("Alt", &_carData.altPos, irsdk_float, 1, "Altitude in meters", "m", IRSDK_LOG_DISK);

	irsdkVar("EngineWarnings", &_carData.engineWarnings, irsdk_bitField, 1, "Bitfield for warning lights", "", IRSDK_LOG_ALL);
	irsdkVar("FuelLevel", &_carData.fuelLevel, irsdk_float, 1, "Liters of fuel remaining", "l", IRSDK_LOG_ALL);
	irsdkVar("FuelLevelPct", &_carData.fuelLevelPct, irsdk_float, 1, "Percent fuel remaining", "%", IRSDK_LOG_ALL);
	irsdkVar("FuelPress", &_carData.fuelPress, irsdk_float, 1, "Engine fuel pressure", "bar", IRSDK_LOG_ALL);

	irsdkVar("WaterTemp", &_carData.waterTemp, irsdk_float, 1, "Engine coolant temp", "C", IRSDK_LOG_ALL);
	irsdkVar("OilTemp", &_carData.oilTemp, irsdk_float, 1, "Engine oil temperature", "C", IRSDK_LOG_ALL);


	// RR wheel data
	/////////////////
	irsdkVar("RRbrakeLinePress", &_carData.wheels[RR].brakeLinePress, irsdk_float, 1, "RR brake line pressure", "bar", IRSDK_LOG_DISK);
	irsdkVar("RRspeed", &_carData.wheels[RR].speed, irsdk_float, 1, "RR wheel speed", "m/s", IRSDK_LOG_DISK);
	irsdkVar("RRpressure", &_carData.wheels[RR].pressure, irsdk_float, 1, "RR tire pressure", "kpa", IRSDK_LOG_DISK);
	irsdkVar("RRtempL", &_carData.wheels[RR].tempL, irsdk_float, 1, "RR tire left surface temperature", "C", IRSDK_LOG_DISK);
	irsdkVar("RRtempM", &_carData.wheels[RR].tempM, irsdk_float, 1, "RR tire middle surface temperature", "C", IRSDK_LOG_DISK);
	irsdkVar("RRtempR", &_carData.wheels[RR].tempR, irsdk_float, 1, "RR tire right surface temperature", "C", IRSDK_LOG_DISK);
	irsdkVar("RRwearL", &_carData.wheels[RR].wearL, irsdk_float, 1, "RR tire left percent tread remaining", "%", IRSDK_LOG_DISK);
	irsdkVar("RRwearM", &_carData.wheels[RR].wearM, irsdk_float, 1, "RR tire middle percent tread remaining", "%", IRSDK_LOG_DISK);
	irsdkVar("RRwearR", &_carData.wheels[RR].wearR, irsdk_float, 1, "RR tire right percent tread remaining", "%", IRSDK_LOG_DISK);


	// LR wheel data
	////////////////
	irsdkVar("LRbrakeLinePress", &_carData.wheels[LR].brakeLinePress, irsdk_float, 1, "LR brake line pressure", "bar", IRSDK_LOG_DISK);
	irsdkVar("LRspeed", &_carData.wheels[LR].speed, irsdk_float, 1, "LR wheel speed", "m/s", IRSDK_LOG_DISK);
	irsdkVar("LRpressure", &_carData.wheels[LR].pressure, irsdk_float, 1, "LR tire pressure", "kpa", IRSDK_LOG_DISK);
	irsdkVar("LRtempL", &_carData.wheels[LR].tempL, irsdk_float, 1, "LR tire left surface temperature", "C", IRSDK_LOG_DISK);
	irsdkVar("LRtempM", &_carData.wheels[LR].tempM, irsdk_float, 1, "LR tire middle surface temperature", "C", IRSDK_LOG_DISK);
	irsdkVar("LRtempR", &_carData.wheels[LR].tempR, irsdk_float, 1, "LR tire right surface temperature", "C", IRSDK_LOG_DISK);
	irsdkVar("LRwearL", &_carData.wheels[LR].wearL, irsdk_float, 1, "LR tire left percent tread remaining", "%", IRSDK_LOG_DISK);
	irsdkVar("LRwearM", &_carData.wheels[LR].wearM, irsdk_float, 1, "LR tire middle percent tread remaining", "%", IRSDK_LOG_DISK);
	irsdkVar("LRwearR", &_carData.wheels[LR].wearR, irsdk_float, 1, "LR tire right percent tread remaining", "%", IRSDK_LOG_DISK);


	// RF wheel data
	////////////////
	irsdkVar("RFbrakeLinePress", &_carData.wheels[RF].brakeLinePress, irsdk_float, 1, "RF brake line pressure", "bar", IRSDK_LOG_DISK);
	irsdkVar("RFspeed", &_carData.wheels[RF].speed, irsdk_float, 1, "RF wheel speed", "m/s", IRSDK_LOG_DISK);
	irsdkVar("RFpressure", &_carData.wheels[RF].pressure, irsdk_float, 1, "RF tire pressure", "kpa", IRSDK_LOG_DISK);
	irsdkVar("RFtempL", &_carData.wheels[RF].tempL, irsdk_float, 1, "RF tire left surface temperature", "C", IRSDK_LOG_DISK);
	irsdkVar("RFtempM", &_carData.wheels[RF].tempM, irsdk_float, 1, "RF tire middle surface temperature", "C", IRSDK_LOG_DISK);
	irsdkVar("RFtempR", &_carData.wheels[RF].tempR, irsdk_float, 1, "RF tire right surface temperature", "C", IRSDK_LOG_DISK);
	irsdkVar("RFwearL", &_carData.wheels[RF].wearL, irsdk_float, 1, "RF tire left percent tread remaining", "%", IRSDK_LOG_DISK);
	irsdkVar("RFwearM", &_carData.wheels[RF].wearM, irsdk_float, 1, "RF tire middle percent tread remaining", "%", IRSDK_LOG_DISK);
	irsdkVar("RFwearR", &_carData.wheels[RF].wearR, irsdk_float, 1, "RF tire right percent tread remaining", "%", IRSDK_LOG_DISK);


	// LF wheel data
	////////////////
	irsdkVar("LFbrakeLinePress", &_carData.wheels[LF].brakeLinePress, irsdk_float, 1, "LF brake line pressure", "bar", IRSDK_LOG_DISK);
	irsdkVar("LFspeed", &_carData.wheels[LF].speed, irsdk_float, 1, "LF wheel speed", "m/s", IRSDK_LOG_DISK);
	irsdkVar("LFpressure", &_carData.wheels[LF].pressure, irsdk_float, 1, "LF tire pressure", "kpa", IRSDK_LOG_DISK);
	irsdkVar("LFtempL", &_carData.wheels[LF].tempL, irsdk_float, 1, "LF tire left surface temperature", "C", IRSDK_LOG_DISK);
	irsdkVar("LFtempM", &_carData.wheels[LF].tempM, irsdk_float, 1, "LF tire middle surface temperature", "C", IRSDK_LOG_DISK);
	irsdkVar("LFtempR", &_carData.wheels[LF].tempR, irsdk_float, 1, "LF tire right surface temperature", "C", IRSDK_LOG_DISK);
	irsdkVar("LFwearL", &_carData.wheels[LF].wearL, irsdk_float, 1, "LF tire left percent tread remaining", "%", IRSDK_LOG_DISK);
	irsdkVar("LFwearM", &_carData.wheels[LF].wearM, irsdk_float, 1, "LF tire middle percent tread remaining", "%", IRSDK_LOG_DISK);
	irsdkVar("LFwearR", &_carData.wheels[LF].wearR, irsdk_float, 1, "LF tire right percent tread remaining", "%", IRSDK_LOG_DISK);



	//irsdkVar("ClutchRPM", &_carData.cl, irsdk_float, 1, "Clutch RPM", "", IRSDK_LOG_ALL);
	//irsdkVar("IsInGarage", &g_isInGarage, irsdk_bool, 1, "is car in garage", "", IRSDK_LOG_LIVE);
	//irsdkVar("IsOnTrack", &g_isOnTrack, irsdk_bool, 1, "1=Car on track physics running", "", IRSDK_LOG_ALL);

	memset(&_carData, 0, sizeof(CarData));
	memset(&_timingData, 0, sizeof(TimingData));
	memset(&_sessionData, 0, sizeof(SessionData));

	_sim = simName;
	checkFolderStructure();

	wstringstream wss;
	wss << _rootPath << "\\" << ODL_LOGFILE_NAME;
	wstring ws = wstring(wss.str().c_str());
	_log = ofstream(ws, std::ios_base::out);

	_irsdkSrv = irsdkServer::instance();
	
	stringstream ss;
	ss << "OpenDataLogger for " << simName << " started";
	string str = string(ss.str().c_str());

	writeToLog(str);
	str = string(_dataLogPath.begin(), _dataLogPath.end());
	writeToLog(str);
}

void COpenDataLogger::writeToLog(string& msg)
{
	time_t t;
	time(&t);

	tm lt;
	localtime_s(&lt, (const time_t*)&t);
	char ts[512];
	strftime(ts, 512, "[%H:%M:%S]: ", (const tm*)&lt);

	stringstream ss;
	ss << ts << msg << endl;

	string str = string(ss.str().c_str());
	_log.write(str.c_str(), str.length());
	_log.flush();
}

void COpenDataLogger::writeToLog(const char* msg)
{
	time_t t;
	time(&t);

	tm lt;
	localtime_s(&lt, (const time_t*)&t);
	char ts[512];
	strftime(ts, 512, "[%H:%M:%S]: ", (const tm*)&lt);

	stringstream ss;
	ss << ts << msg << endl;

	string str = string(ss.str().c_str());
	_log.write(str.c_str(), str.length());
	_log.flush();
}

COpenDataLogger::~COpenDataLogger()
{
	writeToLog(string("OpenDataLogger shutting down"));

	if (_log.is_open())
	{
		_log.flush();
		_log.close();
	}
}

void COpenDataLogger::UpdateData()
{
	// before going into the main data output loop, we must mark the headers as final
	// it is possible for finalizeHeader() to fail, so try every tick, just in case
	if (!_irsdkSrv->isHeaderFinalized())
		_irsdkSrv->finalizeHeader();

	// reset the data for this pass, if something is not updated
	// we will output zero for that tick ... and clear the garbage out of the vars if this is the first call
	if (_irsdkSrv->isInitialized())
		_irsdkSrv->resetSampleVars();

	if (_irsdkSrv->isHeaderFinalized())
		_irsdkSrv->pollSampleVars();
}

void COpenDataLogger::UpdateTelemetryInfo(CarData& data)
{
	memcpy(&_carData, &data, sizeof(CarData));

#ifdef _DEBUG
	stringstream ss;
	ss << "RPM: " << _carData.rpm;
	writeToLog(string(ss.str().c_str()));
#endif
}

void COpenDataLogger::UpdateSessionInfo(SessionData& data)
{
	memcpy(&_sessionData, &data, sizeof(SessionData));

#ifdef _DEBUG
	stringstream ss;
	ss << "time: " << _sessionData.sessionTime;
	writeToLog(string(ss.str().c_str()));
#endif

}

void COpenDataLogger::UpdateTimingInfo(TimingData& data)
{
	memcpy(&_timingData, &data, sizeof(TimingData));
}

void COpenDataLogger::UpdateSessionInfoString(SessionInfoStringData& data)
{

}

void COpenDataLogger::StartSession()
{

}

void COpenDataLogger::StopSession()
{

}

void COpenDataLogger::StartStint()
{
	if (!_irsdkSrv->isDiskLoggingEnabled())
		_irsdkSrv->toggleDiskLogging();
	// --- logging to disk enabled, so the next call to pollSampleVars opens a new file

}

void COpenDataLogger::StopStint()
{
	// write latest values to file
	_irsdkSrv->pollSampleVars();

	if (_irsdkSrv->isDiskLoggingEnabled()) // disk logging is turned on
		_irsdkSrv->toggleDiskLogging(); // turn logging on or off

	// logging to disk is diabled; call pollSampleVars once again to finalize and close the file
	_irsdkSrv->pollSampleVars();
}


void COpenDataLogger::checkFolderStructure()
{
	WCHAR* path = NULL;
	bool dirExists = false;

	HRESULT hr = SHGetKnownFolderPath(FOLDERID_Documents, KF_FLAG_DEFAULT_PATH | KF_FLAG_NOT_PARENT_RELATIVE, NULL, &path);
	if (SUCCEEDED(hr)) {

		std::wstringstream wss;
		wss << path << "\\" << ODL_ROOT_PATH;
		_rootPath = wstring(wss.str().c_str());


		if (!CreateDirectoryW((LPCWSTR)_rootPath.c_str(), NULL)) {

			if (GetLastError() != ERROR_ALREADY_EXISTS) {

				// TODO: throw exception
				//string err = "error creating ODL root path!";
				//_log.write(err.c_str(), err.length());
			}
		}
		wss << "\\" << ODL_DATALOG_PATH;
		_dataLogPath = wstring(wss.str().c_str());
		if (!CreateDirectoryW((LPCWSTR)_dataLogPath.c_str(), NULL)) {

			if (GetLastError() != ERROR_ALREADY_EXISTS) {

				// TODO: throw exception
				//string err = "error creating ODL data log path!";
				//_log.write(err.c_str(), err.length());
			}
		}
		wss << "\\" << _sim.c_str();
		_dataLogPath = wstring(wss.str().c_str());
		if (!CreateDirectoryW((LPCWSTR)_dataLogPath.c_str(), NULL)) {

			if (GetLastError() != ERROR_ALREADY_EXISTS) {

				// TODO: throw exception
				//string err = "error creating ODL data log path!";
				//_log.write(err.c_str(), err.length());
			}
		}

		CoTaskMemFree(path);
	}
}

void COpenDataLogger::SetSampleRate(int sampleRate)
{
	irsdkServer::sampleRate = sampleRate;

	stringstream ss;
	ss << "sample Rate: " << sampleRate;
	writeToLog(ss.str().c_str());
}

void COpenDataLogger::SetVehicleAndTrackName(string& vehicleName, string& trackName)
{
	_vehicleName = vehicleName;
	_trackName = trackName;

	string str = string(_dataLogPath.begin(), _dataLogPath.end());
	_irsdkSrv->SetDataLogPath(str.c_str());
	writeToLog(str.c_str());

	str = _vehicleName + "-" + _trackName;
	_irsdkSrv->SetDataLogFile(str.c_str());
	writeToLog(str.c_str());

#ifdef _DEBUG
	//writeToLog(_vehicleName);
	//writeToLog(_trackName);
	//writeToLog(str);
#endif
}
