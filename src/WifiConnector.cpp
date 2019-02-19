
#include <WiFi.h>
#include "WifiConnector.h"

WifiConnector::WifiConnector(Logger& _logger, const char* _ssid, const char* _password) :
	logger(_logger)
{
    strcpy(ssid, _ssid);
    strcpy(password, _password);
}

WifiConnector::~WifiConnector()
{
    disconnect();
}

bool WifiConnector::connect()
{
	uint16_t attempt = 0;
	logger.log("Connecting to ");
    //char _ssid[30];
    //char _password[30];
    //strcpy(_ssid, ssid.c_str());
    //strcpy(_password, password.c_str());
    logger.logLine(ssid);
    logger.logLine(password);
    WiFi.begin(ssid, password);
	//WiFi.begin(ssid.c_str(), password.c_str());
    logger.logLine("after WiFi.begin() ");
	logger.logLine(ssid);

	uint8_t i = 0;
	while (WiFi.status() != WL_CONNECTED && i++ < 50)
	{
		delay(200);
		logger.log(".");
	}
	++attempt;
	logger.logLine("");
	if (i == 51) 
	{
		logger.log("Connection: TIMEOUT on attempt: ");
		logger.logLine(attempt);
		if (attempt % 2 == 0)
			logger.logLine("Check if access point available or SSID and Password\r\n");
		return false;
	}
	logger.logLine("Connection: ESTABLISHED");
	logger.log("Got IP address: ");
	logger.logLine(WiFi.localIP().toString().c_str());
	return true;
}

bool WifiConnector::disconnect()
{
    WiFi.disconnect();
    return true;
}

