#ifndef CHANNEL_HPP
#define CHANNEL_HPP

#include <string>
#include <vector>
#include <map>
#include "Client.hpp"

class Channel {
    private:
        std::string _name;
        std::string _topic;
        std::vector<Client*> _members;
        std::vector<Client*> _operators;
        bool _isInviteOnly;
        std::vector<std::string> _invitedUsers;
        std::string _password;
        size_t _limit;
        bool _isTopicRestricted; // L-mode +t

    public:
        Channel(std::string name);
        ~Channel();

        std::string getName();
        void addClient(Client* client);
        void addOperator(Client* client);
        bool isOperator(Client* client);
        std::string getClientList();
        void broadcast(std::string msg);
        void broadcast(std::string msg, Client* exclude);
        size_t getMembersCount();
        bool hasClient(Client* client);
        void removeClient(Client* client);
        void removeOperator(Client* client);

        bool getIsInviteOnly();
        void setIsInviteOnly(bool status);
        void inviteUser(std::string nickname);
        bool isInvited(std::string nickname);
        void removeInvite(std::string nickname);

        std::string getPassword();
        void setPassword(std::string password);
        
        size_t getLimit();
        void setLimit(size_t limit);

        std::string getTopic();
        void setTopic(std::string topic);
        bool getIsTopicRestricted();
        void setIsTopicRestricted(bool status);
};

#endif