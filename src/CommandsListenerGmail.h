#pragma once

#include "ICommandsListener.h"
//#include "GmailIMAP.h"
#include "WifiConnector.h"
#include "commandsProvider.h"
#include "Logger.h"

#include <string>
#include <thread>

class CommandsListenerGmail : public ICommandsListener
{
public:
    CommandsListenerGmail(Logger& _logger, WifiConnector& _wifiConnector, 
                    std::string _gmailUser, std::string _gmailPassword, 
                    int _checkIntervalsSeconds, CommandsProvider& _commandsProvider, 
                    std::string _commandIdetifier);

    virtual bool start();
    virtual bool stop();

    void mainLoop(void*);

private:
    Logger&             logger;
    WifiConnector&      wifiConnector;
    std::string         gmailUser;
    std::string         gmailPassword;

    int checkIntervalsSeconds;
    std::unique_ptr<std::thread> myThread;
    std::string         commandIdetifier;
};
