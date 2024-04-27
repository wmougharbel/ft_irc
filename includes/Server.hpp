/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: loandrad <loandrad@student.42wolfsburg.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/22 18:40:04 by walid             #+#    #+#             */
/*   Updated: 2024/04/26 18:23:33 by loandrad         ###   ########.fr       */
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
# include <map>

#define SERVER_IP "127.0.0.1"
#define CLIENT_LEFT " left the server!"
#define WELCOME ", Welcome to the IRC server. Don't get too comfortable.."
#define NO_AUTH ", You couldn't authenticate. Get the F out!!"

class Client;

class Server
{
    private:
        std::string _servIP;
        std::string _port;
        std::string _password;
        int _socket;
        std::vector<pollfd> _pfd;
        std::vector<std::string> _messages;
        std::map<int, Client> _clients;
        // std::vector<Channel *> _channList;
        void _initializeSocket(void);
        void _makeSocketNonBlocking(int sock);
        void _bindListenOnSocket(int sock, struct sockaddr_in addr);
        struct sockaddr_in _createSocketAddress(const std::string& ip, const std::string& port);

    public:
        Server(const std::string &port, const std::string &password);
        ~Server();
        void startServer(void);
        std::string getPassword(void) const;
        void newClientConnects(int sock, std::vector<pollfd> &pfds);
        void existingClientMessage(std::vector<pollfd> &pfds, int i);
        bool didClientAuthenticate(std::string &pass);
        void displayTime(void);
        void setClientInfo(int key);
        void printMessage(const std::string& message, int key);
        //Channel* makeChannel(const std::string &name);
        // void onClientDisconnect(int fd);
        // std::string readMessage(int fd);
        // void createChannel(const std::string& name, const std::string& key);
};

#endif
