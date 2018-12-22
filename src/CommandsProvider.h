#pragma once

#include <string>
#include <map>
#include <functional>

class CommandsProvider
{
public:
    using command_t = std::function<bool(std::string)>;

    bool registerCommand(std::string &commandStr, const command_t &command_cb);

    bool runCommand(std::string& commandFullStr);

private:
    std::map<std::string, command_t> commandsMap;
};
