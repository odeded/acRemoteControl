#pragma once 

#include "AcController.h"

#include <string>
#include <queue>
#include <list>
#include <tuple>
#include <mutex>
#include <condition_variable>
#include "Esp32Thread.h"

using namespace std;

class TimedCommands {
public:
    static bool setTimer(std::string &cmd, std::string &resultMsg);
    static bool setMutex(std::string &cmd, std::string &resultMsg);

    void handleTimedCommands();

    static TimedCommands* get();
    TimedCommands();
    void setParams(AcController& controller, Logger& _logger);

    void getTimersStatus(string& res);

    void mainLoop(void*);
    void unlockMainLoop();

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

    int parseTimeFromString(const string timeStr);
    void addCommand(const Command);
    void clearList();
    void checkAndSendCommand();
    
    chrono::seconds getTimeWait();

    static TimedCommands* instance;

    using TimedCommandsQueue = std::list<Command>;
    TimedCommandsQueue timedCommandsQueue;
    
    Esp32Thread<TimedCommands> thread;
    mutex threadMutex;
    unique_lock<mutex> threadLock;
    condition_variable cv;

    AcController* acController;
    Logger* logger;

	const int LIST_BEGIN = -1;
	const int LIST_END = 5555555;
};
