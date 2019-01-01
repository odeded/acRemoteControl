#pragma once

#include "CommandsProvider.h"
#include "IAcIrCommands.h"

class AcIrCommands : public IAcIrCommands
{
public:
    void sendOn(int temp);
    void sendOff(int temp);

    void setTemp(int temp);
};
