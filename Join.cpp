#include "CommandHandler.hpp"

// This function handles the JOIN command, allowing a client to join a channel.
void joinCommand(Client &client, std::vector<std::string> &params, Server &server)
{
    // The JOIN command requires at least one parameter: the channel name.
    if (params.empty())
    {
        // If no channel is specified, send error 461 (ERR_NEEDMOREPARAMS).
        client.sendMessage("461 :Not enough parameters");
        return;
    }
    std::string channel = params[0];
    // Channel names must start with a '#'.
    if (channel[0] != '#')
    {
        // If not, it's considered an invalid channel name. Send error 403 (ERR_NOSUCHCHANNEL).
        client.sendMessage("403 :No such channel");
        return;
    }
    // Check if the channel already exists on the server.
    Channel *channel_name_already_tacked = server.getChannelByName(channel);
    if (channel_name_already_tacked)
    {
        // --- Joining an existing channel ---
        // If the channel is invite-only (+i).
        if (channel_name_already_tacked->getIsInviteOnly())
        {
            // Check if the client has been invited.
            if(channel_name_already_tacked->isInvited(client.getNickName())){
                // If invited, consume the invitation and allow entry.
                channel_name_already_tacked->removeInvite(client.getNickName());
            }else{
                // If not invited, send error 473 (ERR_INVITEONLYCHAN).
                client.sendMessage("473 :Cannot join " + channel + "(+i)");
                return;
            }
        }

        // If the channel has a user limit (+l) and is full.
        if (channel_name_already_tacked->getLimit() > 0 && channel_name_already_tacked->getMembersCount() >= channel_name_already_tacked->getLimit())
        {
            // Send error 471 (ERR_CHANNELISFULL).
            client.sendMessage("471 :Cannot join " + channel + " (+l)");
            return;
        }

        // If the channel has a password (+k).
        if (!channel_name_already_tacked->getPassword().empty())
        {
            // Check if the client provided the correct password.
            if (params.size() < 2 || params[1] != channel_name_already_tacked->getPassword())
            {
                // If the password is wrong or missing, send error 475 (ERR_BADCHANNELKEY).
                client.sendMessage("475 :Cannot join " + channel + " (+k)");
                return;
            }
        }

        // If all checks pass, add the client to the channel.
        channel_name_already_tacked->addClient(&client);
        // Broadcast the JOIN message to all members of the channel.
        std::string msgJoin = ":" + client.getNickName() + " JOIN :" + channel;
        channel_name_already_tacked->broadcast(msgJoin);
        // Send the list of users in the channel to the client (RPL_NAMREPLY).
        client.sendMessage(":ft_irc 353 " + client.getNickName() + " = " + channel + " :" + channel_name_already_tacked->getClientList());
        // Signal the end of the user list (RPL_ENDOFNAMES).
        client.sendMessage(":ft_irc 366 " + client.getNickName() + " " + channel + " :End of /NAMES list.");
    }
    else
    {
        // --- Creating a new channel ---
        // If the channel does not exist, create a new one.
        Channel *createNewChannel = new Channel(channel);
        if (!createNewChannel)
            return;
        // The creator of the channel automatically becomes an operator.
        createNewChannel->addOperator(&client);
        // Add the client to the new channel.
        createNewChannel->addClient(&client);
        // Add the new channel to the server's list of channels.
        server.addChannel(channel, createNewChannel);
        // Broadcast the JOIN message.
        std::string msgJoin = ":" + client.getNickName() + " JOIN :" + channel;
        createNewChannel->broadcast(msgJoin);
        // Send the user list (RPL_NAMREPLY) and end of list (RPL_ENDOFNAMES) messages.
        client.sendMessage(":ft_irc 353 " + client.getNickName() + " = " + channel + " :" + createNewChannel->getClientList());
        client.sendMessage(":ft_irc 366 " + client.getNickName() + " " + channel + " :End of /NAMES list.");
    }
}