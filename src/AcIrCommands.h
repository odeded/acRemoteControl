#pragma once

#include "CommandsProvider.h"
#include "IAcIrCommands.h"
//#include "Electra-AC-Remote-Encoder/ElectraAcRemoteSender.h"
#include "electraAcRecordedSender/electraAcRecordedSender.h"
#include <queue>

class AcIrCommands// : public IAcIrCommands
{
public:
    //AcIrCommands(int ledPin);

    static bool setCoolCommand(std::string& cmd,std::string& resultMsg);
    static bool setHeatCommand(std::string& cmd,std::string& resultMsg);
    static bool setOff(std::string& cmd,std::string& resultMsg);

    static void setCoolTemp(int temp);
    static void setHeatTemp(int temp);

    static bool checkAndHandleQueue();

    static void sendSome();

private:
    static int getTemp(std::string temp);

    static int IRledPin;
    //static ElectraAcRemoteSender acIrSender;
    static electraAcRecordedSender acIrSender;

public:

    struct AcCmd {
        bool on;
        enum _type {cool, heat} type;
        int temp;
    };
    static std::queue<AcCmd> commandsQueue;
};
