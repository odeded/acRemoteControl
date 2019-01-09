#pragma once

#include <string>
#include <map>
#include <functional>

class CommandsProvider
{
public:
    using command_t = std::function<bool(std::string& cmd,std::string& resultMsg)>;

    bool registerCommand(std::string &commandStr, const command_t &command_cb);

    bool runCommand(std::string commandFullStr, std::string& result);

private:
    std::map<std::string, command_t> commandsMap;
};
