#include "CommandHandler.hpp"

void nickCommand(Client &client, std::vector<std::string> &params, Server &server)
{
    if (params.empty())
    {
        client.sendMessage("431 :No nickname given");
        return;
    }
    std::string wantedname = params[0];
    for (size_t i = 0; i < wantedname.size(); i++)
    {
        if (wantedname[i] == ' ' || wantedname[i] == '#' || wantedname[i] == '*' || wantedname[i] == ':')
        {
            client.sendMessage("432 :Erroneous nickname");
            return;
        }
    }
    Client *check_name_is_tacked = server.getClientByNickname(wantedname);
    if (check_name_is_tacked && client.getNickName() != wantedname)
    {
        client.sendMessage("433 :Nickname is already in use");
    }
    else
    {
        std::string oldNick = client.getNickName();
        client.setNickName(wantedname);
        if (!oldNick.empty() && oldNick != wantedname) {
            server.broadcast(":" + oldNick + " NICK " + wantedname, "");
        }
    }
}