#include "AcIrCommands.h"
//#include "Electra-AC-Remote-Encoder/electraAcRemoteSender.h"
#include "electraAcRecordedSender/electraAcRecordedSender.h"
#include <Arduino.h>

//AcIrCommands::AcIrCommands(int ledPin) : IRledPin(ledPin), acIrSender(IRledPin)
//{
//}

int AcIrCommands::IRledPin = 21;
//ElectraAcRemoteSender AcIrCommands::acIrSender(AcIrCommands::IRledPin);
//electraAcRecordedSender AcIrCommands::acIrSender(AcIrCommands::IRledPin);

bool AcIrCommands::toCool = false;
bool AcIrCommands::toHeat = false;
int  AcIrCommands::toTemp = 0;

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

bool AcIrCommands::setOff(std::string &cmd, std::string &resultMsg)
{
    //acIrSender.sendAcCommand(ElectraAcRemoteSender::State::on,
    //                         ElectraAcRemoteSender::Fan::high,
    //                         ElectraAcRemoteSender::Mode::cool,
    //                         25);
    //acIrSender.sendAcCoolCommand(25);
    toCool = true;
    toTemp = 25;
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
    //acIrSender.sendAcCommand(ElectraAcRemoteSender::State::on,
    //                         ElectraAcRemoteSender::Fan::high,
    //                         ElectraAcRemoteSender::Mode::cool,
    //                         temp);
    toCool = true;
    toTemp = temp;
    //acIrSender.sendAcCoolCommand(temp);
}

void AcIrCommands::setHeatTemp(int temp)
{
    //acIrSender.sendAcCommand(ElectraAcRemoteSender::State::on,
    //                         ElectraAcRemoteSender::Fan::high,
    //                         ElectraAcRemoteSender::Mode::heat,
    //                         temp);
    toHeat = true;
    toTemp = temp;
    //acIrSender.sendAcHeatCommand(temp);
}
