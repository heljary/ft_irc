#include "CommandHandler.hpp"

// This function handles the PING command to check if the connection is still alive.
void myPingCommand(Client& client, std::vector<std::string>& params, Server& server) {
    // This line prevents the compiler from showing an "unused parameter" warning for 'server'.
    (void)server;
    // The PING command requires a parameter (the origin server or token).
    if(params.empty()){
        // If no parameter is given, send error 409 (ERR_NOORIGIN).
        client.sendMessage("409: No origin specified !");
        return;
    }
    // Respond with a PONG message, echoing the parameter received.
    std::string pongMessage = "PONG :" + params[0];
    client.sendMessage(pongMessage);
}