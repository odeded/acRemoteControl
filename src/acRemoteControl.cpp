
#include "WifiConnector.h"
#include "GmailIMAP.h"
#include "Logger.h"
#include "mailItem.h"

Logger logger(9600);
WifiConnector wifi("Oded2", "21065739", logger);
GmailImap gmailImap(logger);

void setup() 
{
  std::string str;
  logger.log("starting...");
  wifi.connect();
  gmailImap.connect();
  gmailImap.selectFolder("INBOX");
  MailItem mail;
  std::string sub = "AC Command";
  gmailImap.getFirstMailBySubject(sub , mail);
  gmailImap.disconnect();
}

void loop() 
{
}
