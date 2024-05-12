/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: loandrad <loandrad@student.42wolfsburg.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/22 18:40:04 by walid             #+#    #+#             */
/*   Updated: 2024/05/07 12:08:41 by loandrad         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef SERVER_HPP
# define SERVER_HPP

# include "../includes/Client.hpp"
# include "../includes/Irc.hpp"
# include "../includes/Channel.hpp"

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
        std::map<int, Client>       _clients;
        std::vector<Channel>        _channList;
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
        void        displayTime(void);
        void        printMessage(const std::string& message, int fd);
        void        closeAll(std::map<int, Client> &clients, int i, std::vector<pollfd> &pfds);
        void        parser(std::string &message, std::map<int, Client> &clients, int i, std::vector<pollfd> &pfds);
        void        getCommand(std::vector<std::string> &message, std::map<int, Client> &clients, int fd, std::string &pass);
        void        createChannel(std::string &name, int fd, std::map<int, Client> &clients);
        std::vector<Channel> getChannels(void) const;
};

#endif
