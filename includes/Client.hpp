#ifndef CLIENT_HPP
# define CLIENT_HPP

# include "irc.hpp"
# include "MockServer.hpp"

// class MockServer;

class Client
{
	private:
		bool		_isOperator;
		bool		_isAuthenticated;
		std::string	_nickname;
		std::string	_username;
		std::string	_clientPassword;
		MockServer	_server;
		Client();

	public:
		Client (std::string nickname, std::string username);
		Client (const Client &);
		Client	&operator=(const Client &);
		~Client();

		//vaiables

		//getters
		bool		getOperator() const;
		bool		getAuthStatus() const;
		std::string	getNickname() const;
		std::string	getUsername()const;
		std::string	getClientPassword() const;

		//setters
		void	setOperator(bool isOperator);
		void	setAuthStatus(bool status);
		void	setNickname(std::string nickname);
		void	setUsername(std::string username);
		void	setClientPassword(std::string clientPassword);
		void	setServerPassword(std::string password);

		//functions
		void	promote();
		void	authenticate();
		void	kick(const Client &toKick);
};

#endif