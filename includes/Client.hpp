/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Client.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: walid <walid@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/22 18:44:43 by walid             #+#    #+#             */
/*   Updated: 2024/04/22 18:44:43 by walid            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef CLIENT_HPP
#define CLIENT_HPP

#include "Irc.hpp"
#include <vector>

class Channel;

class Client
{
private:
	int _fd;
	bool	_isAuthenticated;
	std::vector<std::string> _channels;
	std::string _nickname;
	std::string _username;
	std::string	_realName;

public:
	Client();
	Client(int fd);
	~Client();

	// getters
	std::string getNickname() const;
	std::string getUsername() const;
	int getFd() const;
	std::vector<std::string>	getChannel() const;
	bool	getAuthStatus();

	// setters
	void setNickname(std::string &nickname);
	void setUsername(std::string &username);
	void addChannel(std::string &channel);
	void	setAuthStatus(bool	status);

	// functions
	void sendMessage(std::vector<std::string> &message, int fd) const;
	// void sendToChannel(std::vector<std::string> &message, int fd) const;
};

#endif
