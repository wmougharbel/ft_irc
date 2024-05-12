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
	std::vector<std::string> _channels;
	std::string _nickname;
	std::string _username;
	std::string	_realName;

public:
	Client(int fd);
	~Client();

	// getters
	std::string getNickname() const;
	std::string getUsername() const;
	int getFd() const;
	std::vector<std::string>	getChannel() const;

	// setters
	void setNickname(std::string &nickname);
	void setUsername(std::string &username);
	void addChannel(std::string &channel);

	// functions
	void sendMessage(std::vector<std::string> &message) const;
	void sendToChannel(std::vector<std::string> &message, int fd) const;
};

#endif
