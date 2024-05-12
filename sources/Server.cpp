/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: marvin <marvin@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/22 18:45:30 by walid             #+#    #+#             */
/*   Updated: 2024/05/12 23:30:46 by marvin           ###   ########.fr       */
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
		std::cout << "EXISTING CLIENT I: " << i << std::endl;
		closeAll(clients, i, pfds);
		return;
	}
	else
	{
		// APPROACH 1
		// char *end = strstr(tempBuf, "\r\n");
		// buf.append(tempBuf, end - tempBuf);
		// std::cout << buf << std::endl;
		// parser(buf, clients, i, pfds);
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
		std::cout << "HERE" << std::endl;

	std::cout << "SIZE: " << pfds.size() << std::endl;
	std::cout << "I: " << i << std::endl;
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
			clients[fd].sendMessage(text, it->first);
		it++;
	}
}

void	Server::sendMessageToChannel(std::vector<std::string> &message, std::map<int, Client> &clients, int fd)
{
	std::string target = message[1].substr(1, message[1].length() - 1);
	std::vector<std::string> text;
	
	std::cout << target << std::endl;
	for (size_t i = 1; i < message.size(); i++)
		text.push_back(message[i]);
	for (size_t i = 0; i < _channList.size(); i++)
	{
		if (_channList[i].getName() == target)
		{
			for (size_t j = 0; j < _channList[i].getMembers().size(); j++)
			{
				clients[fd].sendMessage(text, _channList[i].getMembers()[j].getFd());
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
}


void Server::extractPassword(std::vector<std::string> &incoming, std::map<int, Client> &clients, int fd, std::string &serverPass, std::vector<pollfd> &pfds)
{
	if (serverPass == incoming[1])
	{
		clients[fd].setAuthStatus(true);
	}
	else
	{
		std::map<int, Client>::iterator it = clients.find(fd);
		if (it != clients.end())
			clients.erase(it);
		pfds.erase(pfds.begin() + 1);
		std::cout << "Client at : " << fd << " couldn't authenticate";
	}
}



void Server::getCommand(std::vector<std::string> &message, std::map<int, Client> &clients, int fd, std::string &pass, std::vector<pollfd> &pfds)
{
	std::string commands[] = {"JOIN", "NICK", "USER", "PASS", "PRIVMSG"};
	std::string channel_name;
	size_t i;
	int auth_status;
	bool channel_exists = false;

	for (i = 0; i < sizeof(commands) / sizeof(commands[0]); i++)
		if (message[0] == commands[i])
			break;
	
	switch (i)
	{
		case 0:
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
			break;
		
		case 1:
			extractNickname(message, clients, fd);
			displayTime();
			if (clients[fd].getAuthStatus())
				std::cout << clients[fd].getNickname() << CLIENT_JOINED << std::endl;
			break;

		case 2:
			extractUsername(message, clients, fd);
			break;

		case 3:
			extractPassword(message, clients, fd, pass, pfds);
			break;

		case 4:
			std::cout << "MESSAGE DETECTED" << std::endl;
			privMsg(message, clients, fd);
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
