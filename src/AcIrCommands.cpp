#include "AcIrCommands.h"
#include "TimedCommands.h"
#include "timeManager.h"
#include <Arduino.h>

RgbLed* AcIrCommands::rgbLed = nullptr;
AcController* AcIrCommands::acController = nullptr;

void AcIrCommands::setAcController(AcController& controller)
{
    acController = &controller;
}

void AcIrCommands::setRgbLed(RgbLed& led)
{
    rgbLed = &led;
}

bool AcIrCommands::setAcStatus(string& cmd, string& resultMsg)
{
    if (rgbLed == nullptr || acController == nullptr)
        return false;

    if (cmd.empty())
    {
        string str;
        resultMsg = TimeManager::getTimeInDay();
        acController->fillStatus(str);
        resultMsg += "\n" + str;
        TimedCommands::get()->getTimersStatus(str);
        resultMsg += "\n" + str;
        return true;
    }

    AcController::AcCmd acCmd;
    string lowCmd = cmd;
    std::transform(cmd.begin(), cmd.end(), lowCmd.begin(), ::tolower);
    if (lowCmd.find("off") != string::npos)
    {
        acCmd.on = false;
    }
    else if (lowCmd.find("cool") != string::npos)
    {
        acCmd.on = true;
        acCmd.type = AcController::AcCmd::cool;
    }
    else if (lowCmd.find("heat") != string::npos)
    {
        acCmd.on = true;
        acCmd.type = AcController::AcCmd::heat;
    }
    else
    {
        resultMsg = "ERROR: wrong status: ";
        resultMsg += cmd;
        return false;
    }
    
    acController->statusOveride(acCmd);
    return true;
}

bool AcIrCommands::setOff(std::string &cmd, std::string &resultMsg)
{
    if (rgbLed == nullptr || acController == nullptr)
        return false;
    rgbLed->setColor(RgbLed::color::GREEN);
    //commandsQueue.push(AcCmd{false, AcCmd::cool, 25});
    acController->sendCommand(AcController::AcCmd{false, AcController::AcCmd::cool, 25});
    resultMsg = "Turning off";
    return true;
}

bool AcIrCommands::setCoolCommand(std::string &cmd, std::string &resultMsg)
{
    if (rgbLed == nullptr || acController == nullptr)
        return false;
    int temp = getTemp(cmd);
    if (temp == 0)
    {
        resultMsg = "Not a valid temperature";
        return false;
    }
    //AcIrCommands::setCoolTemp(temp);
    acController->sendCommand(AcController::AcCmd{true, AcController::AcCmd::cool, temp});
    resultMsg = "OK - Cooling " + cmd + " degrees";
    return true;
}

bool AcIrCommands::setHeatCommand(std::string &cmd, std::string &resultMsg)
{
    if (rgbLed == nullptr || acController == nullptr)
        return false;
    int temp = getTemp(cmd);
    if (temp == 0)
    {
        resultMsg = "Not a valid temperature";
        return false;
    }
    //AcIrCommands::setHeatTemp(temp);
    acController->sendCommand(AcController::AcCmd{true, AcController::AcCmd::heat, temp});
    resultMsg = "OK - Heating " + cmd + " degrees";
    return true;
}

int AcIrCommands::getTemp(std::string tempStr)
{
    int temp = atoi(tempStr.c_str());
    if (temp >= 16 && temp <= 30)
    {
        return temp;
    }

    return 0;
}


//void AcIrCommands::sendSome()
//{
//    Serial.println("Sending 4...");
    //acIrSender.sendAcCommand(ElectraAcRemoteSender::State::on,
    //                         ElectraAcRemoteSender::Fan::high,
    //                        ElectraAcRemoteSender::Mode::cool,
    //                         26);

    //struct airCon newAc;l
    //int *codes;

    //int fanStrength = 3;
    //int acMode = HEAT; //1=cold, 2=hot
    //int temperature = 27;
    //int state = 0; //0=on, 1=off
    //int acSwing = SWING_OFF;

    //codes = getCodes(&newAc,fanStrength,acMode,temperature,state,acSwing);
    //irRemoteSendRaw(codes,TIMINGS_LENGTH);
//}
