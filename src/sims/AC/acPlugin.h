#pragma once

#include <string>
#include <thread>
#include <Windows.h>

#include <OpenDataLogger.h>
#include "SharedFileOut.h"

#define AC_PLUGIN_EXPORT __declspec(dllexport)

using namespace std;
using namespace OpenDataLogger;

namespace ACPlugin {

	class AC_Plugin {

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

extern "C" AC_PLUGIN_EXPORT void acPlugin();
extern "C" AC_PLUGIN_EXPORT void acPlugin_clean();

//extern "C" void acPlu