#pragma once 

#include <time.h>
#include <string>

using namespace std;

class TimeManager
{
    //standard time -> 16:45
public:
    static int    timeStandardToSecs(const string& _time); 
    static string timeSecsToStandard(const int timeSecs);

    static bool setTime(const string& _time);

    static int getSecondsInDay();
    static string getTimeInDay();

    static bool timeCommand(std::string &cmd, std::string &resultMsg);
};