#include "TimeManager.h"

#include <stdlib.h>
#include <ctime>
#include <cstdio>

const int TimeManager::timeStandardToSecs(const string& _time)
{
    #define checkDigit(ch, low, high) if (ch > '9'+high || ch < '0'+low) return -1
	auto ch2int = [](char c) {return int(c - '0'); };

    if (_time.size() != 5)
    {
        return -1;
    }
    checkDigit(_time[0], 0, 2);
    checkDigit(_time[1], 0, 9);
    if (_time[2] != ':') return -1;
    checkDigit(_time[3], 0, 2);
    checkDigit(_time[4], 0, 9);

    int hours = ch2int(_time[0]) * 10 + ch2int(_time[1]);
    if (hours > 23) return -1;

    int minutes = ch2int(_time[3]) * 10 + ch2int(_time[4]);
    if (minutes > 59) return -1;

	int seconds = hours*3600 + minutes*60;	            
    return seconds;        
}

bool TimeManager::setTime(const string& _time)
{
    int seconds = timeStandardToSecs(_time);
    if (seconds == -1)
    {
        return false;
    }
    timeval tt {seconds, 0};
    settimeofday(&tt, NULL);
    return true;
}

bool TimeManager::timeCommand(std::string &cmd, std::string &resultMsg)
{
    if (cmd.empty())
    {
        resultMsg = getTimeInDay();
        return true;
    }

    if (!setTime(cmd))
    {
        resultMsg = "Time error";
        return false;
    }
    resultMsg = "Time set";
    return true;
}

int TimeManager::getSecondsInDay()
{
    std::time_t secsFromEpoc = std::time(nullptr);
    return secsFromEpoc % (60*60*24); //60*60*24 seconds in day
}

string TimeManager::getTimeInDay()
{
    char numstr[21];
    int secs = getSecondsInDay();
    int hours = (secs / 3600);
    int minutes = (secs % 3600) / 60;
    string result = "";
    sprintf(numstr,"%02d",hours);
    result += numstr;
    result += ":";
    sprintf(numstr,"%02d",minutes);
    result += numstr;
    return result;
}
