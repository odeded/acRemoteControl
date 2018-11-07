
#include "Logger.h"
#include <HardwareSerial.h>

Logger::Logger(int baud)
{
	Serial.begin(baud);
}

void Logger::logLine(const char* msg)
{
	Serial.println(msg);
}

void Logger::log(const char* msg)
{
	Serial.print(msg);
}

void Logger::log(int num)
{
  Serial.print(num);
}

void Logger::logLine(int num)
{
  Serial.println(num);
}
