
#include "GmailIMAP.h"

GmailImap::GmailImap(Logger &_logger) : logger(_logger), msgIndex(0)
{
}

bool GmailImap::connect()
{
	if (!sslClient.connect(IMAP_SERVER, IMAP_PORT))
	{
		logger.logLine("Could not connect to mail server");
		return false;
	}
	std::string str;
	readAllResponses();
	sendCommandAndRecvResponse("LOGIN acshayo@gmail.com Madhima10");
	sendCommandAndRecvResponse("select INBOX");
	sendCommandAndRecvResponse("fetch 1 full");

	return true;
}

bool GmailImap::selectFolder(const char *folder)
{
	return true;
}

bool GmailImap::sendCommandAndRecvResponse(const char *msg)
{
	msgIndex++;

	char numstr[21];

	std::string cmd;
	cmd.push_back(TAG_PREFIX);
	cmd += itoa(msgIndex, numstr, 10);
	cmd += " ";
	cmd += msg;
	logger.log("Sent: ");
	logger.logLine(cmd.c_str());
	if (!sendCommand(cmd.c_str()))
	{
		logger.logLine("ERROR: Failed sending command");
		return false;
	}

	std::string response = "";
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
			//validate message index
			int end = response.find(' ', 1);
			std::string respIndex = response.substr(1, end);
			if (std::atoi(respIndex.c_str()) != msgIndex)
			{
				logger.logLine("ERROR: Got wrong index");
				return false;
			}

			//validate returned OK
			logger.log("===");
			logger.log(response.substr(end + 1, 2).c_str());
			logger.logLine("===");
			if (response.substr(end + 1, 2).compare("OK") != 0)
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

bool GmailImap::sendCommand(const char *msg)
{
	sslClient.print(msg);
	sslClient.print("\r\n");
	return true;
}

bool GmailImap::recvResponseLine(std::string &response)
{
	response = "";
	uint32_t ts = millis();
	while (!sslClient.available())
	{
		if (millis() > (ts + RESPONSE_TIMEOUT))
		{
			logger.logLine("ERROR: Communication timed out");
			return false;
		}
	}
	response = sslClient.readStringUntil('\n').c_str();
	logger.logLine(response.c_str());
	return true;
}

bool GmailImap::readAllResponses()
{
	while (true)
	{
		uint32_t ts = millis();
		while (!sslClient.available())
		{
			if (millis() > (ts + EMPTY_RESPONSES_TIMEOUT))
			{
				return true;
			}
		}
		logger.logLine(sslClient.readStringUntil('\n').c_str());
	}

	return true;
}