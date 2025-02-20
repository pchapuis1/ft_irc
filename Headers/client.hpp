#ifndef CLIENT_HPP
# define CLIENT_HPP

#include "ft_irc.hpp"

using std::string;

class Channel;

class Client
{
	public:
		Client(int socket);
		Client(const Client& rhs);
		~Client();

		Client& operator=(const Client& rhs);

		void	SendMessage(const string& message);
		// Add methods as needed for Client management
		int		getSocket() const;
		string	getNickname() const;
		string	getUsername() const;
		string	getRealname() const;

		void	setSocketState(bool state);
		bool	getSocketState() const;
		void	setUsername(string username);
		void	setNickname(string nickname);
		void	setRealname(string realName);

		void 	setLoginStage(int stage);
		int 	getLoginStage();

	private:
		int			_socket;
		bool		_isOpen;
		string		_nickname;
		string		_username;
		string		_realname;
		int			_loginStage; //Each step increments by one this variable -> 3 means we have all infos

		// Add other Client-related data and methods as needed
};

std::ostream	&operator<<(std::ostream &o, const Client &client);

#endif