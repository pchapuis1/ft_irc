/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   clientChannelManagement.cpp                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pchapuis <pchapuis@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/21 18:05:45 by mfinette          #+#    #+#             */
/*   Updated: 2024/03/21 14:34:02 by pchapuis         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Headers/ft_irc.hpp"

void	Server::setupClient(int socket)
{
	Client newClient(socket);
	newClient.setNickname("");
	newClient.setUsername("");
	newClient.setRealname("");
	newClient.setSocketState(true);
	this->addClientToServer(newClient);
}

void	Server::addClientToServer(const Client& client)
{
	this->_clientList.insert(std::pair<int, Client>(client.getSocket(), client));
}

void	Server::removeClientFromServer(Client& client)
{
	_clientList.erase(_clientList.find(client.getSocket()));
}

void	Server::addChannelToServer(Channel& channel)
{
	_channelList.insert(std::pair<std::string, Channel>(channel.getName(), channel));
}

void	Server::removeChannelFromServer(Channel& channel)
{
	std::map<std::string, Channel>::iterator it = _channelList.find(channel.getName());
    if (it != _channelList.end())
        _channelList.erase(it);
}
