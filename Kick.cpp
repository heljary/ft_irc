#include "CommandHandler.hpp"

void kickCommand(Client& client, std::vector<std::string>& params, Server& server){
    std::string reason = "";
    if(params.size() < 2){
        client.sendMessage("461 :Not enough parameters");
        return;
    }

    std::string channelName = params[0];
    std::string targetName = params[1];

    Channel *channel = server.getChannelByName(channelName);
    if (channel == NULL) {
        client.sendMessage("403 " + channelName + " :No such channel");
        return;
    }

    if (!channel->hasClient(&client)) {
        client.sendMessage("442 " + channelName + " :You're not on that channel");
        return;
    }

    if(channel->isOperator(&client)){
        Client *target = server.getClientByNickname(targetName);
        if(target){
            if(channel->hasClient(target)){
                if(params.size() > 2){
                    reason = params[2];
                }else{
                    reason = "";
                }
                
                std::string kickMsg = ":" + client.getNickName() + " KICK " + channelName + " " + targetName;
                if (!reason.empty()) {
                    kickMsg += " :" + reason;
                }
                channel->broadcast(kickMsg);

                channel->removeClient(target);
                channel->removeOperator(target);
                if (channel->getMembersCount() == 0) {
                    server.removeChannel(channelName);
                }
            }else{
                client.sendMessage("441 " + targetName + " " + channelName + " :They aren't on that channel");
                return;
            }

        }else{
            client.sendMessage("441 " + targetName + " " + channelName + " :They aren't on that channel");
            return;
        }
    }else{
        client.sendMessage("482 " + channelName + " :You're not channel operator");
        return;
    }
}