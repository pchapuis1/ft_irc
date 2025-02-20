#include "../Headers/ft_irc.hpp"

void Command::oMode(Client &client, Channel &channel, string param, char sign) {
	if (!isExistingNick(_server, param))
		return ERR_NOSUCHNICK(client, param, "nick");
	int targetClient = _server.getClient(param).getSocket();
	if (client.getSocket() == targetClient)
		return;
	if (sign == '-' && channel.isOperator(targetClient)) {
		channel.changeOperatorStatusToOff(targetClient);
		std::string modestring = "-o " + param;
		MODE_MESSAGE(client, channel.getName(), modestring);
	}
	else if (_server.getClient(param).getNickname() != "bot" && sign == '+' && !channel.isOperator(targetClient)){
		channel.changeOperatorStatusToOn(targetClient);
		std::string modestring = "+o " + param;
		MODE_MESSAGE(client, channel.getName(), modestring);
	}
	else if (sign == '\0' && channel.isOperator(_server.getClient(param).getSocket())){
		channel.changeOperatorStatusToOff(targetClient);
		std::string modestring = "-o " + param;
		MODE_MESSAGE(client, channel.getName(), modestring);
	}
	else if (sign == '\0' && _server.getClient(param).getNickname() != "bot"){
		channel.changeOperatorStatusToOn(targetClient);
		std::string modestring = "+o " + param;
		MODE_MESSAGE(client, channel.getName(), modestring);
	}
	else return;
	channel.updateClientList();
}

void Command::iMode(Channel &channel, char sign, Client &client) {
	if (sign == '-'){
		if (!channel.getInviteStatus())
			return ;
		channel.changeInviteOnlyStatusToOff();
		MODE_MESSAGE(client, channel.getName(), "-i");
	}
	else if (sign == '+'){
		if (channel.getInviteStatus())
			return ;
		channel.changeInviteOnlyStatusToOn();
		MODE_MESSAGE(client, channel.getName(), "+i");
	}
	else if (channel.getInviteStatus()){
		channel.changeInviteOnlyStatusToOff();
		MODE_MESSAGE(client, channel.getName(), "-i");
	}
	else{
		channel.changeInviteOnlyStatusToOn();
		MODE_MESSAGE(client, channel.getName(), "+i");
	}
}

void Command::tMode(Channel &channel, char sign, Client &client) {

	if (sign == '-'){
		if (!channel.hasTopicRestriction())
			return ;
		channel.changeTopicRestrictionToOff();
		MODE_MESSAGE(client, channel.getName(), "-t");
	}
	else if (sign == '+'){
		if (channel.hasTopicRestriction())
			return ;
		channel.changeTopicRestrictionToOn();
		MODE_MESSAGE(client, channel.getName(), "+t");
	}
	else if (channel.hasTopicRestriction()){
		channel.changeTopicRestrictionToOff();
		MODE_MESSAGE(client, channel.getName(), "-t");
	}
	else{
		channel.changeTopicRestrictionToOn();
		MODE_MESSAGE(client, channel.getName(), "+t");
	}
}

void Command::kMode(Channel &channel, string param, char sign, Client &client) {
	if (sign == '-' && channel.hasPassword() == true) {
		channel.setHasPasswordToFalse();
		MODE_MESSAGE(client, channel.getName(), "-k");
	}
	else if (sign == '+' && param != "") {
		channel.setHasPasswordToTrue();
		channel.setPassword(param);
		std::string modestring = "+k " + channel.getPassword();
		MODE_MESSAGE(client, channel.getName(), modestring);
	}
	else if (sign != '+' && channel.hasPassword() && param == "") {
		channel.setHasPasswordToFalse();
		MODE_MESSAGE(client, channel.getName(), "-k");
	}
	else if (sign != '-' && !channel.hasPassword() && param != "") {
		channel.setHasPasswordToTrue();
		channel.setPassword(param);
		std::string modestring = "+k " + channel.getPassword();
		MODE_MESSAGE(client, channel.getName(), modestring);
	}

}

