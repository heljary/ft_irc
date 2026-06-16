#ifndef CLIENT_HPP
#define CLIENT_HPP

#include <string>

#include <iostream>


class Client{
    private:
        std::string _nickname;
        std::string _username;
        std::string _realname;
        bool _isRegistered;
        bool _passVerified;
        bool _isQuitting;
        int _fd; // Ra9m dyal socket

    public:

        Client();
        //setters
        void setFd(int fd);
        void setNickName(std::string nickname);
        void setUserName(std::string username);
        void setRealName(std::string realname);
        void setIsRegistered(bool status);
        void setIsPassVerified(bool status);
        void setIsQuitting(bool status);
        
        //getters
        int getFd();
        std::string getNickName();
        std::string getUserName();
        std::string getRealName();
        bool getIsRegistered();
        bool getIsPassVerified();
        bool getIsQuitting();


        void sendMessage(std::string msg);

};

#endif