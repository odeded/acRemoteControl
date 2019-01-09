
#include "CommandsListenerGmail.h"
#include "GmailIMAP.h"
#include "Esp32Thread.h"

CommandsListenerGmail::CommandsListenerGmail(
    Logger& _logger, WifiConnector& _wifiConnector, std::string _gmailUser, std::string _gmailPassword,
    int _checkIntervalsSeconds, CommandsProvider& _commandsProvider, std::string _commandIdetifier)
    : ICommandsListener(_commandsProvider),  
    logger(_logger), wifiConnector(_wifiConnector),
      gmailUser(_gmailUser), gmailPassword(_gmailPassword), 
      checkIntervalsSeconds(_checkIntervalsSeconds), commandIdetifier(_commandIdetifier)
{
}

bool CommandsListenerGmail::start()
{
    Esp32Thread<CommandsListenerGmail> t1;
    t1.start( Esp32Thread<CommandsListenerGmail>::RunnningParams(this, nullptr), "GmailCmd", 0);

    return true;
}

bool CommandsListenerGmail::stop()
{
    myThread.reset();
    return true;
}

#define BREAK_ON_FAILUE(X) \
    if (!X)                \
    {                      \
        break;             \
    }

void CommandsListenerGmail::mainLoop(void*)
{
    while (true)
    {
        do
        {
            GmailImap gmailMailReader(logger, gmailUser, gmailPassword, wifiConnector);
            BREAK_ON_FAILUE(gmailMailReader.connect());
            BREAK_ON_FAILUE(gmailMailReader.selectFolder("INBOX"));

            MailItem mail;
            //std::string sub = "AC Command";
            GmailImap::IndexesList mails;
            BREAK_ON_FAILUE(gmailMailReader.searchMailsBySubject(commandIdetifier, mails));
            for (GmailImap::IndexesList::iterator itr = mails.begin(); itr != mails.end(); ++itr)
            {
                gmailMailReader.getMail(*itr, mail);
                gmailMailReader.deleteMail(mail.index);
                logger.log("Running command ");
                logger.logLine(mail.body.c_str());
                std::string result;
                if (!commandsProvider.runCommand(mail.body, result))
                {
                    logger.log("Failed running command ");
                    logger.logLine(mail.body.c_str());
                }
            }
        } while (false);

        logger.log("Going to sleep for ");
        logger.log(checkIntervalsSeconds);
        logger.logLine(" seconds.....");

        delay(checkIntervalsSeconds * 1000);
    }
}