void Command::lMode(Channel &channel, string param, char sign, Client &client) {
	(void) channel;
	
	int nbr = atoi(param.c_str());
	double lNbr = atof(param.c_str());
	if (nbr != lNbr)
		return ;
	if (sign == '-' && channel.getUserLimit() != -1){ //if -l and user limit set
		channel.changeUserLimit(-1);
		MODE_MESSAGE(client, channel.getName(), "-l");
	}
	else if (sign == '+' && param.find_first_not_of("0123456789") == std::string::npos && channel.getUserLimit() != nbr){ //if +l and param numbers only and user limit not already to the same limit
		if (nbr < channel.nbClient())
			return ;
		channel.changeUserLimit(nbr);
		std::string modestring = "+l ";
		std::ostringstream oss;
   		oss << channel.getUserLimit();
		modestring += oss.str();
		MODE_MESSAGE(client, channel.getName(), modestring);
	}
	else if (sign == '\0' && channel.getUserLimit() != -1 && param == "") { //no sign and limit define and no param then -1
		channel.changeUserLimit(-1);
		MODE_MESSAGE(client, channel.getName(), "-l");
	}
	else if (sign == '\0' && param != "" && param.find_first_not_of("0123456789") == std::string::npos) {//no sign and no user limit and valid parameter
		if (channel.getUserLimit() == nbr)
			return;
		if (nbr < channel.nbClient())
			return ;
		channel.changeUserLimit(nbr);
		std::string modestring = "+l ";
		std::ostringstream oss;
   		oss << channel.getUserLimit();
		modestring += oss.str();
		MODE_MESSAGE(client, channel.getName(), modestring);
	}
}

std::string	Server::getModestring(std::string channelName){
	Channel &channel = getChannel(channelName);

	std::string modestring = "+";
	if (channel.hasPassword())
		modestring += "k";
	if (channel.isInviteOnly())
		modestring += "i";
	if (channel.getUserLimit() != -1)
		modestring += "l";
	if (channel.hasTopicRestriction())
		modestring += "t";
	if (modestring.size() == 1)
		return ("");
	return modestring;
}

void	Command::MODE(Client &client) {
	string modeStr = "";
	char sign = '\0';
	size_t j = 2;
	string parameter;

	if (params.size() <= 0)
		return ERR_NEEDMOREPARAMS(client, "MODE");
	if (!_server.channelExisting(params[0]))
		return ERR_NOSUCHCHANNEL(client, params[0]);
	if (params.size() <= 1)
		return RPL_CHANNELMODEIS(client, params[0], _server.getModestring(params[0]));
	if (params.size() >= 2)
		modeStr = params[1];
	Channel &channel = _server.getChannel(params[0]);
	if (!channel.isOperator(client.getSocket()))
		return ERR_CHANOPRIVSNEEDED(client, params[0]);
	if (modeStr == "")
		return RPL_CHANNELMODEIS(client, params[0], _server.getModestring(params[0]));
	parameter = "";
	for (size_t i = 0; i < modeStr.size(); i++) {
		if (j < params.size() && (modeStr[i] == 'o' || modeStr[i] == 'k' || modeStr[i] == 'l'))
			parameter = params[j++];
		if (modeStr[i] == '+' || modeStr[i] == '-') {
			sign = modeStr[i];
			if (modeStr[i + 1] && !isalpha(modeStr[i + 1])) {
				cout << "Error: character after sign should be a letter." << endl;
				break;
			}
			continue;
		}
		if (modeStr[i] == 'o')
			oMode(client, channel, parameter, sign);
		else if (modeStr[i] == 'i')
			iMode(channel, sign, client);
		else if (modeStr[i] == 't')
			tMode(channel, sign, client);
		else if (modeStr[i] == 'k')
			kMode(channel, parameter, sign, client);
		else if (modeStr[i] == 'l')
			lMode(channel, parameter, sign, client);
		else if (isalpha(modeStr[i]))
			ERR_UMODEUNKNOWNFLAG(client);
	}
}