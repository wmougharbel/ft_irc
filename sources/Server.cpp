/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: loandrad <loandrad@student.42wolfsburg.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/22 18:45:30 by walid             #+#    #+#             */
/*   Updated: 2024/04/23 17:27:37 by loandrad         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/Server.hpp"

#define SERVER_IP "127.0.0.1"

bool isRunning = false;

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
        if (poll(&_pfd[0], _pfd.size(), -1) < 0)
            throw std::runtime_error("Error : Failed to poll on the server socket!");
        // check for events on each socket in the _pfd
        for (size_t i = 0; i < _pfd.size(); i++)
        {
            if((_pfd[i].revents & POLLIN) == POLLIN)
            {
                if (i == 0)
                {
                    newClientConnects(_socket, _pfd);
                    // break;
                }
                else
                    existingClientMessage(_pfd, i);
            }
        }
    }
}

std::string Server::getPassword(void) const
{
    return _password;
}

void Server::newClientConnects(int sock, std::vector<pollfd> &pfds)
{
    int         newClientFd;
    sockaddr_in addr = {};
    socklen_t   size = sizeof(addr);

    newClientFd = accept(sock, (sockaddr *)&addr, &size);
    if (newClientFd == -1)
        throw std::runtime_error("Error : Failed to accept client on the server socket!");
    else
    {
        pollfd eachNewClient;
        eachNewClient.fd = newClientFd;
        eachNewClient.events = POLLIN;
        eachNewClient.revents = 0;
        pfds.push_back(eachNewClient);

        Client newClient = Client(newClientFd);
        _clients.push_back(newClient);
    }
}

void Server::existingClientMessage(std::vector<pollfd> &pfds, int i)
{
    std::string buf;
    char tempBuf[1024];
    std::string initialMsgs;

    while (true)
    {
        int readBytes = read(pfds[i].fd, tempBuf, sizeof(tempBuf));
        if (readBytes < 0)
        {
            if (errno == EAGAIN || errno == EWOULDBLOCK)
                break;
            else
                throw std::runtime_error("Error : reading data from client!");
        }
        else if (readBytes == 0 || (pfds[i].revents & POLLHUP) || (pfds[i].revents & POLLERR))
        {
            if (close(pfds[i].fd) == -1)
                throw std::runtime_error("Error : closing client socket!");
            std::cout << "client no. [" << pfds[i].fd << "] disconnected!" << std::endl;
            for(size_t j = 0; j < _clients.size(); j++)
            {
                if(_clients[j].getFd() == pfds[i].fd)
                {
                    _clients.erase(_clients.begin() + j);
                    break;
                }
            }
            pfds.erase(pfds.begin() + i);
            return;
        }
        else
        {
            char *end = strstr(tempBuf, "\r\n");
            buf.append(tempBuf, end - tempBuf);
            _messages.push_back(buf);
            buf.clear();
        }
        if (_messages.size() >= 3)
        {
            initialMsgs = _messages[0].substr(5, std::string::npos);
            if (didClientAuthenticate(initialMsgs))
            {
                std::string nickname = _messages[1].substr(5);
                _clients[i - 1].setNickname(nickname);

                std::string userMessage = _messages[2];
                size_t spacePos = userMessage.find(' ');
                if (spacePos != std::string::npos)
                {
                    size_t nextSpacePos = userMessage.find(' ', spacePos + 1);
                    std::string username = userMessage.substr(spacePos + 1);
                    if (nextSpacePos != std::string::npos)
                    {
                        std::string username = userMessage.substr(spacePos + 1, nextSpacePos - spacePos - 1);
                        _clients[i - 1].setUsername(username); 
                    }
                }
                std::cout << "Welcome to the IRC server " << _clients[i - 1].getNickname() << ". Don't you get too comfortable.." << std::endl;
            }
            else
            {
                std::cout << "Sorry " << _clients[i - 1].getNickname() << ". Your couldn't authenticate. Get Out!!" << std::endl;
                if (close(pfds[i].fd) == -1)
                    throw std::runtime_error("Error: closing client socket!");
                std::cout << "Client no. [" << pfds[i].fd << "] disconnected!" << std::endl;
                pfds.erase(pfds.begin() + i);
            }
            _messages.clear();
            break;
        }
    }
}

// Channel* Server::makeChannel(const std::string &name)
// {
//     Channel* ch = new Channel(name);
//     _channList.push_back(ch);

//     return ch;
// }

bool Server::didClientAuthenticate(std::string &pass)
{
    if (pass == getPassword())
        return true;
    else
        return false;
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
