#include "CommandHandler.hpp"

// This function handles the QUIT command, allowing a client to disconnect from the server.
void quitCommand(Client& client, std::vector<std::string>& params, Server& server) {
    // The default quit reason if none is provided.
    std::string reason = "Client Quit";
    // If the client provides a reason, use it instead.
    if (!params.empty()) {
        reason = params[0];
    }
    // Construct the full QUIT message to be broadcast to other users.
    std::string fullMessage = ":" + client.getNickName() + " QUIT :" + reason;
    // Mark the client as quitting to signal the main server loop to close the connection.
    client.setIsQuitting(true);
    // Broadcast the QUIT message to all relevant channels the user was in.
    server.broadcast(fullMessage,client.getNickName());
}