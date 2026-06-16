#include "Channel.hpp"

Channel::Channel(std::string name) {
    _name = name;
    _topic = ""; // L-topic par defaut khawi
    _isInviteOnly = false; // Par defaut, l-channel machi invite-only
    _password = ""; // Par defaut, makaynch password
    _limit = 0; // 0 ya3ni makaynch limit par defaut
    _isTopicRestricted = false; // Par defaut, l-mode +t ma-khdamch
}

Channel::~Channel() {
}

std::string Channel::getName() {
    return _name;
}

void Channel::addClient(Client* client) {
    _members.push_back(client);
}

void Channel::addOperator(Client* client) {
    _operators.push_back(client);
}

bool Channel::hasClient(Client* client) {
    for (size_t i = 0; i < _members.size(); i++) {
        if (_members[i] == client)
            return true;
    }
    return false;
}

bool Channel::isOperator(Client* client) {
    for (size_t i = 0; i < _operators.size(); i++) {
        if (_operators[i] == client)
            return true;
    }
    return false;
}

std::string Channel::getClientList() {
    std::string list = "";
    for (size_t i = 0; i < _members.size(); i++) {
        list += _members[i]->getNickName() + " ";
    }
    return list;
}

void Channel::removeClient(Client* client) {
    for (std::vector<Client*>::iterator it = _members.begin(); it != _members.end(); ++it) {
        if (*it == client) {
            _members.erase(it);
            break;
        }
    }
}

void Channel::removeOperator(Client* client) {
    for (std::vector<Client*>::iterator it = _operators.begin(); it != _operators.end(); ++it) {
        if (*it == client) {
            _operators.erase(it);
            break;
        }
    }
}

size_t Channel::getMembersCount() {
    return _members.size();
}

void Channel::broadcast(std::string msg) {
    for (size_t i = 0; i < _members.size(); i++) {
        _members[i]->sendMessage(msg);
    }
}

void Channel::broadcast(std::string msg, Client* exclude) {
    for (size_t i = 0; i < _members.size(); i++) {
        if (_members[i] != exclude) {
            _members[i]->sendMessage(msg);
        }
    }
}

bool Channel::getIsInviteOnly() {
    return _isInviteOnly;
}

void Channel::setIsInviteOnly(bool status) {
    _isInviteOnly = status;
}

void Channel::inviteUser(std::string nickname) {
    _invitedUsers.push_back(nickname);
}

bool Channel::isInvited(std::string nickname) {
    for (size_t i = 0; i < _invitedUsers.size(); i++) {
        if (_invitedUsers[i] == nickname)
            return true;
    }
    return false;
}

void Channel::removeInvite(std::string nickname) {
    for (std::vector<std::string>::iterator it = _invitedUsers.begin(); it != _invitedUsers.end(); ++it) {
        if (*it == nickname) {
            _invitedUsers.erase(it);
            break;
        }
    }
}

std::string Channel::getPassword() {
    return _password;
}

void Channel::setPassword(std::string password) {
    _password = password;
}

size_t Channel::getLimit() {
    return _limit;
}

void Channel::setLimit(size_t limit) {
    _limit = limit;
}

std::string Channel::getTopic() {
    return _topic;
}

void Channel::setTopic(std::string topic) {
    _topic = topic;
}

bool Channel::getIsTopicRestricted() {
    return _isTopicRestricted;
}

void Channel::setIsTopicRestricted(bool status) {
    _isTopicRestricted = status;
}