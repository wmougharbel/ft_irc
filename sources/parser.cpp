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

void    checkOrder(std::vector<std::string> message)
{
    std::string commands[] = {"JOIN", "NICK", "USER", "PASS"};
    size_t  i = 0;

    for (i; i < sizeof(commands) / sizeof(commands[0]); i++)
        if (message[0] == commands[i])
            break;
    switch (i)
    {
    case 0:
        std::cout << "Join command detected" << std::endl;
        break;

    case 1:
        std::cout << "Nick command detected" << std::endl;
        break;

    case 2:
        std::cout << "User command detected" << std::endl;
        break;

    case 3:
        std::cout << "Pass command detected" << std::endl;
        break;

    default:
        std::cout << "Unknown command" << std::endl;
        break;
    }
}

void    parser(std::string message)
{
    std::cout << "Message: " << message << std::endl;
    std::vector<std::string>    split = ft_split(message);
    for (size_t i = 0; i < split.size(); i++)
        std::cout << i << ": " << split[i] << std::endl;
    checkOrder(split);
}