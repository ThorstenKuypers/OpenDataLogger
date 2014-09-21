#include <cstring>
#include <cmath>
#include <iomanip>

#include "yaml.h"


void CYaml::GenerateSessionInfo(WeekendInfo& weekendinfo, SessionInfo& sessioninfo, DriverInfo& driverinfo, std::string* sessionInfoStr)
{
	int len = 0;
	std::stringstream yamlStr;

	yamlStr << "---\n";

	// WeekendInfo (current/latest data)
	yamlStr << "WeekendInfo:" << endl;
	yamlStr << " TrackName: " << weekendinfo.TrackName << endl;
	yamlStr << " TrackID: " << weekendinfo.TrackID << endl;
	yamlStr << setprecision(2) << fixed << " TrackLength: " << weekendinfo.TrackLength << " km" << endl;
	yamlStr << " TrackDisplayName: " << weekendinfo.TrackName << endl;
	yamlStr << " TrackDisplayShortName: " << weekendinfo.TrackName << endl;
	yamlStr << " TrackCity: " << weekendinfo.TrackCity << endl;
	yamlStr << " TrackCountry: " << weekendinfo.TrackCountry << endl;
	yamlStr << setprecision(2) << fixed << " TrackAltitude: " << weekendinfo.TrackAltitude << endl;
	yamlStr << " TrackLatitude: " << weekendinfo.TrackLatitude << endl;
	yamlStr << " TrackLongitude: " << weekendinfo.TrackLongitude << endl;
	yamlStr << " TrackNumTurns: " << weekendinfo.TrackNumTurns << endl;
	yamlStr << " TrackPitSpeedLimit: " << weekendinfo.TrackPitSpeedLimit << endl;
	yamlStr << " TrackType: " << weekendinfo.TrackType << endl;
	yamlStr << " TrackWeatherType: " << weekendinfo.TrackWeatherType << endl;
	yamlStr << " TrackSkies:" << weekendinfo.TrackSkies << endl;
	yamlStr << setprecision(2) << fixed << " TrackSurfaceTemp: " << weekendinfo.TrackSurfaceTemp << " C" << endl;
	yamlStr << setprecision(2) << fixed << " TrackAirTemp: " << weekendinfo.TrackAirTemp << " C" << endl;
	yamlStr << setprecision(2) << fixed << " TrackAirPressure: \n" << weekendinfo.TrackAirPressure << " Hg" << endl;
	yamlStr << setprecision(2) << fixed << " TrackWindVel: " << weekendinfo.TrackWindVel << " m/s" << endl;
	yamlStr << setprecision(2) << fixed << " TrackWindDir: " << weekendinfo.TrackWindDir << " rad" << endl;
	yamlStr << " TrackRelativeHumidity: " << weekendinfo.TrackRelativeHumidity << " %" << endl;
	yamlStr << " TrackFogLevel: " << weekendinfo.TrackFogLevel << " %" << endl;
	yamlStr << " SeriesID: " << weekendinfo.SeriesID << endl;
	yamlStr << " SeasonID: " << weekendinfo.SeasonID << endl;
	yamlStr << " SessionID: " << weekendinfo.SessionID << endl;
	yamlStr << " SubSessionID: " << weekendinfo.SubSessionID << endl;
	yamlStr << " LeagueID: " << weekendinfo.LeagueID << endl;
	yamlStr << " Official: " << weekendinfo.Offcial << endl;
	yamlStr << " RaceWeek: " << weekendinfo.RaceWeek << endl;
	yamlStr << " EventType: " << weekendinfo.EventType << endl;
	yamlStr << " Category: " << weekendinfo.Category << endl;
	yamlStr << " SimMode: " << weekendinfo.SimMode << endl;

		// WeekendOptions <<server settings when created<<
	yamlStr << " WeekendOptions:" << endl;
	yamlStr << "  NumStarters: " << weekendinfo.WeekendOptions.NumStarters << endl;
	yamlStr << "  StartingGrid: " << weekendinfo.WeekendOptions.StandingStart << endl;
	yamlStr << "  QualifyScoring: " << weekendinfo.WeekendOptions.QualifyScoring << endl;
	yamlStr << "  CourseCautions: " << weekendinfo.WeekendOptions.CourseCautions << endl;
	yamlStr << "  StandingStart: " << weekendinfo.WeekendOptions.StandingStart << endl;
	yamlStr << "  Restarts: " << weekendinfo.WeekendOptions.Restarts << endl;
	yamlStr << "  WeatherType: " << weekendinfo.WeekendOptions.WeatherType << endl;
	yamlStr << "  Skies: " << weekendinfo.WeekendOptions.Skies << endl;
	yamlStr << "  WindDirection: " << weekendinfo.WeekendOptions.WindDirection << endl;
	yamlStr << "  WindSpeed: " << weekendinfo.WeekendOptions.WindSpeed << " m/s" << endl;
	yamlStr << "  WeatherTemp: " << weekendinfo.WeekendOptions.WeatherTemp << " rad" << endl;
	yamlStr << "  RelativeHumidity: " << weekendinfo.WeekendOptions.RelativeHumidity << " %" << endl;
	yamlStr << "  FogLevel: " << weekendinfo.WeekendOptions.FogLevel << " %" << endl;
	yamlStr << "  Unofficial: " << weekendinfo.WeekendOptions.Unofficial << endl;
	yamlStr << "  CommercialMode: " << weekendinfo.WeekendOptions.CommercialMode << endl;
	yamlStr << "  NightMode: " << weekendinfo.WeekendOptions.NightMode << endl;
	yamlStr << "  IsFixedSetup: " << weekendinfo.WeekendOptions.isFixedSetup << endl;
	yamlStr << "  StrictLapsChecking: " << weekendinfo.WeekendOptions.StrictLapsChecking << endl;
	yamlStr << "  HasOpenRegistration: " << weekendinfo.WeekendOptions.HasOpenRegistration << endl;
	yamlStr << "  HardcoreLevel: " << weekendinfo.WeekendOptions.HardcoreLevel << endl;
	yamlStr << endl;

		// SessionInfo
	yamlStr << "SessionInfo:" << endl;
	yamlStr << " Sessions:" << endl;
	for (int i = 0; i < weekendinfo.WeekendOptions.NumStarters; i++) {
		if (sessioninfo.Sessions[i].SessionTime > 0.0f) {
			yamlStr << " - SessionNum: " << sessioninfo.Sessions[i].SessionNum << endl;
			yamlStr << "   SessionLaps: " << sessioninfo.Sessions[i].SessionLaps << endl;
			yamlStr << setprecision(2) << fixed << "   SessionTime: " << sessioninfo.Sessions[i].SessionTime << endl;
			yamlStr << "   SessionNumLapsToAvg: " << sessioninfo.Sessions[i].SessionNumLapsToAvg << endl;
			yamlStr << "   SessionType: " << sessioninfo.Sessions[i].SessionType << endl;
			yamlStr << setprecision(3) << fixed << "   ResultsAverageLapTime: " << sessioninfo.Sessions[i].ResultsAverageLapTime << endl;
			yamlStr << "   ResultsNumCautionFlags: " << sessioninfo.Sessions[i].ResultsNumCautionFlags << endl;
			yamlStr << "   ResultsNumCautionLaps: " << sessioninfo.Sessions[i].ResultsNumCautionLaps << endl;
			yamlStr << "   ResultsNumLeadChanges: " << sessioninfo.Sessions[i].ResultsNumLeadChanges << endl;
			yamlStr << "   ResultsLapsComplete: " << sessioninfo.Sessions[i].ResultsLapsComplete << endl;
			yamlStr << "   ResultsOfficial: " << sessioninfo.Sessions[i].ResultsOfficial << endl;
			yamlStr << "   ResultsPositions:" << endl;
			for (int j = 0; j < weekendinfo.WeekendOptions.NumStarters; j++) {
				if (sessioninfo.Sessions[i].ResultsPositions[j].CarIdx > -1) {
					yamlStr << "   - Position: " << sessioninfo.Sessions[i].ResultsPositions[j].Position << endl;
					yamlStr << "     ClassPosition: " << sessioninfo.Sessions[i].ResultsPositions[j].ClassPosition << endl;
					yamlStr << "     CarIdx: " << sessioninfo.Sessions[i].ResultsPositions[j].CarIdx << endl;
					yamlStr << "     Lap: " << sessioninfo.Sessions[i].ResultsPositions[j].Lap << endl;
					yamlStr << setprecision(3) << fixed << "     Time: " << sessioninfo.Sessions[i].ResultsPositions[j].Time << endl;
					yamlStr << "     FastestLap: " << sessioninfo.Sessions[i].ResultsPositions[j].FastestLap << endl;
					yamlStr << setprecision(3) << fixed << "     FastestTime: " << sessioninfo.Sessions[i].ResultsPositions[j].FastestTime << endl;
					yamlStr << setprecision(3) << fixed << "     LastTime: " << sessioninfo.Sessions[i].ResultsPositions[j].LastTime << endl;
					yamlStr << "     LapsLed: " << sessioninfo.Sessions[i].ResultsPositions[j].LapsLed << endl;
					yamlStr << "     LapsComplete:" << sessioninfo.Sessions[i].ResultsPositions[j].LapsComplete << endl;
					yamlStr << setprecision(3) << fixed << "     LapsDriven: " << sessioninfo.Sessions[i].ResultsPositions[j].LapsDriven << endl;
					yamlStr << "     Incidents: " << sessioninfo.Sessions[i].ResultsPositions[j].Incidents << endl;
					yamlStr << "     ReasonOutId: " << sessioninfo.Sessions[i].ResultsPositions[j].ReasonOutId << endl;
					yamlStr << "     ReasonOutStr: " << sessioninfo.Sessions[i].ResultsPositions[j].ReasonOutStr << endl;
				}
			}
			yamlStr << "   ResultsFastestLap:" << endl;
			yamlStr << "   - CarIdx: " << sessioninfo.Sessions[i].ResultsFastestLap.CarIdx << endl;
			yamlStr << "     FastestLap: " << sessioninfo.Sessions[i].ResultsFastestLap.FastestLap << endl;
			yamlStr << setprecision(3) << fixed << "     FastestTime:" << sessioninfo.Sessions[i].ResultsFastestLap.FastestTime << endl;
		}
	}
	yamlStr << endl;

		// Camera info <<static<< bare minimum definition<<
	//yamlStr << "CameraInfo:" << endl;
	//yamlStr << " Groups:" << endl;
	//	yamlStr << " - GroupNum: 1\n" << endl
	//	yamlStr << "   GroupName: Rollbar\n" << endl
	//	yamlStr << " - GroupNum: 2\n" << endl
	//	yamlStr << "   GroupName: Cockpit\n" << endl
	//	yamlStr << " - GroupNum: 3\n" << endl
	//	yamlStr << "   GroupName: Nose\n" << endl
	//	yamlStr << " - GroupNum: 4\n" << endl
	//	yamlStr << "   GroupName: Chase\n" << endl
	//	yamlStr << " - GroupNum: 5\n" << endl
	//	yamlStr << "   GroupName: TV\n" << endl
	//	yamlStr << " - GroupNum: 6\n" << endl
	//	yamlStr << "   GroupName: unknown\n" << endl
	//	yamlStr << " - GroupNum: 7\n" << endl
	//	yamlStr << "   GroupName: FrontBumper\n" << endl
	//	yamlStr << " - GroupNum: 8\n" << endl
	//	yamlStr << "   GroupName: RearBumper\n" << endl
	//	yamlStr << " - GroupNum: 9\n" << endl
	//	yamlStr << "   GroupName: Side\n" << endl
	//	yamlStr << " - GroupNum: 10\n" << endl
	//	yamlStr << "   GroupName: Rear\n" << endl
	//	yamlStr << "\n" << endl

		// SessionInfo
	yamlStr << "DriverInfo:" << endl;
	yamlStr << " DriverCarIdx: " << driverinfo.DriverCarIdx << endl;
		//yamlStr << " DriverHeadPosX: %4.3f\n" << driverinfo.DriverHeadPosX << endl
		//yamlStr << " DriverHeadPosY: %4.3f\n" << driverinfo.DriverHeadPosY << endl
		//yamlStr << " DriverHeadPosZ: %4.3f\n" << driverinfo.DriverHeadPosZ << endl
	yamlStr << setprecision(3) << fixed << " DriverCarRedLine: " << driverinfo.DriverCarRedLine << endl;
	yamlStr << setprecision(3) << fixed << " DriverCarFuelKgPerLtr: " << driverinfo.DriverCarFuelKgPerLtr << endl;
	yamlStr << setprecision(3) << fixed << " DriverCarSLFirstRPM: " << driverinfo.DriverCarSLFirstRPM << endl;
	yamlStr << setprecision(3) << fixed << " DriverCarSLShiftRPM: " << driverinfo.DriverCarSLShiftRPM << endl;
	yamlStr << setprecision(3) << fixed << " DriverCarSLLastRPM: " << driverinfo.DriverCarSLLastRPM << endl;
	yamlStr << setprecision(3) << fixed << " DriverCarSLBlinkRPM: " << driverinfo.DriverCarSLBlinkRPM << endl;
	yamlStr << setprecision(3) << fixed << " DriverPitTrkPct: " << driverinfo.DriverPitTrkPct << endl;
	yamlStr << " Drivers:" << endl;
	for (int i = 0; i < weekendinfo.WeekendOptions.NumStarters; i++) {
		if (driverinfo.Drivers[i].CarIdx > -1) {
			yamlStr << " - CarIdx: " << driverinfo.Drivers[i].CarIdx << endl;
			yamlStr << "   UserName: " << driverinfo.Drivers[i].UserName << endl;
			yamlStr << "   AbbrevName: " << driverinfo.Drivers[i].AbbrevName << endl;
			yamlStr << "   Initials: " << driverinfo.Drivers[i].Initials << endl;
			yamlStr << "   UserID: " << driverinfo.Drivers[i].UserID << endl;
			yamlStr << "   CarNumber: " << driverinfo.Drivers[i].CarNumber << endl;
			yamlStr << "   CarPath: " << driverinfo.Drivers[i].CarPath << endl;
			yamlStr << "   CarClassID: " << driverinfo.Drivers[i].CarClassID << endl;
			yamlStr << "   CarID: " << driverinfo.Drivers[i].CarID << endl;
			yamlStr << "   CarClassShortName: " << driverinfo.Drivers[i].CarClassShortName << endl;
			yamlStr << "   CarClassRelSpeed: " << driverinfo.Drivers[i].CarClassRelSpeed << endl;
			yamlStr << "   CarClassLicenseLevel: " << driverinfo.Drivers[i].CarClassLicenseLevel << endl;
			yamlStr << "   CarClassMaxFuel: " << driverinfo.Drivers[i].CarClassMaxFuel << " %%" << endl;
			yamlStr << "   CarClassWeightPenalty: " << driverinfo.Drivers[i].CarClassWeightPenalty << " kg" << endl;
			yamlStr << "   IRating: " << driverinfo.Drivers[i].IRating << endl;
			yamlStr << "   LicLevel: " << driverinfo.Drivers[i].LicLevel << endl;
			yamlStr << "   LicSubLevel: " << driverinfo.Drivers[i].LicSubLevel << endl;
			yamlStr << "   LicColor: " << driverinfo.Drivers[i].LicColor << endl;
			yamlStr << "   ClubName: " << driverinfo.Drivers[i].ClubName << endl;
			yamlStr << "   DivisionName: " << driverinfo.Drivers[i].DivisionName << endl;
		}
	}
	yamlStr << endl;

		// Splits
	yamlStr << "SplitTimeInfo:" << endl;
	yamlStr << " Sectors:" << endl;
	yamlStr << " - SectorNum: 0" << endl;
	//yamlStr << setprecision(3) << fixed << "   SectorStartPct: " << m_sectorPos[1] << endl;
	yamlStr << " - SectorNum: 1" << endl;
	//yamlStr << setprecision(3) << fixed << "   SectorStartPct: " << m_sectorPos[2] << endl;
	yamlStr << " - SectorNum: 2" << endl;
	//yamlStr << setprecision(3) << fixed << "   SectorStartPct: " << m_sectorPos[0] << endl;
	yamlStr << endl;

	if (sessionInfoStr != nullptr)
	{
		*sessionInfoStr = yamlStr.str();
	}
}
