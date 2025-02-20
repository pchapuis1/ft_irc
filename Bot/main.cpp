/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mfinette <mfinette@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/07 14:31:35 by mfinette          #+#    #+#             */
/*   Updated: 2024/03/21 19:03:18 by mfinette         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Bot.hpp"

void signalHandler(int signum) {
	if (signum == SIGINT)
		throw CtrlCException();
}

int startBot(int port, char **argv, int client_socket)
{

	if (!connectToServer(client_socket, port))
		return 1;
	std::cout << "Connected to server. Type 'quit' to exit." << std::endl;
	connectBot(client_socket, argv);
	struct pollfd fds[2];
	fds[0].fd = STDIN_FILENO;
	fds[0].events = POLLIN;
	fds[1].fd = client_socket;
	fds[1].events = POLLIN;
	while (true)
	{
		int ret = poll(fds, 2, -1);
		if (ret == -1)
		{
			std::cerr << "Error in poll" << std::endl;
			break;
		}
		if (fds[0].revents & POLLIN)
		{
			std::cout << "bot > ";
			handleUserInput(client_socket);
		}
		if (fds[1].revents & POLLIN)
		{
			char buffer[BUFFER_SIZE];
			ssize_t bytes_received;
			bytes_received = recv(client_socket, buffer, BUFFER_SIZE, 0);
			if (bytes_received <= 0)
			{
				if (bytes_received == 0)
					std::cerr << "Disconnected from server" << std::endl;
				else
					std::cerr << "Error in recv: " << strerror(errno) << std::endl;
				close(client_socket);
				return 1;
			}
			buffer[bytes_received] = '\0';
			std::cout << YELLOW << buffer << RESET;
			Cmd cmd(buffer);
			sendResponse(client_socket, cmd);
			std::cout << RESET;
		}
			// receiveAndPrintMessage(client_socket);
	}
	close(client_socket);
	return 0;
}

int main(int argc, char *argv[])
{
	if (argc != 3)
	{
		std::cerr << RED <<"Usage: " << argv[0] << " <port_number> <server_password>" << std::endl << RESET;
		return 1;
	}
	int port = atoi(argv[1]);
	if (port <= 0 || port > 65535)
	{
		std::cerr << "Invalid port number" << std::endl;
		return 1;
	}
	int client_socket = createSocket();
	if (client_socket == -1)
		return 1;
	try
	{
		signal(SIGINT, signalHandler);
		startBot(port, argv, client_socket);
	}
	catch(const CtrlCException &e)
	{
		close(client_socket);
		std::cerr << RED << "\nCTRL + C detected: shutting down bot !" << RESET;
	}
	return 0;
}
