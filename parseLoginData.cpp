#include "Headers/ft_irc.hpp"

void getLoginData(string input, Client &client, Server &server) {
	std::istringstream iss(input);
	while (std::getline(iss, input))
	{
		Command cmd(input, server);
		if (cmd.getCmdName() == "CAP")
			continue;
		if (cmd.getCmdName() == "QUIT")
			cmd.QUIT(client);
		if (cmd.getCmdName() != "PASS" && cmd.getCmdName() != "NICK" && cmd.getCmdName() != "USER" && cmd.getCmdName() != "")
			ERR_NOTREGISTERED(client, "channel name");
		if (client.getLoginStage() == STAGE_1) {
			if (cmd.getCmdName() == "PASS")
				if (cmd.PASS(client))
					client.setLoginStage(STAGE_2);
		}
		else {
			if (cmd.getCmdName() == "NICK") {
				if (cmd.NICK(client) && client.getLoginStage() != NICK_ENTERED) {
					if (client.getLoginStage() == STAGE_2)
						client.setLoginStage(NICK_ENTERED);
					else if (client.getLoginStage() == USER_ENTERED) {
						client.setLoginStage(ALL_LOGIN_DATA_ENTERED);
						cout << COLOR_1 << "\nUSER ("  << COLOR_2 << client.getSocket()  << COLOR_1 << ") SUCCESSFULLY LOGGED AS : " << COLOR_2 << client.getNickname() << COLOR_1 << " !" << RESET << endl;
					}
				}
			}
			if (cmd.getCmdName() == "USER" && client.getLoginStage() != USER_ENTERED) {
				if (cmd.USER(client)) {
					if (client.getLoginStage() == STAGE_2)
						client.setLoginStage(USER_ENTERED);
					else if (client.getLoginStage() == NICK_ENTERED) {
						client.setLoginStage(ALL_LOGIN_DATA_ENTERED);
						cout << COLOR_1 << "\nUSER ("  << COLOR_2 << client.getSocket()  << COLOR_1 << ") SUCCESSFULLY LOGGED AS : " << COLOR_2 << client.getNickname() << COLOR_1 << " !" << RESET << endl;
					}
				}
			} 
		}
		// cout << client.getLoginStage() << endl;
	}
}

void execCMD(string input, Client &client, Server &server){
	Command cmd(input, server);
	if (cmd.getCmdName() == "QUIT")
		cmd.QUIT(client);
	if (cmd.getCmdName() == "NICK")
		cmd.NICK(client);
	if (cmd.getCmdName() == "JOIN")
		cmd.JOIN(client);
	if (cmd.getCmdName() == "INVITE")
		cmd.INVITE(client);
	if (cmd.getCmdName() == "PRIVMSG")
		cmd.PRIVMSG(client);
	if (cmd.getCmdName() == "KICK")
		cmd.KICK(client);
	if (cmd.getCmdName() == "TOPIC")
		cmd.TOPIC(client);
	if (cmd.getCmdName() == "PART")
		cmd.PART(client);
	if (cmd.getCmdName() == "MODE")
	 	cmd.MODE(client);
	if (cmd.getCmdName() == "PASS")
		ERR_ALREADYREGISTERED(client);
	if (cmd.getCmdName() == "USER")
		ERR_ALREADYREGISTERED(client);
}
