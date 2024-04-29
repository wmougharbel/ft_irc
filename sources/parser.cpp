/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: marvin <marvin@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/28 21:52:19 by marvin            #+#    #+#             */
/*   Updated: 2024/04/28 21:52:19 by marvin           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/Irc.hpp"

void extractNickname(std::vector<std::string> &incoming, std::map<int, Client> &clients, int fd)
{
    clients[fd].setNickname(incoming[1]);
    std::cout << " => Client's nickname is set as : " << clients[fd].getNickname() << std::endl;
}

void extractUsername(std::vector<std::string> &incoming, std::map<int, Client> &clients, int fd)
{
    clients[fd].setUsername(incoming[1]);
    std::cout << " => Client's username is set as : " << clients[fd].getUsername() << std::endl;
}

void extractPassword(std::vector<std::string> &incoming, std::map<int, Client> &clients, int fd)
{
    std::cout << " => Here is where authentication operation will happen." << std::endl;
}

void    checkOrder(std::vector<std::string> &message, std::map<int, Client> &clients, int fd)
{
    std::string commands[] = {"JOIN", "NICK", "USER", "PASS"};
    size_t i;

    for (i = 0; i < sizeof(commands) / sizeof(commands[0]); i++)
        if (message[0] == commands[i])
            break;
    switch (i)
    {
    case 0:
        std::cout << " => JOIN command detected" << std::endl;
        break;

    case 1:
        std::cout << " => NICK command detected";
        extractNickname(message, clients, fd);
        break;

    case 2:
        std::cout << " => USER command detected";
        extractUsername(message, clients, fd);
        break;

    case 3:
        std::cout << " => PASS command detected";
        extractPassword(message, clients, fd);
        break;

    default:
        std::cout << " => UNKNOWN command" << std::endl;
        break;
    }
}

void    parser(std::string &message, std::map<int, Client> &clients, int i, std::vector<pollfd> &pfds)
{
    int clientFd = pfds[i].fd;
    std::vector<std::string>    split = ft_split(message);
    checkOrder(split, clients, clientFd);
}