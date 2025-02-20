#include "../Headers/ft_irc.hpp"

void	Command::PART(Client &client){
	if (params.size() < 1)
		return ERR_NEEDMOREPARAMS(client, "PART");
	std::vector<string> channel_list = splitWithComa(params[0]);
	std::vector<string> reason_list = splitWithComa(msg);
	for (unsigned int i = 0; i < channel_list.size(); i++){
		if (!_server.channelExisting(channel_list[i]))
			return ERR_NOSUCHCHANNEL(client, channel_list[i]);
		Channel &channel = _server.getChannel(channel_list[i]);
		if (!channel.isClientInChannel(client.getSocket()))
			return ERR_NOTONCHANNEL(client, channel_list[i]);
		std::string msg = ":" + client.getNickname() + " PART " + channel.getName() + " ";
		if (reason_list.size() > i)
			msg += reason_list[i];
		msg += "\r\n";
		send_msg(client, msg);
		channel.RemoveClientFromChannel(&client);
		_server.updateChannel(channel);
	}
}