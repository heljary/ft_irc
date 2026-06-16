#include "CommandHandler.hpp"

void userCommand(Client& client, std::vector<std::string>& params,  Server& server){
    (void)server;
    if(client.getIsRegistered()){
        client.sendMessage("462 : Unauthorized command (already registred)");
        return;
    }

    if(params.size() < 4){
        client.sendMessage("461 :Not enough parameters");
        return;
    }
    if (client.getIsPassVerified() == true && client.getNickName().empty() == false) {
        
        client.setIsRegistered(true);
        client.sendMessage(":ft_irc 001 " + client.getNickName() + " :Welcome to the ft_irc Network");
        client.sendMessage(":ft_irc 002 " + client.getNickName() + " :Your host is ft_irc, running version 1.0");
        client.sendMessage(":ft_irc 003 " + client.getNickName() + " :This server was created today");
        client.sendMessage(":ft_irc 004 " + client.getNickName() + " ft_irc 1.0 o o");
    }

    client.setUserName(params[0]);
    client.setRealName(params[3]);
}