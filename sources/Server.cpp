/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: marvin <marvin@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/22 18:45:30 by walid             #+#    #+#             */
/*   Updated: 2024/05/18 22:42:48 by marvin           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/Server.hpp"

bool isRunning = false;

Server::Server(const std::string &port, const std::string &password) : _servIP(SERVER_IP), _port(port), _password(password), _socket(-1)
{
	_initializeSocket();
	pollfd serverFd;
	serverFd.fd = _socket;
	serverFd.events = POLLIN;
	serverFd.revents = 0;
	_pfd.push_back(serverFd);
}

Server::~Server()
{
	if (_socket != -1)
	{
		close(_socket);
		_socket = -1; // Reset socket to invalid value
	}
	for (pollFdIterator i = _pfd.begin() + 1; i != _pfd.end(); ++i)
	{
		if (i->fd != -1)
		{
			close(i->fd);
			i->fd = -1;
		}
	}
}

void Server::_initializeSocket(void)
{
	struct sockaddr_in serv_addr;

	_socket = socket(AF_INET, SOCK_STREAM, 0);
	if (_socket == -1)
		throw std::runtime_error("Error : Failed to create a socket!");

	_makeSocketNonBlocking(_socket);
	serv_addr = _createSocketAddress(_servIP, _port);
	_bindListenOnSocket(_socket, serv_addr);
}

void Server::_makeSocketNonBlocking(int sock)
{
	int optval;
	optval = 1;
	if (setsockopt(sock, SOL_SOCKET, SO_REUSEADDR, &optval, sizeof(optval)))
		throw std::runtime_error("Error : Failed to set socket options!");

	if (fcntl(sock, F_SETFL, O_NONBLOCK))
		throw std::runtime_error("Error : Failed to set socket to non-blocking mode!");
}

struct sockaddr_in Server::_createSocketAddress(const std::string &ip, const std::string &port)
{
	struct sockaddr_in addr;

	bzero((char *)&addr, sizeof(addr));
	addr.sin_family = AF_INET;
	addr.sin_addr.s_addr = inet_addr(ip.c_str());
	addr.sin_port = htons(atoi(port.c_str()));
	return addr;
}

void Server::_bindListenOnSocket(int sock, struct sockaddr_in addr)
{
	if (bind(sock, (sockaddr *)&addr, sizeof(addr)) < 0)
		throw std::runtime_error("Error : Failed to bind the server socket!");

	if (listen(sock, SOMAXCONN) < 0) // SOMAXCONN allows the system to choose a value for backlog. I think it's set to 128 max.
		throw std::runtime_error("Error : Failed to listen on the server socket!");
}

void Server::startServer(void)
{
	while (isRunning == true)
	{
		if (poll(&_pfd[0], _pfd.size(), -1) < 0)
			throw std::runtime_error("Error : Failed to poll on the server socket!");
		for (pollFdIterator i = _pfd.begin(); i != _pfd.end(); ++i)
		{
			if ((i->revents & POLLIN) == POLLIN)
			{
				if (i->fd == _socket)
				{
					newClient(_socket, _pfd);
					break;
				}
				else
				{
					size_t index = std::distance(_pfd.begin(), i);
					existingClient(_pfd, index, _clients);
					break;
				}
			}
		}
	}
}

std::string Server::getPassword(void) const
{
	return _password;
}

void Server::newClient(int sock, std::vector<pollfd> &pfds)
{
	int clientFd;
	sockaddr_in addr = {};
	socklen_t size = sizeof(addr);

	clientFd = accept(sock, (sockaddr *)&addr, &size);
	if (clientFd == -1)
		throw std::runtime_error("Error : Failed to accept client on the server socket!");
	else
	{
		pollfd eachNewClient;
		eachNewClient.fd = clientFd;
		eachNewClient.events = POLLIN;
		eachNewClient.revents = 0;
		pfds.push_back(eachNewClient);

		_clients[clientFd] = Client(clientFd);
	}
}

void Server::existingClient(std::vector<pollfd> &pfds, int i, std::map<int, Client> &clients)
{
	std::string buf;
	char tempBuf[1024];
	std::string firstMsg;

	int readBytes = recv(pfds[i].fd, tempBuf, sizeof(tempBuf), 0);
	if (readBytes < 0)
	{
		if (errno == EAGAIN || errno == EWOULDBLOCK)
			return;
		else
			throw std::runtime_error("Error : reading data from client!");
	}
	else if (readBytes == 0 || (pfds[i].revents & POLLHUP) || (pfds[i].revents & POLLERR))
	{
		closeAll(clients, i, pfds);
		return;
	}
	else
	{
		// // APPROACH 1
		// char *end = strstr(tempBuf, "\r\n");
		// buf.append(tempBuf, end - tempBuf);
		// std::cout << buf << std::endl;
		// // parser(buf, clients, i, pfds);
		// buf.clear();

		// APPROACH 2
		buf.append(tempBuf, readBytes);
		size_t pos = buf.find("\r\n");
		while (pos != std::string::npos)
		{
			std::string message = buf.substr(0, pos);
			parser(message, clients, i, pfds);
			std::cout << message << std::endl;
			buf.erase(0, pos + 2); // +2 to remove the "\r\n"
			pos = buf.find("\r\n");
		}
	}
}

void Server::closeAll(std::map<int, Client> &clients, int i, std::vector<pollfd> &pfds)
{
	int clientFd = pfds[i].fd;
	
	if (close(clientFd) == -1)
		throw std::runtime_error("Error : closing client socket!");

	if (clientFd != -1)
		printMessage(CLIENT_LEFT, clientFd);

	clientIterator it = clients.find(clientFd);
	if (it != clients.end())
	{
		clients.erase(it);
		return ;
	}
	pfds.erase(pfds.begin() + i);
}

void Server::displayTime(void)
{
	time_t currentTime;
	char dateString[50];

	time(&currentTime);
	tm *localTime = localtime(&currentTime);
	strftime(dateString, sizeof(dateString), "%d-%m-%Y (%H:%M:%S) : ", localTime);

	std::cout << dateString;
}

void Server::printMessage(const std::string &message, int fd)
{
	displayTime();
	for (pollFdIterator i = _pfd.begin() + 1; i != _pfd.end(); ++i)
	{
		if (i->fd == fd)
		{
			std::cout << _clients[fd].getNickname() << message << std::endl;
			break;
		}
	}
}

void Server::parser(std::string &message, std::map<int, Client> &clients, int i, std::vector<pollfd> &pfds)
{
	int clientFd = pfds[i].fd;
	std::vector<std::string> split = ft_split(message);
	std::string servPass = getPassword();
	getCommand(split, clients, clientFd, servPass);
}

Channel     Server::createChannel(std::string &name, int fd, std::map<int, Client> &clients)
{
	Channel chan(name, clients[fd]);
	_channList.push_back(chan);
	return (chan);
}

void SignalHandler(int signum)
{
	if (signum == SIGINT)
	{
		std::cout << "\nInterrupt signal received. Shutting down server..\n";
		isRunning = false;
	}
	exit(signum);
}

std::vector<Channel> Server::getChannels(void) const
{
	return _channList;
}

void Server::extractNickname(std::vector<std::string> &incoming, std::map<int, Client> &clients, int fd)
{
	for (clientIterator it = clients.begin(); it != clients.end(); it++)
	{
		if (incoming[1] == it->second.getNickname())
			return (printInClient("Error, nickname already exists.", fd));
	}
	clients[fd].setNickname(incoming[1]);
}

void Server::extractUsername(std::vector<std::string> &incoming, std::map<int, Client> &clients, int fd)
{
	clients[fd].setUsername(incoming[1]);
}

Channel*	Server::findChannel(const std::string& name)
{
	for (size_t i = 0; i < _channList.size(); i++)
	{
		if (name == _channList[i].getName())
			return (&_channList[i]);
	}
	return (NULL);
}

void	Server::sendMessageToUser(std::vector<std::string> &message, std::map<int, Client> &clients, int fd)
{
	if (message.size() < 3)
		return (printInClient("Usage: </PRIVMSG> <TARGET> <message>", fd));
	std::string target = message[1];
	std::vector<std::string> text;
	clientIterator it = clients.begin();

	for (size_t i = 1; i < message.size(); i++)
		text.push_back(message[i]);
	while (it != clients.end())
	{
		if (it->second.getNickname() == target)
		{
			clients[fd].sendMessage(text, it->first);
		}
		it++;
	}
}

void	Server::sendMessageToChannel(std::vector<std::string> &message, std::map<int, Client> &clients, int fd)
{
	if (message.size() < 3)
		return (printInClient("Usage: </PRIVMSG> <TARGET> <message>", fd));
	std::string target = message[1].substr(1, message[1].length() - 1);
	std::vector<std::string> text;
	
	for (size_t i = 1; i < message.size(); i++)
		text.push_back(message[i]);
	for (size_t i = 0; i < _channList.size(); i++)
	{
		if (_channList[i].getName() == target && _channList[i].isMember(clients[fd].getNickname()))
		{
			for (size_t j = 0; j < _channList[i].getMembers().size(); j++)
			{
				if (fd != _channList[i].getMembers()[j].getFd())
				{
					clients[fd].sendMessage(text, _channList[i].getMembers()[j].getFd());
				}
			}
		}
	}
}

void	Server::privMsg(std::vector<std::string> &message, std::map<int, Client> &clients, int fd)
{
	if (message.size() < 3)
		return (printInClient("Usage: /PRIVMSG #<channel> <message>", fd));
	if (message[1][0] == '#')
		sendMessageToChannel(message, clients, fd);
	else
		sendMessageToUser(message, clients, fd);
}

void Server::kick(std::vector<std::string> &message, std::map<int, Client> &clients, int fd)
{
	if (message.size() != 4)
		return (printInClient("Usage: </KICK> <CHANNEL> <NICK> <REASON>", fd));
	std::string	target = message[3].substr(1, message[3].length() - 1);
	std::string channel = message[2].substr(1, message[2].length() - 1);

	for (channelIterator it = _channList.begin(); it != _channList.end(); it++)
	{
		if (channel == it->getName())
		{
			for (std::vector<Client>::iterator clientIt = it->getOperators().begin(); clientIt != it->getOperators().end(); clientIt++)
			{
				if (clientIt->getNickname() == clients[fd].getNickname())
				{
					if (it->isMember(target) && !it->hasOperatorPrivileges(target))
					{
						it->removeMember(target);
						std::cout << target << " was kicked out of " << it->getName() << std::endl;
						printInClient("You kicked " + target + " out of " + it->getName(), fd);
					}
				}
			}
		}
	}
}

void Server::invite(std::vector<std::string> &message, std::map<int, Client> &clients, int fd)
{
	std::string	target = message[1];
	std::string channel = message[2].substr(1, message[2].length() - 1);
	channelIterator	it = _channList.begin();
	clientIterator	clientIt = clients.begin();

	for (it = _channList.begin(); it != _channList.end(); it++)
		if (it->getName() == channel)
			break;
	if (it == _channList.end())
	{
		printInClient("Couldn't invite client, check your channel's name!", fd);
		return ;
	}
	if (it->isMember(clients[fd].getNickname()) && it->hasOperatorPrivileges(clients[fd].getNickname()))
	{
		for (clientIt = clients.begin(); clientIt != clients.end(); clientIt++)
			if (clientIt->second.getNickname() == target)
				break ;
		if (clientIt == clients.end())
		{
			printInClient("Couldn't invite client, check your client's name!", fd);
			return ;
		}
		it->addMember(clientIt->second);
		printInClient(clients[fd].getNickname() + " invited " + target + " to " + channel, fd);
		printInClient(clients[fd].getNickname() + " invited you " + " to " + channel, clientIt->first);
	}
}

void Server::leave(std::vector<std::string> &message, std::map<int, Client> &clients, int fd)
{
	std::string	channel;
	std::vector<std::string> reason;
	channelIterator it = _channList.begin();
	std::string names;
	std::string reply;

	if (message.size() < 3)
	{
		printInClient("Error, usage: /PART #<channel>", fd);
		return ;
	}
	for (size_t i = 2; i < message.size(); i++)
		reason.push_back(message[i]);
	channel = message[1].substr(1, message[1].length() - 1);
	for (it = _channList.begin(); it != _channList.end(); it++)
	{
		if (it->getName() == channel)
		{
			if (it->isMember(clients[fd].getNickname()))
			{
				for (size_t i = 0; i < it->getMembers().size(); i++)
				{
					reply = clients[fd].getNickname() + " has left #" + channel + "\r\n";
					send(it->getMembers()[i].getFd(), reply.c_str(), reply.length(), 0);
				}
				it->removeMember(clients[fd].getNickname());
				for (size_t i = 0; i < it->getMembers().size(); i++)
				{
					for (size_t j = 0; j < it->getOperators().size(); j++)
					{
						if (it->getOperators()[j].getNickname() == it->getMembers()[i].getNickname())
							names += "@";
					}
					names += it->getMembers()[i].getNickname();
					names += " ";
				}
				for (size_t i = 0; i < it->getMembers().size(); i++)
				{
					reply = std::string(":") + SERVER_IP + " 353 " + it->getMembers()[i].getNickname() + " = #" + channel + " :" + names + "\r\n";
					send(it->getMembers()[i].getFd(), reply.c_str(), reply.length(), 0);
					reply = std::string(":") + SERVER_IP + " 366 " + it->getMembers()[i].getNickname() + " #" + channel + " :End of /NAMES list\r\n";
					send(it->getMembers()[i].getFd(), reply.c_str(), reply.length(), 0);
				}
				reply = std::string(":") + SERVER_IP + " 353 " + clients[fd].getNickname() + " = #" + channel + " :" + names + "\r\n";
				send(fd, reply.c_str(), reply.length(), 0);
				reply = std::string(":") + SERVER_IP + " 366 " + clients[fd].getNickname() + " #" + channel + " :End of /NAMES list\r\n";
				send(fd, reply.c_str(), reply.length(), 0);
				if(it->getMembers().size() == 0)
				{
					std::cout << "Channel #"<< it->getName() << " is now empty and has been removed from the server!" << std::endl;
					_channList.erase(it);
					printInClient("Channel was deleted because of you...", fd);
					return ;
				}
			}
		}
	}
}

void Server::extractPassword(std::vector<std::string> &incoming, std::map<int, Client> &clients, int fd, std::string &serverPass)
{
	if (serverPass == incoming[1])
	{
		clients[fd].setAuthStatus(true);
	}
	else
	{
		send(fd, "Password Incorrect\n", 20, 0);
	}
}

void Server::getCommand(std::vector<std::string> &message, std::map<int, Client> &clients, int fd, std::string &pass)
{
	std::string commands[] = {"JOIN", "NICK", "USER", "PASS", "PRIVMSG", "KICK", "INVITE", "PART", "MODE"};
	std::string channel_name;
	std::string receivedCommand = capitalize(message[0]);
	size_t i;
	bool channel_exists = false;

	for (i = 0; i < sizeof(commands) / sizeof(commands[0]); i++)
		if (receivedCommand == commands[i])
			break;
	switch (i)
	{
		case 0:
			if (clients[fd].getAuthStatus())
			{
				channel_name = message[1].substr(1, message[1].find(' ') - 1);
				std::string names;
				for (channelIterator it = _channList.begin(); it != _channList.end(); ++it)
				{
					if (it->getName() == channel_name)
					{
						it->addMember(clients[fd]);
						std::string reply = ":" + clients[fd].getNickname() + " JOIN #" + channel_name + "\r\n";
						send(fd, reply.c_str(), reply.length(), 0);
						for (size_t i = 0; i < it->getMembers().size(); i++)
						{
							for (size_t j = 0; j < it->getOperators().size(); j++)
							{
								if (it->getOperators()[j].getNickname() == it->getMembers()[i].getNickname())
									names += "@";
							}
							names += it->getMembers()[i].getNickname();
							names += " ";
						}
						reply = std::string(":") + SERVER_IP + " 332 " + clients[fd].getNickname() + " #" + channel_name + " " + it->getTopic() + "\r\n";
						send(fd, reply.c_str(), reply.length(), 0);

						for (size_t i = 0; i < it->getMembers().size(); i++)
						{
							reply = std::string(":") + SERVER_IP + " 353 " + it->getMembers()[i].getNickname() + " = #" + channel_name + " :" + names + "\r\n";
							send(it->getMembers()[i].getFd(), reply.c_str(), reply.length(), 0);
							reply = std::string(":") + SERVER_IP + " 366 " + it->getMembers()[i].getNickname() + " #" + channel_name + " :End of /NAMES list\r\n";
							send(it->getMembers()[i].getFd(), reply.c_str(), reply.length(), 0);
						}
						displayTime();
						std::cout << clients[fd].getNickname() << " added to " << channel_name << std::endl;
						channel_exists = true;
						break;
					}
				}
				if (!channel_exists)
				{
					Channel chan = createChannel(channel_name, fd, clients);

					//join reply
					std::string reply = ":" + clients[fd].getNickname() + " JOIN #" + channel_name + "\r\n";
					send(fd, reply.c_str(), reply.length(), 0);
					//topic reply
					reply = std::string(":") + SERVER_IP + " 332 " + clients[fd].getNickname() + " #" + channel_name + " " + chan.getTopic() + "\r\n";
					std::cout << "topic :" << chan.getTopic() << std::endl;
					send(fd, reply.c_str(), reply.length(), 0);
					//names reply
    				reply = std::string(":") + SERVER_IP + " 353 " + clients[fd].getNickname() + " = #" + channel_name + " :@" + clients[fd].getNickname() + "\r\n";
					send(fd, reply.c_str(), reply.length(), 0);
					//end_of_names reply
    				reply = std::string(":") + SERVER_IP + " 366 " + clients[fd].getNickname() + " #" + channel_name + " :End of /NAMES list\r\n";
					send(fd, reply.c_str(), reply.length(), 0);

					displayTime();
					std::cout << clients[fd].getNickname() << " added to " << channel_name << std::endl;    
				}
			}
			break;
		
		case 1:
			if (clients[fd].getAuthStatus())
			{
				extractNickname(message, clients, fd);
				displayTime();
				std::cout << clients[fd].getNickname() << CLIENT_JOINED << std::endl;
				std::string reply = clients[fd].getNickname() + ", Welcome to the FT_IRC network!\r\n";
				send(fd, reply.c_str(), reply.length(), 0);
			}
			break;

		case 2:
			if (clients[fd].getAuthStatus())
			{
				extractUsername(message, clients, fd);
			}
			break;

		case 3:
			extractPassword(message, clients, fd, pass);
			break;

		case 4:
			if (clients[fd].getAuthStatus())
			{
				//before calling privMsg, need to check if "nickname" / "channel name" is valid and exists.
				privMsg(message, clients, fd);
			}
			break ;
		
		case 5:
			kick(message, clients, fd);
			break ;

		case 6:
			invite(message, clients, fd);
			break;
		
		case 7:
			leave(message, clients, fd);
			break ;

		case 8:
			channel_name = message[1].substr(1);
			Channel* channel = findChannel(channel_name);
			if (!channel) {
				std::string reply = ":127.0.0.1 403 " + clients[fd].getNickname() + " #" + channel_name + " :No such channel\r\n";
				send(fd, reply.c_str(), reply.length(), 0);
			} else {
				channel->mode(std::vector<std::string>(message.begin() + 2, message.end()), clients[fd]);
			}
			break;
			
		default:
			break;
		}
}

int main(int argc, char *argv[])
{
	if (argc != 3)
	{
		std::cout << TO_RUN << std::endl;
		return 1;
	}

	try
	{
		validateInput(argv[1], argv[2]);
	}
	catch(const std::exception& e)
	{
		std::cerr << e.what() << std::endl;
		return 1;
	}

	signal(SIGINT, SignalHandler);
	Server serv(argv[1], argv[2]);
	isRunning = true;

	try
	{
		serv.startServer();
		return 0;
	}
	catch (const std::exception &e)
	{
		std::cerr << e.what() << std::endl;
		return 1;
	}
}
