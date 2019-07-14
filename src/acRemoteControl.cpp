
#include "ConstsPrivate.h"
#include "WifiConnector.h"
#include "Logger.h"
#include "CommandsListenerGmail.h"
#include "CommandsListenerTelegram.h"
#include "CommandsProvider.h"
#include "SetPinCommands.h"
#include "AcController.h"
#include "AcIrCommands.h"
#include "TimedCommands.h"
#include "timeManager.h"

#include <Arduino.h>
#include <WiFi.h>

Logger logger(9600);
WifiConnector wifi(logger, WIFI_SSID, WIFI_PASSWORD);
CommandsProvider cmdProvider;
//CommandsListenerGmail *cmdListenerGmail;
CommandsListenerTelegram *cmdListenerTelegram;
//std::string mailUser = AC_GMAIL_USER;
//std::string mailPassword = AC_GMAIL_PASSWORD;
//std::string mailCmdIdentifier = "AC Command";

RgbLed rgbLed(25, 26, 27);
AcController acController(21, rgbLed, logger);

void setup()
{
    logger.logLine("Starting....");
    AcIrCommands::setAcController(acController);
    AcIrCommands::setRgbLed(rgbLed);
    TimedCommands::get()->setParams(acController, logger);

    cmdProvider.registerCommand("Cool", AcIrCommands::setCoolCommand);
    cmdProvider.registerCommand("Heat", AcIrCommands::setHeatCommand);
    cmdProvider.registerCommand("Off",  AcIrCommands::setOff);
    cmdProvider.registerCommand("Status", AcIrCommands::setAcStatus);
    cmdProvider.registerCommand("Timer", TimedCommands::setTimer);
    cmdProvider.registerCommand("mutex", TimedCommands::setMutex);
    
    cmdProvider.registerCommand("Time", TimeManager::timeCommand);
    
    //cmdProvider.registerCommand("set", SetPinCommands::setPin);
    //cmdProvider.registerCommand("unset", SetPinCommands::unsetPin);

    //cmdListenerGmail = new CommandsListenerGmail(logger, wifi, mailUser, mailPassword,
    //                                        20, cmdProvider, mailCmdIdentifier);
    //cmdListenerGmail->start();
    
    cmdListenerTelegram = new CommandsListenerTelegram(logger, wifi, cmdProvider);
    cmdListenerTelegram->start();

    //TimedCommands::get();
}

int delayMainLoopSec = 10000; //in 1st round wait 10sec for letting all to start correctly

void loop()
{
    delay(delayMainLoopSec);
    delayMainLoopSec = 5000;
    
    if (acController.checkAndHandleQueue())
    {
        delayMainLoopSec = 10000;
    }
}
