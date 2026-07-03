#include "CommandHandler.hpp"

// This function handles the MODE command for channels.
void modeCommand(Client &client, std::vector<std::string> &params, Server &server)
{
    // The MODE command requires at least one parameter (the channel name).
    if (params.empty())
    {
        // If no parameters are given, send error 461 (ERR_NEEDMOREPARAMS).
        client.sendMessage("461 :Not enough parameters");
        return;
    }
    std::string nameChannel = params[0];
    // This implementation only supports channel modes, which must start with '#'.
    if (nameChannel[0] == '#')
    {
        // Placeholder for future logic if needed.
    }
    else
    {
        // User modes are not supported, so send error 502 (ERR_USERSDONTMATCH).
        client.sendMessage("502 :Cant change mode for other users");
        return;
    }
    // Find the channel object by its name.
    Channel *channel = server.getChannelByName(nameChannel);
    if (channel)
    {
        // If only the channel name is provided, the user is requesting the current modes.
        if (params.size() == 1)
        {
            // Build a string representing the active modes.
            std::string activeModes = "+";
            if (channel->getIsInviteOnly())
                activeModes += "i";
            if (channel->getIsTopicRestricted())
                activeModes += "t";
            if (!channel->getPassword().empty())
                activeModes += "k";
            if (channel->getLimit() > 0)
                activeModes += "l";
            // Send reply 324 (RPL_CHANNELMODEIS) with the current modes.
            client.sendMessage("324 " + client.getNickName() + " " + params[0] + " " + activeModes);
            return;
        }
        else
        {
            // To change modes, the user must be a channel operator.
            if (channel->isOperator(&client))
            {
                bool isAdding = true; // Tracks if we are adding ('+') or removing ('-') a mode.
                std::string mode = params[1]; // The mode string, e.g., "+it-k".
                size_t argIndex = 2; // Index for mode arguments (like password or user limit).
                // Iterate over each character in the mode string.
                for (size_t i = 0; i < mode.size(); i++)
                {
                    if (mode[i] == '+')
                        isAdding = true;
                    else if (mode[i] == '-')
                        isAdding = false;
                    // Handle invite-only mode 'i'.
                    else if (mode[i] == 'i')
                        channel->setIsInviteOnly(isAdding);
                    // Handle topic restriction mode 't'.
                    else if (mode[i] == 't')
                        channel->setIsTopicRestricted(isAdding);
                    // Handle channel key (password) mode 'k'.
                    else if (mode[i] == 'k')
                    {
                        if (isAdding) {
                            // If adding a key, it must be provided as the next parameter.
                            if (argIndex < params.size()) {
                                channel->setPassword(params[argIndex]);
                                argIndex++;
                            }
                        } else {
                            // If removing a key, clear the password.
                            channel->setPassword("");
                        }
                    // Handle user limit mode 'l'.
                    }else if(mode[i] == 'l'){
                        int limit = 0;
                        if (isAdding) {
                            // If adding a limit, it must be provided as the next parameter.
                            if (argIndex < params.size()) {
                                std::stringstream ss(params[argIndex]);
                                ss >> limit;
                                channel->setLimit(limit);
                                argIndex++;
                            }
                        } else {
                            // If removing a limit, set it to 0 (unlimited).
                            channel->setLimit(0);
                        }
                    }
                    // Handle operator status mode 'o'.
                    else if (mode[i] == 'o')
                    {
                        // This mode requires a target user nickname as a parameter.
                        if (argIndex < params.size()) {
                            Client* target = server.getClientByNickname(params[argIndex]);
                            if (target) {
                                if (isAdding)
                                    channel->addOperator(target);
                                else
                                    channel->removeOperator(target);
                            }
                            argIndex++;
                        }
                    }
                    else
                    {
                        // If the mode character is unknown, send error 472 (ERR_UNKNOWNMODE).
                        std::string unknownchar(1, mode[i]);
                        client.sendMessage("472 " + unknownchar + " :is unknown mode char to me");
                    }
                }
                // Construct the broadcast message to inform all channel members of the mode change.
                std::string message = ":" + client.getNickName() + " MODE " + params[0] + " " + params[1];
                // Append any mode arguments (like key, limit, or nickname) to the broadcast message.
                for (size_t i = 2; i < argIndex; i++) {
                    if (i < params.size()) {
                        message += " " + params[i];
                    }
                }
                // Broadcast the message to everyone in the channel.
                channel->broadcast(message);
            }
            else
            {
                // If the user is not an operator, send error 482 (ERR_CHANOPRIVSNEEDED).
                client.sendMessage("482 " + params[0] + " :You're not channel operator");
                return;
            }
        }
    }
    else
    {
        // If the channel does not exist, send error 403 (ERR_NOSUCHCHANNEL).
        client.sendMessage("403 " + params[0] + " :No such channel");
        return;
    }
}