#pragma once

#include "CommandsProvider.h"
#include <string>

class ICommandsListener
{
public:
    virtual bool start() = 0;
    virtual bool stop() = 0;

    CommandsProvider& commandsProvider;

public:
    ICommandsListener(CommandsProvider& _commandProvider) : commandsProvider(_commandProvider)
    {}
};
