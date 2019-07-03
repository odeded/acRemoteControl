#pragma once 

#include "electraAcRecordedSender/electraAcRecordedSender.h"
#include "RgbLed.h"
#include "Logger.h"
#include <queue>
#include <string>

class AcController
{
public:
    struct AcCmd {
        bool on;
        enum _type {cool, heat} type;
        int temp;
        inline bool operator==(const AcCmd& a)
        {
            return (a.on==on) && (a.type==type) && (a.temp==temp);
        }
    };

    AcController(int irLed, RgbLed& rgbLed, Logger& logger);

    void sendCommand(AcCmd cmd);

    bool checkAndHandleQueue();


    void fillStatus(string& resultMsg);
    void statusOveride(AcCmd& cmd);
protected:

    void setCurrentStatus(AcCmd& cmd);

    electraAcRecordedSender acIrSender;
    AcCmd currentStatus;
    RgbLed& rgbLed;
    Logger& logger;
    std::queue<AcCmd> commandsQueue;
};
