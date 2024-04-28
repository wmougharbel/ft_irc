/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Irc.hpp                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: walid <walid@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/22 18:44:50 by walid             #+#    #+#             */
/*   Updated: 2024/04/22 18:44:50 by walid            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef Irc_H
# define Irc_H

# include <iostream>
# include <string>
# include <stdexcept>
# include <stdbool.h>
# include "Client.hpp"
# include "Server.hpp"
# include "Channel.hpp"
# include <map>
# include <sstream>

void    validateInput(std::string port, std::string password);
std::vector<std::string> ft_split(std::string message);
void    parser(std::string message);


#endif
