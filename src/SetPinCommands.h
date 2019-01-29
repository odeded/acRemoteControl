#include "CommandsProvider.h"
#include <string>

class SetPinCommands
{
public:
    static bool setPin(std::string command, std::string& result);

    static bool unsetPin(std::string command, std::string& result);

private:
    static int getPinNumber(std::string& str);

};
