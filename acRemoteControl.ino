
#include "WifiConnector.h"
#include "GmailIMAP.h"
#include "Logger.h"

Logger logger(9600);
WifiConnector wifi("Oded2", "21065739", logger);
GmailImap gmailImap(logger);

void setup() 
{
  std::string str;
  logger.log("starting...");
  wifi.connect();
  gmailImap.connect();
}

void loop() 
{
}
