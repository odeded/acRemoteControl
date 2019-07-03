#pragma once 

#include <time.h>
#include <string>

using namespace std;

class TimeManager
{
    //standard time -> 16:45
public:
    static const int timeStandardToSecs(const string& _time); 

    static bool setTime(const string& _time);

    static int getSecondsInDay();
    static string getTimeInDay();

    static bool timeCommand(std::string &cmd, std::string &resultMsg);
};