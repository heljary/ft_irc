#include "Client.hpp"
#include <sys/socket.h> // Zdnaha 3la 9bel send()


Client::Client(){
    _isRegistered = false;
    _passVerified = false;
    _isQuitting = false;
    _fd = -1; // Par defaut ma-3ndouch socket
}

void Client::setFd(int fd){
    _fd = fd;
}

void Client::setNickName(std::string nickname){
    _nickname = nickname;
}

void Client::setUserName(std::string username){
    _username = username;
}

void Client::setRealName(std::string realname){
    _realname = realname;
}

void Client::setIsRegistered(bool status){
    _isRegistered = status;
}

void Client::setIsPassVerified(bool status){
    _passVerified = status;
}

void Client::setIsQuitting(bool status){
    _isQuitting = status;
}


std::string Client::getNickName(){
    return _nickname;
}

std::string Client::getUserName(){
    return _username;
}

std::string Client::getRealName(){
    return _realname;
}
bool Client::getIsRegistered(){
    return _isRegistered;
}

bool Client::getIsPassVerified(){
    return _passVerified;
}

bool Client::getIsQuitting(){
    return _isQuitting;
}

void Client::sendMessage(std::string msg) {
    // Zidna \r\n f l-lekher 7it protocol IRC kay-fardha
    std::string final_msg = msg + "\r\n";
    if (_fd != -1) {
        send(_fd, final_msg.c_str(), final_msg.length(), 0);
    }
    // T9der t-khli cout l-ta7t ghir bach t-b9a t-chouf chno kay-w9e3 f server
    std::cout << "DEBUG -> Sent to [" << _nickname << "]: " << msg << std::endl;
}