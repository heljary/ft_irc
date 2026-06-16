#ifndef COMMANDHANDLER_HPP
#define COMMANDHANDLER_HPP

#include <iostream>
#include <vector>
#include <string>
#include <map>
#include <sstream>
#include <unistd.h>
#include "Client.hpp"
#include "Server.hpp"
#include "Channel.hpp"

class Client;
class Server;

struct Message
{
    std::string prefix;
    std::string command;
    std::vector<std::string> paramters;
};

typedef void (*CommandFunction)(Client &, std::vector<std::string> &, Server &);

void myPingCommand(Client &client, std::vector<std::string> &params, Server &server);
void passCommand(Client &client, std::vector<std::string> &params, Server &server);
void nickCommand(Client &client, std::vector<std::string> &params, Server &server);
void userCommand(Client &client, std::vector<std::string> &params, Server &server);
void quitCommand(Client &client, std::vector<std::string> &params, Server &server);
void privmsgCommand(Client &client, std::vector<std::string> &params, Server &server);
void joinCommand(Client &client, std::vector<std::string> &params, Server &server);
void partCommand(Client &client, std::vector<std::string> &params, Server &server);
void kickCommand(Client &client, std::vector<std::string> &params, Server &server);
void inviteCommand(Client &client, std::vector<std::string> &params, Server &server);
void topicCommand(Client &client, std::vector<std::string> &params, Server &server);
void modeCommand(Client &client, std::vector<std::string> &params, Server &server);
void noticeCommand(Client &client, std::vector<std::string> &params, Server &server);

class CommandHandler
{
private:
    std::map<std::string, CommandFunction> _commands_map;

public:
    CommandHandler();
    void executeCommand(Message &msg, Client &client, Server &server);
};

#endif