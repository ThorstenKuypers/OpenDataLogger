#ifndef YAML_H
#define YAML_H
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

#include <string>
#include <sstream>

#include "..\\OpenGameAPI\\irsdk_server.h"

using namespace std;

// limits
#define MAX_CARS 64
#define MAX_SESSIONS 14

// YAML structs
struct WeekendOptions {
	int NumStarters;
	//char StartingGrid[IRSDK_MAX_STRING];
	std::string StartingGrid;
	//char QualifyScoring[IRSDK_MAX_STRING];		// enum
	std::string QualifyScoring;
	//char CourseCautions[IRSDK_MAX_STRING];		// enum
	std::string CourseCautions;
	int StandingStart;
	//char Restarts[IRSDK_MAX_STRING];			// enum
	std::string Restarts;
	//char WeatherType[IRSDK_MAX_STRING];			// enum
	std::string WeatherType;
	//char Skies[IRSDK_MAX_STRING];				// enum
	std::string Skies;
	//char WindDirection[IRSDK_MAX_STRING];		// enum
	std::string WindDirection;
	float WindSpeed;
	float WeatherTemp;
	int RelativeHumidity;
	int FogLevel;
	int Unofficial;
	//char CommercialMode[IRSDK_MAX_STRING];		// enum
	std::string CommercialMode;
	int NightMode;
	int isFixedSetup;
	//char StrictLapsChecking[IRSDK_MAX_STRING];	// enum
	std::string StrictLapsChecking;
	int HasOpenRegistration;
	int HardcoreLevel;
};

struct SectorInfo {
	int num;
	float LapDistPct;
};

struct WeekendInfo {
	//char TrackName[IRSDK_MAX_STRING];
	std::string TrackName;
	unsigned int TrackID;
	float TrackLength;
	//char TrackDisplayName[IRSDK_MAX_STRING];
	std::string TrackDisplayName;
	//char TrackDisplayShortName[IRSDK_MAX_STRING];
	std::string TrackDisplayShortName;
	//char TrackCity[IRSDK_MAX_STRING];
	std::string TrackCity;
	//char TrackCountry[IRSDK_MAX_STRING];
	std::string TrackCountry;
	float TrackAltitude;
	double TrackLatitude;
	double TrackLongitude;
	int TrackNumTurns;
	float TrackPitSpeedLimit;
	//char TrackType[IRSDK_MAX_STRING];			// enum
	std::string TrackType;
	//char TrackWeatherType[IRSDK_MAX_STRING];	// enum
	std::string TrackWeatherType;
	//char TrackSkies[IRSDK_MAX_STRING];			// enum
	std::string TrackSkies;
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
	//char EventType[IRSDK_MAX_STRING];			// enum
	std::string EventType;
	//char Category[IRSDK_MAX_STRING];			// enum
	std::string Category;
	//char SimMode[IRSDK_MAX_STRING];				// enum
	std::string SimMode;
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
	//char ReasonOutStr[IRSDK_MAX_STRING];
	std::string ReasonOutStr;
};

struct ResultsFastestLap {
	int CarIdx;
	int FastestLap;
	float FastestTime;
};

struct Session {
	int SessionNum;
	//char SessionLaps[IRSDK_MAX_STRING];
	std::string SessionLaps;
	float SessionTime;
	int SessionNumLapsToAvg;
	//char SessionType[IRSDK_MAX_STRING];
	std::string SessionType;
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
	//char UserName[IRSDK_MAX_STRING];
	std::string UserName;
	//char AbbrevName[IRSDK_MAX_STRING];
	std::string AbbrevName;
	//char Initials[IRSDK_MAX_STRING];
	std::string Initials;
	int UserID;
	int CarNumber;
	//char CarPath[IRSDK_MAX_STRING];
	std::string CarPath;
	int CarClassID;
	int CarID;
	//char CarClassShortName[IRSDK_MAX_STRING];
	std::string CarClassShortName;
	int CarClassRelSpeed;
	int CarClassLicenseLevel;
	float CarClassMaxFuel;
	float CarClassWeightPenalty;
	int IRating;
	int LicLevel;
	int LicSubLevel;
	//char LicColor[IRSDK_MAX_STRING];
	std::string LicColor;
	//char ClubName[IRSDK_MAX_STRING];
	std::string ClubName;
	//char DivisionName[IRSDK_MAX_STRING];
	std::string DivisionName;
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
