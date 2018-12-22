#pragma once

#include "IAcCommandsListener.h"
#include "GmailIMAP.h"
#include "commandsProvider.h"
#include "Logger.h"

#include <string>
#include <thread>

class AcCommandsListenerGmail : public IAcCommandsListener
{
public:
    AcCommandsListenerGmail(Logger& _logger, std::string _user, std::string _password, int _checkIntervalsSeconds);

    virtual bool start();
    virtual bool stop();
private:
    void mainLoop();

private:
    Logger&             logger;
    GmailImap           gmailMailReader;
    CommandsProvider    commandsProvider;

    int checkIntervalsSeconds;
    std::unique_ptr<std::thread> myThread;
};
