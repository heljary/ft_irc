#ifndef SERVER_HPP
#define SERVER_HPP

#include <string>
#include <vector>
#include <map>
#include "Client.hpp"
#include "CommandHandler.hpp"

class Channel;

class Server{
    private:
        std::string _password;
        std::map<int, Client*> _clients;
        std::map<std::string, Channel*> _channels;


    public:
        Server(std::string password);
        std::string getPassword() const;
        void addClient(int fd, Client* client);
        void broadcast(std::string message, std::string excludeNickname);
        Client* getClientByNickname(std::string nickname);
        Client* getClientByFd(int fd);
        Channel* getChannelByName(std::string name);
        void addChannel(std::string name, Channel* channel);
        void removeChannel(std::string name);
};

#endif