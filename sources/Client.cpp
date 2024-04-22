/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Client.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: walid <walid@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/22 18:45:10 by walid             #+#    #+#             */
/*   Updated: 2024/04/22 18:45:10 by walid            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */



#include "../includes/Client.hpp"

Client::Client(){}

Client::Client(std::string nickname, std::string username)
{
	if (nickname.empty() || username.empty())
		throw (std::runtime_error("Nickname and username cannot be empty!"));
	_isOperator = false;
	_isAuthenticated = false;
	_nickname = nickname;
	_username = username;
}

Client::Client (const Client &copy)
{
	_isOperator = copy._isOperator;
	_isAuthenticated = copy._isAuthenticated;
	_nickname = copy._nickname;
	_username = copy._username;
}

Client  &Client::operator=(const Client &object)
{
	if (this != &object)
	{
		_isOperator = object._isOperator;
		_isAuthenticated = object._isAuthenticated;
		_nickname = object._nickname;
		_username = object._username;
	}
	return (*this);
}

Client::~Client(){}

bool    Client::getOperator() const
{
	return (_isOperator);
}

bool	Client::getAuthStatus() const
{
	return (_isAuthenticated);
}

std::string Client::getNickname() const
{
	return (_nickname);
}

std::string Client::getUsername() const
{
	return (_username);
}

std::string Client::getClientPassword() const
{
	return (_clientPassword);
}

void    Client::setOperator(bool isOperator)
{
	_isOperator = isOperator;
}

void	Client::setAuthStatus(bool status)
{
	_isAuthenticated = status;
}

void    Client::setNickname(std::string nickname)
{
	_nickname = nickname;
}

void    Client::setUsername(std::string username)
{
	_username = username;
}

void	Client::setClientPassword(std::string clientPassword)
{
	_clientPassword = clientPassword;
}

void	Client::promote()
{
	setOperator(true);
	std::cout << getUsername() << " has been promoted to operator" << std::endl;
}

void	Client::authenticate()
{
	// check if the client's entered password matches server's password
	// if yes change the auth satus to true
	// else throw an exception
	// if (_clientPassword != _server.getPassword())
	// 	throw (std::runtime_error("Incorrect Password!"));
	setAuthStatus(true);
	std::cout << "User authentication successful" << std::endl;
}

void	Client::kick(const Client &toKick)
{
	if (this->getOperator() == false)
		throw (std::runtime_error("Only operators can kick clients out of a channel!"));
	std::cout << this->getUsername() << " kicked " << toKick.getUsername() << " out of the channel" << std::endl;
}

void	Client::setServerPassword(std::string password)
{
	(void)password;
	// _server.setPassword(password);
	// std::cout << "Server password: " << _server.getPassword() << std::endl;
}
