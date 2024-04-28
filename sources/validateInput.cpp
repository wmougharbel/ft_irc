/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   validateInput.cpp                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: walid <walid@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/22 18:45:38 by walid             #+#    #+#             */
/*   Updated: 2024/04/22 18:45:38 by walid            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/Irc.hpp"

bool    isPortNumeric(std::string port)
{
    for (size_t i = 0; i < port.length(); i++)
        if (!isdigit(port[i]))
            return (false);
    return (true);
}

bool    getPortLength(std::string port)
{
    if (port.length() != 4)
        return (false);
    return (true);
}

bool    getPasswordLength(std::string password)
{
    if (password.length() < 4 || password.length() > 8)
        return (false);
    return (true);
}

void    validateInput(std::string port, std::string password)
{
    if (!isPortNumeric(port))
        throw (std::runtime_error("Port argument should only contain digits!"));
    if (!getPortLength(port))
        throw (std::runtime_error("Port argument should contain 4 digits!"));
    if (!getPasswordLength(password))
        throw (std::runtime_error("Password argument should be between 4 and 8 characters long!"));
}

std::vector<std::string> ft_split(std::string message)
{
    std::vector<std::string> split;
    std::stringstream ss(message);
    std::string token;

    while (ss >> token)
        split.push_back(token);
    return (split);
}
