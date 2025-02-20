#include "../Headers/ft_irc.hpp"

void	Command::QUIT(Client &client)
{	
	_server.leaveAll(client);
	QUIT_SERVER(client, msg);
	_server.removeFd(client.getSocket());
	_server.closeClientSocket(client);
	client.setSocketState(false);
	_server.removeClientFromServer(client);
}