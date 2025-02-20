/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mfinette <mfinette@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/13 17:09:36 by mfinette          #+#    #+#             */
/*   Updated: 2024/02/26 12:52:03 by mfinette         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Headers/ft_irc.hpp"

int main(int argc, char **argv)
{
	if (argc != 3){
		std::cout << MAGENTA << "Usage: ./ircserv <port> <password>" << RESET << std::endl;
		return 0;
	}
	Server server(atoi(argv[1]), argv[2]);
	try
	{
		signal(SIGINT, signalHandler);
		server.start();
	}
	catch (const CtrlCException &e)
	{
		std::cout << MAGENTA << "\nCtrl + C detected, closing server" << RESET << std::endl;
		server.closeServer();
		return (1);
	}
	catch (const std::exception &e)
	{
		std::cerr << RED << "Error: " << e.what() << RESET << std::endl;
		server.closeServer();
		return 1;
	}
	catch (...)
	{
		std::cerr << RED << "Unknown error but it'll take more to crash us hihi" << RESET << std::endl;
		server.closeServer();
		return 1;
	}
	return 0;
}
