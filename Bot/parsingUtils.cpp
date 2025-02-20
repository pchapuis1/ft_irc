/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parsingUtils.cpp                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mfinette <mfinette@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/08 19:53:42 by mfinette          #+#    #+#             */
/*   Updated: 2024/03/21 17:22:41 by mfinette         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Bot.hpp"

std::string	getFirstWord(std::string str)
{
	std::istringstream iss(str);
	std::string word;
	std::getline(iss, word, ' ');
	return word;
}

std::string	getSecondWord(std::string str)
{
	std::istringstream iss(str);
	std::string word;
	std::getline(iss, word, ' ');
	std::getline(iss, word, ' ');
	return word;
}

std::string	getRemaining(std::string str)
{
	std::istringstream iss(str);
	std::string word;
	std::getline(iss, word, ' ');
	std::getline(iss, word, ' ');
	std::getline(iss, word, '\r');
	return word;
}

std::string removeBackslashR(std::string str)
{
	std::string newStr;
	for (size_t i = 0; i < str.length(); i++)
	{
		if (str[i] != '\r')
			newStr += str[i];
	}
	return newStr;
}

std::string removeBackslashRN(std::string str)
{
	std::string newStr = str;
	size_t length = newStr.length();
	while (length > 0 && (newStr[length - 1] == '\r' || newStr[length - 1] == '\n'))
	{
		newStr.erase(length - 1);
		length--;
	}
	return newStr;
}

std::string	getFirstWordAfter2Colons(std::string str)
{
	std::istringstream iss(str);
	std::string word;
	std::getline(iss, word, ':');
	std::getline(iss, word, ':');
	std::getline(iss, word, ' ');
	return word;
}

bool	isLastWord(std::string str, std::string toFind)
{
	std::istringstream iss(str);
	std::string word;
	std::string lastWord;
	while (std::getline(iss, word, ' '))
		lastWord = word;
	if (lastWord == toFind)
		return true;
	return false;
}

void	printCommand(Cmd cmd)
{
	if (cmd._isPRIVMSG)
	{
		std::cout << "PRIVMSG DETECTED: " << std::endl;
		std::cout << "isPRIVMSG = " << cmd._isPRIVMSG << std::endl;
		std::cout << "isForBot = " << cmd._isForBot << std::endl;
		std::cout << "fullCommand = " << cmd._fullCmd;
		std::cout << "Sender = " << cmd._sender << std::endl;
		std::cout << "isChannel = " << cmd._isChannel << std::endl;
		if (cmd._isChannel)
			std::cout << "Channel = " << cmd._channel << std::endl;
		std::cout << "Command type = " << "\"" <<cmd._cmdType << "\"" << std::endl;
		std::cout << "Command remaining = \"" << cmd._cmdRemaining << "\"" << std::endl;
	}
}

std::string	getStringWithoutFirstWord(std::string str)
{
	std::istringstream iss(str);
	std::string word;
	std::getline(iss, word, ' ');
	std::getline(iss, word);
	return word;
}

std::string	getStringWithout2FirstWords(std::string str)
{
	std::istringstream iss(str);
	std::string word;
	std::getline(iss, word, ' ');
	std::getline(iss, word, ' ');
	std::getline(iss, word);
	return word;
}

std::string	removeSingleQuotesFromEntireString(std::string str)
{
	std::string newStr;
	for (size_t i = 0; i < str.length(); i++)
	{
		if (str[i] != '\'')
			newStr += str[i];
	}
	return newStr;
}

std::string	removeDoubleQuotesFromEntireString(std::string str)
{
	std::string newStr;
	for (size_t i = 0; i < str.length(); i++)
	{
		if (str[i] != '\"')
			newStr += str[i];
	}
	return newStr;
}

std::string	getApiKey()
{
	std::string apiKey;
	std::ifstream file(".env");
	if (file.is_open())
	{
		std::getline(file, apiKey);
		file.close();
	}
	return apiKey;
}