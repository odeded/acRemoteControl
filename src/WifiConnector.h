#pragma once

#include <string>
#include "Logger.h"

class WifiConnector
{
private:
    Logger& logger;
    char ssid[30];
    char password[30];
	
public:
	WifiConnector(Logger& _logger, const char* _ssid, const char* _password);
    ~WifiConnector();

	bool connect();
    bool disconnect();
};
