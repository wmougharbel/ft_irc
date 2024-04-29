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
void extractNickname(std::vector<std::string> incoming)
{
    std::cout << " => Client's nickname is : " << "\"" << incoming[1] << "\"" << std::endl;
}

void extractUsername(std::vector<std::string> incoming)
{
    std::cout << " => Client's username is : " << "\"" << incoming[1] << "\"" << std::endl;
}

void extractPassword(std::vector<std::string> incoming)
{
    std::cout << " => Client's password is : " << "\"" << incoming[1] << "\"" << std::endl;
}

void    checkOrder(std::vector<std::string> message)
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
        extractNickname(message);
        break;

    case 2:
        std::cout << " => USER command detected";
        extractUsername(message);
        break;

    case 3:
        std::cout << " => PASS command detected";
        extractPassword(message);
        break;

    default:
        std::cout << " => UNKNOWN command" << std::endl;
        break;
    }
}

void    parser(std::string message)
{
    std::vector<std::string>    split = ft_split(message);
    std::cout << "Msg Recvd From Client: " << "\"" << message << "\"";
    checkOrder(split);
}