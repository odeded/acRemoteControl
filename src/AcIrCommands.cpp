#include "AcIrCommands.h"
#include "Electra-AC-Remote/electraAcRemoteEncoder.h"
#include <Arduino.h>

AcIrCommands::AcIrCommands(int ledPin) : IRledPin(ledPin)
{
    pinMode(IRledPin, OUTPUT);      
}

void AcIrCommands::sendSome()
{
    Serial.println("Sending 2...");
    //digitalWrite(IRledPin, HIGH); // this takes about 3 microseconds to happen
    //delayMicroseconds(1000);      
    //digitalWrite(IRledPin, LOW); // this takes about 3 microseconds to happen
    //to26hot//int buf[]={3160, 3020, 1020, 1000, 980, 2020, 2000, 1980, 2040, 940, 1060, 920, 1040, 960, 1040, 960, 1020, 980, 1060, 1940, 2000, 2020, 980, 1020, 2020, 940, 1020, 1000, 980, 1020, 1020, 940, 1020, 960, 1040, 960, 1060, 940, 1040, 960, 1040, 960, 1000, 1020, 1000, 1000, 1000, 960, 1040, 980, 1040, 940, 1020, 960, 1040, 960, 1040, 1960, 2000, 1020, 3060, 3040, 1040, 940, 1020, 2020, 2000, 1960, 2020, 980, 1040, 980, 1000, 960, 1020, 1020, 1000, 980, 1000, 1960, 2020, 2020, 1000, 960, 2020, 980, 1020, 960, 1040, 960, 1040, 980, 1020, 980, 1000, 980, 1020, 960, 1040, 960, 1020, 960, 1040, 980, 1060, 920, 1040, 960, 1060, 940, 1020, 960, 1040, 960, 1040, 960, 1040, 1980, 2000, 980, 3100, 3020, 1020, 960, 1020, 1980, 2020, 2000, 2000, 960, 1060, 940, 1060, 940, 1040, 960, 1040, 960, 1020, 1980, 2000, 2020, 1000, 980, 1980, 1020, 1000, 960, 1020, 1020, 1000, 960, 1040, 1000, 1000, 980, 980, 1020, 1000, 960, 1020, 980, 1020, 980, 1020, 960, 1040, 960, 1040, 960, 1040, 960, 1000, 1020, 1000, 960, 1040, 1960, 2000, 1000, 4060};
    //on26:
    //int buf[]={3140, 4060, 1940, 2040, 1960, 2020, 2040, 960, 1020, 1000, 980, 1020, 980, 1020, 980, 1020, 980, 2000, 1940, 2040, 980, 1000, 2040, 960, 1000, 1000, 1020, 1000, 980, 1020, 940, 1020, 980, 1020, 1020, 980, 980, 1020, 1000, 980, 980, 1020, 980, 1020, 1000, 1020, 1000, 1000, 1000, 960, 1000, 1020, 940, 1020, 980, 2040, 2000, 1020, 3060, 4020, 2040, 1940, 2000, 2000, 2020, 980, 980, 1020, 1000, 1000, 1020, 980, 1020, 1000, 960, 2000, 1960, 2020, 1000, 1000, 2020, 980, 1020, 1020, 980, 1000, 980, 980, 1020, 980, 1040, 960, 980, 1020, 960, 1040, 960, 1020, 980, 1020, 1020, 1020, 960, 1020, 980, 1000, 1000, 1020, 920, 1040, 940, 1060, 940, 2060, 1940, 1040, 3080, 4040, 2040, 1960, 2000, 2020, 1980, 1020, 920, 1040, 960, 1040, 940, 1060, 1000, 1000, 1000, 2020, 1940, 2020, 940, 1060, 2000, 1020, 920, 1040, 960, 1040, 940, 1060, 1000, 1000, 1000, 1000, 960, 1060, 920, 1040, 980, 1040, 980, 1020, 920, 1040, 960, 1040, 940, 1060, 960, 1040, 960, 1060, 920, 1040, 940, 2060, 1980, 1040, 4000};
    //clear:
    //int buf[]={3160, 3040, 960, 1000, 1040, 960, 1040, 980, 980, 1980, 2020, 1000, 980, 1000, 1040, 940, 1000, 1000, 1040, 960, 1000, 2000, 1000, 1000, 2000, 980, 1040, 960, 1000, 1000, 1000, 980, 1040, 960, 1000, 1000, 1040, 960, 1000, 1000, 960, 1020, 1000, 1000, 1020, 1020, 960, 1000, 1040, 980, 980, 1020, 1020, 940, 960, 1040, 1000, 1000, 1020, 960, 1040, 1960, 2040, 980, 3000, 3100, 1000, 1020, 980, 1020, 1000, 1000, 940, 2020, 2060, 920, 1040, 960, 1000, 1000, 1020, 1000, 1000, 960, 1020, 2020, 1000, 960, 2040, 960, 1040, 960, 960, 1020, 1040, 960, 1040, 980, 1020, 980, 980, 980, 980, 1020, 1000, 1000, 1020, 1000, 980, 1000, 1040, 980, 1000, 980, 1020, 960, 1000, 980, 1000, 1000, 1040, 960, 1020, 980, 1000, 2000, 1960, 1020, 3080, 3080, 980, 1000, 980, 1020, 940, 1020, 1040, 1960, 2040, 960, 1000, 1000, 1020, 1020, 980, 1000, 960, 1000, 980, 2020, 960, 1020, 2000, 1000, 980, 1020, 1020, 1000, 980, 1020, 1020, 980, 980, 980, 1040, 960, 1040, 980, 980, 1020, 960, 1000, 1040, 1000, 960, 1020, 1020, 960, 960, 1040, 1000, 1020, 960, 1000, 1040, 960, 980, 1000, 1020, 2000, 2020, 960, 4080};
    //int buf[]={3000,3000,1000,1000,1000,2000,2000,2000,2000,1000,1000,2000,2000,1000,1000,1000,1000,2000,2000,2000,1000,1000,2000,1000,1000,1000,1000,1000,1000,1000,1000,1000,1000,1000,1000,1000,1000,1000,1000,1000,1000,1000,1000,1000,1000,1000,1000,1000,1000,1000,1000,1000,1000,1000,1000,2000,2000,1000,3000,3000,1000,1000,1000,2000,2000,2000,2000,1000,1000,2000,2000,1000,1000,1000,1000,2000,2000,2000,1000,1000,2000,1000,1000,1000,1000,1000,1000,1000,1000,1000,1000,1000,1000,1000,1000,1000,1000,1000,1000,1000,1000,1000,1000,1000,1000,1000,1000,1000,1000,1000,1000,1000,1000,2000,2000,1000,3000,3000,1000,1000,1000,2000,2000,2000,2000,1000,1000,2000,2000,1000,1000,1000,1000,2000,2000,2000,1000,1000,2000,1000,1000,1000,1000,1000,1000,1000,1000,1000,1000,1000,1000,1000,1000,1000,1000,1000,1000,1000,1000,1000,1000,1000,1000,1000,1000,1000,1000,1000,1000,1000,1000,2000,2000,1000,4000,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0};

    //sendBuffer(buf, sizeof(buf)/sizeof(int));
    struct airCon newAc;
    int *codes;

    int fanStrength = 3;
    int acMode = HEAT; //1=cold, 2=hot
    int temperature = 27;
    int state = 0; //0=on, 1=off
    int acSwing = SWING_OFF;

    codes = getCodes(&newAc,fanStrength,acMode,temperature,state,acSwing);
    irRemoteSendRaw(codes,TIMINGS_LENGTH);
}

