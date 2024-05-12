#include "../includes/Client.hpp"
#include <unistd.h>

Client::Client() {}

Client::Client(int fd) : _fd(fd)
{
	_nickname = "Anonymous";
	_username = "Anonymous";
}

Client::~Client() {}

std::string Client::getNickname() const
{
	return (_nickname);
}

std::string Client::getUsername() const
{
	return (_username);
}

int Client::getFd() const
{
	return (_fd);
}

void Client::setNickname(std::string &nickname)
{
	_nickname = nickname;
}

void Client::setUsername(std::string &username)
{
	_username = username;
}

void	Client::addChannel(std::string &channel)
{
	_channels.push_back(channel);
}

std::vector<std::string> Client::getChannel() const
{
	return (_channels);
}

// void Client::sendMessage(std::vector<std::string> &message) const
// {
// 	std::string buffer;
// 	size_t messageToSend;

// 	for (size_t i = 1; i < message.size(); i++)
// 		buffer += " " + message[i];
// 	buffer += "\r\n";
// 	messageToSend = send(_fd, buffer.c_str(), buffer.length(), 0);
// 	if (messageToSend < 0)
// 	{
// 		std::cerr << "Error, could not send message" << std::endl;
// 		return;
// 	}
//     std::cout << buffer;
// }

//not sure about this one
// void	Client::sendToChannel(std::vector<std::string> &message, int fd) const
// {
// 	size_t	j = 0;
// 	size_t i = 0;
// 	while (i < _channels.size())
// 	{
// 		j = 0;
// 		while (j < _channels[i].getMembers().size())
// 		{
// 			_channels[i].getMembers()[j].sendMessage(message);
// 			j++;
// 		}
// 		i++;
//	}
// }
