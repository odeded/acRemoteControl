#pragma once

#include "ICommandsListener.h"
#include "WifiConnector.h"
#include "commandsProvider.h"
#include "Logger.h"
#include "UniversalTelegramBot\UniversalTelegramBot.h"

#include <WiFiClientSecure.h>
#include <string>
#include <thread>

class CommandsListenerTelegram : public ICommandsListener
{
public:
    CommandsListenerTelegram(Logger& _logger, WifiConnector& _wifiConnector, CommandsProvider& _commandsProvider);

    virtual bool start();
    virtual bool stop();

    void mainLoop(void*);

private:
    Logger&             logger;
    WifiConnector&      wifiConnector;

    WiFiClientSecure client;
    UniversalTelegramBot bot;
};