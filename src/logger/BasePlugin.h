#ifndef _ODL_BASE_PLUGIN_H_
#define _ODL_BASE_PLUGIN_H_

#include "yaml.h"

// Definition of Base plugin class - implements functionality common to all plugins

class CBasePlugin
{
public:
	CBasePlugin() {};
	virtual ~CBasePlugin() {};

protected:

	WeekendInfo _weekendInfo;
	SessionInfo _sessionInfo;
	DriverInfo _driverInfo;

	virtual void YamlUpdate(void*) = 0;
};

#endif //_ODL_BASE_PLUGIN_H_