/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: walid <walid@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/02 10:15:16 by walid             #+#    #+#             */
/*   Updated: 2024/05/02 10:15:16 by walid            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/Irc.hpp"

void extractNickname(std::vector<std::string> &incoming, std::map<int, Client> &clients, int fd)
{
    clients[fd].setNickname(incoming[1]);
}

void extractUsername(std::vector<std::string> &incoming, std::map<int, Client> &clients, int fd)
{
    clients[fd].setUsername(incoming[1]);
}

void extractPassword(std::vector<std::string> &incoming, std::map<int, Client> &clients, int fd, std::string &serverPass)
{
    if (serverPass == incoming[1])
    {
        clients[fd].setPass(incoming[1]);
        clients[fd].setAuthStatus(true);
    }
    else
        std::cout << "Client at : " << fd << "couldn't authenticate";
}

void Server::getCommand(std::vector<std::string> &message, std::map<int, Client> &clients, int fd, std::string &pass)
{
    std::string commands[] = {"JOIN", "NICK", "USER", "PASS"};
    std::string channel_name;
    size_t i;
    int auth_status;
    bool channel_exists = false;

    for (i = 0; i < sizeof(commands) / sizeof(commands[0]); i++)
        if (message[0] == commands[i])
            break;
    
    switch (i)
    {
    case 0:
    channel_name = message[1].substr(1, message[1].find(' ') - 1);
    for (std::vector<Channel>::iterator it = _channList.begin(); it != _channList.end(); ++it)
    {
        if (it->getName() == channel_name)
        {
            it->addMember(clients[fd]);
            displayTime();
            std::cout << clients[fd].getNickname() << " added to " << channel_name << std::endl;
            channel_exists = true;
            break;
        }
    }
    if (!channel_exists)
    {
        createChannel(channel_name, fd, clients);
        displayTime();
        std::cout << clients[fd].getNickname() << " added to " << channel_name << std::endl;    
    }
    break;
    case 1:
        extractNickname(message, clients, fd);
        displayTime();
        std::cout << clients[fd].getNickname() << CLIENT_JOINED << std::endl;
        break;

    case 2:
        extractUsername(message, clients, fd);
        break;

    case 3:
        extractPassword(message, clients, fd, pass);
        break;

    default:
        break;
    }
}
