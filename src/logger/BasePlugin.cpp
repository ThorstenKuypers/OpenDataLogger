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

#include "BasePlugin.h"

using namespace OpenDataLogger;

CBasePlugin::CBasePlugin()
{

}

///
/// to prevent a delay, during sound playback, on the main thread
/// the playback call is done on a seperate thread
///
DWORD WINAPI CBasePlugin::SoundPlaybackThreadRoutine(void* param)
{
	_thData* data = (_thData*)param;
	std::wstring dir;

	if (data != nullptr) {
		if (data->status) {

			dir = data->dir + L"\\Sounds\\LoggerOn.wav";
			PlaySoundW(dir.c_str(), NULL, SND_FILENAME);
		}
		else {

			dir = data->dir + L"\\Sounds\\LoggerOff.wav";
			PlaySoundW(dir.c_str(), NULL, SND_FILENAME);

		}

		// release the prevoiusly allocated memory for the threads data
		// was allocated in switchDataLogger() method
		delete data;
		data = nullptr;
	}

	return 0;
}

///
/// setup the needed data for the playback thread
///
void CBasePlugin::switchDataLogger()
{
	_thData* data = new _thData;
	memset(data, 0, sizeof(_thData));

	data->dir = _odl->GetLoggerRootPath(); //_currentDir;
	data->status = _dataLoggerEnabled;

	if (_dataLoggerEnabled) {
		_odl->StartStint();
		_stintRunning = true;
	}
	else if ((_stintRunning) && (!_dataLoggerEnabled)){
		_odl->StopStint();
		_stintRunning = false;
	}

	CreateThread(NULL, 0, &CBasePlugin::SoundPlaybackThreadRoutine, (void*)data, 0, NULL);
}


void CBasePlugin::checkLoggerKeyState()
{
	USHORT keyState = GetAsyncKeyState(VK_CONTROL);
	if (keyState & 0x8000) {
		keyState = GetAsyncKeyState(0x4D); // 0x4D = M key
		if (keyState & 0x8000) {
			if (_dataLoggerEnableKeyDown == false && (_sessionTime - _dataLoggerKeyDelta) > 1.0f) {

				// only switch the data logger if the enable key was previously released and the time span between
				// the key presses were at least one second;
				// this prevents the data logger and sound playback from bouncing between states and ending up in the
				// wrong state
				// -> remember UpdateTelemetry is called 90 times per second!
				_dataLoggerEnableKeyDown = true;
				_dataLoggerKeyDelta = _sessionTime;

				_dataLoggerEnabled = !_dataLoggerEnabled;

				// TODO: toggle actual data logger on/off

				switchDataLogger();
			}
		}
	}
	else {
		_dataLoggerEnableKeyDown = false;
	}
}