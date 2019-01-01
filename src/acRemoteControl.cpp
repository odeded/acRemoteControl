
#include "WifiConnector.h"
#include "Logger.h"
#include "CommandsListenerGmail.h"
#include "CommandsProvider.h"
#include <Arduino.h>

Logger logger(9600);
WifiConnector wifi(logger, "Oded2", "21065739");
CommandsProvider cmdProvider;
CommandsListenerGmail *cmdListener;
std::string mailUser = "acshayo@gmail.com";
std::string mailPassword = "Madhima10";
std::string mailCmdIdentifier = "AC Command";

void setup()
{
    //wifi.connect();
    logger.logLine("Starting....");
    cmdListener = new CommandsListenerGmail(logger, wifi, mailUser, mailPassword,
                                            15, cmdProvider, mailCmdIdentifier);

    cmdListener->start();
}

void loop()
{
    delay(1111111);
}
