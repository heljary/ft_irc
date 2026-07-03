#include "CommandHandler.hpp"

// This function handles the USER command, which is part of the client registration process.
void userCommand(Client& client, std::vector<std::string>& params,  Server& server){
    // This line prevents the compiler from showing an "unused parameter" warning for 'server'.
    (void)server; 

    // Check if the client is already registered.
    if(client.getIsRegistered()){
        // If so, send error 462 (RPL_ALREADYREGISTRED) because the USER command can't be used again.
        client.sendMessage("462 : Unauthorized command (already registred)");
        return;
    }

    // The USER command requires 4 parameters: <username> <hostname> <servername> <realname>.
    if(params.size() < 4){
        // If there are not enough parameters, send error 461 (ERR_NEEDMOREPARAMS).
        client.sendMessage("461 :Not enough parameters");
        return;
    }

    // The client can only be registered if they have already provided a valid password (if required) and a nickname.
    if (client.getIsPassVerified() == true && client.getNickName().empty() == false) {
        // If the conditions are met, mark the client as officially registered.
        client.setIsRegistered(true);
        // Send the standard IRC welcome messages (RPL_WELCOME, RPL_YOURHOST, RPL_CREATED, RPL_MYINFO).
        client.sendMessage(":ft_irc 001 " + client.getNickName() + " :Welcome to the ft_irc Network");
        client.sendMessage(":ft_irc 002 " + client.getNickName() + " :Your host is ft_irc, running version 1.0");
        client.sendMessage(":ft_irc 003 " + client.getNickName() + " :This server was created today");
        client.sendMessage(":ft_irc 004 " + client.getNickName() + " ft_irc 1.0 o o");
    }

    // Store the username and realname provided by the client, regardless of registration status.
    client.setUserName(params[0]);
    client.setRealName(params[3]);
}