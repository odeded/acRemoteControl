#pragma once

#include <string>
#include <WiFiClientSecure.h>
#include "Logger.h"

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
	bool selectFolder(const char* folder);
	//void readMessage();

protected:
	bool sendCommand(const char*);
	bool sendCommandAndRecvResponse(const char*);
	bool recvResponseLine(std::string& response);
	bool readAllResponses();
};
