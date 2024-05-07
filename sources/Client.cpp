#include "../includes/Client.hpp"
#include <unistd.h>

Client::Client() {}

Client::Client(int fd) : _fd(fd)
{
	_isOperator = false;
	_isAuthenticated = false;
	_nickname = "Anonymous";
	_username = "Anonymous";
	_pass = "";
}
// Client::Client(std::string nickname, std::string username)
// {
// 	if (nickname.empty() || username.empty())
// 		throw (std::runtime_error("Nickname and username cannot be empty!"));
// 	_isOperator = false;
// 	_isAuthenticated = false;
// 	_nickname = nickname;
// 	_username = username;
// }

// Client::Client (const Client &copy)
// {
// 	_isOperator = copy._isOperator;
// 	_isAuthenticated = copy._isAuthenticated;
// 	_nickname = copy._nickname;
// 	_username = copy._username;
// }

// Client  &Client::operator=(const Client &object)
// {
// 	if (this != &object)
// 	{
// 		_isOperator = object._isOperator;
// 		_isAuthenticated = object._isAuthenticated;
// 		_nickname = object._nickname;
// 		_username = object._username;
// 	}
// 	return (*this);
// }

Client::~Client() {}

bool Client::getOperator() const
{
	return (_isOperator);
}

bool Client::getAuthStatus() const
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

// std::string Client::getClientPassword() const
// {
// 	return (_clientPassword);
// }

std::string Client::getPass() const
{
	return (_pass);
}

int Client::getFd() const
{
	return (_fd);
}

void Client::setOperator(bool isOperator)
{
	_isOperator = isOperator;
}

void Client::setAuthStatus(bool status)
{
	_isAuthenticated = status;
}

void Client::setNickname(std::string nickname)
{
	_nickname = nickname;
}

void Client::setUsername(std::string username)
{
	_username = username;
}

// void	Client::setClientPassword(std::string clientPassword)
// {
// 	_clientPassword = clientPassword;
// }

// void	Client::setFd(int fd)
// {
// 	_fd = fd;
// }

void Client::setPass(std::string pass)
{
	_pass = pass;
}

void	Client::addChannel(const Channel &channel)
{
	_channels.push_back(channel);
}

std::vector<Channel> Client::getChannel() const
{
	return (_channels);
}
// void	Client::promote()
// {
// 	setOperator(true);
// 	std::cout << getUsername() << " has been promoted to operator" << std::endl;
// }

void Client::authenticate()
{
	// check if the client's entered password matches server's password
	// if yes change the auth satus to true
	// else throw an exception
	// if (_clientPassword != _server.getPassword())
	// 	throw (std::runtime_error("Incorrect Password!"));
	setAuthStatus(true);
	std::cout << "User authentication successful" << std::endl;
}

void Client::kick(const Client &toKick)
{
	if (this->getOperator() == false)
		throw(std::runtime_error("Only operators can kick clients out of a channel!"));
	std::cout << this->getUsername() << " kicked " << toKick.getUsername() << " out of the channel" << std::endl;
}

// void	Client::setServerPassword(std::string password)
// {
// 	(void)password;
// 	// _server.setPassword(password);
// 	// std::cout << "Server password: " << _server.getPassword() << std::endl;
// }

void Client::join(Channel channel)
{
	if (channel.isMember(_nickname))
	{
		std::cerr << _nickname << " is already a member here" << std::endl;
		return;
	}
	_channels.push_back(channel);
	_channels[_channels.size() - 1].addMember(*this);
}

void Client::sendMessage(std::vector<std::string> &message) const
{
	std::string buffer;
	size_t messageToSend;

	for (size_t i = 1; i < message.size(); i++)
		buffer += " " + message[i];
	buffer += "\r\n";
	messageToSend = send(_fd, buffer.c_str(), buffer.length(), 0);
	if (messageToSend < 0)
	{
		std::cerr << "Error, could not send message" << std::endl;
		return;
	}
    std::cout << buffer;
}

void	Client::sendToChannel(std::vector<std::string> &message, int fd) const
{
	size_t	j = 0;
	size_t i = 0;
	while (i < _channels.size())
	{
		j = 0;
		while (j < _channels[i].getMembers().size())
		{
			_channels[i].getMembers()[j].sendMessage(message);
			j++;
		}
		i++;
	}
}