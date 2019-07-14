#include "AcController.h"

AcController::AcController(int irLed, RgbLed &rgbled, Logger &_logger) : 
    acIrSender(irLed), rgbLed(rgbled), logger(_logger)
{
    currentStatus.on = false;
    setCurrentStatus(currentStatus);
}

void AcController::sendCommand(AcController::AcCmd cmd)
{
    rgbLed.setColor(RgbLed::color::GREEN);
    commandsQueue.push(cmd);
}

bool AcController::checkAndHandleQueue()
{
    if (commandsQueue.empty())
    {
        return false;
    }

    AcCmd cmd = commandsQueue.front();
    commandsQueue.pop();
    if (currentStatus == cmd)
    {
        setCurrentStatus(cmd);
        logger.logLine("Same command, doing nothing");
        return false;
    }

    if (!currentStatus.on && cmd.on)
    {
        if (cmd.type == AcCmd::cool)
        {
            logger.logLine("Sending cool IR");
            acIrSender.sendAcCoolCommand(cmd.temp);
            setCurrentStatus(cmd);
            return true;
        }
        else if (cmd.type == AcCmd::heat)
        {
            logger.logLine("Sending heat IR");
            acIrSender.sendAcCoolCommand(cmd.temp);
            setCurrentStatus(cmd);
            return true;
        }
        logger.logLine("ERROR: got wrong command on");
    }
    else if (currentStatus.on && !cmd.on)
    {
        logger.logLine("Sending OFF");
        acIrSender.sendAcCoolCommand(25);
        setCurrentStatus(cmd);
        return true;
    }
    logger.logLine("ERROR: got wrong command");
    setCurrentStatus(cmd);
    return false;
}

void AcController::setCurrentStatus(AcCmd &cmd)
{
    currentStatus = cmd;
    if (currentStatus.on)
    {
        if (currentStatus.type == AcCmd::cool)
            rgbLed.setColor(RgbLed::color::BLUE);
        if (currentStatus.type == AcCmd::heat)
            rgbLed.setColor(RgbLed::color::RED);
    }
    else
    {
        rgbLed.setColor(RgbLed::color::BLACK);
    }
}

void AcController::fillStatus(string &resultMsg)
{
    resultMsg = "AC Status: ";
    if (currentStatus.on)
    {
        if (currentStatus.type == AcCmd::cool)
            resultMsg += "Cooling";
        else if (currentStatus.type == AcCmd::heat)
            resultMsg += "Heating";
    }
    else
    {
        resultMsg += "Off";
    }
}

void AcController::statusOveride(AcCmd& cmd)
{
    setCurrentStatus(cmd);
}
