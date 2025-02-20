#include "../Headers/ft_irc.hpp"

void	Command::KICK(Client &client){
	if (params.size() < 2)
		return ERR_NEEDMOREPARAMS(client, "KICK");
	std::vector<string> client_list = splitWithComa(params[1]);
	std::vector<string> comment_list = splitWithComa(msg);
	if (!_server.channelExisting(this->params[0]))// if channel doesn't exist
		return ERR_NOSUCHCHANNEL(client, this->params[0]);
	Channel &channel = _server.getChannel(this->params[0]);
	for (unsigned int i = 0; i < client_list.size(); i++){
		if (!channel.isOperator(client.getSocket())){ //if not right to kick
			ERR_CHANOPRIVSNEEDED(client, channel.getName());
			continue;
		}
		if (!_server.isClientLog(client_list[i])){ //if the target isn't on the server
			ERR_NOSUCHNICK(client, client_list[i], "nick");
			continue;
		}
		if (!channel.isClientInChannel(_server.getClient(client_list[i]).getSocket())){ //if the target is on the channel
			ERR_USERNOTINCHANNEL(client, client_list[i], channel.getName());
			continue;
		}
		if (!channel.isClientInChannel(client.getSocket())){ //if the client who made the request is not on the channel
			ERR_NOTONCHANNEL(client, channel.getName());
			continue;
		}
		//all conditions are met
		Client &client_target = _server.getClient(client_list[i]);
		std::string msg = ":" + client.getNickname() + " KICK " + channel.getName() + " " + client_target.getNickname() + " ";
		if (comment_list.size() > i)
			msg += comment_list[i];
		msg += "\r\n";
		send_msg(client_target, msg);
		channel.RemoveClientFromChannel(&client_target);
		_server.updateChannel(channel);
	}
}
