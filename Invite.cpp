#include "CommandHandler.hpp"

// This function handles the INVITE command, allowing a user to invite another user to a channel.
void inviteCommand(Client &client, std::vector<std::string> &params, Server &server)
{
    // The INVITE command requires at least two parameters: <nickname> and <channel>.
    if (params.size() < 2)
    {
        // If parameters are insufficient, send error 461 (ERR_NEEDMOREPARAMS).
        client.sendMessage("461 :Not enough parameters");
        return;
    }

    // Extract the target user's nickname and the channel name from the parameters.
    std::string targetName = params[0];
    std::string channelName = params[1];

    // Find the client object for the target user.
    Client *targetUser = server.getClientByNickname(targetName);
    if (!targetUser)
    {
        // If the target user doesn't exist on the server, send error 401 (ERR_NOSUCHNICK).
        client.sendMessage("401 " + targetName + " :No such nick/channel");
        return;
    }

    // Find the channel object.
    Channel *channel = server.getChannelByName(channelName);
    if (!channel)
    {
        // If the channel doesn't exist, send error 403 (ERR_NOSUCHCHANNEL).
        client.sendMessage("403 " + channelName + " :No such channel");
        return;
    }

    // Check if the user sending the invite is actually a member of the channel.
    if (!channel->hasClient(&client)) {
        // If not, send error 442 (ERR_NOTONCHANNEL).
        client.sendMessage("442 " + channelName + " :You're not on that channel");
        return;
    }

    // Check if the target user is already in the channel.
    if (channel->hasClient(targetUser)) {
        // If they are, send error 443 (ERR_USERONCHANNEL).
        client.sendMessage("443 " + targetName + " " + channelName + " :is already on channel");
        return;
    }

    // If the channel is invite-only (+i), only a channel operator can send invites.
    if (channel->getIsInviteOnly() && !channel->isOperator(&client)) {
        // If the user is not an operator, send error 482 (ERR_CHANOPRIVSNEEDED).
        client.sendMessage("482 " + channelName + " :You're not channel operator");
        return;
    }

    // If all checks pass, add the target user to the channel's invite list.
    channel->inviteUser(targetName);
    // Send an INVITE message to the target user.
    targetUser->sendMessage(":" + client.getNickName() + " INVITE " + targetName + " :" + channelName);
    // Send a success reply 341 (RPL_INVITING) to the user who sent the command.
    client.sendMessage(":ft_irc 341 " + client.getNickName() + " " + targetName + " " + channelName);
}