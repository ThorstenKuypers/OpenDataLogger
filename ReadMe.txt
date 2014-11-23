
OpenDataLogger - project structure


openDataLogger: Data set "wrapper" module. It implements a unified set of
			    telemetry data objects which are then fed into the openGameAPI 
				module. It is implemented as a static library against which all
				plugins are linked at compile time.

openGameAPI: Is the core data logger "device". Provides a shared memory 
			 "live-logger" and disk-based (file) telemetry data logging
			 functionality.

plugins:
---------
Actual interfaces which integrate into the corresponding sim to provide sim-specific
telemetry data.