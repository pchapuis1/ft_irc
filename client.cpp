/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   client.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pchapuis <pchapuis@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/15 15:06:24 by mfinette          #+#    #+#             */
/*   Updated: 2024/03/12 14:14:30 by pchapuis         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Headers/ft_irc.hpp"

Client::Client(int socket) : _socket(socket), _isOpen(false), _nickname(""), _username(""), _realname(""), _loginStage(0) 
{
}

Client::~Client()
{
}

Client& Client::operator=(const Client& rhs)
{
	if (this != &rhs)
	{
		_socket = rhs._socket;
		_isOpen = rhs._isOpen;
		_realname = rhs._realname;
		_loginStage = rhs._loginStage;
		_nickname = rhs._nickname;
		_username = rhs._username;
	}
	return *this;
}

Client::Client(const Client& rhs) : _socket(rhs._socket), _isOpen(rhs._isOpen), _nickname(rhs._nickname), _username(rhs._username), _realname(rhs._realname), _loginStage(rhs._loginStage){
}

void	Client::SendMessage(const std::string& message) {
	send(_socket, message.c_str(), message.size(), 0);
}

void	Client::setUsername(string username) {
	_username = username;
}

void	Client::setNickname(string nickname) {
	_nickname = nickname;
}

void	Client::setRealname(string realname) {
	_realname = realname;
}

void	Client::setSocketState(bool state) {
	_isOpen = state;
}

bool	Client::getSocketState() const {
	return _isOpen;
}

void	Client::setLoginStage(int stage) {
	_loginStage = stage;
}

int	Client::getLoginStage() {
	return _loginStage;
}

int		Client::getSocket() const{
	return	_socket;
}

string	Client::getNickname() const{
	return _nickname;
}

string	Client::getUsername() const{
	return _username;
}

string	Client::getRealname() const{
	return _realname;
}

std::ostream	&operator<<(std::ostream &o, const Client &client) {
	o << client.getNickname();
	return o;
}