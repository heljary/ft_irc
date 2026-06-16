#include "CommandHandler.hpp"

void topicCommand(Client &client, std::vector<std::string> &params, Server &server)
{
    if (params.empty())
    {
        client.sendMessage("461 :Not enough parameters");
        return;
    }

    Channel *channel = server.getChannelByName(params[0]);
    if (!channel)
    {
        client.sendMessage("403 " + params[0] + " :No such channel");
        return;
    }

    if (!channel->hasClient(&client))
    {
        client.sendMessage("442 " + params[0] + " :You're not on that channel");
        return;
    }

    if (params.size() < 2)
    {
        if (channel->getTopic().empty()) {
            client.sendMessage("331 " + params[0] + " :No topic is set");
        } else {
            client.sendMessage("332 " + params[0] + " :" + channel->getTopic());
        }
    }
    else
    {
        if (channel->getIsTopicRestricted() && !channel->isOperator(&client)) {
            client.sendMessage("482 " + params[0] + " :You're not channel operator");
            return;
        }
        
        std::string newTopic = params[1];
        channel->setTopic(newTopic);
        
        std::string topicMsg = ":" + client.getNickName() + " TOPIC " + params[0] + " :" + newTopic;
        channel->broadcast(topicMsg);
    }
}