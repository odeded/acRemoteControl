#include "ElectraAcRemoteSender.h"
#include "electraAcRemoteEncoder.h"
#include <Arduino.h>

ElectraAcRemoteSender::ElectraAcRemoteSender(int _IRledPin) : IRledPin(_IRledPin)
{
    pinMode(IRledPin, OUTPUT);
}

void ElectraAcRemoteSender::mark(int time)
{
    int i = 0;
    int cycles = 0;
    cycles = time / 28; /*The carrier wave is 38kHz so each period is 26 micro seconds. After checking the output with an IR receiver, 28 seemed to be better
                          as the hardware isn't as precise as you would hope */
    if (cycles == 0)
    {
        cycles = 1;
    }
    for (i = 0; i < cycles; i++)
    {
        digitalWrite(IRledPin, HIGH);
        delayMicroseconds(13);
        digitalWrite(IRledPin, LOW);
        delayMicroseconds(13);
    }
}

void ElectraAcRemoteSender::space(int time)
{
    digitalWrite(IRledPin, LOW);
    if (time > 0)
    {
        delayMicroseconds(time);
    }
}

void ElectraAcRemoteSender::irRemoteSendRaw(int *codes, int len)
{
    int i;

    for (i = 0; i < len; i++)
    {
        if (i % 2 == 0)
        {
            mark(codes[i]);
        }
        else
        {
            space(codes[i]);
        }
    }
}

void ElectraAcRemoteSender::sendAcCommand(State state, Fan fan, Mode mode, int temp)
{
    struct airCon newAc;
    int *codes;

    codes = getCodes(&newAc, fan, mode, temp, state, SWING_OFF);
    irRemoteSendRaw(codes, TIMINGS_LENGTH);
}
