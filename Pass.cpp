#include "CommandHandler.hpp"

// This function handles the PASS command, used to provide the server password upon connection.
void passCommand(Client& client, std::vector<std::string>& params, Server& server){
    // The PASS command requires one parameter: the password.
    if(params.empty()){
        // If no password is provided, send error 461 (ERR_NEEDMOREPARAMS).
        client.sendMessage("461 :Not enough parameters");
        return;
    }
    // The PASS command cannot be used after the client is already registered.
    if(client.getIsRegistered()){
        // If already registered, send error 462 (RPL_ALREADYREGISTRED).
        client.sendMessage("462 :Unauthorized command (already registered)");
        return;
    }

    // Check if the provided password matches the server's password.
    if(params[0] == server.getPassword()){
        // If it matches, mark the client as having verified the password.
        client.setIsPassVerified(true);
    }else{
        // If it doesn't match, send error 464 (ERR_PASSWDMISMATCH).
        client.sendMessage("464 :Password incorrect");
    }
}