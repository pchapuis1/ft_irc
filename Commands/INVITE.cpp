#include "../Headers/ft_irc.hpp"

void	Command::INVITE(Client &client){
	if (params.size() <= 1)
		return ERR_NEEDMOREPARAMS(client, "INVITE");
	if (!_server.channelExisting(this->params[1]))// if channel doesn't exist
		return ERR_NOSUCHCHANNEL(client, this->params[1]);
	Channel &channel = _server.getChannel(this->params[1]);
	if (!channel.isClientInChannel(client.getSocket())) //if the client who made the request is not on the channel
		return ERR_NOTONCHANNEL(client, channel.getName());
	if (channel.isInviteOnly() && !channel.isOperator(client.getSocket())) //if invite only is On and no right to kick
		return ERR_CHANOPRIVSNEEDED(client, channel.getName());
	if (!_server.isClientLog(this->params[0]))
		return ERR_NOSUCHNICK(client, this->params[0], "nick");
	Client &client_target = _server.getClient(this->params[0]);
	if (channel.isClientInChannel(client_target.getSocket())) //if the target to invite is already in channel
		return ERR_USERONCHANNEL(client, client_target.getNickname(), channel.getName());
	//all conditions are good
	RPL_INVITING(client, client_target.getNickname(), channel.getName()); //send to notify the client that the request as succeed
	std::string msg = ":" + client.getNickname() + " INVITE " + client_target.getNickname() + " :" + channel.getName() + "\r\n";
	send_msg(client_target, msg);
	JoinServeur(client_target, channel);
}