// This procedure sends a 38KHz pulse to the IRledPin
// for a certain # of microseconds. We'll use this whenever we need to send codes
void AcIrCommands::pulseIR(long microsecs)
{
    // we'll count down from the number of microseconds we are told to wait
    cli(); // this turns off any background interrupts

    while (microsecs > 0)
    {
        // 38 kHz is about 13 microseconds high and 13 microseconds low
        digitalWrite(IRledPin, HIGH); // this takes about 3 microseconds to happen
        delayMicroseconds(10);        // hang out for 10 microseconds, you can also change this to 9 if its not working
        digitalWrite(IRledPin, LOW);  // this also takes about 3 microseconds
        delayMicroseconds(10);        // hang out for 10 microseconds, you can also change this to 9 if its not working

        // so 26 microseconds altogether
        microsecs -= 26;
    }

    sei(); // this turns them back on
}

void AcIrCommands::sendBuffer(int *buf, int bufLen)
{
    for (unsigned int i = 0; i < bufLen; i++)
    {
        if (i & 1)
            delayMicroseconds(buf[i]);
        else
            pulseIR(buf[i]);
    }
}

void AcIrCommands::mark (int time)
{
    int i = 0;
    int cycles = 0;
    cycles = time / 28; /*The carrier wave is 38kHz so each period is 26 micro seconds. After checking the output with an IR receiver, 28 seemed to be better
                          as the hardware isn't as precise as you would hope */
    if (cycles == 0){
        cycles = 1;
    }
    for (i = 0;i < cycles;i++){
        digitalWrite(IRledPin, HIGH);
        delayMicroseconds(13);
        digitalWrite(IRledPin, LOW);
        delayMicroseconds(13);
    }
}

void AcIrCommands::space (int time)
{
    digitalWrite(IRledPin, LOW);
    if (time > 0){
        delayMicroseconds(time);
    }
}

void AcIrCommands::irRemoteSendRaw (int *codes, int len)
{
    int i;

    for (i = 0;i < len;i++){
        if (i%2 == 0){
            mark(codes[i]);
        }
        else{
            space(codes[i]);
        }
    }
}