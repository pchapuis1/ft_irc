/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Joke.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mfinette <mfinette@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/08 20:18:37 by mfinette          #+#    #+#             */
/*   Updated: 2024/03/12 11:05:28 by mfinette         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Bot.hpp"

static	int getRandomNumber()
{
	std::srand(static_cast<unsigned int>(std::time(0)));
	return std::rand() % 101;
}

std::string fetchJoke()
{
	std::ifstream file("jokevault.txt");
	std::vector<std::string> jokes;
	std::string line;

	if (file.is_open())
	{
		while (std::getline(file, line))
			jokes.push_back(line);
		file.close();
	}

	if (jokes.empty())
		return "No jokes found in the jokevault.txt file.";
	std::string joke = jokes[getRandomNumber()];
	size_t index = joke.find_first_of("0123456789");
	while (index != std::string::npos)
	{
		joke = joke.substr(0, index) + joke.substr(index + 1);
		index = joke.find_first_of("0123456789");
	}
	joke = joke.substr(2);
	return joke;
}
