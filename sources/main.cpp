/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: walid <walid@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/22 18:45:18 by walid             #+#    #+#             */
/*   Updated: 2024/04/22 18:45:18 by walid            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/Irc.hpp"

int main(int argc, char **argv)
{
	try
	{
		if (argc != 3)
			throw(std::runtime_error("Incorrect number of arguments!\nUsage: ./Ircserv <port> <password>\ne.g Port = 6667\te.g Password = P@$$w0rd"));
		validateInput(argv[1], argv[2]);
		Client c1("cheesus", "walid");
		// MockServer ms;
		// ms.setPassword(argv[2]);
		c1.setServerPassword(argv[2]);
		std::string clientPassword;
		std::cout << "Enter client password: ";
		getline(std::cin, clientPassword);
		c1.setClientPassword(clientPassword);
		std::cout << "Client password: " << c1.getClientPassword() << std::endl;
		c1.authenticate();
	}
	catch (const std::exception &e)
	{
		std::cerr << "Exception caught: " << e.what() << std::endl;
	}
}