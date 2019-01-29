#include "SetPinCommands.h"
#include <Arduino.h>

bool SetPinCommands::setPin(std::string command, std::string& result)
{
    int pin = getPinNumber(command);
    if (pin == -1)
    {
        result = "ERROR: wrong pin number";
    }
    pinMode(pin, OUTPUT);
    digitalWrite(pin, HIGH);
    result = "Set done";
    return true;
}

bool SetPinCommands::unsetPin(std::string command, std::string& result)
{
    int pin = getPinNumber(command);
    if (pin == -1)
    {
        result = "ERROR: wrong pin number";
        return false;
    }
    pinMode(pin, OUTPUT);
    digitalWrite(pin, LOW);
    result = "Unset done";
    return true;
}

int SetPinCommands::getPinNumber(std::string& str)
{
    int pin = atoi(str.c_str());
    if (pin <=0 || pin>=39)
    {
        return -1;
    }

    return pin;
}