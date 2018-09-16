#pragma once

class Logger
{
private:

public:
	Logger(int baud);
	void log(const char* msg);
	void logLine(const char* msg);
  void log(const int num);
  void logLine(const int num);
};
