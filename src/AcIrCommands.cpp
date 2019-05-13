#include "AcIrCommands.h"
//#include "Electra-AC-Remote-Encoder/electraAcRemoteSender.h"
#include "electraAcRecordedSender/electraAcRecordedSender.h"
#include <Arduino.h>

//AcIrCommands::AcIrCommands(int ledPin) : IRledPin(ledPin), acIrSender(IRledPin)
//{
//}

int AcIrCommands::IRledPin = 21;
//ElectraAcRemoteSender AcIrCommands::acIrSender(AcIrCommands::IRledPin);
electraAcRecordedSender AcIrCommands::acIrSender(AcIrCommands::IRledPin);

std::queue<AcIrCommands::AcCmd> AcIrCommands::commandsQueue;
//electraAcRecordedSender AcIrCommands::acSender(21);

RgbLed AcIrCommands::rgbLed(25, 26, 27);

bool AcIrCommands::setOff(std::string &cmd, std::string &resultMsg)
{
    commandsQueue.push(AcCmd{false, AcCmd::cool, 25});
    resultMsg = "Turning off";
    return true;
}

bool AcIrCommands::setCoolCommand(std::string &cmd, std::string &resultMsg)
{
    int temp = getTemp(cmd);
    if (temp == 0)
    {
        resultMsg = "Not a valid temperature";
        return false;
    }
    AcIrCommands::setCoolTemp(temp);
    resultMsg = "OK - Cooling " + cmd + " degrees";
    return true;
}

bool AcIrCommands::setHeatCommand(std::string &cmd, std::string &resultMsg)
{
    int temp = getTemp(cmd);
    if (temp == 0)
    {
        resultMsg = "Not a valid temperature";
        return false;
    }
    AcIrCommands::setHeatTemp(temp);
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

void AcIrCommands::setCoolTemp(int temp)
{
    commandsQueue.push(AcCmd{true, AcCmd::cool, temp});
}

void AcIrCommands::setHeatTemp(int temp)
{
    commandsQueue.push(AcCmd{true, AcCmd::heat, temp});
}

bool AcIrCommands::checkAndHandleQueue()
{
    if (commandsQueue.empty())
    {
        return false;
    }

    AcCmd cmd = commandsQueue.front();
    commandsQueue.pop();

    if (!cmd.on)
    {
        acIrSender.sendAcCoolCommand(25);
        rgbLed.setColor(0, 255, 0);
        //logger.logLine("Sent off IR");
    }
    else
    {
        if (cmd.type == AcCmd::cool)
        {
            acIrSender.sendAcCoolCommand(cmd.temp);
            rgbLed.setColor(0, 0, 255);
            //logger.log("Sent cool temp=");
            //logger.logLine(cmd.temp);
        }
        else if (cmd.type == AcCmd::heat)
        {
            acIrSender.sendAcHeatCommand(cmd.temp);
            rgbLed.setColor(255, 0, 0);
            //logger.log("Sent heat temp=");
            //logger.logLine(cmd.temp);
        }
    }

    return true;
}

void AcIrCommands::sendSome()
{
    Serial.println("Sending 4...");
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
}
