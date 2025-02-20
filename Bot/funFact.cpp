/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   funFact.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mfinette <mfinette@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/11 21:39:07 by maxime            #+#    #+#             */
/*   Updated: 2024/03/12 10:18:50 by mfinette         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Bot.hpp"

static	int getRandomNumber()
{
	std::srand(static_cast<unsigned int>(std::time(0)));
	return std::rand() % 101;
}

std::string fetchFunFact()
{
	std::ifstream file("funfactsvault.txt");
	std::vector<std::string> funfacts;
	std::string line;

	if (file.is_open())
	{
		while (std::getline(file, line))
			funfacts.push_back(line);
		file.close();
	}

	if (funfacts.empty())
		return "No fun facts found in the funfactsvault.txt file.";
	std::string funfact = funfacts[getRandomNumber()];
	int	index = atoi(funfact.c_str());
	if (index > 9)
		funfact = funfact.substr(4, funfact.length() - 4);
	else
		funfact = funfact.substr(3, funfact.length() - 3);
	return funfact;
}
