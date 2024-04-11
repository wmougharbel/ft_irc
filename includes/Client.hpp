#ifndef CLIENT_HPP
# define CLIENT_HPP

# include "irc.hpp"

class Client
{
	private:
		bool		_isOperator;
		bool		_isAuthenticated;
		std::string	_nickname;
		std::string	_username;
		Client();

	public:
		Client (std::string nickname, std::string username);
		Client (const Client &);
		Client	&operator=(const Client &);
		~Client();

		//getters
		bool		getOperator();
		bool		getAuthStatus();
		std::string	getNickname() const;
		std::string	getUsername()const;

		//setters
		void	setOperator(bool isOperator);
		void	setAuthStatus(bool status);
		void	setNickname(std::string nickname);
		void	setUsername(std::string username);

		//functions
		void	promote();
		void	authenticate();
		void	kick(const Client &toKick);
};

#endif