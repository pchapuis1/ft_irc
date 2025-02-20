#ifndef COMMAND_HPP
#define COMMAND_HPP

#include "ft_irc.hpp"
#include "server.hpp"

#define USERLEN 15

#define STAGE_1 0
#define STAGE_2 1
#define NICK_ENTERED 2
#define USER_ENTERED 3
#define ALL_LOGIN_DATA_ENTERED 4

class Command
{
	public:
		Command(string input, Server &server);
		void printCmd();
		
		bool	PASS(Client &client);
		bool	NICK(Client &client);
		bool	USER(Client &client);
		
		void	QUIT(Client &client);
		void	PRIVMSG(Client &client);
		void	JOIN(Client &client);
		void	JoinServeur(Client &client, Channel &channel);
		void	TOPIC(Client &client);
		void	KICK(Client &client);
		void	INVITE(Client &client);
		void	PART(Client &client);

		void	MODE(Client &client);
		void 	oMode(Client &client, Channel &channel, string params, char sign);
		void 	iMode(Channel &channel, char sign, Client &client);
		void 	tMode(Channel &channel, char sign, Client &client);
		void 	kMode(Channel &channel, string params, char sign, Client &client);
		void 	lMode(Channel &channel, string params, char sign, Client &client);

		string	getCmdName();
		string	getCmdParam(int index);
		string	getCmdMessage();
		std::vector<string>	splitWithComa(std::string input);

	private:
		Server &_server;
		string cmdName;
		// Client *client;
		std::vector<string> params;
		string msg;
		// Add command-specific methods as needed
};

#endif