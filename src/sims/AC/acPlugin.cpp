/***
***/

#include "acPlugin.h"

using namespace ACPlugin;

const char* g_vehicleName;
const char* g_trackName;

// global variable to hold plugin instance. Plugin is loaded with rundll for testing and debugging.
// Later if plugin is loaded via python script this instance is voided. 
AC_Plugin* _gInst;


/*** C-interface -> export ***/
void acPlugin()
{
	_gInst = new AC_Plugin();
}

void acPlugin_clean()
{
	if (_gInst) {
		delete _gInst;
		_gInst = NULL;
	}
}
/********************************************************************/


AC_Plugin::AC_Plugin()
{
	bool _updateLoopRunning = false;
	float _lastUpdate =0.0f; 


	_odl = new COpenDataLogger();
	if (!_odl)
		throw 0xFF;
}

AC_Plugin::~AC_Plugin()
{
}

void AC_Plugin::Run()
{
	// open handles to shared memory and map to buffer
	_fmPhysics = OpenFileMapping(FILE_MAP_READ, FALSE, (LPCSTR)AC_PhyMapName.c_str());
	if (_fmPhysics == NULL) {
		// TODO: handle error

		throw - 1;
	}
	_mapBufPhysics = (BYTE*)MapViewOfFile(_fmPhysics, FILE_MAP_READ, 0, 0, sizeof(SPageFilePhysics));
	if (_mapBufPhysics == NULL) {
		// TODO: handle error -> exception

		throw - 11;
	}

	_fmStatic = OpenFileMapping(FILE_MAP_READ, FALSE, (LPCSTR)AC_StaticMapName.c_str());
	if (_fmStatic == NULL) {
		// TODO: handle error

		throw - 2;
	}
	_mapBufStatic = (BYTE*)MapViewOfFile(_fmStatic, FILE_MAP_READ, 0, 0, sizeof(SPageFilePhysics));
	if (_mapBufStatic == NULL) {
		// TODO: handle error -> exception

		throw - 22;
	}

	_fmGraphics = OpenFileMapping(FILE_MAP_READ, FALSE, (LPCSTR)AC_GfxMapName.c_str());
	if (_fmGraphics == NULL) {
		// TODO: handle error

		throw - 3;
	}
	_mapBufGfx = (BYTE*)MapViewOfFile(_fmGraphics, FILE_MAP_READ, 0, 0, sizeof(SPageFilePhysics));
	if (_mapBufGfx == NULL) {
		// TODO: handle error -> exception

		throw - 33;
	}

	// create new thread 
	//_pollingThread = CreateThread(NULL, 0, )
	_updateLoopRunning = true;
	AC_Plugin c;
	_pollingThread = std::thread(&AC_Plugin::updateLoop, std::ref(c), 0);
	
}

void AC_Plugin::updateLoop()
{
	while (_updateLoopRunning) {
		SPageFileGraphic* pf = (SPageFileGraphic*)_mapBufGfx;

		if (_lastUpdate != pf->sessionTimeLeft) {

			// new set of data available
//			_lastUpdate = pf->sessionTimeLeft;
		}

	}
}