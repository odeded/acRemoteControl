
#include <WiFi.h>
#include "WifiConnector.h"

WifiConnector::WifiConnector(const char* _ssid, const char* _password, Logger& _logger) :
	ssid(_ssid), password(_password), logger(_logger)
{}

bool WifiConnector::connect()
{
	uint16_t attempt = 0;
	logger.log("Connecting to ");

	WiFi.begin(ssid.c_str(), password.c_str());
	logger.logLine(ssid.c_str());

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
	//logger.logLine(WiFi.localIP());
	return true;
}
