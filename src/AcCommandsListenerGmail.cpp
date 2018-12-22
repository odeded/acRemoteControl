
#include "AcCommandsListenerGmail.h"

AcCommandsListenerGmail::AcCommandsListenerGmail(
    Logger &_logger, std::string _user, std::string _password, int _checkIntervalsSeconds) : 
    logger(_logger), checkIntervalsSeconds(_checkIntervalsSeconds), gmailMailReader(logger, _user, _password)
{
}

bool AcCommandsListenerGmail::start()
{
    myThread.reset(new std::thread());
    return true;
}

bool AcCommandsListenerGmail::stop()
{
    myThread.reset();
    return true;
}

#define BREAK_ON_FAILUE(X) \
    if (!X)                \
    {                      \
        break;             \
    }

void AcCommandsListenerGmail::mainLoop()
{
    while (true)
    {
        do
        {
            BREAK_ON_FAILUE(gmailMailReader.connect());
            BREAK_ON_FAILUE(gmailMailReader.selectFolder("INBOX"));

            MailItem mail;
            std::string sub = "AC Command";
            GmailImap::IndexesList mails;
            BREAK_ON_FAILUE(gmailMailReader.searchMailsBySubject(sub, mails));
            for (GmailImap::IndexesList::iterator itr = mails.begin(); itr != mails.end(); ++itr)
            {
                gmailMailReader.getMail(*itr, mail);
                logger.logLine(mail.body.c_str());
                commandsProvider.runCommand(mail.body);
            }
        } while (false);
        
        gmailMailReader.disconnect();
        sleep(checkIntervalsSeconds);
    }
}
