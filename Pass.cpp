#include "CommandHandler.hpp"

void passCommand(Client& client, std::vector<std::string>& params, Server& server){
    if(params.empty()){
        client.sendMessage("461 :Not enough parameters");
        return;
    }
    if(client.getIsRegistered()){
        client.sendMessage("462 :Unauthorized command (already registered)");
        return;
    }

    if(params[0] == server.getPassword()){
        client.setIsPassVerified(true);
    }else{
        client.sendMessage("464 :Password incorrect");
    }
}