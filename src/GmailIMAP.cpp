
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
	sendCommandWithoutResponse("LOGIN acshayo@gmail.com Madhima10");
	return true;
}

void GmailImap::disconnect()
{
	//sendCommandWithoutResponse("EXPUNGE");
	sendCommandWithoutResponse("CLOSE");
}

bool GmailImap::getFirstMailBySubject(std::string& subject, MailItem& mail)
{
	char numstr[21];
	std::string command = "";
	std::string response = "";

	command = "SEARCH SUBJECT \"";
	command += subject;
	command += "\"";
	if (!sendCommandGetResponse(command, response))
	{
		return false;
	}

	//find first mail index
	#define SEARCH_PREFIX "* SEARCH "
	int startIndex = response.find(SEARCH_PREFIX);
	if (startIndex == std::string::npos)
	{
		return false;
	}
	startIndex += strlen(SEARCH_PREFIX);
	int index = strtol(response.c_str()+startIndex, NULL, 10);

	command = "FETCH ";
	command += itoa(index, numstr, 10);
	command += " BODY[TEXT]";
	if (!sendCommandGetResponse(command, response))
	{
		return false;
	}

	return true;
}

bool GmailImap::selectFolder(const char *folder)
{
	std::string cmd = "select ";
	cmd += folder;
	sendCommandWithoutResponse(cmd);
	return true;
}

bool GmailImap::sendCommandWithoutResponse(const std::string& message)
{
	std::string str;
	return sendCommandGetResponse(message, str);
}

bool GmailImap::sendCommandGetResponse(const std::string& message, std::string& response)
{
	char numstr[21];
	response = "";
	msgIndex++;

	std::string cmd;
	cmd.push_back(TAG_PREFIX);
	cmd += itoa(msgIndex, numstr, 10);
	cmd += " ";
	cmd += message;
	logger.log("Sent: ");
	logger.logLine(cmd.c_str());
	if (!sendCommand(cmd.c_str()))
	{
		logger.logLine("ERROR: Failed sending command");
		return false;
	}

	std::string responseLine = "";
	do
	{
		if (!recvResponseLine(responseLine))
		{
			logger.logLine("ERROR: Failed getting response line");
			return false;
		}
		response += responseLine;

		if (responseLine[0] == '*' || responseLine[0] == '+')
		{
			continue;
		}

		if (responseLine[0] == TAG_PREFIX)
		{
			//validate message index
			int end = responseLine.find(' ', 1);
			std::string respIndex = responseLine.substr(1, end);
			if (std::atoi(respIndex.c_str()) != msgIndex)
			{
				logger.logLine("ERROR: Got wrong index");
				return false;
			}

			//validate returned OK
			logger.log("===");
			logger.log(responseLine.substr(end + 1, 2).c_str());
			logger.logLine("===");
			if (responseLine.substr(end + 1, 2).compare("OK") != 0)
			{
				logger.logLine("ERROR: Got not OK");
				return false;
			}

			break;
		}

		//logger.logLine("ERROR: Unkown tag");
		//return false;

	} while (true);

	return true;
}

bool GmailImap::sendCommand(const std::string &message)
{
	sslClient.print(message.c_str());
	sslClient.print("\r\n");
	return true;
}

bool GmailImap::recvResponseLine(std::string &responseLine)
{
	responseLine = "";
	uint32_t ts = millis();
	while (!sslClient.available())
	{
		if (millis() > (ts + RESPONSE_TIMEOUT))
		{
			logger.logLine("ERROR: Communication timed out");
			return false;
		}
	}
	responseLine = sslClient.readStringUntil('\n').c_str();
	logger.logLine(responseLine.c_str());
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
