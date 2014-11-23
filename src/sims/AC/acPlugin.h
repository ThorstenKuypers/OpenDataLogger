#pragma once

#include <string>
#include <thread>
#include <Windows.h>

#include <OpenDataLogger.h>
#include <BasePlugin.h>
#include "SharedFileOut.h"

/****************************************************************************************
 * 
 * The plugin system of AC is quite different that thes one used by rFactor. Instead of
 * loading the DLL directly as in rFactor AC uses python scripts to define plugin apps.
 * So this DLL is loaded by the python app odl.py from within AC.
****************************************************************************************/

#define AC_PLUGIN_EXPORT __declspec(dllexport)

using namespace std;
using namespace OpenDataLogger;

namespace ACPlugin {

	class AC_Plugin : public CBasePlugin {

	private:

		const string AC_PhyMapName ="Local\\acpmf_physics";
		const string AC_GfxMapName = "Local\\acpmf_graphics";
		const string AC_StaticMapName = "Local\\acpmf_static";

		// File Mapping Handles
		HANDLE _fmPhysics;
		HANDLE _fmStatic;
		HANDLE _fmGraphics;

		BYTE* _mapBufPhysics;
		BYTE* _mapBufStatic;
		BYTE* _mapBufGfx;

		COpenDataLogger* _odl;

		// thread that handles periodical polling of telemetry data
		//HANDLE _pollingThread;
		std::thread _pollingThread;

		void updateLoop();
		bool _updateLoopRunning;
		float _lastUpdate;

	public:

		AC_Plugin();
		virtual ~AC_Plugin();

		// starts the data acquisition loop
		void Run();
	};

}


/* these defines the export API interface which is used as entry point for pathon
 * ctypes to load this DLL from inside AC app */
extern "C" AC_PLUGIN_EXPORT void acPlugin();
extern "C" AC_PLUGIN_EXPORT void acPlugin_clean();
