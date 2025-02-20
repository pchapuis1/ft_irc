/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   channel.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pchapuis <pchapuis@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: Invalid date        by                   #+#    #+#             */
/*   Updated: 2024/03/21 15:44:21 by pchapuis         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Headers/ft_irc.hpp"

std::ostream& operator<<(std::ostream& os, const Channel& channel)
{
	os << "Channel name: " << channel.getName() << std::endl;
	return os;
}

std::ostream& operator<<(std::ostream& os, std::map<Client *, bool> myClientMap)
{
	for (std::map<Client *, bool>::const_iterator it = myClientMap.begin(); it != myClientMap.end(); ++it) {
		os << "name:" << it->first->getNickname() << endl;
	}
	return os;
}

Channel::Channel(const string& name) : _name(name), _topic(""), _user_limit(-1), _invite_only(false), _has_password(false), _topic_restriction(false)
{
}

Channel::~Channel()
{
}

void Channel::printList(){
	std::map<Client *, bool>::iterator it;

	cout << "\n------------\nclient list\n-------------\n";
	for(it = _client.begin(); it != _client.end(); ++it)
	{
		cout << "client socket: " << it->first->getSocket() << " nickname: " <<  it->first->getNickname();
		if (it->second)
			cout << " is an oprator";
		cout << endl; 
	}
}

void	Channel::AddClientToChannel(Client* client){
	_client.insert(std::pair<Client *, bool>(client, false));
}

void	Channel::RemoveClientFromChannel(Client* client){
	_client.erase(_client.find(client));
}

void	Channel::SendMessageToChannel(string message, string target, Client &from, int restriction)
{	
	for (std::map<Client *, bool>::iterator it = _client.begin(); it != _client.end(); ++it)
	{
		if (it->first->getSocket() == from.getSocket())
			continue;
		if (restriction == 1 && !it->second)
			continue;
		std::string msg = ":" + from.getNickname() + " PRIVMSG " + target + " " + message + "\r\n";
		send_msg(*it->first, msg);
	}
}

bool	Channel::isOperator(int socket)
{
	std::map<Client *, bool>::iterator it;

	for(it = _client.begin(); it != _client.end(); ++it)
	{
		if (it->first->getSocket() == socket)
			return it->second;
	}
	std::cout << "not found" << std::endl;
	return false;
}

std::string	Channel::getName() const
{
	return _name;
}

std::string	Channel::getTopic()
{
	return _topic;
}

std::string	Channel::getTopicAuthor()
{
	return _topic_author;
}

std::string	Channel::getPassword()
{
	return _password;
}

bool	Channel::getInviteStatus()
{
	return _invite_only;
}

std::string	Channel::getSetAt()
{
    std::ostringstream oss;
    oss << _setAt;
    return oss.str();
}

bool	Channel::isInviteOnly()
{
	return _invite_only;
}

bool	Channel::hasPassword()
{
	return _has_password;
}

bool	Channel::hasTopicRestriction(){
	return _topic_restriction;
}

int	Channel::getUserLimit()
{
	return _user_limit;
}

std::map<Client*, bool> 	Channel::getClientMap()
{
	return _client;
}

int	Channel::nbClient()
{
	int	nb = _client.size();

	if (isClientInChannel("bot"))
		nb --;
	return nb;
}

int	Channel::nbOperator()
{
	std::map<Client *, bool>::iterator it;
	int	nb = 0;

	for(it = _client.begin(); it != _client.end(); ++it)
	{
		if (it->second)
			nb ++;
	}
	return nb;
}

std::string Channel::getAllOperator(){
	std::string tmp;
	std::map<Client *, bool>::iterator it;
	
	for(it = _client.begin(); it != _client.end(); ++it)
	{
		if (it->second)
			tmp += " " + it->first->getNickname();
	}
	return tmp;
}

void	Channel::changeOperatorStatusToOff(int socket)
{
	std::map<Client *, bool>::iterator it;

	for(it = _client.begin(); it != _client.end(); ++it)
	{
		if (it->first->getSocket() == socket)
		{
			it->second = false;
			break;
		}
	}
}

void	Channel::changeOperatorStatusToOn(int socket)
{
	std::map<Client *, bool>::iterator it;

	for(it = _client.begin(); it != _client.end(); ++it)
	{
		if (it->first->getSocket() == socket)
		{
			it->second = true;
			break;
		}
	}
}

void	Channel::setNewOperator(){
	std::map<Client *, bool>::iterator it;

	for(it = _client.begin(); it != _client.end(); ++it)
	{
		Client &client = *it->first;
		if (client.getNickname() != "bot"){
			changeOperatorStatusToOn(client.getSocket());
			return;
		}
	}
}

void	Channel::changeUserLimit(int user_limit)
{
	_user_limit = user_limit;
}

void	Channel::changeInviteOnlyStatusToOn()
{
	_invite_only = true;
}

void	Channel::changeInviteOnlyStatusToOff()
{
	_invite_only = false;
}

void	Channel::changePasswordStatusToOn()
{
	_has_password = true;
}

void	Channel::changePasswordStatusToOff()
{
	_has_password = false;
}

void	Channel::changeTopicRestrictionToOn()
{
	_topic_restriction = true;
}

void	Channel::changeTopicRestrictionToOff()
{
	_topic_restriction = false;
}

void	Channel::setTopic(std::string topic){
	_topic = topic;
}

void	Channel::setTopicAuthor(std::string nickname){
	_topic_author = nickname;
}

void	Channel::setSetAt(){
	_setAt = std::time(NULL);
}

void	Channel::setPassword(string pw) {
	_password = pw;
}

void	Channel::setHasPasswordToTrue() {
	_has_password = true;
}

void	Channel::setHasPasswordToFalse() {
	_has_password = false;
}

bool	Channel::isClientInChannel(int socket)
{
	std::map<Client *, bool>::iterator it;

	for(it = _client.begin(); it != _client.end(); ++it)
		if (it->first->getSocket() == socket)
			return true;
	return false;
}

bool	Channel::isClientInChannel(std::string nickname)
{
	std::map<Client *, bool>::iterator it;

	for(it = _client.begin(); it != _client.end(); ++it)
		if (it->first->getNickname() == nickname)
			return true;
	return false;
}

