#include <iostream>
#include <vector>
#include <stdexcept>
#include <unistd.h>
#include <poll.h>
#include <netinet/in.h>
#include <fcntl.h>
#include <arpa/inet.h>
#include <signal.h>
#include <cstring>
#include <stdlib.h>
#include <algorithm>
#include <err.h>
#include "channel.hpp"
#include "client.hpp"

#define SERVER_IP "127.0.0.1"

bool isRunning = false;

class Server
{
    private:
        std::string _servIP;
        std::string _port;
        std::string _password;
        int _socket;
        std::vector<pollfd> _pfd;
        std::vector<Channel *> _channList;
        void _initializeSocket(void);
        void _makeSocketNonBlocking(int sock);
        void _bindListenOnSocket(int sock, struct sockaddr_in addr);
        struct sockaddr_in _createSocketAddress(const std::string& ip, const std::string& port);

    public:
        Server(const std::string &port, const std::string &password);
        ~Server();
        void startServer(void);
        std::string getPassword(void) const;
        void newClientConnects(int sock, std::vector<pollfd> *pfds, int i);
        void existingClientMessage(std::vector<pollfd> &pfds, int i);
        Channel* makeChannel(const std::string &name);
        // void onClientDisconnect(int fd);
        // std::string readMessage(int fd);
        // void createChannel(const std::string& name, const std::string& key);
};

Server::Server(const std::string &port, const std::string &password) : _servIP(SERVER_IP), _port(port), _password(password), _socket(-1)
{
    _initializeSocket();
    pollfd serverFd;
    serverFd.fd = _socket;
    serverFd.events = POLLIN;
    serverFd.revents = 0;
    _pfd.push_back(serverFd);
}

Server::~Server()
{
    if (_socket != -1)
    {
        close(_socket);
        _socket = -1; // Reset socket to invalid value
    }

    // Close any open client sockets
    for (size_t i = 1; i < _pfd.size(); i++)
    {
        if (_pfd[i].fd != -1)
        {
            close(_pfd[i].fd);
            _pfd[i].fd = -1; // Reset client socket to invalid value
        }
    }
}

void Server::_initializeSocket(void)
{
    struct sockaddr_in serv_addr;

    _socket = socket(AF_INET, SOCK_STREAM, 0);
    if (_socket == -1)
        throw std::runtime_error("Error : Failed to create a socket!");

    _makeSocketNonBlocking(_socket);
    serv_addr = _createSocketAddress(_servIP, _port);
    _bindListenOnSocket(_socket, serv_addr);
}

void Server::_makeSocketNonBlocking(int sock)
{
    int optval;
    optval = 1;
    if (setsockopt(sock, SOL_SOCKET, SO_REUSEADDR, &optval, sizeof(optval)))
        throw std::runtime_error("Error : Failed to set socket options!");

    if (fcntl(sock, F_SETFL, O_NONBLOCK))
        throw std::runtime_error("Error : Failed to set socket to non-blocking mode!");
}

struct sockaddr_in Server::_createSocketAddress(const std::string& ip, const std::string& port)
{
    struct sockaddr_in addr;
    
    bzero((char*) &addr, sizeof(addr));
    addr.sin_family = AF_INET;
    addr.sin_addr.s_addr = inet_addr(ip.c_str());
    addr.sin_port = htons(atoi(port.c_str()));
    return addr;
}

void Server::_bindListenOnSocket(int sock, struct sockaddr_in addr)
{
    if (bind(sock, (sockaddr *)&addr, sizeof(addr)) < 0)
        throw std::runtime_error("Error : Failed to bind the server socket!");

    if (listen(sock, SOMAXCONN) < 0) //SOMAXCONN allows the system to choose a value for backlog. I think it's set to 128 max.
        throw std::runtime_error("Error : Failed to listen on the server socket!");
}

void Server::startServer(void)
{
    while(isRunning == true)
    {
        int result = poll(&_pfd[0], _pfd.size(), -1);
        if (result == 0)
            continue; // timeout occurred.
        else if (result == -1)
            throw std::runtime_error("Error : Failed to poll on the server socket!");
        // check for events on each socket in the _pfd
        for (size_t i = 0; i < _pfd.size(); i++)
        {
            if((_pfd[i].revents & POLLIN) == POLLIN)
            {
                if (i == 0)
                    newClientConnects(_socket, &_pfd, i);
                else
                    existingClientMessage(_pfd, i);
            }
        }
    }
    if (isRunning == false)
    {
        if (_socket != -1)
        {
            close(_socket);
            _socket = -1; // Reset socket to invalid value
        }
        for (size_t i = 1; i < _pfd.size(); i++) // Close any open client sockets
        {
            if (_pfd[i].fd != -1)
            {
                close(_pfd[i].fd);
                _pfd[i].fd = -1; // Reset client socket to invalid value
            }
        }
    }
}

std::string Server::getPassword(void) const
{
    return _password;
}

void Server::newClientConnects(int sock, std::vector<pollfd> *pfds, int i)
{
    int         newClient;
    sockaddr_in addr = {};
    socklen_t   size = sizeof(addr);

    newClient = accept(sock, (sockaddr *)&addr, &size);
    if (newClient == -1)
        throw std::runtime_error("Error : Failed to accept client on the server socket!");
    else
    {
        pollfd clientFd;
        clientFd.fd = newClient;
        clientFd.events = POLLIN;
        clientFd.revents = 0;
        pfds[i].push_back(clientFd);
    }
}

void Server::existingClientMessage(std::vector<pollfd> &pfds, int i)
{
    std::string buf;
    char tempBuf[1024];
    while (true)
    {
        int readBytes = read(pfds[i].fd, tempBuf, sizeof(tempBuf));
        if (readBytes == -1)
            throw std::runtime_error("Error : reading data from client!");
        else if (readBytes == 0 || (pfds[i].revents & POLLHUP) || (pfds[i].revents & POLLERR))
        {
            if (close(pfds[i].fd) == -1)
                throw std::runtime_error("Error : closing client socket!");
            std::cout << "client no. [" << pfds[i].fd << "] disconnected!" << std::endl;
            pfds.erase(pfds.begin() + i);
            break;
        }
        // Find the position of the newline or carriage return
        char *end = std::find(tempBuf, tempBuf + readBytes, '\n'); // looking for the '\n' character to stop reading
        if (end == tempBuf + readBytes) // this check is in case '\n' was not found in the above find operation
            end = std::find(tempBuf, tempBuf + readBytes, '\r'); // looking for the '\r' character to stop reading
        buf.append(tempBuf, end - tempBuf); // Append the read data to buf
        std::cout << "Msg received from client no [" << pfds[i].fd << "]: " << buf << std::endl;
        if (end != tempBuf + readBytes) // Stop reading if either '\n' or '\r' found
            break;
    }
}

Channel* Server::makeChannel(const std::string &name)
{
    Channel* ch = new Channel(name);
    _channList.push_back(ch);

    return ch;
}

void SignalHandler(int signum)
{
    if (signum == SIGINT)
    {
        std::cout << "\nInterrupt signal received. Shutting down server..\n";
        isRunning = false;
    }
    exit(signum);
}

int main(int argc, char *argv[])
{
    
    signal(SIGINT, SignalHandler);
    (void)argc;
    
    Server serv(argv[1], argv[2]);
    isRunning = true;
    
    try
    {
        serv.startServer();
        return 0;
    }
    catch(const std::exception& e)
    {
        std::cerr << e.what() << std::endl;
        return 1;
    }
}