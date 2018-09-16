#pragma once

#include "Logger.h"

class WifiConnector
{
private:
	String ssid;
	String password;

	Logger& logger;

public:
	WifiConnector(const char* _ssid, const char* _password, Logger& _logger);

	bool connect();
};
