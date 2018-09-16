
#include "GmailIMAP.h"

GmailImap::GmailImap(Logger& _logger) : logger(_logger), msgIndex(0)
{

}

bool GmailImap::connect()
{
	if (!sslClient.connect(IMAP_SERVER, IMAP_PORT))
	{
		logger.logLine("Could not connect to mail server");
		return false;
	}
 String str;
 recvResponseLine(str);
	sendCommandAndRecvResponse("LOGIN acshayo@gmail.com Madhima10");
	sendCommandAndRecvResponse("select INBOX");
	sendCommand("fetch 1 full");
}

bool GmailImap::selectFolder(const char* folder)
{
	return true;
}

bool GmailImap::sendCommandAndRecvResponse(const char* msg)
{
	msgIndex++;

	String cmd(TAG_PREFIX);
	cmd += msgIndex;
	cmd += " ";
	cmd += msg;
	logger.log("Sent: ");
	logger.logLine(cmd.c_str());
	if (!sendCommand(cmd.c_str()))
	{
		logger.logLine("ERROR: Failed sending command");
		return false;
	}

	String response = "";
	do
	{
		if (!recvResponseLine(response))
		{
			logger.logLine("ERROR: Failed getting response line");
			return false;
		}

		if (response[0] == '*' || response[0] == '+')
		{
			continue;
		}

		if (response[0] == TAG_PREFIX)
		{
			//validate index
			int end = response.indexOf(1, " ");
			int respIndex = response.substring(1, end);
			if (respIndex.toInt() != msgIndex)
			{
				logger.logLine("ERROR: Got wrong index");
				return false;
			}

			//validate returned OK
			if (response.substring(end + 1, end + 3).equals("OK") != 0)
			{
				logger.logLine("ERROR: Got not OK");
				return false;
			}

			break;
		}

		logger.logLine("ERROR: Unkown tag");
		return false;

	} while (true);

	return true;
}

bool GmailImap::sendCommand(const char* msg)
{
	sslClient.print(msg);
	sslClient.print("\r\n");
	return true;
}

bool GmailImap::recvResponseLine(String& response)
{
	response = "";
	uint32_t ts = millis();
	while (!sslClient.available())
	{
		if (millis() > (ts + RESPONSE_TIOMOUT)) {
			logger.logLine("ERROR: Communication timed out");
			return false;
		}
	}
	response = sslClient.readStringUntil('\n');
	logger.logLine(response.c_str());
	return true;
}
