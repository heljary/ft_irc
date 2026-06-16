#include "CommandHandler.hpp"

void privmsgCommand(Client& client, std::vector<std::string>& params, Server& server) {
    if (params.empty()) {
        client.sendMessage("411 :No recipient given (PRIVMSG)");
        return;
    }
    if (params.size() < 2) {
        client.sendMessage("412 :No text to send");
        return;
    }

    std::string target = params[0];
    std::string message = params[1];

    if (target[0] == '#') {
        Channel* channel = server.getChannelByName(target);
        if (channel == NULL) {
            client.sendMessage("401 " + target + " :No such nick/channel");
            return;
        }
        if (!channel->hasClient(&client)) {
            client.sendMessage("404 " + target + " :Cannot send to channel");
            return;
        }
        std::string fullMsg = ":" + client.getNickName() + " PRIVMSG " + target + " :" + message;
        channel->broadcast(fullMsg, &client);
        return;
    }

    Client* recipient = server.getClientByNickname(target);
    if (recipient == NULL) {
        client.sendMessage("401 " + target + " :No such nick/channel");
        return;
    }

    std::string fullMsg = ":" + client.getNickName() + " PRIVMSG " + target + " :" + message;
    recipient->sendMessage(fullMsg);
}