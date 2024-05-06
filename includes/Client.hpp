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
	bool _isOperator;
	bool _isAuthenticated;
	std::vector<Channel> _channels;
	std::string _nickname;
	std::string _username;
	std::string _pass;

public:
	Client();
	Client(int fd);
	// Client (std::string nickname, std::string username);
	// Client (const Client &);
	// Client	&operator=(const Client &);
	~Client();

	// getters
	bool getOperator() const;
	bool getAuthStatus() const;
	std::string getNickname() const;
	std::string getUsername() const;
	std::string getPass() const;
	int getFd() const;

	// setters
	void setOperator(bool isOperator);
	void setAuthStatus(bool status);
	void setNickname(std::string nickname);
	void setUsername(std::string username);
	void setPass(std::string pass);
	void addChannel(const Channel &);

	// functions
	void authenticate();
	void join(Channel channel);
	void kick(const Client &toKick);
	void sendMessage(std::vector<std::string> &message) const;
	void sendToChannel(std::vector<std::string> &message, int fd) const;
};

#endif
