#pragma once

#include <fstream>
#include <sstream>
#include <vector>
#include <ctime>
#include <typeinfo>
#include <Windows.h>

#include "..\..\src\OpenGameAPI\irsdk_defines.h"
#include "..\..\src\OpenGameAPI\irsdk_server.h"
#include "DebugLog.h"

#ifndef UINT
typedef unsigned int UINT;
#endif

using namespace std;

#define ODL_ROOT_PATH		"OpenDataLogger"
#define ODL_DATALOG_PATH	"LOG"
#define ODL_LOGFILE_NAME	"odl.log" // default logfile name

namespace OpenDataLogger {

	enum WheelPosition {
		LF = 0,
		RF,
		LR,
		RR
	};

	///
	/// configuration data
	///
	struct PluginConfig
	{
		string logfilePath;
		bool activateOnStartup;
		char activationKey;
		unsigned char activationKeyModifier;

		char sampleRate;
		bool logAeroData;
		bool logExtendedWheelData;

		// ...
	};

	struct WheelData {
		float brakeLinePress;
		float speed;
		float pressure;
		float tempL;
		float tempM;
		float tempR;
		float wearL;
		float wearM;
		float wearR;
		float rideHeight;

	};

	struct CarData
	{
		bool isOnPitRoad;
		float steeringWheelAngle;
		float throttle;
		float brake;
		float clutch;
		int gear;
		float rpm;
		int lap;
		float lapDist;
		float lapDistPct;
		float longAccel;
		float latAccel;
		float vertAccel;
		float rollRate;
		float pitchRate;
		float yawRate;
		float speed;
		float velocity[3]; // X-Y-Z
		float yaw;
		float pitch;
		float roll;
		float latPos;
		float longPos;
		float altPos;
		bool isOnTrack;
		UINT engineWarnings;
		float fuelLevel;
		float fuelLevelPct;
		float brakeBias;
		float tractionControl;
		float waterTemp;
		float waterLevel;
		float fuelPress;
		float oilTemp;
		float oilPress;
		float oilLevel;
		float voltage;
		float manifoldPress;
		float cfsRideHeight;

		WheelData wheels[4];
	};

	struct SessionData
	{
		double sessionTime;
		UINT sessionState;
		float sessionTimeLeft;
		int sessionLapsLeft;
		float trackTemp;
		float airTemp;

	};

	struct TimingData
	{
		float LapBestLap;
		float LapBestLapTime;
		float LapLastLapTime;
		float LapCurrentLapTime;
		float LapDeltaToBestLap;
		float LapDeltaToBestLap_DD;
		//float LapDeltaToBestLap_OK;
		float LapDeltaToOptimalLap;
		float LapDeltaToOptimalLap_DD;
		//float LapDeltaToOptimalLap_OK;
		float LapDeltaToSessionBestLap;
		float LapDeltaToSessionBestLap_DD;
		//float LapDeltaToSessionBestLap_OK;
		float LapDeltaToSessionOptimalLap;
		float LapDeltaToSessionOptimalLap_DD;
		//float LapDeltaToSessionOptimalLap_OK;
		float LapDeltaToSessionLastlLap;
		float LapDeltaToSessionLastlLap_DD;
		//float LapDeltaToSessionLastlLap_OK;

	};

	struct sectorInfo
	{
		int num;
		float startPct;
	};

	struct SessionInfoResultPosition
	{
		long carIdx;
		int position;
		string driverName;
		string carName;
		string className;
		float fastestLapTime;
		int fastestLap;

	};

	struct SessionInfoStringData
	{
		string trackName;
		float skies;
		float trackTemp;
		float ambientTemp;
		float windVel;
		long sessionID;
		float sessionTime;
		int sessionLaps;
		float resultsFastestLapTime;
		int resultsLapCompleted;

		int playerCarIdx;
		float maxEngineRpm;

		int numCars;
		SessionInfoResultPosition resultsPos[1]; // results info array

		int numSectors;
		sectorInfo* sectors;	// sectors info array
	};


	class COpenDataLogger
	{
	public:
		COpenDataLogger(string& simName);
		COpenDataLogger() {}
		~COpenDataLogger();

		void UpdateTelemetryInfo(CarData& data);
		void UpdateSessionInfo(SessionData& data);
		void UpdateTimingInfo(TimingData& data);
		void UpdateData();
		void UpdateSessionInfoString(SessionInfoStringData& data);
		void UpdateSessionInfoString(std::string sessionStr);

		void StartSession();
		void StopSession();

		void StartStint();
		void StopStint();

		void SetVehicleAndTrackName(string& vehicleName, string& trackName);

		void SetSampleRate(int sampleRate);
		int GetSampleRate() {}
		void SetTrackLength(float length) { _trackLength = length; }

		std::wstring& GetLoggerRootPath() { return _rootPath; }

	private:

		// checks if the ODL folder structure exists
		void checkFolderStructure();

		// load configuration data from ini file
		void loadPluginConfigFromFile(string filename);

		CarData _carData;
		SessionData _sessionData;
		TimingData _timingData;

		//irsdkServer* _irsdkSrv; // instance handle of irsdkServer

		ofstream _log;	// error / debug logfile
		void writeToLog(string& msg);
		void writeToLog(const char* msg);

		string _sim; // name of sim
		wstring _rootPath;	// ODL root path
		wstring _dataLogPath; // directory to which data log files are written (depending on sim)

		string _trackName;
		string _vehicleName;

		float _trackLength;
	};

}