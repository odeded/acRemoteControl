#pragma once

#include <string>
#include <WiFiClientSecure.h>
#include "Logger.h"
#include "mailItem.h"

class GmailImap
{
protected:
	static constexpr int	IMAP_PORT = 993;
	const char*	IMAP_SERVER = "imap.gmail.com";
	static constexpr int	RESPONSE_TIMEOUT = 10000;
	static constexpr int	EMPTY_RESPONSES_TIMEOUT = 5000;
	static constexpr char	TAG_PREFIX = 'A';

	std::string userName;
	std::string password;

	WiFiClientSecure	sslClient;
	Logger&				logger;
	int					msgIndex;
public:
	GmailImap(Logger& logger);

	//void setLogger(Logger& logger);

	bool connect();
	void disconnect();
	bool selectFolder(const char* folder);
	//void readMessage();
	bool getFirstMailBySubject(std::string& subject, MailItem& mail);

protected:
	bool sendCommand(const std::string& message);
	bool sendCommandGetResponse(const std::string& message, std::string& response);
	bool sendCommandWithoutResponse(const std::string& message);
	bool recvResponseLine(std::string& response);
	bool readAllResponses();
};
