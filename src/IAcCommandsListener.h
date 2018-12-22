#pragma once

#include <string>

class IAcCommandsListener
{
public:
    virtual bool start() = 0;
    virtual bool stop() = 0;
};
