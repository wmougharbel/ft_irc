/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: walid <walid@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/22 18:45:30 by walid             #+#    #+#             */
/*   Updated: 2024/05/14 19:42:27 by walid            ###   ########.fr       */
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
	for (std::vector<pollfd>::iterator it = _pfd.begin() + 1; it != _pfd.end(); ++it)
	{
		if (it->fd != -1)
		{
			close(it->fd);
			it->fd = -1;
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
		for (std::vector<pollfd>::iterator it = _pfd.begin(); it != _pfd.end(); ++it)
		{
			if ((it->revents & POLLIN) == POLLIN)
			{
				if (it->fd == _socket)
				{
					newClient(_socket, _pfd);
					break;
				}
				else
				{
					size_t index = std::distance(_pfd.begin(), it);
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
			buf.erase(0, pos + 2); // +2 to remove the "\r\n"
			pos = buf.find("\r\n");
		}

		// APPROACH 3
		// std::string message(tempBuf, readBytes);
		//parser(message, clients, i, pfds);
		// std::cout << message << std::endl;

	}
}

void Server::closeAll(std::map<int, Client> &clients, int i, std::vector<pollfd> &pfds)
{
	int clientFd = pfds[i].fd;
	
	if (close(clientFd) == -1)
		throw std::runtime_error("Error : closing client socket!");

	if (clientFd != -1)
		printMessage(CLIENT_LEFT, clientFd);

	std::map<int, Client>::iterator it = clients.find(clientFd);
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
	for (std::vector<pollfd>::iterator it = _pfd.begin() + 1; it != _pfd.end(); ++it)
	{
		if (it->fd == fd)
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
	getCommand(split, clients, clientFd, servPass, pfds);
}

void     Server::createChannel(std::string &name, int fd, std::map<int, Client> &clients)
{
	Channel chan(name, clients[fd]);
	_channList.push_back(chan);
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
	clients[fd].setNickname(incoming[1]);
}

void Server::extractUsername(std::vector<std::string> &incoming, std::map<int, Client> &clients, int fd)
{
	clients[fd].setUsername(incoming[1]);
}

void	Server::sendMessageToUser(std::vector<std::string> &message, std::map<int, Client> &clients, int fd)
{
	std::string target = message[1].substr(1, message[1].length() - 1);
	std::vector<std::string> text;
	std::map<int, Client>::iterator it = clients.begin();
	
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
	if (message[1][0] == '#')
	{
		sendMessageToChannel(message, clients, fd);
	}
	else if (message[1][0] == '@')
	{
		sendMessageToUser(message, clients, fd);
	}
	else
	{
		printInClient("Please specify target, @ for user, # for channel.", fd);
	}
}

void Server::kick(std::vector<std::string> &message, std::map<int, Client> &clients, int fd)
{
	std::string	target = message[3].substr(1, message[3].length() - 1);
	std::string channel = message[2].substr(1, message[2].length() - 1);

	for (size_t	i = 0; i < _channList.size(); i++)
	{
		if (channel == _channList[i].getName())
		{
			for (size_t j = 0; j < _channList[i].getOperators().size(); j++)
			{
				if (_channList[i].getOperators()[j].getNickname() == clients[fd].getNickname())
				{
					if (_channList[i].isMember(target) && !_channList[i].hasOperatorPrivileges(target))
					{
						_channList[i].removeMember(target);
						std::cout << target << " was kicked out of " << _channList[i].getName() << std::endl;
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
	size_t	i = 0;
	std::map<int, Client>::iterator it = clients.begin();
	
	for (i; i < _channList.size(); i++)
		if (_channList[i].getName() == channel)
			break ;
	if (i >= _channList.size())
	{
		printInClient("Couldn't invite client, check your channel's name!", fd);
		return ;
	}
	if (_channList[i].isMember(clients[fd].getNickname()) && _channList[i].hasOperatorPrivileges(clients[fd].getNickname()))
	{
		for (it; it != clients.end(); it++)
			if (it->second.getNickname() == target)
				break;
		if (it == clients.end())
		{
			printInClient("Couldn't invite client, check your client's name", fd);
			return ;
		}
		_channList[i].addMember(it->second);
		printInClient(clients[fd].getNickname() + " invited " + target + " to " + channel, fd);
		printInClient(clients[fd].getNickname() + " invited " + target + " to " + channel, it->first);
	}
}

void Server::leave(std::vector<std::string> &message, std::map<int, Client> &clients, int fd)
{
	std::string	channel;

	for (size_t	i = 0; i < message.size(); i++)
		std::cout << message[i] << " " << std::endl;
	if (message.size() != 3)
	{
		printInClient("Error, usage: /LEAVE #<channel>", fd);
		return ;
	}
	channel = message[1].substr(1, message[1].length() - 1);
	std::cout << -1 << std::endl;
	for (size_t	i = 0; i < _channList.size(); i++)
	{
		std::cout << 0 << std::endl;

		if (_channList[i].getName() == channel)
		{
			std::cout << 1 << std::endl;
			if (_channList[i].isMember(clients[fd].getNickname()))
			{
				std::cout << 2 << std::endl;
				// if (_channList[i].hasOperatorPrivileges(clients[fd].getNickname()) && (_channList[i].getMembers().size() > 1))
				// {
				// 	std::cout << 3 << std::endl;
				// 	int nextFd = fd + 1;
				// 	while (!_channList[i].isMember(clients[nextFd].getNickname()))
				// 		nextFd++;
				// 	_channList[i].setOperatorPrivileges(clients[nextFd]);
				// }
				printInClient(clients[fd].getNickname() + " left " + channel, fd);
				_channList[i].removeMember(clients[fd].getNickname());
			}
		}
	}
}

void Server::extractPassword(std::vector<std::string> &incoming, std::map<int, Client> &clients, int fd, std::string &serverPass, std::vector<pollfd> &pfds)
{
	// size_t i;

	// for (std::vector<pollfd>::iterator p_it = pfds.begin() + 1; p_it != pfds.end(); ++p_it)
	// {
	// 	if (p_it->fd == fd)
	// 	{
	// 		i = std::distance(pfds.begin(), p_it);
	// 		break;
	// 	}
	// }
	
	if (serverPass == incoming[1])
	{
		clients[fd].setAuthStatus(true);
		std::cout << "Client Authenticated" << std::endl;
	}
	else
	{
		send(fd, "Password Incorrect\n", 20, 0);
	}
}

void Server::getCommand(std::vector<std::string> &message, std::map<int, Client> &clients, int fd, std::string &pass, std::vector<pollfd> &pfds)
{
	std::string commands[] = {"JOIN", "NICK", "USER", "PASS", "PRIVMSG", "KICK", "INVITE", "LEAVE"};
	std::string channel_name;
	size_t i;
	int auth_status;
	bool channel_exists = false;

	for (i = 0; i < sizeof(commands) / sizeof(commands[0]); i++)
		if (capitalize(message[0]) == commands[i])
			break;
	
	switch (i)
	{
		case 0:
			if (clients[fd].getAuthStatus())
			{
				channel_name = message[1].substr(1, message[1].find(' ') - 1);
				for (std::vector<Channel>::iterator it = _channList.begin(); it != _channList.end(); ++it)
				{
					if (it->getName() == channel_name)
					{
						it->addMember(clients[fd]);
						displayTime();
						std::cout << clients[fd].getNickname() << " added to " << channel_name << std::endl;
						channel_exists = true;
						break;
					}
				}
				if (!channel_exists)
				{
					createChannel(channel_name, fd, clients);
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
			}
			break;

		case 2:
			if (clients[fd].getAuthStatus())
			{
				extractUsername(message, clients, fd);
			}
			break;

		case 3:
			extractPassword(message, clients, fd, pass, pfds);
			break;

		case 4:
			if (clients[fd].getAuthStatus())
			{
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

		default:
			break;
		}
}

int main(int argc, char *argv[])
{

	signal(SIGINT, SignalHandler);
	(void)argc;

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
