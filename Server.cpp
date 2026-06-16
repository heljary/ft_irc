#include "Server.hpp"
#include "Channel.hpp"

Server::Server(std::string password) {
    (void)password;
    _password = password;
}

std::string Server::getPassword() const {
    return _password;
}

void Server::broadcast(std::string message, std::string excludeNickname){
    for (std::map<int, Client*>::iterator it = _clients.begin(); it != _clients.end(); ++it) {
        if (it->second->getNickName() != excludeNickname) {
            it->second->sendMessage(message);
        }
    }
}

Client* Server::getClientByNickname(std::string nickname){
    for (std::map<int, Client*>::iterator it = _clients.begin(); it != _clients.end(); ++it) {
        if (it->second->getNickName() == nickname) {
            return it->second;
        }
    }
    return NULL;
}

Client* Server::getClientByFd(int fd){
    if (_clients.find(fd) != _clients.end()) {
        return _clients[fd];
    }
    return NULL;
}

void Server::addClient(int fd, Client* client){
    _clients[fd] = client;
}

Channel* Server::getChannelByName(std::string name){
    if (_channels.find(name) != _channels.end())
        return _channels[name];
    return NULL;
}

void Server::addChannel(std::string name, Channel* channel){
    _channels[name] = channel;
}

void Server::removeChannel(std::string name){
    std::map<std::string, Channel*>::iterator it = _channels.find(name);
    if (it != _channels.end()) {
        delete it->second;
        _channels.erase(it);
    }
}