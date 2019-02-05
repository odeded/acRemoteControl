#pragma once

#include "CommandsProvider.h"
#include "IAcIrCommands.h"

class AcIrCommands// : public IAcIrCommands
{
public:
    AcIrCommands(int ledPin);

    void sendOn(int temp);
    void sendOff(int temp);

    void setTemp(int temp);

    void sendSome();
private:
    void sendBuffer(int* buf, int bufLen);
    void pulseIR(long microsecs);

void irRemoteSendRaw (int *codes, int len);
void space (int time);
void mark (int time);



    int IRledPin;
};
