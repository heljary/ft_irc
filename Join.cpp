#include "CommandHandler.hpp"

void joinCommand(Client &client, std::vector<std::string> &params, Server &server)
{
    if (params.empty())
    {
        client.sendMessage("461 :Not enough parameters");
        return;
    }
    std::string channel = params[0];
    if (channel[0] != '#')
    {
        client.sendMessage("403 :No such channel");
        return;
    }
    Channel *channel_name_already_tacked = server.getChannelByName(channel);
    if (channel_name_already_tacked)
    {
        if (channel_name_already_tacked->getIsInviteOnly())
        {
            if(channel_name_already_tacked->isInvited(client.getNickName())){
                channel_name_already_tacked->removeInvite(client.getNickName());
            }else{
                client.sendMessage("473 :Cannot join " + channel + "(+i)");
                return;
            }
        }

        if (channel_name_already_tacked->getLimit() > 0 && channel_name_already_tacked->getMembersCount() >= channel_name_already_tacked->getLimit())
        {
            client.sendMessage("471 :Cannot join " + channel + " (+l)");
            return;
        }

        if (!channel_name_already_tacked->getPassword().empty())
        {
            if (params.size() < 2 || params[1] != channel_name_already_tacked->getPassword())
            {
                client.sendMessage("475 :Cannot join " + channel + " (+k)");
                return;
            }
        }

        channel_name_already_tacked->addClient(&client);
        std::string msgJoin = ":" + client.getNickName() + " JOIN :" + channel;
        channel_name_already_tacked->broadcast(msgJoin);
        client.sendMessage(":ft_irc 353 " + client.getNickName() + " = " + channel + " :" + channel_name_already_tacked->getClientList());
        client.sendMessage(":ft_irc 366 " + client.getNickName() + " " + channel + " :End of /NAMES list.");
    }
    else
    {
        Channel *createNewChannel = new Channel(channel);
        if (!createNewChannel)
            return;
        createNewChannel->addOperator(&client);
        createNewChannel->addClient(&client);
        server.addChannel(channel, createNewChannel);
        std::string msgJoin = ":" + client.getNickName() + " JOIN :" + channel;
        createNewChannel->broadcast(msgJoin);
        client.sendMessage(":ft_irc 353 " + client.getNickName() + " = " + channel + " :" + createNewChannel->getClientList());
        client.sendMessage(":ft_irc 366 " + client.getNickName() + " " + channel + " :End of /NAMES list.");
    }
}