#include "CommandHandler.hpp"
#include "Client.hpp"
#include "Server.hpp"
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <poll.h>
#include <fcntl.h>
#include <unistd.h>
#include <cstring>
#include <cstdlib>

// Parser sghir (Mock Member 2)
Message parseMsg(std::string raw) {
    Message msg;
    if (raw.empty()) return msg;
    if (raw[raw.size()-1] == '\n') raw.erase(raw.size()-1);
    if (raw.size() > 0 && raw[raw.size()-1] == '\r') raw.erase(raw.size()-1);

    size_t i = 0;
    if (raw[0] == ':') { // T-tjahol l-prefix مؤقتا
        while (i < raw.size() && raw[i] != ' ') i++;
        while (i < raw.size() && raw[i] == ' ') i++;
    }
    size_t cmdEnd = raw.find(' ', i);
    if (cmdEnd == std::string::npos) {
        msg.command = raw.substr(i);
        return msg;
    }
    msg.command = raw.substr(i, cmdEnd - i);
    i = cmdEnd;

    while (i < raw.size()) {
        while (i < raw.size() && raw[i] == ' ') i++;
        if (i >= raw.size()) break;
        if (raw[i] == ':') {
            msg.paramters.push_back(raw.substr(i + 1));
            break;
        }
        size_t paramEnd = raw.find(' ', i);
        if (paramEnd == std::string::npos) {
            msg.paramters.push_back(raw.substr(i));
            break;
        }
        msg.paramters.push_back(raw.substr(i, paramEnd - i));
        i = paramEnd;
    }
    return msg;
}

int main(int argc, char **argv)
{
    if (argc != 3) {
        std::cout << "Usage: ./ircserv <port> <password>\n";
        return 1;
    }
    int port = atoi(argv[1]);
    Server sr1(argv[2]);
    CommandHandler handler;

    int server_fd = socket(AF_INET, SOCK_STREAM, 0);
    int opt = 1;
    setsockopt(server_fd, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt));
    fcntl(server_fd, F_SETFL, O_NONBLOCK);

    sockaddr_in addr;
    addr.sin_family = AF_INET;
    addr.sin_addr.s_addr = INADDR_ANY;
    addr.sin_port = htons(port);
    bind(server_fd, (struct sockaddr*)&addr, sizeof(addr));
    listen(server_fd, 10);

    std::vector<pollfd> fds;
    pollfd srv_poll = {server_fd, POLLIN, 0};
    fds.push_back(srv_poll);

    std::cout << "SERVER STARTED ON PORT " << port << " | PASSWORD: " << argv[2] << "\n";

    std::map<int, std::string> client_buffers;

    while (true) {
        poll(fds.data(), fds.size(), -1);
        for (size_t i = 0; i < fds.size(); i++) {
            if (fds[i].revents & POLLIN) {
                if (fds[i].fd == server_fd) {
                    int client_fd = accept(server_fd, NULL, NULL);
                    fcntl(client_fd, F_SETFL, O_NONBLOCK);
                    pollfd c_poll = {client_fd, POLLIN, 0};
                    fds.push_back(c_poll);

                    Client* c = new Client();
                    c->setFd(client_fd);
                    sr1.addClient(client_fd, c);
                    std::cout << "[+] New connection: FD " << client_fd << "\n";
                    client_buffers[client_fd] = "";
                } else {
                    char buffer[1024];
                    memset(buffer, 0, sizeof(buffer));
                    int bytes = recv(fds[i].fd, buffer, sizeof(buffer)-1, 0);
                    if (bytes <= 0) {
                        std::cout << "[-] Client disconnected: FD " << fds[i].fd << "\n";
                        close(fds[i].fd);
                        client_buffers.erase(fds[i].fd);
                        fds.erase(fds.begin() + i);
                        i--;
                    } else {
                        client_buffers[fds[i].fd] += buffer;
                        std::string &raw = client_buffers[fds[i].fd];
                        size_t pos = 0;
                        while ((pos = raw.find('\n')) != std::string::npos) {
                            std::string line = raw.substr(0, pos);
                            raw.erase(0, pos + 1);
                            Message msg = parseMsg(line);
                            Client* c = sr1.getClientByFd(fds[i].fd);
                            if (c && !msg.command.empty()) handler.executeCommand(msg, *c, sr1);
                        }
                    }
                }
            }
        }
    }
    return 0;
}
