#include "../includes/Client.hpp"

Client::Client(){}

Client::Client(bool isOperator, std::string nickname, std::string username)
{
	_isOperator = isOperator;
	_nickname = nickname;
	_username = username;
}

Client::Client (const Client &copy)
{
	_isOperator = copy._isOperator;
	_nickname = copy._nickname;
	_username = copy._username;
}

Client  &Client::operator=(const Client &object)
{
	if (this != &object)
	{
		_isOperator = object._isOperator;
		_nickname = object._nickname;
		_username = object._username;
	}
	return (*this);
}

Client::~Client(){}

bool    Client::getOperator()
{
	return (_isOperator);
}

std::string Client::getNickname()
{
	return (_nickname);
}

std::string Client::getUsername()
{
	return (_username);
}

void    Client::setOperator(bool isOperator)
{
	_isOperator = isOperator;
}

void    Client::setNickname(std::string nickname)
{
	_nickname = nickname;
}

void    Client::setUsername(std::string username)
{
	_username = username;
}