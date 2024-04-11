#ifndef CLIENT_HPP
# define CLIENT_HPP

# include "irc.hpp"

class Client
{
	private:
		bool		_isOperator;
		std::string	_nickname;
		std::string	_username;
		Client();

	public:
		Client (bool isOperator, std::string nickname, std::string username);
		Client (const Client &);
		Client	&operator=(const Client &);
		~Client();

		//getters
		bool		getOperator();
		std::string	getNickname();
		std::string	getUsername();

		//setters
		void	setOperator(bool isOperator);
		void	setNickname(std::string nickname);
		void	setUsername(std::string username);
};

#endif