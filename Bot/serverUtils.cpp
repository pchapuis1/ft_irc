/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   serverUtils.cpp                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: maxime <maxime@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/08 17:11:55 by mfinette          #+#    #+#             */
/*   Updated: 2024/03/22 11:19:32 by maxime           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Bot.hpp"

const char* RN = "\r\n";

int createSocket(void)
{
	int client_socket = socket(AF_INET, SOCK_STREAM, 0);
	if (client_socket == -1)
	{
		std::cerr << "Error: Failed to create socket" << std::endl;
		return -1;
	}
	return client_socket;
}

bool connectToServer(int client_socket, int port)
{
	sockaddr_in server_addr;
	memset(&server_addr, 0, sizeof(server_addr));
	server_addr.sin_family = AF_INET;
	server_addr.sin_port = htons(port);
	server_addr.sin_addr.s_addr = inet_addr("127.0.0.1");

	if (connect(client_socket, reinterpret_cast<sockaddr*>(&server_addr), sizeof(server_addr)) == -1 &&
			errno != EINPROGRESS)
	{
		std::cerr << "Error: Failed to connect to server" << std::endl;
		close(client_socket);
		return false;
	}
	return true;
}

void sendMessage(int client_socket, const std::string& message)
{
	std::string tmp = message + std::string("\r\n");
	send(client_socket, tmp.c_str(), tmp.length(), MSG_DONTWAIT + MSG_NOSIGNAL);
}

void sendMultipleLineMessage(int client_socket, const std::string& target, const std::string& message)
{
	std::istringstream iss(message);
	std::string line;
	while (std::getline(iss, line, '\n'))
	{
		std::string tmp = "PRIVMSG " + target + " :" + line + RN;
		sendMessage(client_socket, tmp);
		usleep(1000);
	}
}

void sendMultipleLineMessageChatbot(int client_socket, const std::string& target, const std::string& message)
{
	std::string processed_message = message;
	size_t pos = processed_message.find("\\n");
	while (pos != std::string::npos) {
		processed_message.replace(pos, 2, "\n");
		pos = processed_message.find("\\n", pos + 1);
	}

	std::istringstream iss(processed_message);
	std::string line;
	while (std::getline(iss, line, '\n')) {
		std::string tmp = "PRIVMSG " + target + " :" + line + "\r\n"; // Use "\r\n" instead of RN
		sendMessage(client_socket, tmp);
		usleep(1000);
	}
}

void receiveAndPrintMessage(int client_socket)
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
        return;
    }
    buffer[bytes_received] = '\0';
    std::cout << YELLOW << "" << buffer << RESET;
    Cmd cmd(buffer);
    sendResponse(client_socket, cmd);
    std::cout << RESET;
}

void handleUserInput(int client_socket)
{
	std::string message;
	std::getline(std::cin, message);
	if (message == "quit")
		return;
	std::string formattedMessage = message + RN;
	sendMessage(client_socket, formattedMessage);
}

void connectBot(int client_socket, char **argv)
{
	sendMessage(client_socket, "PASS " + std::string(argv[2]) + RN);
	usleep(1000);
	sendMessage(client_socket, "NICK __bot__" + std::string(RN));
	usleep(1000);
	sendMessage(client_socket, "USER bot 0 * :realname" + std::string(RN));
}
