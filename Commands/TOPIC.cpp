#include "../Headers/ft_irc.hpp"

void	Channel::updateEveryClient(){
	std::map<Client *, bool>::iterator it;

	for(it = _client.begin(); it != _client.end(); ++it){
		Client client = *it->first;
		RPL_TOPIC(client, _name, _topic);
		RPL_TOPICWHOTIME(client, _name, _topic_author, getSetAt());
	}
}

void	Command::TOPIC(Client &client){
	if (this->params.size() == 0) //check if a name as been given as parameters
		return ERR_NEEDMOREPARAMS(client, "TOPIC");
	if (!_server.channelExisting(this->params[0])) //if the channel doesn't exist
		return ERR_NOSUCHCHANNEL(client, this->params[0]);
	Channel &channel = _server.getChannel(this->params[0]);
	if (!channel.isClientInChannel(client.getSocket())) //if the client isn't on the channel
		return ERR_NOTONCHANNEL(client, this->params[0]);
	if (msg.size() != 0){ //if there is parameters then modifications of the topic
		if (channel.hasTopicRestriction() && !channel.isOperator(client.getSocket())) //check if the topic can be modify
			return ERR_CHANOPRIVSNEEDED(client, this->params[0]);
		channel.setTopic(this->msg);
		channel.setTopicAuthor(client.getNickname());
		channel.setSetAt();
		channel.updateEveryClient();
	}
	else{ //if no parameters then print it
		if (channel.getTopic().size() == 0)
			return RPL_NOTOPIC(client, this->params[0]);
		RPL_TOPIC(client, this->params[0], channel.getTopic());
		RPL_TOPICWHOTIME(client, this->params[0], channel.getTopicAuthor(), channel.getSetAt());			
	}
}