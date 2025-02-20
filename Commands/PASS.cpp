#include "../Headers/ft_irc.hpp"

bool	Command::PASS(Client &client)
{
	if (params.size() == 0)
		ERR_NEEDMOREPARAMS(client, cmdName);
	else if (params[0] == _server.getServPassword())
		return true;
	else 
		ERR_PASSWDMISSMATCH(client, "unknow");
	return false;
}