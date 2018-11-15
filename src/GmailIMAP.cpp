
#include "GmailIMAP.h"
#include <sstream>

GmailImap::GmailImap(Logger &_logger, std::string _user, std::string _password) : 
	logger(_logger), user(_user), password(_password), msgIndex(0)
{
}

bool GmailImap::connect()
{
	if (!sslClient.connect(IMAP_SERVER, IMAP_PORT))
	{
		logger.logLine("ERROR: Could not connect to mail server");
		return false;
	}

	readAllResponses();

	std::string cmd = "";
	cmd += "LOGIN ";
	cmd += user + " " + password;

	if (!sendCommandWithoutResponse(cmd.c_str()))
	{
		logger.logLine("ERROR: wrong user/password");
		return false;
	}
	return true;
}

bool GmailImap::selectFolder(const char *folder)
{
	std::string cmd = "select ";
	cmd += folder;
	if (!sendCommandWithoutResponse(cmd))
		return false;
	return true;
}

void GmailImap::disconnect()
{
	sendCommandWithoutResponse("EXPUNGE");
	sendCommandWithoutResponse("CLOSE");
}

bool GmailImap::searchMailsBySubject(std::string &subject, IndexesList &mailsIndexes)
{
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

	std::stringstream stream(response.substr(startIndex));
	while (true)
	{
		int n;
		stream >> n;

		if (!stream)
			break;
		mailsIndexes.push_back(n);
	}
	return true;
}

bool GmailImap::getMail(int index, MailItem &mail)
{
	char numstr[21];
	std::string command = "";
	std::string response = "";
	std::string literal = "";

	command = "FETCH ";
	command += itoa(index, numstr, 10);
	command += " BODY[TEXT]";
	if (!sendCommandGetResponse(command, response, &literal))
	{
		return false;
	}

	mail.index = index;
	mail.body = literal;

	return true;
}

bool GmailImap::deleteMail(int index)
{
	char numstr[21];
	std::string command = "";
	std::string response = "";

	command = "STORE ";
	command += itoa(index, numstr, 10);
	command += " +FLAGS (\\Deleted)";
	if (!sendCommandWithoutResponse(command))
	{
		return false;
	}

	return true;
}

bool GmailImap::sendCommandWithoutResponse(const std::string &message)
{
	std::string str;
	return sendCommandGetResponse(message, str);
}

bool GmailImap::sendCommandGetResponse(const std::string &message, std::string &response, std::string* literal)
{
	char numstr[21];
	response = "";
	msgIndex++;

	//send command
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

	//Read response
	do
	{
		std::string responseLine = "";
		if (!recvResponseLine(responseLine))
		{
			logger.logLine("ERROR: Failed getting response line");
			return false;
		}
		response += responseLine;

		if (responseLine[0] == ')' || responseLine.empty())
		{
			continue;
		}

		if (responseLine[0] == '*' || responseLine[0] == '+')
		{
			std::string tempLiteral;
			tempLiteral = readLiteral(responseLine);
			if (!tempLiteral.empty()) 
			{
				if (literal != nullptr)
				{
					*literal = tempLiteral;
				}
				response += tempLiteral;
			}
			continue;
		}

		if (responseLine[0] == TAG_PREFIX)
		{
			if (!validateTaggedLine(responseLine))
			{
				return false;
			}

			break;
		}

		logger.logLine("ERROR: Unkown tag");
		return false;

	} while (true);

	return true;
}

std::string GmailImap::readLiteral(std::string &responseLine)
{
	int openBraceIndex = responseLine.find('{');
	int closeBraceIndex = responseLine.find('}');

	// (from rfc-3501) Look for literal in the form of {<size>} TEXT_HERE_IN_SIZE
	if (!(0 < openBraceIndex && openBraceIndex < closeBraceIndex))
	{
		return "";
	}

	int literalSize = std::atoi(responseLine.substr(openBraceIndex + 1, closeBraceIndex - openBraceIndex - 1).c_str());
	if (literalSize == 0)
	{
		return "";
	}

	std::string literalStr = "";
	readBuffer(literalStr, literalSize);
	return literalStr;
}

bool GmailImap::validateTaggedLine(std::string &responseLine)
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

bool GmailImap::readBuffer(std::string &buffer, int size)
{
	int bytesToRead = size;
	const int MAX_READ = 100;
	char readBuf[MAX_READ + 1];

	while (bytesToRead > 0)
	{
		uint32_t ts = millis();
		while (!sslClient.available())
		{
			if (millis() > (ts + EMPTY_RESPONSES_TIMEOUT))
			{
				return false;
			}
		}

		int recvBytes = sslClient.readBytes(readBuf, min(MAX_READ, bytesToRead));
		readBuf[recvBytes] = '\0';

		bytesToRead -= recvBytes;
		buffer += readBuf;
	}

	return true;
}
