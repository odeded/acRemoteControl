#pragma once

#include "CommandsProvider.h"
//#include "IAcIrCommands.h"
#include "AcController.h"
#include "RgbLed.h"
//#include "Electra-AC-Remote-Encoder/ElectraAcRemoteSender.h"
#include "electraAcRecordedSender/electraAcRecordedSender.h"
#include <queue>

class AcIrCommands// : public IAcIrCommands
{
public:
    //AcIrCommands(int irLedPin, RgbLed rgbLed);

    static void setAcController(AcController& controller);
    static void setRgbLed(RgbLed& led);

    static bool setAcStatus(std::string& cmd,std::string& resultMsg);

    static bool setCoolCommand(std::string& cmd,std::string& resultMsg);
    static bool setHeatCommand(std::string& cmd,std::string& resultMsg);
    static bool setOff(std::string& cmd,std::string& resultMsg);

private:
    static int getTemp(std::string temp);

    static RgbLed* rgbLed;
    static AcController* acController;
};
