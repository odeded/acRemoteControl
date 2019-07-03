#pragma once 

#include "AcController.h"

#include <string>
#include <queue>
#include <tuple>

using namespace std;

class TimedCommands {
public:
    static bool setTimer(std::string &cmd, std::string &resultMsg);

    static void handleTimedCommands();

private:
    struct Command
    {
        int                 timeInDay;
        AcController::AcCmd acCommand;
    };

    // timer Cool 24 16:00 19:00
    struct Type { 
        static const string Cool; 
        static const string Heat;
    };
    static bool parseCommand(const string, Command& on, Command& off, string& errorMsg);

    static int parseTimeFromString(const string timeStr);
    static void addCommand(const Command);

    using commandsQueue=std::queue<Command>;
    static commandsQueue cmdQueue;
};
