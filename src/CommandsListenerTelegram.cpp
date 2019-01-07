#include "ConstsPrivate.h"
#include "CommandsListenerTelegram.h"
#include "Esp32Thread.h"

int Bot_mtbs = 1000; //mean time between scan messages
long Bot_lasttime;   //last time messages' scan has been done

CommandsListenerTelegram::CommandsListenerTelegram(
    Logger &_logger, WifiConnector &_wifiConnector, CommandsProvider &_commandsProvider)
    : logger(_logger), wifiConnector(_wifiConnector),
      ICommandsListener(_commandsProvider), client(),
      bot(BOTtoken, client)
{
}

bool CommandsListenerTelegram::start()
{
    Esp32Thread<CommandsListenerTelegram> t1;
    t1.start(Esp32Thread<CommandsListenerTelegram>::RunnningParams(this, nullptr), "TelegramCmd", 0);

    return true;
}

bool CommandsListenerTelegram::stop()
{
    return true;
}

void CommandsListenerTelegram::mainLoop(void *)
{
    wifiConnector.connect();

    logger.logLine("before loop");
    while (true)
    {
        if (millis() > Bot_lasttime + Bot_mtbs)
        {
            int numNewMessages = bot.getUpdates(bot.last_message_received + 1);

            while (numNewMessages)
            {
                logger.logLine("got response2");
                for (int i = 0; i < numNewMessages; i++)
                {
                    bot.sendMessage(bot.messages[i].chat_id, bot.messages[i].text, "");
                }
                numNewMessages = bot.getUpdates(bot.last_message_received + 1);
            }

            Bot_lasttime = millis();
        }
    }
}