
#include "CommandsProvider.h"

bool CommandsProvider::registerCommand(std::string &commandStr, const command_t &command_cb)
{
    if (commandStr.empty())
    {
        return false;
    }

    commandsMap[commandStr] = command_cb;

    return true;
}

bool CommandsProvider::runCommand(std::string commandFullStr, std::string& result)
{
    //find command & args
    int i = commandFullStr.find(' ');
    std::string cmdArgs;
    if (i != std::string::npos && commandFullStr.size() > i+1)
    {
        cmdArgs = commandFullStr.substr(i+1);
    }
    if (i == std::string::npos)
    {
        i = commandFullStr.size();
    }
    std::string commandStr = commandFullStr.substr(i);

    auto cmdItr = commandsMap.find(commandStr);
    if (cmdItr == commandsMap.end())
    {
        result = "ERROR: Command not fould";
        return false;
    }
    
    command_t& cmdFunc = cmdItr->second;
    return cmdFunc(cmdArgs, result);
}
