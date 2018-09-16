#pragma once

#include <WiFiClientSecure.h>
#include <string>
#include "Logger.h"

class GmailImap
{
protected:
	static constexpr int	IMAP_PORT = 993;
	static constexpr char*	IMAP_SERVER = "imap.gmail.com";
	static constexpr int	RESPONSE_TIOMOUT = 10000;
	static constexpr char	TAG_PREFIX = 'A';

	String userName;
	String password;

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
	bool recvResponseLine(String& response);
};
