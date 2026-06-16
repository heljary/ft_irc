#include "CommandHandler.hpp"

void noticeCommand(Client &client, std::vector<std::string> &params, Server &server)
{
    if (params.empty() || params.size() < 2) {
        return;
    }
    std::string target = params[0];
    std::string message = params[1];

    if (target[0] == '#') {
        Channel* channel = server.getChannelByName(target);
        if (channel == NULL) {
            return;
        }
        if (!channel->hasClient(&client)) {
            return;
        }
        std::string fullMsg = ":" + client.getNickName() + " NOTICE " + target + " :" + message;
        channel->broadcast(fullMsg, &client);
        return;
    }

    Client* recipient = server.getClientByNickname(target);
    if (recipient == NULL) {
        return;
    }

    std::string fullMsg = ":" + client.getNickName() + " NOTICE " + target + " :" + message;
    recipient->sendMessage(fullMsg);
}