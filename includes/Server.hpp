/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: marvin <marvin@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/22 18:40:04 by walid             #+#    #+#             */
/*   Updated: 2024/05/06 19:49:27 by marvin           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef SERVER_HPP
# define SERVER_HPP

# include <iostream>
# include <vector>
# include <stdexcept>
# include <unistd.h>
# include <poll.h>
# include <netinet/in.h>
# include <fcntl.h>
# include <arpa/inet.h>
# include <signal.h>
# include <cstring>
# include <stdlib.h>
# include <algorithm>
# include <err.h>
# include "../includes/Client.hpp"
# include "../includes/Irc.hpp"
# include "../includes/Channel.hpp"
# include <map>

#define SERVER_IP "127.0.0.1"
#define CLIENT_LEFT " left the server!"
#define WELCOME ", Welcome to the IRC server. Don't get too comfortable.."
#define NO_AUTH ", You couldn't authenticate. Get the F out!!"

class Client;
class Channel;

class Server
{
    private:
        std::string                 _servIP;
        std::string                 _port;
        std::string                 _password;
        int                         _socket;
        std::vector<pollfd>         _pfd;
        std::vector<std::string>    _messages;
        std::map<int, Client>       _clients;
        // std::vector<Channel *>      _channList;
        void                        _initializeSocket(void);
        void                        _makeSocketNonBlocking(int sock);
        void                        _bindListenOnSocket(int sock, struct sockaddr_in addr);
        struct sockaddr_in          _createSocketAddress(const std::string& ip, const std::string& port);

    public:
        Server(const std::string &port, const std::string &password);
        ~Server();
        std::string getPassword(void) const;
        void        startServer(void);
        void        newClient(int sock, std::vector<pollfd> &pfds);
        void        existingClient(std::vector<pollfd> &pfds, int i, std::map<int, Client> &clients);
        bool        didClientAuthenticate(std::string &pass);
        void        displayTime(void);
        void        setClientInfo(int key);
        void        printMessage(const std::string& message, int fd);
        void        closeAll(std::map<int, Client> &clients, int i, std::vector<pollfd> &pfds);
        void        parser(std::string &message, std::map<int, Client> &clients, int i, std::vector<pollfd> &pfds);
};

#endif
