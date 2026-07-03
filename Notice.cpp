#include "CommandHandler.hpp"

// This function handles the NOTICE command. It's similar to PRIVMSG but does not send any error replies.
void noticeCommand(Client &client, std::vector<std::string> &params, Server &server)
{
    // NOTICE requires a target and a message. If either is missing, silently fail.
    if (params.empty() || params.size() < 2) {
        return;
    }
    // Extract the target and message from parameters.
    std::string target = params[0];
    std::string message = params[1];

    // Check if the target is a channel.
    if (target[0] == '#') {
        Channel* channel = server.getChannelByName(target);
        // If channel doesn't exist, silently fail.
        if (channel == NULL) {
            return;
        }
        // If the sender is not in the channel, silently fail.
        if (!channel->hasClient(&client)) {
            return;
        }
        // Construct and broadcast the NOTICE message to the channel, excluding the sender.
        std::string fullMsg = ":" + client.getNickName() + " NOTICE " + target + " :" + message;
        channel->broadcast(fullMsg, &client);
        return;
    }

    // If the target is a user.
    Client* recipient = server.getClientByNickname(target);
    // If the user doesn't exist, silently fail.
    if (recipient == NULL) {
        return;
    }

    // Construct and send the NOTICE message to the recipient.
    std::string fullMsg = ":" + client.getNickName() + " NOTICE " + target + " :" + message;
    recipient->sendMessage(fullMsg);
}