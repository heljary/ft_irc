#include "CommandHandler.hpp"

// This function handles the PRIVMSG command, used for sending messages to users or channels.
void privmsgCommand(Client& client, std::vector<std::string>& params, Server& server) {
    // The command requires a recipient (target).
    if (params.empty()) {
        // If no recipient is given, send error 411 (ERR_NORECIPIENT).
        client.sendMessage("411 :No recipient given (PRIVMSG)");
        return;
    }
    // The command also requires a message text.
    if (params.size() < 2) {
        // If no text is given, send error 412 (ERR_NOTEXTTOSEND).
        client.sendMessage("412 :No text to send");
        return;
    }

    // Extract the target (channel or user) and the message.
    std::string target = params[0];
    std::string message = params[1];

    // Check if the target is a channel (starts with '#').
    if (target[0] == '#') {
        Channel* channel = server.getChannelByName(target);
        if (channel == NULL) {
            // If the channel doesn't exist, send error 401 (ERR_NOSUCHNICK).
            client.sendMessage("401 " + target + " :No such nick/channel");
            return;
        }
        // Check if the sender is a member of the channel.
        if (!channel->hasClient(&client)) {
            // If not, send error 404 (ERR_CANNOTSENDTOCHAN).
            client.sendMessage("404 " + target + " :Cannot send to channel");
            return;
        }
        // Construct the full message and broadcast it to the channel, excluding the sender.
        std::string fullMsg = ":" + client.getNickName() + " PRIVMSG " + target + " :" + message;
        channel->broadcast(fullMsg, &client);
        return;
    }

    // If the target is not a channel, assume it's a user.
    Client* recipient = server.getClientByNickname(target);
    if (recipient == NULL) {
        // If the recipient user doesn't exist, send error 401 (ERR_NOSUCHNICK).
        client.sendMessage("401 " + target + " :No such nick/channel");
        return;
    }

    // Construct the full message and send it directly to the recipient.
    std::string fullMsg = ":" + client.getNickName() + " PRIVMSG " + target + " :" + message;
    recipient->sendMessage(fullMsg);
}