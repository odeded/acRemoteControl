
#include "ConstsPrivate.h"
#include "WifiConnector.h"
#include "Logger.h"
//#include "CommandsListenerGmail.h"
#include "CommandsListenerTelegram.h"
#include "CommandsProvider.h"
#include <Arduino.h>
#include <WiFi.h>


Logger logger(9600);
WifiConnector wifi(logger, WIFI_SSID, WIFI_PASSWORD);
CommandsProvider cmdProvider;
//CommandsListenerGmail *cmdListener;
CommandsListenerTelegram *cmdListener;
std::string mailUser = AC_GMAIL_USER;
std::string mailPassword = AC_GMAIL_PASSWORD;
std::string mailCmdIdentifier = "AC Command";

void setup()
{
    logger.logLine("Starting....");

    //WiFi.begin("Oded2", "21065739");
    //cmdListener = new CommandsListenerGmail(logger, wifi, mailUser, mailPassword,
    //                                        15, cmdProvider, mailCmdIdentifier);
    cmdListener = new CommandsListenerTelegram(logger, wifi, cmdProvider);
    cmdListener->start();
}

void loop()
{
    delay(111111);
}
