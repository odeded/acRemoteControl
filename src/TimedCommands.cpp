#include "TimedCommands.h"
#include "timeManager.h"
#include <sstream>
#include <algorithm>

TimedCommands::commandsQueue TimedCommands::cmdQueue;
const string TimedCommands::Type::Cool = "cool";
const string TimedCommands::Type::Heat = "heat";

bool TimedCommands::setTimer(std::string &cmd, std::string &resultMsg)
{
    Command on, off;
    if (cmd.empty())
    {
        resultMsg = TimeManager::getTimeInDay();
        return true;
    }

    if (!parseCommand(cmd, on, off, resultMsg))
    {
        return false;
    }

    addCommand(on);
    addCommand(off);
    resultMsg = "Timer set";    

    return true;
}

void TimedCommands::handleTimedCommands()
{

}

void TimedCommands::addCommand(const Command cmd)
{
    
}

bool TimedCommands::parseCommand(const string str, Command& on, Command& off, string& errorMsg)
{
    std::vector<std::string> elems;
    std::stringstream ss(str);
    std::string item;
    while (std::getline(ss, item, ' ')) 
    {
        elems.push_back(item);
    }

    // timer Cool 24 16:00 19:00
    if (elems.size() != 4)
    {
        errorMsg = "Wrong params";
        return false;
    }

    on.acCommand.on = true;
    off.acCommand.on = false;

    //type
    std::transform(elems[0].begin(), elems[0].end(), elems[0].begin(), ::tolower);
    if (elems[0] == Type::Cool)
    {
        on.acCommand.type = off.acCommand.type = AcController::AcCmd::cool;
    }
    else if (elems[0] == Type::Heat)
    {
        on.acCommand.type = off.acCommand.type = AcController::AcCmd::heat;
    }
    else
    {
        errorMsg = "Type error";
        return false;
    }
    
    //temp
    int temp = std::atoi(elems[1].c_str());
    if (temp == 0)
    {
        errorMsg = "Wrong temp";
        return false;
    }
    on.acCommand.temp = off.acCommand.temp = temp;

    int startTime = TimeManager::timeStandardToSecs(elems[2].c_str());
    int endTime   = TimeManager::timeStandardToSecs(elems[3].c_str());
    if (startTime == -1 || endTime == -1)
    {
        errorMsg = "Wrong times";
        return false;
    }
    on.timeInDay = startTime;
    off.timeInDay = endTime;

    return true;
}
