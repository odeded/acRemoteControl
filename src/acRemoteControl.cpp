
#include "WifiConnector.h"
#include "GmailIMAP.h"
#include "Logger.h"
#include "mailItem.h"
#include <thread>
#include <esp_log.h>

Logger logger(9600);
WifiConnector wifi("Oded2", "21065739", logger);
GmailImap gmailImap(logger, "acshayo@gmail.com", "Madhima10");

#define BREAK_ON_FAILUE(X) \
    if (!X)                \
    {                      \
        break;             \
    }

void setup()
{
    ESP_LOGW(TAG, "Baud rate error %x", &logger);
    ESP_LOGE(TAG, "ERR: Baud rate error %x", &logger);
    return;

    std::string str;
    logger.log("starting...");
    do
    {
        BREAK_ON_FAILUE(wifi.connect());
        BREAK_ON_FAILUE(gmailImap.connect());
        BREAK_ON_FAILUE(gmailImap.selectFolder("INBOX"));

        MailItem mail;
        std::string sub = "AC Command";
        GmailImap::IndexesList mails;
        BREAK_ON_FAILUE(gmailImap.searchMailsBySubject(sub, mails));
        for (GmailImap::IndexesList::iterator itr = mails.begin(); itr != mails.end(); ++itr)
        {
            gmailImap.getMail(*itr, mail);
            logger.logLine(mail.body.c_str());
        }
        //BREAK_ON_FAILUE(gmailImap.deleteMail(*(mails.begin())));

    } while (false);
    gmailImap.disconnect();
}

void loop()
{
}
