/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   JOIN.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pchapuis <pchapuis@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/22 21:58:20 by maxime            #+#    #+#             */
/*   Updated: 2024/03/21 15:54:59 by pchapuis         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../Headers/ft_irc.hpp"

void	Channel::updateClientList(){
	std::map<Client *, bool>::iterator it_client;
	
	for(it_client = _client.begin(); it_client != _client.end(); ++it_client){
		Client client_to = *it_client->first;
		std::map<Client *, bool>::iterator it;
		std::string	server_name = "IRC 42";
		std::string msg = HEADER(client_to) + " 353 " + client_to.getNickname() + " = " + getName() + " :";
		for(it = _client.begin(); it != _client.end(); ++it){
			if (it->second)
				msg += "@";
			msg += it->first->getNickname();
			msg += " ";
		}
		msg.erase(msg.size() - 1);
		msg += "\r\n";
		send_msg(client_to, msg);
		RPL_ENDOFNAMES(client_to, getName());
	}
}

void	Command::JoinServeur(Client& client, Channel& channel){
	JOIN_CHANNEL(client, channel.getName());
	channel.AddClientToChannel(&client);
	if (channel.getTopic().size() != 0){ //if there is a topic
		RPL_TOPIC(client, channel.getName(), channel.getTopic());
		RPL_TOPICWHOTIME(client, channel.getName(), channel.getTopicAuthor(), channel.getSetAt());
	}
	else
		RPL_NOTOPIC(client, channel.getName());
	channel.updateClientList();
}

void	Server::leaveAll(Client &client){
	std::map<std::string, Channel>::iterator it;
	
	for(it = _channelList.begin(); it != _channelList.end(); it++){
		Channel &channel = it->second;
		if (channel.isClientInChannel(client.getSocket())){
			std::string msg = ":" + client.getNickname() + " PART " + channel.getName() + " \r\n";
			send_msg(client, msg);
			channel.RemoveClientFromChannel(&client);
			if (channel.nbClient() == 0){
				removeChannelFromServer(channel);
				it = _channelList.begin();
				if (it == _channelList.end())
					return;
				continue;	
			}
			else if (channel.nbOperator() == 0)
				channel.setNewOperator();
			channel.updateClientList();
		}
	}
}

bool	checkChannelName(std::string name){
	if (name[0] != '#')
		return false;
	for (std::string::size_type i = 0; i < name.length(); ++i)
        if (name[i] == 7) //Ascii code of ^G
			return false;
	return true;
}

void	Command::JOIN(Client &client){
	if (params.size() < 1)
		return ERR_NEEDMOREPARAMS(client, "JOIN");
	std::vector<string> channel_list = splitWithComa(params[0]);
	std::vector<string> key_list;
	if (params.size() > 1)
		key_list = splitWithComa(params[1]);
	for (unsigned int i = 0; i < channel_list.size(); i++){
		if (channel_list[0] == "0") //leave all channels
			_server.leaveAll(client);
		else if (_server.channelExisting(channel_list[i])){ //if channel existing
			Channel& channel = _server.getChannel(channel_list[i]);
			if (client.getNickname() != "bot" && channel.isInviteOnly()){ //if invite only is on
				ERR_INVITEONLYCHAN(client, channel.getName());
				continue;
			}
			if (client.getNickname() != "bot" && channel.getUserLimit() != -1 && channel.getUserLimit() <= channel.nbClient()){ //check if not over the user limit
				ERR_CHANNELISFULL(client, channel.getName());
				continue;
			}
			if (client.getNickname() != "bot" && channel.hasPassword()){
				if	(key_list.size() <= i || channel.getPassword() != key_list[i]){ //check if the password is valid
					ERR_BADCHANNELKEY(client, channel.getName());			
					continue;
				}
			}
			JoinServeur(client, channel);
		}
		else if (client.getNickname() != "bot")
		{
			if (!checkChannelName(channel_list[i]))
				return;
			Channel new_channel(channel_list[i]);
			new_channel.AddClientToChannel(&client);
			new_channel.changeOperatorStatusToOn(client.getSocket());
			_server.addChannelToServer(new_channel);
			JOIN_CHANNEL(client, new_channel.getName());
			new_channel.updateClientList();
			RPL_NOTOPIC(client, new_channel.getName());
		}
	}
}
