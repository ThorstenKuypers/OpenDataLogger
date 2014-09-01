#include <cstring>
#include <cmath>
#include <stdio.h>
#include <assert.h>
#include <iomanip>

#include "yaml.h"

/*
// telemetry update
void rf2plugin::YAMLupdate(const TelemInfoV01 &info) {

}
*/

// scoring update
//void rf2plugin::YAMLupdate(const ScoringInfoV01 &info) {
//	// WeekendInfo
//	strncpy(weekendinfo.TrackName, info.mTrackName, sizeof(info.mTrackName));
//	weekendinfo.WeekendOptions.NumStarters = info.mNumVehicles;
//	weekendinfo.TrackLength = (float)info.mLapDist/1000;
//	weekendinfo.TrackID = djb2((unsigned char *)&info.mTrackName) & 0xFFFFFF;
//
//	if(info.mDarkCloud < 0.25)
//		strcpy (weekendinfo.TrackSkies, "Clear");
//	else if(info.mDarkCloud < 0.5)
//		strcpy (weekendinfo.TrackSkies, "Partly Cloudy");
//	else if(info.mDarkCloud < 0.5)
//		strcpy (weekendinfo.TrackSkies, "Mostly Cloudy");
//	else 
//		strcpy (weekendinfo.TrackSkies, "Overcast");
//
//	// mRaining not used
//	weekendinfo.TrackAirTemp = (float)info.mAmbientTemp;
//	weekendinfo.TrackSurfaceTemp = (float)info.mTrackTemp;
//
//	// Wind isn't actually enabled, meh...
//	weekendinfo.TrackWindVel = sqrtf((info.mWind.x * info.mWind.x) + (info.mWind.y * info.mWind.y));
//	// direction A·B = ||A|| ||B|| cos theta
//	if(weekendinfo.TrackWindVel > 0.0)
//		weekendinfo.TrackWindDir = acosf((info.mWind.x + info.mWind.y) / (sqrtf((info.mWind.x * info.mWind.x) + (info.mWind.y * info.mWind.y)) * sqrtf(1)));
//	else {
//		weekendinfo.TrackWindDir = 0.0;
//	}
//
//	// mOnPathWetness not used
//	// mOffPathWetness not used
//
//	// SessionInfo
//	sessioninfo.Sessions[info.mSession].SessionNum = info.mSession;
//	sessioninfo.Sessions[info.mSession].SessionTime = (float)info.mEndET;
//
//	switch (info.mSession) // (0=testday 1-4=practice 5-8=qual 9=warmup 10-13=race)
//	{
//		case 0:
//		case 1:
//		case 2:
//		case 3:
//		case 4:
//			strcpy (sessioninfo.Sessions[info.mSession].SessionType, "Practice");
//			break;
//		case 5:
//		case 6:
//		case 7:
//		case 8:
//			strcpy (sessioninfo.Sessions[info.mSession].SessionType, "Qualify");
//			break;
//		case 9:
//			strcpy (sessioninfo.Sessions[info.mSession].SessionType, "Warmup");
//			break;
//		case 10:
//		case 11:
//		case 12:
//		case 13:
//			strcpy (sessioninfo.Sessions[info.mSession].SessionType, "Race");
//			break;
//		default:
//			break;
//	}
//
//	if(info.mMaxLaps >= 2147483647)
//		strcpy(sessioninfo.Sessions[info.mSession].SessionLaps, "unlimited");
//	else
//		sprintf(sessioninfo.Sessions[info.mSession].SessionLaps, "%i", info.mMaxLaps);
//	
//	sessioninfo.Sessions[info.mSession].ResultsFastestLap.FastestTime = 999999.9f;
//
//	// DriverInfo
//	for( long i = 0; i < info.mNumVehicles; ++i )
//    {
//		VehicleScoringInfoV01 &vinfo = info.mVehicle[i];
//		if((float)vinfo.mBestLapTime < sessioninfo.Sessions[info.mSession].ResultsFastestLap.FastestTime) {
//			sessioninfo.Sessions[info.mSession].ResultsFastestLap.CarIdx = vinfo.mID;
//			sessioninfo.Sessions[info.mSession].ResultsFastestLap.FastestTime = (float)vinfo.mBestLapTime;
//			//sessioninfo.Sessions[info.mSession].ResultsFastestLap.FastestLap = ; dunno
//		}
//
//		sessioninfo.Sessions[info.mSession].ResultsPositions[i].CarIdx = vinfo.mID;
//		sessioninfo.Sessions[info.mSession].ResultsPositions[i].Position = vinfo.mPlace;
//		sessioninfo.Sessions[info.mSession].ResultsPositions[i].ClassPosition = vinfo.mPlace; // TODO
//		sessioninfo.Sessions[info.mSession].ResultsPositions[i].FastestTime = (float)vinfo.mBestLapTime;
//		sessioninfo.Sessions[info.mSession].ResultsPositions[i].Lap = vinfo.mTotalLaps;
//		sessioninfo.Sessions[info.mSession].ResultsPositions[i].LastTime = (float)vinfo.mLastLapTime;
//		sessioninfo.Sessions[info.mSession].ResultsPositions[i].LapsComplete = vinfo.mTotalLaps;
//		sessioninfo.Sessions[info.mSession].ResultsPositions[i].LapsDriven = vinfo.mTotalLaps; //(float)((double)vinfo.mTotalLaps + (vinfo.mLapDist/info.mLapDist));
//		sessioninfo.Sessions[info.mSession].ResultsPositions[i].ReasonOutId = (int)vinfo.mFinishStatus;
//
//		switch (vinfo.mFinishStatus) // 0=none, 1=finished, 2=dnf, 3=dq
//		{
//			case 0:
//				strcpy(sessioninfo.Sessions[info.mSession].ResultsPositions[i].ReasonOutStr, "Running");
//				break;
//			case 1:
//				strcpy(sessioninfo.Sessions[info.mSession].ResultsPositions[i].ReasonOutStr, "Finished");
//				break;
//			case 2:
//				strcpy(sessioninfo.Sessions[info.mSession].ResultsPositions[i].ReasonOutStr, "DNF");
//				break;
//			case 3:
//				strcpy(sessioninfo.Sessions[info.mSession].ResultsPositions[i].ReasonOutStr, "Disqualified");
//				break;
//			default:
//				break;
//		}
//
//		driverinfo.Drivers[i].CarIdx = vinfo.mID;
//		driverinfo.Drivers[i].UserID = djb2((unsigned char *)&vinfo.mDriverName) & 0xFFFFFF;
//		driverinfo.Drivers[i].CarID = djb2((unsigned char *)&vinfo.mVehicleName) & 0xFFFFFF;
//		driverinfo.Drivers[i].CarClassID = djb2((unsigned char *)&vinfo.mVehicleClass) & 0xFFFFFF;
//		strcpy(driverinfo.Drivers[i].UserName, vinfo.mDriverName);
//		strcpy(driverinfo.Drivers[i].CarPath, vinfo.mVehicleName);
//		strcpy(driverinfo.Drivers[i].CarClassShortName, vinfo.mVehicleClass);
//
//		if(vinfo.mIsPlayer) {
//			driverinfo.DriverCarIdx = vinfo.mID;
//		}
//	}
//	
//	YAMLgenerate();
//}

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
