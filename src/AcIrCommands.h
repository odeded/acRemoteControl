#pragma once

#include "CommandsProvider.h"

class AcIrCommands
{
public:
    void sendOn(int temp);
    void sendOff(int temp);

    void setTemp(int temp);
};
