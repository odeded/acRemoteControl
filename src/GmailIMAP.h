#pragma once

#include "Logger.h"
#include "mailItem.h"
#include <string>
#include <list>
#include <WiFiClientSecure.h>

class GmailImap
{
protected:
	static constexpr int	IMAP_PORT = 993;
	const char*				IMAP_SERVER = "imap.gmail.com";
	static constexpr int	RESPONSE_TIMEOUT = 15000;
	static constexpr int	EMPTY_RESPONSES_TIMEOUT = 5000;
	static constexpr char	TAG_PREFIX = 'A';

	Logger&				logger;
	std::string 		user;
	std::string 		password;

	WiFiClientSecure	sslClient;
	int					msgIndex;
public:
	using IndexesList = std::list<int>;
	
	GmailImap(Logger& logger, std::string _user, std::string _password);

	bool connect();
	void disconnect();
	bool selectFolder(const char* folder);
	bool getMail(int index, MailItem &mail);
	bool searchMailsBySubject(std::string &subject, IndexesList& indexList);
	bool deleteMail(int index);

protected:
	bool sendCommand(const std::string& message);
	bool sendCommandGetResponse(const std::string& message, 
								std::string& response, 
								std::string* literal = nullptr);
	bool sendCommandWithoutResponse(const std::string& message);
	bool recvResponseLine(std::string& response);
	bool readAllResponses();
	bool readBuffer(std::string& buffer, int size);

	bool validateTaggedLine(std::string& responseLine);
	std::string readLiteral(std::string& responseLine);
};
