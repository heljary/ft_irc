#include "CommandHandler.hpp"

void partCommand(Client &client, std::vector<std::string> &params, Server &server)
{
    std::string reason = "";
    if (params.empty())
    {
        client.sendMessage("461 :Not enough parameters");
        return;
    }
    Channel *targetChannel = server.getChannelByName(params[0]);
    if (targetChannel)
    {
        if (!targetChannel->hasClient(&client))
        {
            client.sendMessage("442 " + params[0] + " :You're not on that channel");
            return;
        }
        else
        {
            std::string partMsg = ":" + client.getNickName() + " PART " + params[0];
            if (params.size() > 1)
            {
                partMsg += " :" + params[1];
            }

            targetChannel->broadcast(partMsg);
        }
        targetChannel->removeClient(&client);
        targetChannel->removeOperator(&client);
        size_t numbers_Member = targetChannel->getMembersCount();
        if(numbers_Member == 0){
            server.removeChannel(params[0]);
        }
    }
    else
    {
        client.sendMessage("403 " + params[0] + " :No such channel");
    }
}