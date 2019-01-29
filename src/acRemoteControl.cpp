
#include "ConstsPrivate.h"
#include "WifiConnector.h"
#include "Logger.h"
#include "CommandsListenerGmail.h"
#include "CommandsListenerTelegram.h"
#include "CommandsProvider.h"
#include "SetPinCommands.h"
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

void setup()
{
    logger.logLine("Starting....");
    cmdProvider.registerCommand("set", SetPinCommands::setPin);
    cmdProvider.registerCommand("unset", SetPinCommands::unsetPin);

    //cmdListenerGmail = new CommandsListenerGmail(logger, wifi, mailUser, mailPassword,
    //                                        20, cmdProvider, mailCmdIdentifier);
    //cmdListenerGmail->start();
    
    cmdListenerTelegram = new CommandsListenerTelegram(logger, wifi, cmdProvider);
    cmdListenerTelegram->start();
}

void loop()
{
    delay(111111);
}
