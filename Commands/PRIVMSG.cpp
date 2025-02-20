#include "../Headers/ft_irc.hpp"

void	send_msg(Client client, std::string msg){
	if (send(client.getSocket(), msg.c_str(), msg.length(), 0) == -1)
		cerr << ("Error sending a message");
}

void	Server::sendAll(Client &client, std::string msg){
	std::map<int, Client>::iterator it;
	
	if (msg.size() == 0)
		return ERR_NOTEXTTOSEND(client);
	for(it = _clientList.begin(); it != _clientList.end(); ++it){
		Client &client_target = it->second;
		if (client_target.getNickname() != client.getNickname()){
			std::string message = ":" + client.getNickname() + " PRIVMSG " + client_target.getNickname() + " :" + msg + "\r\n";
			send_msg(client_target, message);
		}
	}
}

void	Command::PRIVMSG(Client &client){
	if (this->params.size() < 1)
		return ERR_NEEDMOREPARAMS(client, "PRIVMSG");
	if (this->params[0][0] == '$') //if target name name start with '$' the msg must be send to all clients of the server
		return _server.sendAll(client, msg);
	int	restriction = 0;
	if (this->params[0][0] == '@'){
		restriction = 1;
		this->params[0] = this->params[0].substr(1);
	}
	if (!_server.isClientLog(this->params[0]) && !_server.channelExisting(this->params[0])) //if it's not a client or a channel
		return ERR_NOSUCHNICK(client, this->params[0], "nick/channel");
	if (!_server.isClientLog(this->params[0])){ //if it's a channel
		if (msg.size() == 0)
			return ERR_NOTEXTTOSEND(client);
		Channel &tmp = _server.getChannel(this->params[0]);
		tmp.SendMessageToChannel(this->msg, this->params[0], client, restriction);
		return;
	}
	if (msg.size() == 0)
		return ERR_NOTEXTTOSEND(client);
	Client &client_target = _server.getClient(this->params[0]);
	std::string message = ":" + client.getNickname() + " PRIVMSG " + client_target.getNickname() + " :" + msg + "\r\n";
	send_msg(client_target, message);
}
