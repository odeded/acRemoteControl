#pragma once

#include <string>
#include "Logger.h"

class WifiConnector
{
private:
	std::string ssid;
	std::string password;

	Logger& logger;

public:
	WifiConnector(const char* _ssid, const char* _password, Logger& _logger);

	bool connect();
};
