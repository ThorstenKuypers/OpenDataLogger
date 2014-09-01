#ifndef YAML_H
#define YAML_H

#include <string>
#include <sstream>

#include "..\\OpenGameAPI\\irsdk_server.h"

using namespace std;

// Macros
#define YPRINT(...)  len += _snprintf(YAMLstring+len, YAMLstring_maxlen-len, __VA_ARGS__)

// limits
#define MAX_CARS 64
#define MAX_SESSIONS 14

// YAML structs
struct WeekendOptions {
	int NumStarters;
	char StartingGrid[IRSDK_MAX_STRING];
	char QualifyScoring[IRSDK_MAX_STRING];		// enum
	char CourseCautions[IRSDK_MAX_STRING];		// enum
	int StandingStart;
	char Restarts[IRSDK_MAX_STRING];			// enum
	char WeatherType[IRSDK_MAX_STRING];			// enum
	char Skies[IRSDK_MAX_STRING];				// enum
	char WindDirection[IRSDK_MAX_STRING];		// enum
	float WindSpeed;
	float WeatherTemp;
	int RelativeHumidity;
	int FogLevel;
	int Unofficial;
	char CommercialMode[IRSDK_MAX_STRING];		// enum
	int NightMode;
	int isFixedSetup;
	char StrictLapsChecking[IRSDK_MAX_STRING];	// enum
	int HasOpenRegistration;
	int HardcoreLevel;
};

struct SectorInfo {
	int num;
	float LapDistPct;
};

struct WeekendInfo {
	char TrackName[IRSDK_MAX_STRING];
	unsigned int TrackID;
	float TrackLength;
	char TrackDisplayName[IRSDK_MAX_STRING];
	char TrackDisplayShortName[IRSDK_MAX_STRING];
	char TrackCity[IRSDK_MAX_STRING];
	char TrackCountry[IRSDK_MAX_STRING];
	float TrackAltitude;
	double TrackLatitude;
	double TrackLongitude;
	int TrackNumTurns;
	float TrackPitSpeedLimit;
	char TrackType[IRSDK_MAX_STRING];			// enum
	char TrackWeatherType[IRSDK_MAX_STRING];	// enum
	char TrackSkies[IRSDK_MAX_STRING];			// enum
	float TrackSurfaceTemp;
	float TrackAirTemp;
	float TrackAirPressure;
	float TrackWindVel;
	float TrackWindDir;
	int TrackRelativeHumidity;
	int TrackFogLevel;
	int SeriesID;
	int SeasonID;
	int SessionID;
	int SubSessionID;
	int LeagueID;
	int Offcial;
	int RaceWeek;
	char EventType[IRSDK_MAX_STRING];			// enum
	char Category[IRSDK_MAX_STRING];			// enum
	char SimMode[IRSDK_MAX_STRING];				// enum
	WeekendOptions WeekendOptions;

	//int numOfSectors;
	//SectorInfo sectors[1]; // array of numOfSectors
};

struct ResultPosition {
	int Position;
	int ClassPosition;
	int CarIdx;
	int Lap;
	float Time;			// laptime with 10ms accuracy can be 8388 seconds using floats, enough I guess
	int FastestLap;
	float FastestTime;
	float LastTime;
	int LapsLed;
	int LapsComplete;
	float LapsDriven;
	int Incidents;
	int ReasonOutId;
	char ReasonOutStr[IRSDK_MAX_STRING];	
};

struct ResultsFastestLap {
	int CarIdx;
	int FastestLap;
	float FastestTime;
};

struct Session {
	int SessionNum;
	char SessionLaps[IRSDK_MAX_STRING];
	float SessionTime;
	int SessionNumLapsToAvg;
	char SessionType[IRSDK_MAX_STRING];
	ResultPosition ResultsPositions[MAX_CARS];
	ResultsFastestLap ResultsFastestLap;
	float ResultsAverageLapTime;
	int ResultsNumCautionFlags;
	int ResultsNumCautionLaps;
	int ResultsNumLeadChanges;
	int ResultsLapsComplete;
	int ResultsOfficial;
};

struct SessionInfo {
	Session Sessions[MAX_SESSIONS]; // see ScoringInfoV01.mSession (0=testday 1-4=practice 5-8=qual 9=warmup 10-13=race)
};

struct Driver {
	int CarIdx;
	char UserName[IRSDK_MAX_STRING];
	char AbbrevName[IRSDK_MAX_STRING];
	char Initials[IRSDK_MAX_STRING];
	int UserID;
	int CarNumber;
	char CarPath[IRSDK_MAX_STRING];
	int CarClassID;
	int CarID;
	char CarClassShortName[IRSDK_MAX_STRING];
	int CarClassRelSpeed;
	int CarClassLicenseLevel;
	float CarClassMaxFuel;
	float CarClassWeightPenalty;
	int IRating;
	int LicLevel;
	int LicSubLevel;
	char LicColor[IRSDK_MAX_STRING];
	char ClubName[IRSDK_MAX_STRING];
	char DivisionName[IRSDK_MAX_STRING];
};

struct DriverInfo {
	int DriverCarIdx;
	float DriverHeadPosX;
	float DriverHeadPosY;
	float DriverHeadPosZ;
	float DriverCarRedLine;
	float DriverCarFuelKgPerLtr;
	float DriverCarSLFirstRPM;
	float DriverCarSLShiftRPM;
	float DriverCarSLLastRPM;
	float DriverCarSLBlinkRPM;
	float DriverPitTrkPct;
	Driver Drivers[MAX_CARS];
};


class CYaml
{
public:
	CYaml();
	~CYaml();

	static int numCars;

	static void GenerateSessionInfo(WeekendInfo& weekendInfo, SessionInfo& sessionInfo, DriverInfo& driverInfo, std::string* sessionInfoStr);

private:
};

#endif //YAML_H
