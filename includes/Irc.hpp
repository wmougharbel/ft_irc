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

# include <string>
# include <stdbool.h>
# include <sstream>
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
# include <map>
# include "Client.hpp"
# include "Server.hpp"
# include "Channel.hpp"

#define SERVER_IP "127.0.0.1"
#define CLIENT_LEFT " left the server!"
#define CLIENT_JOINED " joined the server!"
#define WELCOME ", Welcome to the IRC server. Don't get too comfortable.."
#define NO_AUTH ", couldn't authenticate!"
#define TO_RUN "To use program : ./ircserv <4-digit PORT> <4-8 CHAR LONG PASSWORD>"

void    validateInput(std::string port, std::string password);
std::vector<std::string> ft_split(std::string message);
void    getCommand(std::vector<std::string> &message, std::map<int, Client> &clients, int fd, std::string &pass);
void printInClient(std::string message, int fd);
std::string capitalize(std::string command);

#endif
