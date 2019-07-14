#include "TimedCommands.h"
#include "timeManager.h"
#include <sstream>
#include <algorithm>
#include <stdio.h>
#include <Arduino.h>

TimedCommands *TimedCommands::instance = nullptr;
const string TimedCommands::Type::Cool = "cool";
const string TimedCommands::Type::Heat = "heat";

TimedCommands *TimedCommands::get()
{
    if (instance == nullptr)
        instance = new TimedCommands;

    return instance;
}

bool TimedCommands::setTimer(std::string &cmd, std::string &resultMsg)
{
    Command on, off;
    if (cmd.empty())
    {
        get()->getTimersStatus(resultMsg);
        return true;
    }

    std::transform(cmd.begin(), cmd.end(), cmd.begin(), ::tolower);
    if (cmd.find("clear") != string::npos)
    {
        resultMsg = "Clearing timers";
        get()->clearList();
        return true;
    }

    if (!parseCommand(cmd, on, off, resultMsg))
    {
        resultMsg = "Failed parsing";
        return false;
    }

    get()->addCommand(on);
    get()->addCommand(off);
    resultMsg = "Timer set";

    return true;
}

TimedCommands::TimedCommands()
    : threadLock(threadMutex, std::defer_lock), acController(nullptr), logger(nullptr)
{
    clearList();
    thread.start(Esp32Thread<TimedCommands>::RunnningParams(this, nullptr), "TimedCommands");
}

void TimedCommands::setParams(AcController &controller, Logger &_logger)
{
    acController = &controller;
    logger = &_logger;
}

void TimedCommands::clearList()
{
    timedCommandsQueue.clear();
    Command cmd;
    cmd.timeInDay = LIST_BEGIN;
    timedCommandsQueue.push_front(cmd);
    cmd.timeInDay = LIST_END;
    timedCommandsQueue.push_back(cmd);
    unlockMainLoop();
}

bool TimedCommands::setMutex(std::string &cmd, std::string &resultMsg)
{
    get()->unlockMainLoop();
    return true;
}
chrono::seconds TimedCommands::getTimeWait()
{
    auto commandItr = (++(timedCommandsQueue.begin()));
    auto command = *commandItr;

    int waitSeconds;
    if (command.timeInDay == LIST_END)
    {
        waitSeconds = 60*60;
    }
    else
    {
        waitSeconds = command.timeInDay - TimeManager::getSecondsInDay();
        if (waitSeconds < 0)
            waitSeconds += 60*60*24;
    }
    logger->log("Waiting for seconds = ");
    logger->logLine(waitSeconds);
    return chrono::seconds(waitSeconds);
}

void TimedCommands::unlockMainLoop()
{
    cv.notify_all();
}

void TimedCommands::mainLoop(void *)
{
    delay(10 * 1000);
    cv.notify_all();
    while (true)
    {
        threadLock.lock();
        logger->logLine("----sleeping....");
        if (cv.wait_for(threadLock, getTimeWait()) == cv_status::timeout) //threadMutex.try_lock_for(getTimeWait()))
        {
            //timeout expired
            logger->logLine("----Timed out");
        }
        else
        {
            logger->logLine("----got mutex");
        }

        checkAndSendCommand();

        threadLock.unlock();
    }
}

void TimedCommands::checkAndSendCommand()
{
    //take second item
    auto commandItr = (++(timedCommandsQueue.begin()));
    auto command = *commandItr;

    if (command.timeInDay == LIST_BEGIN || command.timeInDay == LIST_END)
    {
        return;
    }

    if (command.timeInDay < TimeManager::getSecondsInDay() + 30)
    {
        logger->logLine("sending command");
        //send and push to the end
        acController->sendCommand(command.acCommand);
        timedCommandsQueue.erase(commandItr);
        timedCommandsQueue.insert(--(timedCommandsQueue.end()), command);
    }
}

void TimedCommands::getTimersStatus(string &res)
{
    res = "";
    if (timedCommandsQueue.size() == 2)
    {
        res = "No timers";
    }
    for (auto cmd = timedCommandsQueue.begin(); cmd != timedCommandsQueue.end(); ++cmd)
    {
        if ((*cmd).timeInDay == LIST_BEGIN || (*cmd).timeInDay == LIST_END)
            continue;

        res += TimeManager::timeSecsToStandard((*cmd).timeInDay);
        if ((*cmd).acCommand.on)
        {
            if ((*cmd).acCommand.type == AcController::AcCmd::cool)
                res += "-Cool-";
            if ((*cmd).acCommand.type == AcController::AcCmd::heat)
                res += "-Heat-";
            char numstr[21];
            sprintf(numstr, "%d", (*cmd).acCommand.temp);
            res += numstr;
        }
        else
        {
            res += "-Off";
        }
        res += "\n";
    }
}

void TimedCommands::addCommand(const Command newCommand)
{
    //if (timedCommandsQueue.size() == 2)
    //{
    //	timedCommandsQueue.insert(++timedCommandsQueue.begin(), newCommand);
    //	return;
    //}

    int curTime = TimeManager::getSecondsInDay();

    if (newCommand.timeInDay > curTime)
    {
        auto cmd = timedCommandsQueue.begin();
        for (; cmd != timedCommandsQueue.end(); ++cmd)
        {
            if ((*cmd).timeInDay == LIST_BEGIN)
                continue;
            if ((*cmd).timeInDay > newCommand.timeInDay)
                break;
            if ((*cmd).timeInDay < curTime)
                break;
        }
        timedCommandsQueue.insert(cmd, newCommand);
    }
    else
    {
        auto cmd = timedCommandsQueue.rbegin();
        for (; cmd != timedCommandsQueue.rend(); ++cmd)
        {
            if ((*cmd).timeInDay == LIST_END)
                continue;
            if ((*cmd).timeInDay < newCommand.timeInDay)
                break;
            if ((*cmd).timeInDay > curTime)
                break;
        }
        timedCommandsQueue.insert(cmd.base(), newCommand);
    }
    unlockMainLoop();
}

bool TimedCommands::parseCommand(const string str, Command &on, Command &off, string &errorMsg)
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
    int endTime = TimeManager::timeStandardToSecs(elems[3].c_str());
    if (startTime == -1 || endTime == -1)
    {
        errorMsg = "Wrong times";
        return false;
    }
    on.timeInDay = startTime;
    off.timeInDay = endTime;

    return true;
}
