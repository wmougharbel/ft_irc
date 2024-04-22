/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Client.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: walid <walid@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/22 18:44:43 by walid             #+#    #+#             */
/*   Updated: 2024/04/22 18:44:43 by walid            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef CLIENT_HPP
# define CLIENT_HPP

# include "Irc.hpp"

// class MockServer;

class Client
{
	private:
		int			_fd;
		bool		_isOperator;
		bool		_isAuthenticated;
		std::string	_nickname;
		std::string	_username;
		std::string	_clientPassword;
		//Channel		_channel;
		Client();

	public:
		Client(int fd);
		// Client (std::string nickname, std::string username);
		// Client (const Client &);
		// Client	&operator=(const Client &);
		~Client();

		//vaiables

		//getters
		bool		getOperator() const;
		bool		getAuthStatus() const;
		std::string	getNickname() const;
		std::string	getUsername()const;
		std::string	getClientPassword() const;
		int getFd() const;

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
