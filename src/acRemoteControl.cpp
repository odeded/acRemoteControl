
#include "ConstsPrivate.h"
#include "WifiConnector.h"
#include "Logger.h"
#include "CommandsListenerGmail.h"
#include "CommandsListenerTelegram.h"
#include "CommandsProvider.h"
#include "SetPinCommands.h"
#include "AcIrCommands.h"
#include <Arduino.h>
#include <WiFi.h>


Logger logger(9600);
WifiConnector wifi(logger, WIFI_SSID, WIFI_PASSWORD);
CommandsProvider cmdProvider;
CommandsListenerGmail *cmdListenerGmail;
CommandsListenerTelegram *cmdListenerTelegram;
std::string mailUser = AC_GMAIL_USER;
std::string mailPassword = AC_GMAIL_PASSWORD;
std::string mailCmdIdentifier = "AC Command";

//AcIrCommands acRemote(21);
//AcIrCommands acRemote;

void setup()
{
    logger.logLine("Starting....");
    cmdProvider.registerCommand("Cool", AcIrCommands::setCoolCommand);
    cmdProvider.registerCommand("Heat", AcIrCommands::setHeatCommand);
    cmdProvider.registerCommand("Off",  AcIrCommands::setOff);
    cmdProvider.registerCommand("set", SetPinCommands::setPin);
    cmdProvider.registerCommand("unset", SetPinCommands::unsetPin);

    //cmdListenerGmail = new CommandsListenerGmail(logger, wifi, mailUser, mailPassword,
    //                                        20, cmdProvider, mailCmdIdentifier);
    //cmdListenerGmail->start();
    
    cmdListenerTelegram = new CommandsListenerTelegram(logger, wifi, cmdProvider);
    cmdListenerTelegram->start();
}

electraAcRecordedSender acSender(21);
int delayMainLoopSec = 10000;

void loop()
{
    delay(delayMainLoopSec);
    delayMainLoopSec = 1000;
    
    if (AcIrCommands::toCool)
    {
        AcIrCommands::toCool = false;
        acSender.sendAcCoolCommand(AcIrCommands::toTemp);
        logger.logLine("Sent cool");
        delayMainLoopSec = 5000;
    }
    if (false && AcIrCommands::toHeat)
    {
        AcIrCommands::toHeat = false;
        acSender.sendAcHeatCommand(AcIrCommands::toTemp);
        logger.logLine("Sent heat");
        delayMainLoopSec = 5000;
    }
}
