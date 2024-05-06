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
    if (!checkOrder(clients, fd))
        return;
    if (incoming.size() != 2)
    {
        std::cerr << "Error, Nickname should contain two parameters" << std::endl;
        return;
    }
    if (incoming[1].empty() || incoming[1] == "Anonymous")
    {
        std::cerr << "Error, Nickname cannot be empty" << std::endl;
        return;
    }
    clients[fd].setNickname(incoming[1]);
    std::cout << " => Client's nickname is set as : " << clients[fd].getNickname() << std::endl;
}

void extractUsername(std::vector<std::string> &incoming, std::map<int, Client> &clients, int fd)
{
    if (incoming.size() != 5)
    {
        std::cerr << "Error, Username should contain four parameters" << std::endl;
        return;
    }
    if (incoming[1].empty() || incoming[1] == "Anonymous")
    {
        std::cerr << "Error, Username cannot be empty" << std::endl;
        return;
    }
    if (incoming[2] != "0" || incoming[3] != "*")
    {
        std::cerr << "Error, invalid parameters" << std::endl;
        return;
    }
    clients[fd].setUsername(incoming[1]);
    if (!checkOrder(clients, fd))
        return (clients[fd].setUsername("Anonymous"));
    std::cout << " => Client's username is set as : " << clients[fd].getUsername() << std::endl;
}

void extractPassword(std::vector<std::string> &incoming, std::map<int, Client> &clients, int fd, std::string &serverPass)
{
    if (serverPass == incoming[1])
    {
        clients[fd].setPass(incoming[1]);
        clients[fd].setAuthStatus(true);
        std::cout << " => client at fd : " << fd << " authenticated and their auth status is set to : " << clients[fd].getAuthStatus() << std::endl;
    }
    else
        std::cout << " => client at fd : " << fd << " could not authenticate" << std::endl;
}

void getCommand(std::vector<std::string> &message, std::map<int, Client> &clients, int fd, std::string &pass)
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
        extractPassword(message, clients, fd, pass);
        break;

    default:
        clients[fd].sendToChannel(message, fd);
        break;
    }
        clients[fd].sendToChannel(message, fd);

}
